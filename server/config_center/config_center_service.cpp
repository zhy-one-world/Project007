#include "config_center_service.hpp"

#include <boost/bind.hpp>
#include <rlog.hpp>

#include "proto_codec.hpp"

namespace faith
{
	namespace config_center
	{
		namespace
		{
			constexpr unsigned int k_max_packet_size = 1024 * 1024;
			constexpr unsigned int k_send_buffer_size = 4 * 1024 * 1024;
			constexpr unsigned int k_recv_buffer_size = 4 * 1024 * 1024;
			constexpr unsigned int k_connections_limit = 256;
		}

		bool config_center_service::init(const std::string& config_path)
		{
			std::string error;
			if (!m_allowlist.load_from_file(config_path, error))
			{
				_RLOG_(MERROR, "load config failed: " << error);
				return false;
			}

			const auto& cfg = m_allowlist.get();
			redis_options redis_opts;
			redis_opts.host = cfg.redis.host;
			redis_opts.port = cfg.redis.port;
			redis_opts.db = cfg.redis.db;
			redis_opts.password = cfg.redis.password;
			m_registry.set_heartbeat_ttl_sec(cfg.heartbeat_ttl_sec);
			if (!m_registry.connect(redis_opts))
			{
				_RLOG_(MERROR, "redis connect failed, host=" << redis_opts.host
					<< " port=" << redis_opts.port);
				return false;
			}

			_RLOG_(MINFO, "config center loaded, listen=" << cfg.listen_host
				<< ":" << cfg.listen_port
				<< " allowlist=" << cfg.allowed.size()
				<< " ttl=" << cfg.heartbeat_ttl_sec);
			return true;
		}

		bool config_center_service::start()
		{
			const auto& cfg = m_allowlist.get();
			m_tcp_server = std::make_unique<net::tcp_server>(
				boost::bind(&config_center_service::on_serverstatus_changed, this, _1),
				boost::bind(&config_center_service::on_conn_created, this, _1),
				boost::bind(&config_center_service::on_conn_closed, this, _1),
				boost::bind(&config_center_service::on_data_received, this, _1, _2, _3),
				cfg.listen_host,
				cfg.listen_port,
				0);

			m_tcp_server->set_option(net::tcp_server::options::max_packet_size(k_max_packet_size));
			m_tcp_server->set_option(net::tcp_server::options::send_buffer_size(k_send_buffer_size));
			m_tcp_server->set_option(net::tcp_server::options::recv_buffer_size(k_recv_buffer_size));
			m_tcp_server->set_option(net::tcp_server::options::connections_num_limit(k_connections_limit));

			if (!m_tcp_server->start())
			{
				_RLOG_(MERROR, "tcp server start failed on "
					<< cfg.listen_host << ":" << cfg.listen_port);
				m_tcp_server.reset();
				return false;
			}

			_RLOG_(MINFO, "config center acceptor started on "
				<< cfg.listen_host << ":" << cfg.listen_port);
			return true;
		}

		void config_center_service::stop()
		{
			std::vector<net::tcp_server_session_ptr> sessions;
			{
				std::lock_guard<std::mutex> lock(m_session_mutex);
				sessions.reserve(m_sessions.size());
				for (auto& item : m_sessions)
				{
					sessions.push_back(item.second);
				}
				m_sessions.clear();
			}
			if (m_tcp_server)
			{
				for (auto& session : sessions)
				{
					if (session)
					{
						m_tcp_server->close(session);
					}
				}
				m_tcp_server.reset();
			}
			_RLOG_(MINFO, "config center stopped");
		}

		void config_center_service::on_serverstatus_changed(net::tcp_server::e_server_status_type status)
		{
			_RLOG_(MINFO, "config center server status=" << static_cast<unsigned int>(status));
		}

		void config_center_service::on_conn_created(net::tcp_server_session_ptr session)
		{
			if (!session || !m_tcp_server)
			{
				return;
			}
			{
				std::lock_guard<std::mutex> lock(m_session_mutex);
				m_sessions[session.get()] = session;
			}
			_RLOG_(MINFO, "client connected, ip="
				<< m_tcp_server->get_ip_addr(session)
				<< " port=" << m_tcp_server->get_ip_port(session));
		}

		void config_center_service::on_conn_closed(net::tcp_server_session_ptr session)
		{
			if (!session)
			{
				return;
			}
			{
				std::lock_guard<std::mutex> lock(m_session_mutex);
				m_sessions.erase(session.get());
			}
			_RLOG_(MINFO, "client disconnected");
		}

		void config_center_service::on_data_received(
			net::tcp_server_session_ptr session,
			const void* data,
			std::size_t data_len)
		{
			if (!session || data == nullptr || data_len == 0)
			{
				return;
			}

			CcMessage request;
			if (!proto_codec::decode(data, data_len, request))
			{
				_RLOG_(MWARN, "invalid config center frame, len=" << data_len);
				if (m_tcp_server)
				{
					m_tcp_server->close(session);
				}
				return;
			}
			handle_message(session, request);
		}

		void config_center_service::fill_peers(
			google::protobuf::RepeatedPtrField<ServerEndpoint>* peers)
		{
			if (peers == nullptr)
			{
				return;
			}
			std::vector<ServerEndpoint> list;
			std::string error;
			if (!m_registry.list_all(list, error))
			{
				_RLOG_(MWARN, "list peers failed: " << error);
				return;
			}
			for (const auto& item : list)
			{
				*peers->Add() = item;
			}
		}

		void config_center_service::handle_message(
			const net::tcp_server_session_ptr& session,
			const CcMessage& request)
		{
			CcMessage response;
			switch (request.body_case())
			{
			case CcMessage::kRegisterReq:
			{
				auto* rep = response.mutable_register_rep();
				const auto& req = request.register_req();
				if (!req.has_endpoint())
				{
					rep->set_ok(false);
					rep->set_error("missing endpoint");
					break;
				}
				const auto& ep = req.endpoint();
				std::string error;
				auto matched = m_allowlist.match(
					ep.server_type(),
					ep.server_index(),
					ep.internal_host(),
					ep.internal_port(),
					ep.external_host(),
					ep.external_port(),
					error);
				if (!matched)
				{
					rep->set_ok(false);
					rep->set_error(error);
					_RLOG_(MWARN, "register rejected: " << error
						<< " type=" << ep.server_type()
						<< " index=" << ep.server_index());
					break;
				}

				ServerEndpoint stored;
				stored.set_server_type(matched->server_type);
				stored.set_server_index(matched->server_index);
				stored.set_internal_host(matched->internal_host);
				stored.set_internal_port(matched->internal_port);
				stored.set_external_host(matched->external_host);
				stored.set_external_port(matched->external_port);

				if (!m_registry.try_register(stored, error))
				{
					rep->set_ok(false);
					rep->set_error(error);
					_RLOG_(MWARN, "register failed: " << error
						<< " type=" << stored.server_type()
						<< " index=" << stored.server_index());
					break;
				}

				rep->set_ok(true);
				fill_peers(rep->mutable_peers());
				_RLOG_(MINFO, "register ok type=" << stored.server_type()
					<< " index=" << stored.server_index()
					<< " peers=" << rep->peers_size());
				break;
			}
			case CcMessage::kHeartbeatReq:
			{
				auto* rep = response.mutable_heartbeat_rep();
				const auto& req = request.heartbeat_req();
				std::string error;
				if (!m_registry.heartbeat(req.server_type(), req.server_index(), error))
				{
					rep->set_ok(false);
					rep->set_error(error);
				}
				else
				{
					rep->set_ok(true);
				}
				break;
			}
			case CcMessage::kUnregisterReq:
			{
				auto* rep = response.mutable_unregister_rep();
				const auto& req = request.unregister_req();
				std::string error;
				if (!m_registry.unregister(req.server_type(), req.server_index(), error))
				{
					rep->set_ok(false);
					rep->set_error(error);
				}
				else
				{
					rep->set_ok(true);
					_RLOG_(MINFO, "unregister ok type=" << req.server_type()
						<< " index=" << req.server_index());
				}
				break;
			}
			case CcMessage::kQueryRegistryReq:
			{
				auto* rep = response.mutable_query_registry_rep();
				std::string error;
				std::vector<ServerEndpoint> list;
				if (!m_registry.list_all(list, error))
				{
					rep->set_ok(false);
					rep->set_error(error);
				}
				else
				{
					rep->set_ok(true);
					for (const auto& item : list)
					{
						*rep->add_peers() = item;
					}
				}
				break;
			}
			default:
			{
				_RLOG_(MWARN, "unsupported config center message body_case="
					<< static_cast<int>(request.body_case()));
				return;
			}
			}

			send_message(session, response);
		}

		bool config_center_service::send_message(
			const net::tcp_server_session_ptr& session,
			const CcMessage& message)
		{
			if (!m_tcp_server || !session)
			{
				return false;
			}
			std::vector<std::uint8_t> buffer;
			if (!proto_codec::encode(message, buffer))
			{
				_RLOG_(MERROR, "encode CcMessage failed");
				return false;
			}
			const int sent = m_tcp_server->send(session, buffer.data(), buffer.size());
			return sent > 0;
		}
	}
}
