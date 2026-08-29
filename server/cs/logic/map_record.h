#pragma once

#include "logic/type_def.hpp"
#include "server_log.hpp"
#include "base/ecs_world.h"

namespace faith
{
	class map_s2s_map_record;
	class map_s2c_map_record;

	struct s_map_record_base
	{
		virtual void set_data_info(int32 info_index, int32 info_value) = 0;
		virtual void add_data_info(int32 info_index, int32 info_value) = 0;
		virtual int32 get_data_info(int32 info_index) = 0;
		virtual void cast_join_count(int32 join_count) = 0;
		virtual int32 get_join_count() = 0;
		virtual void clear_data_info() = 0;
	};
	struct s_box_map_info : public s_map_record_base
	{
		struct npc_point_info
		{
			int32 m_npc_id;
			int32 m_spawn_point_id;
			npc_point_info()
			{
				clear();
			}
			void clear()
			{
				m_npc_id = 0;
				m_spawn_point_id = 0;
			}
		};
		struct flip_box
		{
			std::vector<int32> m_award_list;
			std::set<int32> m_open_list;
			int32 m_flip_box_id;
			flip_box()
			{
				clear();
			}
			void clear()
			{
				m_award_list.clear();
				m_open_list.clear();
				m_flip_box_id = 0;
			}
		};
		virtual void set_data_info(int32 info_index, int32 info_value) 
		{
			if (info_index < 0 || info_index >= e_role_box_map_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return;
			}
			data_ary[info_index] = info_value;
		}
		virtual void add_data_info(int32 info_index, int32 info_value) 
		{
			if (info_index < 0 || info_index >= e_role_box_map_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return;
			}
			data_ary[info_index] += info_value;
		}
		virtual int32 get_data_info(int32 info_index) 
		{
			if (info_index < 0 || info_index >= e_role_box_map_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return 0;
			}
			return data_ary[info_index];
		}
		virtual void clear_data_info() 
		{
			memset(data_ary, 0, sizeof(data_ary));
			m_npc_point.clear();
			m_flip_box.clear();
		}
		virtual void cast_join_count(int32 join_count)
		{
			return;
		}
		virtual int32 get_join_count()
		{
			return 1;
		}
		int32 data_ary[e_role_box_map_max];
		std::list<npc_point_info> m_npc_point;//±¶œ‰À¢–¬¡–±Ì
		flip_box m_flip_box;
	};
	struct s_element_map_info : public s_map_record_base
	{
		virtual void set_data_info(int32 info_index, int32 info_value)
		{
			if (info_index < 0 || info_index >= e_role_element_map_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return;
			}
			data_ary[info_index] = info_value;
		}
		virtual void add_data_info(int32 info_index, int32 info_value)
		{
			if (info_index < 0 || info_index >= e_role_element_map_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return;
			}
			data_ary[info_index] += info_value;
		}
		virtual int32 get_data_info(int32 info_index)
		{
			if (info_index < 0 || info_index >= e_role_element_map_max)
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
		virtual void cast_join_count(int32 join_count)
		{
			return;
		}
		virtual int32 get_join_count()
		{
			return 1;
		}
		int32 data_ary[e_role_element_map_max];
	};

	struct s_battle_map_info : public s_map_record_base
	{
		virtual void set_data_info(int32 info_index, int32 info_value)
		{
			if (info_index < 0 || info_index >= e_role_battle_map_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return;
			}
			data_ary[info_index] = info_value;
		}
		virtual void add_data_info(int32 info_index, int32 info_value)
		{
			if (info_index < 0 || info_index >= e_role_battle_map_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return;
			}
			data_ary[info_index] += info_value;
		}
		virtual int32 get_data_info(int32 info_index)
		{
			if (info_index < 0 || info_index >= e_role_battle_map_max)
			{
				CONSOLE_ERROR("info_index is out of range info_index:{}", info_index);
				return 0;
			}
			return data_ary[info_index];
		}
		virtual void cast_join_count(int32 join_count)
		{
			data_ary[e_role_battle_map_mop_up_count] -= join_count;
		}
		virtual int32 get_join_count()
		{
			//return data_ary[e_role_battle_map_mop_up_count];
			return 1;
		}
		virtual void clear_data_info()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}

		int32 data_ary[e_role_battle_map_max];
	};

	class map_record
	{
	public:
		map_record();
	public:
		void set_map_id(int32 map_id) { m_map_id = map_id; }
		int32 get_map_id() { return m_map_id; }
		void set_data_info(int32 info_index, int32 value);
		void add_data_info(int32 info_index, int32 value);
		int32 get_data_info(int32 info_index);
		void cast_join_count(int32 join_count);
		int32 get_join_count();
		template<typename T>
		std::shared_ptr<T> get_map_record_data() { return std::dynamic_pointer_cast<T>(m_map_record_data); }
		std::shared_ptr<s_map_record_base> create_map_record_data(e_map_type map_type);
		void clear_data_info();
		void load_map_record(const map_s2s_map_record& msg, Entity* map_ent);
		void save_map_record(map_s2s_map_record* msg, Entity* map_ent);
		void send_map_record_msg(map_s2c_map_record* msg);
	private:
		std::shared_ptr<s_map_record_base>	m_map_record_data;
		int32 								m_map_id;	
		e_map_type							m_map_type;
	};
}