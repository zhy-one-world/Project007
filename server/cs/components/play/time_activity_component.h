#pragma	once

#include "logic/type_def.hpp"
#include "server_log.hpp"

namespace hld
{
	class time_activity_s2s_activity_info;
	class time_activity_s2c_activity_info;
	class player;
	struct TimeActivityTemplate;

	struct s_time_activity_base
	{
		virtual void set_data_info(int32 info_index, int32 info_value) = 0;
		virtual void add_data_info(int32 info_index, int32 info_value) = 0;
		virtual int32 get_data_info(int32 info_index) = 0;
		virtual void clear_data_info() = 0;
	};
	struct s_wheel_plate_info : public s_time_activity_base
	{
		virtual void set_data_info(int32 info_index, int32 info_value)
		{
			if (info_index < 0 || info_index >= e_wheel_plate_info_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return;
			}
			data_ary[info_index] = info_value;
		}
		virtual void add_data_info(int32 info_index, int32 info_value)
		{
			if (info_index < 0 || info_index >= e_wheel_plate_info_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return;
			}
			data_ary[info_index] += info_value;
		}
		virtual int32 get_data_info(int32 info_index)
		{
			if (info_index < 0 || info_index >= e_wheel_plate_info_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return 0;
			}
			return data_ary[info_index];
		}
		virtual void clear_data_info()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}

		int32 data_ary[e_wheel_plate_info_max];
	};
	class c_time_activity
	{
	public:
		c_time_activity();
		~c_time_activity(){}
	public:
		void set_data_info(int32 info_index, int32 value);
		void add_data_info(int32 info_index, int32 value);
		int32 get_data_info(int32 info_index);
		int32 get_activity_id() { return m_activity_id; }
		int64 get_activity_end_time() { return m_activity_end_time; }
		bool is_end_del();
		template<typename T>
		std::shared_ptr<T> get_activity_data() { return std::dynamic_pointer_cast<T>(m_activity_data); }
		std::shared_ptr<s_time_activity_base> create_data(e_time_activity_type map_type);
		void clear_data();
		void set_activity_id(int32 activity_id, int64 end_time);
		void load_data(const time_activity_s2s_activity_info& msg, int64 end_time);
		void generate_db_data(time_activity_s2s_activity_info* msg);
		void generate_client_data(time_activity_s2c_activity_info* msg);
	private:
		int32									m_activity_id;
		int64									m_activity_end_time;
		TimeActivityTemplate*					m_activity_template;
		std::shared_ptr<s_time_activity_base>	m_activity_data;
	};

	struct time_activity_component
	{
		std::unordered_map<int32, std::shared_ptr<c_time_activity>> m_data_map;
	};

	struct system_time_activity_info
	{
		int32 activity_id;
		int32 open_time;
		int32 end_time;
	};
	struct system_time_activity_component
	{
		std::list<std::shared_ptr<system_time_activity_info>> m_begin_activity;
		std::list<std::shared_ptr<system_time_activity_info>> m_run_activity;
	};
}
