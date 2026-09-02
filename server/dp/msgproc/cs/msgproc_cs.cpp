/********************************************************************
	created:	2015年7月24日15:37:47
	file base:	msgproc_cs
	file ext:	cpp
	author:		zhy

	purpose:
*********************************************************************/
#include <stdio.h>
#if _MSC_VER
#define snprintf _snprintf
#endif
#include "msgproc_cs.hpp"
#include "../../dbproxy_service.hpp"
#include <char_msg.hpp>
#include <mail_msg.hpp>
#include <event_msg.hpp>
#include <core.hpp>
#include <Logic/char_def.hpp>
#include "data_manager/data_manager.hpp"
#include "msgproc_achievement.hpp"
#include "msgproc_att.hpp"
#include "msgproc_pokedex.hpp"
#include "msgproc_buff.hpp"
#include "msgproc_element_heart.hpp"
#include "msgproc_item.hpp"
#include "msgproc_worship_target.hpp"
#include "msgproc_mission.hpp"
#include "msgproc_money.hpp"
#include "msgproc_convert.hpp"
#include "msgproc_goods.hpp"
#include "msgproc_skill.hpp"
#include "msgproc_special_name.hpp"
#include "msgproc_welfare.hpp"
#include "msgproc_demons_tower.hpp"
#include "msgproc_mail.hpp"
#include "msgproc_show.hpp"
#include "msgproc_must_do.hpp"
#include "msgproc_title.hpp"
#include "msgproc_guide.hpp"
#include "msgproc_talent.hpp"
#include "msgproc_treasure.hpp"
#include "msgproc_first_time_do.hpp"
#include "msgproc_arena.hpp"
#include "msgproc_highest_record.hpp"
#include "msgproc_service_goal.hpp"
#include "msgproc_auction.hpp"
#include "msgproc_service_rank.hpp"
#include "msgproc_gain_treasure.hpp"
#include "msgproc_cs_time_limit_activity.hpp"
#include "msgproc_boss_island.hpp"
#include "msgproc_oracle_trial.hpp"
#include "msgproc_seven_day_goal.hpp"
#include "server_log.hpp"
#include  "template/template_manager.h"
#include "msgproc_role_harry_info.hpp"
#include "msgproc_phantom.hpp"
#include "msgproc_chat.hpp"
#include "msgproc_recycle.h"
#include "dp2cs.pb.h"
#include "cs2dp.pb.h"
#include "common/Base64.h"
#include "buff_def.hpp"
#include <rlog.hpp>


namespace faith
{


	// 角色上线统一loadindg链处理
	void cs2dp_req_char_online_loading_data(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(cs2dp_load_data_online))
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message("cs2dp_req_char_online_loading_data data_len = {}",  data_len));
			return;
		}

		const cs2dp_load_data_online* pdata = static_cast<const cs2dp_load_data_online*>(data_ptr);
		if (NULL == pdata)
		{
			_RLOG_(MINFO, "cs2dp_req_char_online_loading_data NULL == pdata ");
			return;
		}
		cs2dp_req_load_char_info_cs(connindex, pdata->role_guid, pdata->up_role_guid, pdata->unit_array_index);
	}

	void cs2dp_req_loading_arena_player_data(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_load_arena_player* pmsg = static_cast<const cs2dp_load_arena_player*>(data_ptr);
		if (NULL == pmsg || data_len != sizeof(cs2dp_load_arena_player))
		{
			_RLOG_(MINFO, "cs2dp_req_loading_arena_player_data NULL == pdata ");
			return;
		}

		cs2dp_req_load_arena_char_role_info(connindex, pmsg->role_guid, pmsg->map_guid);
		cs2dp_req_load_arena_char_att(connindex, pmsg->role_guid, pmsg->map_guid);
		cs2dp_req_load_arena_char_skill(connindex, pmsg->role_guid, pmsg->map_guid);
		cs2dp_req_load_arena_char_item(connindex, pmsg->role_guid, pmsg->map_guid);

		cs2dp_req_load_arena_char_spirit(connindex, pmsg->role_guid, pmsg->map_guid);
		cs2dp_req_load_arena_char_fight_att(connindex, pmsg->role_guid, pmsg->map_guid);
		cs2dp_req_load_arena_char_buff(connindex, pmsg->role_guid, pmsg->map_guid);
		cs2dp_req_load_arena_char_belief(connindex, pmsg->role_guid, pmsg->map_guid);
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	void cs2dp_req_load_char_info_cs(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		dp2cs_load_role_info msgData;
		int32 data_num = 0;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(up_role_guid.server_64) << _XTEXT(".sp_role_info_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.logic_id = e_data_flag_info;
		query.role_guid = role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&cs2dp_req_load_char_info_cs_end, _1, connindex, role_guid, up_role_guid, unit_array_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_load_other_info(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		cs2dp_req_load_char_achievement(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_att(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_pokedex(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_tinder(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_buff(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_element_heart(connindex, role_guid, up_role_guid, unit_array_index);
		for (int32 i = e_bag_type_none + 1; i < e_bag_type_max; ++i)
		{
			cs2dp_req_load_char_data(connindex, role_guid, up_role_guid, unit_array_index, e_data_flag(e_data_flag_item + i));
		}
		cs2dp_req_load_char_data(connindex, role_guid, up_role_guid, unit_array_index, e_data_flag_logic_info);
		cs2dp_req_load_char_data(connindex, role_guid, up_role_guid, unit_array_index, e_data_flag_time);
		cs2dp_req_load_char_data(connindex, role_guid, up_role_guid, unit_array_index, e_data_flag_map);
		cs2dp_req_load_char_data(connindex, role_guid, up_role_guid, unit_array_index, e_data_flag_sprite_fetter);
		cs2dp_req_load_char_data(connindex, role_guid, up_role_guid, unit_array_index, e_data_flag_passive_skill);
		cs2dp_req_load_char_data(connindex, role_guid, up_role_guid, unit_array_index, e_data_flag_item_upgrade);
		cs2dp_req_load_char_data(connindex, role_guid, up_role_guid, unit_array_index, e_data_flag_time_activity);
		cs2dp_req_load_char_mission(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_money(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_goods(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_skill(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_legion_skill(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_special_name(connindex, role_guid, up_role_guid, unit_array_index);
		//cs2dp_req_load_char_mail(connindex, role_guid, up_role_guid, unit_array_index);
		//cs2dp_req_load_char_mail_item(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_show(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_globel_mail_had_cs(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_worship_target(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_yesterday_must_do_remain(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_convert(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_welfare(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_active_degree(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_welfare_random_get(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_title(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_guide_record(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_first_time_do(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_talent(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_player_highest_record(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_service_goal_proc(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_daily_must_do_count(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_auction_load_self_selling(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_service_rank_reward_state(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_gain_treasure_info(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_treasure(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_time_limit_activity_info(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_boss_island_info(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_seven_day_goal_proc(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_role_harry_info(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_phantom(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_oracle_trial_info(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_chat_record(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_load_person_information_proc(connindex, role_guid, up_role_guid, unit_array_index);


		cs2dp_load_role_competition_proc(connindex, role_guid, up_role_guid, unit_array_index, e_competition_type_ladder);
		cs2dp_load_role_competition_proc(connindex, role_guid, up_role_guid, unit_array_index, e_competition_type_element);
		
		cs2dp_req_load_char_recycle(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_recycle_task(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_req_load_char_recycle_invited(connindex, role_guid, up_role_guid, unit_array_index);

		cs2dp_load_role_dragontrip_proc(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_load_role_skytreasure_proc(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_load_role_starark_proc(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_load_role_time_feed_back_proc(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_load_role_time_limit_gift_proc(connindex, role_guid, up_role_guid, unit_array_index);
		cs2dp_load_subscribe_daily_info_proc(connindex, role_guid, up_role_guid, unit_array_index);
	}

	static void cs2dp_load_role_info_send_lua(uint32 connindex, const dp2cs_load_role_info& msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_info_db msg_db;
		msg_db.set_role_guid(msgData.data_info.role_guid.server_64);
		msg_db.set_account(msgData.data_info.account);
		msg_db.set_role_name(msgData.data_info.role_name);
		msg_db.set_role_appearance(msgData.data_info.role_appearance);
		msg_db.set_role_first_kill_ary(msgData.data_info.role_first_kill_npc_id_arr);
		msg_db.set_map_guid(msgData.data_info.map_guid.server_64);
		for (int32 i = 0; i < e_role_i64_info_max; i++)
		{
			msg_db.add_data_i64_ary(msgData.data_info.data_i64_ary[i]);
		}
		for (int32 i = 0; i < e_role_info_max; i++)
		{
			msg_db.add_data_ary(msgData.data_info.data_ary[i]);
		}
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_character);

	}
	void cs2dp_req_load_char_info_cs_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		if (!result.error && result.query.data_select.row_count == 1)
		{
			dp2cs_load_role_info msgData;
			msgData.unit_array_index = unit_array_index;
			db_read_data(&result, &(msgData.data_info), sizeof(msgData.data_info));
			msgData.data_info.data_ary[e_role_info_server_id] = SERVERCONFIG->game_id;
			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			data_set* data_set_ptr = data_manager::get_instance().add_data_set(role_guid);
			if (nullptr != data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_info, &(msgData.data_info), sizeof(msgData.data_info));
			}
			cs2dp_req_load_other_info(connindex, role_guid, up_role_guid, unit_array_index);
		}
		else
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_info;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
	}

	void cs2dp_req_save_char_info_cs(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(cs2dp_save_character))
		{
			return;
		}
		const cs2dp_save_character* pdata = static_cast<const cs2dp_save_character*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (pdata->role_info.data_ary[e_role_info_recharge_num] > 0)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call sp_gm_set_role_right('")
				<< pdata->role_info.account << _XTEXT("',")
				<< e_role_right_vip_point << _XTEXT(",")
				<< pdata->role_info.data_ary[e_role_info_recharge_num] << _XTEXT(")");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
		// 更新角色信息表
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->role_info.role_guid.server_64) << _XTEXT(".sp_role_info_save(");
		sql << _XTEXT("'UPDATE role_info SET ");
		sql << "role_appearance = " << "\\\'" << pdata->role_info.role_appearance << "\\\',";
		sql << "map_guid = " << pdata->role_info.map_guid.server_64 << ",";
		sql << "role_first_kill_npc_id_arr = " << "\\\'" << pdata->role_info.role_first_kill_npc_id_arr << "\\\',";
		int32 i64_info_len = e_role_i64_info_max;
		for (int32 i = 0; i < i64_info_len; ++i)
		{
			sql << e_role_i64_info_str[i] << " = " << pdata->role_info.data_i64_ary[i] << ",";
		}
		int32 info_len = e_role_info_max - 1;
		int32 msg_len = 0;
		for (int32 i = 0; i < info_len; ++i)
		{
			sql << e_role_info_str[i] << " = " << pdata->role_info.data_ary[i] << ",";
		}
		sql << e_role_info_str[info_len] << " = " << pdata->role_info.data_ary[info_len];
		sql << " WHERE role_guid =" << pdata->role_info.role_guid.server_64 << ";');";

		db_query_type query;
		query.logic_id = e_data_flag_info;
		query.role_guid = pdata->role_info.role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_info.role_guid, pdata->unit_array_index, e_data_flag_info, pdata->save_type_ex, sql.c_str());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_save_char_info_cs_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 unit_array_index, e_data_flag save_flag, int32 save_type_ex, xstring sql)
	{
		if (result.error)
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message("cs2dp_req_save_char_info_cs_end sql:{} error:{} type:{} save_flag:{} save_type_ex:{}",  sql,  result.error,  result.query_type,  int32(save_flag),  save_type_ex));
			cs2dp_req_send_info_end(false, connindex, role_guid, unit_array_index, save_flag, save_type_ex);
		}
		else
		{
			if (save_type_ex == (int32)e_save_data_type_exit_game)
			{
				_RLOG_(MINFO, ::faith::log_detail::format_message("cs2dp_req_save_char_info_cs_end role_guid = {} save_flag = {}",  role_guid.server_64,  int32(save_flag)));
			}
			cs2dp_req_send_info_end(true, connindex, role_guid, unit_array_index, save_flag, save_type_ex);
		}
	}
	void cs2dp_req_send_info_end(bool result, uint32 connindex, const guid_64& role_guid, int32 unit_array_index, e_data_flag save_flag, int32 save_type_ex)
	{
		dp2cs_save_data_result reply;
		reply.role_guid = role_guid;
		reply.unit_array_index = unit_array_index;
		reply.save_flag = save_flag;
		reply.save_type_ex = save_type_ex;
		reply.is_ok = result;
		dbproxy_service::getInstance().send_message(connindex, &reply, sizeof(reply));
	}
	void cs2dp_req_del_char_mail(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_del_char_mail* pdata = static_cast<const cs2dp_del_char_mail*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(cs2dp_del_char_mail))
		{
			return;
		}

		if (pdata->data_num < 1)
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->unit_guid.server_64) << _XTEXT(".sp_role_mail_delete_batch('");

		int32 last_index = pdata->data_num - 1;
		for (int32 i = 0; i < pdata->data_num; ++i)
		{
			sql << pdata->mail_guid[i].server_64;

			if (i == last_index)
			{
				sql << "')";
			}
			else
			{
				sql << ",";
			}
		}

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = pdata->mail_guid[0];
		query.handler = boost::bind(&cs2dp_req_del_char_mail_end, _1, connindex, pdata->unit_guid, pdata->unit_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_del_char_mail_end(db_result_type result, uint32 connindex, guid_64 unit_guid, int32 unit_array_index)
	{
		dp2cs_del_mail_end msg;
		msg.unit_index = unit_array_index;
		msg.unit_guid = unit_guid;
		msg.del_result = result.error;
		dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(dp2cs_del_mail_end));
	}
	void cs2dp_req_del_char_mail_item(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_del_char_mail_item* pdata = static_cast<const cs2dp_del_char_mail_item*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(cs2dp_del_char_mail_item))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->unit_guid.server_64) << _XTEXT(".sp_role_mail_item_delete(") << pdata->item_guid.server_64 << ")";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = pdata->item_guid;
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_add_char_mail(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_add_char_mail* pdata = static_cast<const cs2dp_add_char_mail*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(cs2dp_add_char_mail))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		//sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_mail_add(") << pdata->role_guid.server_64 << ",'";
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << _XTEXT(".sp_role_mail_add(") << pdata->role_guid.server_64 << ","
			<< pdata->mail_info.mail_guid.server_64 << ",'";

		guid_64 item1_guid = guid_64(pdata->mail_info.data_ary[EMailInfo_ItemGuid11], pdata->mail_info.data_ary[EMailInfo_ItemGuid12]);
		guid_64 item2_guid = guid_64(pdata->mail_info.data_ary[EMailInfo_ItemGuid21], pdata->mail_info.data_ary[EMailInfo_ItemGuid22]);
		guid_64 item3_guid = guid_64(pdata->mail_info.data_ary[EMailInfo_ItemGuid31], pdata->mail_info.data_ary[EMailInfo_ItemGuid32]);
		guid_64 item4_guid = guid_64(pdata->mail_info.data_ary[EMailInfo_ItemGuid41], pdata->mail_info.data_ary[EMailInfo_ItemGuid42]);
		guid_64 item5_guid = guid_64(pdata->mail_info.data_ary[EMailInfo_ItemGuid51], pdata->mail_info.data_ary[EMailInfo_ItemGuid52]);
		guid_64 sender_guid = guid_64(pdata->mail_info.data_ary[EMailInfo_SenderGuid1], pdata->mail_info.data_ary[EMailInfo_SenderGuid2]);

		sql << "(" << pdata->role_guid.server_64 << ","
			<< pdata->mail_info.mail_guid.server_64 << ",";

		sql << pdata->mail_info.data_ary[EMailInfo_IsNeedDelete] << ",";
		sql << pdata->mail_info.data_ary[EMailInfo_SendTime] << ",";
		sql << item1_guid.server_64 << ",";
		sql << item2_guid.server_64 << ",";
		sql << item3_guid.server_64 << ",";
		sql << item4_guid.server_64 << ",";
		sql << item5_guid.server_64 << ",";
		sql << pdata->mail_info.data_ary[EMailInfo_MoneyTyp1] << ",";
		sql << pdata->mail_info.data_ary[EMailInfo_MoneyNum1] << ",";
		sql << pdata->mail_info.data_ary[EMailInfo_MoneyTyp2] << ",";
		sql << pdata->mail_info.data_ary[EMailInfo_MoneyNum2] << ",";
		sql << pdata->mail_info.data_ary[EMailInfo_ReadState] << ",";
		sql << sender_guid.server_64 << ",";
		sql << pdata->mail_info.data_ary[EMailInfo_ServerId];

		sql << ",\\\'" << pdata->mail_info.text_contents << "\\\',";
		sql << "\\\'" << pdata->mail_info.sender_name << "\\\',";
		sql << "\\\'" << pdata->mail_info.mail_title << "\\\'";

		sql << ")";


		sql << "');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = pdata->role_guid;
		query.handler = boost::bind(&cs2dp_req_add_char_mail_end, _1);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_add_char_mail_end(db_result_type result)
	{


	}

	void cs2dp_req_load_char_globel_mail_had_cs(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(up_role_guid.server_64) << _XTEXT(".sp_role_event_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.logic_id = e_data_flag_globel_mail_had;
		query.role_guid = role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&cs2dp_req_load_char_globel_mail_had_cs_end, _1, connindex, role_guid, unit_array_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	static void cs2dp_load_role_event_send_lua(uint32 connindex, const dp2cs_load_player_had_globel_mail_end & reply_event)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(reply_event.role_guid.server_64);
		msg.set_unit_array_index(reply_event.unit_index);

		faith::db_proto::role_event_db msg_db;
		for (int32 i = 0; i < max_globel_mail_in_same_time; i++)
		{
			msg_db.add_data_ary(reply_event.data_info[i].server_64);
		}

		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}

		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_player_had_globel_mail_end);

	}
	void cs2dp_req_load_char_globel_mail_had_cs_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_globel_mail_guid_db sql_data;
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_globel_mail_guid_db);
		if (result.error || data_size < table_len * result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.error_code = e_data_flag_globel_mail_had;
			error.array_index = unit_array_index;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(&sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size);

			dp2cs_load_player_had_globel_mail_end reply_event;
			reply_event.unit_index = unit_array_index;
			reply_event.role_guid = role_guid;
			memcpy(&(reply_event.data_info), &sql_data.data_info, sizeof(sql_data.data_info));

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_player_had_globel_mail_end);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_event, sizeof(reply_event));
			}
			else
			{
				cs2dp_load_role_event_send_lua(connindex, reply_event);
			}
			return;
		}
	}

	//获取其他玩家信息
	void cs2dp_req_get_other_player_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		cs2dp_req_get_other_player_info_base(connindex, data_ptr, data_len);
		//cs2dp_req_get_other_player_info_equiping(connindex, data_ptr, data_len);
		//cs2dp_req_get_other_player_info_special_name(connindex, data_ptr, data_len);
		//cs2dp_req_get_other_player_info_base_group(connindex, data_ptr, data_len);
		//cs2dp_req_get_other_player_info_spirit(connindex, data_ptr, data_len);
		//cs2dp_req_get_other_player_info_wing(connindex, data_ptr, data_len);
		//cs2dp_req_get_other_player_info_mount(connindex, data_ptr, data_len);
		//cs2dp_req_get_other_player_info_buff(connindex, data_ptr, data_len);
		//cs2dp_req_get_other_player_info_belief(connindex, data_ptr, data_len);
	}
	void cs2dp_req_get_other_player_info_base(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_get_other_player_info* pdata = static_cast<const cs2dp_get_other_player_info*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(cs2dp_get_other_player_info))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->target_guid.server_64) << _XTEXT(".sp_get_other_player_base_info(") << pdata->target_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = pdata->target_guid;
		query.handler = boost::bind(&cs2dp_req_get_other_player_info_base_end, _1, connindex, pdata->role_guid, pdata->target_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_get_other_player_info_base_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid)
	{
		if (!result.error && result.query.data_select.row_count == 1)
		{
			faith::dp2cs_get_other_player_info_base_end msgData;
			msgData.role_guid = role_guid;
			db_read_data(&result, &(msgData.data_info), sizeof(msgData.data_info));
			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));

			cs2dp_req_get_other_player_info_equiping(connindex, role_guid, target_guid, msgData.data_info);
		}
	}
	void cs2dp_req_get_other_player_info_equiping(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(target_guid.server_64) << _XTEXT(".sp_get_other_player_equiping_info(") << target_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = target_guid;
		query.handler = boost::bind(&cs2dp_req_get_other_player_info_equiping_end, _1, connindex, role_guid, target_guid, unit_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_get_other_player_info_equiping_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		faith::dp2cs_get_other_player_info_equiping_end msgData;
		int32	table_len = result.query.data_select.row_count;
		if (!result.error && table_len <= role_show_item_num)
		{
			msgData.role_guid = role_guid;
			db_read_data(&result, &(msgData.data_info), sizeof(msgData.data_info));
			msgData.data_info[0].role_guid = target_guid;	//防止没有找到任何装备时返回到客户端的角色guid为空
			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));

			cs2dp_req_get_other_player_info_special_name(connindex, role_guid, target_guid, unit_info);
		}
	}
	void cs2dp_req_get_other_player_info_special_name(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(target_guid.server_64) << _XTEXT(".sp_get_other_player_special_name_info(") << target_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = target_guid;
		query.handler = boost::bind(&cs2dp_req_get_other_player_info_special_name_end, _1, connindex, role_guid, target_guid, unit_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_get_other_player_info_special_name_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		if (!result.error)
		{
			faith::dp2cs_get_other_player_info_special_name_end msgData;
			msgData.role_guid = role_guid;
			db_read_data(&result, &(msgData.data_info), sizeof(msgData.data_info));
			msgData.data_info.role_guid = target_guid;	//防止没有找到任何称号时返回到客户端的角色guid为空
			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));

			cs2dp_req_get_other_player_info_base_group(connindex, role_guid, target_guid, unit_info);
		}
	}
	void cs2dp_req_get_other_player_info_base_group(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(target_guid.server_64) << _XTEXT(".sp_get_group_name_by_role_guid(") << target_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = target_guid;
		query.handler = boost::bind(&cs2dp_req_get_other_player_info_base_group_end, _1, connindex, role_guid, target_guid, unit_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_get_other_player_info_base_group_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		if (!result.error)
		{
			faith::dp2cs_get_other_player_info_base_group_end msgData;
			msgData.role_guid = role_guid;
			msgData.target_guid = target_guid;
			db_read_data(&result, &(msgData.data_info), sizeof(msgData.data_info));
			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));

			cs2dp_req_get_other_player_info_spirit(connindex, role_guid, target_guid, unit_info);
		}
	}
	void cs2dp_req_get_other_player_info_spirit(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(target_guid.server_64) << _XTEXT(".sp_get_other_player_spirit_info(") << target_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = target_guid;
		query.handler = boost::bind(&cs2dp_req_get_other_player_info_spirit_end, _1, connindex, role_guid, target_guid, unit_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_get_other_player_info_spirit_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		if (!result.error)
		{
			faith::dp2cs_get_other_player_info_spirit_end msgData;
			msgData.role_guid = role_guid;
			db_read_data(&result, &(msgData.data_info), sizeof(msgData.data_info));
			msgData.data_info.role_guid = target_guid;	//防止没有找到任何精灵时返回到客户端的角色guid为空
			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));

			cs2dp_req_get_other_player_info_wing(connindex, role_guid, target_guid, unit_info);
		}
	}
	void cs2dp_req_get_other_player_info_wing(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		int32 showd_wing_template_id = unit_info.data_ary[e_role_info_wing_showd_template_id];
		int32 wing_illusion_range_id = 0;
		if (showd_wing_template_id != 0)
		{
			ItemTemplate* item_wing_template_ptr = GET_TEMPLATE(ItemTemplate, showd_wing_template_id);
			if (item_wing_template_ptr != nullptr)
			{
				WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, item_wing_template_ptr->logic_id);
				if (wing_template_ptr != nullptr)
				{
					if (wing_template_ptr->IllusionOriginalIdRange.size() == faith::e_wing_illusion_originalid_range_max)
					{
						ItemTemplate* item_wing_real_template_ptr = template_manager::get_instance().get_item_template_ptr_by_logic_id(wing_template_ptr->IllusionOriginalIdRange[faith::e_wing_illusion_originalid_range_low]);
						if (item_wing_real_template_ptr != nullptr)
						{
							wing_illusion_range_id = item_wing_real_template_ptr->attribute_id;
						}
					}
				}
			}
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(target_guid.server_64) << _XTEXT(".sp_get_other_player_wing_info(") << target_guid.server_64 << _XTEXT(",") << showd_wing_template_id << _XTEXT(",") << wing_illusion_range_id << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = target_guid;
		query.handler = boost::bind(&cs2dp_req_get_other_player_info_wing_or_mount_end, _1, connindex, role_guid, target_guid, unit_info, true);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_get_other_player_info_mount(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		int32 showd_mount_template_id = unit_info.data_ary[e_role_info_mount_showd_template_id];
		int32 mount_illusion_range_id = 0;
		if (showd_mount_template_id != 0)
		{
			ItemTemplate* item_mount_template_ptr = GET_TEMPLATE(ItemTemplate, showd_mount_template_id);
			if (item_mount_template_ptr != nullptr)
			{
				MountTemplate* mount_template_ptr = GET_TEMPLATE(MountTemplate, item_mount_template_ptr->logic_id);
				if (mount_template_ptr != nullptr)
				{
					if (mount_template_ptr->IllusionOriginalIdRange.size() == 2)
					{
						ItemTemplate* item_mount_real_template_ptr = template_manager::get_instance().get_item_template_ptr_by_logic_id(mount_template_ptr->IllusionOriginalIdRange[0]);
						if (item_mount_real_template_ptr != nullptr)
						{
							mount_illusion_range_id = item_mount_real_template_ptr->attribute_id;
						}
					}
				}
			}
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(target_guid.server_64) << _XTEXT(".sp_get_other_player_mount_info(") << target_guid.server_64 << _XTEXT(",") << showd_mount_template_id << _XTEXT(",") << mount_illusion_range_id << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = target_guid;
		query.handler = boost::bind(&cs2dp_req_get_other_player_info_wing_or_mount_end, _1, connindex, role_guid, target_guid, unit_info, false);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_get_other_player_info_wing_or_mount_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info, bool is_wing)
	{
		faith::dp2cs_get_other_player_info_wing_or_mount_end msgData;
		int sql_count = result.query.data_select.row_count;

		if (!result.error && sql_count <= role_show_item_num)
		{
			msgData.role_guid = role_guid;
			db_read_data(&result, &(msgData.data_info), sizeof(msgData.data_info));

			msgData.data_info[0].role_guid = target_guid;	//防止没有找到任何翅膀或坐骑时返回到客户端的角色guid为空
			msgData.is_wing = is_wing;						//防止没有找到任何翅膀或坐骑时返回到客户端的包类型为空
			//msgData.data_info[0].data_info.data_ary[faith::e_item_info_container_type] = bag_type;	//防止没有找到任何翅膀或坐骑时返回到客户端的包类型为空

			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));

			if (msgData.is_wing)
			{
				cs2dp_req_get_other_player_info_mount(connindex, role_guid, target_guid, unit_info);
			}
			else
			{
				cs2dp_req_get_other_player_info_buff(connindex, role_guid, target_guid);
			}
			cs2dp_req_get_other_player_info_feather(connindex, role_guid, target_guid, unit_info);
		}

	}

	void cs2dp_req_get_other_player_info_buff(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(target_guid.server_64) << _XTEXT(".sp_get_other_player_buff_info(") << target_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = target_guid;
		query.handler = boost::bind(&cs2dp_req_get_other_player_info_buff_end, _1, connindex, role_guid, target_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_get_other_player_info_buff_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid)
	{
		faith::dp2cs_get_other_player_info_buff_end msgData;
		int32	table_len = result.query.data_select.row_count;
		if (!result.error && table_len <= faith::MAX_LS_ENUM_BUFF)
		{
			int32 data_size = sizeof(s_buff_save_db);
			s_buff_save_db sql_data[MAX_LS_ENUM_BUFF];
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			msgData.role_guid = role_guid;
			msgData.target_guid = target_guid;

			for (int32 i = 0; i < table_len; ++i)
			{
				msgData.buff_info[i * e_enum_buff_info_max + e_enum_buff_info_id] = sql_data[i].data_info.data_ary[EBuffInfo_template_id];
				msgData.buff_info[i * e_enum_buff_info_max + e_enum_buff_info_effect_index] = sql_data[i].data_info.data_ary[EBuffInfo_effect_index];
				msgData.data_num += e_enum_buff_info_max;
			}
			dbproxy_service::getInstance().send_message(connindex, &msgData, msgData.get_pak_length());
			cs2dp_req_get_other_player_info_appearance(connindex, role_guid, target_guid);
		}
	}
	void cs2dp_req_get_other_player_info_feather(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(target_guid.server_64) << _XTEXT(".sp_role_feather_load(") << target_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = target_guid;
		query.handler = boost::bind(&cs2dp_req_get_other_player_info_feather_end, _1, connindex, role_guid, target_guid, unit_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_get_other_player_info_feather_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info)
	{
		faith::dp2cs_get_other_player_info_feather_end msgData;
		int32	table_len = result.query.data_select.row_count;
		if (!result.error)
		{
			if (table_len <= e_feather_sub_type_feather4)
			{
				msgData.role_guid = role_guid;
				db_read_data(&result, &(msgData.feather_info), sizeof(msgData.feather_info));
				msgData.feather_info[0].role_guid = target_guid;
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				msgData.role_guid = role_guid;
				msgData.feather_info[0].role_guid = target_guid;
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}

		}
	}

	void cs2dp_req_get_other_player_info_appearance(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(target_guid.server_64) << _XTEXT(".sp_load_role_appearance_id(") << target_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = target_guid;
		query.handler = boost::bind(&cs2dp_req_get_other_player_info_appearance_end, _1, connindex, role_guid, target_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_get_other_player_info_appearance_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid)
	{
		faith::dp2cs_get_other_player_info_appearance_end msgData;
		msgData.role_guid = role_guid;
		msgData.target_guid = target_guid;
		int32	table_len = result.query.data_select.row_count;
		if (!result.error && table_len == 1)
		{
			db_read_data(&result, &(msgData.data_info), sizeof(msgData.data_info));
			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
		}
		dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
	}

	//void cs2dp_save_person_information_to_db_proc

	static void cs2dp_load_role_person_information_send_lua(uint32 connindex, const dp2cs_get_person_information& msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);
		faith::cs2dp_proto::role_person_information_db msg_db;
		
		msg_db.set_role_guid(msgData.person_info.role_guid.server_64);
		msg_db.set_infor_state(msgData.person_info.infor_state);
		msg_db.set_reward_state(msgData.person_info.reward_state);
		msg_db.set_gender(msgData.person_info.gender);
		msg_db.set_personalized_signature(msgData.person_info.personalized_signature);
		msg_db.set_birthday_year(msgData.person_info.birthday_year);
		msg_db.set_birthday_mouth(msgData.person_info.birthday_mouth);
		msg_db.set_birthday_day(msgData.person_info.birthday_day);
		msg_db.set_city(msgData.person_info.city);
		msg_db.set_hometown(msgData.person_info.hometown);
		msg_db.set_online_state(msgData.person_info.online_state);
		for (int32 i = 0; i < person_information_tag_num_max; i++)
		{
			msg_db.add_tags(msgData.person_info.tags[i]);
		}

		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_get_person_information);

		
	}

	void cs2dp_load_person_information_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		dp2cs_get_person_information msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_player_information, &msgData.person_info, data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_get_person_information);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_person_information_send_lua(connindex, msgData);
			}
		}
		else {
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_get_person_information(") << role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.logic_id = e_data_flag_player_information;
			query.role_guid = role_guid;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&cs2dp_get_person_information_end_proc, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}
	void cs2dp_get_person_information_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_role_person_information infor;
		int32 data_size = sizeof(infor);
		int32 table_len = result.query.data_select.row_count;
		if (result.error || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_player_information;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else {
			dp2cs_get_person_information msgData;
			msgData.role_guid = role_guid;
			db_read_data(&result, &(msgData.person_info), sizeof(msgData.person_info));
			//dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			msgData.person_info.role_guid = role_guid;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_get_person_information);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_person_information_send_lua(connindex, msgData);
			}

			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_player_information, &msgData.person_info, sizeof(s_role_person_information), 1);
			}
		}
	}

	bool cs2dp_save_role_person_information_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		s_role_person_information db_row_info;

		faith::cs2dp_proto_role_person_information_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}
	
		db_row_info.role_guid.server_64 = msg.role_guid();
		db_row_info.infor_state = msg.infor_state();
		db_row_info.reward_state = msg.reward_state();
		parse_msg::getInstance().my_memcopy_string(db_row_info.gender, person_infor_gender_max, msg.gender());
		parse_msg::getInstance().my_memcopy_string(db_row_info.personalized_signature , person_infor_personalized_signature_max, msg.personalized_signature());
		db_row_info.birthday_year = msg.birthday_year();
		db_row_info.birthday_mouth = msg.birthday_mouth();
		db_row_info.birthday_day = msg.birthday_day();
		parse_msg::getInstance().my_memcopy_string(db_row_info.city, person_infor_city_max, msg.city());
		parse_msg::getInstance().my_memcopy_string(db_row_info.hometown, person_infor_hometown_max, msg.hometown());
		parse_msg::getInstance().my_memcopy_string(db_row_info.online_state , person_infor_online_state_max, msg.online_state());
		for (int32 i = 0 ; i < msg.tags().size(); i++)
		{
			db_row_info.tags[i] = msg.tags(i);
		}

		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}

		data_set_ptr->add_data_info(e_data_flag_player_information, &db_row_info, sizeof(s_role_person_information), 1);

		return true;


	}

	void cs2dp_load_role_time_feed_back_proc(uint32 connindex, const guid_64 & role_guid, const guid_64 & up_role_guid, const int32 & unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		dp2cs_get_time_feed_back_to_db_end msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_time_feed_back, &msgData.info_list, msgData.date_num))
		{

			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;

			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_time_feed_back_info_load(") << up_role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_time_feed_back;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_load_role_time_feed_back_end_proc, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_load_role_time_feed_back_end_proc(db_result_type result, uint32 connindex, const guid_64 & role_guid, const int32 & unit_array_index)
	{
		s_time_feed_back_db_info sql_data[time_feed_back_to_db_num];
		int32 data_size = sizeof(sql_data);
		int32 table_len = result.query.data_select.row_count;
		if (result.error || data_size < table_len * result.query.data_select.row_size || table_len > time_feed_back_to_db_num)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_time_feed_back;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, table_len * result.query.data_select.row_size);
			dp2cs_get_time_feed_back_to_db_end reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.date_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_data.info_list[i]), &(sql_data[i]), sizeof(s_time_feed_back_db_info));
			}
		
			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_get_time_feed_back_to_db_end));
	
			if (table_len > 0)
			{
				data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
				if (data_set_ptr)
				{
					data_set_ptr->add_data_info(e_data_flag_time_feed_back, &reply_data.info_list, sizeof(s_time_feed_back_db_info) * reply_data.date_num, table_len);
				}
			}
		}
	}

	void cs2dp_save_role_time_feed_back_proc(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const cs2dp_save_time_feed_back_to_db* pdata = static_cast<const cs2dp_save_time_feed_back_to_db*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_time_feed_back, &pdata->info_list, pdata->date_num * sizeof(s_time_feed_back_db_info), pdata->date_num);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_time_feed_back, pdata->save_type_ex);
			}
		}
		if (pdata->date_num <= 0)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << _XTEXT(".sp_time_feed_back_info_clear(") << pdata->role_guid.server_64 << ");";
			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_time_feed_back;
			query.role_guid = pdata->role_guid;
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
		else
		{
			for (int32 i = 0; i < pdata->date_num; ++i)
			{
				sql_builder sql(db_manager::getInstance().get_db_link().game_db);
				sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << _XTEXT(".sp_time_feed_back_info_save(") << pdata->role_guid.server_64 << ",";
				sql << i << ",";
				sql << "'(";
				sql << pdata->info_list[i].role_guid.server_64 << ",";
				sql << pdata->info_list[i]._info.template_id << ",";
				sql << pdata->info_list[i]._info.sub_template_id << ",";
				sql << pdata->info_list[i]._info.info_state << ",";
				sql << pdata->info_list[i]._info.get_time << ",";
				sql << pdata->info_list[i]._info.end_time << ",";
				sql << pdata->info_list[i]._info.money_type << ",";
				sql << pdata->info_list[i]._info.money_num << ")'";
				sql << ");";
				db_query_type query;
				query.sql_str.assign(sql.c_str(), sql.size());
				query.logic_id = e_data_flag_time_feed_back;
				query.role_guid = pdata->role_guid;
				db_manager::getInstance().get_db_link().game_db.add_query(query);
			}
		}
	}

	void cs2dp_load_role_time_limit_gift_proc(uint32 connindex, const guid_64 & role_guid, const guid_64 & up_role_guid, const int32 & unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		dp2cs_get_time_limit_gift_to_db_end msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_time_limit_gift, &msgData.info_list, msgData.date_num))
		{

			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;

			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_time_limit_gift_info_load(") << up_role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_time_limit_gift;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_load_role_time_limit_gift_end_proc, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_load_role_time_limit_gift_end_proc(db_result_type result, uint32 connindex, const guid_64 & role_guid, const int32 & unit_array_index)
	{
		s_time_limit_gift_db_info sql_data[time_limit_gift_db_num];
		int32 data_size = sizeof(sql_data);
		int32 table_len = result.query.data_select.row_count;
		if (result.error || data_size < table_len * result.query.data_select.row_size || table_len > time_limit_gift_db_num)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_time_limit_gift;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, table_len * result.query.data_select.row_size);
			dp2cs_get_time_limit_gift_to_db_end reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.date_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_data.info_list[i]), &(sql_data[i]), sizeof(s_time_limit_gift_db_info));
			}

			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_get_time_limit_gift_to_db_end));

			if (table_len > 0)
			{
				data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
				if (data_set_ptr)
				{
					data_set_ptr->add_data_info(e_data_flag_time_limit_gift, &reply_data.info_list, sizeof(s_time_limit_gift_db_info) * reply_data.date_num, table_len);
				}
			}
		}
	}

	void cs2dp_save_role_time_limit_gift_proc(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const cs2dp_save_time_limit_gift_to_db* pdata = static_cast<const cs2dp_save_time_limit_gift_to_db*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_time_limit_gift, &pdata->info_list, pdata->date_num * sizeof(s_time_limit_gift_db_info), pdata->date_num);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_time_limit_gift, pdata->save_type_ex);
			}
		}
		for (int32 i = 0; i < pdata->date_num; ++i)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << _XTEXT(".sp_time_limit_gift_info_save(") << pdata->role_guid.server_64 << ",";
			sql << pdata->info_list[i]._info.template_id << ",";
			sql << "'(";
			sql << pdata->info_list[i].role_guid.server_64 << ",";
			sql << pdata->info_list[i]._info.template_id << ",";
			sql << pdata->info_list[i]._info.state_info << ",";
			sql << pdata->info_list[i]._info.begin_time << ",";
			sql << pdata->info_list[i]._info.end_time << ",";
			sql << pdata->info_list[i]._info.trigger_num << ")'";
			sql << ");";
			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_time_limit_gift;
			query.role_guid = pdata->role_guid;
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}

	}

	void cs2dp_load_subscribe_daily_info_proc(uint32 connindex, const guid_64 & role_guid, const guid_64 & up_role_guid, const int32 & unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		dp2cs_get_subscribe_daily_info_to_db_end msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_subscribe_daily_info, &msgData.info_list, msgData.date_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;
			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_subscribe_daily_info_load(") << up_role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_subscribe_daily_info;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_load_subscribe_daily_info_end_proc, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_load_subscribe_daily_info_end_proc(db_result_type result, uint32 connindex, const guid_64 & role_guid, const int32 & unit_array_index)
	{
		s_subscribe_daily_db_info sql_data[time_limit_gift_db_num];
		int32 data_size = sizeof(sql_data);
		int32 table_len = result.query.data_select.row_count;
		if (result.error || data_size < table_len * result.query.data_select.row_size || table_len > time_limit_gift_db_num)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_subscribe_daily_info;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, table_len * result.query.data_select.row_size);
			dp2cs_get_subscribe_daily_info_to_db_end reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.date_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_data.info_list[i]), &(sql_data[i]), sizeof(s_subscribe_daily_db_info));
			}

			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_get_subscribe_daily_info_to_db_end));

			if (table_len > 0)
			{
				data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
				if (data_set_ptr)
				{
					data_set_ptr->add_data_info(e_data_flag_subscribe_daily_info, &reply_data.info_list, sizeof(s_subscribe_daily_db_info) * reply_data.date_num, table_len);
				}
			}
		}
	}

	void cs2dp_save_subscribe_daily_info_proc(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const cs2dp_save_subscribe_daily_info_to_db* pdata = static_cast<const cs2dp_save_subscribe_daily_info_to_db*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_subscribe_daily_info, &pdata->info_list, pdata->date_num * sizeof(s_subscribe_daily_db_info), pdata->date_num);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_subscribe_daily_info, pdata->save_type_ex);
			}
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << _XTEXT(".sp_subscribe_daily_info_save(") << pdata->role_guid.server_64 << ",'";
		int32 tempLen = pdata->date_num - 1;
		int32 sql_pro_End = e_pokedex_Info_max - 1;

		for (int32 i = 0; i < pdata->date_num; i++)
		{
			sql << "(" << pdata->role_guid.server_64 << ",";
			sql << pdata->info_list[i]._info.must_do_type << ",";
			sql << pdata->info_list[i]._info.info_state;
			if (i == tempLen)
			{
				sql << ")";
			}
			else
			{
				sql << "),";
			}
		}

		sql << "');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.logic_id = e_data_flag_subscribe_daily_info;
		query.role_guid = pdata->role_guid;
		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_subscribe_daily_info, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_save_person_information_to_db_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(cs2dp_save_person_information_to_db))
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message("cs2dp_save_person_information_to_db_proc data_len = {}",  data_len));
			return;
		}

		const cs2dp_save_person_information_to_db* pdata = static_cast<const cs2dp_save_person_information_to_db*>(data_ptr);
		if (NULL == pdata)
		{
			_RLOG_(MINFO, "cs2dp_save_person_information_to_db_proc NULL == pdata ");
			return;
		}

		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_player_information, &pdata->person_info, sizeof(s_role_person_information), 1);
			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_player_information, pdata->save_type_ex);
			}
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << "call " << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << ".sp_save_person_information(";
		sql << pdata->role_guid.server_64 << ",";
		sql << "'(";
		sql << pdata->role_guid.server_64 << ",";
		sql << pdata->person_info.infor_state << ",";
		sql << pdata->person_info.reward_state << ",";
		sql << "\\\'" << pdata->person_info.gender << "\\\'" << ",";
		sql << "\\\'" << pdata->person_info.personalized_signature << "\\\'" << ",";
		sql << pdata->person_info.birthday_year << ",";
		sql << pdata->person_info.birthday_mouth << ",";
		sql << pdata->person_info.birthday_day << ",";
		sql << "\\\'" << pdata->person_info.city << "\\\'" << ",";
		sql << "\\\'" << pdata->person_info.hometown << "\\\'" << ",";
		sql << "\\\'" << pdata->person_info.online_state << "\\\'" << ",";
		sql << pdata->person_info.tags[0] << ",";
		sql << pdata->person_info.tags[1] << ",";
		sql << pdata->person_info.tags[2] << ",";
		sql << pdata->person_info.tags[3] << ",";
		sql << pdata->person_info.tags[4] << ",";
		sql << pdata->person_info.tags[5] << ",";
		sql << pdata->person_info.tags[6] << ",";
		sql << pdata->person_info.tags[7] << ")'";
		sql << ");";
		db_query_type query;
		query.logic_id = e_data_flag_player_information;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());

		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_player_information, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_save_person_information_to_db_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 unit_array_index, e_data_flag data_flag, int32 save_type_ex)
	{
		if (result.error == 0)
		{
			cs2dp_req_send_info_end(true, connindex, role_guid, unit_array_index, data_flag, save_type_ex);
		}
		else
		{
			//CONSOLE_INFO("cs2dp_req_save_person_information_cs_end sql =") << " error=" << result.error << " type=" << result.query_type << " save_flag=" << e_data_flag_player_information << " save_type_ex=" << save_type);
			cs2dp_req_send_info_end(false, connindex, role_guid, unit_array_index, data_flag, save_type_ex);
		}
	}
	void cs2dp_get_other_person_information_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(cs2dp_get_other_person_information))
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message("cs2dp_save_person_information_to_db_proc data_len = ",  data_len));
			return;
		}

		const cs2dp_get_other_person_information* pdata = static_cast<const cs2dp_get_other_person_information*>(data_ptr);
		if (NULL == pdata)
		{
			_RLOG_(MINFO, "cs2dp_save_person_information_to_db_proc NULL == pdata ");
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->target_guid.server_64) << _XTEXT(".sp_get_person_information(") << pdata->target_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&cs2dp_get_other_person_information_end_proc, _1, connindex, pdata->role_guid, pdata->target_guid, pdata->unit_array_index, pdata->is_ask_player_base_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_get_other_person_information_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const int32& unit_array_index, bool is_ask_player_base_info)
	{
		s_role_person_information infor;
		int32 data_size = sizeof(infor);
		int32 table_len = result.query.data_select.row_count;
		if (result.error || data_size != result.query.data_select.row_size)
		{
		}
		else {
			dp2cs_get_other_person_information msgData;
			msgData.role_guid = role_guid;
			msgData.target_guid = target_guid;
			msgData.is_ask_player_base_info = is_ask_player_base_info;
			db_read_data(&result, &(msgData.person_info), sizeof(msgData.person_info));
			dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
		}
	}
	static void  cs2dp_load_role_competition_send_lua(uint32 connindex, const dp2cs_get_role_competition& msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_competition_db msg_db;

		msg_db.set_role_guid(msgData.competition_info.role_guid.server_64);
		msg_db.set_have_type(msgData.competition_info.have_type);
		msg_db.set_is_buy(msgData.competition_info.is_buy);
		msg_db.set_buy_manual_type(msgData.competition_info.buy_manual_type);
		msg_db.set_point_add_percent(msgData.competition_info.point_add_percent);
		msg_db.set_season(msgData.competition_info.season);
		msg_db.set_competition_begin_time(msgData.competition_info.competition_begin_time);
		msg_db.set_competition_end_time(msgData.competition_info.competition_end_time);
		msg_db.set_level(msgData.competition_info.level);
		msg_db.set_cur_exp(msgData.competition_info.cur_exp);
		msg_db.set_need_reset(msgData.competition_info.need_reset);
		msg_db.set_assist_fight_guid(msgData.competition_info.assist_fight_guid.server_64);

		for (int32 i  = 0; i < role_competition_tag_num; i++)
		{
			msg_db.add_reward_state(msgData.competition_info.reward_state[i]);

		}
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);


		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_get_role_competition);


	}

	void cs2dp_load_role_competition_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index, const int32& competition_type)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		dp2cs_get_role_competition msgData;
		int32 data_num = 1;
		int64 data_flag = e_data_flag_player_competition;
		msgData.competition_info.have_type = competition_type;
		if (competition_type == e_competition_type_element)
		{
			data_flag = e_data_flag_element_competition;
		}
		if (data_set_ptr && data_set_ptr->get_data_info(data_flag, &msgData.competition_info, data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;
			msgData.competition_info.have_type = competition_type;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_get_role_competition);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_competition_send_lua(connindex, msgData);
			}
		}
		else {
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_get_role_competition(") << role_guid.server_64
			<< _XTEXT(",") << competition_type << _XTEXT(");");

			db_query_type query;
			query.logic_id = data_flag;
			query.role_guid = role_guid;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&cs2dp_load_role_competition_end_proc, _1, connindex, role_guid, unit_array_index, competition_type);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}
	void cs2dp_load_role_competition_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index, const int32& competition_type)
	{
		s_role_competition_info infor;
		int32 data_size = sizeof(infor);
		int32 table_len = result.query.data_select.row_count;
		
		int64 data_flag = e_data_flag_player_competition;
		if (competition_type == e_competition_type_element)
		{
			data_flag = e_data_flag_element_competition;
		}
		if (result.error || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = data_flag;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			dp2cs_get_role_competition msgData;
			msgData.unit_array_index = unit_array_index;
			msgData.role_guid = role_guid;
			msgData.competition_info.have_type = competition_type;
			db_read_data(&result, &(msgData.competition_info), sizeof(msgData.competition_info));

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_get_role_competition);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_competition_send_lua(connindex, msgData);
			}
			msgData.competition_info.role_guid = role_guid;
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(data_flag, &msgData.competition_info, sizeof(s_role_competition_info), 1);
			}
		}
	}
	bool cs2dp_save_role_competition_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		s_role_competition_info db_row_info;

		faith::cs2dp_proto_role_competition_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		int64 data_flag = e_data_flag_player_competition;
		if (msg.have_type() == e_competition_type_element)
		{
			data_flag = e_data_flag_element_competition;
		}

		db_row_info.role_guid.server_64 = msg.role_guid();
		db_row_info.have_type = msg.have_type();
		db_row_info.is_buy = msg.is_buy();
		db_row_info.buy_manual_type = msg.buy_manual_type();
		db_row_info.point_add_percent = msg.point_add_percent();
		db_row_info.season = msg.season();
		db_row_info.competition_begin_time = msg.competition_begin_time();
		db_row_info.competition_end_time = msg.competition_end_time();
		db_row_info.level = msg.level();
		db_row_info.cur_exp = msg.cur_exp();
		db_row_info.need_reset = msg.need_reset();
		db_row_info.assist_fight_guid.server_64 = msg.assist_fight_guid();
		for (int32 i = 0; i < msg.reward_state().size(); i++)
		{
			db_row_info.reward_state[i] = msg.reward_state(i);
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}

		data_set_ptr->add_data_info(data_flag, &db_row_info, sizeof(s_role_competition_info), 1);

		return true;
	}



	void cs2dp_save_role_competition_to_db_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(cs2dp_save_role_competition_to_db))
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message("cs2dp_save_role_competition_to_db_proc data_len = ",  data_len));
			return;
		}

		const cs2dp_save_role_competition_to_db* pdata = static_cast<const cs2dp_save_role_competition_to_db*>(data_ptr);
		if (NULL == pdata)
		{
			_RLOG_(MINFO, "cs2dp_save_role_competition_to_db_proc NULL == pdata ");
			return;
		}

		e_data_flag data_flag = e_data_flag_player_competition;
		if (pdata->competition_info.have_type == e_competition_type_element)
		{
			data_flag = e_data_flag_element_competition;
		}

		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(data_flag, &pdata->competition_info, sizeof(s_role_competition_info), 1);
			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, data_flag, pdata->save_type_ex);
			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << "call " << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << ".sp_save_role_competition(";
		sql << pdata->role_guid.server_64 << ",";
		sql << pdata->competition_info.have_type << ",";
		sql << "'(";
		sql << pdata->role_guid.server_64 << ",";
		sql << pdata->competition_info.have_type << ",";
		sql << pdata->competition_info.is_buy << ",";
		sql << pdata->competition_info.buy_manual_type << ",";
		sql << pdata->competition_info.point_add_percent << ",";
		sql << pdata->competition_info.season << ",";
		sql << pdata->competition_info.competition_begin_time << ",";
		sql << pdata->competition_info.competition_end_time << ",";
		sql << pdata->competition_info.level << ",";
		sql << pdata->competition_info.cur_exp << ",";
		sql << pdata->competition_info.need_reset << ",";
		sql << pdata->competition_info.assist_fight_guid.server_64 << ",";

		sql << pdata->competition_info.reward_state[0] << ",";
		sql << pdata->competition_info.reward_state[1] << ",";
		sql << pdata->competition_info.reward_state[2] << ",";
		sql << pdata->competition_info.reward_state[3] << ",";
		sql << pdata->competition_info.reward_state[4] << ",";
		sql << pdata->competition_info.reward_state[5] << ",";
		sql << pdata->competition_info.reward_state[6] << ")'";
		sql << ");";
		db_query_type query;
		query.logic_id = data_flag;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());

		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			//query.handler = boost::bind(&cs2dp_save_role_competition_to_db_end_proc, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_player_competition, pdata->save_type_ex);
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, data_flag, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_save_role_competition_to_db_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 unit_array_index, e_data_flag data_flag, int32 save_type_ex)
	{
		if (result.error == 0)
		{
			cs2dp_req_send_info_end(true, connindex, role_guid, unit_array_index, data_flag, save_type_ex);
		}
		else
		{
			//CONSOLE_INFO("cs2dp_req_save_person_information_cs_end sql =") << " error=" << result.error << " type=" << result.query_type << " save_flag=" << e_data_flag_player_information << " save_type_ex=" << save_type);
			cs2dp_req_send_info_end(false, connindex, role_guid, unit_array_index, data_flag, save_type_ex);
		}
	}

	static void cs2dp_load_role_dragontrip_send_lua(uint32 connindex, const dp2cs_get_role_dragontrip& msgData)
	{
		faith::dp2cs_proto_load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_dragontrip_db db_msg;
		for (int32 i = 0; i < e_dragontrip_max; i++ )
		{
			db_msg.add_data_ary(msgData.dragontrip_info.data_ary[i]);
		}
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&db_msg);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->get_google_data_len());
		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_get_role_dragontrip);


	}
	void cs2dp_load_role_dragontrip_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		dp2cs_get_role_dragontrip msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_dragontrip, &msgData.dragontrip_info, data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_get_role_dragontrip);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_dragontrip_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_role_dragontrip_load(") << up_role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_dragontrip;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_load_role_dragontrip_end_proc, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}

	}
	void cs2dp_load_role_dragontrip_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_dragontrip_info_db sql_data;
		int32 data_size = sizeof(sql_data);
		int32 table_len = result.query.data_select.row_count;
		if (result.error || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_dragontrip;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else {

			db_read_data(&result, &sql_data, data_size);

			dp2cs_get_role_dragontrip msgData;
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;

			msgData.dragontrip_info = sql_data.data_info;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_get_role_dragontrip);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_dragontrip_send_lua(connindex, msgData);
			}

			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_dragontrip, &msgData.dragontrip_info, sizeof(s_dragontrip_info), 1);
			}
		}
	}
	bool cs2dp_save_role_dragontrip_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		s_dragontrip_info row_info;

		faith::cs2dp_proto_role_dragontrip_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			row_info.data_ary[i] = msg.data_ary(i);
		}
	

		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}

		data_set_ptr->add_data_info(e_data_flag_dragontrip, &row_info, sizeof(s_dragontrip_info), 1);

		return true;


	}
	void cs2dp_save_role_dragontrip_to_db_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_role_dragontrip_to_db* pdata = static_cast<const cs2dp_save_role_dragontrip_to_db*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_dragontrip, &pdata->dragontrip_info, sizeof(pdata->dragontrip_info), 1);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_dragontrip, pdata->save_type_ex);

			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << _XTEXT(".sp_role_dragontrip_save('") << pdata->role_guid.server_64 << "','";

		sql << "(" << pdata->role_guid.server_64 << ",";


		for (int32 i = 0; i < e_dragontrip_max; ++i)
		{
			if (i != e_dragontrip_max - 1)
			{
				sql << pdata->dragontrip_info.data_ary[i] << ",";
			}
			else
			{
				sql << pdata->dragontrip_info.data_ary[i];
			}
		}


		sql << ")');";

		db_query_type query;
		query.logic_id = e_data_flag_dragontrip;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_dragontrip, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_load_role_skytreasure_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_role_skytreasure_load(") << up_role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.logic_id = e_data_flag_skytreasure;
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_load_role_skytreasure_end_proc, _1, connindex, role_guid, unit_array_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	static void cs2dp_load_role_skytreasure_send_lua(uint32 connindex, const dp2cs_get_role_skytreasure& msgData)
	{
		faith::dp2cs_proto::load_role_skytreasure msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);
		msg.set_row_count(msgData.data_num);
		msg.set_db_data((void *)&msgData, sizeof(dp2cs_get_role_skytreasure));
		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_get_role_skytreasure);
	}

	void cs2dp_load_role_skytreasure_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		struct s_skytreasure_db_row
		{
			guid_64 role_guid;
			s_skytreasure_info skytreasure_info;
		}sql_data[faith::max_skytreasure_num];

		int32 row_size = sizeof(s_skytreasure_db_row);
		int32 row_count = result.query.data_select.row_count;
		if (result.error || row_size != result.query.data_select.row_size || row_count > faith::max_skytreasure_num)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_skytreasure;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, row_size * row_count);

			dp2cs_get_role_skytreasure msgData;
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;
			msgData.data_num = 0;
			for (int32 i = 0; i < row_count; ++i)
			{
				msgData.skytreasure_data_list[i] = sql_data[i].skytreasure_info;
				++(msgData.data_num);
			}

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_get_role_skytreasure);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_skytreasure_send_lua(connindex, msgData);

			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_skytreasure, &msgData, sizeof(msgData), 1);
			}
		}
	}

	void cs2dp_save_role_skytreasure_to_db_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_role_skytreasure_to_db* pdata = static_cast<const cs2dp_save_role_skytreasure_to_db*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		if (data_len != sizeof(cs2dp_save_role_skytreasure_to_db))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << _XTEXT(".sp_role_skytreasure_save(") << pdata->role_guid.server_64 << ",";
		sql << pdata->skytreasure_info.skytreasure_layer_num << "," << pdata->skytreasure_info.skytreasure_pos << ",'";
		sql << "(" << pdata->role_guid.server_64 << "," << pdata->skytreasure_info.skytreasure_layer_num << "," << pdata->skytreasure_info.skytreasure_pos << "," << pdata->skytreasure_info.skytreasure_pos_in_array << ")";
		sql << "');";
		db_query_type query;
		query.logic_id = e_data_flag_skytreasure;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}


	void cs2dp_load_role_starark_send_lua(uint32 connindex, const dp2cs_get_role_starark& msgData)
	{
		faith::dp2cs_proto::load_role_starark msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);
		faith::db_proto::role_starark_db *db_data = msg.mutable_db_data();
		if (db_data == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < e_starark_max; i++)
		{
			db_data->add_data_ary(msgData.starark_info.data_ary[i]);
		}
		db_data->set_body(msgData.starark_info.body);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_get_role_starark);
	}
	void cs2dp_load_role_starark_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		dp2cs_get_role_starark msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_starark, &msgData.starark_info, data_num))
		{

				msgData.role_guid = role_guid;
				msgData.unit_array_index = unit_array_index;

				bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_get_role_starark);
				if (is_use == false)
				{
					dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			    }
				else
				{
					cs2dp_load_role_starark_send_lua(connindex, msgData);
				}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_role_starark_load(") << up_role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_starark;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_load_role_starark_end_proc, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}

	}
	void cs2dp_load_role_starark_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_starark_info_db sql_data;
		int32 data_size = sizeof(sql_data);
		int32 table_len = result.query.data_select.row_count;
		if (result.error || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_starark;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else {

			db_read_data(&result, &sql_data, data_size);

			dp2cs_get_role_starark msgData;
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;
			msgData.starark_info = sql_data.data_info;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_get_role_starark);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_starark_send_lua(connindex, msgData);
			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_starark, &msgData.starark_info, sizeof(s_starark_info), 1);
			}
		}
	}
	bool cs2dp_save_role_starark_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		s_starark_info starark_info;

		faith::db_proto::role_starark_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			starark_info.data_ary[i] = msg.data_ary(i);
		}
		int32 len = msg.body().size() > starark_max_db_len ? starark_max_db_len : msg.body().size();
		memcpy(starark_info.body, msg.body().c_str(), len);

		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}
		
		data_set_ptr->add_data_info(e_data_flag_starark, &starark_info, sizeof(s_starark_info), 1);

		return true;	 
	}
	void cs2dp_save_role_starark_to_db_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_role_starark_to_db* pdata = static_cast<const cs2dp_save_role_starark_to_db*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_starark, &pdata->starark_info, sizeof(pdata->starark_info), 1);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_starark, pdata->save_type_ex);

			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << _XTEXT(".sp_role_starark_save('") << pdata->role_guid.server_64 << "','";

		sql << "(" << pdata->role_guid.server_64 << ",";


		for (int32 i = 0; i < e_starark_max; ++i)
		{			
			sql << pdata->starark_info.data_ary[i] << ",";
		}

		sql << "\\'";
		sql << pdata->starark_info.body << "";
		sql << "\\'";

		sql << ")');";

		db_query_type query;
		query.logic_id = e_data_flag_starark;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_starark, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_save_push_sql_lua(uint32 connindex, int64 role_guid, int32 unit_array_index, int64 data_flag, const char *sql, int32 save_type_ex)
	{

		db_query_type query;
		query.logic_id = data_flag;
		query.role_guid = role_guid;
		query.sql_str.assign(sql);
		xstring str_sql(sql);
		if (save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, role_guid, unit_array_index, e_data_flag(data_flag), save_type_ex, str_sql);
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_save_push_sql_no_cache_lua(uint32 connindex, int64 role_guid, int32 unit_array_index, int64 data_flag, const char *sql, int32 save_type_ex)
	{

		db_query_type query;
		query.logic_id = data_flag;
		query.role_guid = role_guid;
		query.sql_str.assign(sql);
		xstring str_sql(sql);
		query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, role_guid, unit_array_index, e_data_flag(data_flag), save_type_ex, str_sql);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_save_push_sql_no_callback_lua(const char *sql,int64 data_flag, int64 role_guid)
	{
		db_query_type query;
		query.logic_id = data_flag;
		query.role_guid.server_64 = role_guid;
		query.sql_str.assign(sql);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}


	void cs2dp_req_load_char_data(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index, e_data_flag data_flag)
	{
		ZoneScoped;
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		packet_s2s* data = parse_msg::getInstance().get_server_msg();
		if (data_set_ptr && data_set_ptr->get_proto_info(data_flag, data->google_data, data->google_data_len))
		{
			data->wheader = e_msgindex_dp2cs_load_char_data;
			data->logic_guid = role_guid;
			data->logic_index = unit_array_index;
			data->data_flag = data_flag;
			dbproxy_service::getInstance().send_message(connindex, data, data->get_packet_len());
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(up_role_guid.server_64) << _XTEXT(".sp_role_data_load(") << role_guid.server_64 <<",'"<< data_manager::get_table_name(data_flag).c_str() << _XTEXT("');");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = data_flag;
			query.role_guid = role_guid;
			query.handler = [connindex, role_guid, unit_array_index, data_flag](db_result_type result) 
				{
					int32 table_len = result.query.data_select.row_count;
					int32 table_size = result.query.data_select.row_size;
					if (result.error || table_len > 1)
					{
						CONSOLE_ERROR("error role_guid:{} unit_array_index:{} result.error:{} table_len:{} table_size:{}", role_guid.server_64, unit_array_index, result.error, table_len, table_size);
						dp2cs_load_data_error error;
						error.role_guid = role_guid;
						error.array_index = unit_array_index;
						error.error_code = data_flag;
						error.error_result = e_error_code_db_error;
						dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
						return;
					}
					else
					{
						static s_db_data_array sql_data;
						sql_data.clear_data();
						db_read_data(&result, &sql_data, table_size);
						auto str_len = strlen(sql_data.data_array);

						xstring str_2 = Base64::Decode(sql_data.data_array, str_len);

						packet_s2s* data = parse_msg::getInstance().serialze_server(str_2.c_str(), str_len, e_msgindex_dp2cs_load_char_data, role_guid, unit_array_index);
						data->data_flag = data_flag;
						dbproxy_service::getInstance().send_message(connindex, data, data->get_packet_len());

						data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
						if (data_set_ptr)
						{
							data_set_ptr->add_data_info(data_flag, data->google_data, data->get_google_data_len());
						}
					}
				};
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_req_save_char_data(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		packet_s2s* save_data_ptr = (packet_s2s*)data_ptr;
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(save_data_ptr->logic_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(save_data_ptr->data_flag, save_data_ptr->google_data, save_data_ptr->google_data_len);

			if (save_data_ptr->logic_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, save_data_ptr->logic_guid, save_data_ptr->logic_index, e_data_flag(save_data_ptr->data_flag), save_data_ptr->logic_ex);
			}
		}
		xstring str_base64 = Base64::Encode(save_data_ptr->google_data, save_data_ptr->google_data_len);

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(save_data_ptr->logic_guid.server_64) << _XTEXT(".sp_role_data_save(") << save_data_ptr->logic_guid.server_64 << ",'"<< data_manager::get_table_name(save_data_ptr->data_flag).c_str() <<"','\\\'"<< str_base64.c_str() <<"\\\'');";

		db_query_type query;
		query.logic_id = save_data_ptr->data_flag;
		query.role_guid = save_data_ptr->logic_guid;
		query.sql_str.assign(sql.c_str(), sql.size());

		if (save_data_ptr->logic_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, save_data_ptr->logic_guid, save_data_ptr->logic_index, e_data_flag(save_data_ptr->data_flag), save_data_ptr->logic_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

}