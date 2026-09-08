#include "gateway_load_client.hpp"

#include <algorithm>
#include <chrono>
#include <random>
#include <thread>

#include <boost/bind.hpp>
#include <rlog.hpp>

#include "base.hpp"
#include "game.pb.h"
#include "login.pb.h"
#include "net.pb.h"
#include "utility/serialize_msg.h"
#include "game_enum_def_s.h"

namespace gateway_test_client
{
	namespace
	{
		constexpr std::uint32_t max_pending_pings = 1024;
		constexpr std::uint32_t timeout_intervals = 5;
		constexpr std::chrono::seconds retry_interval(5);
		constexpr std::chrono::milliseconds busy_retry_interval(200);
		constexpr std::uint32_t verbose_log_connection_limit = 3;
		constexpr std::uint32_t connection_ramp_batch_size = 16;

		std::mutex g_tcp_client_mutex;

		bool should_log_connection_detail(
			std::uint32_t connection_index,
			std::uint32_t total_connections)
		{
			return total_connections <= 8 || connection_index < verbose_log_connection_limit;
		}

		bool is_same_session(
			const faith::net::tcp_client_session_ptr& lhs,
			const faith::net::tcp_client_session_ptr& rhs)
		{
			return !lhs.owner_before(rhs) && !rhs.owner_before(lhs);
		}
	}

	load_client::load_client(const options& config)
		: m_options(config)
		, m_tcp_client(faith::net::tcp_client::get_instance())
		, m_rng(std::random_device{}())
	{
		if (m_options.hold_max_seconds > 0 &&
			m_options.hold_min_seconds > m_options.hold_max_seconds)
		{
			std::swap(m_options.hold_min_seconds, m_options.hold_max_seconds);
		}
	}

	load_client::~load_client()
	{
		stop();
	}

	bool load_client::start()
	{
		if (m_started || m_options.connections == 0 ||
			m_options.interval_milliseconds == 0 ||
			m_options.scheduler_threads == 0)
		{
			return false;
		}

		faith::net::scheduler::getInstance().set_option(
			faith::net::scheduler::options::thread_num(m_options.scheduler_threads));
		m_connections.reserve(m_options.connections);
		for (std::uint32_t i = 0; i < m_options.connections; ++i)
		{
			const std::uint32_t thread_id = i % m_options.scheduler_threads;
			m_connections.emplace_back(
				std::make_unique<connection_object>(i, thread_id));
		}
		_RLOG_(MINFO, "gateway_test_client created " << m_options.connections
			<< " connection objects on " << m_options.scheduler_threads
			<< " scheduler threads");
		faith::net::scheduler::getInstance().startup(false);
		m_started = true;
		m_stopping.store(false);
		m_next_status_log = clock_type::now() + std::chrono::seconds(5);

		for (std::uint32_t i = 0; i < m_options.connections; ++i)
		{
			const std::uint32_t thread_id =
				m_connections[i]->scheduler_thread_id;
			if (i >= connection_ramp_batch_size)
			{
				break;
			}
			faith::net::scheduler::getInstance().post(
				boost::bind(&load_client::connect_one, this, i),
				thread_id);
		}
		m_next_ramp_connection = std::min(
			m_options.connections, connection_ramp_batch_size);

		m_timer_index = faith::net::scheduler::getInstance().add_timer(
			m_options.interval_milliseconds,
			0,
			boost::bind(&load_client::on_timer, this, _1));
		if (m_timer_index == faith::net::scheduler::scheduler_invalid_timer_index)
		{
			stop();
			return false;
		}
		m_maintenance_timer_index = faith::net::scheduler::getInstance().add_timer(
			1000,
			0,
			boost::bind(&load_client::on_maintenance_timer, this, _1));
		if (m_maintenance_timer_index ==
			faith::net::scheduler::scheduler_invalid_timer_index)
		{
			stop();
			return false;
		}
		return true;
	}

	void load_client::stop()
	{
		if (!m_started)
		{
			return;
		}

		m_stopping.store(true);
		if (m_timer_index != faith::net::scheduler::scheduler_invalid_timer_index)
		{
			faith::net::scheduler::getInstance().remove_timer(m_timer_index);
			m_timer_index = faith::net::scheduler::scheduler_invalid_timer_index;
		}
		if (m_maintenance_timer_index !=
			faith::net::scheduler::scheduler_invalid_timer_index)
		{
			faith::net::scheduler::getInstance().remove_timer(
				m_maintenance_timer_index);
			m_maintenance_timer_index =
				faith::net::scheduler::scheduler_invalid_timer_index;
		}

		std::vector<faith::net::tcp_client_session_ptr> sessions;
		sessions.reserve(m_connections.size());
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			for (const std::unique_ptr<connection_object>& object : m_connections)
			{
				if (object->session)
				{
					sessions.push_back(object->session);
					object->session.reset();
				}
				object->state.connected = false;
				object->state.connecting = false;
				object->state.handshake_received = false;
				object->state.intentional_disconnect = false;
				object->state.disconnecting = false;
				object->state.pending_pings.clear();
				object->state.next_retry = time_point::max();
				object->state.next_disconnect = time_point::max();
			}
		}
		for (const faith::net::tcp_client_session_ptr& session : sessions)
		{
			std::lock_guard<std::mutex> tcp_lock(g_tcp_client_mutex);
			m_tcp_client.disconnect(session);
		}

		faith::net::scheduler::getInstance().request_stop();
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		faith::net::scheduler::getInstance().shutdown();
		m_started = false;
	}

	void load_client::connect_one(std::uint32_t connection_index)
	{
		if (m_stopping.load())
		{
			return;
		}

		{
			std::lock_guard<std::mutex> lock(m_mutex);
			connection_state& connection =
				m_connections[connection_index]->state;
			if (connection.connected || connection.connecting ||
				connection.disconnecting ||
				m_connections[connection_index]->session)
			{
				return;
			}
			connection.connecting = true;
			connection.next_retry = time_point::max();
			++m_statistics.connection_attempts;
		}

		faith::net::tcp_client_session_ptr session;
		{
			std::lock_guard<std::mutex> tcp_lock(g_tcp_client_mutex);
			session = m_tcp_client.connect_to(
				m_options.host,
				std::to_string(m_options.port),
				boost::bind(&load_client::on_connection, this, connection_index, _1, _2, _3),
				boost::bind(&load_client::on_closed, this, connection_index, _1),
				boost::bind(&load_client::on_received, this, connection_index, _1, _2, _3));
		}

		if (!session)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_connections[connection_index]->state.connecting = false;
			// Slot may still be tearing down; retry quickly instead of waiting 5s.
			m_connections[connection_index]->state.next_retry =
				clock_type::now() + busy_retry_interval;
			++m_statistics.connection_failures;
			return;
		}

		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_connections[connection_index]->session = session;
		}
	}

	void load_client::schedule_next_disconnect(
		connection_state& connection,
		time_point now)
	{
		if (m_options.hold_max_seconds == 0)
		{
			connection.next_disconnect = time_point::max();
			return;
		}

		const std::uint32_t hold_seconds = random_hold_seconds();
		connection.next_disconnect = now + std::chrono::seconds(hold_seconds);
	}

	std::uint32_t load_client::random_hold_seconds()
	{
		const std::uint32_t min_seconds = m_options.hold_min_seconds;
		const std::uint32_t max_seconds = m_options.hold_max_seconds;
		if (min_seconds >= max_seconds)
		{
			return max_seconds;
		}
		std::uniform_int_distribution<std::uint32_t> distribution(
			min_seconds, max_seconds);
		return distribution(m_rng);
	}

	void load_client::disconnect_one(std::uint32_t connection_index)
	{
		if (connection_index >= m_connections.size() || m_stopping.load())
		{
			return;
		}

		faith::net::tcp_client_session_ptr session;
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			connection_state& connection =
				m_connections[connection_index]->state;
			if (!connection.connected || connection.disconnecting ||
				connection.intentional_disconnect)
			{
				return;
			}
			connection.intentional_disconnect = true;
			connection.disconnecting = true;
			connection.connected = false;
			connection.pending_pings.clear();
			++connection.session_epoch;
			++m_statistics.intentional_disconnects;
			session = m_connections[connection_index]->session;
		}

		if (should_log_connection_detail(connection_index, m_options.connections))
		{
			_RLOG_(MINFO, "gateway_test_client timed disconnect, connection "
				<< connection_index);
		}
		if (session)
		{
			std::lock_guard<std::mutex> tcp_lock(g_tcp_client_mutex);
			m_tcp_client.disconnect(session);
		}

		std::lock_guard<std::mutex> lock(m_mutex);
		if (!is_same_session(m_connections[connection_index]->session, session))
		{
			return;
		}
		connection_state& connection =
			m_connections[connection_index]->state;
		++m_statistics.connections_closed;
		connection.connecting = false;
		connection.handshake_received = false;
		connection.disconnecting = false;
		connection.pending_pings.clear();
		m_connections[connection_index]->session.reset();
		connection.intentional_disconnect = false;
		connection.next_retry = clock_type::now() +
			std::chrono::milliseconds(
				m_options.reconnect_delay_milliseconds);
		connection.next_disconnect = time_point::max();
	}

	void load_client::on_connection(
		std::uint32_t connection_index,
		faith::net::tcp_client_session_ptr session,
		faith::net::tcp_client::e_connect_info status,
		xstring info)
	{
		handle_connection(connection_index, session, status, info);
	}

	void load_client::handle_connection(
		std::uint32_t connection_index,
		faith::net::tcp_client_session_ptr session,
		faith::net::tcp_client::e_connect_info status,
		xstring info)
	{
		if (connection_index >= m_connections.size() || m_stopping.load())
		{
			return;
		}

		if (status == faith::net::tcp_client::e_ci_connection_successed)
		{
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				if (!is_same_session(
					m_connections[connection_index]->session, session))
				{
					return;
				}
				connection_state& connection =
					m_connections[connection_index]->state;
				connection.connected = true;
				connection.connecting = false;
				connection.disconnecting = false;
				connection.handshake_received = false;
				connection.intentional_disconnect = false;
				connection.pending_pings.clear();
				m_connections[connection_index]->session = session;
				schedule_next_disconnect(connection, clock_type::now());
				++m_statistics.connection_successes;
			}
			if (should_log_connection_detail(connection_index, m_options.connections))
			{
				_RLOG_(MINFO, "gateway_test_client connection " << connection_index
					<< " linked on scheduler thread "
					<< m_connections[connection_index]->scheduler_thread_id);
			}
		}
		else if (status == faith::net::tcp_client::e_ci_connection_failed ||
			status == faith::net::tcp_client::e_ci_addr_resovle_failed ||
			status == faith::net::tcp_client::e_ci_common_error)
		{
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				if (!is_same_session(
					m_connections[connection_index]->session, session))
				{
					return;
				}
				m_connections[connection_index]->state.connected = false;
				m_connections[connection_index]->state.connecting = false;
				m_connections[connection_index]->state.next_retry =
					clock_type::now() + retry_interval;
				++m_connections[connection_index]->state.session_epoch;
				m_connections[connection_index]->session.reset();
				++m_statistics.connection_failures;
			}
			_RLOG_(MERROR, "connection " << connection_index << " failed: "
				<< info << "; retry in 5 seconds");
		}
	}

	void load_client::on_closed(
		std::uint32_t connection_index,
		faith::net::tcp_client_session_ptr session)
	{
		handle_closed(connection_index, session);
	}

	void load_client::handle_closed(
		std::uint32_t connection_index,
		faith::net::tcp_client_session_ptr session)
	{
		if (connection_index >= m_connections.size() || m_stopping.load())
		{
			return;
		}

		std::lock_guard<std::mutex> lock(m_mutex);
		if (!is_same_session(m_connections[connection_index]->session, session))
		{
			return;
		}
		connection_state& connection =
			m_connections[connection_index]->state;
		if (connection.connected)
		{
			connection.connected = false;
			++m_statistics.connections_closed;
		}
		connection.connecting = false;
		connection.handshake_received = false;
		connection.disconnecting = false;
		connection.pending_pings.clear();
		++connection.session_epoch;
		m_connections[connection_index]->session.reset();
		if (connection.intentional_disconnect)
		{
			connection.intentional_disconnect = false;
			connection.next_retry = clock_type::now() +
				std::chrono::milliseconds(
					m_options.reconnect_delay_milliseconds);
		}
		else
		{
			connection.next_retry = clock_type::now() + retry_interval;
		}
		connection.next_disconnect = time_point::max();
	}

	void load_client::on_received(
		std::uint32_t connection_index,
		faith::net::tcp_client_session_ptr session,
		const void* data,
		size_t data_length)
	{
		handle_received(connection_index, session, data, data_length);
	}

	void load_client::handle_received(
		std::uint32_t connection_index,
		faith::net::tcp_client_session_ptr session,
		const void* data_ptr,
		size_t data_length)
	{
		if (connection_index >= m_connections.size() || data_ptr == nullptr ||
			m_stopping.load() || data_length == 0 ||
			data_length < sizeof(faith::packet_base) + sizeof(faith::s_client_uid) +
			sizeof(std::int32_t))
		{
			return;
		}

		{
			std::lock_guard<std::mutex> lock(m_mutex);
			if (!is_same_session(m_connections[connection_index]->session, session))
			{
				return;
			}
		}

		const faith::packet_c2s_s2c* packet =
			static_cast<const faith::packet_c2s_s2c*>(data_ptr);
		if (packet->google_data_len < 0 ||
			packet->google_data_len > MAX_C2S_S2C_PACKAGE_SIZE ||
			sizeof(faith::packet_base) + sizeof(faith::s_client_uid) +
			sizeof(std::int32_t) + static_cast<size_t>(packet->google_data_len) !=
			data_length)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			++m_statistics.malformed_responses;
			return;
		}

		if (packet->wheader == faith::e_msgindex_s2c_rdeencryption)
		{
			bool should_send_login = false;
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				if (!m_connections[connection_index]->state.handshake_received)
				{
					m_connections[connection_index]->state.handshake_received = true;
					++m_statistics.handshakes;
					if (should_log_connection_detail(
						connection_index, m_options.connections))
					{
						_RLOG_(MINFO, "gateway_test_client connection " << connection_index
							<< " handshake received");
					}
				}
				if (m_options.mode == "login" &&
					!m_connections[connection_index]->state.login_sent)
				{
					should_send_login = true;
				}
			}
			if (should_send_login)
			{
				faith::net::scheduler::getInstance().post(
					boost::bind(&load_client::send_login_for_connection, this, connection_index),
					m_connections[connection_index]->scheduler_thread_id);
			}
			return;
		}

		if (packet->wheader == faith::e_msgindex_s2c_client_login ||
			packet->wheader == faith::e_msgindex_s2c_login_queue_status)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			++m_statistics.login_responses;
			_RLOG_(MINFO, "gateway_test_client login response header="
				<< packet->wheader
				<< " connection=" << connection_index
				<< " payload_len=" << packet->google_data_len);
			return;
		}

		if (m_options.mode == "login")
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			++m_statistics.unexpected_messages;
			_RLOG_(MINFO, "gateway_test_client login-mode message header="
				<< packet->wheader << " connection=" << connection_index);
			return;
		}

		if (packet->wheader != faith::e_msgindex_gateway2c_ping)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			++m_statistics.unexpected_messages;
			return;
		}

		faith::game_proto_gateway2client_ping response;
		if (!response.ParseFromArray(packet->google_data, packet->google_data_len))
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			++m_statistics.malformed_responses;
			return;
		}

		const time_point now = clock_type::now();
		std::uint64_t latency_microseconds = 0;
		bool latency_ready = false;
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			++m_statistics.responses;
			if (!m_connections[connection_index]->state.pending_pings.empty())
			{
				const time_point sent =
					m_connections[connection_index]->state.pending_pings.front();
				m_connections[connection_index]->state.pending_pings.pop_front();
				const auto latency =
					std::chrono::duration_cast<std::chrono::microseconds>(
						now - sent).count();
				latency_microseconds = static_cast<std::uint64_t>(
					std::max<std::int64_t>(0, latency));
				latency_ready = true;
			}
		}
		if (latency_ready &&
			should_log_connection_detail(connection_index, m_options.connections))
		{
			record_latency(latency_microseconds);
			_RLOG_(MINFO, "gateway_test_client ping response, connection "
				<< connection_index << ", latency "
				<< (latency_microseconds / 1000.0) << " ms");
		}
		else if (latency_ready)
		{
			record_latency(latency_microseconds);
		}
	}

	void load_client::on_timer(unsigned int)
	{
		if (!m_stopping.load())
		{
			send_pings();
		}
	}

	void load_client::on_maintenance_timer(unsigned int)
	{
		if (m_stopping.load())
		{
			return;
		}

		const time_point now = clock_type::now();
		for (std::uint32_t started = 0;
			m_next_ramp_connection < m_options.connections &&
			started < connection_ramp_batch_size;
			++started, ++m_next_ramp_connection)
		{
			const std::uint32_t connection_index = m_next_ramp_connection;
			faith::net::scheduler::getInstance().post(
				boost::bind(&load_client::connect_one, this, connection_index),
				m_connections[connection_index]->scheduler_thread_id);
		}
		cycle_disconnects(now);
		retry_connections(now);
		if (now < m_next_status_log)
		{
			return;
		}

		std::uint64_t connected_count = 0;
		std::uint64_t handshake_count = 0;
		std::uint64_t request_count = 0;
		std::uint64_t response_count = 0;
		std::uint64_t timed_disconnect_count = 0;
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			for (const std::unique_ptr<connection_object>& object : m_connections)
			{
				if (object->state.connected)
				{
					++connected_count;
				}
				if (object->state.handshake_received)
				{
					++handshake_count;
				}
			}
			request_count = m_statistics.requests;
			response_count = m_statistics.responses;
			timed_disconnect_count = m_statistics.intentional_disconnects;
		}
		_RLOG_(MINFO, "gateway_test_client alive: total connections "
			<< m_options.connections << ", connected " << connected_count
			<< ", handshakes " << handshake_count
			<< ", requests " << request_count
			<< ", responses " << response_count
			<< ", timed disconnects " << timed_disconnect_count);
		m_next_status_log = now + std::chrono::seconds(5);
	}

	void load_client::cycle_disconnects(time_point now)
	{
		if (m_options.hold_max_seconds == 0)
		{
			return;
		}

		for (std::uint32_t i = 0; i < m_options.connections; ++i)
		{
			bool should_disconnect = false;
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				connection_state& connection = m_connections[i]->state;
				if (connection.connected &&
					!connection.intentional_disconnect &&
					now >= connection.next_disconnect)
				{
					should_disconnect = true;
				}
			}

			if (should_disconnect)
			{
				faith::net::scheduler::getInstance().post(
					boost::bind(&load_client::disconnect_one, this, i),
					m_connections[i]->scheduler_thread_id);
			}
		}
	}

	void load_client::retry_connections(time_point now)
	{
		for (std::uint32_t i = 0; i < m_options.connections; ++i)
		{
			bool should_retry = false;
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				connection_state& connection = m_connections[i]->state;
				if (!connection.connected && !connection.connecting &&
					!connection.disconnecting &&
					now >= connection.next_retry)
				{
					// Prevent maintenance timer from posting duplicates.
					connection.next_retry = time_point::max();
					should_retry = true;
				}
			}

			if (should_retry)
			{
				faith::net::scheduler::getInstance().post(
					boost::bind(&load_client::connect_one, this, i),
					m_connections[i]->scheduler_thread_id);
			}
		}
	}

	void load_client::send_pings()
	{
		if (m_options.mode == "login")
		{
			return;
		}
		const time_point now = clock_type::now();
		for (std::uint32_t i = 0; i < m_options.connections; ++i)
		{
			faith::net::scheduler::getInstance().post(
				boost::bind(&load_client::send_ping_for_connection, this, i, now),
				m_connections[i]->scheduler_thread_id);
		}
	}

	void load_client::send_login_for_connection(std::uint32_t connection_index)
	{
		if (m_stopping.load() || connection_index >= m_connections.size())
		{
			return;
		}

		faith::login_proto_login login;
		login.set_logic_account(m_options.account);
		login.set_client_account(m_options.account);
		login.set_password(m_options.password);
		login.set_login_type(faith::e_login_type_new_account_and_password);
		login.set_client_version("1.0.0");
		login.set_server_id(m_options.server_id);
		login.set_device_type("pc");
		login.set_custom_info("gateway_test_client");
		auto* sdk = login.mutable_sdk_data();
		sdk->set_app_key("100001");
		sdk->set_channel_id("0");
		sdk->set_device_id("test_device");

		faith::packet_c2s_s2c packet;
		faith::serialize_msg::get_instance().set_serialize_msg_new(
			packet, &login, faith::e_msgindex_c2s_client_login);

		faith::net::tcp_client_session_ptr session;
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			connection_state& connection = m_connections[connection_index]->state;
			if (!connection.connected || !connection.handshake_received ||
				connection.login_sent || connection.disconnecting ||
				!m_connections[connection_index]->session)
			{
				return;
			}
			connection.login_sent = true;
			session = m_connections[connection_index]->session;
			++m_statistics.login_requests;
		}

		bool send_ok = false;
		{
			std::lock_guard<std::mutex> tcp_lock(g_tcp_client_mutex);
			send_ok = m_tcp_client.send(session, &packet, packet.get_packet_len());
		}

		if (!send_ok)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			++m_statistics.send_failures;
			m_connections[connection_index]->state.login_sent = false;
			_RLOG_(MERROR, "gateway_test_client login send failed, connection "
				<< connection_index);
			return;
		}
		_RLOG_(MINFO, "gateway_test_client login sent account="
			<< m_options.account
			<< " server_id=" << m_options.server_id
			<< " connection=" << connection_index);
	}

	void load_client::send_ping_for_connection(
		std::uint32_t connection_index,
		time_point now)
	{
		if (m_stopping.load())
		{
			return;
		}

		faith::game_proto_client2gateway_ping request;
		request.set_client_time(
			std::chrono::duration_cast<std::chrono::milliseconds>(
				now.time_since_epoch()).count());
		faith::packet_c2s_s2c packet;
		faith::serialize_msg::get_instance().set_serialize_msg_new(
			packet, &request, faith::e_msgindex_c2gateway_ping);

		faith::net::tcp_client_session_ptr session;
		std::uint64_t session_epoch = 0;
		bool should_log = false;
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			connection_state& connection =
				m_connections[connection_index]->state;
			expire_timeouts(connection, now);
			if (!connection.connected || connection.disconnecting ||
				connection.intentional_disconnect ||
				!m_connections[connection_index]->session ||
				connection.pending_pings.size() >= max_pending_pings)
			{
				return;
			}
			connection.pending_pings.push_back(now);
			++m_statistics.requests;
			session_epoch = connection.session_epoch;
			session = m_connections[connection_index]->session;
			should_log = should_log_connection_detail(
				connection_index, m_options.connections);
		}

		bool send_ok = false;
		{
			std::lock_guard<std::mutex> tcp_lock(g_tcp_client_mutex);
			send_ok = m_tcp_client.send(session, &packet, packet.get_packet_len());
		}

		{
			std::lock_guard<std::mutex> lock(m_mutex);
			connection_state& connection =
				m_connections[connection_index]->state;
			if (connection.session_epoch != session_epoch ||
				m_connections[connection_index]->session != session)
			{
				return;
			}
			if (!send_ok)
			{
				++m_statistics.send_failures;
			}
		}
		if (send_ok && should_log)
		{
			_RLOG_(MINFO, "gateway_test_client ping sent, connection "
				<< connection_index);
		}
	}

	void load_client::expire_timeouts(
		connection_state& connection,
		time_point now)
	{
		const auto timeout = std::chrono::milliseconds(
			static_cast<std::int64_t>(m_options.interval_milliseconds) *
			timeout_intervals);
		while (!connection.pending_pings.empty() &&
			now - connection.pending_pings.front() > timeout)
		{
			connection.pending_pings.pop_front();
			++m_statistics.timeouts;
		}
	}

	void load_client::record_latency(std::uint64_t latency_microseconds)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_statistics.latency_microseconds += latency_microseconds;
		if (m_statistics.min_latency_microseconds == 0 ||
			latency_microseconds < m_statistics.min_latency_microseconds)
		{
			m_statistics.min_latency_microseconds = latency_microseconds;
		}
		m_statistics.max_latency_microseconds = std::max(
			m_statistics.max_latency_microseconds, latency_microseconds);
	}

	void load_client::print_summary() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		const double average_ms = m_statistics.responses == 0
			? 0.0
			: static_cast<double>(m_statistics.latency_microseconds) /
			static_cast<double>(m_statistics.responses) / 1000.0;

		_RLOG_(MINFO, "Gateway load test summary");
		_RLOG_(MINFO, "  connections attempted: " << m_statistics.connection_attempts);
		_RLOG_(MINFO, "  connections succeeded: " << m_statistics.connection_successes);
		_RLOG_(MINFO, "  connections failed:    " << m_statistics.connection_failures);
		_RLOG_(MINFO, "  connections closed:    " << m_statistics.connections_closed);
		_RLOG_(MINFO, "  timed disconnects:     " << m_statistics.intentional_disconnects);
		_RLOG_(MINFO, "  RDE handshakes:        " << m_statistics.handshakes);
		_RLOG_(MINFO, "  login requests:        " << m_statistics.login_requests);
		_RLOG_(MINFO, "  login responses:       " << m_statistics.login_responses);
		_RLOG_(MINFO, "  ping requests:         " << m_statistics.requests);
		_RLOG_(MINFO, "  ping responses:        " << m_statistics.responses);
		_RLOG_(MINFO, "  ping timeouts:         " << m_statistics.timeouts);
		_RLOG_(MINFO, "  malformed responses:   " << m_statistics.malformed_responses);
		_RLOG_(MINFO, "  unexpected messages:   " << m_statistics.unexpected_messages);
		_RLOG_(MINFO, "  send failures:         " << m_statistics.send_failures);
		_RLOG_(MINFO, "  latency average:       " << average_ms << " ms");
		_RLOG_(MINFO, "  latency minimum:       "
			<< m_statistics.min_latency_microseconds / 1000.0 << " ms");
		_RLOG_(MINFO, "  latency maximum:       "
			<< m_statistics.max_latency_microseconds / 1000.0 << " ms");
	}
}
