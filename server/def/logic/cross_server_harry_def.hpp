/********************************************************************
  created: 2019年4月4日
  file base: cross_server_harry_def
  file ext: hpp
  author: zhangshuo
  purpose: 
*********************************************************************/
#ifndef _CROSS_SERVER_HARRY_DEF_HPP_
#define _CROSS_SERVER_HARRY_DEF_HPP_

#include "char_def.hpp"
#include "game_enum_def_s.h"

namespace faith 
{
#pragma pack(push,1)

	const int32 special_harry_mark = 10000000;			//特殊采集给serverid增加一个固定值，标记一下
	const int32 special_money_time = 900;				//特殊采集存在时长，sec
	const int32 harry_map_id = 71211107;				//跨服掠夺地图ID
	const int32 harry_map_first_spawn_id = 82072057;	//跨服掠夺地图第一个npc spawnid
	const int32 harry_special_buff = 10992046;			//采集到物品需要加的buff
	const int32 pk_winner_first_spawn_id = 82072060;	//跨服职业竞技第一个npc spawnid
	const int32 dp_save_max_server_num = 100;			//跨服掠夺最大存储服务器数量

	enum e_server_money
	{
		e_server_money_init,//初始值
		e_server_money_minimum,//最低值
		e_server_money_special,//特殊采集次数
		e_server_money_max
	};

	enum e_harry_branch_mission
	{
		e_harry_branch_mission_normal_harry_count,//普通掠夺完成次数
		e_harry_branch_mission_special_harry_count,//特殊掠夺完成次数
		e_harry_branch_mission_kill_ct_count,//击杀保卫者次数
		e_harry_branch_mission_kill_t_count,//击杀掠夺者次数
		e_harry_branch_mission_normal_harry_count_only_money,//普通掠夺完成次数(以下是只获得掠夺币的最大次数)
		e_harry_branch_mission_special_harry_count_only_money,//特殊掠夺完成次数
		e_harry_branch_mission_kill_ct_count_only_money,//击杀保卫者次数
		e_harry_branch_mission_kill_t_count_only_money,//击杀掠夺者次数
		e_harry_branch_mission_max
	};

	enum e_harry_type
	{
		e_harry_type_normal_harry,//普通掠夺
		e_harry_type_special_harry,//特殊掠夺
		e_harry_type_kill_ct,//击杀守卫者
		e_harry_type_kill_t,//击杀掠夺者
		e_harry_type_max
	};

	enum e_fake_type
	{
		e_fake_type_first_harry_player,	//掠夺第一名
		e_fake_type_second_harry_player,
		e_fake_type_third_harry_player,
		e_fake_type_best_barserker,		//最强狂战士
		e_fake_type_best_wizard,		//法师
		e_fake_type_best_guardian,		//骑士
		e_fake_type_best_assassinator,	//暗杀者
		e_fake_type_max
	};

	struct s_server_harry_msg
	{
		int32		server_id;
		int32		server_money;
		int32		special_money;
		guid_64		first_player_guid;
		guid_64		second_player_guid;
		guid_64		third_player_guid;
		xchar		first_player_name[max_name_size + 1];
		s_server_harry_msg()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_server_harry_msg));
		}
		void set_role_name(xstring role_name)
		{
			memset(first_player_name, 0, sizeof(first_player_name));
			memcpy(first_player_name, role_name.c_str(), role_name.size() > sizeof(first_player_name) ? sizeof(first_player_name) : role_name.size());
		}
		bool is_valid()
		{
			return server_id > 0;
		}
		bool operator < (const s_server_harry_msg & other)const
		{
			return server_money > other.server_money;
		}
		void clear_role_name()
		{
			memset(first_player_name, 0, sizeof(first_player_name));
		}
		bool to_proto(faith::st_proto::st_server_harry_msg* st_harry_ptr)
		{
			if (st_harry_ptr == nullptr)
			{
				return false;
			}
			st_harry_ptr->set_server_id(server_id);
			st_harry_ptr->set_server_money(server_money);
			st_harry_ptr->set_special_money(special_money);
			st_harry_ptr->set_first_player_guid(first_player_guid.server_64);
			st_harry_ptr->set_second_player_guid(second_player_guid.server_64);
			st_harry_ptr->set_third_player_guid(third_player_guid.server_64);
			st_harry_ptr->set_first_player_name(first_player_name);
			return true;
		}
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::st_proto::st_server_harry_msg& st_harry_ref)
		{
			server_id = st_harry_ref.server_id();
			server_money = st_harry_ref.server_money();
			special_money = st_harry_ref.special_money();
			first_player_guid.server_64 = st_harry_ref.first_player_guid();
			second_player_guid.server_64 = st_harry_ref.second_player_guid();
			third_player_guid.server_64 = st_harry_ref.third_player_guid();
			my_memcopy_string(first_player_name, max_name_size, st_harry_ref.first_player_name());
		}
	};

	struct s_harry_money_change
	{
		int32		server_id;
		int32		change_money;
		int32		change_special_money;
		s_harry_money_change()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_harry_money_change));
		}
		bool is_valid()
		{
			return server_id > 0;
		}
	};

	struct s_role_harry_info
	{
		int32		data_ary[e_harry_record_max];
		s_role_harry_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_role_harry_info));
		}
	};

	struct s_db_role_harry_info
	{
		guid_64		role_guid;
		int32		data_ary[e_harry_record_max];
		s_db_role_harry_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_db_role_harry_info));
		}
	};
	
	struct s_award_player_harry
	{
		guid_64		role_guid;
		int32		player_rank;
		s_award_player_harry()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_award_player_harry));
		}
	};

#pragma pack(pop)
}
#endif
