/********************************************************************
created: 2019年7月25日
file base: oracle_trial_mgr
file ext: cpp
author: 赵玉明

purpose: 天启试炼
*********************************************************************/


#include "connection_mgr.hpp"
#include "logic/drop.h"
#include "logic/item_set.h"
#include "logic/unit_man.h"
#include "lua/script_mgr.h"
#include "oracle_trial_mgr.h"
#include "internal/oracle_trial_msg.hpp"
#include "template/template_manager.h"
#include "utility/parse_msg.h"
#include "internet/cs2dp.pb.h"
#include "internet/game.pb.h"
#include "internet/net.pb.h"
#include "internal/ranking_msg.hpp"

namespace faith
{
	oracle_trial_mgr::oracle_trial_mgr()
	{
		m_array_index = 0;
		clear_data();
	}

	oracle_trial_mgr::~oracle_trial_mgr()
	{

	}

	void oracle_trial_mgr::clear_data()
	{
		m_oracle_trial_info.clear_data();
	}

	//加载试炼信息
	void oracle_trial_mgr::load_oracle_trial_info_from_db(const s_oracle_trial_info_db* oracle_trial_info, int32 data_num)
	{
		m_oracle_trial_info.clear_data();
		for (int32 i = 0; i < data_num; ++i)
		{
			if (oracle_trial_info[i].tier_num - 1 > data_num || oracle_trial_info[i].tier_num - 1 < 0)
			{
				continue;
			}
			m_oracle_trial_info.tier_num[oracle_trial_info[i].tier_num - 1].customs_state = oracle_trial_info[i].customs_state;
			m_oracle_trial_info.tier_num[oracle_trial_info[i].tier_num - 1].reward_stat = oracle_trial_info[i].reward_stat;
		}
		send_player_oracle_trial_info_all();
	}
	bool oracle_trial_mgr::load_oracle_trial_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_oracle_trial_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_oracle_trial_info_db));
		if (p_data == nullptr)
		{
			return false;
		}
		s_oracle_trial_info_db *p_row = (s_oracle_trial_info_db *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_oracle_trial_row db_row = msg.row_data(i);
			p_row->role_guid.server_64 = db_row.role_guid();
			p_row->tier_num = db_row.tier_num();
			p_row->customs_state = db_row.customs_state();
			p_row->reward_stat = db_row.reward_stat();
			p_row++;
		}

		int32 data_num = msg.row_count() > GAMECONFIG->OracleTrialTierNum ? GAMECONFIG->OracleTrialTierNum : msg.row_count();
		load_oracle_trial_info_from_db((const s_oracle_trial_info_db *)p_data, data_num);
		return true;

	}

	//保存试炼信息
	void oracle_trial_mgr::save_oracle_trial_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return ;
		}
		if (m_oracle_trial_info.get_hight_rank() <= 10)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_oracle_trial_info);
		if (!is_use)
		{
			cs2dp_save_oracle_trial_info req;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = player_ref.get_array_index();
			req.save_type = save_type;
			req.data_num = 0;
			int32 _index = m_oracle_trial_info.get_hight_rank() / 10 > max_tier_num ? max_tier_num : m_oracle_trial_info.get_hight_rank() / 10;
			for (int32 i = 0; i < _index; ++i)
			{
				req.info_data[i].role_guid = player_ref.get_unit_guid();
				req.info_data[i].tier_num = i + 1;
				req.info_data[i].customs_state = get_tier_customs_state_info(i + 1);
				req.info_data[i].reward_stat = get_tier_reward_state_info(i + 1);
				++req.data_num;
			}
			player_ref.send_message_to_dp(&req, sizeof(cs2dp_save_oracle_trial_info));
			return;
		}
		faith::cs2dp_proto::save_role_oracle_trial msg;

		msg.set_role_guid(player_ref.get_unit_guid().server_64);
		msg.set_unit_array_index(player_ref.get_array_index());
		msg.set_save_type_ex(save_type);

		faith::cs2dp_proto::role_oracle_trial_db *db_data = msg.mutable_db_data();
		if (db_data == nullptr)
		{
			return;
		}

		int32 _index = m_oracle_trial_info.get_hight_rank() / 10 > max_tier_num ? max_tier_num : m_oracle_trial_info.get_hight_rank() / 10;
		int32 row_count = 0;
		for (int32 i = 0; i < _index; ++i)
		{
			faith::cs2dp_proto::role_oracle_trial_row * db_row = db_data->add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			db_row->set_role_guid(player_ref.get_unit_guid().server_64);
			db_row->set_tier_num(i + 1);
			db_row->set_customs_state(get_tier_customs_state_info(i + 1));
			db_row->set_reward_stat(get_tier_reward_state_info(i + 1));
			++row_count;
		}
		db_data->set_row_count(row_count);
		if (row_count == 0)
		{
			db_data->add_row_data();
		}

		player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_oracle_trial_info);
	}

	//发送玩家的试炼信息到客户端
	void oracle_trial_mgr::send_player_oracle_trial_info_all()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return ;
		}
		game_proto_oracle_trial_info_all msg;
		int32 _index = (m_oracle_trial_info.get_hight_rank() / 10) < GAMECONFIG->OracleTrialTierNum ? (m_oracle_trial_info.get_hight_rank() / 10) : GAMECONFIG->OracleTrialTierNum;
		for (int32 i = 0; i < _index; ++i)
		{
			game_proto_oracle_trial_customs_info* req = msg.add_all_info();
			req->set_customs_state(m_oracle_trial_info.tier_num[i].customs_state);
			req->set_reward_stat(m_oracle_trial_info.tier_num[i].reward_stat);
		}
		
			msg.set_max_commping(GAMECONFIG->OracleTrialCommpingupNum);
			msg.set_max_tier(GAMECONFIG->OracleTrialTierNum);
			msg.set_show_prompt(is_show_prompt());
		
		msg.set_hight_rank(m_oracle_trial_info.get_hight_rank());
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_oracle_trial_info_all);
	}

	//设置玩家的指定层数的试炼信息
	void oracle_trial_mgr::set_play_oracle_trial_info(int32 tier_num, int32 customs_num, int32 customs_state)
	{
		if (tier_num < 1 || tier_num > GAMECONFIG->OracleTrialTierNum || customs_num < 1 || customs_num > max_customs_num || customs_state < m_oracle_trial_info.tier_num[tier_num - 1].customs_state)
		{
			return;
		}
		int32 old_rank = m_oracle_trial_info.get_all_stat_num();
		m_oracle_trial_info.tier_num[tier_num - 1].customs_state = customs_state;
		send_player_oracle_trial_info_all();
		//如果最高纪录提高了 就发送纪录到ws并同步到排行榜上
		if (m_oracle_trial_info.get_all_stat_num() > old_rank)
		{
			player& player_ref = unit_man::get_player(m_array_index);
			if (!player_ref.is_valid())
			{
				return;
			}
			cs2ws_sync_oracle_trial_rank msg;
			msg.role_guid = player_ref.get_unit_guid();
			msg.rank_value = m_oracle_trial_info.get_all_stat_num();
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
		}	
	}

	//设置玩家指定层数的奖励信息
	void oracle_trial_mgr::set_play_oracle_trial_reward_info(int32 tier_num, int32 reward_num, int32 reward_state)
	{
		if (tier_num < 1 || tier_num > GAMECONFIG->OracleTrialTierNum || reward_num < 1 || reward_num > max_reward_num || reward_state < m_oracle_trial_info.tier_num[tier_num - 1].reward_stat)
		{
			return;
		}
		m_oracle_trial_info.tier_num[tier_num - 1].reward_stat = reward_state;
		send_player_oracle_trial_info_all();
	}

	//获得指定层数的试炼信息
	int32 oracle_trial_mgr::get_tier_customs_state_info(int32 tier_num)
	{
		return m_oracle_trial_info.tier_num[tier_num - 1].customs_state;
	}

	//获得指定层数的奖励信息
	int32 oracle_trial_mgr::get_tier_reward_state_info(int32 tier_num)
	{
		return  m_oracle_trial_info.tier_num[tier_num - 1].reward_stat;
	}
	
	//发送玩家扫荡奖励
	void oracle_trial_mgr::send_mopping_up_award()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return ;
		}
		game_proto_oracle_trial_commpingup_end msg;

		int32 moppingup_num = GAMECONFIG->OracleTrialCommpingupNum;
		if (moppingup_num <= 0)
		{
			return ;
		}
		if (player_ref.get_boss_island_mgr().get_boss_island_info(e_island_record_oracle_trial_count) >= moppingup_num)
		{
			msg.set_error_index(e_oracle_trial_error_commping_up_no_have_num);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_oracle_trial_commpingup_end);
			return;
		}
		int32 result = lua_get_comppingup_item_list();
		msg.set_error_index(result);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_oracle_trial_commpingup_end);
	}

	//获得玩家最高通关关卡
	int32 oracle_trial_mgr::get_hight_rank_num()
	{
		return m_oracle_trial_info.get_hight_rank();
	}

	//玩家领取星级奖励的返回结果
	void oracle_trial_mgr::get_oracle_trial_stat_num_raward(int32 tier_num, int32 reward_num)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return ;
		}

		game_proto_send_oracle_trial_customs_award_end msg;
		if (tier_num < 1 || tier_num >  GAMECONFIG->OracleTrialTierNum || reward_num < 1 || reward_num > max_reward_num)
		{
			msg.set_error_index(e_oracle_trial_error_max);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_oracle_trial_customs_award_end);
			return;
		}
		int32 error_index = lua_get_stat_num_award(tier_num, reward_num);
		msg.set_error_index(error_index);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_oracle_trial_customs_award_end);
	}

	void oracle_trial_mgr::send_commingup_item_list(int32 item_id[max_commingup_item_id], int32 item_num[max_commingup_item_id], int32 _item_num)
	{
		if (_item_num <= 0)
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		std::vector<int32> item_arr;
		for (int32 i = 0; i < _item_num; ++i)
		{
			item_arr.push_back(item_id[i]);
			item_arr.push_back(item_num[i]);
		}
		player_ref.get_item_set().put_in_bag(e_server_log_add_item_oracle_trial, 0, item_arr);
		player_ref.get_boss_island_mgr().set_boss_island_info(e_island_record_oracle_trial_count, player_ref.get_boss_island_mgr().get_boss_island_info(e_island_record_oracle_trial_count) + 1);
		player_ref.get_boss_island_mgr().send_player_boss_island_info_one(e_island_record_oracle_trial_count);

		player_ref.set_must_do_count(e_daily_must_do_typ_oracle_trial, player_ref.get_must_do_count(e_daily_must_do_typ_oracle_trial) + 1);
		player_ref.send_daily_must_do_count_info_one(e_daily_must_do_typ_oracle_trial);
		player_ref.set_daily_active_degree_info(e_daily_must_do_typ_oracle_trial);

		send_player_oracle_trial_info_all();
	}

	void oracle_trial_mgr::send_fuben_over_info(int32 map_id, int32 old_statr, int32 new_statr)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}		

		player_ref.set_must_do_count(e_daily_must_do_typ_oracle_trial, player_ref.get_must_do_count(e_daily_must_do_typ_oracle_trial) + 1);
		player_ref.send_daily_must_do_count_info_one(e_daily_must_do_typ_oracle_trial);
		player_ref.set_daily_active_degree_info(e_daily_must_do_typ_oracle_trial);

		game_proto_send_oracle_trial_funben_over_info msg;
		msg.set_map_id(map_id);
		msg.set_old_statr(old_statr);
		msg.set_new_statr(new_statr);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_oracle_trial_funben_over_info);
	}

	oracle_trial_play_info oracle_trial_mgr::get_m_oracle_trial_info()
	{
		return m_oracle_trial_info;
	}

	item_num_info oracle_trial_mgr::get_drop_list_by_id(int32 drop_id)
	{
		item_num_info _item;
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return _item;
		}
		std::vector<s_item_template_info> drop_item_list;
		cdrop::gen_drop_list_by_drop_id(drop_id, drop_item_list, player_ref.get_unit_info(e_role_info_class_type), player_ref.get_unit_info(e_role_info_exp_level));
		for (int32 i = 0; i < drop_item_list.size(); ++i)
		{
			if (i < max_commingup_item_id)
			{
				_item.item_id[i] = drop_item_list[i].m_item_id;
				_item.item_num[i] = drop_item_list[i].m_item_num;
				_item.num++;
			}		
		}
		return _item;
	}

	int32 oracle_trial_mgr::lua_get_comppingup_item_list()
	{
		int32 result = 0;
		//script_mgr::get_instance().call_func("reload", 0);
		script_mgr::get_instance().call_func("oracle_trial_mgr", "oracle_get_comppingup_item_list", 1, false, "%d>%d", m_array_index, &result);
		return result;
	}

	int32 oracle_trial_mgr::lua_get_stat_num_award(int32 tier_num, int32 reward_num)
	{
		int32 result = 0;
		script_mgr::get_instance().call_func("oracle_trial_mgr", "get_stat_num_award", 1, false, "%d%d%d>%d", m_array_index, tier_num, reward_num, &result);
		return result;
	}

	void oracle_trial_mgr::lua_map_game_over(int32 trial_num, int32 customs_num, int32 customs_state)
	{
		script_mgr::get_instance().call_func("oracle_trial_mgr", "map_game_over", 0, false, "%d%d%d%d", m_array_index, trial_num, customs_num, customs_state);
	}

	bool oracle_trial_mgr::is_show_prompt()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
			return false;

		if (false ==  player_ref.get_func_unlock_mgr().is_func_unlock(oracle_trial_unlock_id))
		{
			return false;
		}
			
		if (m_oracle_trial_info.get_hight_rank() / 10  != GAMECONFIG->OracleTrialTierNum)
			return true;
		int32 _index = GAMECONFIG->OracleTrialTierNum;
		for (int32 i = 0; i < _index; ++i)
		{
			if (m_oracle_trial_info.tier_num[i].customs_state != complete_state || m_oracle_trial_info.tier_num[i].reward_stat != complete_reward_state)
				return true;
		}
		if (player_ref.get_boss_island_mgr().get_boss_island_info(e_island_record_oracle_trial_count) < GAMECONFIG->OracleTrialCommpingupNum)
			return true;
		return false;
	}	
}
