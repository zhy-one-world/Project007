#include "gateway_load_client.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <exception>
#include <string>
#include <thread>
#include <vector>

#include <rlog.hpp>

#include "connection/config_center_client.hpp"
#include "http/http_access_mgr.hpp"

namespace
{
	std::atomic<bool> interrupted{ false };

	void on_signal(int)
	{
		interrupted.store(true);
	}

	void print_usage(const char* executable)
	{
		_RLOG_(MINFO, "Usage: " << executable << " [options]");
		_RLOG_(MINFO, "  --host <host>                    Gateway host for load mode (default: 127.0.0.1)");
		_RLOG_(MINFO, "  --port <port>                    Gateway port for load mode (default: 2200)");
		_RLOG_(MINFO, "  --connections <count>            concurrent connections (default: 1000)");
		_RLOG_(MINFO, "  --duration <seconds>             test duration, 0 means Ctrl+C (default: 60)");
		_RLOG_(MINFO, "  --interval <milliseconds>        ping interval (default: 1000)");
		_RLOG_(MINFO, "  --hold-min <seconds>             min connected hold before disconnect (default: 10)");
		_RLOG_(MINFO, "  --hold-max <seconds>             max connected hold, 0 disables timed disconnect (default: 30)");
		_RLOG_(MINFO, "  --reconnect-delay <ms>           delay before reconnect after timed disconnect (default: 200)");
		_RLOG_(MINFO, "  --threads <count>                client scheduler threads (default: 4)");
		_RLOG_(MINFO, "  --mode <load|login>              test mode (default: load)");
		_RLOG_(MINFO, "  --account <name>                 login account (login mode)");
		_RLOG_(MINFO, "  --password <pwd>                 login password (login mode)");
		_RLOG_(MINFO, "  --server-id <id>                 select game_id / server_id (default: 10200)");
		_RLOG_(MINFO, "  --config-center-host <host>      config_center host (login mode, default: 127.0.0.1)");
		_RLOG_(MINFO, "  --config-center-port <port>      config_center port (login mode, default: 19000)");
		_RLOG_(MINFO, "  --config-center-https <0|1>      use https for config_center (default: 1)");
		_RLOG_(MINFO, "  --gateway-index <n>              pick nth matching gateway (default: 0)");
		_RLOG_(MINFO, "  --help                           show this help");
	}

	bool parse_uint(
		const std::string& value,
		std::uint32_t minimum,
		std::uint32_t maximum,
		std::uint32_t& result)
	{
		try
		{
			size_t parsed = 0;
			const unsigned long number = std::stoul(value, &parsed);
			if (parsed != value.size() || number < minimum || number > maximum)
			{
				return false;
			}
			result = static_cast<std::uint32_t>(number);
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	bool parse_arguments(int argc, char** argv, gateway_test_client::options& config)
	{
		for (int i = 1; i < argc; ++i)
		{
			const std::string argument = argv[i];
			if (argument == "--help" || argument == "-h")
			{
				print_usage(argv[0]);
				return false;
			}
			if (i + 1 >= argc)
			{
				_RLOG_(MERROR, "missing value for " << argument);
				return false;
			}

			const std::string value = argv[++i];
			std::uint32_t number = 0;
			if (argument == "--host")
			{
				config.host = value;
			}
			else if (argument == "--port" &&
				parse_uint(value, 1, 65535, number))
			{
				config.port = static_cast<std::uint16_t>(number);
			}
			else if (argument == "--connections" &&
				parse_uint(value, 1, 8192, number))
			{
				config.connections = number;
			}
			else if (argument == "--duration" &&
				parse_uint(value, 0, 86400, number))
			{
				config.duration_seconds = number;
			}
			else if (argument == "--interval" &&
				parse_uint(value, 1, 3600000, number))
			{
				config.interval_milliseconds = number;
			}
			else if (argument == "--hold-min" &&
				parse_uint(value, 0, 86400, number))
			{
				config.hold_min_seconds = number;
			}
			else if (argument == "--hold-max" &&
				parse_uint(value, 0, 86400, number))
			{
				config.hold_max_seconds = number;
			}
			else if (argument == "--reconnect-delay" &&
				parse_uint(value, 0, 3600000, number))
			{
				config.reconnect_delay_milliseconds = number;
			}
			else if (argument == "--threads" &&
				parse_uint(value, 1, 64, number))
			{
				config.scheduler_threads = number;
			}
			else if (argument == "--mode")
			{
				if (value != "load" && value != "login")
				{
					_RLOG_(MERROR, "--mode must be load or login");
					return false;
				}
				config.mode = value;
			}
			else if (argument == "--account")
			{
				config.account = value;
			}
			else if (argument == "--password")
			{
				config.password = value;
			}
			else if (argument == "--server-id" &&
				parse_uint(value, 1, 1000000, number))
			{
				config.server_id = static_cast<std::int32_t>(number);
			}
			else if (argument == "--config-center-host")
			{
				config.config_center_host = value;
			}
			else if (argument == "--config-center-port" &&
				parse_uint(value, 1, 65535, number))
			{
				config.config_center_port = static_cast<std::uint16_t>(number);
			}
			else if (argument == "--config-center-https" &&
				parse_uint(value, 0, 1, number))
			{
				config.config_center_https = (number != 0);
			}
			else if (argument == "--gateway-index" &&
				parse_uint(value, 0, 1024, number))
			{
				config.gateway_index = number;
			}
			else
			{
				_RLOG_(MERROR, "invalid option or value: " << argument);
				return false;
			}
		}

		if (config.mode == "login")
		{
			config.connections = 1;
			config.hold_max_seconds = 0;
			if (config.duration_seconds == 60)
			{
				config.duration_seconds = 30;
			}
		}
		return true;
	}

	bool resolve_gateway_from_config_center(gateway_test_client::options& config)
	{
		faith::net::scheduler::getInstance().set_option(
			faith::net::scheduler::options::thread_num(
				config.scheduler_threads > 0 ? config.scheduler_threads : 2));
		faith::net::scheduler::getInstance().startup(false);
		faith::http_access_mgr::get_instance().init(false);

		std::vector<faith::config_center_client::peer_endpoint> peers;
		std::string error;
		if (!faith::config_center_client::getInstance().query_registry_sync(
				config.config_center_host,
				static_cast<int>(config.config_center_port),
				config.config_center_https,
				peers,
				error))
		{
			_RLOG_(MERROR, "config_center registry query failed: " << error);
			return false;
		}

		std::vector<faith::config_center_client::peer_endpoint> gateways;
		for (const auto& peer : peers)
		{
			_RLOG_(MINFO, "registry gateway game_id=" << peer.game_id
				<< " external=" << peer.external_host << ":" << peer.external_port);
			if (peer.game_id == config.server_id)
			{
				gateways.push_back(peer);
			}
		}

		if (gateways.empty())
		{
			_RLOG_(MERROR, "no gateway found in registry for server_id/game_id="
				<< config.server_id);
			return false;
		}
		if (config.gateway_index >= gateways.size())
		{
			_RLOG_(MERROR, "gateway-index " << config.gateway_index
				<< " out of range, matching gateways=" << gateways.size());
			return false;
		}

		const auto& selected = gateways[config.gateway_index];
		if (selected.external_host.empty() || selected.external_port <= 0)
		{
			_RLOG_(MERROR, "selected gateway missing external endpoint");
			return false;
		}

		config.host = selected.external_host;
		config.port = static_cast<std::uint16_t>(selected.external_port);
		_RLOG_(MINFO, "selected gateway[" << config.gateway_index << "/" << gateways.size()
			<< "] game_id=" << selected.game_id
			<< " endpoint=" << config.host << ":" << config.port);
		return true;
	}
}

int main(int argc, char** argv)
{
	faith::rlog::options rlog_options;
	rlog_options.basename = "gateway_test_client";
	if (!faith::rlog::init(rlog_options))
	{
		return 1;
	}

	gateway_test_client::options config;
	if (!parse_arguments(argc, argv, config))
	{
		faith::rlog::shutdown();
		return argc > 1 && (std::string(argv[1]) == "--help" ||
			std::string(argv[1]) == "-h") ? 0 : 1;
	}

	if (config.mode == "login")
	{
		if (!resolve_gateway_from_config_center(config))
		{
			faith::rlog::shutdown();
			return 1;
		}
	}

	std::signal(SIGINT, on_signal);
	std::signal(SIGTERM, on_signal);

	gateway_test_client::load_client client(config);
	if (!client.start())
	{
		_RLOG_(MERROR, "failed to start Gateway load client");
		faith::rlog::shutdown();
		return 1;
	}

	if (config.mode == "login")
	{
		_RLOG_(MINFO, "Gateway login probe started: " << config.host << ':' << config.port
			<< " account=" << config.account
			<< " server_id=" << config.server_id
			<< " via config_center=" << config.config_center_host
			<< ":" << config.config_center_port);
	}
	else if (config.hold_max_seconds == 0)
	{
		_RLOG_(MINFO, "Gateway load test started: " << config.connections
			<< " connections to " << config.host << ':' << config.port
			<< ", ping every " << config.interval_milliseconds
			<< " ms (timed disconnect disabled)");
	}
	else
	{
		_RLOG_(MINFO, "Gateway load test started: " << config.connections
			<< " connections to " << config.host << ':' << config.port
			<< ", ping every " << config.interval_milliseconds << " ms (hold "
			<< config.hold_min_seconds << '-' << config.hold_max_seconds
			<< "s then disconnect/reconnect, reconnect delay "
			<< config.reconnect_delay_milliseconds << "ms)");
	}

	const auto deadline = std::chrono::steady_clock::now() +
		std::chrono::seconds(config.duration_seconds);
	while (!interrupted.load() &&
		(config.duration_seconds == 0 ||
			std::chrono::steady_clock::now() < deadline))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	client.stop();
	client.print_summary();
	faith::rlog::shutdown();
	return 0;
}
