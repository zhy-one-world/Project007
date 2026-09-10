#pragma once

#include <atomic>
#include <vector>

namespace faith
{
	// Shared lifecycle base for server-side services.
	// start() recursively starts dependencies first; stop() only stops self.
	class server_service
	{
	public:
		virtual ~server_service() = default;
		virtual const char* service_name() const = 0;

		void depend_on(server_service& dep);
		bool start();
		void stop();
		bool is_started() const { return m_started.load(); }

	protected:
		server_service() = default;

		virtual bool on_start() = 0;
		virtual void on_stop() = 0;

	private:
		std::vector<server_service*> m_deps;
		std::atomic<bool> m_started{ false };
		std::atomic<bool> m_starting{ false };
	};
}
