#include "marry_mgr.h"
#include "logic/unit_man.h"
#include "logic/player.hpp"
#include "lua/script_mgr.h"
#include "invalid_word/invalid_ansi_word.h"
#include "internet/net.pb.h"
#include "internet/relation.pb.h"
#include "internal/marry_msg.hpp"
#include "connection_mgr.hpp"
#include "utility/init_unit.h"

namespace faith
{
	marry_mgr::marry_mgr()
	{
		m_array_index = 0;
	}
	marry_mgr::~marry_mgr()
	{
	}

	void marry_mgr::activate_wedding_ring(int32 marry_item_id)
	{
		//script_mgr::get_instance().call_func("reload", 0);
		script_mgr::get_instance().call_func(nullptr, "activate_wedding_ring", 0, false, "%d%d", m_array_index, marry_item_id);
	}

	void marry_mgr::close_wedding_ring()
	{
		//script_mgr::get_instance().call_func("reload", 0);
		script_mgr::get_instance().call_func(nullptr, "close_wedding_ring", 0, false, "%d", m_array_index);
	}

	void marry_mgr::refresh_wedding_ring_info(bool is_marry, int32 marry_item_id, int32 mate_ring_level, int32 heart_value)
	{
		//script_mgr::get_instance().call_func(nullptr, "refresh_wedding_ring_info", 0, false, "%d%d%d%d%d", m_array_index, is_marry ? 1 : 0, marry_item_id, mate_ring_level, heart_value);
	}

	void marry_mgr::refresh_mate_wedding_ring_level(int32 mate_ring_level)
	{
		script_mgr::get_instance().call_func(nullptr, "refresh_mate_wedding_ring_level", 0, false, "%d%d", m_array_index, mate_ring_level);
	}

	void marry_mgr::refresh_heart_value(int32 heart_value)
	{
		//script_mgr::get_instance().call_func("reload", 0);
		script_mgr::get_instance().call_func(nullptr, "refresh_heart_value", 0, false, "%d%d", m_array_index, heart_value);
		sync_heart_value();


	}

	int32 marry_mgr::get_ring_level()
	{
		int32 result = 0;
		script_mgr::get_instance().call_func(nullptr, "get_ring_level", 1, false, "%d>%d", m_array_index, &result);
		return result;
	}

	void marry_mgr::wedding_ring_upgrade(int32 use_up_item)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() && player_ref.is_self_server())
		{
			//script_mgr::get_instance().call_func("reload", 0);
			int32 result = 0;
			script_mgr::get_instance().call_func(nullptr, "wedding_ring_upgrade", 1, false, "%d%d>%d", m_array_index, use_up_item, &result);
			relation_proto_s2c_upgrade_wedding_ring_result send;
			send.set_result(result);
			player_ref.send_message_to_self(&send, e_msgindex_s2c_upgrade_wedding_ring_result);
		}
	}
	void marry_mgr::reset_marry_dati()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		player_ref.set_unit_info(e_role_info_marry_dati_id, 0);
		player_ref.set_unit_info(e_role_info_marry_dati_num, 0);
		player_ref.set_unit_info(e_role_info_marry_dati_right_num, 0);
		player_ref.set_unit_info(e_role_info_marry_dati_current_answer, 0);
	}

	void marry_mgr::begin_marry_dati(bool is_near)
	{
		//script_mgr::get_instance().call_func("reload_module", 0, "%s", "marry_mgr");
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() && player_ref.is_self_server())
		{
			if (player_ref.had_couple() == false)
			{
				player_ref.send_notice("90096775");
				return;
			}	
			int32 result = 0;
			//判断队伍
			if (!player_ref.is_player_in_team())
			{
				player_ref.send_notice("90096775");
				return;
			}
			if (player_ref.get_team_info().captain_guid != player_ref.get_unit_guid())
			{
				player_ref.send_notice("90096775");
				return;
			}
			if (player_ref.get_team_info().member_num != 2)
			{
				player_ref.send_notice("90096775");
				return;
			}
			player& couple_ref = unit_man::get_player(player_ref.get_couple_guid());

			if (!couple_ref.is_valid() || !couple_ref.is_self_server())
			{
				player_ref.send_notice("90096777");
				//对方不在线
				return;
			}
			if (is_near == 0)
			{
				player_ref.send_notice("90096776");
				couple_ref.send_notice("90096776");
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "begin_marry_dati", 1, false, "%d%d>%d", m_array_index, couple_ref.get_array_index(), &result);
			if (result == 0)
			{
				relation_proto_s2c_confirm_marry_examination req;
				couple_ref.send_message_to_self(&req, e_msgindex_s2c_confirm_marry_examination);
				player_ref.send_notice("90096912");
			}
			else 
			{
				player_ref.send_notice(std::to_string(result));
			}
		}
	}

	void marry_mgr::confirm_marry_dati(int32 is_receive)
	{
		//script_mgr::get_instance().call_func("reload_module", 0, "%d", "marry_mgr");
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() && player_ref.is_self_server())
		{
			if (is_receive == 0)
			{
				int32 timu_id = 0;
				script_mgr::get_instance().call_func(nullptr, "random_dati", 1, false, ">%d", &timu_id);
				int32 old_id = player_ref.get_unit_info(e_role_info_marry_dati_id);
				if (old_id == 0)
				{
					script_mgr::get_instance().call_func(nullptr, "set_new_timu", 0, false, "%d%d", m_array_index, timu_id);
				}
				player& couple_ref = unit_man::get_player(player_ref.get_couple_guid());
				if (couple_ref.is_valid() && couple_ref.is_self_server())
				{
					int32 old_id_couple = couple_ref.get_unit_info(e_role_info_marry_dati_id);
					if (old_id_couple == 0)
					{
						script_mgr::get_instance().call_func(nullptr, "set_new_timu", 0, false, "%d%d", couple_ref.get_array_index(), timu_id);
					}
					couple_ref.get_marry_mgr().sync_dati_marry(1);
				}
				sync_dati_marry(1);
			}
			else 
			{
				player& couple_ref = unit_man::get_player(player_ref.get_couple_guid());
				if (couple_ref.is_valid() && couple_ref.is_self_server())
				{
					couple_ref.send_notice("90096896");
				}
			}
		}
	}
	void marry_mgr::sync_heart_value()
	{
		player& player_ref = unit_man::get_player(m_array_index);

		if (player_ref.is_valid())
		{
			relation_proto_s2c_sync_marry_heart_value msg;
			msg.set_heart_value(get_heart_value());
			//灵犀值
			player_ref.send_message_to_self(&msg, e_mgsindex_s2c_sync_marry_heart_value);
		}	
	}
	//stage = 0 服务器同步
	//stage = 1 开始答题
	//stage = 2 答题中
	void marry_mgr::sync_dati_marry(int32 stage)
	{
		player& player_ref = unit_man::get_player(m_array_index);

		if (player_ref.is_valid() && player_ref.is_self_server())
		{
			int32 dati_id = player_ref.get_unit_info(e_role_info_marry_dati_id);
			int32 cur_num = player_ref.get_unit_info(e_role_info_marry_dati_num);
			int32 cur_right_num = player_ref.get_unit_info(e_role_info_marry_dati_right_num);
			relation_proto_s2c_sync_marry_examination msg;
			msg.set_dati_id(dati_id);
			msg.set_dati_num(cur_num);
			msg.set_dati_right_num(cur_right_num);
			msg.set_heart_value(get_heart_value());
			msg.set_dati_stage(stage);
			//灵犀值
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_sync_marry_examination);
		}
	}

	void marry_mgr::confirm_marry_operate(int32 operate_type, int32 ext_param_1, int32 ext_param_2)
	{
		//script_mgr::get_instance().call_func("reload_module", 0, "%d","marry_mgr" );
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() && player_ref.is_self_server())
		{
			int32 is_right = -1;
			player& couple_ref = unit_man::get_player(player_ref.get_couple_guid());
			if (couple_ref.is_valid() && couple_ref.is_self_server())
			{
				if (operate_type == 0)
				{
					int32 m_answer1 = ext_param_2;
					int32 m_answer2 = 0;
					script_mgr::get_instance().call_func(nullptr, "confirm_dati", 2, false, "%d%d%d>%d%d", m_array_index, couple_ref.get_array_index(), ext_param_2, &is_right, &m_answer2);
					if (is_right != -1)
					{
						sync_dati_marry(2);
						couple_ref.get_marry_mgr().sync_dati_marry(2);
						relation_proto_s2c_operate_marry_examination reult_msg;
						reult_msg.set_operate_type(operate_type);
						reult_msg.set_result(is_right);
						reult_msg.set_my_answer(m_answer1);
						reult_msg.set_couple_answer(m_answer2);
						player_ref.send_message_to_self(&reult_msg, e_msgindex_s2c_operate_marry_examination);
						reult_msg.set_my_answer(m_answer2);
						reult_msg.set_couple_answer(m_answer1);
						couple_ref.send_message_to_self(&reult_msg, e_msgindex_s2c_operate_marry_examination);
					}

				}
				else if (operate_type == 1)
				{
					//终止答题
					relation_proto_s2c_operate_marry_examination reult_msg_couple;
					reult_msg_couple.set_operate_type(operate_type);
					reult_msg_couple.set_result(is_right);
					couple_ref.send_message_to_self(&reult_msg_couple, e_msgindex_s2c_operate_marry_examination);
					//couple_ref.send_notice("90096780");
				}
			}
		}
	}
	void marry_mgr::add_heart_value(int32 add_value,int32 send_type)
	{
		if (0 == add_value)
			return;
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() && player_ref.is_self_server())
		{
			cs2ws_add_heart_value msg;
			msg.role_guid = player_ref.get_unit_guid();
			msg.add_value = add_value;
			msg.send_type = send_type;
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_add_heart_value));
		}
	}
	int32 marry_mgr::get_heart_value()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() && player_ref.is_self_server())
		{
			//citem * equip_item = player_ref.get_item_set().get_wedding_equip_item();
			//if (nullptr != equip_item)
			//{
			//	return equip_item->get_data_info(e_item_info_random_property2);
			//}
		}
		return 0;
	}

	void marry_mgr::cost_bless_money(const guid_64& target_guid)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() && player_ref.is_self_server())
		{
			bool success = false;
			int32 need_money_num = GAMECONFIG->CoupleBlessNeedMoney;
			if (need_money_num <= player_ref.get_money_data(e_money_type_jewel_bind))
			{
				success = true;
				player_ref.cut_money(e_money_type_jewel_bind, need_money_num, e_server_log_cut_money_couple_bless);
			}
			else if(need_money_num <= player_ref.get_money_data(e_money_type_jewel))
			{
				success = true;
				player_ref.cut_money(e_money_type_jewel, need_money_num, e_server_log_cut_money_couple_bless);
			}
			if (success)
			{
				cs2ws_cost_bless_money_result msg;
				strncpy(msg.role_name, player_ref.get_name(),max_name_size);
				msg.target_guid = target_guid;
				connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_cost_bless_money_result));
			}
		}
	}

	void marry_mgr::req_use_fireworks(const guid_64& obj_guid, const std::string& target_name, const std::string& target_blessing)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid())
		{
			//citem* item_ptr = player_ref.get_item_set().get_item(obj_guid);
			//if (nullptr == item_ptr)
			//	return ;
			//ItemTemplate* real_item_template_ptr = item_ptr->get_item_info_ptr();
			//if (real_item_template_ptr == nullptr)
			//	return;
			////检测是否是烟花道具
			//if (real_item_template_ptr->item_type != e_item_type_expendable || real_item_template_ptr->sub_type != e_prop_sub_type_fireworks)
			//	return;

			////使用通用接口
			//citem* end_item = nullptr;
			//int32 template_name = real_item_template_ptr->ItemName;
			//bool Success = player_ref.get_item_set().item_use(item_ptr->get_item_guid(), end_item);
			//if (Success)
			//{
			//	//发送公告
			//	int32 notice_id = 93000293;
			//	std::vector<std::string> vec_notice_str;
			//	vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			//	vec_notice_str.push_back(player_ref.get_name());
			//	vec_notice_str.push_back(init_unit::change_i32_to_string(template_name));
			//	std::string tmp_target_name = target_name;
			//	invalid_ansi_word::replace_invalid_ansi_str(tmp_target_name, false);
			//	vec_notice_str.push_back(tmp_target_name);
			//	std::string tmp_target_blessing = target_blessing;
			//	invalid_ansi_word::replace_invalid_ansi_str(tmp_target_blessing, false);
			//	vec_notice_str.push_back(tmp_target_blessing);
			//	xstring notice_str = init_unit::implode(vec_notice_str);
			//	cchat_mgr::send_notice(notice_id, notice_str, guid_64());
			//}
		}
	}

	void marry_mgr::req_lucky_operator(e_lucky_info_req_type type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false || player_ref.is_self_server() == false)
			return;
		switch (type)
		{
		case e_lucky_info_req_type_vec:
		{
			cs2ws_send_lucky_info_to_client msg;
			msg.target_guid = player_ref.get_unit_guid();
			msg.req_type = e_lucky_info_req_type_vec;
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_send_lucky_info_to_client));
			return;
		}
		break;
		case e_lucky_info_req_type_special_info:
		{
			int32 has_use_time = player_ref.get_logic_data(e_role_logic_info_daily_lucky_times);
			if (has_use_time >= GAMECONFIG->DailyLuckyFreeTime)
				return;
			player_ref.set_logic_data(e_role_logic_info_daily_lucky_times, has_use_time + 1);
			player_ref.send_logic_one(e_role_logic_info_daily_lucky_times);

			cs2ws_send_lucky_info_to_client msg;
			msg.target_guid = player_ref.get_unit_guid();
			msg.req_type = e_lucky_info_req_type_special_info;
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_send_lucky_info_to_client));
			return;
		}
		break;
		case e_lucky_info_req_type_buy_special_times:
		{
			int32 has_use_time = player_ref.get_logic_data(e_role_logic_info_daily_lucky_times);
			if (has_use_time == 0)
				return;
			player_ref.set_logic_data(e_role_logic_info_daily_lucky_times, has_use_time - 1);
			player_ref.send_logic_one(e_role_logic_info_daily_lucky_times);
			bool success = false;
			int32 need_money_num = GAMECONFIG->LuckyBuyNeedValue;
			if (need_money_num <= player_ref.get_money_data(e_money_type_jewel_bind))
			{
				success = true;
				player_ref.cut_money(e_money_type_jewel_bind, need_money_num, e_server_log_cut_money_buy_special_lucky_times);
			}
			else if (need_money_num <= player_ref.get_money_data(e_money_type_jewel))
			{
				success = true;
				player_ref.cut_money(e_money_type_jewel, need_money_num, e_server_log_cut_money_buy_special_lucky_times);
			}
			if (success)
			{
				player_ref.set_logic_data(e_role_logic_info_daily_lucky_times, has_use_time - 1);
				player_ref.send_logic_one(e_role_logic_info_daily_lucky_times);
				player_ref.send_notice("90096794");
			}
			return;
		}
		break;
		default:
			break;
		}
		return;
	}
}