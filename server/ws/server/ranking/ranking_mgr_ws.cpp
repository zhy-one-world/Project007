#include "ws_client.hpp"
#include "ranking_mgr_ws.h"
#include "template/template_manager.h"
#include <ranking_msg.hpp>
#include "utility/cs_date.hpp"
#include "server/mail/event_ws_mgr.h"
#include "utility/init_unit.h"
#include "utility/random.h"
#include "server_log.hpp"
#include <Logic/time_def.hpp>
#include <Logic/activity_def.hpp>
#include "world_server.hpp"
#include "server/legion/legion_ws_mgr.h"
#include "cross_group/gate_proxy.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include <Utility/parse_msg.h>
#include "server/big_player_ws_mgr.h"
#include "server/relation/marry_mgr_ws.hpp"
#include "server/mail/event_ws_mgr.h"
#include "server/activity/cross_ladder_ws_mgr.h"
#include "lua/script_mgr.h"
#include "globle_data.h"
#include "ranking.pb.h"
#include "server/mail/mail_event_ws.h"
#include "ranking_cross.h"
#include "game.pb.h"
#include "net.pb.h"

using namespace hld;

bool m_flag_save_ranking[e_RankingIndex_max];
std::list<s_ranking_player_info> m_ranking_all[e_RankingIndex_max];
std::map<ui64, s_service_rank_player_info> m_service_rank_all[e_service_rank_type_max];
guid_64 m_service_rank_first_player[e_service_rank_type_max];
int32 m_cur_srv_avg_level;
int32 m_world_avg_level_cur;
int32 m_world_avg_level_last;
int32 m_service_rank_check_index = e_service_rank_type_max;
bool m_loading_end_flag = false;
bool m_is_need_save_db_level = false;
std::map<int32, ranking_cross>	m_ranking_top_group;
bool m_b_ranking_top_group[max_server_num] = { 0 };


void  ranking_mgr_ws::init_manager()
{
	for (int32 i = 0; i < e_RankingIndex_max; ++i)
	{
		m_ranking_all[i].clear();
		m_flag_save_ranking[i] = false;
	}
	m_cur_srv_avg_level = 0;
	m_world_avg_level_last = -1;
	m_world_avg_level_cur = -1;
	for (int32 i = 0; i < e_service_rank_type_max; ++i)
	{
		m_service_rank_all[i].clear();
		m_service_rank_first_player[i].clear_data();
	}
	m_ranking_top_group.clear();
}

void  ranking_mgr_ws::heart_tick(int64& time_now)
{ 
	static int64 m_sec_tick = 0;
	if (m_sec_tick < time_now )
	{
		m_sec_tick = time_now + hld::minute_tick_time;			//分钟tick
		open_server_first_day_proc();
	}
	static int64 m_ranking_tick = 0;
	if (m_ranking_tick < time_now && true == gate_proxy::getInstance().is_gate_run()  && world_server::getInstance().get_server_id() != world_server::getInstance().get_cross_id())
	{
		//两分钟刷新一次
		m_ranking_tick = time_now + hld::minute_tick_time * 2;			//秒钟tick 
		send_gs_ranking(e_RankingIndex_gs, e_RankingIndex_gs_all);
		send_gs_ranking(e_RankingIndex_legion_core, e_RankingIndex_legion_gs_all);
		send_gs_ranking(e_RankingIndex_level, e_RankingIndex_level_all);
	}
	static int64 m_next_save_tick = 0;
	if (m_next_save_tick < time_now)
	{
		m_next_save_tick = time_now + hld::thrity_second_tick_time + random_gen::get_random(0, thrity_second_tick_time);
		tick_save_ranking_list();
	}

	static int64 sync_srv_avg_lv_stamp = 0;
	if (time_now >= sync_srv_avg_lv_stamp)
	{
		sync_srv_avg_level_to_cs();
		if (m_cur_srv_avg_level > 0)
		{
			sync_srv_avg_lv_stamp = time_now + minute_tick_time * 5;
		}
	}
		
	time_info cur_time_info = time_helper::get_cur_time_new();
	static bool m_cur_time_is_zero = false;
	if (cur_time_info.hour_in_day == 23 && cur_time_info.minute_in_hour == 55)	//调整到23点
	{
		m_cur_time_is_zero = true;
	} 
	else
	{
		m_cur_time_is_zero = false;
	}
	static int64 m_old_time = 0;
	if (m_cur_time_is_zero && cur_time_info.millisecond >= m_old_time)
	{
		static int32 m_cur_rank_index = 0;
		if (m_cur_rank_index >= e_RankingIndex_max)
		{
			m_cur_rank_index = 0;
			m_old_time = cur_time_info.millisecond + day_tick_time;
			return;
		}
		server_log::rank_list_log(get_ranking_list_by_type((e_RankingIndex)m_cur_rank_index), m_cur_rank_index);
		m_cur_rank_index++;
	}

	//全民冲榜更新检测
	if (m_service_rank_check_index < e_service_rank_type_max && m_loading_end_flag)
	{
		ServiceRankTemplate* service_rank_template_ptr = GET_TEMPLATE(ServiceRankTemplate, service_rank_first_template_id + m_service_rank_check_index);
		if (nullptr != service_rank_template_ptr)
		{
			if (service_rank_template_ptr->EndTime <= globle_data::get_instance().get_server_on_days())
			{
				set_service_rank_map_by_type((e_service_rank_type)m_service_rank_check_index);
			}
		}
	}
}

void ranking_mgr_ws::clear_data_by_ranking_type(e_RankingIndex ranking_type)
{
	if (ranking_type >= e_RankingIndex_max)
	{
		return;
	}
	m_ranking_all[ranking_type].clear();
	m_flag_save_ranking[ranking_type] = true;
}
	 
void ranking_mgr_ws::set_ranking_save_flag(int32 ranking_type, bool is_save)
{
	if (ranking_type < 0 || ranking_type >= e_RankingIndex_max)
	{
		return;
	}
	m_flag_save_ranking[ranking_type] = is_save;
}

void ranking_mgr_ws::save_ranking_info_to_db(const s_ranking_player_info& ranking_info)
{ 
	//ws2dp_save_ranking_info_one save_pak;
	//save_pak.ranking_info = ranking_info;
	//ws_client::getInstance().send_to_dp(&save_pak, sizeof(save_pak));

	server2dp_proto_ws2dp_save_ranking_info_one msg;
	server2dp_proto_s_ranking_player_info* player_info = msg.mutable_ranking_info();
	if (nullptr == player_info)
	{
		return;
	}
	player_info->set_role_guid(ranking_info.role_guid);
	player_info->set_role_name(ranking_info.role_name);
	player_info->set_ranking_type(ranking_info.ranking_type);
	player_info->set_ranking_value(ranking_info.ranking_value);
	player_info->set_save_time(ranking_info.save_time);
	player_info->set_supporting_guid(ranking_info.supporting_guid.server_64);
	player_info->set_supporting_name(ranking_info.supporting_name);
	player_info->set_vip_level(ranking_info.vip_level);
	player_info->set_peak(ranking_info.Peak);
	player_info->set_server_id(ranking_info.server_id);
	player_info->set_game_channel(ranking_info.game_channel);
	player_info->set_standby_parameter(ranking_info.standby_parameter);
	ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_ranking_info_one);
}

void ranking_mgr_ws::req_load_all_ranking_from_db()
{
	static bool m_load_flag = false;
	if (m_load_flag == true)
	{
		return;
	}
	m_load_flag = true;
	for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max; ++ranking_type)
	{
		//ws2dp_load_all_ranking_info req;
		//req.ranking_type = ranking_type;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));

		script_mgr::get_instance().call_func("ws2dp_send", "send_load_all_ranking_info_lua", 0, false, "%d", ranking_type);
	}
}

void ranking_mgr_ws::tick_save_ranking_list()
{
	for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max; ++ranking_type)
	{
		if (m_flag_save_ranking[ranking_type])
		{
			m_flag_save_ranking[ranking_type] = false;
			ws2dp_save_one_total_ranking_info req;
			req.data_num = 0;
			req.data_type = ranking_type;
			for (auto ite_list = m_ranking_all[ranking_type].begin(); ite_list != m_ranking_all[ranking_type].end();++ite_list)
			{
				if (req.data_num >= hld::TopMaxNum)
				{
					break;
				}
				req.data_info.data_ary[req.data_num] =  *ite_list;
				int64 test_info = req.data_info.data_ary[req.data_num].ranking_value;
				req.data_num++;
			} 
			ws_client::getInstance().send_to_dp(&req, sizeof(req)); 
		}
	}
}

void ranking_mgr_ws::save_service_rank_list_to_db_by_type(e_service_rank_type service_rank_type)
{
	if (service_rank_type >= e_service_rank_type_max)
	{
		return;
	}
		
	ws2dp_save_one_service_rank_info req;
	req.data_num = 0;
	for (auto ite_map = m_service_rank_all[service_rank_type].begin(); ite_map !=m_service_rank_all[service_rank_type].end(); ++ite_map)
	{
		if (req.data_num >= hld::ShowTopMaxNum)
		{
			break;
		}
		req.data_info.data_ary[req.data_num++] = ite_map->second;
	}
	ws_client::getInstance().send_to_dp(&req, sizeof(req));

	if (service_rank_type + 1> m_service_rank_check_index)
	{
		m_service_rank_check_index = service_rank_type + 1;
		world_server::getInstance().save_server_attr_val_db(e_srv_attr_val_service_rank_check, m_service_rank_check_index);
	}
}

void ranking_mgr_ws::set_service_rank_map_by_type(e_service_rank_type service_rank_type)
{
	if (service_rank_type >= e_service_rank_type_max || m_service_rank_check_index > service_rank_type)
	{
		return;
	}

	ServiceRankTemplate* service_rank_template_ptr = GET_TEMPLATE(ServiceRankTemplate, service_rank_first_template_id + service_rank_type);
	if (nullptr == service_rank_template_ptr)
	{
		return;
	}

	int32 ranking_type = service_rank_template_ptr->RankingType;
	int32 data_num = 0;
	m_service_rank_all[service_rank_type].clear();
	for (auto ite_list = m_ranking_all[ranking_type].begin(); ite_list != m_ranking_all[ranking_type].end(); ++ite_list)
	{
		if (data_num >= ShowTopMaxNum)
		{
			break;
		}
		if (ite_list->ranking_value < service_rank_template_ptr->MinValue)
		{
			break;
		}
		data_num++;
		s_service_rank_player_info player_info;
		player_info.role_guid = ite_list->role_guid;
		player_info.rank_type = service_rank_type;
		player_info.role_num = data_num;
		m_service_rank_all[service_rank_type].insert({ ite_list->role_guid.server_64, player_info });
		if (data_num == 1)
		{
			m_service_rank_first_player[service_rank_type] = ite_list->role_guid;
		}
	}

	save_service_rank_list_to_db_by_type(service_rank_type);

	for (auto ite_map = m_service_rank_all[service_rank_type].begin(); ite_map != m_service_rank_all[service_rank_type].end(); ++ite_map)
	{
		client_session* session_ptr = client_session_mgr::getInstance().get_session(ite_map->first);
		if (nullptr == session_ptr)
		{
			continue;
		}
		send_service_rank_info_one_to_client(session_ptr, ite_map->second.role_num, service_rank_type);
	}
}

void ranking_mgr_ws::req_load_all_server_rank_from_db()
{
	static bool m_service_rank_load_flag = false;
	if (m_service_rank_load_flag)
	{
		return;
	}
	m_service_rank_load_flag = true;
	for (int32 rank_type = 0; rank_type < e_service_rank_type_max; ++rank_type)
	{
		//ws2dp_load_all_service_rank_info req;
		//req.rank_type = rank_type;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));

		script_mgr::get_instance().call_func("ws2dp_send", "send_load_all_service_rank_info_lua", 0, false, "%d", rank_type);
	}
}

void ranking_mgr_ws::load_all_server_rank_from_db(const s_service_rank_player_info* load_info, int32 data_num)
{
	if (nullptr == load_info)
	{
		return;
	}
	for (int32 ranking_count = 0; ranking_count < data_num; ++ranking_count)
	{
		s_service_rank_player_info player_info = load_info[ranking_count];
			
		m_service_rank_all[player_info.rank_type].insert({player_info.role_guid.server_64, player_info});

		if (player_info.role_num == 1)
		{
			m_service_rank_first_player[player_info.rank_type] = player_info.role_guid;
		}
	}
}

void ranking_mgr_ws::set_service_rank_check_index(int32 service_rank_check_index)
{ 
	m_service_rank_check_index = service_rank_check_index; 
}

int32 ranking_mgr_ws::get_service_rank_check_index() 
{ 
	return m_service_rank_check_index; 
}

bool ranking_mgr_ws::fixed_ranking_supporting_data(s_ranking_player_info* load_info, const guid_64& legion_guid, const xstring& legion_name)
{
	if (nullptr == load_info)
	{
		return false;
	}
	if (load_info->role_guid.is_valid() == false)
	{
		return false;
	}
	int32 rangking_type = load_info->ranking_type;
	switch ((e_RankingIndex)rangking_type)
	{
	case e_RankingIndex_gs:
	case e_RankingIndex_level:
	case e_RankingIndex_box_map_level:
	case e_RankingIndex_elves_gs:
	case e_RankingIndex_demonstower:
	case e_RankingIndex_wing:
	case e_RankingIndex_mount:
	case e_RankingIndex_worship:
	case e_RankingIndex_arena:
	case e_RankingIndex_legion_core:
	case e_RankingIndex_gs_barserker:
	case e_RankingIndex_gs_wizard:
	case e_RankingIndex_gs_guardian:
	case e_RankingIndex_gs_assassinator:
	case e_RankingIndex_level_barserker:
	case e_RankingIndex_level_wizard:
	case e_RankingIndex_level_guardian:
	case e_RankingIndex_level_assassinator:
	case e_RankingIndex_equipment_main:
	case e_RankingIndex_belief:
	case e_RankingIndex_element:
	case e_RankingIndex_military_rank:
	case e_RankingIndex_achievement:
	case e_RankingIndex_illustrated:
	case e_RankingIndex_vip_level_one:
	case e_RankingIndex_vip_level_five:
	case e_RankingIndex_vip_level_seven:
	case e_RankingIndex_recharge_rmb:
	case e_RankingIndex_inspiration:
	case e_RankingIndex_time_limit_activity_total_recharge:
	case e_RankingIndex_time_limit_activity_total_cost:
	case e_RankingIndex_time_limit_activity_treasure:
	case e_RankingIndex_cross_server_plinder_guard_value:
	case e_RankingIndex_feather_heart:
	case e_RankingIndex_cross_time_limit_activity_total_recharge:
	{
		load_info->set_supporting_name(legion_name);
		return true;
	}
	break;			
	default:
		break;
	}
	return false;
}

bool ranking_mgr_ws::fixed_ranking_legion_name_data(s_ranking_player_info* load_info, const guid_64& legion_guid, const xstring& legion_name)
{
	if (nullptr == load_info)
	{
		return false;
	}
	if (load_info->supporting_guid.is_valid() == false)
	{
		return false;
	}
	int32 rangking_type = load_info->ranking_type;
	switch ((e_RankingIndex)rangking_type)
	{
	case e_RankingIndex_legion_core: //军团榜 -总战力排行	
	case e_RankingIndex_legion_territory: //军团榜-领地排行
	case e_RankingIndex_legion_boss: //军团榜-军团boss
	case e_RankingIndex_legion_bonfire: //军团榜-军团篝火
	case e_RankingIndex_world_boss_one: //世界BOSS-boss1
	case e_RankingIndex_world_boss_two: //世界BOSS-boss2
	case e_RankingIndex_world_boss_three: //世界BOSS-boss3
	case e_RankingIndex_world_boss_four: //世界BOSS-boss4
	case e_RankingIndex_world_boss_five: //世界BOSS-boss5
	case e_RankingIndex_world_boss_six: //世界BOSS-boss6
	case e_RankingIndex_world_boss_seven: //世界BOSS-boss7
	{
		load_info->set_role_name(legion_name);
		return true;
	}
	break;
	default:
		break;
	}
	return false;
}
void ranking_mgr_ws::set_gate_ranking_legion_name(const guid_64& legion_guid, const xstring& legion_name)
{
	for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max; ranking_type++)
	{
		if (!init_unit::is_rank_type_cross_server(e_RankingIndex(ranking_type)))
		{
			continue;
		}
		ranking_list* _ranking_list = get_ranking_list_by_type(e_RankingIndex(ranking_type));
		if (nullptr == _ranking_list)
		{
			continue;
		}
		ranking_list_ite ite = _ranking_list->begin();
		for (; ite != _ranking_list->end(); ++ite)
		{
			s_ranking_player_info* play_ranking_info = &(*ite);
			if (play_ranking_info->role_guid == legion_guid)
			{
				play_ranking_info->set_role_name(legion_name);
				m_flag_save_ranking[ranking_type] = true;
			}
			else if (play_ranking_info->supporting_guid == legion_guid)
			{
				play_ranking_info->set_supporting_name(legion_name);
				m_flag_save_ranking[ranking_type] = true;
			}
		}	
	}
		
}

void ranking_mgr_ws::set_ranking_leigon_chief_guid(int32 rank_type, const guid_64 & legion_guid, const guid_64 & role_guid, const xstring& legion_chief_name, const int32& legion_chief_vip_level)
{
	if (legion_guid.is_valid() == false || role_guid.is_valid() == false || rank_type < 0 || rank_type > e_RankingIndex_max)
	{
		return;
	}
	s_ranking_player_info* player_ranking = get_ranking_player_info_by_type((e_RankingIndex)rank_type, legion_guid);
	if (player_ranking == nullptr)
	{
		return;
	}
	player_ranking->supporting_guid = role_guid;
	player_ranking->set_supporting_name(legion_chief_name);
	player_ranking->vip_level = legion_chief_vip_level;
}

void ranking_mgr_ws::change_ranking_player_legion_info(const guid_64& ranking_guid, const guid_64& legion_guid, const xstring& legion_name)
{
	if (ranking_guid.is_valid() == false)
	{
		return;
	}
	for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max; ranking_type++)
	{
		s_ranking_player_info* player_ranking = get_ranking_player_info_by_type((e_RankingIndex)ranking_type, ranking_guid);
		if (nullptr == player_ranking)
		{
			continue;
		}
		if (fixed_ranking_supporting_data(player_ranking, legion_guid, legion_name))
		{
			set_ranking_save_flag(ranking_type, true);
			//save_ranking_info_to_db(*player_ranking);
		}
			
	}
}


void ranking_mgr_ws::load_ranking_from_db(const s_ranking_player_info* load_info, int32 data_num, int32 ranking_type)
{
	if (ranking_type < e_RankingIndex_gs || ranking_type >= e_RankingIndex_max || nullptr == load_info)
	{
		return;
	}
	for (int32 ranking_count = 0; ranking_count < data_num; ++ranking_count)
	{
		s_ranking_player_info player_info = load_info[ranking_count];
		if (!player_info.role_guid.is_valid())
		{
			continue;
		}
		//因战力值从32位改为64 以下排行榜是更换了战力的存储位置,防止上线时新字段为空导致显示问题,所有在load结束时将原有战力放入到新的战力存储中
		if (ranking_type == e_RankingIndex_arena && player_info.standby_parameter <= 0)
		{
			player_info.standby_parameter = player_info.supporting_guid.A;
		}
		if (ranking_type == e_RankingIndex_cross_ladder_last_seaon && player_info.standby_parameter <= 0)
		{
			player_info.standby_parameter = player_info.supporting_guid.A;
		}
		if (ranking_type == e_RankingIndex_cross_ladder_cur_season && player_info.standby_parameter <= 0)
		{
			player_info.standby_parameter = player_info.supporting_guid.A;
		}

		ranking_list* ranking_ptr = &m_ranking_all[ranking_type];
		if (nullptr == ranking_ptr)
		{
			continue;
		}
		bool is_exist = false;
		ranking_list_ite ite_find = ranking_ptr->begin();
		for (; ite_find != ranking_ptr->end(); ite_find++)
		{
			if (player_info.role_guid == (*ite_find).role_guid)
			{
				is_exist = true;
				break;
			}
		}

		if (false == is_exist)
		{	
			if (ranking_type == e_RankingIndex_arena)
			{
				insert_player_arena(player_info);
			}
			else
			{
				sync_player_info_insert(player_info, ranking_ptr);
			}	
		}
	}
	m_loading_end_flag = true;
	if (world_server::getInstance().is_sky_island_server())
	{
		send_get_oracle_trial_info();
	}
	else
	{
		send_gs_ranking(e_RankingIndex_Oracle_Trial, e_RankingIndex_Oracle_Trial);
	}
	//排行榜全部加载完 向gate服发送当前服的等级
	if (e_RankingIndex_max - 1 == ranking_type)
	{
		//未开启跨服直接设置
		world_server::getInstance().set_ws_loading_flag(e_ws_flag_ranking_info);
	}
}

void ranking_mgr_ws::load_one_worship_ranking_from_db(const guid_64& sender, const int32& ranking_type, const int32& worship_add_value, const s_ranking_player_info& one_load_info, const int32& worship_type, const int32& worship_result)
{
	if (one_load_info.role_guid.is_valid() == false
		|| sender.is_valid() == false)
	{
		return;
	}
	if (one_load_info.ranking_type < 0
		|| one_load_info.ranking_type >= e_RankingIndex_max
		|| ranking_type < 0
		|| ranking_type >= e_RankingIndex_max)
	{
		return;
	}

	s_ranking_player_info player_info;
	player_info = one_load_info;
	//更新威望排行榜
	sync_player_info(player_info);
	//回消息给sender和target
	client_session* sender_ptr = client_session_mgr::getInstance().get_session(sender);
	if (sender_ptr != nullptr)
	{
		ws2cs_req_worship_player_end pak;
		pak.role_guid = sender;
		pak.target_guid = one_load_info.role_guid;
		pak.ranking_type = ranking_type;
		pak.worship_type = worship_type;
		pak.target_worship_value = player_info.ranking_value;
		pak.worship_result = worship_result;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_req_worship_player_end);
		if (is_use == false)
		{
			sender_ptr->send_to_cs(&pak, sizeof(ws2cs_req_worship_player_end));
		}
		else
		{
			hld::ws2cs_proto::req_worship_player_end pro_msg;
			pak.to_proto(pro_msg);
			sender_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_req_worship_player_end);
		}
	}
}


void ranking_mgr_ws::create_robot_data()
{
	//排行榜中不加入机器人
	return;
	//template_manager::template_type* robot_name_tmpl_table = template_manager::get_instance().get_templates(e_ArenaRobotTemplate);
	//if (nullptr == robot_name_tmpl_table)
	//{
	//	return;
	//}
	//int64 cur_time = time_helper::get_time();

	//template_manager::template_type::iterator ite;
	//ws2dp_add_robot_worship add_worship_msg;
	//for (ite = robot_name_tmpl_table->begin(); ite != robot_name_tmpl_table->end(); ++ite)
	//{
	//	if (add_worship_msg.data_num >= rank_max)
	//	{
	//		break;
	//	}
	//	ArenaRobotTemplate* robot_template_ptr = (ArenaRobotTemplate*)(ite->second);
	//	if (robot_template_ptr != nullptr)
	//	{
	//		xstring robot_name = "???";
	//		StringTemplate* robot_name_template_ptr = GET_TEMPLATE(StringTemplate, robot_template_ptr->RobotNameId);
	//		if (robot_name_template_ptr->StrList.size() > 0)
	//		{
	//			robot_name = robot_name_template_ptr->StrList[0];
	//		}

	//		s_ranking_player_info temp_robot_data;
	//		temp_robot_data.ranking_type = robot_template_ptr->Rank;

	//		temp_robot_data.role_guid = ite->first;
	//		//反正是机器人，直接用TemplateId做Guid

	//		temp_robot_data.set_role_name(robot_name);
	//		temp_robot_data.ranking_value = ite->first;
	//		temp_robot_data.save_time = cur_time;

	//		m_ranking_all[e_RankingIndex_arena].push_back(temp_robot_data);

	//		add_worship_msg.data_array[add_worship_msg.data_num].role_guid = ite->first;
	//		add_worship_msg.data_array[add_worship_msg.data_num].worship_value = 0;
	//		add_worship_msg.data_array[add_worship_msg.data_num].save_time = cur_time;

	//		add_worship_msg.data_num++;
	//	}
	//}
	//ws_client::getInstance().send_to_dp(&add_worship_msg, add_worship_msg.get_pak_length());
}

void ranking_mgr_ws::save_worship_to_db(const guid_64& sender_guid, const s_ranking_player_info* target, int32& add_value, int32& worship_type)
{
	//if (nullptr == target)
	//{
	//	return;
	//}
	//ws2dp_save_ranking_worship req;
	//req.sender_guid = sender_guid;
	//req.target_data = *target;
	//req.target_data.Peak = 1;  //威望榜默认全存1
	//req.worship_add_value = add_value;
	//req.worship_type = worship_type;
	//ws_client::getInstance().send_to_dp(&req, sizeof(req));

	server2dp_proto_ws2dp_save_ranking_worship msg;
	server2dp_proto_s_ranking_player_info* player_info = msg.mutable_target_data();
	if (nullptr == player_info)
	{
		return;
	}
	player_info->set_role_guid(target->role_guid);
	player_info->set_role_name(target->role_name);
	player_info->set_ranking_type(target->ranking_type);
	player_info->set_ranking_value(target->ranking_value);
	player_info->set_save_time(target->save_time);
	player_info->set_supporting_guid(target->supporting_guid.server_64);
	player_info->set_supporting_name(target->supporting_name);
	player_info->set_vip_level(target->vip_level);
	player_info->set_peak(1);
	player_info->set_server_id(target->server_id);
	player_info->set_game_channel(target->game_channel);
	player_info->set_standby_parameter(target->standby_parameter);
	msg.set_sender_guid(sender_guid.server_64);
	msg.set_worship_add_value(add_value);
	msg.set_worship_type(worship_type);
	ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_ranking_worship);
}

int32 ranking_mgr_ws::insert_player_arena(s_ranking_player_info& player_info)
{
	if (player_info.ranking_type != e_RankingIndex_arena)
	{
		return -1;
	}
	if (m_ranking_all[e_RankingIndex_arena].empty())
	{
		m_ranking_all[e_RankingIndex_arena].push_back(player_info);
		return 1;
	}
	else
	{
		s_ranking_player_info list_head = m_ranking_all[e_RankingIndex_arena].front();
		s_ranking_player_info list_tail = m_ranking_all[e_RankingIndex_arena].back();

		if (player_info.ranking_value < list_head.ranking_value)
		{
			m_ranking_all[e_RankingIndex_arena].push_front(player_info);
			return 1;
		}
		else if(player_info.ranking_value > list_tail.ranking_value)
		{
			m_ranking_all[e_RankingIndex_arena].push_back(player_info);
			int32 ranking_num = m_ranking_all[e_RankingIndex_arena].size();
			return ranking_num;
		}
		else
		{
			ranking_list_ite arena_ite_high = m_ranking_all[e_RankingIndex_arena].begin();
			ranking_list_ite arena_ite_low = m_ranking_all[e_RankingIndex_arena].begin();
			arena_ite_low++;
			//能进行这么插入的说明 不会是第一
			for (int32 Index = 2; arena_ite_low != m_ranking_all[e_RankingIndex_arena].end(); Index++)
			{
				if ((arena_ite_high->ranking_value < player_info.ranking_value)
					&& (arena_ite_low->ranking_value > player_info.ranking_value))
				{
					m_ranking_all[e_RankingIndex_arena].insert(arena_ite_low, player_info);
					return Index;
				}
				arena_ite_high++;
				arena_ite_low++;
				if (Index > TopMaxNum)
				{
					break;		
				}
			}
		}
	}
	return -1;			//没有进榜
}


void ranking_mgr_ws::sync_player_arena(s_ranking_player_info& my_player_info, s_ranking_player_info& target_player_info, bool is_robot)
{
	//一定是 我击败目标 才会进入这个函数
	if (my_player_info.role_guid.is_valid() == false)
	{
		return;
	}

	if (!is_robot && target_player_info.role_guid.is_valid() == false)
	{
		return;
	}

	if (false == m_ranking_all[e_RankingIndex_arena].empty())
	{
		ranking_list_ite remove_arena_ite = m_ranking_all[e_RankingIndex_arena].begin();
		for (; remove_arena_ite != m_ranking_all[e_RankingIndex_arena].end(); )
		{
			if (remove_arena_ite->role_guid == my_player_info.role_guid)
			{
				ranking_list_ite temp_ite = remove_arena_ite++;
				m_ranking_all[e_RankingIndex_arena].erase(temp_ite);
			}
			else if (!is_robot && remove_arena_ite->role_guid == target_player_info.role_guid)
			{
				ranking_list_ite temp_ite = remove_arena_ite++;
				m_ranking_all[e_RankingIndex_arena].erase(temp_ite);
			}
			else
			{
				++remove_arena_ite;
			}
		}
	}
	bool is_binsert_my = false;
	bool is_binsert_target = false;
	if (arena_not_in_list_rank != my_player_info.ranking_value)
	{
		is_binsert_my = true;
	}

	if (!is_robot)
	{
		if (arena_not_in_list_rank != target_player_info.ranking_value)
		{
			is_binsert_target = true;
		}
	}
		
	//插入排行榜, 竞技场排名-1就是没有排名 不会进入排行榜中，要修改db中的排名值
	if (is_binsert_my)
	{
		insert_player_arena(my_player_info);
	}
	if (is_binsert_target)
	{
		insert_player_arena(target_player_info);
	}

	set_ranking_save_flag(my_player_info.ranking_type, true);
	//save_ranking_info_to_db(my_player_info);
	if (!is_robot)
	{
		set_ranking_save_flag(target_player_info.ranking_type, true);
		//save_ranking_info_to_db(target_player_info);
	} 

	while (m_ranking_all[e_RankingIndex_arena].size() > TopMaxNum)
	{
		m_ranking_all[e_RankingIndex_arena].pop_back();
	}

	// 竞技场前十更换公告
	if (MinRankingPost <= my_player_info.ranking_value &&  my_player_info.ranking_value <= MaxRankingPost)
	{
		int32 notice_id = 93000031;
		std::string player_name = std::string(my_player_info.role_name);	//角色名
		int32 create_time = time_helper::get_cur_time_new().second;
		std::string ranking = template_manager::get_instance().int_to_string(my_player_info.ranking_value + 1);

		std::vector<std::string> vec_str;
		vec_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_str.push_back(player_name);
		vec_str.push_back(ranking);
		std::string notice_str = init_unit::implode(vec_str);
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, my_player_info.role_guid, notice_str);
	}
}

void ranking_mgr_ws::sync_after_merge_first_player_deal(s_ranking_player_info& my_player_info) //出现竞技场排行第一名玩家数据没存入 ranking 库 bug，进行特殊逻辑处理
{
	if (my_player_info.role_guid.is_valid() == false)
	{
		return;
	}
	bool is_binsert_my = false;
	if (arena_not_in_list_rank != my_player_info.ranking_value)
	{
		is_binsert_my = true;
	}
	if (is_binsert_my)
	{
		s_ranking_player_info list_head = m_ranking_all[e_RankingIndex_arena].front();
		if (my_player_info.ranking_value < list_head.ranking_value)
		{
			m_ranking_all[e_RankingIndex_arena].push_front(my_player_info);
		}
	}
	while (m_ranking_all[e_RankingIndex_arena].size() > TopMaxNum)
	{
		m_ranking_all[e_RankingIndex_arena].pop_back();
	}
}
void ranking_mgr_ws::sync_player_arena_lua(const char * my_str, int32 my_len, const char * dst_str, int32 dst_len, bool is_robot)
{
	if (my_str == nullptr || dst_str == nullptr)
	{
		return;
	}
	hld::st_proto::st_ranking_player_info st_tmp;
	bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&st_tmp, my_str, my_len, false);
	if (is_sucess == false)
	{
		return;
	}

	s_ranking_player_info my_player_info;

	my_player_info.from_proto(st_tmp);

	st_tmp.Clear();

	is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&st_tmp, dst_str, dst_len, false);
	if (is_sucess == false)
	{
		return;
	}

	s_ranking_player_info target_player_info;


	target_player_info.from_proto(st_tmp);

	sync_player_arena(my_player_info, target_player_info, is_robot);
 
}


void ranking_mgr_ws::sync_player_arena_gs(const guid_64& role_guid, int64 gs_value)
{
	ranking_list_ite ite = m_ranking_all[e_RankingIndex_arena].begin();
	for (; ite != m_ranking_all[e_RankingIndex_arena].end();++ite)
	{
		if (role_guid == (*ite).role_guid)
		{
			(*ite).supporting_guid.server_64 = gs_value;
		}
	}
}

s_ranking_player_info* ranking_mgr_ws::get_ranking_player_info_by_type(e_RankingIndex ranking_index, const guid_64& target_guid)
{
	if (target_guid.is_valid() == false)
	{
		return nullptr;
	}
	ranking_list* list_ptr = get_ranking_list_by_type(ranking_index);
	if (nullptr == list_ptr)
	{
		return nullptr;
	}
	ranking_list_ite ite = list_ptr->begin();
	for (; ite != list_ptr->end(); ++ite)
	{
		if (ite->role_guid == target_guid)
		{
			return &(*ite);
		}
	}
	return nullptr;
}


ranking_list* ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex ranking_index)
{
	switch (ranking_index)
	{
	case hld::e_RankingIndex_legion_core:
	case hld::e_RankingIndex_legion_territory:
	case hld::e_RankingIndex_legion_boss:
	case hld::e_RankingIndex_legion_bonfire:
	case hld::e_RankingIndex_world_boss_one:	
	case hld::e_RankingIndex_world_boss_two:
	case hld::e_RankingIndex_world_boss_three:
	case hld::e_RankingIndex_world_boss_four:
	case hld::e_RankingIndex_world_boss_five:
	case hld::e_RankingIndex_world_boss_six:
	case hld::e_RankingIndex_world_boss_seven:
	case hld::e_RankingIndex_world_elite_one:
	case hld::e_RankingIndex_world_elite_two:
	case hld::e_RankingIndex_world_elite_three:
	case hld::e_RankingIndex_world_elite_four:
	case hld::e_RankingIndex_world_elite_five:
	case hld::e_RankingIndex_world_elite_six:
	case hld::e_RankingIndex_world_elite_seven:
	case hld::e_RankingIndex_world_elite_eight:
	{
		return legion_ws_mgr::get_instance().get_legion_ranking_by_ranking_type(ranking_index);
	}
		break;
	default:
		return &m_ranking_all[ranking_index];
		break;
	}
	return nullptr;
}
s_ranking_player_info*  ranking_mgr_ws::get_ranking_info_by_type(e_RankingIndex ranking_index, int32 begin_index, int32 end_index)
{
	ranking_list* ranking_list_ptr = get_ranking_list_by_type(ranking_index);
	if ( nullptr == ranking_list_ptr)
	{
		return nullptr;
	}
	if (begin_index > end_index)
	{
		int32 temp_index = begin_index;
		begin_index = end_index;
		end_index = temp_index;
	}
	if (end_index == 0 || ranking_list_ptr->size() < end_index)
	{
		return nullptr;
	}
	int32 rand_index = random_gen::get_random(begin_index, end_index);
	int32 add_index = 0;
	for (ranking_list_ite it = ranking_list_ptr->begin(); it != ranking_list_ptr->end(); ++it)
	{
		add_index++;
		if (add_index >= rand_index)
		{
			return &(*it);
		}
	}
	return nullptr;
}
bool ranking_mgr_ws::is_legion_ranking(e_RankingIndex ranking_index)
{
	switch (ranking_index)
	{ 
	case hld::e_RankingIndex_legion_core:
	case hld::e_RankingIndex_legion_territory:
	case hld::e_RankingIndex_legion_boss:
	case hld::e_RankingIndex_legion_bonfire:
	case hld::e_RankingIndex_world_boss_one:
	case hld::e_RankingIndex_world_boss_two:
	case hld::e_RankingIndex_world_boss_three:
	case hld::e_RankingIndex_world_boss_four:
	case hld::e_RankingIndex_world_boss_five:
	case hld::e_RankingIndex_world_boss_six:
	case hld::e_RankingIndex_world_boss_seven:
	case hld::e_RankingIndex_world_elite_one:
	case hld::e_RankingIndex_world_elite_two:
	case hld::e_RankingIndex_world_elite_three:
	case hld::e_RankingIndex_world_elite_four:
	case hld::e_RankingIndex_world_elite_five:
	case hld::e_RankingIndex_world_elite_six:
	case hld::e_RankingIndex_world_elite_seven:
	case hld::e_RankingIndex_world_elite_eight:

	case hld::e_RankingIndex_cross_boss_one: //跨服世界boss1
	case hld::e_RankingIndex_cross_boss_two: //`跨服世界boss2
	case hld::e_RankingIndex_cross_boss_three: //跨服世界boss3
	case hld::e_RankingIndex_cross_boss_four: //跨服世界boss4
	case hld::e_RankingIndex_cross_boss_five: //跨服世界boss5
	case hld::e_RankingIndex_cross_boss_six: //跨服世界boss6
	case hld::e_RankingIndex_cross_boss_seven: //跨服世界boss7
	case hld::e_RankingIndex_cross_boss_eight: //跨服世界boss8
	case hld::e_RankingIndex_cross_boss_nine: //跨服世界boss9
	case hld::e_RankingIndex_cross_boss_ten: //跨服世界boss10
	case hld::e_RankingIndex_cross_boss_eleven: //跨服世界boss11
	case hld::e_RankingIndex_cross_boss_tweleve: //跨服世界boss12
	{
		return true;
	}
		break;
	default:
		break;
	}
	return false;
}


bool ranking_mgr_ws::make_ranking_data(ranking_proto_ranking_player_info_lua* ranking_player_data,const s_ranking_player_info& player_data)
{
	if (nullptr ==  ranking_player_data)
	{
		return false;
	}
	ranking_player_data->set_player_name(player_data.role_name);
	ranking_player_data->set_role_guid_a(player_data.role_guid.A);
	ranking_player_data->set_role_guid_b(player_data.role_guid.B);
	ranking_player_data->set_ranking_type(player_data.ranking_type);
	ranking_player_data->set_ranking_value(player_data.ranking_value);
	ranking_player_data->set_supporting_guid_a(player_data.supporting_guid.A);
	ranking_player_data->set_supporting_guid_b(player_data.supporting_guid.B);
	ranking_player_data->set_supporting_name(player_data.supporting_name);
	ranking_player_data->set_vip_level(player_data.vip_level);
	ranking_player_data->set_peak(player_data.Peak);
	ranking_player_data->set_server_id(player_data.server_id);
	ranking_player_data->set_standby_parameter(player_data.standby_parameter);
	//目前这里未增加渠道号字段
	return true;
}

bool ranking_mgr_ws::make_ranking_data(ranking_proto_ranking_player_info* ranking_player_data, const s_ranking_player_info& player_data)
{
	if (nullptr == ranking_player_data)
	{
		return false;
	}
	ranking_player_data->set_player_name(player_data.role_name);
	ranking_player_data->set_role_guid(player_data.role_guid.server_64);
	ranking_player_data->set_ranking_type(player_data.ranking_type);
	ranking_player_data->set_ranking_value(player_data.ranking_value);
	ranking_player_data->set_supporting_guid(player_data.supporting_guid.server_64);
	ranking_player_data->set_supporting_name(player_data.supporting_name);
	ranking_player_data->set_vip_level(player_data.vip_level);
	ranking_player_data->set_peak(player_data.Peak);
	ranking_player_data->set_server_id(player_data.server_id);
	ranking_player_data->set_standby_parameter(player_data.standby_parameter);
	//目前这里未增加渠道号字段
	return true;
}

void ranking_mgr_ws::c2ws_get_ranking_list_func(e_RankingIndex ranking_index, const guid_64& role_guid)
{
	if (!m_loading_end_flag)
	{
		return;
	}
	if (ranking_index >= e_RankingIndex_max)
	{
		return;
	}

	client_session* session = client_session_mgr::getInstance().get_session(role_guid);
	if (nullptr == session)
	{
		return;
	}
	if (ranking_index == e_RankingIndex_cross_ladder_cur_season || ranking_index == e_RankingIndex_cross_ladder_last_seaon || 
		ranking_index ==  e_RankingIndex_element_war_cur_season || ranking_index ==  e_RankingIndex_element_war_last_season)
	{
		int32 cross_group_ip = cross_ladder_ws_mgr_new::get_instance().get_match_server_id();
		int32 server_id = world_server::getInstance().get_server_id();
		ws2gate_all_gs_ranking msg;
		msg.server_id = server_id;
		msg.role_guid = role_guid;
		msg.rank_index = ranking_index;
		cross::send_msg_to_ws(session->get_role_guid(), cross_group_ip, e_msgindex_ws2ws_req_all_gs_ranking, &msg, sizeof(msg));
		return;
	}
	else if (init_unit::is_rank_type_cross_server(ranking_index) && world_server::getInstance().get_server_id() != world_server::getInstance().get_cross_id())
	{
		int32 cross_group_ip = world_server::getInstance().get_server_id();// session->get_role_info_data(e_role_info_server_id);
		int32 server_id = world_server::getInstance().get_cross_id();
		ws2gate_all_gs_ranking msg;
		msg.server_id = cross_group_ip;
		msg.role_guid = role_guid;
		msg.rank_index = ranking_index;
		cross::send_msg_to_ws(session->get_role_guid(), server_id, e_msgindex_ws2ws_req_all_gs_ranking, &msg, sizeof(msg));
		return;
	}
	ranking_proto_get_top_list_end  resp;
	resp.set_ranking_index(ranking_index);
	ranking_list* ranking_list_ptr = get_ranking_list_by_type(ranking_index);
	if (nullptr == ranking_list_ptr)
	{
		send_operate_result_to_client(role_guid, e_ranking_operate_result_error);
		return;
	}
	if (ranking_list_ptr->size() <= 0)
	{
		send_operate_result_to_client(role_guid, e_ranking_operate_result_ranking_no_player);
		return;
	}
	int32 top_count = 1;
	bool is_first_send = true;
	for (ranking_list_ite ite = ranking_list_ptr->begin(); ite != ranking_list_ptr->end(); ++ite, ++top_count)
	{
		if (resp.ByteSize() > packege_length_max)
		{
			if (is_first_send)
			{
				is_first_send = false;
				resp.set_if_first_send(1);
			}
			else
			{
				resp.set_if_first_send(0);
			}
			resp.set_if_last_send(0);
			session->send_to_client(&resp, e_msgindex_s2c_ranking_get_top_end);
			resp.clear_top_player_info();
		}
		auto temp_info = resp.add_top_player_info();
		make_ranking_data(temp_info, (*ite));
		if (top_count >= ShowTopMaxNum)
		{
			break;
		}
	}
	if (is_first_send)
	{
		resp.set_if_first_send(1);
	}
	else
	{
		resp.set_if_first_send(0);
	}
	resp.set_if_last_send(1);
	session->send_to_client(&resp, e_msgindex_s2c_ranking_get_top_end);
}

void ranking_mgr_ws::ws2gws_get_ranking_list_func(const void* data_ptr, size_t data_len)
{
	ws2gate_all_gs_ranking* cross_info_ptr = (ws2gate_all_gs_ranking*)data_ptr; 

	if (cross_info_ptr==nullptr)
	{
		return;
	}
	int32 server_id = cross_info_ptr->server_id;
	guid_64 role_guid = cross_info_ptr->role_guid;
	int32 rank_index = cross_info_ptr->rank_index;
	if (!m_loading_end_flag)
	{
		return;
	}
	ranking_proto_get_top_list_end_lua  resp;
	resp.set_ranking_index(rank_index);
	ranking_list* ranking_list_ptr = get_ranking_list_by_type((e_RankingIndex)rank_index);
	if (nullptr == ranking_list_ptr)
	{
		return;
	}
	if (ranking_list_ptr->size() <= 0)
	{
		gate2ws_get_ranking_list_error result_msg;
		result_msg.role_guid = role_guid;
		result_msg.result = e_ranking_operate_result_ranking_no_player;
		cross::send_msg_to_ws(role_guid, server_id, e_msgindex_ws2ws_get_ranking_list_error, &result_msg, sizeof(result_msg));
		return;
	}
	int32 top_count = 1;
	bool is_first_send = true;
	for (ranking_list_ite ite = ranking_list_ptr->begin(); ite != ranking_list_ptr->end(); ++ite, ++top_count)
	{
		if (resp.ByteSize() > packege_length_max)
		{
			if (is_first_send)
			{
				is_first_send = false;
				resp.set_if_first_send(1);
			}
			else
			{
				resp.set_if_first_send(0);
			}
			resp.set_if_last_send(0);							
			cross::send_msg_to_ws(role_guid, server_id, e_msgindex_ws2ws_res_all_gs_ranking, &resp);
			resp.clear_top_player_info();
		}
		ranking_proto_ranking_player_info_lua* temp_info = resp.add_top_player_info();
		make_ranking_data(temp_info, (*ite));
		if (top_count >= ShowTopMaxNum)
		{
			break;
		}
	}
	if (is_first_send)
	{
		resp.set_if_first_send(1);
	}
	else
	{
		resp.set_if_first_send(0);
	}
	resp.set_if_last_send(1);
	cross::send_msg_to_ws(role_guid, server_id, e_msgindex_ws2ws_res_all_gs_ranking, &resp);
}

void ranking_mgr_ws::c2ws_get_my_rank_func(e_RankingIndex ranking_type, const guid_64& sender_guid, const guid_64& legion_guid)
{
	if (ranking_type >= e_RankingIndex_max)
	{
		return;
	}
	if (sender_guid.is_valid() == false)
	{
		return;
	}
	client_session*	session = client_session_mgr::getInstance().get_session(sender_guid);
	if (nullptr == session)
	{
		return;
	}
	int32 gate_server_id = world_server::getInstance().get_cross_id();	
	int32 world_server_id = world_server::getInstance().get_server_id();
	int32 ranking_level = -1;
	int64 ranking_value = 0;
	if(legion_guid.is_valid() == true)
	{
		if (init_unit::is_rank_type_cross_server(ranking_type) && world_server_id != gate_server_id)
		{
			ws2gate_my_rank msg;
			msg.player_guid = legion_guid;
			msg.sender_guid = sender_guid;
			msg.server_id = world_server_id;// session->get_role_info_data(e_role_info_server_id);
			msg.rank_type = ranking_type;
			cross::send_msg_to_ws(legion_guid, gate_server_id, e_msgindex_ws2ws_req_get_my_rank_func, &msg, sizeof(msg));
			return;
		}
		get_player_rank_level(ranking_type, legion_guid, ranking_level, ranking_value);
	}
	else
	{
		if (init_unit::is_rank_type_cross_server(ranking_type) && world_server_id != gate_server_id)
		{
			ws2gate_my_rank msg;
			msg.player_guid = sender_guid;
			msg.sender_guid = sender_guid;
			msg.server_id = world_server_id;// session->get_role_info_data(e_role_info_server_id);
			msg.rank_type = ranking_type;
			cross::send_msg_to_ws(sender_guid, gate_server_id, e_msgindex_ws2ws_req_get_my_rank_func, &msg, sizeof(msg));
			return;
		}
		get_player_rank_level(ranking_type, sender_guid, ranking_level, ranking_value);
		if (ranking_type == e_RankingIndex_couple && -1 == ranking_level)
		{
			get_player_rank_level(ranking_type, sender_guid, ranking_level, ranking_value);
			guid_64 main_role_id = marry_mgr_ws::get_instance().get_main_role_guid(sender_guid);
			if(main_role_id.is_valid() && main_role_id != sender_guid)
				get_player_rank_level(ranking_type, main_role_id, ranking_level, ranking_value);
		}
	}

		
	ranking_proto_get_my_rank_end pak;
	pak.set_rank_type(ranking_type);
	pak.set_rank_level(ranking_level);
	pak.set_rank_value(ranking_value);
	session->send_to_client(&pak, e_msgindex_s2c_ranking_get_my_rank_end);
}

void ranking_mgr_ws::ws2gate_get_my_rank_func( const guid_64& player_guid, int32 server_id, const guid_64& role_guid, int32 ranking_type)
{
	if (player_guid.is_valid() == false || !init_unit::is_rank_type_cross_server(ranking_type))
	{
		return;
	}
	int32 ranking_level = -1;
	int64 ranking_value = 0;
	get_player_rank_level(ranking_type, player_guid, ranking_level, ranking_value);
	//client_session*	session = client_session_mgr::getInstance().get_session(role_guid);
	//if (nullptr == session)
	//{
	//	return;
	//}
	ranking_proto_get_my_rank_end pak;
	pak.set_rank_type(ranking_type);
	pak.set_rank_level(ranking_level);
	pak.set_rank_value(ranking_value);
	cross::send_msg_to_ws(role_guid, server_id, e_msgindex_ws2ws_res_get_my_rank_func, &pak);
	//session->send_to_client(&pak, e_msgindex_s2c_ranking_get_my_rank_end);
}

void ranking_mgr_ws::send_operate_result_to_client(const guid_64& sender_guid, const int32& result_id)
{
	if (result_id < 0 )
	{
		return;
	}
	client_session*	session = client_session_mgr::getInstance().get_session(sender_guid);
	if (nullptr == session)
	{
		return;
	}
	ranking_proto_ranking_operate_result result_pak;
	result_pak.set_operate_result(result_id);
	session->send_to_client(&result_pak, e_msgindex_s2c_ranking_operate_result);
}
	

void ranking_mgr_ws::get_player_worship_value(const guid_64& sender_guid, const guid_64& target_guid)
{
	if (sender_guid.is_valid() == false || target_guid.is_valid() == false )
	{
		return;
	}
	//ws2dp_get_worship_value pak;
	//pak.sender = sender_guid;
	//pak.target = target_guid;
	//ws_client::getInstance().send_to_dp(&pak, sizeof(pak));

	script_mgr::get_instance().call_func("ws2dp_send", "send_load_player_worship_value_lua", 0, false, "%l%l", sender_guid.server_64, target_guid.server_64);
}


void ranking_mgr_ws::get_player_worship_value_end(bool is_load, const guid_64& sender_guid, const s_ranking_player_info& ranking_info)
{
	if (sender_guid.is_valid() == false )
	{
		return;
	}

	if (false == is_load)
	{
		send_operate_result_to_client(sender_guid, e_ranking_operate_result_no_worship_value);
		return;
	}

	hld::ranking_proto_get_player_ranking_value_end  response;
	hld::ranking_proto_ranking_player_info* target_player_info = response.mutable_target_player_info();
	if (false == make_ranking_data(target_player_info, ranking_info))
	{
		return;
	}
	client_session*	session = client_session_mgr::getInstance().get_session(sender_guid);
	if (nullptr == session)
	{
		return;
	}
	session->send_to_client(&response, e_msgindex_s2c_get_player_ranking_value_end);
}

void ranking_mgr_ws::open_server_first_day_proc()
{
	static bool m_had_send = false;
	if (!m_had_send)
	{
		if (globle_data::get_instance().get_server_on_days() > 1)
		{
			for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max; ++ranking_type)
			{
				ranking_list* ranking_ptr = get_ranking_list_by_type((e_RankingIndex)ranking_type);
				if (nullptr == ranking_ptr)
				{
					continue;
				}
				if (ranking_ptr->size() > 0)
				{
					s_ranking_player_info new_top = ranking_ptr->front();
					s_ranking_player_info old_top;
					old_top.ranking_type = new_top.ranking_type;
					sync_first_rank_player_info(old_top, new_top, false);
				}
			}
			m_had_send = true;
		}
	}
}

void ranking_mgr_ws::send_gs_ranking(e_RankingIndex get_ranking_type, e_RankingIndex cross_ranking_type)
{
	if (true == gate_proxy::getInstance().is_gate_run() )
	{
		if (world_server::getInstance().get_server_id() != world_server::getInstance().get_cross_id())
		{
			int32 server_id = world_server::getInstance().get_cross_id();
			if (server_id <= 0)
			{
				return;
			}
			ranking_list* ranking_list_ptr = get_ranking_list_by_type(get_ranking_type);
			if (nullptr == ranking_list_ptr)
			{
				//send_operate_result_to_client(role_guid, e_ranking_operate_result_error);
				return;
			}
			if (ranking_list_ptr->size() <= 0)
			{
				//send_operate_result_to_client(role_guid, e_ranking_operate_result_ranking_no_player);
				return;
			}
			ws2gate_gs_ranking  msg;
			msg.server_id = world_server::getInstance().get_server_id();
			msg.ranking_index = cross_ranking_type;
			int32 i = 0;
			bool is_first_send = true;
			for (ranking_list_ite ite = ranking_list_ptr->begin(); ite != ranking_list_ptr->end(); ++ite)
			{
				if (i % WS_RANKING_LENGTH == 0 && i != 0 )
				{
					if (is_first_send)
					{
						is_first_send = false;
						msg.if_first_send = 1;
					}
					else
					{
						msg.if_first_send = 0;
					}
					msg.if_last_send = 0;
					msg.data_num = WS_RANKING_LENGTH;
					cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_res_gs_ranking, &msg, sizeof(msg));
					msg.clear_data_array();
					i -= WS_RANKING_LENGTH;
				}
				msg.data_ary[i % WS_RANKING_LENGTH] = *ite;
				msg.data_ary[i % WS_RANKING_LENGTH].ranking_type = cross_ranking_type;
				i++;
			}
			msg.data_num = i;
			if (is_first_send)
			{
				msg.if_first_send = 1;
			}
			else
			{
				msg.if_first_send = 0;
			}
			msg.if_last_send = 1;
			cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_res_gs_ranking, &msg, sizeof(msg));
		}
	}
}

void ranking_mgr_ws::get_gs_ranking(const void* data_ptr, size_t data_len)
{
	ws2gate_gs_ranking* cross_info_ptr = (ws2gate_gs_ranking*)data_ptr;
	if (cross_info_ptr == nullptr)
		return;
	int32 index = -1;
	e_RankingIndex rank_idex = (e_RankingIndex)cross_info_ptr->ranking_index;
	if (!init_unit::is_rank_type_cross_server(rank_idex))
	{
		return;
	}
	if (cross_info_ptr->if_first_send == 1)
	{
		m_ranking_top_group[cross_info_ptr->server_id].get_rank_list(rank_idex).clear();
	}
	for (int32 i = 0; i < cross_info_ptr->data_num && i < WS_RANKING_LENGTH ; i++)
	{
		cross_info_ptr->data_ary[i].server_id = cross_info_ptr->server_id;
		m_ranking_top_group[cross_info_ptr->server_id].get_rank_list(rank_idex).push_back(cross_info_ptr->data_ary[i]);
	}
	if (cross_info_ptr->if_last_send == 1)
	{
		m_ranking_all[rank_idex] = delete_ranking_by_server_id(m_ranking_all[rank_idex], cross_info_ptr->server_id);
		m_ranking_all[rank_idex] = merge_ranking(m_ranking_all[rank_idex], m_ranking_top_group[cross_info_ptr->server_id].get_rank_list(rank_idex));
	}
}

void ranking_mgr_ws::delete_ranking_with_server_list_refresh_end()
{
	delete_ranking_with_server_lits_refresh_by_type(e_RankingIndex_gs_all);
	delete_ranking_with_server_lits_refresh_by_type(e_RankingIndex_legion_gs_all);
	delete_ranking_with_server_lits_refresh_by_type(e_RankingIndex_level_all);
	delete_ranking_with_server_lits_refresh_by_type(e_RankingIndex_Oracle_Trial);
}

void ranking_mgr_ws::delete_ranking_with_server_lits_refresh_by_type(e_RankingIndex ranking_type)
{
	ranking_list* list_ptr = get_ranking_list_by_type(ranking_type);
	if (nullptr == list_ptr)
	{
		return;
	}
	ranking_list_ite ite = list_ptr->begin();
	for (; ite != list_ptr->end();)
	{
		if (!world_server::getInstance().is_have_this_server(ite->server_id))
		{
			ite = list_ptr->erase(ite);
		}
		else
		{
			++ite;
		}
	}
	return;
}

ranking_list ranking_mgr_ws::delete_ranking_by_server_id(ranking_list ranking, int32 server_id)
{
	ranking_list ranking_all;
	ranking_all.clear();
	ranking_list_ite player_info = ranking.begin();
	int32 i = 0;
	for (i = 0; i < TopMaxNum; i++)
	{
		if (player_info == ranking.end())
		{
			break;
		}
		if ((*player_info).server_id != server_id)
		{
			ranking_all.push_back(*player_info);				
		}
		player_info++;
	}
	return ranking_all;
}
ranking_list ranking_mgr_ws::merge_ranking(ranking_list ranking1,ranking_list ranking2)
{
	ranking_list ranking_all;
	ranking_all.clear();
	ranking_list_ite player_info1 = ranking1.begin();
	ranking_list_ite player_info2 = ranking2.begin();
	int32 i = 0;
	int32 j = 0;
	for ( i = 0; i < TopMaxNum ; i++)
	{
		if (player_info1 == ranking1.end())
		{
			j = 1;
			break;
		}
		else if (player_info2 == ranking2.end())
		{
			j = 2;
			break;
		}
		if ((*player_info1).role_guid == (*player_info2).role_guid)
		{
			ranking_all.push_back(*player_info2);
			player_info1++;
			player_info2++;
		}
		else if ((*player_info1).ranking_value >= (*player_info2).ranking_value)
		{				
			ranking_all.push_back(*player_info1);
			player_info1++;

		}
		else
		{				
			ranking_all.push_back(*player_info2);
			player_info2++;
		}
	}
	if (j == 1)
	{
		while(i++ < TopMaxNum)
		{
			if (player_info2 == ranking2.end())
			{
				break;
			}
			ranking_all.push_back(*player_info2);
			player_info2++;
		}
	}
	else if (j == 2)
	{
		while (i++ < TopMaxNum)
		{
			if (player_info1 == ranking1.end())
			{
				break;
			}
			ranking_all.push_back(*player_info1);
			player_info1++;

		}
	}
	return ranking_all;
}
void ranking_mgr_ws::c2ws_get_player_ranking_info_func(e_RankingIndex ranking_type, const guid_64& sender_guid, const guid_64& target_guid)
{
	if (sender_guid.is_valid() == false || target_guid.is_valid() == false)
	{
		send_operate_result_to_client(sender_guid, e_ranking_operate_result_error);
		return;
	}
		
	switch (ranking_type)
	{
	case e_RankingIndex_gs:
	case e_RankingIndex_level:
	case e_RankingIndex_box_map_level:
	case e_RankingIndex_demonstower:
	case e_RankingIndex_wing:
	case e_RankingIndex_mount:
	case e_RankingIndex_legion_core:
	case e_RankingIndex_gs_barserker:
	case e_RankingIndex_gs_wizard:
	case e_RankingIndex_gs_guardian:
	case e_RankingIndex_gs_assassinator:
	case e_RankingIndex_level_barserker:
	case e_RankingIndex_level_wizard:
	case e_RankingIndex_level_guardian:
	case e_RankingIndex_level_assassinator:
	case e_RankingIndex_equipment_main:
	case e_RankingIndex_arena:
	case e_RankingIndex_element_rank_list:
	{
		s_ranking_player_info* target_player = get_ranking_player_info_by_type(ranking_type , target_guid);
		if (nullptr == target_player)
		{
			send_operate_result_to_client(sender_guid, e_ranking_operate_result_error);
			return;
		}
		ranking_proto_get_player_ranking_value_end pak;
		ranking_proto_ranking_player_info* temp_pak = pak.mutable_target_player_info();
		if (false == make_ranking_data(temp_pak, *target_player))
		{
			return;
		}
		client_session*	session = client_session_mgr::getInstance().get_session(sender_guid);
		if (nullptr == session)
		{
			return;
		}
		session->send_to_client(&pak, e_msgindex_s2c_get_player_ranking_value_end);
	}
		break;
	case e_RankingIndex_worship:
	{
		get_player_worship_value(sender_guid, target_guid);
	}
		break;
	case e_RankingIndex_max:
		break;
	default:
		break;
	}


}


void ranking_mgr_ws::cs2ws_sync_player_vip_level_func(const guid_64& role_guid, const int32& vip_level)
{
	if (role_guid.is_valid() == false)
	{
		return;
	}

	for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max; ranking_type++)
	{
		s_ranking_player_info* player_ranking = get_ranking_player_info_by_type((e_RankingIndex)ranking_type, role_guid);
		if (nullptr == player_ranking)
		{
			continue;
		}
		player_ranking->vip_level= vip_level;
		set_ranking_save_flag(ranking_type, true);
	}
	//ws2dp_update_ranking_vip_level req;
	//req.role_guid = role_guid;
	//req.vip_level = vip_level;
	//ws_client::getInstance().send_to_dp(&req, sizeof(req));
}

/************************************************************************/

void ranking_mgr_ws::sync_player_info(const s_ranking_player_info& player_info)
{
	int32 ranking_index = player_info.ranking_type;
	if (ranking_index< 0 || ranking_index >= e_RankingIndex_max)
	{
		return;
	}
	s_game_info*  game_info_ptr = world_server::getInstance().get_server_config_by_server_id(player_info.server_id);
	if (game_info_ptr || cross_ladder_ws_mgr_new::get_instance().is_match_server())
	{
		ranking_list* ranking_ptr = get_ranking_list_by_type((e_RankingIndex)ranking_index);
		if (nullptr == ranking_ptr)
		{
			return;
		}
		s_ranking_player_info old_first_player_info;
		if (ranking_ptr->empty() == false)
		{
			ranking_list_ite ite_remove = ranking_ptr->begin();
			old_first_player_info = *ite_remove;
			for (; ite_remove != ranking_ptr->end(); ite_remove++)
			{
				if (player_info.role_guid == (*ite_remove).role_guid)
				{
					if (player_info.ranking_value != (*ite_remove).ranking_value)
					{
						//改排行榜是否刷新是由跨服判断的 增加判断防止本服刷新数据导致数据出现失误
						if (ranking_index == e_RankingIndex_cross_time_limit_activity_total_recharge && player_info.ranking_value < (*ite_remove).ranking_value)
						{
							return;
						}
						ranking_ptr->erase(ite_remove);
						break;
					}
					else
					{
						return;
					}
				}
			}
		}
		if (sync_player_info_insert(player_info, ranking_ptr))
		{
			set_ranking_save_flag(player_info.ranking_type, true);
		}

		while (ranking_ptr->size() > TopMaxNum)
		{
			ranking_ptr->pop_back();
		}

		s_ranking_player_info new_first_player_info;
		if (ranking_ptr->empty() == false)
		{
			new_first_player_info = ranking_ptr->front();
		}

		sync_first_rank_player_info(old_first_player_info, new_first_player_info);
	}
}

bool ranking_mgr_ws::sync_player_info_insert(const s_ranking_player_info& player_info, ranking_list* ranking_ptr)
{
	if (nullptr == ranking_ptr)
	{
		return false;
	}

	if (ranking_ptr->empty())
	{
		ranking_ptr->push_back(player_info);
		return true;
	}

	s_ranking_player_info list_head = ranking_ptr->front();
	s_ranking_player_info list_tail = ranking_ptr->back();

	if (player_info.compare(list_head) > 0)
	{
		ranking_ptr->push_front(player_info);
		return true;
	}
	else if (player_info.compare(list_tail) <= 0)
	{
		ranking_ptr->push_back(player_info);
		return true;
	}
	else
	{
		ranking_list_ite it_end = ranking_ptr->end();
		ranking_list_ite it_next = ranking_ptr->begin();
		for (ranking_list_ite it_begin = ranking_ptr->begin(); it_begin != it_end; ++it_begin)
		{
			++it_next;
			if (it_next == it_end)
			{
				return false;
			}
			if (player_info.compare(*it_begin) <= 0
				&& player_info.compare(*it_next) > 0)
			{
				ranking_ptr->insert(it_next, player_info);
				return true;
			}
		}
	}

	return false;
}

void ranking_mgr_ws::cs2ws_req_add_worship_func(e_RankingIndex ranking_index, int32 worship_type, const guid_64& sender_guid, const guid_64& target_guid)
{
	if (worship_type < 0
		|| worship_type >= GAMECONFIG->WorshipAddValueArray.size())
	{
		return;
	}
	client_session* sender_ptr = client_session_mgr::getInstance().get_session(sender_guid);
	if (sender_ptr == nullptr)
	{
		if (worship_type == e_ranking_worship_type_money)
		{
			if (GAMECONFIG->MoneyWorshipCost.size() >= 2)
			{
				s_mail_info temp_mail_info;
				std::vector<guid_64> temp_item_guid_array;
				init_unit::init_mail_data_array(temp_mail_info, temp_item_guid_array
					, GAMECONFIG->MoneyWorshipCost[0], GAMECONFIG->MoneyWorshipCost[1], 0, 0
					, 0);

				const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
				const xchar* mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_worship_failed_title);
				const xchar* mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_worship_failed_context);
				if (sender_name != nullptr
					&& mail_title != nullptr
					&& mail_content != nullptr)
				{
					int32 string_len = std::strlen(sender_name);
					if (string_len> max_name_size)
					{
						string_len = max_name_size;
					}
					memcpy(temp_mail_info.sender_name, sender_name, string_len);

					string_len = std::strlen(mail_title);
					if (string_len > max_mail_title_length)
					{
						string_len = max_mail_title_length;
					}
					memcpy(temp_mail_info.mail_title, mail_title, string_len);


					string_len = std::strlen(mail_content);
					if (string_len > max_text_contents_length)
					{
						string_len = max_text_contents_length;
					}
					memcpy(temp_mail_info.text_contents, mail_content, string_len);

					mail_ws_mgr::get_instance().send_mail(sender_guid, 0, temp_mail_info, nullptr, 0);
				}
			}
		}
		return;
	}


	//到榜中找名字
	ranking_list* ranking_ptr = get_ranking_list_by_type(ranking_index);
	if (nullptr == ranking_ptr)
	{
		return;
	}
	s_ranking_player_info* target_data = nullptr; 
	bool is_legion_type  = is_legion_ranking((e_RankingIndex)ranking_index); 
	if (is_legion_type)
	{
		ranking_list_ite ite = ranking_ptr->begin();
		for (; ite != ranking_ptr->end(); ++ite)
		{ 
			if (target_guid == ite->supporting_guid)		//有可能是军团排行榜 增加的是军团长的威望
			{
				target_data = &(*ite);
				break;
			}
		}
	}
	else
	{
		ranking_list_ite ite = ranking_ptr->begin();
		for (; ite != ranking_ptr->end(); ++ite)
		{
			if (target_guid == ite->role_guid)
			{
				target_data = &(*ite);
				break;
			} 
		}
	}
	if (target_data == nullptr)
	{
		ws2cs_req_worship_player_end msg;
		msg.role_guid = sender_guid;
		msg.target_guid = target_guid;
		msg.ranking_type = ranking_index;
		msg.worship_type = worship_type;
		msg.target_worship_value = 0;
		msg.worship_result = EWorshipEndType_unknown;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_req_worship_player_end);
		if (is_use == false)
		{
			sender_ptr->send_to_cs(&msg, sizeof(ws2cs_req_worship_player_end));
		}
		else
		{
			hld::ws2cs_proto::req_worship_player_end pro_msg;
			msg.to_proto(pro_msg);
			sender_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_req_worship_player_end);

		}
		return;
	}

	//发消息给db 让db去增加 并返回增加后的值给ws ，ws接到消息后同步给target和sender，并且查看是否 更改排行榜
	save_worship_to_db(sender_guid, target_data, GAMECONFIG->WorshipAddValueArray[worship_type], worship_type);
}

void ranking_mgr_ws::get_player_rank_level(int32 ranking_index, const guid_64& player_guid, int32& rank_level, int64& rank_value)
{
	if (ranking_index < 0 || ranking_index >= e_RankingIndex_max)
	{
		rank_level = -1;
		rank_value = 0;
		return;
	}
	int32 Index = 1;
	ranking_list* ranking_ptr = get_ranking_list_by_type((e_RankingIndex)ranking_index);
	if (nullptr == ranking_ptr)
	{
		rank_level = -1;
		rank_value = 0;
		return;
	}
	ranking_list_ite ite = ranking_ptr->begin();
	for (; ite != ranking_ptr->end(); ++ite, ++Index)
	{
		if (player_guid == (*ite).role_guid)
		{
			rank_level = Index;
			rank_value = (*ite).ranking_value;
			break;
		}
	}
}

guid_64 ranking_mgr_ws::get_ranking_player_guid(e_RankingIndex ranking_index, int32 ranking_pos)
{

	ranking_list* rangking_ptr = get_ranking_list_by_type(ranking_index);
	if (rangking_ptr == nullptr)
	{
		return guid_64();
	}
	if (ranking_index >= e_RankingIndex_max
		|| ranking_pos < 0 || ranking_pos >= rangking_ptr->size())
	{
		return guid_64();
	}
	ranking_list_ite it = rangking_ptr->begin();
	for (int32 i = 0; i < ranking_pos; ++i)
	{
		it++;
	}
	if (it == rangking_ptr->end())
	{
		return guid_64();
	}
	return it->role_guid;
}

int32 ranking_mgr_ws::get_ranking_title_type(int32  ranking_type)
{
	if (ranking_type < 0 || ranking_type >= e_RankingIndex_max)
	{
		return -1;
	}
	switch (ranking_type)
	{
	case e_RankingIndex_gs:
		return hld::e_title_type_gs_rank_first;
		break;
	case e_RankingIndex_box_map_level:
		return hld::e_title_type_money_rank_first;
		break;
	case e_RankingIndex_worship:
		return hld::e_title_type_worship_rank_first;
		break;
	case e_RankingIndex_arena:
		return hld::e_title_type_arena_rank_first;
		break;
	case e_RankingIndex_gs_barserker:
		return hld::e_title_type_gs_ranking_barserker;
		break;
	case e_RankingIndex_gs_wizard:
		return hld::e_title_type_gs_ranking_wizard;
		break;
	case e_RankingIndex_gs_guardian:
		return hld::e_title_type_gs_ranking_guardian;
		break;
	case e_RankingIndex_gs_assassinator:
		return hld::e_title_type_gs_ranking_assassinator;
		break;
	default:
		return -1;
		break;
	}
	return -1;
}

void ranking_mgr_ws::first_ranking_player_online_func( const guid_64& role_guid, bool(&loading_title_flag)[hld::e_title_type_max])
{
	if (globle_data::get_instance().get_server_on_days() > 1)
	{ 
		for (int32 index = 0; index < e_RankingIndex_max; index++)
		{
			if (get_ranking_player_guid((e_RankingIndex)index, 0) == role_guid)
			{
				int32 flag_pos = get_ranking_title_type(index);
				if (flag_pos >= 0)
				{
					loading_title_flag[flag_pos] = true;
				}
			}
		}
	} 
}

void ranking_mgr_ws::sync_first_rank_player_info(const s_ranking_player_info& old_first_player_info, const s_ranking_player_info& new_first_player_info , bool is_notice)
{
	if (old_first_player_info.ranking_type != new_first_player_info.ranking_type)
	{
		return;
	}
	int32 ranking_type = new_first_player_info.ranking_type;


	int32 big_player_type = big_player_ws_mgr::get_instance().get_big_player_type_with_ranking_type(ranking_type);
	if (big_player_type > 0 && globle_data::get_instance().get_server_on_days() > 1)
	{
		big_player_ws_mgr::get_instance().set_big_player_guid((e_big_player_type)big_player_type, new_first_player_info.role_guid, 11);
	}

	if (old_first_player_info.role_guid == new_first_player_info.role_guid)
	{
		return;
	}

	//client_session* session = nullptr;

	ws2cs_sync_first_rank_end resp;
	resp.is_notice = is_notice;
	client_session* old_session = client_session_mgr::getInstance().get_session(old_first_player_info.role_guid);
	if (nullptr != old_session)
	{
		resp.role_guid = old_first_player_info.role_guid;
		resp.is_first = false;
		resp.ranking_type = old_first_player_info.ranking_type;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_sync_first_rank_end);
		if (is_use == false)
		{
			old_session->send_to_cs(&resp, sizeof(ws2cs_sync_first_rank_end));
		}
		else
		{
			hld::ws2cs_proto::sync_first_rank_end pro_msg;
			resp.to_proto(pro_msg);
			old_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_sync_first_rank_end);
		}
		//old_session->send_to_cs(&resp, sizeof(ws2cs_sync_first_rank_end));
	}
	//session = nullptr;
	client_session* new_session = client_session_mgr::getInstance().get_session(new_first_player_info.role_guid);
	if (nullptr != new_session)
	{
		resp.role_guid = new_first_player_info.role_guid;
		resp.is_first = true;
		resp.ranking_type = new_first_player_info.ranking_type;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_sync_first_rank_end);
		if (is_use == false)
		{
			new_session->send_to_cs(&resp, sizeof(ws2cs_sync_first_rank_end));
		}
		else
		{
			hld::ws2cs_proto::sync_first_rank_end pro_msg;
			resp.to_proto(pro_msg);
			new_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_sync_first_rank_end);
		}
		//new_session->send_to_cs(&resp, sizeof(ws2cs_sync_first_rank_end));
	}
}

int32 ranking_mgr_ws::calcu_srv_avg_level(int32 rank_min_index, int32 rank_max_index)
{
	int32 avg_level_value = 0;
	if (rank_max_index < rank_min_index)
	{
		return avg_level_value;
	}

	ranking_list* level_ranking_list= ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_level);
	if (nullptr == level_ranking_list)
	{
		return avg_level_value;
	}
		
	int32 lv_player_num = 0;
	int32 rank_idx = 1;
	ranking_list_ite ite = level_ranking_list->begin();
	for (; ite != level_ranking_list->end(); ++ite, ++rank_idx)
	{
		if (rank_idx < rank_min_index)
		{
			continue;
		}
		if (rank_idx > rank_max_index)
		{
			break;
		}
		avg_level_value += ite->ranking_value;
		lv_player_num += 1;
	}
	if (lv_player_num == 0)
	{
		ite = level_ranking_list->begin();
		for (; ite != level_ranking_list->end(); ++ite, ++rank_idx)
		{
			avg_level_value += ite->ranking_value;
			lv_player_num += 1;
		}
	}
	if (lv_player_num != 0)
	{
		avg_level_value = avg_level_value / lv_player_num;
	}
	return avg_level_value;
}

int32 ranking_mgr_ws::calcu_all_server_avg_level()
{
	int32 avg_level_value = 0;
	ActivityCommonConfigTemplate* bro_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_broken_sky);
	if (nullptr == bro_cfg_ptr || bro_cfg_ptr->ParamIntArr1.size() < 2)
	{
		return avg_level_value;
	}

	ranking_list* level_ranking_list = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_level_all);
	if (nullptr == level_ranking_list)
	{
		return avg_level_value;
	}

	int32 lv_player_num = 0;
	int32 rank_idx = 1;
	ranking_list_ite ite = level_ranking_list->begin();
	for (; ite != level_ranking_list->end(); ++ite, ++rank_idx)
	{
		if (rank_idx < bro_cfg_ptr->ParamIntArr1[0])
		{
			continue;
		}
		if (rank_idx > bro_cfg_ptr->ParamIntArr1[1])
		{
			break;
		}
		avg_level_value += ite->ranking_value;
		lv_player_num += 1;
	}
	if (lv_player_num == 0)
	{
		ite = level_ranking_list->begin();
		for (; ite != level_ranking_list->end(); ++ite, ++rank_idx)
		{
			avg_level_value += ite->ranking_value;
			lv_player_num += 1;
		}
	}
	if (lv_player_num != 0)
	{
		avg_level_value = avg_level_value / lv_player_num;
	}
	return avg_level_value;
}

void ranking_mgr_ws::sync_srv_avg_level_to_cs()
{
	//int32 cur_server_exp_level = 0;
	//ActivityCommonConfigTemplate* bro_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_broken_sky);
	//if (nullptr != bro_cfg_ptr && bro_cfg_ptr->ParamIntArr1.size() >= 2)
	//{
	//	cur_server_exp_level = calcu_srv_avg_level(bro_cfg_ptr->ParamIntArr1[0], bro_cfg_ptr->ParamIntArr1[1]);
	//}
	int32 start = 0, end = 0;
	get_world_level_param(start, end);
	int32 cur_server_exp_level = calcu_srv_avg_level(start, end);
		
	if (cur_server_exp_level == m_cur_srv_avg_level)
	{
		return;
	}
	m_cur_srv_avg_level = cur_server_exp_level;
		
	send_world_srv_lv_info_all();
}

int32 ranking_mgr_ws::get_srv_avg_level()
{
	int32 cur_level = m_cur_srv_avg_level;
	int32 world_level = m_world_avg_level_last;
	if (cur_level < world_level)
	{
		return world_level;
	}
	return cur_level;
}
int32 ranking_mgr_ws::get_world_level_last()
{ 
	return m_world_avg_level_last;
}
int32 ranking_mgr_ws::get_world_level_cur()
{ 
	return m_world_avg_level_cur; 
}

int32 ranking_mgr_ws::get_rank_level_with_num(int32 num)
{
	int32 avg_level_value = 0;
	if (num <= 0)
	{
		return avg_level_value;
	}

	ranking_list* level_ranking_list = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_level);
	if (nullptr == level_ranking_list)
	{
		return avg_level_value;
	}

	int32 lv_player_num = 0;
	int32 rank_idx = 1;
	ranking_list_ite ite = level_ranking_list->begin();
	for (; ite != level_ranking_list->end(); ++ite, ++rank_idx)
	{
		if (rank_idx > num)
		{
			break;
		}
		avg_level_value += ite->ranking_value;
		lv_player_num += 1;
	}
	if (lv_player_num != 0)
	{
		avg_level_value = avg_level_value / lv_player_num;
	}
	return avg_level_value;
		
}

void ranking_mgr_ws::send_world_srv_lv_info_all(int32 conn_index)
{
	//ws2cs_sync_server_avg_lv sync_msg;
	//sync_msg.srv_avg_lv = m_cur_srv_avg_level;
	//sync_msg.world_lv_cur = m_world_avg_level_cur;
	//sync_msg.world_lv_last = m_world_avg_level_last;
	//faith_server_mgr::getInstance().send_message(&sync_msg, sizeof(sync_msg), conn_index, e_server_type_cs);
	ws2cs_sync_server_avg_lv sync_msg;
	sync_msg.srv_avg_lv = m_cur_srv_avg_level;
	sync_msg.world_lv_cur = m_world_avg_level_cur;
	sync_msg.world_lv_last = m_world_avg_level_last;
	bool is_use = proto_by_lua(e_msgindex_ws2cs_sync_server_avg_lv);
	if (is_use == false)
	{
		world_server::getInstance().broadcast(&sync_msg, sizeof(sync_msg), e_server_type_cs);
	}
	else
	{
		hld::ws2cs_proto::sync_server_avg_lv pro_msg;
		sync_msg.to_proto(pro_msg);
		world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2cs_sync_server_avg_lv, e_server_type_cs);
	}
}

void ranking_mgr_ws::get_world_level_param(int32 &start, int32& end)
{
	start = 1;
	end = 20;
	int32 server_open_days = globle_data::get_instance().get_server_on_days();
	int32 num = GAMECONFIG->WorldLevelArray.size();
	if (num == 0 || num % 4 != 0)
	{
		return;
	}
	for (int32 i = 0; i < num; i++)
	{
		if (GAMECONFIG->WorldLevelArray[i * 4 + 0] <= server_open_days &&
			server_open_days <= GAMECONFIG->WorldLevelArray[i * 4 + 1])
		{
			start = GAMECONFIG->WorldLevelArray[i * 4 + 2];
			end = GAMECONFIG->WorldLevelArray[i * 4 + 3];
			break;
		}
	}
	if (start > end)
	{
		start = 1;
		end = 20;
	}
}

int32 ranking_mgr_ws::get_world_level_for_old()
{
	int32 ret_level = 1;

	int32 server_open_days = globle_data::get_instance().get_server_on_days();
	int32 len = get_exp_server_begin_level_length_by_region();//GAMECONFIG->ExpServerBeginLevel.size();
	if (server_open_days <= 1)
	{
		return ret_level;
	}
	else if (server_open_days <= len + 1)
	{
		ret_level = get_get_exp_server_begin_level(server_open_days - 2);//GAMECONFIG->ExpServerBeginLevel[server_open_days - 2];
	}
	else
	{
		//ActivityCommonConfigTemplate* bro_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_broken_sky);
		//if (nullptr != bro_cfg_ptr && bro_cfg_ptr->ParamIntArr1.size() >= 2)
		//{
		//	ret_level = calcu_srv_avg_level(bro_cfg_ptr->ParamIntArr1[0], bro_cfg_ptr->ParamIntArr1[1]);
		//}
		int32 start = 0, end = 0;
		get_world_level_param(start, end);
		ret_level = calcu_srv_avg_level(start, end);
		/*	if (ret_level <= GAMECONFIG->ExpServerBeginLevel[len - 1])
			{
				ret_level = GAMECONFIG->ExpServerBeginLevel[len - 1];
			}*/
		int32 config_max = get_get_exp_server_begin_level(len - 1);
		if (ret_level <= config_max)
		{
			ret_level = config_max;
		}
	}
	return ret_level;
}

int32 ranking_mgr_ws::get_world_level_for_now()
{
	int32 ret_level = 0;

	int32 server_open_days = globle_data::get_instance().get_server_on_days();
	int32 len = get_exp_server_begin_level_length_by_region();//GAMECONFIG->ExpServerBeginLevel.size();
	if (server_open_days <= 0)
	{
		ret_level = get_get_exp_server_begin_level(0);//GAMECONFIG->ExpServerBeginLevel[0];
	}
	else if (server_open_days <= len)
	{
		ret_level = get_get_exp_server_begin_level(server_open_days - 1);//GAMECONFIG->ExpServerBeginLevel[server_open_days - 1];
	}
	else
	{
		//ActivityCommonConfigTemplate* bro_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_broken_sky);
		//if (nullptr != bro_cfg_ptr && bro_cfg_ptr->ParamIntArr1.size() >= 2)
		//{
		//	ret_level = calcu_srv_avg_level(bro_cfg_ptr->ParamIntArr1[0], bro_cfg_ptr->ParamIntArr1[1]);
		//}
		int32 start = 0, end = 0;
		get_world_level_param(start, end);
		ret_level = calcu_srv_avg_level(start, end);
		/*	if (ret_level <= GAMECONFIG->ExpServerBeginLevel[len - 1])
			{
				ret_level = GAMECONFIG->ExpServerBeginLevel[len - 1];
			}*/
		int32 config_max = get_get_exp_server_begin_level(len - 1);
		if (ret_level <= config_max)
		{
			ret_level = config_max;
		}
	}

	return ret_level;
}

void ranking_mgr_ws::load_world_level_from_db(int32 lv_cur, int32 lv_last)
{
	if (lv_cur > 0)
	{
		m_world_avg_level_cur = lv_cur;
		if (lv_last <= 0)
		{
			m_world_avg_level_last = 1;
		}
		else
		{
			m_world_avg_level_last = lv_last;
		}
	}
	else
	{
		m_is_need_save_db_level = true;
		//world_server::getInstance().save_server_attr_val_db(e_srv_attr_val_world_lv_cur, m_world_avg_level_cur);
		//world_server::getInstance().save_server_attr_val_db(e_srv_attr_val_world_lv_last, m_world_avg_level_last);
	}
}

void ranking_mgr_ws::sync_calcu_world_level_tick_day()
{
	if (true == m_is_need_save_db_level)
	{
		m_world_avg_level_cur = get_world_level_for_now();
		m_world_avg_level_last = get_world_level_for_old();
		world_server::getInstance().save_server_attr_val_db(e_srv_attr_val_world_lv_cur, m_world_avg_level_cur);
		world_server::getInstance().save_server_attr_val_db(e_srv_attr_val_world_lv_last, m_world_avg_level_last);
		send_world_srv_lv_info_all();
		m_is_need_save_db_level = false;
		game_proto_server_exp_level msg;
		msg.set_exp_level(m_world_avg_level_cur);
		msg.set_exp_percent(-1);
		msg.set_exp_last_level(m_world_avg_level_last);
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_server_exp_level);
		return;
	}

	int32 new_level = get_world_level_for_now();
	if (m_world_avg_level_cur > new_level)
	{
		new_level = m_world_avg_level_cur;
	}
	if (globle_data::get_instance().get_server_on_days() > 1)
	{
		m_world_avg_level_last = m_world_avg_level_cur;
	}
	m_world_avg_level_cur = new_level;

		
	world_server::getInstance().save_server_attr_val_db(e_srv_attr_val_world_lv_cur, m_world_avg_level_cur);
	world_server::getInstance().save_server_attr_val_db(e_srv_attr_val_world_lv_last, m_world_avg_level_last);
		
	game_proto_server_exp_level msg;
	msg.set_exp_level(m_world_avg_level_cur);
	msg.set_exp_percent(-1);
	msg.set_exp_last_level(m_world_avg_level_last);
	client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_server_exp_level);
		
	send_world_srv_lv_info_all();
}

void ranking_mgr_ws::send_avg_level_to_client(client_session* session)
{
	if (nullptr == session)
	{
		return;
	}
	game_proto_send_world_level msg;
	int32 WorldLevel = 0;
	WorldLevel = calcu_all_server_avg_level();
	msg.set_server_level(WorldLevel);

	session->send_to_client(&msg, e_msgindex_s2c_send_world_level);
}

void ranking_mgr_ws::send_service_rank_info_all_to_client(client_session* session)
{
	if (nullptr == session)
	{
		return;
	}

	ranking_proto_service_rank_info_all msg;
	for (int32 i = 0; i < e_service_rank_type_max; i++)
	{
		ranking_proto_service_rank_info_one* info_ptr = msg.add_service_rank_info();
		info_ptr->set_service_rank_type(i);
		info_ptr->set_rank_index(get_player_service_rank_index(session->get_role_guid(), i));
	}
	session->send_to_client(&msg, e_msgindex_s2c_service_rank_info_all);
}

void ranking_mgr_ws::send_service_rank_info_one_to_client(client_session* session, int32 rank_num, int32 send_type)
{
	if (send_type < e_service_rank_type_level || send_type >= e_service_rank_type_max || nullptr == session)
	{
		return;
	}
	ranking_proto_service_rank_info_one msg;
	msg.set_service_rank_type(send_type);
	msg.set_rank_index(rank_num);
	session->send_to_client(&msg, e_msgindex_s2c_service_rank_info_one);
}

void ranking_mgr_ws::send_service_rank_first_player_to_client_by_type(client_session* session, int32 send_type)
{
	if (send_type < e_service_rank_type_level || send_type >= e_service_rank_type_max || nullptr == session)
	{
		return;
	}

	ranking_proto_service_rank_get_first_player_end msg;
	msg.set_service_rank_type(send_type);
	msg.add_first_player_guid(m_service_rank_first_player[send_type].A);
	msg.add_first_player_guid(m_service_rank_first_player[send_type].B);
	session->send_to_client(&msg, e_msgindex_s2c_service_rank_get_first_player_end);
}

int32 ranking_mgr_ws::get_player_service_rank_index(const guid_64& role_guid, const int32& rank_type)
{
	if (rank_type < e_service_rank_type_level || rank_type >= e_service_rank_type_max)
	{
		return 0;
	}

	auto result = m_service_rank_all[rank_type].find(role_guid.server_64);
	if (result != m_service_rank_all[rank_type].end())
	{
		return result->second.role_num;
	}
	else
	{
		return 0;
	}
}

void ranking_mgr_ws::sync_oracle_trial_play_info(guid_64 play_guid, int32 rank_value)
{
	client_session* session_ptr = client_session_mgr::getInstance().get_session(play_guid);
	if (session_ptr == nullptr || play_guid.is_valid() == false)
	{
		return;
	}
	ws2ws_sync_oracle_trial_rank msg;
	s_ranking_player_info rank_info;
	rank_info.role_guid = play_guid;
	rank_info.set_role_name(session_ptr->get_role_name());
	rank_info.ranking_type = e_RankingIndex_Oracle_Trial;
	rank_info.ranking_value = rank_value;
	rank_info.save_time = time_helper::get_time();
	if (session_ptr->get_legion_guid().is_valid())
	{
		rank_info.supporting_guid = session_ptr->get_legion_guid();
		rank_info.set_supporting_name(session_ptr->get_legion_info().legion_name);
	}
	rank_info.vip_level = session_ptr->get_role_info_data(e_role_info_vip_level);
	rank_info.server_id = session_ptr->get_role_info_data(e_role_info_server_id);
	rank_info.set_game_channel(session_ptr->m_login_third_data.param14);
	sync_player_info(rank_info);
	msg.play_info = rank_info;

	int32 server_id = world_server::getInstance().get_cross_id();
	cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_sync_oracle_trial_rank, &msg, sizeof(msg));
}

void ranking_mgr_ws::get_oracle_trial_rank_info(guid_64 play_guid)
{
	client_session* session_ptr = client_session_mgr::getInstance().get_session(play_guid);
	if (session_ptr == nullptr || play_guid.is_valid() == false)
	{
		return;
	}
	ws2ws_get_oracle_trial_rank msg;
	msg.play_guid = play_guid;
	msg.server_id = session_ptr->get_role_info_data(e_role_info_server_id);
	int32 server_id = world_server::getInstance().get_cross_id();
	cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_get_oracle_trial_rank, &msg, sizeof(msg));
}

void ranking_mgr_ws::send_oracle_trial_rank_info(guid_64 play_guid, int32 server_id)
{
	if (!play_guid.is_valid() || server_id <= 0)
	{
		return;
	}
	ws2ws_get_oracle_trial_rank_end msg;
	ranking_list* rank_list = get_ranking_list_by_type(e_RankingIndex_Oracle_Trial);
	if (rank_list == nullptr)
	{
		return ;
	}
	msg.play_guid = play_guid;
	ranking_list_ite it = rank_list->begin();
	msg.data_num = rank_list->size() > oracle_trial_rank_info ? oracle_trial_rank_info : rank_list->size();
	for (int32 i = 0; i < msg.data_num; ++i, it++)
	{
		msg.play_info[i] = *it;
	}
	cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_get_oracle_trial_rank_end, &msg, sizeof(msg));
}

void ranking_mgr_ws::send_get_oracle_trial_info()
{
	ws2ws_get_oracle_ranking_info msg;
	std::set<int32> server_id_arr;
	world_server::getInstance().get_server_id_arr(server_id_arr);
	for (std::set<int32>::iterator it = server_id_arr.begin(); it != server_id_arr.end(); ++it)
	{
		msg.server_id = *it;
		if (msg.server_id == world_server::getInstance().get_server_id())
		{
			continue;
		}
		cross::send_msg_to_ws(guid_64(), msg.server_id, e_msgindex_ws2ws_get_oracle_trial_info, &msg, sizeof(msg));
	}
}

void ranking_mgr_ws::can_send_oracle_trial_notice(const guid_64 & old_guid, const guid_64 & play_guid)
{
	if (false == play_guid.is_valid())
	{
		return;
	}
	ranking_list* rank_list = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_Oracle_Trial);
	if (nullptr == rank_list || rank_list->empty())
	{
		return;
	}
	ranking_list_ite it = rank_list->begin();
	s_ranking_player_info play_rank_info = *it;
	guid_64 new_fist_play = play_rank_info.role_guid;

	if (old_guid != play_guid && new_fist_play == play_guid)
	{
		s_ranking_player_info* play_info = get_ranking_player_info_by_type(e_RankingIndex_Oracle_Trial, play_guid);
		if (nullptr ==  play_info)
		{
			return;
		}
		ws2ws_send_oracle_ranking_notice msg;
		msg.player_guid = play_info->role_guid;
		msg.legion_guid = play_info->supporting_guid;
		msg.set_play_name(play_info->role_name);
		msg.set_legion_name(play_info->supporting_name);
		msg.server_id = play_info->server_id;		
		cross::send_msg_to_ws(guid_64(), 0, e_msgindex_ws2ws_send_oracle_ranking_notice, &msg, sizeof(msg));
	}

}

void ranking_mgr_ws::send_oracle_trial_notice(const xstring player_name, int32 server_id)
{
	s_game_info* server_config = world_server::getInstance().get_server_config_by_server_id(server_id);
	if (nullptr == server_config)
	{
		return;
	}
	xstring server_name = server_config->server_name;
	int32 notice_str_id = oracle_trial_first_play_notice_id;
	std::vector<std::string> notice_str_params;
	notice_str_params.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_str_id));
	notice_str_params.push_back(server_name);
	notice_str_params.push_back(player_name);
	std::string notice_str = init_unit::implode(notice_str_params);
	int32 create_time = time_helper::get_cur_time_new().second;
	event_ws_mgr::get_instance().send_notice_to_all(notice_str_id, create_time, guid_64(), notice_str);

}

void ranking_mgr_ws::send_oracle_trial_notice(const xstring player_name, const xstring legion_name, int32 server_id)
{
	s_game_info* server_config = world_server::getInstance().get_server_config_by_server_id(server_id);
	if (nullptr == server_config)
	{
		return;
	}
	xstring server_name = server_config->server_name;
	int32 notice_str_id = oracle_trial_first_play_and_legion_notice_id;
	std::vector<std::string> notice_str_params;
	notice_str_params.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_str_id));
	notice_str_params.push_back(server_name);
	notice_str_params.push_back(legion_name);
	notice_str_params.push_back(player_name);
	std::string notice_str = init_unit::implode(notice_str_params);
	int32 create_time = time_helper::get_cur_time_new().second;
	event_ws_mgr::get_instance().send_notice_to_all(notice_str_id, create_time, guid_64(), notice_str);
}

void ranking_mgr_ws::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
{
	for ( int32 i = e_RankingIndex_gs; i < e_RankingIndex_max; i++)
	{
		ranking_list& rank_list = m_ranking_all[i];
		auto iter = rank_list.begin();
		for (; iter != rank_list.end(); iter++ )
		{
			if ( iter->role_guid == role_guid)
			{
				iter->set_role_name(role_name);
			}
			if (iter->supporting_guid == role_guid)
			{
				iter->set_supporting_name(role_name);
			}
		}
	}
}

void ranking_mgr_ws::change_ranking_legion_name_info(const guid_64& ranking_guid, const guid_64& legion_guid, const xstring& legion_name)
{
	if (ranking_guid.is_valid() == false)
	{
		return;
	}
	for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max; ranking_type++)
	{
			s_ranking_player_info* legion_ranking = get_ranking_player_info_by_type((e_RankingIndex)ranking_type, legion_guid);
			if (nullptr == legion_ranking)
			{
				continue;
			}
			if (fixed_ranking_legion_name_data(legion_ranking, legion_guid, legion_name))
			{
				set_ranking_save_flag(ranking_type, true);
			}
	}
}

void ranking_mgr_ws::delete_ranking_by_role_guid(e_RankingIndex ranking_index, const guid_64& target_guid)
{
	if (target_guid.is_valid() == false)
	{
		return;
	}
	ranking_list* list_ptr = get_ranking_list_by_type(ranking_index);
	if (nullptr == list_ptr)
	{
		return;
	}
	ranking_list_ite ite = list_ptr->begin();
	for (; ite != list_ptr->end(); ++ite)
	{
		if (ite->role_guid == target_guid)
		{
			list_ptr->erase(ite);
			return;
		}
	}
	return;
}

void ranking_mgr_ws::replace_cross_ladder_ranking()
{
	m_ranking_all[e_RankingIndex_cross_ladder_last_seaon].clear();
	m_ranking_all[e_RankingIndex_cross_ladder_last_seaon] = m_ranking_all[e_RankingIndex_cross_ladder_cur_season];
	m_ranking_all[e_RankingIndex_cross_ladder_cur_season].clear();

	ranking_list* _ranking_list = &m_ranking_all[e_RankingIndex_cross_ladder_last_seaon];
	for (auto iter = _ranking_list->begin(); iter != _ranking_list->end(); ++iter)
	{
		iter->ranking_type = e_RankingIndex_cross_ladder_last_seaon;
	}
	m_flag_save_ranking[e_RankingIndex_cross_ladder_last_seaon] = true;
	m_flag_save_ranking[e_RankingIndex_cross_ladder_cur_season] = true;
}
void ranking_mgr_ws::replace_element_war_ranking()
{
	m_ranking_all[e_RankingIndex_element_war_last_season].clear();
	m_ranking_all[e_RankingIndex_element_war_last_season] = m_ranking_all[e_RankingIndex_element_war_cur_season];
	m_ranking_all[e_RankingIndex_element_war_cur_season].clear();

	ranking_list* _ranking_list = &m_ranking_all[e_RankingIndex_element_war_last_season];
	for (auto iter = _ranking_list->begin(); iter != _ranking_list->end(); ++iter)
	{
		iter->ranking_type = e_RankingIndex_element_war_last_season;
	}
	m_flag_save_ranking[e_RankingIndex_element_war_last_season] = true;
	m_flag_save_ranking[e_RankingIndex_element_war_cur_season] = true;
}

int32 ranking_mgr_ws::get_exp_server_begin_level_length_by_region()
{
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return 0;
	}
	int32 region = region_template_ptr->RegionCode;
	int32 size = GAMECONFIG->ExpServerBeginLevel.size();

	int32 region_max = (int32)e_version_region_type_max;

	if (size == 0 || size % region_max != 0)
	{
		return 0;
	}
	int32 per_max = size / region_max;
	int32 length = 0;

	for (int32 i = 0; i < per_max; i++)
	{
		if (GAMECONFIG->ExpServerBeginLevel[i * region_max + region] == 0)
		{
			break;
		}
		length++;
	}
	return length;
}
int32 ranking_mgr_ws::get_get_exp_server_begin_level(int32 offset)
{
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return 0;
	}
	int32 region = region_template_ptr->RegionCode;
	int32 size = GAMECONFIG->ExpServerBeginLevel.size();

	int32 region_max = (int32)e_version_region_type_max;
	if (size == 0 || size % region_max != 0)
	{
		return 0;
	}
	int32 per_max = size / region_max;

	if (offset < 0 || offset >= per_max)
	{
		return 0;
	}
	return GAMECONFIG->ExpServerBeginLevel[offset * region_max + region];

}
