#include "gateway_load_client.hpp"



#include <atomic>

#include <chrono>

#include <csignal>

#include <cstdlib>

#include <exception>

#include <string>

#include <thread>



#include <rlog.hpp>



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

		_RLOG_(MINFO, "  --host <host>              Gateway host (default: 127.0.0.1)");

		_RLOG_(MINFO, "  --port <port>              Gateway port (default: 2200)");

		_RLOG_(MINFO, "  --connections <count>      concurrent connections (default: 1000)");

		_RLOG_(MINFO, "  --duration <seconds>       test duration, 0 means Ctrl+C (default: 60)");

		_RLOG_(MINFO, "  --interval <milliseconds>  ping interval (default: 1000)");

		_RLOG_(MINFO, "  --hold-min <seconds>       min connected hold before disconnect (default: 10)");

		_RLOG_(MINFO, "  --hold-max <seconds>       max connected hold, 0 disables timed disconnect (default: 30)");

		_RLOG_(MINFO, "  --reconnect-delay <ms>     delay before reconnect after timed disconnect (default: 200)");

		_RLOG_(MINFO, "  --threads <count>          client scheduler threads (default: 4)");

		_RLOG_(MINFO, "  --help                     show this help");

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

			else

			{

				_RLOG_(MERROR, "invalid option or value: " << argument);

				return false;

			}

		}

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



	std::signal(SIGINT, on_signal);

	std::signal(SIGTERM, on_signal);



	gateway_test_client::load_client client(config);

	if (!client.start())

	{

		_RLOG_(MERROR, "failed to start Gateway load client");

		faith::rlog::shutdown();

		return 1;

	}



	if (config.hold_max_seconds == 0)

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


