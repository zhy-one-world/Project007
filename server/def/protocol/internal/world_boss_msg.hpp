/********************************************************************
created: 2016/10/17
file base: world boss
file ext: hpp
author: luoxingyu

purpose:
*********************************************************************/
#ifndef _FAITH_WORLD_BOSS_HPP_
#define _FAITH_WORLD_BOSS_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/world_boss_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/map_def.hpp"
#include "internet/ws2cs.pb.h"
#include "team_def.hpp"

namespace hld
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_ws2dp_save_world_boss = hld::e_msg_base_world_boss,
		e_msgindex_ws2dp_load_world_boss,
		e_msgindex_dp2ws_load_world_boss_end,
		e_msgindex_cs2ws_load_world_boss_by_ws,
		e_msgindex_cs2ws_world_boss_dead,
		e_msgindex_ws2cs_create_world_boss,
		e_msgindex_cs2ws_create_world_boss_success,
		e_msgindex_ws2cs_remove_world_boss,
		e_msgindex_cs2ws_remove_world_boss_success,
		e_msgindex_ws2cs_create_first_kill_prize,
		e_msgindex_cs2ws_has_get_first_kill_prize,
		e_msgindex_cs2ws_npc_left_hp_per,
		e_msgindex_cs2ws_npc_damage_list,

		
		e_msgindex_cs2ws_check_can_assist_fight_end,
		e_msgindex_ws2cs_transfer_assist_fight,

		e_msgindex_cs2ws_cancel_assist_fight,
		e_msgindex_ws2cs_sync_assist_fight_info,
		e_msgindex_cs2ws_start_assist_fight,
		e_msgindex_cs2ws_end_assist_fight,
		e_msgindex_cs2ws_join_team_assist_fight,
	};

	struct ws2dp_save_world_boss_to_db : public hld::packet_base
	{
		int32						data_num;
		s_world_boss_info			data_info[max_world_boss_and_gold_army_num];
		ws2dp_save_world_boss_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_world_boss;
		}

		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_world_boss_info));
		}
	};

	struct ws2dp_load_world_boss : public hld::packet_base
	{
		//这里什么都不用传，其实就是个load使能，load整张表
		ws2dp_load_world_boss()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_world_boss;
		}
	};

	struct dp2ws_load_world_boss_end : public hld::packet_base
	{
		int32						data_num;
		s_world_boss_info			data_info[max_world_boss_and_gold_army_num];
		dp2ws_load_world_boss_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_world_boss_end;
		}

		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_world_boss_info));
		}
	};

	struct cs2ws_world_boss_dead : public packet_base
	{
		int32						world_boss_spawn_template_id;
		xchar						killer_name[max_name_size + 1];
		cs2ws_world_boss_dead()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_world_boss_dead;
		}
	};

	struct ws2cs_create_world_boss : public packet_base
	{
		int32						world_boss_spawn_template_id;
		int32						world_boss_random_pos_arr_id;
		ws2cs_create_world_boss()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_create_world_boss;
		}
		void to_proto(hld::ws2cs_proto::create_world_boss& msg)
		{
			msg.set_world_boss_spawn_template_id(world_boss_spawn_template_id);
			msg.set_world_boss_random_pos_arr_id(world_boss_random_pos_arr_id);
		}
		void from_proto(const hld::ws2cs_proto::create_world_boss& msg)
		{
			world_boss_spawn_template_id = msg.world_boss_spawn_template_id();
			world_boss_random_pos_arr_id = msg.world_boss_random_pos_arr_id();
		}
	};

	struct cs2ws_create_world_boss_success : public packet_base
	{
		int32						world_boss_spawn_template_id;
		int32						world_boss_npc_array_index;
		int32						world_boss_line_id;
		guid_64						world_boss_guid;
		cs2ws_create_world_boss_success()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_create_world_boss_success;
		}
	};

	struct cs2ws_remove_world_boss_success : public packet_base
	{
		int32						world_boss_spawn_template_id;
		cs2ws_remove_world_boss_success()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_remove_world_boss_success;
		}
	};

	struct ws2cs_remove_world_boss : public packet_base
	{
		int32						world_boss_npc_array_index;
		guid_64						world_boss_guid;
		ws2cs_remove_world_boss()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_remove_world_boss;
		}
		void to_proto(hld::ws2cs_proto::remove_world_boss& msg)
		{
			msg.set_world_boss_npc_array_index(world_boss_npc_array_index);
			msg.set_world_boss_guid(world_boss_guid.server_64);
		}
		void from_proto(const hld::ws2cs_proto::remove_world_boss& msg)
		{
			world_boss_npc_array_index = msg.world_boss_npc_array_index();
			world_boss_guid.server_64 = msg.world_boss_guid();
		}
	};


	struct ws2cs_create_first_kill_prize : public packet_base
	{
		guid_64						first_killer_guid;
		int32						first_kill_welfare_template_id;
		ws2cs_create_first_kill_prize()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_create_first_kill_prize;
		}
		void to_proto(hld::ws2cs_proto::create_first_kill_prize& msg)
		{
			msg.set_first_killer_guid(first_killer_guid.server_64);
			msg.set_first_kill_welfare_template_id(first_kill_welfare_template_id);
		}
		void from_proto(const hld::ws2cs_proto::create_first_kill_prize& msg)
		{
			first_killer_guid.server_64 = msg.first_killer_guid();
			first_kill_welfare_template_id = msg.first_kill_welfare_template_id();
		}
	};


	struct cs2ws_has_get_first_kill_prize : public packet_base
	{
		int32						first_kill_welfare_template_id;
		int32						is_get_prize;
		s_item_info					reward_array[max_world_boss_reward_num];
		cs2ws_has_get_first_kill_prize()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_has_get_first_kill_prize;
		}
	};

	struct cs2ws_npc_left_hp_per : public packet_base
	{
		int32						npc_spawn_point_template_id;
		int32						left_hp_per;
		cs2ws_npc_left_hp_per()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_npc_left_hp_per;
		}
	};

	struct cs2ws_npc_damage_list : public packet_base
	{
		bool	is_legion_score;
		int32	record_num;
		score_record top_records[sync_damage_rank_count];
		int32	npc_id;
		int32	boss_type;
		cs2ws_npc_damage_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_npc_damage_list;
		}
	};
	struct cs2ws_check_can_assist_fight_end : public packet_base
	{
		guid_64 role_guid;
		guid_64 assist_fight_guid;
		bool check_result;
		cs2ws_check_can_assist_fight_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_check_can_assist_fight_end;
		}
	};
	struct ws2cs_transfer_assist_fight : public packet_base
	{
		guid_64 role_guid;
		s_assist_fight_info m_assist_fight_info;
		ws2cs_transfer_assist_fight()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_transfer_assist_fight;
		}
	};
	struct cs2ws_cancel_assist_fight : public packet_base
	{
		int32 cancel_type;
		guid_64 role_guid;
		guid_64 assist_fight_guid;
		cs2ws_cancel_assist_fight()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_cancel_assist_fight;
		}
	};
	struct ws2cs_sync_assist_fight_info : public packet_base
	{
		guid_64 role_guid;
		s_assist_fight_info m_assist_fight_info; 
		ws2cs_sync_assist_fight_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_sync_assist_fight_info;
		}
		bool to_proto(hld::ws2cs_proto::sync_assist_fight_info& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			hld::st_proto::st_assist_fight_info*  st_assit_ptr = msg.mutable_m_assist_fight_info();
			if (st_assit_ptr == nullptr)
			{
				return false;
			}

			m_assist_fight_info.to_proto(st_assit_ptr);
			return true;
		}
		void from_proto(const hld::ws2cs_proto::sync_assist_fight_info& msg)
		{
			role_guid.server_64 = msg.role_guid();
			m_assist_fight_info.from_proto(msg.m_assist_fight_info());

		}
	};
	struct cs2ws_start_assist_fight : public packet_base
	{
		s_assist_fight_info m_assist_fight_info;
		cs2ws_start_assist_fight()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_start_assist_fight;
		}
	};
	struct cs2ws_end_assist_fight : public packet_base
	{
		guid_64     asist_fight_guid;
		guid_64     help_list[assist_fight_max_num];
		int32 reward_id_1;
		int32 reward_id_2;
		cs2ws_end_assist_fight()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_end_assist_fight;
		}
	};
	
	struct cs2ws_join_team_assist_fight : public packet_base
	{
		guid_64				team_guid;
		s_team_member_info	member_info;
		cs2ws_join_team_assist_fight()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_join_team_assist_fight;
		}
	};

#pragma pack(pop)
}
#endif