/********************************************************************
created: 2019年4月24日
file base: server_harry_mgr
file ext: cpp
author: zhangshuo

purpose: 跨服掠夺
*********************************************************************/


#include "cell_server.hpp"
#include "connection_mgr.hpp"
#include "internet/cs2dp.pb.h"
#include "internal/cross_server_harry_msg.hpp"
#include "Logic/activity_def.hpp"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "server_harry_mgr.h"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "utility/parse_msg.h"
#include "internet/game.pb.h"
#include "internet/net.pb.h"

namespace faith
{
	server_harry_mgr::server_harry_mgr()
	{
		m_player_ptr = nullptr;
	}

	server_harry_mgr::~server_harry_mgr()
	{

	}

	void server_harry_mgr::clear_data()
	{

	}

	void server_harry_mgr::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
	}

	bool server_harry_mgr::is_in_harry_game_time()
	{
		return cell_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_harry, e_activity_time_get_all) > 0;
	}

	bool server_harry_mgr::is_in_harry_special_game_time()
	{
		return cell_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_harry, e_activity_time_get_all, true) > 0;
	}

	void server_harry_mgr::save_role_harry_info_to_db(int32 type)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_role_harry_info);
		if (is_use == false)
		{
			cs2dp_save_role_harry_info req;
			req.save_type_ex = type;
			req.role_guid = m_player_ptr->get_unit_guid();
			req.unit_array_index = m_player_ptr->get_array_index();
			for (int32 i = 0; i < e_harry_record_max; i++)
			{
				req.info_data.data_ary[i] = m_harry_info.data_ary[i];
			}
			m_player_ptr->send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			faith::cs2dp_proto::save_role_harry_record msg;
			msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
			msg.set_unit_array_index(m_player_ptr->get_array_index());
			msg.set_save_type_ex(type);

			faith::cs2dp_proto::role_harry_record_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			for (int32 i = 0; i < e_harry_record_max; i++)
			{
				db_data->add_data_ary(m_harry_info.data_ary[i]);
			}
			m_player_ptr->send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_role_harry_info);
		}
	}

	void server_harry_mgr::load_role_harry_info_from_db(const s_db_role_harry_info & harry_info)
	{
		for (int32 i = 0; i < e_harry_record_max; i++)
		{
			m_harry_info.data_ary[i] = harry_info.data_ary[i];
		}
		send_role_harry_info_all();
	}
	bool server_harry_mgr::load_role_harry_record_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_harry_record_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_db_role_harry_info db_data;

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}	
		load_role_harry_info_from_db(db_data);
		return true;

	}
	

	void server_harry_mgr::clear_harry_info_all()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		for (int32 i = 0; i < e_harry_record_max; i++)
		{
			m_harry_info.data_ary[i] = 0;
		}
		m_player_ptr->change_harry_buff();
		send_role_harry_info_all();
	}

	void server_harry_mgr::refresh_day_harry_info()
	{
		for (int32 i = e_harry_record_normal_harry_count; i < e_harry_record_harry_item_server_id; i++)
		{
			m_harry_info.data_ary[i] = 0;
		}
		send_role_harry_info_all();
	}

	void server_harry_mgr::set_harry_data_info(int32 idex, int32 value)
	{
		if (idex < e_harry_record_harry_value || idex >= e_harry_record_max)
		{
			return;
		}
		m_harry_info.data_ary[idex] = value;
		send_role_harry_info_one(idex);
	}

	int32 server_harry_mgr::get_harry_data_info(int32 idex)
	{
		if (idex < e_harry_record_harry_value || idex >= e_harry_record_max)
		{
			return 0;
		}
		return m_harry_info.data_ary[idex];
	}

	void server_harry_mgr::finish_collect_set_item_server_id(int32 server_id, bool is_special_item)
	{
		int32 item_server_id = server_id;
		if (is_special_item)
		{
			item_server_id += special_harry_mark;
		}
		set_harry_data_info(e_harry_record_harry_item_server_id, item_server_id);
		if (m_player_ptr)
		{
			server_log::cross_server_harry_log(m_player_ptr->get_third_info(), m_player_ptr->get_unit_info_inst(), m_player_ptr->get_login_type(), faith::e_cross_server_harry_oper_get, is_special_item ? 1 : 0, server_id, get_harry_data_info(e_harry_record_normal_harry_count), get_harry_data_info(e_harry_record_special_harry_count));

			set_log_var(log_head);

			m_player_ptr->get_log_common_head_info(log_head);

			server_log::serverCrossServerHarry(log_head, faith::e_cross_server_harry_oper_get, is_special_item ? 1 : 0, server_id, get_harry_data_info(e_harry_record_normal_harry_count), get_harry_data_info(e_harry_record_special_harry_count));
		}
	}

	void server_harry_mgr::add_defend_value(int32 value)
	{
		m_harry_info.data_ary[e_harry_record_defend_value] += value;
		send_role_harry_info_one(e_harry_record_defend_value);
		sync_defend_data_ranking_data();
	}

	void server_harry_mgr::add_harry_value(int32 value)
	{
		m_harry_info.data_ary[e_harry_record_harry_value] += value;
		send_role_harry_info_one(e_harry_record_harry_value);
	}

	void server_harry_mgr::add_cross_rob_must_daily_count()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		int32 cur_count = m_player_ptr->get_must_do_count(e_daily_must_do_typ_cross_rob);
		if (cur_count > 0)
		{
			return;
		}
		bool is_finish = true;
		for (int32 i = e_harry_type_normal_harry;i<e_harry_type_max;i++)
		{
			if (can_have_item_with_harry_type(i))
			{
				is_finish = false;
				break;
			}
		}
		if (is_finish)
		{
			m_player_ptr->set_must_do_count(e_daily_must_do_typ_cross_pk, cur_count + 1);
			m_player_ptr->send_daily_must_do_count_info_one(e_daily_must_do_typ_cross_pk);
			m_player_ptr->set_daily_active_degree_info(e_daily_must_do_typ_cross_pk);
			int32 must_do_type = e_daily_must_do_typ_cross_pk;
			server_log::activity_role_log(m_player_ptr->get_third_info(), m_player_ptr->m_role_info, must_do_type,harry_map_id);
		}

	}

	void server_harry_mgr::add_defend_or_harry_value_with_ratio(float ratio, bool is_defend_value)
	{
		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_com_cfg)
		{
			return;
		}
		int32 base_value = act_com_cfg->ParamInt3;
		int32 last_value = base_value * ratio;
		if (is_defend_value)
		{
			add_defend_value(last_value);
		}
		else
		{
			add_harry_value(last_value);
		}
		
	}

	bool server_harry_mgr::is_have_item()
	{
		int32 server_item_id = get_harry_data_info(e_harry_record_harry_item_server_id);
		return server_item_id > 0;
	}

	void server_harry_mgr::clear_server_item_with_player_dead()
	{
		int32 item_server_id = get_harry_data_info(e_harry_record_harry_item_server_id);
		int32 is_special_value = item_server_id / special_harry_mark;
		if (is_special_value > 0)
		{
			item_server_id -= special_harry_mark;
		}
		s_server_harry_msg& server_harry_msg = cell_server::getInstance().get_harry_msg_with_server_id(item_server_id);
		server_harry_msg.server_money += 1;
		send_ws_change_server_money(false, 1, item_server_id);
		set_harry_data_info(e_harry_record_harry_item_server_id, 0);

		if (m_player_ptr)
		{
			server_log::cross_server_harry_log(m_player_ptr->get_third_info(), m_player_ptr->get_unit_info_inst(), m_player_ptr->get_login_type(), faith::e_cross_server_harry_oper_dead_drop, is_special_value > 0 ? 1 : 0, item_server_id, get_harry_data_info(e_harry_record_normal_harry_count), get_harry_data_info(e_harry_record_special_harry_count));

			set_log_var(log_head);

			m_player_ptr->get_log_common_head_info(log_head);

			server_log::serverCrossServerHarry(log_head, faith::e_cross_server_harry_oper_dead_drop, is_special_value > 0 ? 1 : 0, item_server_id, get_harry_data_info(e_harry_record_normal_harry_count), get_harry_data_info(e_harry_record_special_harry_count));
		}
	}

	bool server_harry_mgr::can_have_item_with_harry_type(int32 harry_type)
	{
		if (harry_type < e_harry_type_normal_harry || harry_type >= e_harry_type_max)
		{
			return false;
		}
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return false;
		}
		std::vector<int32> branch_mission_count_arr = act_cfg_ptr->ParamIntArr2;
		if (branch_mission_count_arr.size() < e_harry_branch_mission_max)
		{
			return false;
		}
		int32 cur_count = 0;
		cur_count = get_harry_data_info(harry_type + 2);
		if (cur_count >= branch_mission_count_arr[harry_type])
		{
			return false;
		}
		return true;
	}

	bool server_harry_mgr::can_have_money_with_harry_type(int32 harry_type)
	{
		if (harry_type < e_harry_type_normal_harry || harry_type >= e_harry_type_max)
		{
			return false;
		}
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return false;
		}
		std::vector<int32> branch_mission_count_arr = act_cfg_ptr->ParamIntArr2;
		if (branch_mission_count_arr.size() < e_harry_branch_mission_max)
		{
			return false;
		}
		int32 cur_count = 0;
		cur_count = get_harry_data_info(harry_type + 2);
		if (cur_count >= branch_mission_count_arr[harry_type + 4])
		{
			return false;
		}
		return true;
	}

	bool server_harry_mgr::can_harry_item()
	{
		if (!is_in_harry_game_time())
		{
			return false;
		}
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return false;
		}
		int32 max_count = act_cfg_ptr->ParamInt2;
		int32 cur_count = get_harry_data_info(e_harry_record_normal_harry_count);
		if (cur_count >= max_count && !is_special_collect())
		{
			return false;
		}
		return true;
	}

	bool server_harry_mgr::can_harry_item_with_server_money()
	{
		if (!is_in_harry_game_time())
		{
			return false;
		}
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return false;
		}
		std::vector<int32> server_money_arr = act_cfg_ptr->ParamIntArr1;
		if (server_money_arr.size() < e_server_money_max)
		{
			return false;
		}
		int32 min_server_money = server_money_arr[e_server_money_minimum];
		const s_server_harry_msg cur_server_harry_msg = cell_server::getInstance().get_cur_server_harry_msg();
		int32 cur_server_money = cur_server_harry_msg.server_money;
		if (cur_server_money <= min_server_money)
		{
			return false;
		}
		return true;
	}

	bool server_harry_mgr::is_special_collect()
	{
		const s_server_harry_msg cur_server_harry_msg = cell_server::getInstance().get_cur_server_harry_msg();
		int32 cur_server_money = cur_server_harry_msg.server_money;
		int32 cur_server_special_money = cur_server_harry_msg.special_money;
		if (is_have_special_harry_item_count() && cur_server_special_money > 0 && is_in_harry_special_game_time())
		{
			return true;
		}
		return false;
	}

	bool server_harry_mgr::is_have_special_harry_item_count()
	{
		int32 cur_special_harry_count = get_harry_data_info(e_harry_record_special_harry_count);
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return false;
		}
		std::vector<int32> branch_mission_count_arr = act_cfg_ptr->ParamIntArr2;
		if (branch_mission_count_arr.size() < e_harry_branch_mission_max)
		{
			return false;
		}
		int32 max_special_harry_count = branch_mission_count_arr[e_harry_branch_mission_special_harry_count];
		if (cur_special_harry_count >= max_special_harry_count)
		{
			return false;
		}
		return true;
	}

	void server_harry_mgr::finish_gather_item()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		m_player_ptr->send_notice("90203506");
		int32 cur_server_id = cell_server::getInstance().get_server_id();
		s_server_harry_msg& cur_harry_msg = cell_server::getInstance().get_cur_server_harry_msg();
		if (is_special_collect())
		{
			cur_harry_msg.special_money -= 1;
			cur_harry_msg.server_money -= 1;
			send_ws_change_server_money(true, -1, cur_server_id);
			finish_collect_set_item_server_id(cur_server_id, true);
		}
		else
		{
			cur_harry_msg.server_money -= 1;
			send_ws_change_server_money(false, -1, cur_server_id);
			finish_collect_set_item_server_id(cur_server_id, false);
		}
		m_player_ptr->change_harry_buff();
	}

	void server_harry_mgr::send_ws_change_server_money(bool is_special_harry, int32 change_value, int32 server_id)
	{
		cs2ws_send_change_server_money msg;
		msg.is_special_harry = is_special_harry;
		msg.change_value = change_value;
		msg.server_id = server_id;
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
	}


	void server_harry_mgr::kill_player_with_harry(int32 other_player_index)
	{
		int64 cur_time_sec = time_helper::get_cur_time_new().second;
		ActivityCommonConfigTemplate* act_com_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_com_cfg_ptr)
		{
			return;
		}
		int32 interval_time = act_com_cfg_ptr->ParamInt5;
		player& killed_player_ref = unit_man::get_player(other_player_index);
		if (nullptr == m_player_ptr || false == killed_player_ref.is_valid())
		{
			return;
		}
		if (killed_player_ref.get_last_kill_me_player_guid() == m_player_ptr->get_unit_guid() && killed_player_ref.get_last_kill_me_player_time() + interval_time <= cur_time_sec)//检测是否刷人头
		{
			return;
		}
		if (m_player_ptr->get_last_kill_player_guid() == killed_player_ref.get_unit_guid() && m_player_ptr->get_last_kill_player_time() + interval_time <= cur_time_sec)
		{
			return;
		}
		int32 cur_server_id = cell_server::getInstance().get_server_id();
		int32 cur_player_server_id = m_player_ptr->get_unit_info(e_role_info_server_id);
		int32 other_player_server_id = killed_player_ref.get_unit_info(e_role_info_server_id);
		if (cur_server_id != cur_player_server_id && cur_server_id != other_player_server_id)
		{
			return;
		}
		if (cur_player_server_id == other_player_server_id)
		{
			return;
		}
		bool is_defend = true;
		if (other_player_server_id == cur_server_id)
		{
			is_defend = false;
		}
		guid_64 cur_player_id = m_player_ptr->get_unit_guid();
		guid_64 cur_other_player_id = killed_player_ref.get_unit_guid();
		int32 cur_player_level = m_player_ptr->get_owner_unit_level();
		int32 cur_other_level = killed_player_ref.get_owner_unit_level();
		bool whether_add_sorce = unit_man::kill_player_num(cur_player_id, cur_other_player_id, cur_player_level, cur_other_level); //加在哪里呢
		if (!whether_add_sorce)
		{
			return;
		}
		float award_ratio = get_award_ratio_with_kill_player_level(killed_player_ref.get_unit_info(e_role_info_exp_level));

		std::vector<int32> money_arr;
		std::vector<int32> item_arr;
		int32 harry_type = e_harry_type_kill_ct;
		if (is_defend)
		{
			harry_type = e_harry_type_kill_t;
		}
		get_award_item_and_money_with_harry_type(item_arr, money_arr, harry_type);
		if (item_arr.size() % 2 != 0 || item_arr.size() <= 0 || money_arr.size() % 2 != 0 || money_arr.size() <= 0)
		{
			return;
		}
		add_defend_or_harry_value_with_ratio(award_ratio, is_defend);

		e_money_type money_type = (e_money_type)money_arr[0];
		int32 money_value = money_arr[1] * award_ratio;

		if (can_have_item_with_harry_type(harry_type))
		{
			m_player_ptr->get_item_set().put_in_bag(e_server_log_add_item_harry, other_player_server_id, item_arr);
		}
		if (can_have_money_with_harry_type(harry_type))
		{
			m_player_ptr->add_money(money_type, money_value, e_server_log_add_money_harry, -1, true);
			m_player_ptr->send_money_one(money_type);
		}

		if (is_defend)
		{
			int32 cur_harry_count = get_harry_data_info(e_harry_record_kill_terrorist_num);
			cur_harry_count++;
			set_harry_data_info(e_harry_record_kill_terrorist_num, cur_harry_count);
		}
		else
		{
			int32 cur_harry_count = get_harry_data_info(e_harry_record_kill_ct_num);
			cur_harry_count++;
			set_harry_data_info(e_harry_record_kill_ct_num, cur_harry_count);
		}

	}

	void server_harry_mgr::give_server_item_to_npc()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		bool is_special_item = false;
		int32 item_server_id = get_harry_data_info(e_harry_record_harry_item_server_id);
		int32 is_special_value = item_server_id / special_harry_mark;
		if (is_special_value > 0)
		{
			is_special_item = true;
			item_server_id -= special_harry_mark;
		}
		const s_server_harry_msg& harry_msg = cell_server::getInstance().get_harry_msg_with_server_id(item_server_id);
		if (harry_msg.server_id <= 0)
		{
			return;
		}
		float award_ratio = get_award_ratio_with_server_money(harry_msg.server_money);
		std::vector<int32> money_arr;
		std::vector<int32> item_arr;
		int32 harry_type = e_harry_type_normal_harry;
		if (is_special_item)
		{
			harry_type = e_harry_type_special_harry;
		}
		get_award_item_and_money_with_harry_type(item_arr, money_arr, harry_type);
		if (item_arr.size() % 2 != 0 || item_arr.size() <= 0 || money_arr.size() % 2 != 0 || money_arr.size() <= 0)
		{
			return;
		}

		set_harry_data_info(e_harry_record_harry_item_server_id, 0);

		add_defend_or_harry_value_with_ratio(award_ratio, false);
		
		e_money_type money_type = (e_money_type)money_arr[0];
		int32 money_value = money_arr[1] * award_ratio;

		if (can_have_item_with_harry_type(harry_type))
		{
			m_player_ptr->get_item_set().put_in_bag(e_server_log_add_item_harry, item_server_id, item_arr);
		}
		if (can_have_money_with_harry_type(harry_type))
		{
			m_player_ptr->add_money(money_type, money_value, e_server_log_add_money_harry, -1, true);
			m_player_ptr->send_money_one(money_type);
		}

		if (is_special_item)
		{
			int32 cur_harry_count = get_harry_data_info(e_harry_record_special_harry_count);
			cur_harry_count++;
			set_harry_data_info(e_harry_record_special_harry_count, cur_harry_count);
		}
		else
		{
			int32 cur_harry_count = get_harry_data_info(e_harry_record_normal_harry_count);
			cur_harry_count++;
			set_harry_data_info(e_harry_record_normal_harry_count, cur_harry_count);
		}
	

		int32 cur_server_id = m_player_ptr->get_unit_info(e_role_info_server_id);
		s_server_harry_msg& cur_harry_msg = cell_server::getInstance().get_cur_server_harry_msg();
		cur_harry_msg.server_money += 1;
		m_player_ptr->send_notice("90203548");
		send_ws_change_server_money(false, 1, cur_server_id);//提交采集时 不加对方服务器特殊采集

		if (m_player_ptr)
		{
			server_log::cross_server_harry_log(m_player_ptr->get_third_info(), m_player_ptr->get_unit_info_inst(), m_player_ptr->get_login_type(), faith::e_cross_server_harry_oper_commit, is_special_item ? 1 : 0, item_server_id, get_harry_data_info(e_harry_record_normal_harry_count), get_harry_data_info(e_harry_record_special_harry_count));

			set_log_var(log_head);

			m_player_ptr->get_log_common_head_info(log_head);

			server_log::serverCrossServerHarry(log_head, faith::e_cross_server_harry_oper_commit, is_special_item ? 1 : 0, item_server_id, get_harry_data_info(e_harry_record_normal_harry_count), get_harry_data_info(e_harry_record_special_harry_count));

		}
	}

	float server_harry_mgr::get_award_ratio_with_server_money(float server_money)
	{
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return .0f;
		}
		std::vector<int32> init_money_arr = act_cfg_ptr->ParamIntArr1;
		if (init_money_arr.size() < e_server_money_max)
		{
			return .0f;
		}
		float init_money = init_money_arr[e_server_money_init];
		if (init_money <= 0)
		{
			return .0f;
		}
		float ratio = server_money / init_money;
		if (is_in_harry_special_game_time())
		{
			float special_ratio = (float)act_cfg_ptr->ParamInt6 / 100.0f;
			ratio += special_ratio;
		}
		return ratio;
	}

	float server_harry_mgr::get_award_ratio_with_kill_player_level(float other_player_level)
	{
		if (nullptr == m_player_ptr)
		{
			return .0f;
		}
		float cur_player_level = m_player_ptr->get_unit_info(e_role_info_exp_level);
		float ratio = (cur_player_level + other_player_level) / (2 * cur_player_level);
		return ratio;
	}

	void server_harry_mgr::get_award_item_and_money_with_harry_type(std::vector<int32>& item_arr, std::vector<int32>& money_arr, int32 harry_type)
	{
		item_arr.clear();
		money_arr.clear();
		if (harry_type < e_harry_type_normal_harry || harry_type >= e_harry_type_max)
		{
			return;
		}
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return;
		}
		int32 money_value = act_cfg_ptr->ParamInt4;
		std::vector<int32> cur_item_arr;
		switch (harry_type)
		{
		case e_harry_type_normal_harry:
			cur_item_arr = act_cfg_ptr->ParamIntArr4;
			break;
		case e_harry_type_special_harry:
			cur_item_arr = act_cfg_ptr->ParamIntArr5;
			break;
		case e_harry_type_kill_ct:
		case e_harry_type_kill_t:
			cur_item_arr = act_cfg_ptr->ParamIntArr6;
			break;
		default:
			break;
		}
		if (money_value <= 0 || cur_item_arr.size() <= 0 || cur_item_arr.size() % 2 != 0)
		{
			return;
		}
		item_arr = cur_item_arr;
		money_arr.push_back((int32)e_money_type_cross_server_money);
		money_arr.push_back(money_value);
	}

	void server_harry_mgr::send_role_harry_info_one(int32 harry_record_type)
	{
		if (harry_record_type < e_harry_record_harry_value || harry_record_type >= e_harry_record_max || m_player_ptr == nullptr)
		{
			return;
		}
		add_cross_rob_must_daily_count();
		game_proto_role_harry_msg_info_one msg;
		msg.set_harry_idex(harry_record_type);
		msg.set_harry_value(get_harry_data_info(harry_record_type));
		m_player_ptr->send_message(&msg, e_msgindex_s2c_role_harry_msg_info_one);
	}

	void server_harry_mgr::send_role_harry_info_all()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		game_proto_role_harry_msg_info_all msg;
		for (int32 i = 0; i < e_harry_record_max; ++i)
		{
			msg.add_harry_value_arr(get_harry_data_info(i));
		}
		m_player_ptr->send_message(&msg, e_msgindex_s2c_role_harry_msg_info_all);
	}

	void server_harry_mgr::sync_defend_data_ranking_data()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		int64 ranking_value = m_player_ptr->get_server_harry_mgr().get_harry_data_info(e_harry_record_defend_value);
		m_player_ptr->get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_cross_server_plinder_guard_value, ranking_value);
	}

}
