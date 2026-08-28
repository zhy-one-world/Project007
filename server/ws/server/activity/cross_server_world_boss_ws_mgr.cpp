/********************************************************************
created: 2019年5月24日
file base: cross_server_world_boss_ws_mgr
file ext: cpp
author: zhaoyuming
purpose:
*********************************************************************/
#include "cross_server_world_boss_ws_mgr.h"
#include "world_server.hpp"
#include "template/template_manager.h"
#include "cross_group/cross_transfer_logic.hpp"
#include "Logic/activity_def.hpp"
#include "utility/init_unit.h"
#include "../mail/event_ws_mgr.h"
#include "../ranking/ranking_mgr_ws.h"
#include "globle_data.h"
#include "system/scene/cs_map_mgr_system.h"
#include "system/scene/cs_map_system.h"
#include "server/legion/legion_ws_mgr.h"
#include "components/scene/cs_map_component.h"
#include "ws_client.hpp"
#include "net.pb.h"
#include "game.pb.h"

namespace hld
{
	cross_server_world_boss_ws_mgr::cross_server_world_boss_ws_mgr()
	{
		clear_data();

		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			m_boss_kill_all_info[i].clear_data();
		}
		for (int32 i = 0; i < max_server_num; ++i)
		{
			m_boss_level[i].clear_data();
		}
	}

	void cross_server_world_boss_ws_mgr::tick(int64 time_new)
	{
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss))
		{
			if (true == is_in_game_ready_time() && false == is_have_world_boss_map() && true == world_server::getInstance().is_sky_island_server())
			{
				init_cross_server_world_boss_map();
			}
			if (is_in_game_ready_time() && m_ready_notice)
			{
				int32 notice_id = cross_boss_activity_ready_notice_id;
				std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
				int32 create_time = time_helper::get_cur_time_new().second;
				event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
				m_game_notice = true;
				m_ready_notice = false;
				m_all_boss_damage_list.clear_data();
				m_is_send_legion_welfare = true;
				for (int32 i = 0; i < m_boss_max_num; ++i)
				{
					m_boss_kill_all_info[i].clear_data();
					m_boss_kill_all_info[i].boss_template_id =get_world_boss_id_with_type(i);
					m_boss_damage_list[i].clear_data();
					ranking_mgr_ws::clear_data_by_ranking_type(e_RankingIndex(e_RankingIndex_cross_boss_one+i));
				}
			}
			if (is_in_game_time() && m_game_notice)
			{
				int32 notice_id = cross_boss_activity_begin_notice_id;
				std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
				int32 create_time = time_helper::get_cur_time_new().second;
				event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
				m_game_notice = false;
				m_ready_notice = true;
			}
			
			if (is_in_game_all_time() == false && m_is_send_legion_welfare)
			{
				send_legion_welfare_rank();
				m_all_boss_damage_list.clear_data();
				m_is_send_legion_welfare = false;
			}
		}
	}

	void cross_server_world_boss_ws_mgr::clear_data()
	{
		m_is_need_clear = false;
		m_ready_notice = true;
		m_game_notice = false;
		m_game_over_flag = false;
		m_boss_level_true = 0;
		m_boss_level_false = 0;
		m_all_boss_damage_list.clear_data();
		m_is_send_legion_welfare = false;
		for (int32 i = 0; i < map_max; i++)
		{
			m_game_over_flag = false;
			m_is_cur_map_end[i] = false;
			m_boss_damage_list[i].clear_data();			//boss伤害列表
			m_map_state[i] = e_map_state_max;
			m_cross_world_boss_map_id[i] = 0;
		}
		for (int32 i = 0; i < m_boss_max_num; i++)
		{
			m_boss_all_damage[i] = 0;
			m_boss_id[i] = 0;
		}
		for (int32 i = 0; i < max_server_num; ++i)
		{
			m_boss_level[i].clear_data();
		}
	}

	void cross_server_world_boss_ws_mgr::init_manager()
	{
		send_get_boss_show_info_all();
	}

	//创建地图
	void cross_server_world_boss_ws_mgr::init_cross_server_world_boss_map()
	{
		for (int32 i = 0; i < map_max; i++)
		{
			auto map_ent = cs_map_mgr_system::init_map_by_battle(get_cross_server_world_boss_map_temp_id(i));
			if (map_ent)
			{
				m_cross_world_boss_map_id[i] = map_ent->getEntityId();
			}
		}
		get_with_save_server_level();	//获得其他服务器的平均等级
	}

	//判断玩家是否可以进行跨服并传送到活动地图
	e_error_code cross_server_world_boss_ws_mgr::is_can_join_with_cross(guid_64 role_guid, int32 map_type)
	{
		client_session* cur_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == cur_session_ptr)
		{
			return e_error_code_map_cross_server_pk_session_error;
		}
		if (is_in_game_ready_time() == false && is_in_game_all_time() == false)
		{
			cur_session_ptr->send_notice("90090360");  //跨服世界boss活动未开启
			return e_error_code_map_cross_server_world_boss_begin;
		}
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss))
		{
			cur_session_ptr->send_notice("90305015");//跨服世界boss活动还没有开放
			return e_error_code_map_cross_server_world_boss_not_start;
		}
		int32 map_id = get_cross_server_world_boss_map_temp_id(map_type);
		MapTemplate* map_ptr = GET_TEMPLATE(MapTemplate, map_id);
		if (map_ptr == nullptr || map_ptr->EnterLevel > cur_session_ptr->get_role_info_data(e_role_info_exp_level))
		{
			cur_session_ptr->send_notice("90090228");//等级不足以进入活动地图
			return e_error_code_map_max_players;
		}
		return e_error_code_success;
	}

	//判断地图是否能传送
	e_error_code cross_server_world_boss_ws_mgr::is_can_join_with_cross_gate(int32 map_type)
	{
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss))
		{
			return e_error_code_map_cross_server_world_boss_begin;
		}
		if (is_in_game_ready_time() == false && is_in_game_time() == false)
		{
			return e_error_code_map_cross_server_world_boss_begin;
		}

		int32 war_idex = 0;
		guid_64 map_guid = guid_64();
		get_world_boss_map_guid(map_guid, war_idex, map_type);

		if (cs_map_system::is_full(map_guid))
		{
			return e_error_code_map_max_players;
		}
		if (true == m_game_over_flag)
		{
			return e_error_code_map_cross_server_world_boss_end;
		}
		return e_error_code_success;
	}

	//判断是否可以跨服并传送到活动地图
	void cross_server_world_boss_ws_mgr::recv_other_ws_transger_gate_map(guid_64 role_guid, int32 server_id, int32 map_type)
	{
		e_error_code ret = is_can_join_with_cross_gate(map_type);
		int32 error_ret = ret;
		int32 war_idex = 0;
		int32 map_temp_id = get_cross_server_world_boss_map_temp_id(map_type);
		if (map_temp_id == -1)
		{
			return;
		}
		guid_64 map_guid = guid_64();
		get_world_boss_map_guid(map_guid, war_idex, map_type);
		if (!map_guid.is_valid())
		{
			return;
		}
		ws2ws_transfer_cross_world_boss_map_result msg;
		msg.role_guid = role_guid;
		msg.result = error_ret;
		msg.map_guid = map_guid;
		msg.map_temp_id = map_temp_id;
		msg.war_idex = war_idex;
		msg.server_id = world_server::getInstance().get_cross_id();
		msg.activity_type = (int32)e_activity_type_cross_server_world_boss;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_cross_world_boss_map_result, &msg, sizeof(msg));
	}

	//跨服并传送到活动地图
	void cross_server_world_boss_ws_mgr::transfer_to_map_with_gate_msg(guid_64 role_guid, int32 error_ret, int32 map_temp_id, guid_64 map_guid, int32 war_idex, int32 server_id)
	{
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			return;
		}
		e_error_code ret = (e_error_code)error_ret;
		std::string notice_id = "";
		switch (ret)
		{
		case hld::e_error_code_success:
			break;
		case hld::e_error_code_map_init_map_err:
			notice_id = "90090579";
			break;
		case hld::e_error_code_map_cross_server_world_boss_begin:
			notice_id = "90090360";//跨服世界boss活动未开始
			break;
		case hld::e_error_code_map_cross_server_world_boss_end:
			notice_id = "90090360";//跨服世界boss活动已结束
			break;
		case hld::e_error_code_map_cross_server_world_boss_not_start:
			notice_id = "90305015";//跨服世界boss活动未开启
			break;
		default:
			break;
		}
		if (ret != e_error_code_success)
		{
			if (notice_id != "")
			{
				session_ptr->send_notice(notice_id);
				return;
			}
			cs_map_mgr_system::send_map_error_to_client(session_ptr, ret);
			return;
		}
		else if (map_temp_id > 0)
		{
			if (server_id <= 0)
			{
				server_id = world_server::getInstance().get_cross_id();
			}
			cs_map_mgr_system::transfer_to_map(session_ptr, map_temp_id, map_guid, war_idex, server_id);
		}
	}


	//获得活动地图的guid
	void cross_server_world_boss_ws_mgr::get_world_boss_map_guid(guid_64 & map_guid, int32 & war_idex, int32 map_type)
	{
		map_guid = guid_64();

		if (m_cross_world_boss_map_id[map_type].is_valid())
		{
			auto map_ent = get_entity(m_cross_world_boss_map_id[map_type]);
			if (map_ent)
			{
				map_guid = map_ent->getEntityId();
				war_idex = 0;
			}
		}
	}

	//根据数字获得活动地图的模板id
	int32 cross_server_world_boss_ws_mgr::get_cross_server_world_boss_map_temp_id(int32 map_type)
	{
		if (map_type >= map_max || map_type < 0)
		{
			return -1;
		}
		ActivityCommonConfigTemplate* cross_server_boss_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_world_boss);
		if (nullptr == cross_server_boss_config_ptr)
		{
			return -1;
		}
		if (cross_server_boss_config_ptr->ParamIntArr4.size() % cross_boss_map_template_num != 0 || cross_server_boss_config_ptr->ParamIntArr4.size() / cross_boss_map_template_num < map_max)
		{
			return -1;
		}
		return cross_server_boss_config_ptr->ParamIntArr4[map_type * cross_boss_map_template_num];
		
	}

	int32 cross_server_world_boss_ws_mgr::get_world_boss_id_with_type(int32 idex)
	{
		if (idex >= m_boss_max_num || idex < 0)
		{
			return -1;
		}
		ActivityCommonConfigTemplate* m_world_boss_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_world_boss);
		if (nullptr == m_world_boss_config_ptr)
		{
			return -1;
		}
		if (m_world_boss_config_ptr->ParamIntArr4.size() % (m_map_max_boss + 1) != 0 || (m_world_boss_config_ptr->ParamIntArr4.size() / (m_map_max_boss + 1)) < map_max)
		{
			return -1;
		}
		for (int32 i = 0; i < map_max; ++i)
		{
			for (int32 o = 0; o < m_map_max_boss; o++)
			{
				m_boss_id[i * 3 + o] = m_world_boss_config_ptr->ParamIntArr4[i * 4 + o + 1];
			}

		}
		return m_boss_id[idex];
	}

	bool cross_server_world_boss_ws_mgr::is_have_live_boss()
	{
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (m_boss_kill_all_info[i].is_dead == false)
			{
				return true;
			}
		}
		return false;
	}

	void cross_server_world_boss_ws_mgr::send_legion_welfare_rank()
	{
		ws2ws_send_legion_welfare_rank msg;
		m_all_boss_damage_list.sort();
		std::vector<score_record> record_score = m_all_boss_damage_list.get_score_record_list_all();
		int32 rank_index = 1;
		for (int32 idx = 0; idx < record_score.size(); idx++)
		{
			msg.legion_guid = record_score[idx].role_guid;
			msg.rank_index = rank_index;
			cross::send_msg_to_ws(guid_64(), record_score[idx].server_id, e_msg_ws2ws_send_legion_welfare_rank, &msg, sizeof(msg));
			++rank_index;
		}
	}

	//发送所有boss被击杀信息
	void cross_server_world_boss_ws_mgr::send_get_boss_show_info_all()
	{
		ws2dp_load_cross_server_world_boss_msg_all msg;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	//是否在活动时间内
	bool cross_server_world_boss_ws_mgr::is_in_game_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_world_boss, e_activity_time_get_gaming);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}

	//是否在活动全部时间内
	bool cross_server_world_boss_ws_mgr::is_in_game_all_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_world_boss, e_activity_time_get_all);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}

	//是否在活动的准备时间内
	bool cross_server_world_boss_ws_mgr::is_in_game_ready_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_world_boss, e_activity_time_get_ready);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}

	//活动地图是否存在
	bool cross_server_world_boss_ws_mgr::is_have_world_boss_map()
	{
		for (int i = 0; i < map_max; i++)
		{
			if (m_cross_world_boss_map_id[i].is_valid() == false)
			{
				return false;
			}
			auto map_ws_ptr = get_entity(m_cross_world_boss_map_id[i]);
			if (nullptr == map_ws_ptr)
			{
				return false;
			}
		}
		return true;
	}

	bool cross_server_world_boss_ws_mgr::is_in_cross_boss_map(guid_64 map_guid)
	{
		for (int32 i = 0; i < map_max; ++i)
		{
			if (map_guid == m_cross_world_boss_map_id[i])
			{
				return true;
			}
		}
		return false;
	}

	//发送boss死亡给所有地图中的玩家
	void cross_server_world_boss_ws_mgr::sync_kill_boss_to_all_map_inst(int32 map_template_id, int32 killer_unit_index, int32 be_kill_boss_guid, guid_64 killer_guid)
	{
		cross_boss_on_dead(killer_guid, be_kill_boss_guid);
	}

	//boss死亡要处理的信息
	void cross_server_world_boss_ws_mgr::cross_boss_on_dead(guid_64 kill_boss_play_guid, int32 be_kill_boss_id)
	{
		client_session* player_session = client_session_mgr::getInstance().get_session(kill_boss_play_guid);
		if (player_session == nullptr)
		{
			return;
		}
		ws2ws_cross_refresh_kill_boss_show_info msg;
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (m_boss_kill_all_info[i].boss_template_id == be_kill_boss_id || m_boss_kill_all_info[i].boss_template_id == 0)
			{
				m_boss_kill_all_info[i].boss_template_id = be_kill_boss_id;
				m_boss_kill_all_info[i].is_dead = true;
				m_boss_kill_all_info[i].boss_be_kill_time = time_helper::get_cur_time_new().second;
				m_boss_kill_all_info[i].set_play_name(player_session->get_role_name());
				m_boss_kill_all_info[i].kill_boss_server_id = player_session->get_role_info_data(e_role_info_server_id);
				m_boss_kill_all_info[i].kill_boss_play_id = player_session->get_role_guid();
				m_boss_kill_all_info[i].kill_boss_legion_id = player_session->get_legion_guid();
				if (player_session->get_legion_guid().is_valid())
				{
					m_boss_kill_all_info[i].set_legion_name(player_session->get_legion_info().legion_name);
				}
				msg.boss_info = m_boss_kill_all_info[i];
				break;
			}	
		}
		cross::send_msg_to_ws(guid_64(), 0, e_msgindex_ws2ws_refresh_kill_boss_show_info, &msg, sizeof(msg));
		save_boss_info_to_dp(be_kill_boss_id);//保存boss死亡信息到数据库
	}

	//给军团伤害排行中的军团发奖励
	void cross_server_world_boss_ws_mgr::make_cross_world_boss_legion_award(guid_64 legion_guid, int32 boss_id, int32 rank_num, guid_64* play_guid_list, int32 play_num)
	{
	if (!legion_guid.is_valid() || play_guid_list== nullptr || play_num <= 0)
		{
			return;
		}
		legion_ws* rank_legion = legion_ws_mgr::get_instance().get_legion(legion_guid);
		NpcTemplate* _npc_template_ptr = GET_TEMPLATE(NpcTemplate, boss_id);
		ActivityCommonConfigTemplate* cross_server_boss_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_world_boss);
		if (nullptr == cross_server_boss_config_ptr || nullptr == _npc_template_ptr || nullptr == rank_legion)
		{
			return;
		}
		//rank_legion->send_legion_welfare_by_npc(boss_id, rank_num);
		std::vector<s_item_template_info> _item_list;
		std::vector<int32>& rwd_data = _npc_template_ptr->RankRewards;
		globle_data::get_instance().get_item_list_by_rank_rwd(rank_num, rwd_data, _item_list, ranking_mgr_ws::get_world_level_last());
		for (auto& it : _item_list)
		{
			rank_legion->give_activity_reward(cross_server_boss_config_ptr, play_guid_list, play_num, it.m_item_id, it.m_item_num);
		}
	}

	//发送军团伤害排名
	void cross_server_world_boss_ws_mgr::send_legion_act_rank(const cs2ws_make_cross_server_world_legion_award& act_rank)
	{
		int32 boss_id = act_rank.boss_id;
		guid_64 kill_boss_play_guid = act_rank.kill_boss_play_guid;
		int32 boss_index = -1;
		for (int32 i = 0; i < m_boss_max_num; i++)
		{
			if (boss_id == get_world_boss_id_with_type(i))
			{
				boss_index = i;
				break;
			}
		}
		if (boss_index < 0)
		{
			return;
		}
		std::vector<score_record> record_score;
		record_score.clear();
		for (int32 i = 0; i < act_rank.score_num; i++)
		{
			if (!act_rank.legion_score_rank[i].role_guid.is_valid())
			{
				continue;
			}
			record_score.push_back(act_rank.legion_score_rank[i]);
			m_boss_damage_list[boss_index].add_score_record_by_list(act_rank.legion_score_rank[i]);	
			if (m_is_send_legion_welfare)
			{
				m_all_boss_damage_list.add_score_record_by_list(act_rank.legion_score_rank[i]);
			}
		}
		m_boss_damage_list[boss_index].sort();
		send_cross_boss_ranking_list(boss_id);
		std::map<guid_64, activity_legion_play_map> _legion_rank_mems_all;
		_legion_rank_mems_all.clear();

		for (int32 i = 0; i <act_rank.member_num; ++i)
		{
			const activity_legion_play& mem_score_info = act_rank.member_scores[i];
			const guid_64& legion_guid = mem_score_info.legion_guid;
			if (false == legion_guid.is_valid())
			{
				continue;
			}
			activity_legion_play_map& _legion_rank_mems = _legion_rank_mems_all[legion_guid];
			_legion_rank_mems[mem_score_info.play_guid] = mem_score_info;
		}

		int32 rank_num = 1;//排名
		int32 act_legion_num = legion_act_num;//需要发送奖励的军团数量
		if (record_score.size() < legion_act_num)
		{
			act_legion_num = record_score.size();
		}
		client_session* play_ptr = client_session_mgr::getInstance().get_session(kill_boss_play_guid);
		ws2ws_make_cross_server_world_legion_award msg;
		for (int32 i = 0; i < act_legion_num; i++)
		{	
			msg.boss_tem_id = boss_id;
			activity_legion_play_map _attend_mems_map;
			_attend_mems_map.clear();
			score_record* _record_score = m_boss_damage_list[boss_index].get_score_record_at(i);
			if (_record_score == nullptr)
			{
				continue;
			}
			auto ita = _legion_rank_mems_all.find(_record_score->role_guid);
			if (ita == _legion_rank_mems_all.end())
			{
				continue;
			}
			else
			{
				_attend_mems_map = ita->second;
			}
			msg.legion_guid = _record_score->role_guid;
			msg.score_rank = i + 1;
			msg.play_num = 0;
			for (auto ite = _attend_mems_map.begin(); ite != _attend_mems_map.end(); ++ite)
			{
				if (msg.play_num >= max_hurt_monster_player_num)
				{
					break;
				}
				activity_legion_play member_attend = ite->second;
				msg.play_guid_list[msg.play_num] = member_attend.play_guid;
				++msg.play_num;
			}
			int32 server_id = _record_score->server_id;
			if (server_id <= 0 || server_id == world_server::getInstance().get_cross_id())
			{
				continue;
			}
			cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_make_cross_server_world_legion_award, &msg, sizeof(msg));
			//发放击杀boss军团的奖励
			if (nullptr != play_ptr && play_ptr->get_legion_guid() == _record_score->role_guid)
			{
				ws2ws_grant_kill_boss_legion_award req;
				req.boss_id = boss_id;
				req.kill_boss_legion_guid = play_ptr->get_legion_guid();
				req.play_num = msg.play_num;
				if (msg.play_num > max_hurt_monster_player_num)
				{
					req.play_num = max_hurt_monster_player_num;
				}
				for (int32 o = 0; o < req.play_num; ++o)
				{
					req.play_guid_list[o] = msg.play_guid_list[o];
				}
				cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_grant_kill_boss_legion_award, &req, sizeof(req));
			}
		}
	}

	//发放击杀boss军团的奖励
	void cross_server_world_boss_ws_mgr::make_kill_boss_legion_awaed(guid_64 kill_boss_legion_guid, int32 kill_boss_tmp_id, guid_64* play_guid_list, int32 play_num)
	{
		if (!kill_boss_legion_guid.is_valid() || play_guid_list == nullptr || play_num <= 0)
		{
			return;
		}
		ActivityCommonConfigTemplate* cross_server_boss_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_world_boss);
		if (nullptr == cross_server_boss_config_ptr)
		{
			return;
		}
		std::vector<int32> item_list = cross_server_boss_config_ptr->ParamIntArr2;
		if (item_list.size() <= 0 || item_list.size() % 2 != 0 || item_list.size() / 2 < m_boss_max_num)
		{
			return;
		}
		int32 drop_bg = 0;
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (item_list[i*2] == kill_boss_tmp_id)
			{
				drop_bg = item_list[i*2 + 1];
				break;
			}
		}
		if (drop_bg <= 0)
		{
			return;
		}
		std::vector<s_item_template_info> _item_list;
		globle_data::get_instance().get_item_list_by_drop_bg(drop_bg, _item_list, 0, ranking_mgr_ws::get_world_level_last());
		if (_item_list.size() <= 0)
		{
			return;
		}
		legion_ws* _rank_legion = legion_ws_mgr::get_instance().get_legion(kill_boss_legion_guid);
		if (_rank_legion == nullptr)
		{
			return;
		}
		for (auto& it : _item_list)
		{
			_rank_legion->give_activity_reward(cross_server_boss_config_ptr, play_guid_list, play_num, it.m_item_id, it.m_item_num);
		}

	}

	//活动地图结束
	void cross_server_world_boss_ws_mgr::cross_boss_map_game_over(int32 map_template)
	{
		for (int32 i = 0; i < map_max; ++i)
		{
			if (get_cross_server_world_boss_map_temp_id(i) == map_template)
			{
				for (int32 o = 0; o < m_map_max_boss; ++o)
				{
					if (m_boss_kill_all_info[i * m_map_max_boss + o].is_dead == false)
					{
						save_boss_info_to_dp(m_boss_kill_all_info[i * m_map_max_boss + o].boss_template_id);
					}

				}
			}
			auto map_ent = get_entity(m_cross_world_boss_map_id[i]);
			if (nullptr == map_ent)
			{
				continue;
			}
			auto map_cp = map_ent->get_component<cs_map_component>();
			if (map_template == map_cp->m_map_template_id)
			{
				map_cp->m_map_state = e_map_state_over;
				m_cross_world_boss_map_id[i] = 0;
				break;
			}
		}
	}

	//发送需要显示的击杀boss信息
	void cross_server_world_boss_ws_mgr::send_kill_boss_show(client_session* session)
	{
		if (session == nullptr || session->get_role_guid().is_valid() == false)
		{
			return;
		}
		game_proto_set_cross_boss_show_info msg;
		for (int32 i = 0; i < map_max; ++i)
		{
			if (is_in_game_all_time())
			{
				int32 be_kill_boss_num = m_map_max_boss;//每个地图一共3个boss 死亡一个就减少1
				for (int o = 0; o < m_map_max_boss; ++o)
				{
					if (m_boss_kill_all_info[i*m_map_max_boss + o].is_dead)
					{
						--be_kill_boss_num;
					}
				}
				msg.add_boss_residue_num(be_kill_boss_num);
			}
			msg.add_player_name(m_boss_kill_all_info[i*m_map_max_boss].kill_boss_play_name);
			if (sizeof(m_boss_kill_all_info[i*m_map_max_boss].kill_boss_play_legion_name) > 0)
			{
				msg.add_legion_name(m_boss_kill_all_info[i*m_map_max_boss].kill_boss_play_legion_name);
			}
		}
		session->send_to_client(&msg, e_msgindex_s2c_set_cross_boss_show_info);
	}

	//发送消息给所有玩家
	void cross_server_world_boss_ws_mgr::send_message_to_all_cs(const void * data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr || data_len == 0)
		{
			return;
		}
		world_server::getInstance().broadcast(data_ptr, data_len, e_server_type_cs);
	}

	//保存击杀boss信息到数据库
	void cross_server_world_boss_ws_mgr::save_boss_info_to_dp(int32 boss_template_id)
	{
		ws2dp_save_cross_server_world_boss_msg_all msg;
		msg.boss_template_id = -1;
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (m_boss_kill_all_info[i].boss_template_id == boss_template_id)
			{
				msg.boss_template_id = boss_template_id;
				if (m_boss_kill_all_info[i].is_dead)
				{
					msg.boss_is_dead = 1;
				}
				else
				{
					msg.boss_is_dead = 0;
					ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
					return;
				}
				msg.kill_boss_player_guid = m_boss_kill_all_info[i].kill_boss_play_id;
				msg.kill_boss_legion_guid = m_boss_kill_all_info[i].kill_boss_legion_id;
				msg.kill_boss_player_server_id = m_boss_kill_all_info[i].kill_boss_server_id;
				msg.kill_boss_time = m_boss_kill_all_info[i].boss_be_kill_time;
				msg.set_role_name(m_boss_kill_all_info[i].kill_boss_play_name);
				msg.set_legion_name(m_boss_kill_all_info[i].kill_boss_play_legion_name);
				break;
			}
		}
		if (msg.boss_template_id == -1)
		{
			return;
		}
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	//用于接收gate服发来的boss信息并保存
	void cross_server_world_boss_ws_mgr::set_kill_boss_show_info_all(const cross_boss_info * boss_info, int32 data_num)
	{
		for (int32 i = 0; i < data_num; ++i)
		{
			for (int32 o = 0; o < m_boss_max_num; ++o)
			{
				if (boss_info[i].boss_template_id == m_boss_kill_all_info[o].boss_template_id || m_boss_kill_all_info[o].boss_template_id == 0)
				{
					m_boss_kill_all_info[o] = boss_info[i];
					break;
				}
			}
		}
	}

	//发送boss被击杀公告
	void cross_server_world_boss_ws_mgr::send_boss_dead_notice(int32 boss_template, int32 notice_id, int32 item_num, s_item_info * data_ary, std::string notice_string)
	{
		if (notice_id <= 0 || data_ary == nullptr || notice_string.length() <= 0)
		{
			return;
		}
		//发送boss被击杀的公告
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_string, data_ary, item_num);
		NpcTemplate* npc_tem_ptr = GET_TEMPLATE(NpcTemplate, boss_template);
		if (npc_tem_ptr == nullptr)
		{
			return;
		}
		int32 boss_name_id = npc_tem_ptr->NpcName;
		//boss被击杀发送奖励已放入到拍卖行公告
		int32 award_notice_str_id = cross_boss_be_kill_award_notice_id;
		std::vector<std::string> notice_str_params;
		notice_str_params.push_back(template_manager::get_instance().get_str_id_by_notice_id(award_notice_str_id));
		notice_str_params.push_back(template_manager::get_instance().int_to_string(boss_name_id));
		std::string award_notice_str = init_unit::implode(notice_str_params);
		event_ws_mgr::get_instance().send_notice_to_all(award_notice_str_id, create_time, guid_64(), award_notice_str);
	}

	//加载boss击杀信息
	void cross_server_world_boss_ws_mgr::locd_boss_show_info(const cross_world_boss_info_to_db * dp_info, int32 data_num)
	{
		if (nullptr == dp_info)
		{
			return;
		}	
		for (int32 i = 0; i < data_num; ++i)
		{
			if (dp_info[i].boss_template_id > 0)
			{
				for (int32 o = 0; o < m_boss_max_num; ++o)
				{
					if (dp_info[i].boss_template_id == m_boss_kill_all_info[o].boss_template_id)
					{
						m_boss_kill_all_info[o].set_play_name(dp_info[i].kill_boss_player_name);
						m_boss_kill_all_info[o].set_legion_name(dp_info[i].kill_boss_legion_name);
						if (dp_info[i].boss_is_dead == 1)
						{
							m_boss_kill_all_info[o].is_dead = true;
						}
						else
						{
							m_boss_kill_all_info[o].is_dead = false;
						}
						
						break;
					}
				}
			}
		}
	}

	//接收gate服的boss信息并保存
	void cross_server_world_boss_ws_mgr::set_kill_boss_show_info(const cross_boss_info& boss_info)
	{
		if (boss_info.boss_template_id <= 0)
		{
			return;
		}
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (m_boss_kill_all_info[i].boss_template_id == boss_info.boss_template_id)
			{
				m_boss_kill_all_info[i] = boss_info;
			}
		}
	}

	//发送所有boss信息给其他服务器 参数是服务器id (0代表发给所有服务器)
	void cross_server_world_boss_ws_mgr::send_boss_dead_info_all_to_all_server(int32 server_id)
	{
		ws2ws_cross_refresh_kill_boss_show_info_all msg;
		msg.data_num = 0;
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			msg.boss_info[i] = m_boss_kill_all_info[i];
			++msg.data_num;
		}
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_refresh_kill_boss_show_info_all, &msg, sizeof(msg));

	}

	//将boss伤害排名保存到排行榜中
	void cross_server_world_boss_ws_mgr::send_cross_boss_ranking_list(int32 boss_id)
	{
		int32 boss_index = -1;
		for(int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (get_world_boss_id_with_type(i) == boss_id)
			{
				boss_index = i;
				break;
			}
		}
		if (boss_index < 0)
		{
			return;
		}
		std::vector<score_record> rank_list = m_boss_damage_list[boss_index].get_score_record_list_all();
		if (rank_list.size() <= 0)
		{
			return;
		}
		ws2ws_get_legion_chief_guid msg;
		for (int32 i = 0; i < rank_list.size(); ++i)
		{
			s_ranking_player_info list_info;
			list_info.role_guid = rank_list[i].role_guid;
			list_info.supporting_guid = rank_list[i].role_guid;
			list_info.set_role_name(rank_list[i].role_name);
			list_info.set_supporting_name(rank_list[i].role_name);
			list_info.ranking_value = rank_list[i].score/10000;
			list_info.save_time = time_helper::get_cur_time_new().second;
			list_info.server_id = rank_list[i].server_id;
			list_info.ranking_type = boss_index + e_RankingIndex_cross_boss_one;
			ranking_mgr_ws::sync_player_info(list_info);
			if (rank_list[i].server_id <= 0)
			{
				continue;
			}
			msg.legion_guid = rank_list[i].role_guid;
			msg.rank_type = boss_index + e_RankingIndex_cross_boss_one;
			cross::send_msg_to_ws(guid_64(), rank_list[i].server_id, e_msg_ws2ws_get_legion_chief_guid, &msg, sizeof(msg));
		}
	}

	//接收服务器信息保存伤害列表
	void cross_server_world_boss_ws_mgr::set_cross_boss_ranking_list(int32 boss_template_id, s_ranking_player_info * ranking_info, int32 data_num)
	{
		/*if (boss_template_id <= 0 || ranking_info == nullptr || data_num <= 0)
		{
			return;
		}
		bool boss_id_is_true = false;
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (m_boss_id[i] == boss_template_id)
			{
				boss_id_is_true = true;
				break;
			}
		}
		if (boss_id_is_true == false)
		{
			return;
		}
		for (int32 i = 0; i < data_num; ++i)
		{
			ranking_mgr_ws::sync_player_info(ranking_info[i]);
		}*/
	}

	//发送boss死亡信息给所有服务器
	void cross_server_world_boss_ws_mgr::send_boss_dead_info_to_all_server(int32 boss_temp_id, int32 server_id)
	{
		ws2ws_cross_refresh_kill_boss_show_info msg;
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (m_boss_kill_all_info[i].boss_template_id == boss_temp_id)
			{
				msg.boss_info = m_boss_kill_all_info[i];
			}
		}
		cross::send_msg_to_ws(guid_64(), 0, e_msgindex_ws2ws_refresh_kill_boss_show_info, &msg, sizeof(msg));
	}

	//向服务器发送所有boss信息
	void cross_server_world_boss_ws_mgr::send_cross_boss_info_all_to_other_ws(int32 server_id)
	{
		send_boss_dead_info_all_to_all_server(server_id);
	}

	void cross_server_world_boss_ws_mgr::change_legion_name(guid_64 legion_guid, const xstring& legion_name)
	{
		if (!legion_guid.is_valid() || legion_name.size() <= 0)
		{
			return;
		}
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (m_boss_kill_all_info[i].kill_boss_legion_id == legion_guid)
			{
				m_boss_kill_all_info[i].set_legion_name(legion_name);
				save_boss_info_to_dp(m_boss_kill_all_info[i].boss_template_id);
			}
			m_boss_damage_list[i].change_legion_name_func(legion_guid, legion_name.c_str());
		}		
		send_boss_dead_info_all_to_all_server(0);//将信息同步到所有服务器
	}

	//同步创建boss到cs
	void cross_server_world_boss_ws_mgr::sync_create_boos_to_cs(guid_64 from_map_guid)
	{
		if (!from_map_guid.is_valid())
		{
			return;
		}
		ws2cs_cross_server_world_boss_creat_boss creat_boss_msg;
		set_boss_level();
		creat_boss_msg.true_boss_level = get_true_boss_level();
		creat_boss_msg.false_boss_level = get_false_boss_level();
		creat_boss_msg.from_map_guid = from_map_guid;
		if (m_game_over_flag)
		{
			creat_boss_msg.true_boss_level = -1; //表明创建副本时活动还在 副本内要创建boss时活动结束
			creat_boss_msg.false_boss_level = -1;
		}
		bool is_use = proto_by_lua(e_msgindex_ws2cs_cross_server_world_boss_creat_boss);
		if (is_use == false)
		{
			send_message_to_all_cs(&creat_boss_msg, sizeof(creat_boss_msg));
		}
		else
		{
			hld::ws2cs_proto::cross_server_world_boss_creat_boss pro_msg;
			creat_boss_msg.to_proto(pro_msg);
			world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2cs_cross_server_world_boss_creat_boss, e_server_type_cs);
		}
	}

	//获得服务器平均等级真
	int32 cross_server_world_boss_ws_mgr::get_true_boss_level()
	{
		return m_boss_level_true;
	}

	//获得服务器平均等级假
	int32 cross_server_world_boss_ws_mgr::get_false_boss_level()
	{
		return m_boss_level_false;
	}

	//设置服务器等级
	void cross_server_world_boss_ws_mgr::set_boss_level()
	{
		int32 level_true = 0;
		int32 level_false = 0;
		int32 server_num = 0;
		for (int32 i = 0; i < max_server_num; ++i)
		{
			if (m_boss_level[i].is_valid())
			{
				level_true += m_boss_level[i].true_boss_level;
				level_false += m_boss_level[i].false_boss_level;
				++server_num;
			}
		}
		if (server_num <= 0)
		{
			return;
		}
		m_boss_level_true = level_true / server_num;
		m_boss_level_false = level_false / server_num;
	}

	//发送获得服务器等级
	void cross_server_world_boss_ws_mgr::get_with_save_server_level()
	{
		ws2ws_get_cross_server_level msg;
		msg.server_id = world_server::getInstance().get_server_id();
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss))
		{
			return;
		}
		cross::send_msg_to_ws(guid_64(), 0, e_msgindex_ws2ws_get_cross_server_level, &msg, sizeof(msg));
	}

	//发送服务器等级
	void hld::cross_server_world_boss_ws_mgr::send_server_level(int32 server_id)
	{
		ActivityCommonConfigTemplate* cross_server_boss_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_world_boss);
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss) || cross_server_boss_config_ptr == nullptr)
		{
			return;
		}
		ws2ws_get_cross_server_level_end msg;
		msg.true_server_level = ranking_mgr_ws::get_rank_level_with_num(cross_server_boss_config_ptr->ParamInt1);
		msg.false_server_level = ranking_mgr_ws::get_rank_level_with_num(cross_server_boss_config_ptr->ParamInt2);
		msg.server_id = world_server::getInstance().get_server_id();
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_get_cross_server_level_end, &msg, sizeof(msg));
	}

	//收到服务器等级的处理
	void cross_server_world_boss_ws_mgr::server_level_func(int32 true_server_level, int32 false_server_level, int32 server_id)
	{
		int32 empty_idex = -1;
		for (int32 i = 0; i < max_server_num; ++i)
		{
			if (m_boss_level[i].server_id == server_id)
			{
				m_boss_level[i].true_boss_level = true_server_level;
				m_boss_level[i].false_boss_level = false_server_level;
				return;
			}
			if (m_boss_level[i].is_valid() == false)
			{
				empty_idex = i;
			}
		}
		if (empty_idex == -1)
		{
			return;
		}
		else
		{
			m_boss_level[empty_idex].true_boss_level = true_server_level;
			m_boss_level[empty_idex].false_boss_level = false_server_level;
			m_boss_level[empty_idex].server_id = server_id;
		}
	}
}
