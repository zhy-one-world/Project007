#include "cc_main_service.hpp"

#include <rlog.hpp>

#include "config_center_service.hpp"

namespace faith
{
	void cc_main_service::configure(const std::string& config_path)
	{
		m_config_path = config_path.empty() ? "./config_center.xml" : config_path;
	}

	bool cc_main_service::on_start()
	{
		_RLOG_(MINFO, "cc_main_service on_start begin, config=" << m_config_path);
		auto& svc = config_center::config_center_service::getInstance();
		if (!svc.init(m_config_path))
		{
			_RLOG_(MERROR, "config_center_service init failed");
			return false;
		}
		if (!svc.start())
		{
			_RLOG_(MERROR, "config_center_service start failed");
			return false;
		}
		_RLOG_(MINFO, "cc_main_service on_start end");
		return true;
	}

	void cc_main_service::on_stop()
	{
		_RLOG_(MINFO, "cc_main_service on_stop begin");
		config_center::config_center_service::getInstance().stop();
		_RLOG_(MINFO, "cc_main_service on_stop end");
	}
}
