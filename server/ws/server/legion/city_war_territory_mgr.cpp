#include "city_war_territory_mgr.h"
#include "template/template_manager.h"
#include <legion_msg.hpp>
#include "ws_client.hpp"
#include "legion_ws_mgr.h"
#include "Logic/activity_def.hpp"
#include "utility/cs_date.hpp"
#include "utility/init_unit.h"
#include "../client_session_mgr.hpp"
#include "world_server.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_server_pk_msg.hpp"
#include "../ws/server/ranking/ranking_mgr_ws.h"
#include "server_log.hpp"
#include "net.pb.h"
namespace faith
{
	
	city_war_territory_mgr::city_war_territory_mgr()
	{
		m_territory_lists.clear();
		m_overlord_territory_info.clear_data();
		m_legion_city_war_rank.clear();
		m_abstention_city_war_legion.clear();
		m_finish_war_territory_num = 0;
		m_need_del_overload_city_bid_info = false;
	}
	
	s_city_war_territory_info* city_war_territory_mgr::get_territory_info(int32 territory_id)
	{
		if (territory_id <= 0)
		{
			return nullptr;
		}
		if (territory_id == overlord_war_territory_flag)
		{
			if (m_overlord_territory_info.territory_id <= 0)
			{
				m_overlord_territory_info.set_territory_id(territory_id);
			}
			return &m_overlord_territory_info;
		}
		CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, territory_id);
		if (nullptr == territory_cfg)
		{
			return nullptr;
		}
		//map[key] 如果map不包含key 会在map中插入一个key的元素 value取默认值
		s_city_war_territory_info& territory_info = m_territory_lists[territory_id];
		if (territory_info.territory_id <= 0)
		{
			territory_info.set_territory_id(territory_id);
		}
		return &territory_info;
	}

	s_city_war_territory_info*	city_war_territory_mgr::get_territory_info_by_map(guid_64 map_guid)
	{
		for (territory_lists_type::iterator iter = m_territory_lists.begin(); iter != m_territory_lists.end(); ++iter)
		{
			s_city_war_territory_info& terr_info = iter->second;
			if (terr_info.city_war_map_guid == map_guid)
			{
				return &terr_info;
			}
		}
		if (m_overlord_territory_info.city_war_map_guid == map_guid)
		{
			return &m_overlord_territory_info;
		}
		return nullptr;
	}
	
	void city_war_territory_mgr::save_bid_info_into_db(int32 territory_id, int32 bid_index)
	{
		if (territory_id == overlord_war_territory_flag)
		{
			return;
		}
		s_city_war_territory_info* terr_info = get_territory_info(territory_id);
		if (nullptr == terr_info)
		{
			return;
		}
		s_city_war_bid_info* bid_info = terr_info->get_bid_info(bid_index);
		if (nullptr == bid_info) //||!bid_info->is_valid()
		{
			return;
		}
		//ws2dp_save_war_bid_info save_war_bid_info_msg;
		//save_war_bid_info_msg.bid_info = *bid_info;
		//ws_client::getInstance().send_to_dp(&save_war_bid_info_msg, sizeof(save_war_bid_info_msg));

		server2dp_proto_ws2dp_save_war_bid_info msg;
		server2dp_proto_s_city_war_bid_info* bid_info_msg = msg.mutable_bid_info();
		if (nullptr == bid_info_msg)
		{
			return;
		}
		bid_info_msg->set_territory_id(bid_info->territory_id);
		bid_info_msg->set_index_in_terr(bid_info->index_in_terr);
		bid_info_msg->set_legion_guid(bid_info->legion_guid);
		bid_info_msg->set_cur_bid_price(bid_info->cur_bid_price);
		bid_info_msg->set_is_prior_maintain(bid_info->is_prior_maintain);
		bid_info_msg->set_occupy_time_stamp(bid_info->occupy_time_stamp);
		bid_info_msg->set_server_id(bid_info->server_id);
		bid_info_msg->set_legion_name(bid_info->legion_name);
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_war_bid_info);
	}
	void city_war_territory_mgr::clear_bid_info_in_db()
	{
		ws2dp_clear_war_bid_info clear_war_bid_info_msg;
		ws_client::getInstance().send_to_dp(&clear_war_bid_info_msg, sizeof(clear_war_bid_info_msg));
	}

	void city_war_territory_mgr::load_bid_info(const s_city_war_bid_info& bid_info_db)
	{
		int32 territory_id = bid_info_db.territory_id;
		if (overlord_war_territory_flag == territory_id)
		{
			s_city_war_territory_info* terr_info = get_territory_info(territory_id);
			if (nullptr == terr_info)
			{
				return;
			}
			s_city_war_bid_info* bid_info = terr_info->get_bid_info(bid_info_db.index_in_terr);
			if (nullptr == bid_info)
			{
				return;
			}
			*bid_info = bid_info_db;
			legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(bid_info_db.legion_guid);
			if (nullptr != legion_ws_ptr)
			{
				legion_ws_ptr->add_city_war_territory(overlord_war_territory_flag);
			}
			return;
		}
		s_city_war_territory_info* terr_info = get_territory_info(territory_id);
		if (nullptr == terr_info)
		{
			return;
		}
		s_city_war_bid_info* bid_info = terr_info->get_bid_info(bid_info_db.index_in_terr);
		if (nullptr == bid_info)
		{
			return;
		}

		// kero这里要确认下有问题没
		*bid_info = bid_info_db;
		if (bid_info->cur_bid_price == territory_occupier_price_flag)
		{
			legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(bid_info_db.legion_guid);
			if (nullptr != legion_ws_ptr)
			{
				legion_ws_ptr->add_city_war_territory(territory_id);
			}
		}
	}

	void city_war_territory_mgr::on_load_bid_info_finish()
	{
		cacul_occupy_territory_rank();
		if (m_overlord_territory_info.territory_id <= 0)
		{
			m_overlord_territory_info.set_territory_id(overlord_war_territory_flag);
		}
	}

	void city_war_territory_mgr::add_abstention_city_war_legion(int32 territory_id, xstring legion_name)
	{
		m_abstention_city_war_legion[territory_id] = legion_name;
	}

	void city_war_territory_mgr::clear_territory_legion_bid_info(int32 territory_id, guid_64 legion_guid)
	{
		s_city_war_territory_info* terr_info = get_territory_info(territory_id);
		if (nullptr == terr_info)
		{
			return;
		}
		s_city_war_bid_info* bid_info = terr_info->get_bid_info(legion_guid);
		if (nullptr == bid_info)
		{
			return;
		}
		bid_info->clear_data();
		save_bid_info_into_db(territory_id, bid_info->index_in_terr);
	}

	void city_war_territory_mgr::del_all_overload_city_bid_info()
	{
		
		s_city_war_territory_info* terr_info = get_territory_info(overlord_war_territory_flag);
		if (nullptr == terr_info)
		{
			return;
		}
		m_need_del_overload_city_bid_info = false;
		for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
		{
			s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(idx);
			if (nullptr == bid_info_ptr)
			{
				continue;
			}
			legion_ws* old_legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(bid_info_ptr->legion_guid);
			if (old_legion_ws_ptr)
			{
				old_legion_ws_ptr->del_city_war_territory(overlord_war_territory_flag);
			}
			bid_info_ptr->clear_data();
		}
		for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
		{
			s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(i);
			if (nullptr == bid_info_ptr)
			{
				continue;
			}
			//ws2dp_save_war_bid_info save_war_bid_info_msg;
			//save_war_bid_info_msg.bid_info = *bid_info_ptr;
			//ws_client::getInstance().send_to_dp(&save_war_bid_info_msg, sizeof(save_war_bid_info_msg));//将参加霸主军团的军团存个档

			server2dp_proto_ws2dp_save_war_bid_info msg;
			server2dp_proto_s_city_war_bid_info* bid_info_msg = msg.mutable_bid_info();
			if (nullptr == bid_info_msg)
			{
				return;
			}
			bid_info_msg->set_territory_id(bid_info_ptr->territory_id);
			bid_info_msg->set_index_in_terr(bid_info_ptr->index_in_terr);
			bid_info_msg->set_legion_guid(bid_info_ptr->legion_guid);
			bid_info_msg->set_cur_bid_price(bid_info_ptr->cur_bid_price);
			bid_info_msg->set_is_prior_maintain(bid_info_ptr->is_prior_maintain);
			bid_info_msg->set_occupy_time_stamp(bid_info_ptr->occupy_time_stamp);
			bid_info_msg->set_server_id(bid_info_ptr->server_id);
			bid_info_msg->set_legion_name(bid_info_ptr->legion_name);
			ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_war_bid_info);
		}
	}

	bool city_war_territory_mgr::is_have_overload_city_bid_info()
	{
		bool is_have = false;
		for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
		{
			s_city_war_territory_info* terr_info = get_territory_info(overlord_war_territory_flag);
			if (nullptr == terr_info)
			{
				return false;
			}
			s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(idx);
			if (nullptr == bid_info_ptr)
			{
				continue;
			}
			legion_ws* old_legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(bid_info_ptr->legion_guid);
			if (old_legion_ws_ptr)
			{
				is_have = true;
				break;
			}
		}
		return is_have;
	}

	void city_war_territory_mgr::set_occupy_legion_guid(int32 territory_id, guid_64 win_legion_guid)
	{
		if (world_server::getInstance().is_sky_island_server()) 
		{
			if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
			{
				return;
			}
			s_city_war_territory_info* terr_info = get_territory_info(territory_id);
			if (nullptr == terr_info)
			{
				return;
			}
			guid_64	old_city_guid = guid_64();
			int32 old_index = -1;
			int32 server_id = 0;
			xstring legion_name;
			bool isdefense_win = false;
			// 战败处理
			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				s_city_war_bid_info* bid_info = terr_info->get_bid_info(i);
				if (nullptr == bid_info)
				{
					continue;
				}
				if (win_legion_guid == bid_info->legion_guid)
				{
					old_index = i;
					server_id = bid_info->server_id;
					legion_name = bid_info->legion_name;
				}
				if (win_legion_guid == bid_info->legion_guid && territory_occupier_price_flag == bid_info->cur_bid_price) {
					isdefense_win = true;
				}
				if (win_legion_guid != bid_info->legion_guid) 
				{
					send_cross_territory_war_end_Info_to_other_server(bid_info->server_id, bid_info->legion_guid, territory_id, false, false);
				}
				if (territory_occupier_bid_idx == i)
				{
					old_city_guid = bid_info->legion_guid;
					// 占领者索引 不清理先 后面处理				
					continue;
				}
				if (overlord_war_territory_flag == territory_id)
				{
					continue;
				}
				bid_info->clear_data();
				save_bid_info_into_db(territory_id, i);
			}
			//战胜处理
			send_cross_territory_war_end_Info_to_other_server(server_id, win_legion_guid, territory_id, true, isdefense_win);
			
			s_city_war_bid_info* occ_bid_info = terr_info->get_bid_info(territory_occupier_bid_idx);
			if (nullptr == occ_bid_info)
			{
				return;
			}

			if (win_legion_guid == occ_bid_info->legion_guid && territory_occupier_price_flag == occ_bid_info->cur_bid_price)
			{
				//防守胜利
				return;
			}
			occ_bid_info->clear_data();
			occ_bid_info->legion_guid = win_legion_guid;
			occ_bid_info->cur_bid_price = territory_occupier_price_flag;
			occ_bid_info->occupy_time_stamp = time_helper::get_cur_time_new().second;
			occ_bid_info->server_id = server_id;
			occ_bid_info->set_legion_name(legion_name);
			if (overlord_war_territory_flag == territory_id && old_index >= 0 && old_city_guid.is_valid())
			{
				s_city_war_bid_info* old_bid_info = terr_info->get_bid_info(old_index);
				if (old_bid_info != nullptr)
				{
					old_bid_info->clear_data();
					old_bid_info->legion_guid = old_city_guid;
					old_bid_info->cur_bid_price = 1;
				}
			}
			if (overlord_war_territory_flag == territory_id)
			{
				for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
				{
					s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(i);
					if (nullptr == bid_info_ptr)
					{
						continue;
					}
					//ws2dp_save_war_bid_info save_war_bid_info_msg;
					//save_war_bid_info_msg.bid_info = *bid_info_ptr;
					//ws_client::getInstance().send_to_dp(&save_war_bid_info_msg, sizeof(save_war_bid_info_msg));//将参加霸主军团的军团存个档
			
					server2dp_proto_ws2dp_save_war_bid_info msg;
					server2dp_proto_s_city_war_bid_info* bid_info_msg = msg.mutable_bid_info();
					if (nullptr == bid_info_msg)
					{
						return;
					}
					bid_info_msg->set_territory_id(bid_info_ptr->territory_id);
					bid_info_msg->set_index_in_terr(bid_info_ptr->index_in_terr);
					bid_info_msg->set_legion_guid(bid_info_ptr->legion_guid);
					bid_info_msg->set_cur_bid_price(bid_info_ptr->cur_bid_price);
					bid_info_msg->set_is_prior_maintain(bid_info_ptr->is_prior_maintain);
					bid_info_msg->set_occupy_time_stamp(bid_info_ptr->occupy_time_stamp);
					bid_info_msg->set_server_id(bid_info_ptr->server_id);
					bid_info_msg->set_legion_name(bid_info_ptr->legion_name);
					ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_war_bid_info);
				}
				return;
			}
			//occ_bid_info->is_prior_maintain = 0;
			save_bid_info_into_db(territory_id, territory_occupier_bid_idx);
		}
		else {
			s_city_war_territory_info* terr_info = get_territory_info(territory_id);
			if (nullptr == terr_info)
			{
				return;
			}
			legion_ws* winner_legion = legion_ws_mgr::get_instance().get_legion(win_legion_guid);
			if (nullptr == winner_legion)
			{
				return;
			}
			guid_64	old_city_guid = guid_64();
			int32 old_index = -1;
			// 战败处理
			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				s_city_war_bid_info* bid_info = terr_info->get_bid_info(i);
				if (nullptr == bid_info)
				{
					continue;
				}
				if (win_legion_guid == bid_info->legion_guid)
				{
					old_index = i;
				}
				legion_ws* old_legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(bid_info->legion_guid);
				if (win_legion_guid != bid_info->legion_guid && old_legion_ws_ptr) // && territory_occupier_price_flag == bid_info->cur_bid_price
				{
					// 输了的那两个军团
					old_legion_ws_ptr->get_event_logger().add_lose_city_by_war_event(territory_id);
					old_legion_ws_ptr->del_city_war_territory(territory_id);
					old_legion_ws_ptr->send_aoi_legion_info_to_all_member();
				}

				if (territory_occupier_bid_idx == i)
				{
					old_city_guid = bid_info->legion_guid;
					// 占领者索引 不清理先 后面处理
					continue;
				}
				if (overlord_war_territory_flag == territory_id)
				{
					continue;
				}
				bid_info->clear_data();
				save_bid_info_into_db(territory_id, i);
			}

			//战胜处理
			if (territory_id == overlord_war_territory_flag)
			{
				winner_legion->get_event_logger().add_win_overlord_city_war_event(winner_legion->get_chief_name());
			}
			else
			{
				winner_legion->get_event_logger().add_occupy_city_event(winner_legion->get_chief_name(), territory_id);
				winner_legion->send_city_war_reward_with_terr_id(territory_id);
			}

			s_city_war_bid_info* occ_bid_info = terr_info->get_bid_info(territory_occupier_bid_idx);
			if (nullptr == occ_bid_info)
			{
				return;
			}

			if (win_legion_guid == occ_bid_info->legion_guid && territory_occupier_price_flag == occ_bid_info->cur_bid_price)
			{
				//防守胜利
				return;
			}

			// 后面是首次占领的逻辑
			winner_legion->add_city_war_territory(territory_id);
			winner_legion->send_aoi_legion_info_to_all_member();
			occ_bid_info->clear_data();
			occ_bid_info->legion_guid = win_legion_guid;
			occ_bid_info->cur_bid_price = territory_occupier_price_flag;
			occ_bid_info->occupy_time_stamp = time_helper::get_cur_time_new().second;
			if (overlord_war_territory_flag == territory_id && old_index >= 0 && old_city_guid.is_valid())
			{
				s_city_war_bid_info* old_bid_info = terr_info->get_bid_info(old_index);
				if (old_bid_info != nullptr)
				{
					old_bid_info->clear_data();
					old_bid_info->legion_guid = old_city_guid;
					old_bid_info->cur_bid_price = 1;
				}
			}
			if (overlord_war_territory_flag == territory_id)
			{
				for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
				{
					s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(i);
					if (nullptr == bid_info_ptr)
					{
						continue;
					}
					//ws2dp_save_war_bid_info save_war_bid_info_msg;
					//save_war_bid_info_msg.bid_info = *bid_info_ptr;
					//ws_client::getInstance().send_to_dp(&save_war_bid_info_msg, sizeof(save_war_bid_info_msg));//将参加霸主军团的军团存个档

					server2dp_proto_ws2dp_save_war_bid_info msg;
					server2dp_proto_s_city_war_bid_info* bid_info_msg = msg.mutable_bid_info();
					if (nullptr == bid_info_msg)
					{
						return;
					}
					bid_info_msg->set_territory_id(bid_info_ptr->territory_id);
					bid_info_msg->set_index_in_terr(bid_info_ptr->index_in_terr);
					bid_info_msg->set_legion_guid(bid_info_ptr->legion_guid);
					bid_info_msg->set_cur_bid_price(bid_info_ptr->cur_bid_price);
					bid_info_msg->set_is_prior_maintain(bid_info_ptr->is_prior_maintain);
					bid_info_msg->set_occupy_time_stamp(bid_info_ptr->occupy_time_stamp);
					bid_info_msg->set_server_id(bid_info_ptr->server_id);
					bid_info_msg->set_legion_name(bid_info_ptr->legion_name);
					ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_war_bid_info);			
				}
				return;
			}
			//occ_bid_info->is_prior_maintain = 0;
			save_bid_info_into_db(territory_id, territory_occupier_bid_idx);
		}
	}
	
	guid_64 city_war_territory_mgr::get_occupy_legion_guid(int32 territory_id)
	{
		s_city_war_territory_info* terr_info = get_territory_info(territory_id);
		if (nullptr == terr_info)
		{
			return 0;
		}
		s_city_war_bid_info* bid_info = terr_info->get_bid_info(territory_occupier_bid_idx);
		if (nullptr == bid_info)
		{
			return 0;
		}
		if (territory_occupier_price_flag != bid_info->cur_bid_price)
		{
			return 0;
		}
		return bid_info->legion_guid;
	}
	// 返回值说明:
	//  1 正常执行
	// -1 军团剩余资金不足以进行下一次竞标
	// -2 重复竞标 暂时没有这种
	int32 city_war_territory_mgr::bid_one_territory(guid_64 legion_guid, int32 territory_id, int32 bid_index, guid_64 role_guid)
	{
		if (!world_server::getInstance().is_sky_island_server() && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return -1;
		}
		CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, territory_id);
		if (nullptr == territory_cfg)
		{
			return -1;
		}

		//所有城池只允许竞标一个------------------
		bool is_have_bid = false;
		for (int32 i = 121000001; i < 121000005; i++)
		{
			if (territory_id == i)
			{
				continue;
			}
			s_city_war_territory_info* terr_info = get_territory_info(i);
			if (nullptr != terr_info)
			{
				s_city_war_bid_info* exist_bid_info = terr_info->get_bid_info(legion_guid);
				if (exist_bid_info && exist_bid_info->cur_bid_price != territory_occupier_price_flag)
				{
					is_have_bid = true;
					break;
				}
			}
		}

		if (true == is_have_bid)
		{
			return -2;
		}
		//------------------------------------------

		s_city_war_territory_info* terr_info = get_territory_info(territory_id);
		if (nullptr == terr_info)
		{
			return -1;
		}
		
		s_city_war_bid_info* exist_bid_info = terr_info->get_bid_info(legion_guid);
		if (exist_bid_info && exist_bid_info->index_in_terr != bid_index)
		{
			return -2;
		}

		s_city_war_bid_info* bid_info = terr_info->get_bid_info(bid_index);
		if (nullptr == bid_info)
		{
			return -1;
		}
		
		guid_64 old_legion = bid_info->legion_guid;
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return -1;
		}

		if (territory_occupier_price_flag == bid_info->cur_bid_price)
		{
			return -1;
		}
		
		int32 next_bid_price = bid_info->cur_bid_price + territory_cfg->BidAddMoney;
		if (bid_info->cur_bid_price < territory_cfg->BidMoney)
		{
			next_bid_price = territory_cfg->BidMoney;
		}
		int32 need_price = next_bid_price;
		if (legion_guid == old_legion)
		{
			need_price -= bid_info->cur_bid_price;
		}
		
		int32 cur_legion_glory_glow = legion_ws_ptr->get_legion_info(ELegionInfo_asset_money);
		if (cur_legion_glory_glow < need_price)
		{
			return -1;
		}
		
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		if (legion_guid != old_legion)
		{
			legion_ws* old_legion_ws_ptr = legion_ws_mgr_ref.get_legion(old_legion);
			if (nullptr != old_legion_ws_ptr)
			{
				old_legion_ws_ptr->add_asset_money(bid_info->cur_bid_price);
				old_legion_ws_ptr->add_glory_glow(bid_info->cur_bid_price);
				old_legion_ws_ptr->del_city_war_territory(territory_id);
				old_legion_ws_ptr->send_mail_to_officer_with_city_bid(territory_id);
				client_session* session_old = client_session_mgr::getInstance().get_session(old_legion_ws_ptr->get_chief_guid());
				if (nullptr != session_old)
				{
					legion_ws_mgr_ref.get_legion_city_war().send_territory_info_one(session_old, territory_id);
				}
				//添加竞标成功事件
				legion_ws_ptr->get_event_logger().add_bid_win_event(territory_id);
				//添加竞标失败事件
				old_legion_ws_ptr->get_event_logger().add_bid_fail_event(territory_id);
			}
		}
		legion_ws_ptr->add_asset_money(-need_price);
		legion_ws_ptr->add_glory_glow(-need_price);
		legion_ws_ptr->add_city_war_territory(territory_id);
		bid_info->cur_bid_price = next_bid_price;
		bid_info->legion_guid = legion_guid;
		bid_info->set_legion_name(legion_ws_ptr->get_legion_name());
		save_bid_info_into_db(territory_id, bid_index);
		
		client_session* session_cur = client_session_mgr::getInstance().get_session(role_guid);
		if (session_cur)
		{
			legion_ws_mgr_ref.get_legion_city_war().send_territory_info_one(session_cur, territory_id);
		}
		return 1;
	}

	bool city_war_territory_mgr::set_is_prior_maintain(guid_64 legion_guid, int32 territory_id)
	{
		s_city_war_territory_info* terr_info = get_territory_info(territory_id);
		if (nullptr == terr_info)
		{
			return false;
		}
		s_city_war_bid_info* bid_info = terr_info->get_bid_info(territory_occupier_bid_idx);
		if (nullptr == bid_info)
		{
			return false;
		}
		if (territory_occupier_price_flag != bid_info->cur_bid_price)
		{
			return false;
		}
		if (!world_server::getInstance().is_sky_island_server()) {
			legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
			if (nullptr == legion_ws_ptr)
			{
				return false;
			}
		}
		if (legion_guid != bid_info->legion_guid)
		{
			return false;
		}
		bid_info->is_prior_maintain = !bid_info->is_prior_maintain;
		save_bid_info_into_db(territory_id, bid_info->index_in_terr);
		return bid_info->is_prior_maintain;
	}

	void city_war_territory_mgr::on_begin_war(int32 territory_id, guid_64 map_guid)
	{
		s_city_war_territory_info* terr_info = get_territory_info(territory_id);
		if (nullptr == terr_info)
		{
			return;
		}
		terr_info->city_war_map_guid = map_guid;
	}

	void city_war_territory_mgr::get_overload_city_war_bid_legion()
	{
		s_city_war_territory_info* terr_info = get_territory_info(overlord_war_territory_flag);
		if (nullptr == terr_info)
		{
			return;
		}
		legion_ws_mgr::get_instance().refresh_all_legion_power();//更新一次所有军团的战力

		ranking_list* legion_rank = legion_ws_mgr::get_instance().get_legion_ranking_by_ranking_type(e_RankingIndex_legion_core);

		ActivityCommonConfigTemplate* act_com_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + faith::e_activity_type_overlord_city_war);
		if (nullptr != act_com_ptr)
		{
			if (nullptr != legion_rank)
			{
				legion_rank_list& rank_list_ref = *legion_rank;
				ranking_list::iterator new_rank_iter = rank_list_ref.begin();
				for (int32 rank_idx = 0; rank_idx < max_apply_city_war_legion_num; rank_idx++)
				{
					s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(rank_idx);
					if (nullptr == bid_info_ptr)
					{
						continue;
					}
					legion_ws* old_legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(bid_info_ptr->legion_guid);
					if (old_legion_ws_ptr)
					{
						old_legion_ws_ptr->del_city_war_territory(overlord_war_territory_flag);
					}
					bid_info_ptr->clear_data();

					if (new_rank_iter == rank_list_ref.end())
					{
						continue;
					}
					bid_info_ptr->legion_guid = new_rank_iter->role_guid;
					bid_info_ptr->cur_bid_price = 1;
					legion_ws* new_legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(bid_info_ptr->legion_guid);
					if (new_legion_ws_ptr)
					{
						new_legion_ws_ptr->add_city_war_territory(overlord_war_territory_flag);
					}
					++new_rank_iter;
				}
			}
		}
		for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
		{
			s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(i);
			if (nullptr == bid_info_ptr)
			{
				continue;
			}
			//ws2dp_save_war_bid_info save_war_bid_info_msg;
			//save_war_bid_info_msg.bid_info = *bid_info_ptr;
			//ws_client::getInstance().send_to_dp(&save_war_bid_info_msg, sizeof(save_war_bid_info_msg));//将参加霸主军团的军团存个档
			server2dp_proto_ws2dp_save_war_bid_info msg;
			server2dp_proto_s_city_war_bid_info* bid_info_msg = msg.mutable_bid_info();
			if (nullptr == bid_info_msg)
			{
				return;
			}
			bid_info_msg->set_territory_id(bid_info_ptr->territory_id);
			bid_info_msg->set_index_in_terr(bid_info_ptr->index_in_terr);
			bid_info_msg->set_legion_guid(bid_info_ptr->legion_guid);
			bid_info_msg->set_cur_bid_price(bid_info_ptr->cur_bid_price);
			bid_info_msg->set_is_prior_maintain(bid_info_ptr->is_prior_maintain);
			bid_info_msg->set_occupy_time_stamp(bid_info_ptr->occupy_time_stamp);
			bid_info_msg->set_server_id(bid_info_ptr->server_id);
			bid_info_msg->set_legion_name(bid_info_ptr->legion_name);
			ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_war_bid_info);
		}
	}

	void city_war_territory_mgr::on_end_war(int32 territory_id, guid_64 winner_legion_guid, int32 server_id,bool need_send_notice)
	{
		if (world_server::getInstance().is_sky_island_server())
		{
			s_city_war_territory_info* terr_info = get_territory_info(territory_id);
			if (nullptr == terr_info)
			{
				return;
			}

			terr_info->city_war_map_guid.clear_data();
			terr_info->city_war_finish_stamp = time_helper::get_cur_time_new().second;

			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(i);
				if (nullptr == bid_info_ptr)
				{
					continue;
				}				
				//通知ws
				city_war_territory_mgr::get_instance().send_cross_territory_war_end_one_territory_to_other_server(bid_info_ptr->server_id, bid_info_ptr->legion_guid, territory_id, false);				
			}

			if (true == need_send_notice)
			{
				//通知ws
				city_war_territory_mgr::get_instance().send_cross_territory_war_end_one_territory_to_other_server(server_id, winner_legion_guid, territory_id, true);
			}

			if (territory_id == overlord_war_territory_flag)
			{
				//王城争霸不用执行后面的逻辑
				for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
				{
					s_city_war_territory_info* terr_info = get_territory_info(overlord_war_territory_flag);
					if (nullptr == terr_info)
					{
						return;
					}
					s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(i);
					if (nullptr == bid_info_ptr)
					{
						continue;
					}
					legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(bid_info_ptr->legion_guid);
					if (nullptr != legion_ws_ptr)
					{
						bool is_winner_legion = false;
						if (bid_info_ptr->legion_guid == winner_legion_guid)
						{
							is_winner_legion = true;
						}
						//legion_ws_ptr->send_mail_to_all_member_with_master_city_war(is_winner_legion);暂时不发邮件
					}
				}
				return;
			}

			if (winner_legion_guid.is_valid())
			{
				set_occupy_legion_guid(territory_id, winner_legion_guid);
			}

			m_finish_war_territory_num++;
			//都结束了算排行榜
			//int32 terr_num = template_manager::get_instance().get_template_row_size(e_CityWarTerritoryTemplate) - 1; // 去掉王城的配置项
			int32 terr_num = cross_server_territory_city_max_params;
			if (m_finish_war_territory_num >= terr_num)
			{

				cacul_occupy_territory_rank();
				//初始化m_overlord_territory_info
				//m_overlord_territory_info.clear_data();
				//city_war_rank_type::iterator rank_iter = m_legion_city_war_rank.begin();

				m_abstention_city_war_legion.clear();
				m_finish_war_territory_num = 0;
				city_war_rank_type::iterator rank_iter = m_legion_city_war_rank.begin();
				for (; rank_iter != m_legion_city_war_rank.end(); ++rank_iter)
				{
					s_city_war_legion_rank_info& rank_info = *rank_iter;
					ws2ws_refresh_award_state(rank_info.server_id, rank_info.legion_guid);
				}
			}
		}
		else {
			s_city_war_territory_info* terr_info = get_territory_info(territory_id);
			if (nullptr == terr_info)
			{
				return;
			}

			terr_info->city_war_map_guid.clear_data();
			terr_info->city_war_finish_stamp = time_helper::get_cur_time_new().second;

			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(i);
				if (nullptr == bid_info_ptr)
				{
					continue;
				}
				legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(bid_info_ptr->legion_guid);
				if (nullptr != legion_ws_ptr)
				{
					legion_ws_ptr->send_one_territory_to_all_mem(territory_id);
				}
			}

			if (true == need_send_notice)
			{
				territory_notice(territory_id, winner_legion_guid);
			}

			if (territory_id == overlord_war_territory_flag)
			{
				//王城争霸不用执行后面的逻辑
				for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
				{
					s_city_war_territory_info* terr_info = get_territory_info(overlord_war_territory_flag);
					if (nullptr == terr_info)
					{
						return;
					}
					s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(i);
					if (nullptr == bid_info_ptr)
					{
						continue;
					}
					
					legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(bid_info_ptr->legion_guid);
					if (nullptr != legion_ws_ptr)
					{
						bool is_winner_legion = false;
						if (bid_info_ptr->legion_guid == winner_legion_guid)
						{
							is_winner_legion = true;
						}
						//legion_ws_ptr->send_mail_to_all_member_with_master_city_war(is_winner_legion);暂时不发邮件
					}
				}
				return;
			}

			if (winner_legion_guid.is_valid())
			{
				set_occupy_legion_guid(territory_id, winner_legion_guid);
			}

			m_finish_war_territory_num++;
			//都结束了算排行榜
			//int32 terr_num = template_manager::get_instance().get_template_row_size(e_CityWarTerritoryTemplate) - 1; // 去掉王城的配置项
			int32 terr_num = max_territory_num;
			if (m_finish_war_territory_num >= terr_num)
			{

				cacul_occupy_territory_rank();
				//初始化m_overlord_territory_info
				//m_overlord_territory_info.clear_data();
				//city_war_rank_type::iterator rank_iter = m_legion_city_war_rank.begin();

				m_abstention_city_war_legion.clear();
				m_finish_war_territory_num = 0;

				city_war_rank_type::iterator rank_iter = m_legion_city_war_rank.begin();
				for (; rank_iter != m_legion_city_war_rank.end(); ++rank_iter)
				{
					s_city_war_legion_rank_info& rank_info = *rank_iter;
					legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(rank_info.legion_guid);
					if (nullptr == legion_ws_ptr)
					{
						continue;
					}
					legion_ws_ptr->refresh_city_war_can_award();
				}
			}
		}		
	}
	void city_war_territory_mgr::ws2ws_refresh_award_state(int32 server_id,guid_64 legion_guid) {
		ws2ws_refresh_reward_info msg;
		msg.legion_guid = legion_guid;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_refresh_reward_info, &msg, sizeof(msg));
	}
	void city_war_territory_mgr::ws2ws_refresh_award_state_end(guid_64 legion_guid) {
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		legion_ws_ptr->refresh_city_war_can_award();
	}
	int32 city_war_territory_mgr::get_city_num_cross_server() {
		int32 server_id = 0;
		int32 server_num = world_server::getInstance().get_server_num() - 1;//减掉一个gate服
		int32 city_num = 0;
		if (server_num < 6) 
		{
			city_num = min_territory_num_cross_server;
		}
		else 
		{
			city_num = server_num - 2;
			city_num = city_num > max_territory_num_cross_server ? max_territory_num_cross_server : city_num;
		}		
		return city_num;
	}

	void city_war_territory_mgr::cacul_occupy_territory_rank()
	{
		if (world_server::getInstance().is_sky_island_server()) {
			template_manager::template_type* territory_cfgs = template_manager::get_instance().get_templates(e_CityWarTerritoryTemplate);
			if (nullptr == territory_cfgs)
			{
				return;
			}
			std::unordered_map<ui64, s_city_war_legion_rank_info> tmp_rank_legion;
			template_manager::template_type::iterator ite;
			for (ite = territory_cfgs->begin(); ite != territory_cfgs->end(); ++ite)
			{
				CityWarTerritoryTemplate* territory_cfg = (CityWarTerritoryTemplate*)(ite->second);
				if (nullptr == territory_cfg || overlord_war_territory_flag == territory_cfg->attribute_id)
				{
					continue;
				}
				s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(territory_cfg->attribute_id);
				if (nullptr == terr_info_ptr)
				{
					continue;
				}
				s_city_war_bid_info* bid_info = terr_info_ptr->get_bid_info(territory_occupier_bid_idx);
				if (nullptr == bid_info || !bid_info->legion_guid.is_valid())
				{
					continue;
				}
				s_city_war_legion_rank_info& rank_info = tmp_rank_legion[bid_info->legion_guid.server_64];
				rank_info.legion_guid = bid_info->legion_guid;
				rank_info.occupy_terr_num++;
				rank_info.occupy_terr_score += territory_cfg->Score;
				rank_info.server_id = bid_info->server_id;
				if (territory_cfg->Score > rank_info.occupy_terr_highest_score)
				{
					rank_info.occupy_terr_highest_score = territory_cfg->Score;
				}
			}

			m_legion_city_war_rank.clear();
			for (std::unordered_map<ui64, s_city_war_legion_rank_info>::iterator map_ite = tmp_rank_legion.begin(); map_ite != tmp_rank_legion.end(); ++map_ite)
			{
				m_legion_city_war_rank.push_back(map_ite->second);
			}
			m_legion_city_war_rank.sort(std::greater<s_city_war_legion_rank_info>());
		}
		else {
			template_manager::template_type* territory_cfgs = template_manager::get_instance().get_templates(e_CityWarTerritoryTemplate);
			if (nullptr == territory_cfgs)
			{
				return;
			}

			std::unordered_map<ui64, s_city_war_legion_rank_info> tmp_rank_legion;
			template_manager::template_type::iterator ite;
			for (ite = territory_cfgs->begin(); ite != territory_cfgs->end(); ++ite)
			{
				CityWarTerritoryTemplate* territory_cfg = (CityWarTerritoryTemplate*)(ite->second);
				if (nullptr == territory_cfg || overlord_war_territory_flag == territory_cfg->attribute_id)
				{
					continue;
				}
				s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(territory_cfg->attribute_id);
				if (nullptr == terr_info_ptr)
				{
					continue;
				}
				s_city_war_bid_info* bid_info = terr_info_ptr->get_bid_info(territory_occupier_bid_idx);
				if (nullptr == bid_info || !bid_info->legion_guid.is_valid())
				{
					continue;
				}
				s_city_war_legion_rank_info& rank_info = tmp_rank_legion[bid_info->legion_guid.server_64];
				rank_info.legion_guid = bid_info->legion_guid;
				rank_info.occupy_terr_num++;
				rank_info.occupy_terr_score += territory_cfg->Score;
				if (territory_cfg->Score > rank_info.occupy_terr_highest_score)
				{
					rank_info.occupy_terr_highest_score = territory_cfg->Score;
				}
			}

			m_legion_city_war_rank.clear();
			for (std::unordered_map<ui64, s_city_war_legion_rank_info>::iterator map_ite = tmp_rank_legion.begin(); map_ite != tmp_rank_legion.end(); ++map_ite)
			{
				m_legion_city_war_rank.push_back(map_ite->second);
			}
			m_legion_city_war_rank.sort(std::greater<s_city_war_legion_rank_info>());

			legion_rank_list* _rank_list = legion_ws_mgr::get_instance().get_legion_rank(e_legion_rank_type_city_war);
			if (nullptr == _rank_list)
			{
				return;
			}
			_rank_list->clear();
			for (city_war_rank_type::const_iterator iter = m_legion_city_war_rank.begin(); iter != m_legion_city_war_rank.end(); ++iter)
			{
				const s_city_war_legion_rank_info& rank_info = *iter;

				legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(rank_info.legion_guid);
				if (nullptr == legion_ws_ptr)
				{
					continue;
				}
				s_ranking_player_info new_rank_info;
				legion_ws_ptr->gen_info_in_rank_list(new_rank_info, rank_info.occupy_terr_score, e_legion_rank_type_city_war);
				_rank_list->push_back(new_rank_info);
			}
		}
		
	}

	void city_war_territory_mgr::territory_notice(int32 territory_id, guid_64 winner_legion_guid)
	{
		legion_ws* winner_legion = legion_ws_mgr::get_instance().get_legion(winner_legion_guid);
		if (winner_legion == nullptr)
		{
			return;
		}

		if (territory_id == overlord_war_territory_flag)
		{
			winner_legion->notice_world_mistress();
			////添加世界霸主胜利事件
			//winner_legion->get_event_logger().add_win_overlord_city_war_event(winner_legion->get_chief_name());
		}
		else
		{
			winner_legion->notice_occupied_treeitory(territory_id);
			////添加城战胜利事件
			//winner_legion->get_event_logger().add_occupy_city_event(winner_legion->get_chief_name(), territory_id);
		}
	}

	void city_war_territory_mgr::get_all_attended_legions(unit_guid_map& legion_guid_map)
	{
		legion_guid_map.clear();
		for (territory_lists_type::iterator iter = m_territory_lists.begin(); iter != m_territory_lists.end(); ++iter)
		{
			s_city_war_territory_info& terr_info_ref = iter->second;
			for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
			{
				s_city_war_bid_info* bid_info = terr_info_ref.get_bid_info(idx);
				if (nullptr == bid_info || false == bid_info->is_valid())
				{
					continue;
				}
				legion_guid_map[bid_info->legion_guid.server_64] = idx;
			}
		}
	}

	void city_war_territory_mgr::territory_war_clear()
	{
		//清库
		del_all_overload_city_bid_info();
		m_territory_lists.clear();
		m_abstention_city_war_legion.clear();
		m_legion_city_war_rank.clear();
		m_finish_war_territory_num = 0;
	}

	//跨服竞价
	void city_war_territory_mgr::req_cross_server_apply_city_war_bid(guid_64 legion_guid,int32 terr_id, int32 bid_index,guid_64 role_guid) {

		ws2ws_legion_proto_apply_city_war_bid msg;
		msg.bid_index = bid_index;
		msg.terr_id = terr_id;
		msg.role_guid = role_guid;
		msg.legion_guid = legion_guid;
		msg.serverid = world_server::getInstance().get_server_id();
		int32 server_id = world_server::getInstance().get_cross_id();
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_legion_proto_apply_city_war_bid, &msg, sizeof(msg));
	}
	void city_war_territory_mgr::check_cross_server_apply_city_war_bid(int32 serverid,guid_64 legion_guid, int32 terr_id, int32 bid_index,guid_64 role_guid)
	{
		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			return;
		}
		int32 result = 0;
		int32 need_price;
		int32 cur_bid_price;
		int32 next_bid_price;
		guid_64 old_legion;
		do 
		{
			CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, terr_id);
			if (nullptr == territory_cfg)
			{
				result = -1;
				break;
			}
			//所有城池只允许竞标一个------------------
			bool is_have_bid = false;
			for (int32 i = cross_server_territory_war_flag; i < cross_server_territory_war_flag + get_city_num_cross_server(); i++)
			{
				if (terr_id == i)
				{
					continue;
				}
				s_city_war_territory_info* terr_info = get_territory_info(i);
				if (nullptr != terr_info)
				{
					s_city_war_bid_info* exist_bid_info = terr_info->get_bid_info(legion_guid);
					if (exist_bid_info && exist_bid_info->cur_bid_price != territory_occupier_price_flag)
					{
						is_have_bid = true;
						break;
					}
				}
			}

			if (true == is_have_bid)
			{
				result = -2;
				break;
			}
			//------------------------------------------

			s_city_war_territory_info* terr_info = get_territory_info(terr_id);
			if (nullptr == terr_info)
			{
				result =  -1;
				break;
			}

			s_city_war_bid_info* exist_bid_info = terr_info->get_bid_info(legion_guid);
			if (exist_bid_info && exist_bid_info->index_in_terr != bid_index)
			{
				result =  -2;
				break;
			}

			s_city_war_bid_info* bid_info = terr_info->get_bid_info(bid_index);
			if (nullptr == bid_info)
			{
				result = -1;
				break;
			}

			old_legion = bid_info->legion_guid;	

			if (territory_occupier_price_flag == bid_info->cur_bid_price)
			{
				result = -1;
				break;
			}
			next_bid_price = bid_info->cur_bid_price + territory_cfg->BidAddMoney;
			if (bid_info->cur_bid_price < territory_cfg->BidMoney)
			{
				next_bid_price = territory_cfg->BidMoney;
			}
			need_price = next_bid_price;
			cur_bid_price = bid_info->cur_bid_price;
			if (legion_guid == old_legion)
			{
				need_price -= bid_info->cur_bid_price;
			}	
			if (!city_war_mgr.is_during_bid_time(terr_id) || city_war_mgr.is_city_war_begin(terr_id))
			{
				result = -1;
				break;
			}
		} while (false);
		
		ws2ws_legion_proto_apply_city_war_bid_response msg;
		msg.terr_id = terr_id;
		msg.result = result;
		msg.needprice = need_price;
		msg.role_guid = role_guid;
		msg.next_bid_price = next_bid_price;
		msg.old_legion_guid = old_legion;
		msg.bid_index = bid_index;
		cross::send_msg_to_ws(guid_64(), serverid, e_msgindex_ws2ws_legion_proto_apply_city_war_bid_response, &msg, sizeof(msg));
	}
	void city_war_territory_mgr::cross_server_return_city_war_bid_response(guid_64 old_legion, int32 cur_bid_price,int32 terr_id)
	{
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws* old_legion_ws_ptr = legion_ws_mgr_ref.get_legion(old_legion);
		if (nullptr != old_legion_ws_ptr)
		{
			old_legion_ws_ptr->add_asset_money(cur_bid_price);
			old_legion_ws_ptr->add_glory_glow(cur_bid_price);
			old_legion_ws_ptr->del_city_war_territory(terr_id);
			old_legion_ws_ptr->send_mail_to_officer_with_city_bid(terr_id);
			//添加竞标失败事件
			old_legion_ws_ptr->get_event_logger().add_bid_fail_event(terr_id);
		}
	}
	void city_war_territory_mgr::cross_server_apply_city_war_bid_response(int32 terr_id,int32 bid_idnex,int32 result, int32 need_price, guid_64 role_guid,int32 next_bid_price, guid_64 old_legion)
	{
		bool not_in_time = false;
		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
		if (!city_war_mgr.is_during_bid_time(terr_id))
		{
			not_in_time = true;
		}
		int32 result1 = result;	
		if (result1 == 0 && !not_in_time) 
		{
			do 
			{
				legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
				legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(role_guid);
				if (nullptr == legion_ws_ptr)
				{
					return;
				}
				guid_64 legion_guid = legion_ws_ptr->get_legion_guid();

				int32 cur_legion_glory_glow = legion_ws_ptr->get_legion_info(ELegionInfo_asset_money);
				if (cur_legion_glory_glow < need_price)
				{
					result1 = -1;
					break;
				}
				
				ws2ws_legion_proto_apply_city_war_bid_end msg;
				msg.terr_id = terr_id;
				msg.bid_index = bid_idnex;
				msg.legion_guid = legion_guid;
				msg.role_guid = role_guid;
				msg.next_bid_price = next_bid_price;
				memcpy(msg.legion_name, legion_ws_ptr->get_legion_name(), max_name_size);
				msg.server_id = world_server::getInstance().get_server_id();
				msg.need_price = need_price;
				//更新gate
				int32 server_id = world_server::getInstance().get_cross_id();
				cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_legion_proto_apply_city_war_bid_end, &msg, sizeof(msg));
			} while (false);		
		}
		//通知
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}
		if (result1 == 0 && !not_in_time)//如果为0不返回竞标成功，因为接下来有可能会失败
		{
			return;
		}
		if (not_in_time)
		{
			session->send_notice("90202467");
		}
		//返回竞价结束信息
		legion_proto_apply_city_war_bid_end apply_city_war_bid_end_msg;
		apply_city_war_bid_end_msg.set_result(result1);
		session->send_to_client(&apply_city_war_bid_end_msg, e_msgindex_s2c_apply_city_war_bid_end);
	}
	void city_war_territory_mgr::cross_server_apply_city_war_bid_end(int32 territory_id,int32 bid_index,guid_64 legion_guid,guid_64 role_guid,int32 next_bid_price,int32 server_id,xchar legion_name[], int32 need_price)
	{
		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
		if (!city_war_mgr.is_during_bid_time(territory_id) || city_war_mgr.is_city_war_begin(territory_id))
		{
			return;
		}
		s_city_war_territory_info* terr_info = get_territory_info(territory_id);
		if (nullptr == terr_info)
		{
			return;
		}
		int32 terr_id = 0;
		int32 cur_bid_price = 0;
		guid_64 old_legion;
		int32 old_server_id = 0;
		s_city_war_bid_info* exist_bid_info = terr_info->get_bid_info(legion_guid);
		if (exist_bid_info && exist_bid_info->index_in_terr != bid_index)
		{
			return;
		}
		s_city_war_bid_info* bid_info = terr_info->get_bid_info(bid_index);
		if (nullptr == bid_info)
		{
			return;
		}
		terr_id = bid_info->territory_id;
		cur_bid_price = bid_info->cur_bid_price;
		old_legion = bid_info->legion_guid;
		old_server_id = bid_info->server_id;
		bool is_same_legion = (old_legion == legion_guid);

		bid_info->cur_bid_price = next_bid_price;
		bid_info->legion_guid = legion_guid;
		bid_info->server_id = server_id;
		bid_info->set_legion_name(legion_name);
		save_bid_info_into_db(territory_id, bid_index);
		//更新客户端	
		ws2wsreq_legion_send_territory_info msg;
		msg.role_guid = role_guid;
		msg.info = *terr_info;
		msg.need_price = need_price;
		msg.legion_guid = legion_guid;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2wsreq_legion_send_territory_info, &msg, sizeof(msg));
		
		//向所有参加该城池竞价的军团推送信息
		ws2ws_legion_proto_push_city_war_info cur_city_info;
		cur_city_info.city_info = *terr_info;
		for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
		{
			s_city_war_bid_info* bid_info = terr_info->get_bid_info(i);
			if (nullptr == bid_info)
			{
				continue;
			}
			cur_city_info.legion_guid = bid_info->legion_guid;
			cross::send_msg_to_ws(guid_64(), bid_info->server_id, e_msgindex_ws2ws_legion_proto_push_city_war_info, &cur_city_info, sizeof(cur_city_info));
		}
		//竞价返还
		if (!is_same_legion)
		{
			ws2ws_legion_proto_return_city_war_bid_money money_return_msg;
			money_return_msg.legion_guid = old_legion;
			money_return_msg.price = cur_bid_price;
			money_return_msg.terri_id = terr_id;
			cross::send_msg_to_ws(guid_64(), old_server_id, e_msgindex_ws2ws_legion_proto_return_city_war_bid_money, &money_return_msg, sizeof(money_return_msg));
		}
	}
	void city_war_territory_mgr::push_city_info_to_legion(guid_64 legion_guid, s_city_war_territory_info city_info)
	{
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		legion_proto_req_territory_info_end req_territory_info_end_msg;
		legion_proto_city_war_territory_info* territory_info_msg = req_territory_info_end_msg.mutable_terr_info();
		legion_ws_mgr_ref.get_legion_city_war().fill_territory_info_msg_cross(territory_info_msg, city_info);
		legion_ws_ptr->send_message_to_all_member(&req_territory_info_end_msg, e_msgindex_s2c_get_city_war_territory_info_end);
	}
	void city_war_territory_mgr::cross_server_apply_city_war_bid_end(guid_64 role_guid, s_city_war_territory_info info, int32 need_price, guid_64 legion_guid)
	{
		client_session* session_cur = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_cur)
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		//添加竞标成功事件
		legion_ws_ptr->get_event_logger().add_bid_win_event(info.territory_id);
		legion_ws_ptr->add_asset_money(-need_price);
		legion_ws_ptr->add_glory_glow(-need_price);
		legion_ws_ptr->add_city_war_territory(info.territory_id);

		legion_proto_apply_city_war_bid_end apply_city_war_bid_end_msg;
		apply_city_war_bid_end_msg.set_result(0);
		session_cur->send_to_client(&apply_city_war_bid_end_msg, e_msgindex_s2c_apply_city_war_bid_end);

		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();	
		legion_proto_req_territory_info_end req_territory_info_end_msg;
		legion_proto_city_war_territory_info* territory_info_msg = req_territory_info_end_msg.mutable_terr_info();
		legion_ws_mgr_ref.get_legion_city_war().fill_territory_info_msg_cross(territory_info_msg, info);

		session_cur->send_to_client(&req_territory_info_end_msg, e_msgindex_s2c_get_city_war_territory_info_end);
	}
	//跨服参战
	void city_war_territory_mgr::req_cross_server_enter_city_war_map(guid_64 role_guid,int32 terr_id,guid_64 legin_guid) 
	{
		ws2ws_transfer_cross_pk_map msg;
		msg.class_type = 0;
		msg.role_guid = role_guid;
		msg.server_id = world_server::getInstance().get_server_id();
		msg.activity_type = e_activity_type_city_war_cross_server;
		msg.legion_guid = legin_guid;
		msg.terr_id = terr_id;
		int32 server_id = world_server::getInstance().get_cross_id();
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_s2c_pk_state_end, &msg, sizeof(msg));
	}
	void city_war_territory_mgr::recv_other_ws_transfer_gate_map(guid_64 role_guid, int32 terr_id, guid_64 legin_guid,int32 server_id) 
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			return;
		}

		e_error_code ret = e_error_code_success;	
		guid_64 map_guid;
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws_city_war& city_war_mgr = legion_ws_mgr_ref.get_legion_city_war();
		do 
		{
			if (city_war_mgr.is_city_war_begin(terr_id) == false)
			{
				ret = e_error_code_map_cross_territory_war_not_begin;
				break;
			}

			if (city_war_mgr.exist_bid_info(terr_id, legin_guid) == false)
			{
				ret = e_error_code_map_no_territory_war_qualification;
				break;
			}	
			//每日参战判断并更新所在服务器id
			s_city_war_territory_info* terr_info = get_territory_info(terr_id);
			if (nullptr == terr_info)
			{
				return;
			}
			s_city_war_bid_info* exist_bid_info = terr_info->get_bid_info(legin_guid);
			if (nullptr == exist_bid_info)
			{
				return;
			}
			if (exist_bid_info->server_id != server_id)
			{
				exist_bid_info->server_id = server_id;
				save_bid_info_into_db(terr_id, exist_bid_info->index_in_terr);
			}
			map_guid = city_war_mgr.get_city_war_map_guid(terr_id);
			if (!map_guid.is_valid())
			{
				ret = e_error_code_map_cross_city_war_map_error;
				break;
			}
		} while (false);
		int32 error_ret = ret;
		int32 map_temp_id = city_war_map_id;
		int32 war_idx = get_war_idx(legin_guid, terr_id);

		ws2ws_transfer_cross_pk_map_result msg;
		msg.role_guid = role_guid;
		msg.result = error_ret;
		msg.map_guid = map_guid;
		msg.map_temp_id = map_temp_id;
		msg.war_idex = war_idx;
		msg.activity_type = (int32)e_activity_type_city_war_cross_server;
		msg.server_id = world_server::getInstance().get_server_id();
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_s2c_pk_map_island_state_msg, &msg, sizeof(msg));
	}
	int32 city_war_territory_mgr::get_war_idx(guid_64 legion_guid,int terr_id) 
	{
		s_city_war_territory_info*  terr_info = get_territory_info(terr_id);
		if (nullptr == terr_info)
		{
			return 1;
		}
		int32 index = 1;
		for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
		{
			s_city_war_bid_info* bid_info_ptr = terr_info->get_bid_info(idx);
			if (nullptr == bid_info_ptr)
			{
				continue;
			}
			if (bid_info_ptr->legion_guid == legion_guid) 
			{
				index = idx + 1;
				break;
			}
		}
		return index;
	}
	//跨服优先维护
	void city_war_territory_mgr::req_cross_server_territory_prior_maintain(guid_64 legion_guid, int32 TerritoryIds[], guid_64 role_guid,int32 territoryids_num)
	{
			ws2ws_set_territory_prior_maintain msg;
			msg.legin_guid = legion_guid;
			for (int i = 0; i < territoryids_num; i++)
			{
				msg.territory_ids[i] = TerritoryIds[i];
			}
			msg.serverid = world_server::getInstance().get_server_id();
			msg.role_guid = role_guid;
			msg.territory_ids_num = territoryids_num;
			int32 server_id = world_server::getInstance().get_cross_id();
			cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_set_territory_prior_maintain, &msg, sizeof(msg));
				
	}
	void city_war_territory_mgr::req_cross_server_territory_prior_maintain_gate(guid_64 legion_guid, int32 TerritoryIds[], int32 server_id, guid_64 role_guid, int32 territoryids_num)
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			return;
		}
		ws2ws_set_territory_prior_maintain_response msg;
		for (int32 idx = 0; idx < territoryids_num; idx++)
		{
			int32 terr_id = TerritoryIds[idx];
			bool prior_ret = city_war_territory_mgr::get_instance().set_is_prior_maintain(legion_guid, terr_id);

			msg.territory_ids[idx] = terr_id;
			msg.prior_rets[idx] = prior_ret;
		}
		msg.territory_ids_num = territoryids_num;
		msg.role_guid = role_guid;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_set_territory_prior_maintain_response, &msg, sizeof(msg));
	}
	void city_war_territory_mgr::req_cross_server_territory_prior_maintain_end(guid_64 role_guid, int32 TerritoryIds[],bool prior_rets[], int32 territoryids_num)
	{
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

		// 如果申请参加城战的角色没有加入任何军团就不能申请
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(role_guid);

		if (nullptr != legion_ws_ptr) 
		{
			for (int32 idx = 0; idx < territoryids_num; idx++)
			{
				int32 terr_id = TerritoryIds[idx];
				if (terr_id == 0)
				{
					continue;;
				}
				bool prior_ret = prior_rets[idx];
				if (prior_ret) 
				{
					legion_ws_ptr->add_cross_city_territories_prior(terr_id);
					legion_ws_ptr->del_city_war_territory_common(terr_id);
				}
				else
				{
					legion_ws_ptr->add_cross_city_territories_common(terr_id);
					legion_ws_ptr->del_city_war_territory_prior(terr_id);
				}
			}
		}
		//更新客户端信息
		client_session* session_cur = client_session_mgr::getInstance().get_session(role_guid);
		if (session_cur)
		{
			legion_proto_set_territory_prior_maintain_end set_territory_prior_maintain_end_msg;

			for (int32 idx = 0; idx < territoryids_num; idx++)
			{
				int32 terr_id = TerritoryIds[idx];
				if (terr_id == 0) 
				{
					continue;;
				}
				bool prior_ret = prior_rets[idx];
				set_territory_prior_maintain_end_msg.add_terr_ids(terr_id);
				set_territory_prior_maintain_end_msg.add_is_priors(prior_ret);
			}
			session_cur->send_to_client(&set_territory_prior_maintain_end_msg, e_msgindex_s2c_set_territory_prior_maintain_end);
		}
	}
	void city_war_territory_mgr::req_cross_server_legion_city_war_info(guid_64 role_guid)
	{
		ws2wsreq_legion_get_city_war_info msg;
		msg.role_guid = role_guid;
		msg.server_id = world_server::getInstance().get_server_id();
		int32 server_id_ = world_server::getInstance().get_cross_id();
		cross::send_msg_to_ws(guid_64(), server_id_, e_msgindex_ws2wsreq_legion_get_city_war_info, &msg, sizeof(msg));	
	}
	void city_war_territory_mgr::req_cross_server_legion_city_war_info_gate(guid_64 role_guid, int32 server_id) 
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		int32 serverId = server_id;
		ws2wsreq_legion_get_city_war_info_end msg;
		msg.role_guid = role_guid;

		template_manager::template_type* territory_cfgs = template_manager::get_instance().get_templates(e_CityWarTerritoryTemplate);
		if (nullptr == territory_cfgs)
		{
			return;
		}
		int32 index = 0;
		template_manager::template_type::iterator ite;
		for (ite = territory_cfgs->begin(); ite != territory_cfgs->end(); ++ite)
		{
			CityWarTerritoryTemplate* territory_cfg = (CityWarTerritoryTemplate*)(ite->second);
			if (nullptr == territory_cfg)
			{
				continue;
			}
			if (territory_cfg->attribute_id == overlord_war_territory_flag)
			{
				continue;
			}
			s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(territory_cfg->attribute_id);
			if (nullptr == terr_info_ptr)
			{
				continue;
			}
			msg.war_info[index] = *terr_info_ptr;
			index++;
		}
		cross::send_msg_to_ws(guid_64(), serverId, e_msgindex_ws2wsreq_legion_get_city_war_info_end, &msg, sizeof(msg));
	}

	void city_war_territory_mgr::req_cross_server_legion_city_war_info_end(guid_64 role_guid,s_city_war_territory_info war_info[])
	{
		client_session* session_cur = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_cur)
		{
			return;
		}
		legion_proto_get_city_war_info_end get_city_war_info_end_msg;
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws_city_war& city_war_mgr = legion_ws_mgr_ref.get_legion_city_war();

		for (int32 i = 0; i < cross_server_territory_city_max_params; i++ ) 
		{
			s_city_war_territory_info info = war_info[i];
			legion_proto_city_war_territory_info* territory_info_msg = get_city_war_info_end_msg.add_territory_info();		
			city_war_mgr.fill_territory_info_msg_cross(territory_info_msg, info);
		}
		session_cur->send_to_client(&get_city_war_info_end_msg, e_msgindex_s2c_get_city_war_info_end);
	}
	//领奖
	void city_war_territory_mgr::req_get_occupation_daily_award(guid_64 role_guid,guid_64 legion_guid,int32 terri_id)
	{
		ws2wsreq_get_occupation_daily_award msg;
		msg.role_guid = role_guid;
		msg.legion_guid = legion_guid;
		msg.terr_id = terri_id;
		msg.server_id = world_server::getInstance().get_server_id();
		int32 server_id_ = world_server::getInstance().get_cross_id();
		cross::send_msg_to_ws(guid_64(), server_id_, e_msgindex_ws2wsreq_get_occupation_daily_award, &msg, sizeof(msg));
	}
	void city_war_territory_mgr::req_get_occupation_daily_award_gate(guid_64 role_guid, guid_64 legion_guid, int32 terri_id, int32 server_id) 
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		int notice = 0;
		int serverId = server_id;
		do
		{
			legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
			if (legion_guid != city_war_mgr.get_occupy_legion_guid(terri_id))
			{
				notice = 90090403;//未达到领取条件
				break;
			}
			if (true == city_war_mgr.is_during_city_war_time(terri_id) && terri_id != overlord_war_territory_flag)
			{
				notice = 90202127;//如果处于城战期间则不能领取奖励
				break;
			}
		} while (false);
		ws2wsreq_get_occupation_daily_award_end msg;
		msg.role_guid = role_guid;
		msg.terr_id = terri_id;
		msg.legion_guid = legion_guid;
		msg.notice = notice;
		cross::send_msg_to_ws(guid_64(), serverId, e_msgindex_ws2wsreq_get_occupation_daily_award_end, &msg, sizeof(msg));		
	}
	void city_war_territory_mgr::req_get_occupation_daily_award_end(guid_64 role_guid, guid_64 legion_guid, int32 terri_id, int32 notice)
	{
		client_session* session_cur = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_cur)
		{
			return;
		}
		if (notice != 0) 
		{
			session_cur->send_notice(init_unit::change_i32_to_string(notice));
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session_cur->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		s_legion_member_info* member_info_ptr = legion_ws_ptr->get_member(session_cur->get_role_guid());
		if (nullptr == member_info_ptr)
		{
			return;
		}
		int32 legion_city_war_award_flag_data = member_info_ptr->data_ary[e_legion_member_info_last_get_city_award];
		int32 terr_id = terri_id;
		int32 rwd_flag_idx = terr_id - overlord_war_territory_flag + 1;

		int32 is_got_rwd = init_unit::get_flag_data(legion_city_war_award_flag_data, rwd_flag_idx);
		if (1 == is_got_rwd)
		{
			session_cur->send_notice("90095160");
			return;
		}
		init_unit::mark_flag_data(legion_city_war_award_flag_data, rwd_flag_idx);
		legion_ws_ptr->update_member_info_one(session_cur->get_role_guid(), e_legion_member_info_last_get_city_award, legion_city_war_award_flag_data);
		// 通知CS获取奖励
		ws2cs_get_occupation_daily_award get_occupation_daily_award_msg;
		get_occupation_daily_award_msg.role_guid = session_cur->get_role_guid();
		get_occupation_daily_award_msg.territory_id = terr_id;

		bool is_use = proto_by_lua(e_msg_index_ws2cs_get_occupation_daily_award);
		if (is_use == false)
		{
			session_cur->send_to_cs(&get_occupation_daily_award_msg, sizeof(get_occupation_daily_award_msg));
		}
		else
		{
			faith::ws2cs_proto::get_occupation_daily_award pro_msg;
			get_occupation_daily_award_msg.to_proto(pro_msg);
			session_cur->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_get_occupation_daily_award);
		}
	}
	void city_war_territory_mgr::send_cross_territory_war_begin_notice_to_other_server(int32 server_id)
	{
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		ws2wssend_cross_war_begin_notice msg;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2wssend_cross_war_begin_notice, &msg, sizeof(msg));
	}
	void city_war_territory_mgr::send_cross_territory_war_end_one_territory_to_other_server(int32 server_id,guid_64 legion_guid,int32 terri_id,bool need_send_winner_msg)
	{
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		ws2ws_end_one_territory_to_all_mem msg;
		msg.legion_guid = legion_guid;
		msg.territory_id = terri_id;
		msg.need_send_winner_msg = need_send_winner_msg;
		cross::send_msg_to_ws(guid_64(), 0, e_msgindex_ws2ws_end_one_territory_to_all_mem, &msg, sizeof(msg));
	}
	void city_war_territory_mgr::send_cross_territory_war_end_Info_to_other_server(int32 server_id, guid_64 legion_guid, int32 terri_id, bool is_winner,bool isdefense_win)
	{
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		ws2ws_end_war_info msg;
		msg.legion_guid = legion_guid;
		msg.territory_id = terri_id;
		msg.is_winner = is_winner;
		msg.is_defense_win = isdefense_win;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_end_war_info, &msg, sizeof(msg));
	}
	void city_war_territory_mgr::on_cross_server_territory_war_end_info(guid_64 legion_guid, int32 terri_id, bool is_winner,bool is_defense_win)
	{
		if (is_winner)
		{
			legion_ws* winner_legion = legion_ws_mgr::get_instance().get_legion(legion_guid);
			//战胜处理
			if (winner_legion != nullptr)
			{
				winner_legion->get_event_logger().add_occupy_city_event(winner_legion->get_chief_name(), terri_id);
				winner_legion->send_city_war_reward_with_terr_id(terri_id);
				if (!is_defense_win)
				{
					winner_legion->add_cross_city_territories_common(terri_id);
					winner_legion->del_city_war_territory_prior(terri_id);
				}
				else 
				{
					winner_legion->add_cross_city_territories_prior(terri_id);
					winner_legion->del_city_war_territory_common(terri_id);
				}
				winner_legion->send_aoi_legion_info_to_all_member();
			}
		}
		else 
		{
			legion_ws* old_legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
			if (old_legion_ws_ptr) // && territory_occupier_price_flag == bid_info->cur_bid_price
			{
				// 输了的那两个军团
				old_legion_ws_ptr->get_event_logger().add_lose_city_by_war_event(terri_id);
				old_legion_ws_ptr->del_city_war_territory_common(terri_id);
				old_legion_ws_ptr->del_city_war_territory_prior(terri_id);
				old_legion_ws_ptr->send_aoi_legion_info_to_all_member();
			}
		}
	}

	//检查合服之后的服务器id
	//如果city_war_bid_info表里的serverid不存在，直接删除
	//因为跨服逻辑修改，调整为服务器被挪走时，删除该服务器的军团
	void city_war_territory_mgr::remove_bid_info_when_server_remove(int32 server_id)
	{
		for (territory_lists_type::iterator iter = m_territory_lists.begin(); iter != m_territory_lists.end(); ++iter)
		{
			s_city_war_territory_info& terr_info_ref = iter->second;
			for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
			{
				s_city_war_bid_info* bid_info = terr_info_ref.get_bid_info(idx);
				if (nullptr == bid_info || false == bid_info->is_valid())
				{
					continue;
				}
				if (server_id == bid_info->server_id)
				{
					clear_territory_legion_bid_info(terr_info_ref.territory_id, bid_info->legion_guid);
				}
			}
		}
	}

	void city_war_territory_mgr::clear_bid_info_for_time()
	{
		for (territory_lists_type::iterator iter = m_territory_lists.begin(); iter != m_territory_lists.end(); ++iter)
		{
			s_city_war_territory_info& terr_info_ref = iter->second;
			for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
			{
				s_city_war_bid_info* bid_info = terr_info_ref.get_bid_info(idx);
				if (nullptr == bid_info || false == bid_info->is_valid())
				{
					continue;
				}
				clear_territory_legion_bid_info(terr_info_ref.territory_id, bid_info->legion_guid);

			}
		}
		if (world_server::getInstance().is_sky_island_server())
		{
			send_city_info_all_to_all_ws();
		}
	}

	void city_war_territory_mgr::clear_all_bid_info()
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		for (territory_lists_type::iterator iter = m_territory_lists.begin(); iter != m_territory_lists.end(); ++iter)
		{
			s_city_war_territory_info& terr_info_ref = iter->second;
			for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
			{
				s_city_war_bid_info* bid_info = terr_info_ref.get_bid_info(idx);
				if (nullptr == bid_info || false == bid_info->is_valid())
				{
					continue;
				}
				clear_territory_legion_bid_info(terr_info_ref.territory_id, bid_info->legion_guid);
			
			}
		}
	}

	void city_war_territory_mgr::refresh_bid_info_when_server_list_end()
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		ranking_list* list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_legion_gs_all);
		for (territory_lists_type::iterator iter = m_territory_lists.begin(); iter != m_territory_lists.end(); ++iter)
		{
			s_city_war_territory_info& terr_info_ref = iter->second;
			for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
			{
				s_city_war_bid_info* bid_info = terr_info_ref.get_bid_info(idx);
				if (nullptr == bid_info || false == bid_info->is_valid())
				{
					continue;
				}
				guid_64 legion_guid = bid_info->legion_guid;
				if (nullptr != list_ptr)
				{
					ranking_list_ite ite = list_ptr->begin();
					for (; ite != list_ptr->end(); ++ite)
					{
						if (ite->role_guid == legion_guid)
						{
							bid_info->server_id = ite->server_id;
							break;
						}
					}
				}
				if (!world_server::getInstance().is_have_this_server(bid_info->server_id))
				{
					CONSOLE_INFO("refresh_bid_info_when_server_list_end_del:{} legion_guid:{}", bid_info->server_id, bid_info->legion_guid.server_64);
					clear_territory_legion_bid_info(terr_info_ref.territory_id, bid_info->legion_guid);
				}
			}
		}
	}

	void city_war_territory_mgr::send_city_info_all_to_all_ws() 
	{
		send_city_info_all_to_other_ws(0);
		//std::set<int32> server_id_arr;
		//world_server::getInstance().get_server_id_arr(server_id_arr);
		//for (std::set<int32>::iterator it = server_id_arr.begin(); it != server_id_arr.end(); ++it)
		//{
		//	if (*it == world_server::getInstance().get_cross_id())
		//	{
		//		continue;
		//	}
		//	
		//}
	}
	void city_war_territory_mgr::change_legion_name(guid_64 legion_guid, xstring legion_name)
	{
		for (int32 i = 0; i < m_bid_infos.bid_infos_num; ++i)
		{
			if (m_bid_infos.bid_infos[i].legion_guid == legion_guid)
			{
				m_bid_infos.bid_infos[i].set_legion_name(legion_name);
			}
		}
		for (int32 i = 0; i < max_apply_city_war_legion_num; ++i)
		{
			if (m_overlord_territory_info.bid_legions[i].legion_guid == legion_guid)
			{
				m_overlord_territory_info.bid_legions[i].set_legion_name(legion_name);
			}
		}
		for (auto it = m_territory_lists.begin(); it != m_territory_lists.end(); ++it)
		{
			s_city_war_territory_info& territory_info = it->second;
			for (int32 i = 0; i < max_apply_city_war_legion_num; ++i)
			{
				if (territory_info.bid_legions[i].legion_guid == legion_guid)
				{
					territory_info.bid_legions[i].set_legion_name(legion_name);
					save_bid_info_into_db(territory_info.territory_id, i);
				}
			}
		}
		send_city_info_all_to_all_ws();
	}
	void city_war_territory_mgr::send_city_info_all_to_other_ws(int32 server_id) 
	{
		s_city_war_bid_info	bid_legions[cross_server_territory_city_bid_info_max];
		int32 index = 0;
		for (territory_lists_type::iterator iter = m_territory_lists.begin(); iter != m_territory_lists.end(); ++iter)
		{
			if (index >= cross_server_territory_city_bid_info_max)
			{
				break;
			}
			s_city_war_territory_info& terr_info_ref = iter->second;
			if (terr_info_ref.territory_id == overlord_war_territory_flag) 
			{
				continue;
			}
			for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
			{
				s_city_war_bid_info* bid_info = terr_info_ref.get_bid_info(idx);
				if (nullptr == bid_info || false == bid_info->is_valid())
				{
					continue;
				}
				if (bid_info->cur_bid_price == territory_occupier_price_flag && server_id == bid_info->server_id) 
				{
					bid_legions[index] = *bid_info;
				}
				index++;
			}
		}
		ws2ws_bid_info_to_ws msg;
		msg.bid_infos_num = index;
		for (int32 i = 0; i < cross_server_territory_city_bid_info_max;i++)
		{
			msg.bid_infos[i] = bid_legions[i];
		}
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_bid_info_to_ws, &msg, sizeof(msg));
	}
	void city_war_territory_mgr::send_city_info_all_to_other_ws_end(guid_64 legion_guid, s_city_war_bid_info bid_infos[],int32 bid_info_num)
	{
		m_bid_infos.bid_infos_num = bid_info_num;
		for (int32 i = 0 ; i < bid_info_num;i++)
		{
			if (i >= cross_server_territory_city_bid_info_max) 
			{
				break;
			}
			m_bid_infos.bid_infos[i] = bid_infos[i];
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		if (legion_ws_mgr_ref.get_is_load_over()) 
		{
			pre_load_bid_info();
		}
		else 
		{
			legion_ws_mgr_ref.set_is_need_load(true);
		}
	}
	void city_war_territory_mgr::pre_load_bid_info()
	{
		for (int32 i = 0; i < m_bid_infos.bid_infos_num; i++)
		{
			if (i >= cross_server_territory_city_bid_info_max) 
			{
				break;
			}
			s_city_war_bid_info bid_info =  m_bid_infos.bid_infos[i];
			if (false == bid_info.is_valid() || bid_info.territory_id == 0) 
			{
				continue;
			}
			if (bid_info.cur_bid_price != territory_occupier_price_flag)
			{
				continue;;
			}
			legion_ws* m_legion = legion_ws_mgr::get_instance().get_legion(bid_info.legion_guid);
			if (m_legion != nullptr)
			{
				if (bid_info.is_prior_maintain) 
				{
					m_legion->add_cross_city_territories_prior(bid_info.territory_id);
				}
				else 
				{
					m_legion->add_cross_city_territories_common(bid_info.territory_id);
				}
			}
		}
	}
	void city_war_territory_mgr::send_cross_territory_war_maintain_terrories_to_other_server(int32 server_id)
	{
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		ws2ws_maintain_terrories msg;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_maintain_terrories, &msg, sizeof(msg));
	}
}