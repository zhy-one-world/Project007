#pragma once
namespace faith
{
	class client_session;
	class csv_synchronization_mgr
	{
	public:
		static csv_synchronization_mgr& get_instance()
		{
			static csv_synchronization_mgr instance;
			return instance;
		}
		void clear_data() {}
		void send_template(client_session*) {}
	};
}
