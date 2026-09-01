/********************************************************************
created: 2018年9月6日
file base: boss_island_def
file ext: h
author: zhangshuo

purpose: 永恒岛
*********************************************************************/
#ifndef _BOSS_ISLAND_DEF_HPP_
#define _BOSS_ISLAND_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "team_def.hpp"

namespace faith
{
#pragma pack(push,1)

	struct s_boss_island_info
	{
		int32								data_ary[e_island_record_max];
		s_boss_island_info()
		{
			clear_data();
		}
		void clear_data()
		{
			for (int32 i = 0; i < e_island_record_max; i++)
			{
				data_ary[i] = 0;
			}
		}
	};

	struct s_boss_island_info_db
	{
		guid_64							role_guid;
		s_boss_island_info				data_info;
		s_boss_island_info_db()
		{
			clear_data();
		}
		void clear_data()
		{
			role_guid.clear_data();
			data_info.clear_data();
		}
	};

	struct s_boss_island_player_info
	{
		guid_64											player_guid;
		xchar											player_name[max_name_size + 1];
		s_boss_island_player_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_boss_island_player_info));
		}
		void set_role_name(const xstring& p_role_name)
		{
			memset(player_name, 0, sizeof(player_name));
			memcpy(player_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
	};

#pragma pack(pop)
}

#endif
