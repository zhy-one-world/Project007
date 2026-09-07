#include "config_center_service.hpp"

#include <boost/bind.hpp>
#include <rlog.hpp>

#include <net/http_server.hpp>

#include "http/http_access_mgr.hpp"

namespace faith
{
	namespace config_center
	{
		namespace
		{
			bool parse_json_body(const std::string& text, Json::Value& out, std::string& error)
			{
				Json::CharReaderBuilder builder;
				const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
				if (!reader->parse(text.data(), text.data() + text.size(), &out, &error))
				{
					return false;
				}
				return out.isObject();
			}

			Json::Value endpoint_to_json(const server_endpoint& endpoint)
			{
				Json::Value item;
				item["server_type"] = endpoint.server_type;
				item["game_id"] = endpoint.game_id;
				item["internal_host"] = endpoint.internal_host;
				item["internal_port"] = endpoint.internal_port;
				item["external_host"] = endpoint.external_host;
				item["external_port"] = endpoint.external_port;
				return item;
			}
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
				_RLOG_(MWARN, "redis unavailable, using in-memory registry for local verify, host="
					<< redis_opts.host << " port=" << redis_opts.port);
			}
			else
			{
				_RLOG_(MINFO, "redis connected, host=" << redis_opts.host
					<< " port=" << redis_opts.port);
			}

			http_access_mgr::get_instance().init(false);
			m_http_inited = true;

			_RLOG_(MINFO, "config center loaded, listen=" << cfg.listen_host
				<< ":" << cfg.listen_port
				<< " https=" << (cfg.use_https ? 1 : 0)
				<< " app_key=" << cfg.app_key
				<< " registry=" << (m_registry.using_redis() ? "redis" : "memory")
				<< " allowed_games=" << cfg.allowed_games.size()
				<< " ttl=" << cfg.heartbeat_ttl_sec);
			return true;
		}

		bool config_center_service::start()
		{
			const auto& cfg = m_allowlist.get();
			http_listen_options options;
			options.bind_ip = cfg.listen_host;
			options.port = cfg.listen_port;
			options.scheme = cfg.use_https ? http_scheme::https : http_scheme::http;
			options.ssl.cert_file = cfg.ssl.cert_file;
			options.ssl.key_file = cfg.ssl.key_file;

			if (!http_access_mgr::get_instance().listen(
					options,
					boost::bind(&config_center_service::on_http_request, this, _1)))
			{
				_RLOG_(MERROR, "http(s) listen failed on "
					<< cfg.listen_host << ":" << cfg.listen_port);
				return false;
			}

			_RLOG_(MINFO, "config center acceptor started on "
				<< (cfg.use_https ? "https://" : "http://")
				<< cfg.listen_host << ":" << cfg.listen_port);
			return true;
		}

		void config_center_service::stop()
		{
			http_server::getInstance().stop();
			_RLOG_(MINFO, "config center stopped");
		}

		void config_center_service::on_http_request(const http_inbound_request& request)
		{
			const std::string& path = request.path;
			const bool is_post = request.method == 2; // EVHTTP_REQ_POST
			const bool is_get = request.method == 1;  // EVHTTP_REQ_GET

			if (path == "/v1/register" && is_post)
			{
				Json::Value body;
				std::string error;
				if (!parse_json_body(request.body, body, error))
				{
					Json::Value rep;
					rep["ok"] = false;
					rep["error"] = "invalid json body";
					reply_json(request.handle, 400, rep);
					return;
				}
				handle_register(request.handle, body);
				return;
			}
			if (path == "/v1/heartbeat" && is_post)
			{
				Json::Value body;
				std::string error;
				if (!parse_json_body(request.body, body, error))
				{
					Json::Value rep;
					rep["ok"] = false;
					rep["error"] = "invalid json body";
					reply_json(request.handle, 400, rep);
					return;
				}
				handle_heartbeat(request.handle, body);
				return;
			}
			if (path == "/v1/unregister" && is_post)
			{
				Json::Value body;
				std::string error;
				if (!parse_json_body(request.body, body, error))
				{
					Json::Value rep;
					rep["ok"] = false;
					rep["error"] = "invalid json body";
					reply_json(request.handle, 400, rep);
					return;
				}
				handle_unregister(request.handle, body);
				return;
			}
			if (path == "/v1/registry" && is_get)
			{
				handle_query(request.handle);
				return;
			}

			Json::Value rep;
			rep["ok"] = false;
			rep["error"] = "not found";
			reply_json(request.handle, 404, rep);
		}

		void config_center_service::handle_register(long handle, const Json::Value& body)
		{
			Json::Value rep;
			const std::string app_key = body.get("app_key", "").asString();
			const std::string server_type = body.get("server_type", "").asString();
			const int game_id = body.isMember("game_id")
				? body.get("game_id", -1).asInt()
				: body.get("server_index", -1).asInt();
			const std::string internal_host = body.get("internal_host", "").asString();
			const int internal_port = body.get("internal_port", 0).asInt();
			const std::string external_host = body.get("external_host", "").asString();
			const int external_port = body.get("external_port", 0).asInt();

			if (server_type.empty())
			{
				rep["ok"] = false;
				rep["error"] = "missing server_type";
				reply_json(handle, 400, rep);
				return;
			}
			if (internal_host.empty() || internal_port <= 0)
			{
				rep["ok"] = false;
				rep["error"] = "missing internal endpoint";
				reply_json(handle, 400, rep);
				return;
			}

			std::string error;
			if (!m_allowlist.match(app_key, game_id, error))
			{
				rep["ok"] = false;
				rep["error"] = error;
				_RLOG_(MWARN, "register rejected: " << error
					<< " type=" << server_type
					<< " game_id=" << game_id
					<< " app_key=" << app_key);
				reply_json(handle, 403, rep);
				return;
			}

			server_endpoint stored;
			stored.server_type = server_type;
			stored.game_id = game_id;
			stored.internal_host = internal_host;
			stored.internal_port = internal_port;
			stored.external_host = external_host;
			stored.external_port = external_port;

			if (!m_registry.try_register(stored, error))
			{
				rep["ok"] = false;
				rep["error"] = error;
				_RLOG_(MWARN, "register failed: " << error
					<< " type=" << stored.server_type
					<< " game_id=" << stored.game_id);
				reply_json(handle, 409, rep);
				return;
			}

			std::vector<server_endpoint> peers;
			m_registry.list_by_game_id(game_id, peers, error);
			Json::Value peers_json(Json::arrayValue);
			for (const auto& peer : peers)
			{
				peers_json.append(endpoint_to_json(peer));
			}
			rep["ok"] = true;
			rep["peers"] = peers_json;
			_RLOG_(MINFO, "register ok type=" << stored.server_type
				<< " game_id=" << stored.game_id
				<< " internal=" << stored.internal_host << ":" << stored.internal_port
				<< " external=" << stored.external_host << ":" << stored.external_port
				<< " peers=" << peers.size());
			reply_json(handle, 200, rep);
		}

		void config_center_service::handle_heartbeat(long handle, const Json::Value& body)
		{
			Json::Value rep;
			const std::string server_type = body.get("server_type", "").asString();
			const int game_id = body.isMember("game_id")
				? body.get("game_id", -1).asInt()
				: body.get("server_index", -1).asInt();
			std::string error;
			if (!m_registry.heartbeat(server_type, game_id, error))
			{
				rep["ok"] = false;
				rep["error"] = error;
				reply_json(handle, 404, rep);
				return;
			}
			rep["ok"] = true;
			reply_json(handle, 200, rep);
		}

		void config_center_service::handle_unregister(long handle, const Json::Value& body)
		{
			Json::Value rep;
			const std::string server_type = body.get("server_type", "").asString();
			const int game_id = body.isMember("game_id")
				? body.get("game_id", -1).asInt()
				: body.get("server_index", -1).asInt();
			std::string error;
			if (!m_registry.unregister(server_type, game_id, error))
			{
				rep["ok"] = false;
				rep["error"] = error;
				reply_json(handle, 500, rep);
				return;
			}
			rep["ok"] = true;
			_RLOG_(MINFO, "unregister ok type=" << server_type << " game_id=" << game_id);
			reply_json(handle, 200, rep);
		}

		void config_center_service::handle_query(long handle)
		{
			Json::Value rep;
			std::string error;
			std::vector<server_endpoint> peers;
			if (!m_registry.list_all(peers, error))
			{
				rep["ok"] = false;
				rep["error"] = error;
				reply_json(handle, 500, rep);
				return;
			}
			Json::Value peers_json(Json::arrayValue);
			for (const auto& peer : peers)
			{
				peers_json.append(endpoint_to_json(peer));
			}
			rep["ok"] = true;
			rep["peers"] = peers_json;
			reply_json(handle, 200, rep);
		}

		void config_center_service::reply_json(long handle, int status, const Json::Value& body)
		{
			Json::StreamWriterBuilder writer;
			writer["indentation"] = "";
			http_access_mgr::get_instance().reply(handle, status, Json::writeString(writer, body));
		}
	}
}
