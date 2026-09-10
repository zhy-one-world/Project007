#include "service/server_service.hpp"

#include <algorithm>
#include <rlog.hpp>

namespace faith
{
	void server_service::depend_on(server_service& dep)
	{
		if (&dep == this)
		{
			_RLOG_(MERROR, "server_service '" << service_name()
				<< "' cannot depend on itself");
			return;
		}
		if (std::find(m_deps.begin(), m_deps.end(), &dep) != m_deps.end())
		{
			return;
		}
		m_deps.push_back(&dep);
		_RLOG_(MINFO, "server_service '" << service_name()
			<< "' depends on '" << dep.service_name() << "'");
	}

	bool server_service::start()
	{
		if (m_started.load())
		{
			return true;
		}
		if (m_starting.exchange(true))
		{
			_RLOG_(MERROR, "server_service '" << service_name()
				<< "' start dependency cycle detected");
			return false;
		}

		for (server_service* dep : m_deps)
		{
			if (dep == nullptr)
			{
				continue;
			}
			_RLOG_(MINFO, "server_service '" << service_name()
				<< "' starting dependency '" << dep->service_name() << "'");
			if (!dep->start())
			{
				m_starting = false;
				_RLOG_(MERROR, "server_service '" << service_name()
					<< "' aborted: dependency '" << dep->service_name()
					<< "' failed to start");
				return false;
			}
		}

		_RLOG_(MINFO, "server_service '" << service_name() << "' on_start");
		const bool ok = on_start();
		m_starting = false;
		if (!ok)
		{
			_RLOG_(MERROR, "server_service '" << service_name()
				<< "' on_start failed");
			return false;
		}

		m_started = true;
		_RLOG_(MINFO, "server_service '" << service_name() << "' started");
		return true;
	}

	void server_service::stop()
	{
		if (!m_started.exchange(false) && !m_starting.load())
		{
			return;
		}
		_RLOG_(MINFO, "server_service '" << service_name() << "' on_stop");
		on_stop();
		m_starting = false;
		_RLOG_(MINFO, "server_service '" << service_name() << "' stopped");
	}
}
