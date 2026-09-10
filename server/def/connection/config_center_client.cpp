#include "connection/config_center_client.hpp"

#include <thread>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <json/json.h>
#include <rlog.hpp>

#include <net/http_types.hpp>

#include "http/http_access_mgr.hpp"

namespace faith
{
	namespace
	{
		bool parse_peers_array(const Json::Value& peers_json, std::vector<config_center_client::peer_endpoint>& out)
		{
			if (!peers_json.isArray())
			{
				return false;
			}
			out.clear();
			for (const auto& item : peers_json)
			{
				config_center_client::peer_endpoint peer;
				peer.server_type = item.get("server_type", "gateway").asString();
				if (peer.server_type.empty())
				{
					peer.server_type = "gateway";
				}
				peer.game_id = item.isMember("game_id")
					? item.get("game_id", 0).asInt()
					: item.get("server_index", 0).asInt();
				peer.internal_host = item.get("internal_host", "").asString();
				peer.internal_port = item.get("internal_port", 0).asInt();
				peer.external_host = item.get("external_host", "").asString();
				peer.external_port = item.get("external_port", 0).asInt();
				out.push_back(peer);
			}
			return true;
		}
	}

	std::string config_center_client::make_base_url() const
	{
		return make_base_url(m_params.center_host, m_params.center_port, m_params.use_https);
	}

	std::string config_center_client::make_base_url(
		const std::string& center_host,
		int center_port,
		bool use_https) const
	{
		const char* scheme = use_https ? "https://" : "http://";
		return std::string(scheme) + center_host + ":"
			+ boost::lexical_cast<std::string>(center_port);
	}

	void config_center_client::poll_until(std::chrono::steady_clock::time_point deadline)
	{
		while (std::chrono::steady_clock::now() < deadline)
		{
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				if (m_wait_done)
				{
					return;
				}
			}
			http_access_mgr::get_instance().tick(0);
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}

	bool config_center_client::request_json(
		const std::string& url,
		int method,
		const std::string& body,
		std::string& response_body,
		std::string& error,
		int timeout_ms)
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_wait_done = false;
			m_wait_ok = false;
			m_wait_error.clear();
			m_wait_body.clear();
		}

		http_request request;
		request.url = url;
		request.method = static_cast<e_http_request_type>(method);
		request.body = body;
		if (!body.empty())
		{
			request.headers.push_back("Content-Type: application/json");
		}
		request.ssl.verify_peer = false;
		request.ssl.verify_host = false;

		const bool queued = http_access_mgr::get_instance().request_async(
			request,
			[this](const http_response& response)
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				m_wait_done = true;
				if (response.error_code != 0)
				{
					m_wait_ok = false;
					m_wait_error = response.error.empty()
						? ("curl error code=" + boost::lexical_cast<std::string>(response.error_code))
						: response.error;
					m_wait_body.clear();
				}
				else
				{
					m_wait_ok = true;
					m_wait_body = response.body;
					if (response.http_status < 200 || response.http_status >= 300)
					{
						m_wait_error = "http status="
							+ boost::lexical_cast<std::string>(response.http_status);
					}
				}
				m_cv.notify_all();
			});

		if (!queued)
		{
			error = "failed to queue http request";
			return false;
		}

		const auto deadline = std::chrono::steady_clock::now()
			+ std::chrono::milliseconds(timeout_ms > 0 ? timeout_ms : 10000);
		poll_until(deadline);

		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_wait_done)
		{
			error = "http request timeout";
			return false;
		}
		if (!m_wait_ok && m_wait_body.empty())
		{
			error = m_wait_error.empty() ? "http request failed" : m_wait_error;
			return false;
		}
		response_body = m_wait_body;
		error = m_wait_error;
		return true;
	}

	bool config_center_client::post_json(
		const std::string& path,
		const std::string& body,
		std::string& response_body,
		std::string& error,
		int timeout_ms)
	{
		return request_json(
			make_base_url() + path,
			static_cast<int>(e_http_request_type_post),
			body,
			response_body,
			error,
			timeout_ms);
	}

	bool config_center_client::query_registry_sync(
		const std::string& center_host,
		int center_port,
		bool use_https,
		std::vector<peer_endpoint>& out,
		std::string& error,
		int timeout_ms)
	{
		out.clear();
		if (center_host.empty() || center_port <= 0)
		{
			error = "invalid config_center endpoint";
			return false;
		}

		std::string response_text;
		std::string req_error;
		if (!request_json(
				make_base_url(center_host, center_port, use_https) + "/v1/registry",
				static_cast<int>(e_http_request_type_get),
				"",
				response_text,
				req_error,
				timeout_ms))
		{
			error = req_error.empty() ? "registry query failed" : req_error;
			return false;
		}

		Json::CharReaderBuilder reader_builder;
		Json::Value root;
		std::string parse_error;
		const std::unique_ptr<Json::CharReader> reader(reader_builder.newCharReader());
		if (!reader->parse(
				response_text.data(),
				response_text.data() + response_text.size(),
				&root,
				&parse_error) ||
			!root.isObject())
		{
			error = "invalid registry response";
			return false;
		}
		if (!root.get("ok", false).asBool())
		{
			error = root.get("error", "registry rejected").asString();
			return false;
		}
		if (!parse_peers_array(root["peers"], out))
		{
			error = "invalid registry peers";
			return false;
		}
		error.clear();
		_RLOG_(MINFO, "config_center registry query ok, gateways=" << out.size());
		return true;
	}

	bool config_center_client::register_sync(const register_params& params, std::string& error)
	{
		if (params.center_host.empty() || params.center_port <= 0 ||
			params.app_key.empty() || params.server_type.empty() ||
			params.game_id < 0 || params.internal_host.empty() ||
			params.internal_port <= 0)
		{
			error = "invalid register params";
			return false;
		}

		m_params = params;
		m_registered = false;
		m_peers.clear();

		Json::Value body;
		body["app_key"] = params.app_key;
		body["server_type"] = params.server_type;
		body["game_id"] = params.game_id;
		body["internal_host"] = params.internal_host;
		body["internal_port"] = params.internal_port;
		body["external_host"] = params.external_host;
		body["external_port"] = params.external_port;
		Json::StreamWriterBuilder writer;
		writer["indentation"] = "";
		const std::string payload = Json::writeString(writer, body);

		std::string response_text;
		std::string req_error;
		if (!post_json("/v1/register", payload, response_text, req_error, params.timeout_ms))
		{
			error = req_error.empty() ? "register request failed" : req_error;
			return false;
		}

		Json::CharReaderBuilder reader_builder;
		Json::Value root;
		std::string parse_error;
		const std::unique_ptr<Json::CharReader> reader(reader_builder.newCharReader());
		if (!reader->parse(
				response_text.data(),
				response_text.data() + response_text.size(),
				&root,
				&parse_error) ||
			!root.isObject())
		{
			error = "invalid register response";
			return false;
		}

		if (!root.get("ok", false).asBool())
		{
			error = root.get("error", "register rejected").asString();
			return false;
		}

		m_peers.clear();
		const Json::Value& peers = root["peers"];
		if (peers.isArray())
		{
			for (const auto& item : peers)
			{
				peer_endpoint peer;
				peer.server_type = item.get("server_type", "").asString();
				peer.game_id = item.isMember("game_id")
					? item.get("game_id", 0).asInt()
					: item.get("server_index", 0).asInt();
				peer.internal_host = item.get("internal_host", "").asString();
				peer.internal_port = item.get("internal_port", 0).asInt();
				peer.external_host = item.get("external_host", "").asString();
				peer.external_port = item.get("external_port", 0).asInt();
				m_peers.push_back(peer);
			}
		}

		m_registered = true;
		error.clear();
		_RLOG_(MINFO, "config_center register ok, peers=" << m_peers.size());
		return true;
	}

	void config_center_client::configure(const register_params& params)
	{
		m_params = params;
	}

	bool config_center_client::ensure_registered()
	{
		std::string error;
		if (!register_sync(m_params, error))
		{
			_RLOG_(MWARN, "config_center_client ensure_registered failed: " << error);
			m_registered = false;
			return false;
		}
		start_heartbeat();
		return true;
	}

	bool config_center_client::on_start()
	{
		m_running = true;
		http_access_mgr::get_instance().init(false);

		if (!ensure_registered())
		{
			_RLOG_(MWARN, "config_center_client register deferred, retry every "
				<< k_retry_interval_ms << "ms");
			schedule_register_retry();
		}
		// Process stays up even if config_center is not ready yet.
		return true;
	}

	void config_center_client::on_stop()
	{
		m_running = false;
		clear_register_retry_timer();
		if (m_heartbeat_timer != net::scheduler::scheduler_invalid_timer_index)
		{
			net::scheduler::getInstance().remove_timer(m_heartbeat_timer);
			m_heartbeat_timer = net::scheduler::scheduler_invalid_timer_index;
		}

		if (m_registered.exchange(false))
		{
			Json::Value body;
			body["server_type"] = m_params.server_type;
			body["game_id"] = m_params.game_id;
			Json::StreamWriterBuilder writer;
			writer["indentation"] = "";
			std::string response;
			std::string error;
			post_json("/v1/unregister", Json::writeString(writer, body), response, error, 3000);
		}
	}

	void config_center_client::clear_register_retry_timer()
	{
		if (m_register_retry_timer != net::scheduler::scheduler_invalid_timer_index)
		{
			net::scheduler::getInstance().remove_timer(m_register_retry_timer);
			m_register_retry_timer = net::scheduler::scheduler_invalid_timer_index;
		}
	}

	void config_center_client::schedule_register_retry()
	{
		if (!m_running.load())
		{
			return;
		}
		if (m_register_retry_timer != net::scheduler::scheduler_invalid_timer_index)
		{
			return;
		}
		m_register_retry_timer = net::scheduler::getInstance().add_timer(
			static_cast<uint32>(k_retry_interval_ms),
			boost::bind(&config_center_client::on_register_retry_timer, this, _1));
	}

	void config_center_client::on_register_retry_timer(uint32)
	{
		if (!m_running.load())
		{
			return;
		}
		if (m_registered.load())
		{
			clear_register_retry_timer();
			return;
		}
		if (ensure_registered())
		{
			clear_register_retry_timer();
		}
	}

	void config_center_client::start_heartbeat()
	{
		if (!m_registered.load())
		{
			return;
		}
		if (m_heartbeat_timer != net::scheduler::scheduler_invalid_timer_index)
		{
			return;
		}
		clear_register_retry_timer();
		const int interval = m_params.heartbeat_interval_ms > 0
			? m_params.heartbeat_interval_ms
			: 10000;
		m_heartbeat_timer = net::scheduler::getInstance().add_timer(
			static_cast<uint32>(interval),
			0,
			boost::bind(&config_center_client::on_heartbeat_timer, this, _1));
		_RLOG_(MINFO, "config_center heartbeat started, interval_ms=" << interval);
	}

	void config_center_client::on_heartbeat_timer(uint32)
	{
		if (!m_running.load())
		{
			return;
		}
		if (!m_registered.load())
		{
			schedule_register_retry();
			return;
		}
		Json::Value body;
		body["server_type"] = m_params.server_type;
		body["game_id"] = m_params.game_id;
		Json::StreamWriterBuilder writer;
		writer["indentation"] = "";

		http_request request;
		request.url = make_base_url() + "/v1/heartbeat";
		request.method = e_http_request_type_post;
		request.body = Json::writeString(writer, body);
		request.headers.push_back("Content-Type: application/json");
		request.ssl.verify_peer = false;
		request.ssl.verify_host = false;
		http_access_mgr::get_instance().request_async(
			request,
			[this](const http_response& response)
			{
				if (response.error_code != 0 ||
					response.http_status < 200 ||
					response.http_status >= 300)
				{
					_RLOG_(MWARN, "config_center heartbeat failed status="
						<< response.http_status
						<< " curl=" << response.error_code
						<< " err=" << response.error
						<< "; will re-register");
					m_registered = false;
					if (m_heartbeat_timer != net::scheduler::scheduler_invalid_timer_index)
					{
						net::scheduler::getInstance().remove_timer(m_heartbeat_timer);
						m_heartbeat_timer = net::scheduler::scheduler_invalid_timer_index;
					}
					schedule_register_retry();
				}
			});
	}
}
