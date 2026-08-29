#include "big_player_ws_mgr.h"
#include "utility/cs_date.hpp"
#include "utility/guid_gen.h"
#include "utility/globle_data.h"
#include "client_session_mgr.hpp"
#include <mail_msg.hpp>
#include "legion/legion_ws_city_war.h"
#include "legion/legion_ws.h"
#include "legion/legion_ws_mgr.h"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_group/gate_proxy.hpp"
#include "world_server.hpp"
#include "utility/init_unit.h"
#include <world_server_msg.hpp>
#include "lua/script_mgr.h"
#include "system/scene/cs_map_mgr_system.h"
#include "net.pb.h"
#include "character.pb.h"

namespace faith
{
	big_player_ws_mgr::big_player_ws_mgr()
	{
		clear_data();
	}

	big_player_ws_mgr::~big_player_ws_mgr()
	{


	}

	void big_player_ws_mgr::clear_data()
	{
		for (int32 i = 0; i < e_big_player_type_max; ++i)
		{
			player_guid_array[i].clear_data();
			player_info_array[i].clear();
			create_list[i] = -1;
			statue_guid_array[i].clear_data();
		}
		m_last_save_time = 0;
		m_timer = 0;
		m_last_tick_time = 0;
	}

	void big_player_ws_mgr::tick(int32 dt)
	{
		if (is_use_lua())
		{
			return;
		}
		m_timer += dt;
		if (m_timer - m_last_tick_time > 1000)
		{
			m_last_tick_time = m_timer;
			if (m_timer - m_last_save_time > 180000)
			{
				m_last_save_time = m_timer;
				//三分钟一存档
				save_data_to_db();
			}
			for (int32 i = 0; i < e_big_player_type_max; ++i)
			{
				//if (create_list[i] >= 0)
				//{
				//	if (crate_big_player_statue_by_big_type(create_list[i]))
				//	{
				//		create_list[i] = -1;
				//	}
				//}
			}
		}
	}

	void big_player_ws_mgr::clear_title_with_big_player_clear(int32 big_player_type)
	{
		int32 title_type = get_big_player_title_type(big_player_type);
		if (title_type < 0)
		{
			return;
		}
		client_session* temp_all_session = client_session_mgr::getInstance().get_all_session();
		if (nullptr == temp_all_session)
		{
			return;
		}
		int32 session_used_num = client_session_mgr::getInstance().get_session_array_used();
		for (int32 i = 0; i <= session_used_num; ++i)
		{
			client_session& client_session_ref = temp_all_session[i];
			if (client_session_ref.is_vaild())
			{
				ws2cs_clear_big_player_title msg;
				msg.role_guid = client_session_ref.get_role_guid();
				msg.title_type = title_type;
				bool is_use = proto_by_lua(e_msg_index_ws2cs_clear_pk_king_title);
				if (is_use == false)
				{
					client_session_ref.send_to_cs(&msg, sizeof(msg));
				}
				else
				{
					faith::ws2cs_proto::clear_big_player_title pro_msg;
					pro_msg.set_role_guid(client_session_ref.get_role_guid().server_64);
					pro_msg.set_title_type(title_type);
					client_session_ref.send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_clear_pk_king_title);
				}
			}
		}
	}

	void big_player_ws_mgr::set_big_player_guid(e_big_player_type big_type, guid_64 player_guid, int32 from)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "big_player_mgr_set_big_player_guid", 0, false, "%d%l%d", big_type, player_guid.server_64,from);
			return;
		}
		if (big_type >= e_big_player_type_max)
		{
			return;
		}
		guid_64 old_player_guid = player_guid;
		player_guid_array[big_type] = player_guid;
		//save_data_to_db();
		//load_detail_data(big_type);
	}

	guid_64 big_player_ws_mgr::get_big_player_guid(e_big_player_type big_type)
	{
		if (is_use_lua())
		{
			xstring  ret = "";
			script_mgr::get_instance().call_func(nullptr, "big_player_mgr_get_big_player_guid", 1, false, "%d>%s", big_type, &ret);
			int64  ret_64 = init_unit::change_string_to_i64(ret);
			return guid_64(ret_64);
		}
		if (big_type >= e_big_player_type_max)
		{
			return guid_64();
		}
		return player_guid_array[big_type];
	}

	void big_player_ws_mgr::save_data_to_db()
	{
		server2dp_proto_ws2dp_save_big_player_to_db msg;
		for (int32 i = 0; i < e_big_player_type_max; ++i)
		{
			msg.add_role_guid(player_guid_array[i].server_64);
			msg.add_player_type(i);
		}
		 ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_big_player);
		//ws2dp_save_big_player_to_db msg;
		//for (int32 i = 0; i < e_big_player_type_max; ++i)
		//{
		//	msg.data_info[i].role_guid = player_guid_array[i];
		//	msg.data_info[i].player_type = i;
		//}
		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_save_big_player_to_db));
	}
	void big_player_ws_mgr::load_data_by_db()
	{
		server2dp_proto_ws2dp_load_big_player_by_db msg;
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_big_player);

		//ws2dp_load_big_player_by_db msg;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_load_big_player_by_db));
	}
	void big_player_ws_mgr::load_detail_data(int32 index)
	{
		if (index < 0 || index >= e_big_player_type_max)
		{
			return;
		}
		guid_64 TempGuid= player_guid_array[index];
		if ( !TempGuid.is_valid() )
		{
			return;
		}

		server2dp_proto_ws2dp_load_big_player_detail msg;
		msg.set_role_guid(TempGuid.server_64);
		msg.set_big_type(index);
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_big_player_detail);
		//ws2dp_load_big_player_detail msg;
		//msg.role_guid = TempGuid;
		//msg.big_type = index;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_load_big_player_detail));

		//这俩不load
		player_info_array[index].mask_data_block(s_fake_player_info::efpi_spirit);
		player_info_array[index].mask_data_block(s_fake_player_info::efpi_fight_att);
	}
	void big_player_ws_mgr::load_data_by_db_end(const s_big_player_db* data_info, int32 data_num)
	{
		if (is_use_lua())
		{
			return;
		}
		if (data_num > e_big_player_type_max)
		{
			return;
		}
		for (int32 i = 0; i < data_num; ++i)
		{
			int32 player_type = data_info[i].player_type;
			if (player_type >= e_big_player_type_max)
			{
				continue;
			}
			if (false == world_server::getInstance().is_sky_island_server() && true == init_unit::is_cross_server_big_player(i, world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity), world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war)))
			{
				continue;
			}
			player_guid_array[player_type] = data_info[i].role_guid;
			player_info_array[player_type].guid = guid_gen::make_guid();
			load_detail_data(player_type);
		}
		if (world_server::getInstance().get_cross_id() <= 0)
		{
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_big_player);
		}
		else
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_big_player);
				send_big_player_msg_to_other_server(true);
			}
			else
			{
				world_server::getInstance().send_to_gate_get_msg(e_ws_flag_big_player);
			}
		}
	}
	void big_player_ws_mgr::handle_base_detail_info(int32 big_type,const s_unit_info role_info)
	{
		if (big_type < 0 || big_type >= e_big_player_type_max)
		{
			return;
		}
		player_info_array[big_type].role_info = role_info;
		player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_info);
		load_end(big_type);
	}
	void big_player_ws_mgr::handle_item_info(int32 big_type, const s_item_info* item_info)
	{
		if (big_type < 0 || big_type >= e_big_player_type_max)
		{
			return;
		}
		int32 item_count = 0;
		for (int32 i = 0; i < role_show_item_num; ++i)
		{
			if (!item_info[i].item_guid.is_valid())
			{
				continue;
			}
			player_info_array[big_type].item_data[i] = item_info[i];
			item_count++;
		}
		player_info_array[big_type].item_num = item_count;
		player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_item);
		load_end(big_type);
	}
	void big_player_ws_mgr::handle_att_info(int32 big_type, const s_base_att_info& att_info)
	{
		if (big_type < 0 || big_type >= e_big_player_type_max)
		{
			return;
		}
		player_info_array[big_type].att_info = att_info;
		player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_att);
		load_end(big_type);
	}
	void big_player_ws_mgr::handle_skill_info(int32 big_type, const s_skill_info* skill_info)
	{
		if (big_type < 0 || big_type >= e_big_player_type_max)
		{
			return;
		}
		for (int32 i = 0; i < MAX_SKILL_NUM; ++i)
		{
			player_info_array[big_type].skill_data[i] = skill_info[i];
		}
		player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_skill);
		load_end(big_type);
	}

	void big_player_ws_mgr::handle_buff_info(int32 big_type, const s_buff_info* buff_info)
	{
		if (big_type < 0 || big_type >= e_big_player_type_max)
		{
			return;
		}
		for (int32 i = 0; i < MAN_UNIT_BUFF_INST; ++i)
		{
			player_info_array[big_type].buff_data[i] = buff_info[i];
		}
		player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_buff);
		load_end(big_type);
	}

	void big_player_ws_mgr::load_end(int32 big_type)
	{
		if (player_info_array[big_type].all_data_block_masked())
		{
			for (int32 i = 0; i < e_big_player_type_max; ++i)
			{
				if (create_list[i] < 0)
				{
					create_list[i] = big_type;
					break;
				}
			}
		}
	}

	bool big_player_ws_mgr::crate_big_player_statue_by_big_type(int32 big_type)
	{
		if (!player_info_array[big_type].all_data_block_masked())
		{
			return false;
		}

		cs_map_mgr_system::send_big_player(faith::big_world_map_id, player_info_array[big_type], big_type);
		player_info_array[big_type].reset_data_block_mask();
		return true;
	}

	void big_player_ws_mgr::crate_big_player_statue_success(int32 big_type, guid_64 statue_guid)
	{
		if (is_use_lua())
		{
			return;

		}
		if (big_type < 0 || big_type >= e_big_player_type_max)
		{
			return;
		}
		statue_guid_array[big_type] = statue_guid;
		send_big_player_statue_info();
	}

	void big_player_ws_mgr::send_big_player_statue_info()
	{
		character_proto_update_big_player_statue_guid msg;	
		for (int32 i = 0; i < e_big_player_type_max; ++i)
		{
			msg.add_statue_guid(statue_guid_array[i].A);
			msg.add_statue_guid(statue_guid_array[i].B);
		}
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_update_big_player_statue_guid);
	}

	void big_player_ws_mgr::load_statue_by_ws_proc(guid_64 request_player_guid)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "big_player_mgr_load_statue_by_ws_proc", 0, false, "%l", request_player_guid.server_64);
			return;
		}
		character_proto_update_big_player_statue_guid msg;
		for (int32 i = 0; i < e_big_player_type_max; ++i)
		{
			msg.add_statue_guid(statue_guid_array[i].A);
			msg.add_statue_guid(statue_guid_array[i].B);
		}
		client_session* temp_session = client_session_mgr::getInstance().get_session(request_player_guid);
		// nullptr说明不在线/不存在，cs_uid = 0 说明不在CS上（比如传送中）
		if ((temp_session != nullptr) && (temp_session->get_cs_conn_index() >= 0))
		{
			temp_session->send_to_client(&msg, e_msgindex_s2c_update_big_player_statue_guid);
		}
	}

	void big_player_ws_mgr::send_big_player_msg_to_other_server(bool is_load_flag, int32 server_id)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "big_player_mgr_send_big_player_msg_to_other_server", 0, false, "%b%d", is_load_flag, server_id);
			return;
		}
		if (false == world_server::getInstance().is_sky_island_server() || false == gate_proxy::getInstance().is_gate_run())
		{
			return;
		}
		ws2ws_send_big_player_msg msg;
		for (int32 i = 0; i < e_big_player_type_max; i++)
		{
			msg.data_info[i].player_type = i;
			msg.data_info[i].role_guid = player_guid_array[i];
		}
		msg.is_load_flag = is_load_flag;

		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_big_player_info, &msg, sizeof(msg));
	}

	void big_player_ws_mgr::set_title_session_login(const guid_64& role_guid, bool(&loading_title_flag)[faith::e_title_type_max])
	{
		if (is_use_lua())
		{
			for (int32 i = 0; i < e_big_player_type_max; i++)
			{
				bool bRet = false;
				script_mgr::get_instance().call_func(nullptr, "big_player_mgr_is_set_title_session_login", 1, false, "%d%l", i, role_guid.server_64, &bRet);
				if (bRet)
				{
					int32 flag_pos = get_big_player_title_type(i);
					if (flag_pos >= 0)
					{
						loading_title_flag[flag_pos] = true;
					}
				}
			}
			return;
		}
		for (int32 i = 0; i < e_big_player_type_max; i++)
		{

			if (get_big_player_guid((e_big_player_type)i) == role_guid)
			{
				int32 flag_pos = get_big_player_title_type(i);
				if (flag_pos >= 0)
				{
					loading_title_flag[flag_pos] = true;
				}
			}
		}
	}

	int32 big_player_ws_mgr::get_big_player_title_type(int32 player_type)
	{
		switch (player_type)
		{
		case e_big_player_type_lord_of_city:
			return e_title_type_legion_city_win_chief;
			break;
		case e_big_player_type_lord_or_war:
			return e_title_type_kingofpk;
			break;
		case e_big_player_type_best_barserker:
			return e_title_type_best_barserker;
			break;
		case e_big_player_type_best_wizard:
			return e_title_type_best_wizard;
			break;
		case e_big_player_type_best_guardian:
			return e_title_type_best_guardian;
			break;
		case e_big_player_type_best_assassinator:
			return e_title_type_best_assassinator;
			break;
		case e_big_player_type_gs_first_barserker:
			return e_title_type_gs_ranking_barserker;
			break;
		case e_big_player_type_gs_first_wizard:
			return e_title_type_gs_ranking_wizard;
			break;
		case e_big_player_type_gs_first_guardian:
			return e_title_type_gs_ranking_guardian;
			break;
		case e_big_player_type_gs_first_assassinator:
			return e_title_type_gs_ranking_assassinator;
			break;
		case e_big_player_type_gs_first_player:
			return e_title_type_gs_rank_first;
			break;
		case e_big_player_type_money_first_player:
			return e_title_type_money_rank_first;
			break;
		case e_big_player_type_worship_first_player:
			return e_title_type_worship_rank_first;
			break;
		case e_big_player_type_arena_first_player:
			return e_title_type_arena_rank_first;
			break;
		default:
			return -1;
			break;
		}
		return -1;
	}

	int32 big_player_ws_mgr::get_big_player_type_with_ranking_type(int32 ranking_type)    
	{
		switch (ranking_type)
		{
		case e_RankingIndex_gs_barserker:
			return e_big_player_type_gs_first_barserker;
			break;
		case e_RankingIndex_gs_wizard:
			return e_big_player_type_gs_first_wizard;
			break;
		case e_RankingIndex_gs_guardian:
			return e_big_player_type_gs_first_guardian;
			break;
		case e_RankingIndex_gs_assassinator:
			return e_big_player_type_gs_first_assassinator;
			break;
		case e_RankingIndex_gs:
			return faith::e_big_player_type_gs_first_player;
			break;
		case e_RankingIndex_box_map_level:
			return faith::e_big_player_type_money_first_player;
			break;
		case e_RankingIndex_worship:
			return faith::e_big_player_type_worship_first_player;
			break;
		case e_RankingIndex_arena:
			return faith::e_big_player_type_arena_first_player;
			break;
		default:
			return -1;
			break;
		}
		return -1;
	}

	bool big_player_ws_mgr::is_use_lua()
	{
		static int is_get = 0;
		static bool getRet = false;
		if (is_get == 0)
		{
			bool bRet = false;
			script_mgr::get_instance().call_func(nullptr, "is_big_player_use_lua", 1, false, ">%b", &bRet);
			getRet = bRet;
			is_get = 1;
			return bRet;
		}
		return getRet;

	}
}
