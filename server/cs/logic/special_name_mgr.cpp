/********************************************************************
created: 2016年7月14日13:56:28
file base: Special_name
file ext: cpp
author: zhangminghai

purpose:about role's Special_name_mgr
*********************************************************************/

#include "special_name_mgr.hpp"
#include "special_name_def.hpp"
#include "template/template_manager.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "logic/buff_man.h"
#include "char_def.hpp"
#include "internet/cs2dp.pb.h"
#include "utility/parse_msg.h"
#include "internal/special_name_msg.hpp"
#include "internet/special_name.pb.h"
#include "internet/net.pb.h"

namespace faith
{

	special_name_mgr::special_name_mgr()
	{
		m_array_index = 0;
		clear_data();
	}

	special_name_mgr::~special_name_mgr(void)
	{
		clear_data();
	}
	//清空所有称号
	void special_name_mgr::clear_data()
	{
		m_special_name_ary.clear_data();
	}
	
	int32 special_name_mgr::get_value_by_id(int32 id)
	{
		if (id < 0 || id >= faith::e_special_name_type_max)
		{
			return 0;
		}
		return m_special_name_ary.data_ary[id];
	}

	bool special_name_mgr::set_value_by_id(int32 id, int32 value)
	{
		if (id < 0 || id >= faith::e_special_name_type_max)
		{
			return false;
		}
		m_special_name_ary.data_ary[id] = value;
		return true;
	}

	//读取所有称号
	void special_name_mgr::load_data_from_db(const s_special_name_info& special_name_data)
	{
		m_special_name_ary.clear_data();		
		for (int32 i = 0; i < e_special_name_type_max;i++)
		{
			set_value_by_id(i, special_name_data.data_ary[i]);
		}
		send_special_name_all();
	}

	bool special_name_mgr::load_data_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_special_name_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_special_name_info db_data;

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}


		load_data_from_db(db_data);

		return true;

	}
	
	//存储所有称号
	void special_name_mgr::save_data_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);

		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_special_name);
		if (is_use == false)
		{
			cs2dp_save_char_special_name req;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_array_index;
			req.name_data = m_special_name_ary;
			req.save_type_ex = save_type;
			player_ref.send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			faith::cs2dp_proto::save_role_special_name msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(save_type);

			faith::cs2dp_proto::role_special_name_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			for (int32 i = 0; i < e_special_name_type_max; i++)
			{
				db_data->add_data_ary(m_special_name_ary.data_ary[i]);
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_special_name);
		}
	}
	
	//添加一个称号
	//void special_name_mgr::add_special_name(int32 special_name_id,int32 name_type)
	//{

	//}
	int32   special_name_mgr::get_name_level_by_type(int32 name_type)
	{
		return get_value_by_id(name_type);
	}

	//删除有一个称号
	void special_name_mgr::remove_special_name(int32 name_type,int32 name_level)
	{
		
	}
	
	//升级当前称号
	bool special_name_mgr::upgrade_sepcial_name(int32 name_type,int32 name_level)
	{
		if ((name_type < 0) || (name_type >= e_special_name_type_max) || (0 != name_type % 2))
		{
			return false;
		}	
		if (get_name_level_by_type(name_type) != name_level)
		{
			return false;
		}

		SpecialNameTemplate* special_name_template_ptr = template_manager::get_instance().get_template_by_type_and_level_and_star(name_type,name_level, get_value_by_id(name_type+1));
		if (nullptr == special_name_template_ptr)
		{
			return false;
		}

		if (special_name_template_ptr->UpgradeCost.size() != e_money_tuple_max)
		{
			return false;
		}
		/*该称号是否到达最高级*/
		if (special_name_template_ptr->NextLevelId == 0)
		{
			return false;
		}


		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.can_cut_money((e_money_type)special_name_template_ptr->UpgradeCost[e_money_tuple_id], special_name_template_ptr->UpgradeCost[e_money_tuple_num]) == false)
		{
			return false;
		}

		temp_player.cut_money((e_money_type)special_name_template_ptr->UpgradeCost[e_money_tuple_id], special_name_template_ptr->UpgradeCost[e_money_tuple_num], e_server_log_cut_money_sepcial_name_upgrade, special_name_template_ptr->attribute_id);
		
		//修改称号等级
		SpecialNameTemplate* next_name_ptr = GET_TEMPLATE(SpecialNameTemplate, special_name_template_ptr->NextLevelId);
		if (next_name_ptr == nullptr)
		{
			return false;
		}

		buff_man::del_buff_by_template_id(temp_player.get_array_index(), temp_player.get_array_index(), special_name_template_ptr->BuffLogicId);
		buff_man::add_buff_inst(temp_player.get_array_index(), temp_player.get_array_index(), next_name_ptr->BuffLogicId);

		if (!set_value_by_id(name_type, next_name_ptr->NameLevel))
		{
			return false;
		}
		if (e_special_name_type_military_rank == name_type || e_special_name_type_military_rank_star == name_type)
		{
			temp_player.get_mission_mgr().target_check(e_mission_end_type_arena_military_rank);
			temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_raise_rank);
		}
		if (e_special_name_type_acheivement == name_type || e_special_name_type_acheivement_star == name_type)
		{
			temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_raise_achievement);
		}
		if (e_special_name_type_acheivement == name_type)
		{
			if (!set_value_by_id(name_type + 1, 0))
			{
				return false;
			}
			return true;
		}
		else if (e_special_name_type_military_rank == name_type)
		{
			if (!set_value_by_id(name_type + 1, 1))
			{
				return false;
			}
			return true;
		}

		return false;
	}

	bool special_name_mgr::promote_special_name(int32 name_type)
	{
		if ((name_type < 0) || (name_type >= faith::e_special_name_type_max))
		{
			return false;
		}

		if (max_num_of_star <= get_value_by_id(name_type + 1)) {
			return false;
		}
		SpecialNameTemplate* special_name_template_ptr = template_manager::get_instance().get_template_by_type_and_level_and_star(name_type,get_value_by_id(name_type), get_value_by_id(name_type + 1));
		if (nullptr == special_name_template_ptr)
		{
			return false;
		}
		if (e_money_tuple_max != special_name_template_ptr->UpgradeCost.size())
		{
			return false;
		}
		player& temp_player = unit_man::get_player(m_array_index);

		if (temp_player.can_cut_money((e_money_type)special_name_template_ptr->UpgradeCost[e_money_tuple_id], special_name_template_ptr->UpgradeCost[e_money_tuple_num]) == false)
		{
			return false;
		}
		temp_player.cut_money((e_money_type)special_name_template_ptr->UpgradeCost[e_money_tuple_id], special_name_template_ptr->UpgradeCost[e_money_tuple_num], e_server_log_cut_money_sepcial_name_upgrade, special_name_template_ptr->attribute_id);
		SpecialNameTemplate* next_name_ptr = GET_TEMPLATE(SpecialNameTemplate, special_name_template_ptr->NextLevelId);
		if (next_name_ptr == nullptr)
		{
			return false;
		}
		buff_man::del_buff_by_template_id(temp_player.get_array_index(), temp_player.get_array_index(), special_name_template_ptr->BuffLogicId);
		buff_man::add_buff_inst(temp_player.get_array_index(), temp_player.get_array_index(), next_name_ptr->BuffLogicId);
		if (!set_value_by_id(name_type + 1, next_name_ptr->NameStar))
		{
			return false;
		}
		return true;
	}


	//暂时只有获取自己的称号
	void  special_name_mgr::send_special_name_all()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		special_name_proto_special_name_person_names response;
		response.set_role_guid(temp_player.get_unit_guid().server_64);

		for (int32 i = 0; i < e_special_name_type_max; i = i + 2)
		{
			response.add_special_name_level(get_value_by_id(i));
			response.add_special_name_star(get_value_by_id(i + 1));
		}
		temp_player.send_message_to_self(&response, e_msgindex_s2c_special_name_get_all);
	}
	void special_name_mgr::send_special_name(unit& temp_player)
	{
		special_name_proto_special_name_person_names response;
		player& m_player = unit_man::get_player(m_array_index);
		response.set_role_guid(m_player.get_unit_guid().server_64);
		for (int32 i = 0; i < e_special_name_type_max; i = i + 2)
		{
			response.add_special_name_level(get_value_by_id(i));
			response.add_special_name_star(get_value_by_id(i + 1));
		}
		temp_player.send_message_to_self(&response, e_msgindex_s2c_special_name_get_all);
	}

	void special_name_mgr::sync_ranking_special_name(int32 name_type)
	{
		player& temp_player = unit_man::get_player(m_array_index);

		if (temp_player.is_valid() == false)
		{
			return;
		}

		int32 ranking_type = -1;
		if (e_special_name_type_military_rank == name_type)
		{
			ranking_type = e_RankingIndex_military_rank;
		}
		else if (e_special_name_type_acheivement == name_type)
		{
			ranking_type = e_RankingIndex_achievement;
		}
		if (-1 != ranking_type)
		{
			SpecialNameTemplate* special_name_template_ptr = template_manager::get_instance().get_template_by_type_and_level_and_star(name_type, get_value_by_id(name_type), get_value_by_id(name_type + 1));
			if (nullptr != special_name_template_ptr)
			{
				if (military_rank_start_id == special_name_template_ptr->attribute_id || acheivement_start_id == special_name_template_ptr->attribute_id)
				{
					return;
				}
				temp_player.get_ranking_mgr().set_and_sync_ranking_data((e_RankingIndex)ranking_type, special_name_template_ptr->attribute_id);
			}
		}
	}

	void special_name_mgr::sync_military_rank_to_ws()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		int32 military_rank_value = get_name_level_by_type(e_special_name_type_military_rank);
		temp_player.sync_data_to_ws(e_sync_cs2ws_military_rank, military_rank_value, temp_player.get_arena_mgr().current_rank());
	}

	int32 special_name_mgr::get_achievement_title_level()
	{
		return m_special_name_ary.data_ary[e_special_name_type_acheivement];
	}

	int32 special_name_mgr::get_solider_rank_level()
	{
		return m_special_name_ary.data_ary[e_special_name_type_military_rank];
	}

	int32 special_name_mgr::get_solider_rank_star()
	{
		return m_special_name_ary.data_ary[e_special_name_type_military_rank_star];
	}

	void special_name_mgr::send_special_name_operate_end_msg(bool success,int32 name_type,int32 operate_type)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		special_name_proto_special_name_operate_end response;
		response.set_role_guid(temp_player.get_unit_guid().server_64);
		response.set_operation_end_state((int32)success);
		response.set_special_name_type(name_type);
		response.set_special_name_level(get_value_by_id(name_type));
		response.set_operation_type(operate_type);
		response.set_special_name_star(get_value_by_id(name_type + 1));
		temp_player.send_message_to_aoi(&response, e_msgindex_s2c_special_name_operate);
	}
	void special_name_mgr::send_special_name_upgrade_notice(const int32& name_type)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		SpecialNameTemplate* special_name_template_ptr = template_manager::get_instance().get_template_by_type_and_level_and_star(name_type, get_value_by_id(name_type), get_value_by_id(name_type + 1));
		if (special_name_template_ptr == nullptr)
		{
			return;
		}

		if (special_name_template_ptr->NoticeId > 0)
		{
			s_chat_notice_info notice_data;
			const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(special_name_template_ptr->NameString);
			notice_data.item_name.push_back(temp_string);
			temp_player.get_chat_mgr().send_notice_new(special_name_template_ptr->NoticeId, notice_data);
		}
	}

	void special_name_mgr::init_client_special_name()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		SpecialNameTemplate* special_name_template_ptr = nullptr;
		int32 buff_template_id = 0;
		for (int32 i = 0; i < e_special_name_type_max; i = i + 2)
		{
			special_name_template_ptr = template_manager::get_instance().get_template_by_type_and_level_and_star(i, get_value_by_id(i), get_value_by_id(i + 1));
			if (special_name_template_ptr != nullptr)
			{
				buff_template_id = special_name_template_ptr->BuffLogicId;
				buff_man::add_buff_inst(temp_player.get_array_index(), temp_player.get_array_index(), buff_template_id);
			}
		}
	}
}