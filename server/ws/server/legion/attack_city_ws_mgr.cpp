/********************************************************************
created: 2021年9月23日
file base: attack_city_ws_mgr
file ext: cpp
author: zhaoyuming
purpose:
*********************************************************************/
#include "attack_city_ws_mgr.h"
#include "template/template_manager.h"
#include <legion_msg.hpp>
#include "ws_client.hpp"
#include "legion_ws_mgr.h"
#include "Logic/activity_def.hpp"
#include "utility/cs_date.hpp"
#include "../client_session_mgr.hpp"
#include "cross_group/gate_proxy.hpp"
#include "world_server.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "../../server/mail/event_ws_mgr.h"
#include "../ws/server/ranking/ranking_mgr_ws.h"
#include "server_log.hpp"
#include "Logic/attack_city_def.hpp"
#include "system/scene/cs_map_mgr_system.h"
#include "components/scene/cs_map_component.h"
#include "system/scene/cs_map_system.h"
#include "game.pb.h"
#include "net.pb.h"
namespace faith
{
	attack_city_ws_mgr::attack_city_ws_mgr()
	{
		clear_data();
	}
	void attack_city_ws_mgr::tick(int64 time_new)
	{
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_attack_city) && world_server::getInstance().is_sky_island_server())
		{

			if (is_in_check_time() && false == m_is_check_time)
			{
				// 军团检查阶段 对所有已经分组的军团进行检查
				m_is_check_time = true;
				game_check();
			}

			if (is_in_group_time() && false == m_is_group_time)
			{
				// 军团分组阶段 对军团进行分组
				m_is_group_time = true;
				game_group();
			}

			if (is_in_game_time() && false == m_is_begin_time)
			{
				// 活动正式开始 初始化活动地图
				m_is_begin_time = true;
				game_init();
			}

			if (false == is_in_game_time() && m_is_begin_time)
			{
				// 不在活动时间就是活动结束了
				game_end();
			}

			if (false == m_is_group_time && m_tem_game_group_time < time_new)
			{
				// 一小时刷新一次 刷新军团临时分组信息
				tem_game_group();
				if (m_is_legion_data)
				{
					// 保存军团数据
					save_group_data_all();
					m_is_legion_data = false;
				}
				else
				{
					m_is_legion_data = true;
					// 并且保存一次数据
					save_rank_data_all();
				}
			
	
			}
		}
	}
	void attack_city_ws_mgr::clear_data()
	{
		m_is_begin_time = false;
		m_is_check_time = false;
		m_is_group_time = false;
		m_empty_legion_info.clear_data();
		for (int32 i = 0; i < e_attack_city_group_level_max; ++i)
		{
			m_map_guid_list[i] = 0;
		}
		for (int32 i = 0; i < e_attack_city_data_max; ++i)
		{
			m_config_data[i] = 0;
		}
		m_legion_info_list.clear();
		m_tem_legion_info_list.clear();
		m_rank_list.clear();
		m_tem_game_group_time = 0;
	}
	void attack_city_ws_mgr::init_manager(bool need_load_dp)
	{
		// 是否loadDP数据
		if (need_load_dp)
		{
			load_group_data();
			load_rank_data();
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_attack_city);
		if (nullptr == config_ptr || config_ptr->ParamIntArr1.size() < e_attack_city_data_max)
		{
			return;
		}
		// 初始化活动表数据
		for (int32 i = 0; i < e_attack_city_data_max; ++i)
		{
			m_config_data[i] = config_ptr->ParamIntArr1[i];
		}
	}
	void attack_city_ws_mgr::load_rank_data()
	{
		// 发送Load排名信息
		ws2dp_attack_city_load_rank_info msg;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}
	void attack_city_ws_mgr::clear_rank_data(bool is_clear_dp)
	{
		// 清理排名列表
		m_rank_list.clear();
		if (is_clear_dp)
		{
			// 清理数据库中的排名新
			clear_rank_dp_data();
		}
	}
	void attack_city_ws_mgr::add_rank_data(const s_attack_city_rank_info & rank_info, bool is_save)
	{
		// 增加排名信息
		m_rank_list.push_back(rank_info);
		if (is_save)
		{
			save_rank_data_one(rank_info);
		}
		CONSOLE_INFO("add_rank_data legion_guid:{} role_guid:{} group_level:{} group_rank:{}", rank_info.legion_guid.server_64, rank_info.role_guid.server_64, rank_info.group_level, rank_info.group_rank);
	}

	void attack_city_ws_mgr::save_rank_data_all(bool is_clear_dp)
	{
		CONSOLE_INFO("attack_city_ws save_rank_data_all");
		if (is_clear_dp)
		{
			// 是否需要先清理数据库
			clear_rank_dp_data();
		}

		// 保存所有排名信息
		for (int32 i = 0; i < m_rank_list.size(); ++i)
		{
			save_rank_data_one(m_rank_list[i]);
		}
	}
	void attack_city_ws_mgr::save_rank_data_one(const s_attack_city_rank_info& rank_info)
	{
		// 保存排名信息到数据库
		ws2dp_attack_city_save_rank_info msg;
		msg.rank_info = rank_info;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}
	void attack_city_ws_mgr::load_group_data()
	{
		// 加载军团信息
		ws2dp_attack_city_load_legion_info msg;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}
	void attack_city_ws_mgr::clear_group_dp_data()
	{
		// 清理军团信息
		ws2dp_attack_city_clear_legion_info msg;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}
	void attack_city_ws_mgr::clear_rank_dp_data()
	{ 
		// 清理军团信息
		ws2dp_attack_city_clear_rank_info msg;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}
	void attack_city_ws_mgr::load_group_data_end(const s_attack_city_legion_info * dp_info, int32 data_num)
	{
		if (nullptr == dp_info)
		{
			return;
		}
		// 缓存军团信息
		m_legion_info_list.clear();
		for (int32 i = 0; i < data_num; ++i)
		{
			m_legion_info_list.push_back(dp_info[i]);
		}
	}
	void attack_city_ws_mgr::save_group_data(const s_attack_city_legion_info& legion_info)
	{
		// 保存军团信息
		ws2dp_attack_city_save_legion_info msg;
		msg.legion_info = legion_info;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}
	void attack_city_ws_mgr::save_group_data_all(bool is_clear_dp)
	{
		CONSOLE_INFO("save_group_data_all");
		if (is_clear_dp)
		{
			// 是否需要先清理数据库
			clear_group_dp_data();
		}

		// 将所有记录信息保存
		for (int32 i = 0; i < m_legion_info_list.size(); ++i)
		{
			save_group_data(m_legion_info_list[i]);
		}
	}
	void attack_city_ws_mgr::game_init()
	{
		ws2cs_attack_city_init_data msg;
		for (int32 i = 0; i < e_attack_city_group_level_max; ++i)
		{
			msg.clear_data();
			msg.group_level = i;
			for (int32 j = 0; j < attack_city_group_max_num; ++j)
			{
				if (m_legion_info_list.size() <= i * attack_city_group_max_num + j)
				{
					break;
				}
				msg.legion_list[j] = m_legion_info_list[i * attack_city_group_max_num + j];
			}
			
			// 判断是否有可参加军团
			if (false ==  msg.legion_list[0].is_valid())
			{
				CONSOLE_ERROR("attack_city_ws game_init init_end level = {}", i);
				break;
			}

			// 创建军团
			auto map_ent = cs_map_mgr_system::init_map_by_battle(get_config_data(e_attack_city_data_map_id));
			if (map_ent == nullptr)
			{
				CONSOLE_ERROR("attack_city_ws game_init create_error_code level = {}", i);
				continue;
			}
			CONSOLE_INFO("attack_city_ws game_init create_map_succeed map_guid = {}", map_ent->getEntityId());
			m_map_guid_list[i] = map_ent->getEntityId();
			msg.map_guid = m_map_guid_list[i];
			world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_cs);
		}
		// 创建地图后再删除排名
		// 清除排名记录
		clear_rank_data(true);
	}
	void attack_city_ws_mgr::game_check()
	{
		for (int32 i = 0; i < m_legion_info_list.size(); ++i)
		{
			if (m_legion_info_list[i].is_valid())
			{
				// 设置为未检测状态 如过有返回值说明军团没有问题
				m_legion_info_list[i].is_check = (int32)e_attack_check_type_no_check;
				ws2ws_attack_city_legion_check msg;
				msg.legion_guid = m_legion_info_list[i].legion_guid;
				cross::send_msg_to_ws(guid_64(), m_legion_info_list[i].server_id, e_msgindex_ws2ws_attack_city_legion_check, &msg, sizeof(msg));
			}
			else
			{
				m_legion_info_list[i].clear_data();
			}
		}
	}
	void attack_city_ws_mgr::check_legion_is_valid(guid_64 legion_guid)
	{
		// 如果获取不到军团信息说明军团无效 不需要返回
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		ws2ws_attack_city_legion_check_end msg;
		msg.legion_guid = legion_guid;
		cross::send_msg_to_ws(guid_64(), world_server::getInstance().get_cross_id(), e_msgindex_ws2ws_attack_city_legion_check_end, &msg, sizeof(msg));
	}
	void attack_city_ws_mgr::set_legion_is_valid(guid_64 legion_guid)
	{
		s_attack_city_legion_info& legion_info = get_legion_info(legion_guid);
		if (false == legion_info.is_valid())
		{
			return;
		}
		// 如果有返回结果说明军团有效
		legion_info.is_check = (int32)e_attack_check_type_check;
	}
	void attack_city_ws_mgr::game_group()
	{
		// 获取跨服军团战力排名  
		ranking_list* ranking_list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_legion_gs_all);
		if (nullptr == ranking_list_ptr)
		{
			CONSOLE_INFO("attack_city_ws game_group ranking_list is_empty");
			return;
		}

		// 创建临时数组
		std::vector<s_attack_city_legion_info> _info_list;
		_info_list.clear();

		// 先检查军团是否有效
		for (int32 i = 0; i < m_legion_info_list.size(); ++i)
		{
			// 军团id有效, 只有C组以上的军团有效 检查没有问题 放入到临时数组中, 
			if (m_legion_info_list[i].is_valid() && m_legion_info_list[i].is_check > (int32)e_attack_check_type_no_check &&  m_legion_info_list[i].group_level > -1 && m_legion_info_list[i].group_level < (e_attack_city_group_level_c * attack_city_group_max_num))
			{
				_info_list.push_back(m_legion_info_list[i]);
			}
		}
		// 清空旧列表
		m_legion_info_list.clear();
		// 对列表进行排序
		std::sort(_info_list.begin(), _info_list.end());

		ranking_list_ite cur_list_ite = ranking_list_ptr->begin();
		for (int32 i = 0; i < (e_attack_city_group_level_max * attack_city_group_max_num); ++i, ++cur_list_ite)
		{
			if (cur_list_ite == ranking_list_ptr->end())
			{
				break;
			}

			// 判断军团是否在分组中
			bool is_have = false;
			for (int32 j = 0; j < _info_list.size(); ++j)
			{
				if (_info_list[j].legion_guid == cur_list_ite->role_guid)
				{
					is_have = true;
					break;
				}
			}
			// 如果军团不存在存入到临时数组中
			if (false == is_have)
			{
				s_attack_city_legion_info new_info;
				new_info.clear_data();
				new_info.legion_guid = cur_list_ite->role_guid;
				new_info.set_legion_name(cur_list_ite->role_name);
				new_info.server_id = cur_list_ite->server_id;
				_info_list.push_back(new_info);
			}
			// 如果分组已经找满结束遍历
			if (_info_list.size() >= (e_attack_city_group_level_max * attack_city_group_max_num))
			{
				break;
			}
		}
		ws2ws_send_attack_city_group_mail msg;
		for (int32 i = 0; i < _info_list.size() && i < (e_attack_city_group_level_max * attack_city_group_max_num); i++)
		{
			// 设置分组排名
			_info_list[i].group_level = i;
			msg.legion_guid[i] = _info_list[i].legion_guid;
			CONSOLE_INFO("attack_city_ws game_group level:{} legion_guid:{}", _info_list[i].group_level, _info_list[i].legion_guid.server_64);
		}
		m_legion_info_list = _info_list;
		m_tem_legion_info_list = _info_list;
		// 保存分组记录
		save_group_data_all(true);
		CONSOLE_INFO("attack_city_ws game_group succeed");
		cross::send_msg_to_ws(guid_64(), 0, e_msgindex_ws2ws_send_attack_city_group_mail, &msg, sizeof(msg));
	}
	void attack_city_ws_mgr::tem_game_group()
	{
		int64 time_now = utility::get_tick_count();
		m_tem_game_group_time = time_now + 3600;
		ranking_list* ranking_list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_legion_gs_all);
		if (nullptr == ranking_list_ptr)
		{
			return;
		}
		m_tem_legion_info_list.clear();
		for (int32 i = 0; i < m_legion_info_list.size(); ++i)
		{
			// 军团id有效, 检查没有问题, 只有C组以上的军团有效
			if (m_legion_info_list[i].is_valid() && m_legion_info_list[i].group_level > -1 && m_legion_info_list[i].group_level < (e_attack_city_group_level_c * attack_city_group_max_num))
			{
				// 将分组信息放入到临时数组
				m_tem_legion_info_list.push_back(m_legion_info_list[i]);
			}
		}
		// 对列表进行排序

		std::sort(m_tem_legion_info_list.begin(), m_tem_legion_info_list.end());
		ranking_list_ite cur_list_ite = ranking_list_ptr->begin();
		for (int32 i = 0; i < (e_attack_city_group_level_max * attack_city_group_max_num); i++, ++cur_list_ite)
		{
			if (cur_list_ite == ranking_list_ptr->end())
			{
				break;
			}
			bool is_have = false;
			for (int32 j = 0; j < m_tem_legion_info_list.size(); ++j)
			{
				if (m_tem_legion_info_list[j].legion_guid == cur_list_ite->role_guid)
				{
					is_have = true;
					break;
				}
			}
			if (false == is_have)
			{
				s_attack_city_legion_info new_info;
				new_info.clear_data();
				new_info.group_level = m_tem_legion_info_list.size();
				new_info.legion_guid = cur_list_ite->role_guid;
				new_info.set_legion_name(cur_list_ite->role_name);
				new_info.server_id = cur_list_ite->server_id;
				m_tem_legion_info_list.push_back(new_info);
			}
			if (m_tem_legion_info_list.size() >= (e_attack_city_group_level_max * attack_city_group_max_num))
			{
				break;
			}
		}
	}
	void attack_city_ws_mgr::game_end()
	{
		// 重置状态
		m_is_check_time = false;
		m_is_group_time = false;
		m_is_begin_time = false;
		// 清除地图信息
		for (int32 i = 0; i < e_attack_city_group_level_max; ++i)
		{
			m_map_guid_list[i] = 0;
		}
		// 保存分组信息
		save_group_data_all(true);
		// 保存个人排名新
		save_rank_data_all(true);
		// 计算临时分组
		tem_game_group();
	}
	void attack_city_ws_mgr::send_attack_city_legion_info(guid_64 legion_guid, guid_64 role_guid, int32 server_id)
	{
		game_proto_attack_city_info_list msg;
		msg.set_is_have_rank(false);
		msg.set_legion_level(-1);
		bool is_have_legion = false;

		for (int32 i = 0; i < m_rank_list.size(); ++i)
		{
			// 如果可以在排名列表中找到 说明军团有排名信息
			if (m_rank_list[i].legion_guid == legion_guid)
			{
				msg.set_is_have_rank(true);
				break;
			}
		}
		msg.set_is_in_game_time(0);
		for (int32 i = 0; i < m_legion_info_list.size(); ++i)
		{
			if (m_legion_info_list[i].group_level == 0)
			{
				// 设置连胜军团信息
				msg.set_win_num_legion_name(m_legion_info_list[i].legion_name);
				msg.set_win_num(m_legion_info_list[i].winning_streak_num);
			}
			if (m_legion_info_list[i].legion_guid == legion_guid && m_legion_info_list[i].group_level < (e_attack_city_group_level_c * attack_city_group_max_num))
			{
				// 设置自己军团排名
				is_have_legion = true;
				msg.set_is_in_game_time(1);
				msg.set_legion_level(m_legion_info_list[i].group_level);
			}
		}
		
		if (m_is_begin_time || m_is_group_time)
		{
			msg.set_is_in_game_time(2);
		}

		// 设置分组信息
		for (int32 i = 0; i < m_tem_legion_info_list.size(); ++i)
		{
			game_proto_attack_city_info_level_info* legion_info = msg.add_list_info();
			if (nullptr == legion_info)
			{
				break;
			}
			legion_info->set_guid_a(m_tem_legion_info_list[i].legion_guid.A);
			legion_info->set_guid_b(m_tem_legion_info_list[i].legion_guid.B);
			legion_info->set_legion_name(m_tem_legion_info_list[i].legion_name);
			legion_info->set_group_level(m_tem_legion_info_list[i].group_level);
			if (false == is_have_legion && m_tem_legion_info_list[i].legion_guid == legion_guid)
			{
				msg.set_legion_level(m_tem_legion_info_list[i].group_level);
			}
		}

		// 如果分组信息中找不到军团就到战力排名中寻找
		if (false == is_have_legion)
		{
			ranking_list* ranking_list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_legion_gs_all);
			if (nullptr == ranking_list_ptr)
			{
				return;
			}
			int32 rank_num = 1;
			msg.set_gs_rank(-1);
			ranking_list_ite cur_list_ite = ranking_list_ptr->begin();
			for (int32 i = 0; i < 100; ++i, ++cur_list_ite)
			{
				if (cur_list_ite == ranking_list_ptr->end())
				{
					break;
				}
				bool is_have = false;
				for (int32 j = 0; j < m_legion_info_list.size(); ++j)
				{
					if (m_legion_info_list[j].legion_guid == cur_list_ite->role_guid && m_legion_info_list[j].group_level < (e_attack_city_group_level_c * attack_city_group_max_num))
					{
						is_have = true;
						break;
					}
				}
				if (is_have)
				{
					continue;
				}
				if (cur_list_ite->role_guid == legion_guid)
				{
					msg.set_gs_rank(rank_num);
					break;
				}
				rank_num++;
				if (rank_num > (e_attack_city_group_level_max * attack_city_group_max_num))
				{
					break;
				}
			}
		}
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			cross::send_msg_to_ws(role_guid, server_id, e_msgindex_ws2ws_send_attack_city_legion_info, &msg, sizeof(msg));
		}
		else
		{
			session_ptr->send_to_client(&msg, e_msgindex_s2c_attack_city_legion_info);
		}
		
	}
	void attack_city_ws_mgr::send_attack_city_legion_rank_info(guid_64 legion_guid, guid_64 role_guid, int32 server_id)
	{
		game_proto_get_attack_city_rank_list_end msg;
		// 找到军团排名分组
		int32 legion_level = -1;
		for (int32 i = 0; i < m_rank_list.size(); ++i)
		{
			if (m_rank_list[i].legion_guid == legion_guid)
			{
				legion_level = m_rank_list[i].group_level;
				break;
			}
		}

		// 判断分组是否有效
		if (legion_level < e_attack_city_group_level_s || legion_level > e_attack_city_group_level_c)
		{
			return;
		}

		// 找到分组中的所有军团
		std::vector<guid_64> legion_guid_list;
		legion_guid_list.clear();
		for (int32 i = 0; i < m_rank_list.size(); ++i)
		{
			if (m_rank_list[i].legion_guid == m_rank_list[i].role_guid && m_rank_list[i].group_level == legion_level)
			{
				legion_guid_list.push_back(m_rank_list[i].legion_guid);
			}
		}

		// 找出参与玩家数据
		for (int32 i = 0; i < legion_guid_list.size(); ++i)
		{
			game_proto_attack_city_rank_info_list* list_info = msg.add_rank_list();
			if (nullptr == list_info)
			{
				continue;
			}
			guid_64 legion_guid = legion_guid_list[i];
			for (int32 j = 0; j < m_rank_list.size(); ++j)
			{
				if (legion_guid == m_rank_list[j].legion_guid)
				{
					s_attack_city_rank_info legion_info = m_rank_list[j];
					if (legion_info.legion_guid == legion_info.role_guid)
					{
						list_info->set_legion_name(legion_info.role_name);
						list_info->set_legion_source(legion_info.get_role_info(e_attack_city_rank_info_role_source));
						list_info->set_guid_a(legion_info.legion_guid.A);
						list_info->set_guid_b(legion_info.legion_guid.B);
						list_info->set_mode_index(legion_info.group_rank);
						list_info->set_show_index(legion_info.get_role_info(e_attack_city_rank_info_rank_index));
					}
					else
					{
						game_proto_attack_city_rank_info* rank_info = list_info->add_info_list();
						if (nullptr == rank_info)
						{
							continue;
						}
						rank_info->set_guid_a(legion_info.role_guid.A);
						rank_info->set_guid_b(legion_info.role_guid.B);
						rank_info->set_role_name(legion_info.role_name);
						rank_info->set_role_source(legion_info.get_role_info(e_attack_city_rank_info_role_source));
						rank_info->set_kill_player(legion_info.get_role_info(e_attack_city_rank_info_kill_player_num));
						rank_info->set_kill_statue(legion_info.get_role_info(e_attack_city_rank_info_kill_statue_num));
						rank_info->set_kill_door(legion_info.get_role_info(e_attack_city_rank_info_kill_gate_num));
						rank_info->set_repair_statue(legion_info.get_role_info(e_attack_city_rank_info_repair_statue_num));
						rank_info->set_kill_boss(legion_info.get_role_info(e_attack_city_rank_info_kill_boss_num));
					}
				}
			}
		}
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			cross::send_msg_to_ws(role_guid, server_id, e_msgindex_ws2ws_send_attack_city_legion_rank_info, &msg, sizeof(msg));
		}
		else
		{
			session_ptr->send_to_client(&msg, e_msgindex_s2c_attack_city_legion_rank_info);
		}
	}
	void attack_city_ws_mgr::send_load_succend(int32 server_id)
	{
		ws2ws_sync_attack_city_legion_info msg;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_sync_attack_city_legion_info, &msg, sizeof(msg));
	}
	void attack_city_ws_mgr::refresh_legion_list()
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_attack_city))
		{
			return;
		}
		std::vector<s_attack_city_legion_info> _info_list;
		_info_list.clear();
		ranking_list* list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_legion_gs_all);
		for (int32 i = 0; i < m_legion_info_list.size(); i++)
		{
			guid_64 legion_guid = m_legion_info_list[i].legion_guid;
			// 先刷新军团服务器
			if (nullptr != list_ptr)
			{
				ranking_list_ite ite = list_ptr->begin();
				for (; ite != list_ptr->end(); ++ite)
				{
					if (ite->role_guid == legion_guid)
					{
						m_legion_info_list[i].server_id = ite->server_id;
						break;
					}
				}
			}
			// 判断服务器是否有效
			if (!world_server::getInstance().is_have_this_server(m_legion_info_list[i].server_id))
			{
				CONSOLE_INFO("attack_city_ws refresh_del_legion:{} legion_guid:{}", m_legion_info_list[i].server_id, m_legion_info_list[i].legion_guid.server_64);
			}
			else
			{
				_info_list.push_back(m_legion_info_list[i]);
			}
		}
		m_legion_info_list = _info_list;
		save_group_data_all(true);
	}
	void attack_city_ws_mgr::clear_legion_list()
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_attack_city))
		{
			return;
		}
		m_legion_info_list.clear();
		save_group_data_all(true);

		clear_rank_data(true);
	}
	void attack_city_ws_mgr::change_legion_name(guid_64 legion_guid, xstring legion_name)
	{
		for (int32 i = 0; i < m_legion_info_list.size(); i++)
		{
			if (m_legion_info_list[i].legion_guid == legion_guid)
			{
				m_legion_info_list[i].set_legion_name(legion_name.c_str());
			}
		}

		for (int32 i = 0; i < m_rank_list.size(); i++)
		{
			if (m_rank_list[i].legion_guid == legion_guid&& m_rank_list[i].legion_guid == m_rank_list[i].role_guid)
			{
				m_rank_list[i].set_role_name(legion_name.c_str());
			}
		}
	}
	bool attack_city_ws_mgr::is_in_game_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_attack_city, e_activity_time_get_gaming);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}
	bool attack_city_ws_mgr::is_in_check_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_attack_city, e_activity_time_get_ready);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}
	bool attack_city_ws_mgr::is_in_group_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_attack_city, e_activity_time_get_ready);
		if (act_left > 0 && act_left < get_config_data(e_attack_city_data_group_time))
		{
			return true;
		}
		return false;
	}
	void attack_city_ws_mgr::recv_other_ws_transger_gate_map(guid_64 role_guid, guid_64 legion_guid, int32 server_id)
	{
		ws2ws_transfer_attack_city_map_result msg;
		guid_64 map_guid;
		msg.role_guid = role_guid;
		msg.legion_guid = legion_guid;
		msg.map_temp_id = get_config_data(e_attack_city_data_map_id);
		msg.war_idex = 0;
		msg.activity_type = (int32)e_activity_type_attack_city;
		msg.server_id = world_server::getInstance().get_cross_id();		//这个server_id是要传送到的地图服务器id
		msg.result = get_map_guid_by_legion(legion_guid, map_guid);
		if (msg.result == e_error_code_success)
		{
			msg.map_guid = map_guid;
			msg.result = check_can_transfer_to_map(map_guid);
		}
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_transfer_attack_city_map_result, &msg, sizeof(msg));
	}

	int32 attack_city_ws_mgr::check_can_transfer_to_map(guid_64 map_guid)
	{
		// 检查活动是否开启
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_attack_city))
		{
			return (int32)e_error_code_map_attack_city_game_not_open;
		}
		//检查是否在活动时间内
		if (is_in_game_time() == false)
		{
			return (int32)e_error_code_map_attack_city_game_not_begin;
		}
		if (false == map_guid.is_valid())
		{
			return (int32)e_error_code_map_attack_city_map_not_join;
		}
		//检查地图是否有效
		auto map_ent = get_entity(map_guid);
		if (nullptr == map_ent)
		{
			return (int32)e_error_code_map_attack_city_map_not_join;
		}
		auto map_cp = map_ent->get_component<cs_map_component>();
		if (map_cp->m_map_state >= e_map_state_over)
		{
			return (int32)e_error_code_map_attack_city_map_not_join;
		}
		//检查地图是否已满
		if (cs_map_system::is_full(map_guid))
		{
			return (int32)e_error_code_map_attack_city_player_max;
		}
		return (int32)e_error_code_success;
	}
	void attack_city_ws_mgr::transfer_to_map_with_gate_msg(guid_64 role_guid, int32 error_ret, int32 map_temp_id, guid_64 map_guid, int32 war_idex, int32 server_id)
	{
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			CONSOLE_INFO("attack_city_ws transfer_error role_guid = {}", role_guid.server_64);
			return;
		}
		e_error_code ret = (e_error_code)error_ret;
		std::string notice_id = "";
		switch (ret)
		{
		case faith::e_error_code_success:
			break;
		case faith::e_error_code_map_attack_city_game_not_open:
			notice_id = "90306347";//活动未开启
			break;
		case faith::e_error_code_map_attack_city_game_not_begin:
			notice_id = "90306348";//活动未开始
			break;
		case faith::e_error_code_map_attack_city_map_not_join:
			notice_id = "90306349";//活动地图未创建
			break;
		case faith::e_error_code_map_attack_city_legion_not_join:
			notice_id = "90306350";//军团无法参与
			break;
		case faith::e_error_code_map_attack_city_player_max:
			notice_id = "90306351";//地图玩家已满
			break;
		case faith::e_error_code_map_attack_city_game_end:
			notice_id = "90306352";//活动已结束
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
	int32 attack_city_ws_mgr::get_map_guid_by_legion(guid_64 legion_guid, guid_64& map_guid)
	{
		s_attack_city_legion_info& legion_info = get_legion_info(legion_guid);
		if (false == legion_info.is_valid())
		{
			return (int32)e_error_code_map_attack_city_legion_not_join;
		}
		// 如果设置为2 说明军团对应地图已经结束了
		if (legion_info.is_check >= (int32)e_attack_check_type_join_end)
		{
			return (int32)e_error_code_map_attack_city_game_end;
		}
		int32 group_level = legion_info.group_level / attack_city_group_max_num;
		if (group_level < 0 || group_level >= e_attack_city_group_level_max)
		{
			return (int32)e_error_code_map_attack_city_legion_not_join;
		}
		map_guid = m_map_guid_list[group_level];
		return (int32)e_error_code_success;
	}
	s_attack_city_legion_info & attack_city_ws_mgr::get_legion_info(guid_64 legion_guid)
	{
		for (int32 i = 0; i < m_legion_info_list.size(); ++i)
		{
			if (m_legion_info_list[i].legion_guid == legion_guid)
			{
				return m_legion_info_list[i];
			}
		}
		return m_empty_legion_info;
	}
	void attack_city_ws_mgr::send_info_all_to_other_ws(int32 server_id)
	{
		send_load_succend(server_id);
	}
	int32 attack_city_ws_mgr::get_config_data(int32 data_type)
	{
		if (data_type < 0 || data_type >= e_attack_city_data_max)
		{
			return -1;
		}
		return m_config_data[data_type];
	}
}