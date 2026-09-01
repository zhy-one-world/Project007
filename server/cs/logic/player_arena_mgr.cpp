 /********************************************************************
created: 2016年8月23日13:56:28
file base: arena_mgr
file ext: cpp
author: zhangminghai

purpose: about arena system's arena_mgr
*********************************************************************/

#include "time.hpp"
#include "connection_mgr.hpp"
#include "player_arena_mgr.hpp"
#include <Utility/parse_msg.h>
#include "utility/cs_date.hpp"
#include "template/StringConst_S.h"
#include "logic/player.hpp"
#include "template/template_manager.h"
#include "logic/unit_man.h"
#include "game_enum_def_s.h"
#include "server_log.hpp"
#include "internal/core.hpp"
#include "internet/arena.pb.h"
#include "internet/net.pb.h"
#include "utility/init_unit.h"
#include "internal/arena_msg.hpp"
#include "utility/globle_data.h"

namespace faith
{
	player_arena_mgr::challenge_name_map   player_arena_mgr::name_map;

	

	void player_arena_mgr::heart_tick(const int64& new_time)
	{
		if (m_is_requiring
			&& m_requiring_safty_timer < new_time)
		{
			set_is_requiring(false);
		}
	}

	void player_arena_mgr::set_player_ptr(int32& unit_index)
	{
		m_unit_index = unit_index;
	}
	void player_arena_mgr::clear_data()
	{
		m_current_rank = arena_not_in_list_rank;
		m_today_add_time = 0;
		set_is_requiring(false);
	}
	void player_arena_mgr::get_arena_rank()
	{
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		cs2ws_req_get_arena_rank req;
		req.role_guid = temp_player.get_unit_guid();

		bool is_use = proto_by_lua(e_msg_index_cs2ws_req_get_arena_rank);
		if (is_use == false)
		{
			connection_mgr::getInstance().send_to_ws(&req, sizeof(cs2ws_req_get_arena_rank));
		}
		else
		{
			faith::cs2ws_proto::req_get_arena_rank pro_msg;
			req.to_proto(pro_msg);

			connection_mgr::getInstance().send_to_ws_lua(&pro_msg, e_msg_index_cs2ws_req_get_arena_rank, 0);
		}
	}

	void player_arena_mgr::get_arena_rank_end(int32 arena_rank, int32 is_request)
	{
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		m_current_rank = arena_rank;
		arena_proto_get_arena_rank_end  resp;
		resp.add_role_guid(temp_player.get_unit_guid().A);
		resp.add_role_guid(temp_player.get_unit_guid().B);
		resp.set_arena_rank(arena_rank);
		temp_player.send_message_to_self(&resp, e_msgindex_s2c_get_rank_end);

		if (is_request == 1)
		{
			temp_player.update_three_day_arean_rank(arena_rank);
		}

	}

	void player_arena_mgr::challenge_over(const guid_64& target_role_guid, int32 end_result, int32 target_rank, int32 self_rank)
	{
		set_is_requiring(false);
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return ;
		}

		//放在 save_role_info 处理，放在这的话，因为玩家掉线（主动离开），会 先 save 再到这里，所以这里设置的值就没意义了
		//temp_player.set_time_data(e_time_type_arena_colling_time, GAMECONFIG->ArenaClearChallengeCdTime * 1000 + utility::get_tick_count());
		//temp_player.send_time_one(e_time_type_arena_colling_time);
		
		int32 temp_cur_value = temp_player.get_welfare_mgr().get_active_degree_info(e_daily_active_degree_type_attend_three_arena); // 活跃度
		temp_player.get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_attend_three_arena, temp_cur_value + 1);

		cs2ws_chellenge_over req;
		req.role_guid = temp_player.get_unit_guid();
		req.target_guid = target_role_guid;
		req.role_info.role_guid = temp_player.get_unit_guid();
		req.role_info.set_role_name(temp_player.get_name());
		req.role_info.gs_value = temp_player.get_unit_gs_value();
		req.role_info.role_template_id = temp_player.get_unit_info(e_role_info_template_id);
		req.role_info.vip_level = temp_player.get_vip_level();
		req.role_info.set_game_channel(temp_player.get_third_info().param14);
		req.role_info.rank_pos = self_rank;
		req.role_info.military_level = temp_player.get_special_name_mgr().get_name_level_by_type(e_special_name_type_military_rank);
		req.chellenge_result = end_result;
		req.target_rank = target_rank;
		bool is_use = proto_by_lua(e_msg_index_cs2ws_chellenge_over);
		if (is_use == false)
		{
			connection_mgr::getInstance().send_to_ws(&req, sizeof(cs2ws_chellenge_over));
		}
		else
		{
			faith::cs2ws_proto::chellenge_over pro_msg;
			req.to_proto(pro_msg);
			connection_mgr::getInstance().send_to_ws_lua(&pro_msg, e_msg_index_cs2ws_chellenge_over);
		}
		int32 m_old_rank = m_current_rank;
		switch (end_result)
		{
		case e_arena_end_type_failed:
		case e_arena_end_type_draw:
		{
			temp_player.set_unit_info(e_role_info_arena_winning_streak, 0);
			temp_player.send_info_one(e_role_info_arena_winning_streak);
		}
		break;
		case e_arena_end_type_win:
		{
			temp_player.set_unit_info(e_role_info_arena_winning_streak, temp_player.get_unit_info(e_role_info_arena_winning_streak) + 1);
			temp_player.send_info_one(e_role_info_arena_winning_streak);
			m_current_rank = target_rank;
			req_get_match_player();
			temp_player.get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_arena_win); //新限时行为竞技场获胜计数
		}
		break;
		default:
			break;
		}

		if (end_result == e_arena_end_type_win)
		{
			add_challeng_log(temp_player.get_unit_guid(), target_role_guid, true, end_result, m_old_rank, target_rank);
		}
		else
		{
			add_challeng_log(temp_player.get_unit_guid(), target_role_guid, true, end_result, m_old_rank, m_old_rank);
		}

		int32 winning_streak = temp_player.get_unit_info(e_role_info_arena_winning_streak);
		if ((winning_streak % 10 == 0) && winning_streak != 0)
		{
			ArenaRankRewardTemplate* arena_template_ptr = template_manager::get_instance().get_arena_reward_by_arena_pos(m_current_rank);		// 为了拿到NoticeID
			if (arena_template_ptr)
			{
				int32 notice_id = arena_template_ptr->NoticeId;
				std::vector<std::string> vec_notice_str;
				vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
				vec_notice_str.push_back(temp_player.get_name());
				vec_notice_str.push_back(template_manager::get_instance().int_to_string(winning_streak));
				std::string notice_str = init_unit::implode(vec_notice_str);
				temp_player.get_chat_mgr().send_notice(notice_id, notice_str);
			}
		}
	}

	void player_arena_mgr::get_challenge_logs()
	{
		cs2dp_load_arena_log req;

		player& temp_player = unit_man::get_player(m_unit_index);

		if (temp_player.is_valid() == false)
		{
			return ;
		}
		req.role_guid = temp_player.get_unit_guid();
		req.array_index = temp_player.get_array_index();
		temp_player.send_message_to_dp( &req, sizeof(cs2dp_load_arena_log));
	}

	
	void player_arena_mgr::get_challenge_logs_end(std::vector<s_chanllenge_log_info>&  challenge_logs)
	{//此处发消息给客户端
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		if (challenge_logs.size() > 0)
		{
			arena_proto_req_get_challenge_log_end  log_msg;
			log_msg.set_role_guid(temp_player.get_unit_guid().server_64);

			//发个空的这玩意清理记录
			log_msg.set_is_need_clear(true);
			temp_player.send_message_to_self(&log_msg, e_msgindex_s2c_req_get_challenge_log_end);

			log_msg.set_is_need_clear(false);

			for (int32 i = 0; i < challenge_logs.size(); ++i)
			{
				arena_proto_challenge_log* temp_log = log_msg.add_my_challenge_logs();
				temp_log->set_role_guid(challenge_logs[i].role_guid.server_64);
				temp_log->set_target_guid(challenge_logs[i].target_guid.server_64);
				temp_log->set_role_name(challenge_logs[i].role_name);
				temp_log->set_target_name(challenge_logs[i].target_name);
				temp_log->set_role_rank(challenge_logs[i].role_rank);
				temp_log->set_target_rank(challenge_logs[i].target_rank);
				temp_log->set_initiative(challenge_logs[i].initiative);
				temp_log->set_result(challenge_logs[i].result);
				temp_log->set_date(challenge_logs[i].date);

				if (log_msg.ByteSize() > packege_length_max)
				{
					temp_player.send_message_to_self(&log_msg, e_msgindex_s2c_req_get_challenge_log_end);
					log_msg.clear_my_challenge_logs();
				}
			}
			if (log_msg.my_challenge_logs_size() > 0)
			{
				temp_player.send_message_to_self(&log_msg, e_msgindex_s2c_req_get_challenge_log_end);
			}
		}
	}

	void player_arena_mgr::add_challeng_log(const guid_64& challenge_role_guid, const guid_64& target_role_guid, bool initiative, int32 end_result, int32 role_rank, int32 target_rank)
	{
		player& temp_player = unit_man::get_player(m_unit_index);

		if (temp_player.is_valid() == false)
		{
			return;
		}

		const xchar* temp_target_name = get_current_challenge_player_name(target_role_guid);
		if (nullptr == temp_target_name)
		{
			return;
		}
		cs2dp_add_arena_log req;
		s_chanllenge_log_info	role_log,target_log;
		time_t now_time;
		time(&now_time);
		role_log.role_guid = challenge_role_guid;
		role_log.set_role_name(temp_player.get_name());
		role_log.role_rank = role_rank;
		role_log.target_guid = target_role_guid;
		role_log.set_target_name(temp_target_name);
		role_log.target_rank = target_rank;
		role_log.initiative = int32(initiative);
		role_log.result = end_result;
		role_log.date = now_time;
		req.log_data = role_log;
		temp_player.send_message_to_dp( &req, sizeof(cs2dp_add_arena_log));
	}


	void player_arena_mgr::req_get_match_player()
	{
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		cs2ws_req_choose_list req;
		req.role_guid = temp_player.get_unit_guid();
		req.cur_pos = m_current_rank;
		bool is_use = proto_by_lua(e_msg_index_cs2ws_req_challenge_list);
		if (is_use == false)
		{
			connection_mgr::getInstance().send_to_ws(&req, sizeof(cs2ws_req_choose_list));
		}
		else
		{
			faith::cs2ws_proto::req_choose_list pro_msg;
			req.to_proto(pro_msg);
			connection_mgr::getInstance().send_to_ws_lua(&pro_msg, e_msg_index_cs2ws_req_challenge_list);
		}
	}


	void player_arena_mgr::req_get_first_three_player()
	{
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		cs2ws_first_three req;
		req.role_guid = temp_player.get_unit_guid();

		bool is_use = proto_by_lua(e_msg_index_cs2ws_first_three);
		if (is_use == false)
		{
			connection_mgr::getInstance().send_to_ws(&req, sizeof(cs2ws_first_three));
		}
		else
		{
			faith::cs2ws_proto::first_three pro_msg;
			req.to_proto(pro_msg);
			connection_mgr::getInstance().send_to_ws_lua(&pro_msg, e_msg_index_cs2ws_first_three);
		}
		//connection_mgr::getInstance().send_to_ws( &req, sizeof(cs2ws_first_three));
	}

	

	
	void player_arena_mgr::req_match_player_end(const player_arena_rank* target_guids)
	{
		m_match_player.clear();
		for (int32 i = 0; i < 3; ++i)
		{
			m_match_player.push_back(target_guids[i]);
			add_map_element(target_guids[i].role_guid, target_guids[i].role_name);

		}

		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		arena_proto_get_match_player_end response;
		for (int32 i = 0; i < 3; ++i)
		{
			arena_proto_arena_player_info*  temp_player_info = response.add_match_playrs();
			temp_player_info->add_role_guid(target_guids[i].role_guid.A);
			temp_player_info->add_role_guid(target_guids[i].role_guid.B);
			temp_player_info->set_role_name(target_guids[i].role_name);
			temp_player_info->set_gs_value(init_unit::change_i64_to_string(target_guids[i].gs_value));
			temp_player_info->set_role_template_id(target_guids[i].role_template_id);
			temp_player_info->set_arena_ranking(m_match_player[i].rank_pos);
			temp_player_info->set_military_rank(m_match_player[i].military_level);
			temp_player_info->set_robot_template_id(m_match_player[i].robot_template_id);
		}

		temp_player.send_message_to_self(&response, e_msgindex_s2c_get_match_player_end);
	}
	void player_arena_mgr::lua_req_match_player_end(const char *data_ptr, int32 data_len)
	{

		faith::ws2cs_proto::req_choose_list msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_req_choose_list packet;

		packet.from_proto(msg);

		req_match_player_end(packet.target_player);
	}

	void player_arena_mgr::req_challenge(const guid_64& target_role_guid, bool is_cost_money, int32 target_cur_pos, int32 self_cur_pos)
	{
		int32 end_type = e_arena_match_success;

		arena_proto_req_challenge_end	resp;
		resp.add_target_guid(target_role_guid.A);
		resp.add_target_guid(target_role_guid.B);

		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		if (m_is_requiring)
		{
			return;
		}

		int32 vip_level = temp_player.get_vip_level();
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(vip_level);
		if (vip_template_ptr == nullptr
			|| GAMECONFIG->ArenaClearChallengeCdCost.size() < 2
			|| GAMECONFIG->ArenaPurchaseChallengeTimesCost.size() < 2)
		{
			resp.set_req_end(e_item_string_unkown);
			resp.set_new_pos(m_current_rank);
			temp_player.send_message_to_self(&resp, e_msgindex_s2c_req_challenge_end);
			return;
		}

		bool is_no_cd = vip_template_ptr->IsArenaChallengeNoCD > 0;

		int32 money_cost_type = GAMECONFIG->ArenaClearChallengeCdCost[0];
		//ArenaClearChallengeCdCost 和 ArenaClearChallengeCdCost 要花的钱ID应该是一样的，现在随便拿一个，以后改了再说
		int32 money_cost_value = 0;

		uint64 colling_time = temp_player.get_time_data(e_time_type_arena_colling_time);
		if (!is_no_cd
			&& colling_time > utility::get_tick_count())
		{//有CD并且仍在冷却中
			if (is_cost_money)
			{//“老子要花钱！”
				money_cost_value += GAMECONFIG->ArenaClearChallengeCdCost[1];
			}
			else
			{//讲真，应该到不了这，不花钱的话，客户端就拦下了
				resp.set_req_end(e_arena_challenge_is_colling);
				resp.set_new_pos(m_current_rank);
				temp_player.send_message_to_self(&resp, e_msgindex_s2c_req_challenge_end);
				return;
			}
		}

		if (temp_player.get_unit_info(e_role_info_arena_times) >= GAMECONFIG->ArenaTimes)
		{
			if (temp_player.get_unit_info(e_role_info_arena_vip_buy_times) < vip_template_ptr->ArenaBuyTimeLimit
				&& is_cost_money)
			{//能买，而且确实要买
				money_cost_value += GAMECONFIG->ArenaPurchaseChallengeTimesCost[1];
			}
			else
			{//讲真，应该到不了这，不花钱的话，客户端就拦下了
				resp.set_req_end(e_arena_challenge_times_not_enough);
				resp.set_new_pos(m_current_rank);
				temp_player.send_message_to_self(&resp, e_msgindex_s2c_req_challenge_end);
				return;
			}
		}

		if (money_cost_value > 0)
		{
			if (!temp_player.can_cut_money(e_money_type(money_cost_type), money_cost_value))
			{
				resp.set_req_end(e_item_string_jewel);
				resp.set_new_pos(m_current_rank);
				temp_player.send_message_to_self(&resp, e_msgindex_s2c_req_challenge_end);
				return;
			}
			temp_player.set_time_data(e_time_type_arena_colling_time, 0);
			temp_player.send_time_one(e_time_type_arena_colling_time);
		}

		cs2ws_req_challenge_msg req;
		req.role_guid = temp_player.get_unit_guid();
		req.target_guid = target_role_guid;
		req.cost_money_type = money_cost_type;
		req.cost_money_num = money_cost_value;
		req.target_cur_pos = target_cur_pos;
		req.self_cur_pos = self_cur_pos;
		bool is_use = proto_by_lua(e_msg_index_cs2ws_req_challenge);
		if (is_use == false)
		{
			connection_mgr::getInstance().send_to_ws(&req, sizeof(cs2ws_req_challenge_msg));
		}
		else
		{
			faith::cs2ws_proto::req_challenge_msg pro_msg;
			req.to_proto(pro_msg);
			connection_mgr::getInstance().send_to_ws_lua(&pro_msg, e_msg_index_cs2ws_req_challenge);
		}

		server_log::arena_challenge_log(temp_player.get_third_info(), temp_player.get_unit_info_inst());
		

		set_is_requiring(true);
	}


	void player_arena_mgr::do_challenge(guid_64 target_role_guid, int32 robot_id, int32 req_result
		, int32 cost_money_type, int32 cost_money_num
		, int32 target_cur_pos, int32 self_cur_pos)
	{
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		if (req_result == e_arena_match_success)
		{
			if (cost_money_num > 0
				&& !temp_player.can_cut_money(e_money_type(cost_money_type), cost_money_num))
			{//这里钱必须够
				set_is_requiring(false);
				return;
			}

			VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(temp_player.get_vip_level());
			if (vip_template_ptr == nullptr)
			{
				set_is_requiring(false);
				return;
			}
			int32 cur_free_times = temp_player.get_unit_info(e_role_info_arena_times);
			int32 cur_vip_purchase_times = temp_player.get_unit_info(e_role_info_arena_vip_buy_times);
			if (cur_free_times >= GAMECONFIG->ArenaTimes)
			{
				if (cur_vip_purchase_times >= vip_template_ptr->ArenaBuyTimeLimit)
				{
					//这里必须能买或者有免费
					set_is_requiring(false);
					return;
				}
				else
				{
					temp_player.set_unit_info(e_role_info_arena_vip_buy_times, cur_vip_purchase_times + 1);
					temp_player.send_info_one(e_role_info_arena_vip_buy_times);
					temp_player.get_mission_mgr().target_check(e_mission_end_type_clear_arena, 1);
				}
			}
			else
			{
				temp_player.set_unit_info(e_role_info_arena_times, cur_free_times + 1);
				temp_player.send_info_one(e_role_info_arena_times);

				temp_player.get_mission_mgr().target_check(e_mission_end_type_clear_arena,1);
				
			}

			temp_player.cut_money(e_money_type(cost_money_type), cost_money_num, e_server_log_cut_money_arena_challenge);


			//加载npc
			teleport_control& tc = temp_player.get_transfer_control();
			tc.set_dynamic_param_int(0, target_role_guid.A);
			tc.set_dynamic_param_int(1, target_role_guid.B);
			tc.set_dynamic_param_int(2, robot_id);
			tc.set_dynamic_param_int(3, target_cur_pos);
			tc.set_dynamic_param_int(4, self_cur_pos);
		}
		else
		{
			set_is_requiring(false);
		}

		arena_proto_req_challenge_end resp;
		resp.add_target_guid(target_role_guid.A);
		resp.add_target_guid(target_role_guid.B);
		resp.set_req_end(req_result);
		resp.set_new_pos(self_cur_pos);
		temp_player.send_message_to_self(&resp, e_msgindex_s2c_req_challenge_end);
	}

	void player_arena_mgr::get_reward()
	{
		//提示信息都在客户端做了，这不用
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		int64 cur_time = utility::get_tick_count();
		if (temp_player.get_time_data(e_time_type_arena_last_get_reward_time) > cur_time)
		{
			return;
		}
		const ArenaRankRewardTemplate* reward_template_ptr = template_manager::get_instance().get_arena_reward_by_arena_pos(m_current_rank);
		if (reward_template_ptr == nullptr)
		{
			return;
		}

		int32 money_reward_num = reward_template_ptr->MoneyRewardArray.size() / 2;
		//获取奖励的提示的 参数
		std::vector<s_item_template_info> item_tuple_array;

		int32 item_reward_num = reward_template_ptr->ItemRewardArray.size() / 2;
		std::vector<citem*> item_array;
		item_array.reserve(item_reward_num);
		std::vector<s_item_template_info> item_create_info_array;
		item_create_info_array.reserve(item_reward_num);
		int32 empty_slot_req_num = 0;
		int32 element_empty_slot_req_num = 0;
		for (int32 i = 0; i < item_reward_num; ++i)
		{
			int32 item_id = reward_template_ptr->ItemRewardArray[i * 2];
			int32 item_num = reward_template_ptr->ItemRewardArray[i * 2 + 1];
			ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
			if(item_template_ptr == nullptr)
			{
				continue;
			}
			int32 max_pile_num = item_template_ptr->max_pile_num;
			if (max_pile_num <= 0)
			{
				max_pile_num = 1;
			}
			int32 slot_need_num = item_num / max_pile_num;
			if (item_num % max_pile_num != 0)
			{
				slot_need_num += 1;
			}
			if (item_template_ptr->item_type == e_item_type_element_heart)
			{
				element_empty_slot_req_num += slot_need_num;
			}
			else
			{
				empty_slot_req_num += slot_need_num;
			}

			item_create_info_array.push_back(s_item_template_info(item_id, item_num, 1));


			item_tuple_array.push_back(item_id);
			item_tuple_array.push_back(item_num);
			item_tuple_array.push_back(1);
		}

		//if (!temp_player.get_item_set().is_empty_slot_enough(e_bag_type_bag, empty_slot_req_num)
		//	|| !temp_player.get_item_set().is_empty_slot_enough(e_bag_type_elemenet, element_empty_slot_req_num))
		//{
		//	arena_proto_get_arena_reward_end msg;
		//	msg.set_reward_template_id(reward_template_ptr->attribute_id);
		//	msg.set_get_result(e_element_heart_bag_full);
		//	temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_arena_reward_end);
		//	return;
		//}

		//item_array = temp_player.get_item_set().create_items(e_server_log_add_item_arena_reward, item_reward_num, item_create_info_array);

		//for (int32 i = 0; i < money_reward_num; ++i)
		//{
		//	int32 money_type = reward_template_ptr->MoneyRewardArray[i * 2];
		//	int32 money_num = reward_template_ptr->MoneyRewardArray[i * 2 + 1];

		//	money_num = get_vip_add(money_num);

		//	temp_player.add_money_or_exp(e_money_type(money_type), money_num, e_server_log_add_money_arena_rank, reward_template_ptr->attribute_id);

		//	item_tuple_array.push_back({ money_type ,money_num });
		//}

		//temp_player.get_item_set().put_in_bag(item_array, e_bag_type_none, e_item_mail_type_arena_reward);
		//temp_player.get_item_set().get_item_send_promp_msg_to_client(item_tuple_array);

		//到下次领奖CD
		int32 cur_today_time = time_helper::get_today_time_in_sec();
		if (cur_today_time < GAMECONFIG->ArenaGetRewardTime)
		{
			temp_player.set_time_data(e_time_type_arena_last_get_reward_time, init_unit::get_end_time(GAMECONFIG->ArenaGetRewardTime - cur_today_time));
			temp_player.send_time_one(e_time_type_arena_last_get_reward_time);
		}
		else
		{
			int32 to_next_get_time = time_helper::get_time_by_today_stamp_new(day_time_second - 1).second - time_helper::get_cur_time_new().second;
			to_next_get_time += GAMECONFIG->ArenaGetRewardTime;
			temp_player.set_time_data(e_time_type_arena_last_get_reward_time, init_unit::get_end_time(to_next_get_time));
			temp_player.send_time_one(e_time_type_arena_last_get_reward_time);
		}

		if (temp_player.is_need_daily_finish(e_daily_must_do_typ_arena) > 0)
		{
			temp_player.set_must_do_count(e_daily_must_do_typ_arena, temp_player.get_must_do_count(e_daily_must_do_typ_arena) + 1);
			temp_player.send_daily_must_do_count_info_one(e_daily_must_do_typ_arena);
			temp_player.set_daily_active_degree_info(e_daily_must_do_typ_arena);
		}



		arena_proto_get_arena_reward_end msg;
		msg.set_reward_template_id(reward_template_ptr->attribute_id);
		msg.set_get_result(e_item_string_succeed);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_arena_reward_end);
	}

	int64 player_arena_mgr::get_vip_add(int64 value)
	{
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return value;
		}
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(temp_player.get_vip_level());
		if (vip_template_ptr == nullptr)
		{
			return value;
		}
		if (vip_template_ptr->ArenaBuyTimeLimit <= 0)
		{
			return value;
		}

		int64  result = value *(100 + vip_template_ptr->ArenaBuyTimeLimit) / 100;

		return result;

	}

	void player_arena_mgr::mopping_remain_times()
	{
		return;
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		if (GAMECONFIG->ArenaMoppingSingleCost.size() < 2)
		{
			return;
		}
		PlayerUpgradeTemplate* upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, temp_player.get_unit_info(e_role_info_upgrade_id));
		if (upgrade_template_ptr == nullptr)
		{
			return;
		}
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(temp_player.get_vip_level(true));
		if (vip_template_ptr == nullptr)
		{
			return;
		}
		arena_proto_req_mopping_remain_times_end msg;
		int32 remain_times = 0;
		//if (temp_player.get_unit_info(e_role_info_arena_times) < GAMECONFIG->ArenaTimes)
		//{
		//	remain_times = GAMECONFIG->ArenaTimes - temp_player.get_unit_info(e_role_info_arena_times);
		//}
		//else
		{
			remain_times = vip_template_ptr->ArenaBuyTimeLimit - temp_player.get_unit_info(e_role_info_arena_vip_buy_times);
		}

		if (remain_times <= 0)
		{
			msg.set_mopping_result(e_arena_mopping_no_times);
			temp_player.send_message_to_self(&msg, e_msgindex_s2c_req_mopping_remain_times_end);
			return;
		}

		int32 money_cost_id = GAMECONFIG->ArenaMoppingSingleCost[0];
		int32 money_cost_num = GAMECONFIG->ArenaMoppingSingleCost[1] * remain_times;
		if (!temp_player.can_cut_money(e_money_type(money_cost_id), money_cost_num))
		{
			return;
		}

		temp_player.cut_money(e_money_type(money_cost_id), money_cost_num, e_server_log_cut_money_mopping_arena);

		int64 m_exp_get_value = upgrade_template_ptr->BaseExp * GAMECONFIG->ArenaCommonRewardFactor * remain_times;
		int64 m_money_get_value = upgrade_template_ptr->BaseCoin * GAMECONFIG->ArenaCommonRewardFactor * remain_times;
		int64 m_reputation_get_value = GAMECONFIG->ArenaReputationReward * remain_times;
		temp_player.add_money_or_exp(e_money_type_exp, m_exp_get_value, e_server_log_add_money_arena_mopping);
		temp_player.add_money_or_exp(e_money_type_silver_bind, m_money_get_value, e_server_log_add_money_arena_mopping);
		temp_player.add_money_or_exp(e_money_type_reputation, m_reputation_get_value, e_server_log_add_money_arena_mopping);

	/*	if (temp_player.get_unit_info(e_role_info_arena_times) >= GAMECONFIG->ArenaTimes)*/
		{
			temp_player.set_unit_info(e_role_info_arena_vip_buy_times, vip_template_ptr->ArenaBuyTimeLimit);
			temp_player.send_info_one(e_role_info_arena_vip_buy_times);
			int32 need_finish_count = temp_player.is_need_daily_finish(e_daily_must_do_typ_arena);
			if (need_finish_count > 0)
			{
				int32 add_count = remain_times < need_finish_count ? remain_times : need_finish_count;
				temp_player.set_must_do_count(e_daily_must_do_typ_arena, temp_player.get_must_do_count(e_daily_must_do_typ_arena) + add_count);
				temp_player.send_daily_must_do_count_info_one(e_daily_must_do_typ_arena);
				temp_player.set_daily_active_degree_info(e_daily_must_do_typ_arena);

			}
			temp_player.get_mission_mgr().target_check(e_mission_end_type_clear_arena, remain_times);
		
		}
	/*	else
		{
			temp_player.set_unit_info(e_role_info_arena_times, GAMECONFIG->ArenaTimes);
			temp_player.set_must_do_count(e_daily_must_do_typ_arena, GAMECONFIG->ArenaTimes);
			temp_player.send_daily_must_do_count_info_one(e_daily_must_do_typ_arena);
			temp_player.set_daily_active_degree_info(e_daily_must_do_typ_arena);

			temp_player.get_mission_mgr().target_check(e_mission_end_type_clear_arena, remain_times);
			temp_player.send_info_one(e_role_info_arena_times);
		}*/
	
		msg.set_mopping_result(0);

		msg.add_mopping_reward(e_money_type_exp);
		msg.add_mopping_reward(m_exp_get_value);

		msg.add_mopping_reward(e_money_type_silver_bind);
		msg.add_mopping_reward(m_money_get_value);

		msg.add_mopping_reward(e_money_type_reputation);
		msg.add_mopping_reward(m_reputation_get_value);

		temp_player.send_message_to_self(&msg, e_msgindex_s2c_req_mopping_remain_times_end);
	}

	void player_arena_mgr::set_is_requiring(bool is_requiring)
	{
		m_is_requiring = is_requiring;
		if (is_requiring)
		{
			m_requiring_safty_timer = init_unit::get_end_time(2.0f);
		}
		else
		{
			m_requiring_safty_timer = 0;
		}
	}

	void player_arena_mgr::sync_gs_value(int64 gs_value)
	{
		if (m_current_rank < 0
			|| m_current_rank >= rank_max)
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		cs2ws_arena_sync_gs_value req;
		req.role_guid = temp_player.get_unit_guid();
		req.cur_pos = m_current_rank;
		req.gs_value = gs_value;
		bool is_use = proto_by_lua(e_msg_index_cs2ws_arena_sync_gs_value);
		if (is_use == false)
		{
			connection_mgr::getInstance().send_to_ws(&req, sizeof(cs2ws_arena_sync_gs_value));
		}
		else
		{
			faith::cs2ws_proto::arena_sync_gs_value pro_msg;
			req.to_proto(pro_msg);
			connection_mgr::getInstance().send_to_ws_lua(&pro_msg, e_msg_index_cs2ws_arena_sync_gs_value);
		}
	}
	
	const xchar* player_arena_mgr::get_current_challenge_player_name(const guid_64& target_guid)
	{
		//for (uint32 i = 0; i < m_match_player.size(); ++i)
		//{
		//	if (target_guid == m_match_player[i].role_guid)
		//	{
		//		return m_match_player[i].role_name;
		//	}
		//}
		//return nullptr;
		return get_name(target_guid);
	}

	bool player_arena_mgr::lua_req_challenge_to_ws(int32 target_guid_a, int32 target_guid_b, int32 money_cost_type, int32 money_cost_value, int32 target_cur_pos, int32 self_cur_pos)
	{
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return false;
		}
		guid_64 target_role_guid(target_guid_a, target_guid_b);

		cs2ws_req_challenge_msg req;
		req.role_guid = temp_player.get_unit_guid();
		req.target_guid = target_role_guid;
		req.cost_money_type = money_cost_type;
		req.cost_money_num =  money_cost_value;
		req.target_cur_pos = target_cur_pos;
		req.self_cur_pos = self_cur_pos;
		bool is_use = proto_by_lua(e_msg_index_cs2ws_req_challenge);
		if (is_use == false)
		{
			connection_mgr::getInstance().send_to_ws(&req, sizeof(cs2ws_req_challenge_msg));
		}
		else
		{
			faith::cs2ws_proto::req_challenge_msg pro_msg;
			req.to_proto(pro_msg);
			connection_mgr::getInstance().send_to_ws_lua(&pro_msg, e_msg_index_cs2ws_req_challenge);
		}
		//connection_mgr::getInstance().send_to_ws(&req, sizeof(cs2ws_req_challenge_msg));

		return true;
	}
	bool player_arena_mgr::lua_set_reward_time()
	{
		player& temp_player = unit_man::get_player(m_unit_index);
		if (temp_player.is_valid() == false)
		{
			return false;
		}

		int32 cur_today_time = time_helper::get_today_time_in_sec();
		if (cur_today_time < GAMECONFIG->ArenaGetRewardTime)
		{
			temp_player.set_time_data(e_time_type_arena_last_get_reward_time, init_unit::get_end_time(GAMECONFIG->ArenaGetRewardTime - cur_today_time));
			temp_player.send_time_one(e_time_type_arena_last_get_reward_time);
		}
		else
		{
			int32 to_next_get_time = time_helper::get_time_by_today_stamp_new(day_time_second - 1).second - time_helper::get_cur_time_new().second;
			to_next_get_time += GAMECONFIG->ArenaGetRewardTime;
			temp_player.set_time_data(e_time_type_arena_last_get_reward_time, init_unit::get_end_time(to_next_get_time));
			temp_player.send_time_one(e_time_type_arena_last_get_reward_time);
		}
		return true;

	}


	void player_arena_mgr::add_map_element(const guid_64& role_guid, xstring name)
	{
		if (role_guid.is_valid() == false || name == "")
		{
			return;
		}
		name_map[role_guid] = name;
	}
	const char *  player_arena_mgr::get_name(const guid_64& role_guid)
	{
		challenge_name_map::iterator it = name_map.find(role_guid);
		if (it == name_map.end())
		{
			return nullptr;
		}
		return  it->second.c_str();
	}

}