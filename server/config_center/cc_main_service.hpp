#pragma once

#include <string>

#include <singleton.hpp>
#include "service/server_service.hpp"

namespace faith
{
	// Process-level orchestrator for config center startup / shutdown.
	class cc_main_service
		: public server_service
		, public singleton<cc_main_service>
	{
		friend class singleton<cc_main_service>;

	public:
		void configure(const std::string& config_path = "./config_center.xml");
		const char* service_name() const override { return "cc_main_service"; }

	protected:
		bool on_start() override;
		void on_stop() override;

	private:
		cc_main_service() = default;

		std::string m_config_path = "./config_center.xml";
	};
}
