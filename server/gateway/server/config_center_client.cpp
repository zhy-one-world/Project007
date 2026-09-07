#include "config_center_client.hpp"

#include <thread>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <json/json.h>
#include <rlog.hpp>

#include "http/http_access_mgr.hpp"

namespace faith
{
	std::string config_center_client::make_base_url() const
	{
		const char* scheme = m_params.use_https ? "https://" : "http://";
		return std::string(scheme) + m_params.center_host + ":"
			+ boost::lexical_cast<std::string>(m_params.center_port);
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

	bool config_center_client::post_json(
		const std::string& path,
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
		request.url = make_base_url() + path;
		request.method = e_http_request_type_post;
		request.body = body;
		request.headers.push_back("Content-Type: application/json");
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

	bool config_center_client::register_sync(const register_params& params, std::string& error)
	{
		if (params.center_host.empty() || params.center_port <= 0 ||
			params.server_type.empty() || params.internal_host.empty() ||
			params.internal_port <= 0)
		{
			error = "invalid register params";
			return false;
		}

		m_params = params;
		m_registered = false;
		m_peers.clear();

		Json::Value body;
		body["server_type"] = params.server_type;
		body["server_index"] = params.server_index;
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
				peer.server_index = item.get("server_index", 0).asInt();
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
		const int interval = m_params.heartbeat_interval_ms > 0
			? m_params.heartbeat_interval_ms
			: 10000;
		m_heartbeat_timer = net::scheduler::getInstance().add_timer(
			static_cast<uint32>(interval),
			0,
			boost::bind(&config_center_client::on_heartbeat_timer, this, _1));
		_RLOG_(MINFO, "config_center heartbeat started, interval_ms=" << interval);
	}

	void config_center_client::stop()
	{
		if (m_heartbeat_timer != net::scheduler::scheduler_invalid_timer_index)
		{
			net::scheduler::getInstance().remove_timer(m_heartbeat_timer);
			m_heartbeat_timer = net::scheduler::scheduler_invalid_timer_index;
		}

		if (m_registered.exchange(false))
		{
			Json::Value body;
			body["server_type"] = m_params.server_type;
			body["server_index"] = m_params.server_index;
			Json::StreamWriterBuilder writer;
			writer["indentation"] = "";
			std::string response;
			std::string error;
			post_json("/v1/unregister", Json::writeString(writer, body), response, error, 3000);
		}
	}

	void config_center_client::on_heartbeat_timer(uint32)
	{
		if (!m_registered.load())
		{
			return;
		}
		Json::Value body;
		body["server_type"] = m_params.server_type;
		body["server_index"] = m_params.server_index;
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
			[](const http_response& response)
			{
				if (response.error_code != 0 ||
					response.http_status < 200 ||
					response.http_status >= 300)
				{
					_RLOG_(MWARN, "config_center heartbeat failed status="
						<< response.http_status
						<< " curl=" << response.error_code
						<< " err=" << response.error);
				}
			});
	}
}
