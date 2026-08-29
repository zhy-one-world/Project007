#include <algorithm>
#include "legion_ws_mgr.h"
#include <legion_msg.hpp>
#include "Logic/time_def.hpp"
#include "invalid_word/invalid_ansi_word.h"
#include "../mail/event_ws_mgr.h"
#include "utility/cs_date.hpp"
#include "city_war_territory_mgr.h"
#include "Logic/activity_def.hpp"
#include "utility/init_unit.h"
#include "template/template_manager.h"
#include "server_log.hpp"
#include "../ranking/ranking_mgr_ws.h"
#include "world_server.hpp"
#include "../activity/cross_server_city_war_ws_mgr.h"
#include "cross_group/cross_transfer_logic.hpp"
#include "../legion/legion_ws_answer.h"

#include "globle_data.h"
#include "net.pb.h"

namespace faith
{
	legion_ws_mgr::legion_ws_mgr()
	{
		m_ordered_legion_list.reserve(max_legion_count);
		
		for (int32 i = 0; i < max_legion_count; ++i)
		{
			m_legion_name_searcher[i] = "";
		}

		m_reorder_legion_list_time = 0;
		m_legion_mgr_tick_five_min_last = 0;
		m_is_load_over = false;
		m_min_interval = 0;
		memset(m_last_update_rank_stamp, 0, sizeof(m_last_update_rank_stamp));
		m_is_need_refresh_overload_bid_city = 0;
		m_is_need_load = false;
	}

	void legion_ws_mgr::tick(const uint64& tick_time)
	{
		tm* now_tm = time_helper::get_localtime();
		if (nullptr == now_tm)
		{
			return;
		}

		for (auto ite = m_legion_map.begin(); ite != m_legion_map.end(); )
		{
			// tick�����ɾ��legion Ҫ�����ַ�ʽ��ֹ������ʧЧ
			auto temp_it = ite++;
			temp_it->second.tick(tick_time);
		}
		m_legion_city_war.tick(tick_time);

		uint64 legion_list_interval = tick_time - m_reorder_legion_list_time;
		if (legion_list_interval >= hour_tick_time)
		{
			m_reorder_legion_list_time = tick_time;
			update_ordered_legion_list();
		}
		
		int64 tick_five_min_cur = tick_time - m_legion_mgr_tick_five_min_last;
		if (tick_five_min_cur > minute_tick_time * 5)
		{
			m_legion_mgr_tick_five_min_last = tick_time;
			tick_5_min(tick_time);
		}

		static int32 legion_mgr_tick_min_last = 0;
		int32 tick_min_cur = tick_time / minute_tick_time;
		if (tick_min_cur > legion_mgr_tick_min_last)
		{
			legion_mgr_tick_min_last = tick_min_cur;
			tick_1_min(tick_time);
		}
		

		int32 legion_mgr_tick_day_cur = now_tm->tm_year * 1000 + now_tm->tm_yday;
		//int32 legion_mgr_tick_day_cur = tick_time / day_tick_time;
		static int32 tick_day_last = legion_mgr_tick_day_cur; //��֤��Ҫ�������ִ�п��� �����ͣ��ά���ڼ������ ��ִ��һ�� kero
		if (legion_mgr_tick_day_cur > tick_day_last)
		{
			tick_day_last = legion_mgr_tick_day_cur;
			tick_1_day(tick_time);
		}
	}

	void legion_ws_mgr::tick_1_min(const uint64& tick_time)
	{
		for (auto ite = m_legion_map.begin(); ite != m_legion_map.end(); )
		{
			// tick�����ɾ��legion Ҫ�����ַ�ʽ��ֹ������ʧЧ
			auto temp_it = ite++;
			temp_it->second.tick_1_min(tick_time);
			temp_it->second.save_legion_applicant_to_db();
		}
		m_legion_city_war.tick_1_min(tick_time);
		tick_1_min_legion_activity(tick_time);

		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war) && true == is_need_refresh_overload_city_bid())
		{
			city_war_territory_mgr::get_instance().set_need_del_overload_city_bid_info(false);
			city_war_territory_mgr::get_instance().get_overload_city_war_bid_legion();
		}
		if (true == city_war_territory_mgr::get_instance().get_need_del_overload_city_bid_info())
		{
			city_war_territory_mgr::get_instance().del_all_overload_city_bid_info();
		}
	}

	void legion_ws_mgr::tick_5_min(const uint64& tick_time)
	{
		for (auto ite = m_legion_map.begin(); ite != m_legion_map.end(); )
		{
			// tick�����ɾ��legion Ҫ�����ַ�ʽ��ֹ������ʧЧ
			auto temp_it = ite++;
			temp_it->second.tick_5_min(tick_time);
		}
	}
	
	void legion_ws_mgr::tick_1_day(const uint64& tick_time)
	{
		
	}

	void legion_ws_mgr::refresh_day_for_zero()
	{
		for (auto ite = m_legion_map.begin(); ite != m_legion_map.end(); )
		{
			// tick�����ɾ��legion Ҫ�����ַ�ʽ��ֹ������ʧЧ
			auto temp_it = ite++;
			temp_it->second.refresh_day_for_zero();
		}
		city_war_territory_mgr::get_instance().cacul_occupy_territory_rank();
	}

	void legion_ws_mgr::clear_all_legion_city_war_member()
	{
		for (auto ite = m_legion_map.begin(); ite != m_legion_map.end(); )
		{
			auto temp_it = ite++;
			temp_it->second.clear_all_city_war_member();
		}
		//ws2dp_delete_legion_city_war_member_info c_member_info_msg;
		//c_member_info_msg.del_type = e_legion_use_contribution_type_city_war;
		//ws_client::getInstance().send_to_dp(&c_member_info_msg, sizeof(c_member_info_msg));

		script_mgr::get_instance().call_func("ws2dp_send", "send_delete_legion_city_war_member_info_lua", 0, false, "%d", (int32)e_legion_use_contribution_type_city_war);

	}

	void legion_ws_mgr::refresh_week_for_zero()
	{
	}

	void legion_ws_mgr::refresh_week_for_twenty_three()
	{
		int32 active_legion_num = 0;

		//�������Ÿ�����ȡ��¼
		//ws2dp_delete_legion_city_war_member_info c_member_info_msg;
		//c_member_info_msg.del_type = e_legion_use_contribution_type_legion_welfare;
		//ws_client::getInstance().send_to_dp(&c_member_info_msg, sizeof(c_member_info_msg));

		script_mgr::get_instance().call_func("ws2dp_send", "send_delete_legion_city_war_member_info_lua", 0, false, "%d", (int32)e_legion_use_contribution_type_legion_welfare);

		for (auto ite = m_legion_map.begin(); ite != m_legion_map.end(); )
		{
			// tick�����ɾ��legion Ҫ�����ַ�ʽ��ֹ������ʧЧ
			auto  temp_it = ite++;
			//�������Ÿ�����ȡ����
			temp_it->second.clear_all_city_war_member_by_type(e_legion_use_contribution_type_legion_welfare);
			//���ž��Ÿ�������
			temp_it->second.legion_welfare_account();
			//ͳ�ƻ�Ծ��������
			if (temp_it->second.week_is_have_player_active())
			{
				active_legion_num++;
			}
			temp_it->second.refresh_week_for_twenty_three();
		}

		//ÿ��23:00��¼���ܻ�Ծ��������
		set_log_var(log_head);
		set_log_common_head_part3(log_head, "null", world_server::getInstance().get_server_id(), utility::get_tick_count());

		server_log::serverLegionActivrNumLog(log_head, active_legion_num);
	}

	void legion_ws_mgr::refresh_day_for_five()
	{
		for (auto ite = m_legion_map.begin(); ite != m_legion_map.end(); )
		{
			// tick�����ɾ��legion Ҫ�����ַ�ʽ��ֹ������ʧЧ
			auto temp_it = ite++;
			temp_it->second.refresh_day_for_five();
		}
		city_war_territory_mgr::get_instance().set_need_del_overload_city_bid_info(true);

	}

	void legion_ws_mgr::refresh_all_legion_power()
	{
		legion_ws_map& legion_map = get_legion_map();
		legion_ws_map_it ite;
		for (ite = legion_map.begin(); ite != legion_map.end(); ++ite)
		{
			legion_ws& legion_ws_ref = ite->second;
			legion_ws_ref.recalcu_legion_fighting_power();
		}
	}

	int32 legion_ws_mgr::is_can_create_legion()
	{
		if (legion_ws_answer::get_instance().is_in_legion_answer_all_time())
		{
			return e_legion_error_is_in_legion_answer_time;
		}
		return 0;
	}

	void legion_ws_mgr::delete_legion_from_db(guid_64 legion_guid)
	{
		//ws2dp_delete_legion_info delete_legion_info_msg;
		//delete_legion_info_msg.legion_guid = legion_guid;
		//ws_client::getInstance().send_to_dp(&delete_legion_info_msg, sizeof(delete_legion_info_msg));
		script_mgr::get_instance().call_func("ws2dp_send", "send_delete_legion_info_lua", 0, false, "%l", legion_guid.server_64);	
	}

	// ������:
	//  1 ��ȷִ��
	// -1 ������Ϣ����
	// -2 �������ѱ�ʹ��
	// -3 Ҫ����ľ����Ѿ��ھ��Ź�������
	int32 legion_ws_mgr::add_legion(legion_ws& new_legion)
	{
		const guid_64& legion_guid = new_legion.get_legion_guid();
		if (legion_guid.is_valid() == false)
		{
			new_legion.clear_member();
			return -1;
		}

		// �����������Ƿ��Ѿ���ʹ��
		std::string temp_legion_name = new_legion.get_legion_name();
		if (exist_name_in_legion_name_searcher(temp_legion_name) == true)
		{
			new_legion.clear_member();
			return -2;
		}

		if (m_legion_map.find(legion_guid.server_64) != m_legion_map.end())
		{
			new_legion.clear_member();
			return -3;
		}

		m_legion_map.insert({ legion_guid.server_64, new_legion });

		// ���������Ʒ���������Ʋ�ѯ�б���,�Է�ֹ�����������ŵ����󴴽���ͬ���Ƶľ���
		add_name_in_legion_name_searcher(temp_legion_name);

		// ����Ų�ѯ�б��������¾��ŵ���Ϣ
		add_into_ordered_legion_list(m_ordered_legion_list, new_legion.get_legion_gs_value(), legion_guid);
		if (new_legion.get_legion_info(ELegionInfo_auto_accept_new_member) > 0)
		{
			add_into_auto_accept_new_member_ordered_legion_list(new_legion.get_legion_gs_value(), legion_guid);
		}

		return 1;
	}

	void legion_ws_mgr::del_rank_with_del_legion(guid_64 legion_guid)
	{
		ranking_list* legion_rank = legion_ws_mgr::get_instance().get_legion_ranking_by_ranking_type(e_RankingIndex_legion_core);
		if (nullptr == legion_rank)
		{
			return;
		}
		legion_rank_list& rank_list_ref = *legion_rank;
		auto ite = rank_list_ref.begin();
		for (ite; ite != rank_list_ref.end(); ite++)
		{
			if (ite->role_guid == legion_guid)
			{
				rank_list_ref.erase(ite);
				break;
			}
		}
	}

	void legion_ws_mgr::del_legion(guid_64 legion_guid, bool need_del_db, guid_64 del_role_guid, const login_fixed_data& third_info, const int32& login_type)
	{
		legion_ws_map::iterator ite = m_legion_map.find(legion_guid.server_64);
		if (ite == m_legion_map.end())
		{
			return;
		}
		cross_server_city_war_ws_mgr::get_instance().refresh_rank_with_del_legion(legion_guid);
		cross_server_city_war_ws_mgr::get_instance().send_to_gate_change_legion_msg(legion_guid, e_change_cross_city_type_del_legion);
		legion_ws& legion_ws_ref = ite->second;
		// ��ɢ�ľ��ŵ����ƻ��ÿ��Ա���������ʹ��
		std::string temp_legion_name = legion_ws_ref.get_legion_name();
		clear_name_in_legion_name_searcher(temp_legion_name);
		
		legion_ws_ref.del_legion_station_map();
		// �����ŴӾ��Ų�ѯ�б���ժ��
		del_from_ordered_legion_list(m_ordered_legion_list, legion_guid);
		del_from_ordered_legion_list(m_ordered_auto_accept_new_member_legion_list, legion_guid);
		del_rank_with_del_legion(legion_guid);
		if (need_del_db)
		{
			if (m_legion_city_war.get_overlord_legion() == legion_ws_ref.get_legion_guid())
			{
				m_legion_city_war.del_overlord_legion(true);
			}
			
		//	legion_ws_ref.get_boss_record_set().clear_boss_record_to_db();
			legion_ws_ref.clear_legion_city_war();
			legion_ws_ref.get_event_logger().clear_legion_event_in_db();
			legion_ws_ref.get_bonus_info_ins().clear_legion_bonus_info_in_db();
			legion_ws_ref.get_warehouse().clear_legion_warehouse_info_in_db(legion_guid);
			// PS: �뱣����֪ͨ��Ա��ɢ���ŵ���Ϣ����֮�����������ŵ�����,��Ϊ������Ϣ�Ĺ�����Ҫ�������ŵĳ�Ա�б�
			// �ѽ�ɢ���ŵ���Ϣ���͸�ÿһ����Ա
			legion_proto_dissolve_legion_end dissolve_legion_end_to_client_msg;
			legion_ws_ref.send_message_to_all_member(&dissolve_legion_end_to_client_msg, e_msgindex_s2c_dissolve_legion_end);
			
			delete_legion_from_db(legion_guid);
		}
		legion_ws_ref.clear_member();
		m_legion_map.erase(ite);
	}

	legion_ws* legion_ws_mgr::get_legion(guid_64 legion_guid)
	{
		auto ite = m_legion_map.find(legion_guid.server_64);
		if (ite != m_legion_map.end())
		{
			return &(ite->second);
		}

		return nullptr;
	}

	void legion_ws_mgr::create_legion_begin(guid_64 player_guid, const xchar* l_name, int32 l_name_len, int32 l_level, bool auto_accept)
	{
		client_session* session = client_session_mgr::getInstance().get_session(player_guid);
		if (nullptr == session)
		{
			return;
		}

		// �������Ѿ���ĳ�������оͲ��ܴ�������
		if (get_unit_legion(player_guid) != nullptr)
		{
			return;
		}
		// ���ŵ����ֱ�����legion_name_size_min_limit��legion_name_size_max_limit�ĳ���
		if (l_name_len < legion_name_size_min_limit || l_name_len > max_name_size)
		{
			return;
		}
		// ����Ƿ����
		if (invalid_ansi_word::is_valid_ansi_str(l_name) == false)
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, e_legion_error_error_legion_name_illegal);
			return;
		}

		//�ж��Ƿ��ھ��Żʱ���� �ڻʱ���ڲ������������� 
		/*int32 error_num = is_can_create_legion();
		if (error_num > 0)
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, (e_legion_error)error_num);
			return;
		}*/

		// ����Ƿ���ڷǷ��ַ�
		if (invalid_ansi_word::include_invalid_ansi_str(l_name))
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, e_legion_error_error_legion_name_illegal);
			return;
		}
		// Ŀǰֻ�ܴ���1����2���ľ���
		if (l_level < 1 || l_level > 2)
		{
			return;
		}

		// �����������Ƿ��Ѿ��ﵽ����
		if (get_legion_num() >= max_legion_count)
		{
			send_legion_error_to_session(session, e_legion_error_full_legion_num);
			return;
		}

		int32 legion_id = first_legion_template_id + l_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_id);
		if (nullptr == legion_template_ptr)
		{
			return;
		}
		if (session->m_role_info.data_ary[e_role_info_exp_level] < legion_template_ptr->CreateNeedLevel)
		{
			return;
		}
		if (session->m_role_info.data_ary[e_role_info_vip_level] < legion_template_ptr->CreateNeedVipLevel)
		{
			session->send_notice("90096662");
			return;
		}
		legion_ws new_legion_ws;
		new_legion_ws.set_legion_name(l_name, l_name_len);
		new_legion_ws.set_legion_info(ELegionInfo_auto_accept_new_member, auto_accept, false);

		new_legion_ws.set_legion_info(ELegionInfo_construction_level_main, l_level, false);
		new_legion_ws.set_legion_info(ELegionInfo_construction_level_1, 1, false);//����ʱ��������Ĭ��Ϊ1
		new_legion_ws.set_legion_info(ELegionInfo_construction_level_2, 1, false);
		new_legion_ws.set_legion_info(ELegionInfo_construction_level_3, 1, false);

		s_legion_member_info chief_info;
		generate_player_legion_info(session, chief_info);
		chief_info.data_ary[e_legion_member_info_job_title] = e_legion_job_title_chief;

		int32 ret = add_legion(new_legion_ws);
		if (1 != ret)
		{
			switch (ret)
			{
			case -2:
				send_legion_error_to_session(session, e_legion_error_repeat_legion_name);
				break;
			default:
				break;
			}
			return;
		}

		// ����ʼ�������ŵ���Ϣ���͵����������ڵ�cs��
		ws2cs_create_legion create_legion_end_to_cs_msg;
		create_legion_end_to_cs_msg.legion_guid = new_legion_ws.get_legion_guid();
		memcpy(create_legion_end_to_cs_msg.legion_name, l_name, l_name_len);
		create_legion_end_to_cs_msg.legion_level = l_level;
		create_legion_end_to_cs_msg.auto_accept_new_member = auto_accept;
		create_legion_end_to_cs_msg.chief_info = chief_info;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_create_legion);
		if (is_use == false)
		{
			session->send_to_cs(&create_legion_end_to_cs_msg, sizeof(create_legion_end_to_cs_msg));
		}
		else
		{
			faith::ws2cs_proto::create_legion pro_msg;
			create_legion_end_to_cs_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_create_legion);
		}
	}

	void legion_ws_mgr::create_legion_end(s_legion_member_info&	chief_info, guid_64 legion_guid, e_legion_error error_code)
	{
		legion_ws* legion_ws_ptr = get_legion(legion_guid);

		do
		{
			if (e_legion_error_none != error_code)
			{
				break;
			}
			if (nullptr == legion_ws_ptr)
			{
				break;
			}
			//�޸�Ĭ�Ͼ��Ź���
			const std::string& announcement_content = template_manager::get_instance().get_str_by_string_template_id(legion_init_announcement_str_id);
			if (announcement_content.length() <= max_legion_announcement_size)
			{
				legion_ws_ptr->set_announcement(announcement_content.c_str(), announcement_content.size(), false, false);
				legion_ws_ptr->send_legion_announcement();
			}

			if (false == legion_ws_ptr->add_member(chief_info, false, false))
			{
				break;
			}

			client_session* chief_session = client_session_mgr::getInstance().get_session(chief_info.role_guid);
			if (nullptr == chief_session)
			{
				break;
			}

			legion_proto_create_legion_end create_legion_end_msg;
			legion_proto_legion_info* legion_info_msg = create_legion_end_msg.mutable_legion_info_data();
			if (!legion_ws_ptr->fill_legion_info_msg(*legion_info_msg))
			{
				break;
			}
			//���
			legion_ws_ptr->save_legion_info_to_db();

			chief_session->send_to_client(&create_legion_end_msg, e_msgindex_s2c_create_legion_end);
			
			//�������Ź���				
			int32 notice_id = GAMECONFIG->CreateLegionNotice;
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(legion_ws_ptr->get_member(legion_ws_ptr->get_chief_guid())->role_name);
			vec_notice_str.push_back(legion_ws_ptr->get_legion_name());
			std::string notice_str = init_unit::implode(vec_notice_str);
			int32 create_time = time_helper::get_cur_time_new().second;
			event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, legion_ws_ptr->get_chief_guid(), notice_str);

			// ��������Ƶ����ļ
			legion_ws_ptr->send_legion_recruit(chief_info.role_guid, chief_info.role_name);
			legion_ws_ptr->recalcu_legion_fighting_power();
			update_ordered_legion_list();

			xchar title_id[8];
			memset(title_id, 0, 8);
			printf(title_id, "%d", e_legion_job_title_chief);
			legion_ws_ptr->create_legion_station_map();
			return;

		} while (false);

		del_legion(legion_guid, false);
	}

	bool legion_ws_mgr::join_to_legion(client_session* player_session, guid_64 legion_guid, bool need_send_message_to_client)
	{
		if (nullptr == player_session || false == legion_guid.is_valid())
		{
			return false;
		}
		legion_ws* legion_ws_ptr = get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return false;
		}
		const LegionConstructionsTemplate* legion_temp_ptr = legion_ws_ptr->get_legion_cfg();
		if (legion_temp_ptr == nullptr)
		{
			return false;
		}
		int32 need_level = legion_temp_ptr->CreateNeedLevel;
		if (player_session->get_role_info_data(e_role_info_exp_level) < need_level)
		{
			return false;
		}
		if (false == check_can_join_legion(*legion_ws_ptr, player_session->get_role_guid()))
		{
			return false;
		}
		// // ����Ҫ����ľ����Ƿ��Զ������³�Ա���в�ͬ�Ĳ���
		if (legion_ws_ptr->is_auto_accept_new_member())
		{
			s_legion_member_info new_member_info;
			generate_player_legion_info(player_session, new_member_info);
			if (false == legion_ws_ptr->add_member(new_member_info, false, true, player_session->m_login_third_data, player_session->m_login_type))
			{
				return false;
			}
			legion_ws_applicant_list& applicant_list_ref = legion_ws_ptr->get_applicant_list();
			applicant_list_ref.del_applicant_with_join_legion(player_session->get_role_guid());
			legion_proto_apply_to_join_legion_end apply_join_legion_end_msg;
			apply_join_legion_end_msg.set_apply_stage(e_legion_apply_stage_auto_accepted);
			player_session->send_to_client(&apply_join_legion_end_msg, e_msgindex_s2c_apply_to_join_legion_end);
		}
		else
		{
			legion_ws_applicant new_applicant(player_session->get_role_guid(), legion_guid);
			legion_ws_applicant_list& applicant_list_ref = legion_ws_ptr->get_applicant_list();
			legion_ws_applicant_list& need_save_db_list = legion_ws_ptr->get_need_save_db_applicant_list();
			new_applicant.init_by_applicant_guid(player_session->get_role_guid(), legion_guid);
			e_add_applicant_ret ret = applicant_list_ref.add_applicant(new_applicant);
			switch (ret)
			{
			case e_add_applicant_ok:
			{
				need_save_db_list.add_applicant(new_applicant);
				//new_applicant.save_to_db();
				if (true == need_send_message_to_client)
				{
					legion_proto_apply_to_join_legion_end apply_end_msg;
					apply_end_msg.set_apply_stage(e_legion_apply_stage_send_success);
					player_session->send_to_client(&apply_end_msg, e_msgindex_s2c_apply_to_join_legion_end);
				}
				legion_ws_ptr->send_applicant_info_list_to_have_job_member();
			}
			break;

			case e_add_applicant_legion_full: // ��������ŵ������б�����
				if (true == need_send_message_to_client)
				{
					send_legion_error_to_session(player_session, e_legion_error_applicant_list_full);
				}
				break;

			case e_add_applicant_repeat: // �Ѿ���������ŷ��͹�������
				if (true == need_send_message_to_client)
				{
					send_legion_error_to_session(player_session, e_legion_error_applicant_already_exist);
					player_session->send_notice("90090490");
				}
				break;
			default:
				break;
			}
		}
		return true;
	}

	void legion_ws_mgr::convenient_join_legion(client_session* session)
	{
		if (nullptr == session || false == session->is_vaild())
		{
			return;
		}

		// ���ȱ����Զ����˵ľ���
		int32 len = m_ordered_auto_accept_new_member_legion_list.size();
		if (len > 0)
		{
			for (const fighting_power_legion_pair& entry : m_ordered_auto_accept_new_member_legion_list)
			{
				bool join_result = join_to_legion(session, entry.legion_guid, false);
				if (true == join_result)
				{
					return;
				}
			}
		}

		// �������з��Զ����˵ľ���
		len = m_ordered_legion_list.size();
		if (len > 0)
		{
			for (const fighting_power_legion_pair& entry : m_ordered_legion_list)
			{
				join_to_legion(session, entry.legion_guid, false);
			}

			legion_proto_convenient_join_legion_end convenient_join_legion_end_msg;
			session->send_to_client(&convenient_join_legion_end_msg, e_msgindex_s2c_convenient_join_legion_end);
		}
	}

	bool legion_ws_mgr::check_can_join_legion(const legion_ws& legion_ref, guid_64 player_guid, client_session* op_member_session)
	{
		//client_session* join_player_session = client_session_mgr::getInstance().get_session(player_guid);
		
		// �����Ƿ��Ѿ�����
		if (legion_ref.is_legion_full())
		{
			return false;
		}

		// �������Ѿ���ĳ��������
		if (get_unit_legion(player_guid) != nullptr)
		{
			send_legion_error_to_session(op_member_session, e_legion_error_player_joined_other_legion);
			return false;
		}

		return true;
	}

	bool legion_ws_mgr::check_can_leave_legion(const legion_ws& legion_ref, guid_64 player_guid, client_session* op_member_session)
	{
		if (player_guid.is_valid() == false)
		{
			return false;
		}

		client_session* noti_session = nullptr;

		if (op_member_session) //��Ա����
		{
			// �����Լ����Լ�
			if (op_member_session->get_role_guid() == player_guid)
			{
				return false;
			}
			
			e_legion_job_title op_member_job_title = legion_ref.get_job_title(op_member_session->get_role_guid());
			// û���κ�ְλ�Ľ�ɫ��������
			if (op_member_job_title <= e_legion_job_title_none)
			{
				return false;
			}
			
			e_legion_job_title leaver_job_title = legion_ref.get_job_title(player_guid);
			if (leaver_job_title >= op_member_job_title)
			{
				return false;
			}

			noti_session = op_member_session;
		}
		else
		{
			noti_session = client_session_mgr::getInstance().get_session(player_guid);
		}
		
		if (legion_ref.get_member_count() <= 1)
		{
			return false;
		}

		//if (legion_ref.get_member(player_guid) == nullptr)
		//{
		//	return;
		//}

		// ����ǳ�ս��ս����,��ô�ڳ�ս�ڼ䲻���˳�����
		if (get_legion_city_war().is_during_city_war(legion_ref.get_legion_guid()) == true)
		{
			send_legion_error_to_session(noti_session, e_legion_error_can_not_leave_legion_during_city_war);
			return false;
		}

		return true;
	}

	void legion_ws_mgr::on_player_join(guid_64 legion_guid, guid_64 player_guid, const xstring& legion_name)
	{
		ranking_mgr_ws::change_ranking_player_legion_info(player_guid, legion_guid, legion_name);

		client_session* member_session = client_session_mgr::getInstance().get_session(player_guid);
		if (member_session)
		{
			//�������ľ����ǣ���սʤ���ľ��������ӳƺ�
			if (legion_guid == get_legion_city_war().get_overlord_legion())
			{
				get_legion_city_war().send_one_add_city_war_winner_title(member_session);
			}
			
		}
	}

	void legion_ws_mgr::on_player_leave(guid_64 legion_guid, guid_64 player_guid, const xstring& legion_name)
	{
		guid_64 empty_legion_guid;
		ranking_mgr_ws::change_ranking_player_legion_info(player_guid, empty_legion_guid, "");

		//����ǳ�սʤ��һ��ɾ����Ա��ɾ����սʤ����title
		if (legion_guid == get_legion_city_war().get_overlord_legion())
		{
			//����ǳ���
			if (get_legion_city_war().get_city_master_guid() == player_guid)
			{
				//get_legion_city_war().send_del_city_master_stuff(player_guid);
				//get_legion_city_war().ws2dp_del_overlord_legion_stuff_proc(e_del_overlord_stuff_event_only_master, player_guid);
			}
			get_legion_city_war().send_one_del_city_war_winner_title(player_guid);
			get_legion_city_war().ws2dp_del_overlord_legion_stuff_proc(e_del_overlord_stuff_event_only_one_member, player_guid);
		}
		
		client_session* legion_member_session = client_session_mgr::getInstance().get_session(player_guid);
		if (nullptr != legion_member_session)
		{
			ws2cs_clear_player_legion_info clear_player_legion_info_msg;
			clear_player_legion_info_msg.role_guid = player_guid;
			bool is_use = proto_by_lua(e_msg_index_ws2cs_clear_player_legion_info);
			if (is_use == false)
			{
				legion_member_session->send_to_cs(&clear_player_legion_info_msg, sizeof(clear_player_legion_info_msg));
			}
			else
			{
				faith::ws2cs_proto::clear_player_legion_info pro_msg;
				pro_msg.set_role_guid(player_guid.server_64);
				legion_member_session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_clear_player_legion_info);
			}
		}
	}

	void legion_ws_mgr::generate_player_legion_info(client_session* session, s_legion_member_info& member_info, legion_ws_applicant* applicant_info)
	{
		if (nullptr == session)
		{
			if (nullptr == applicant_info)
			{
				return;
			}
			applicant_info->generate_player_legion_info(member_info);
			member_info.data_ary[e_legion_member_info_last_logout_stamp] = time_helper::get_cur_time_new().second;
		}
		else
		{
			member_info.role_guid = session->get_role_guid();
			member_info.set_role_name(session->m_role_info.role_name);
			member_info.gs_value = session->get_role_gs_value();
			member_info.data_ary[e_legion_member_info_level] = session->m_role_info.data_ary[e_role_info_exp_level];
			member_info.data_ary[e_legion_member_info_player_template_id] = session->m_role_info.data_ary[e_role_info_template_id];
			member_info.data_ary[e_legion_member_info_is_online] = 1;
			member_info.data_ary[e_legion_member_info_vip_level] = session->m_role_info.data_ary[e_role_info_vip_level];
			member_info.data_ary[e_legion_member_info_is_show_vip] = session->m_role_info.data_ary[e_role_info_is_show_vip];
		}
		member_info.data_ary[e_legion_member_info_donate_contribution] = 0;
		member_info.data_ary[e_legion_member_info_add_fuel_times] = 0;
		member_info.data_ary[e_legion_member_info_job_title] = e_legion_job_title_none;
		member_info.data_ary[e_legion_member_info_last_get_city_award] = 0xFFFFFFFF;
		member_info.data_ary[e_legion_member_info_agora_id] = 0;
		member_info.data_ary[e_legion_member_info_join_time_stamp] = time_helper::get_cur_time_new().second;
		member_info.data_ary[e_legion_member_info_get_master_city_award] = 0;
	}

	void legion_ws_mgr::send_legion_error_to_session(client_session* session, e_legion_error err_type)
	{
		if (nullptr == session)
		{
			return;
		}
	
		legion_proto_legion_error legion_error_msg;
		legion_error_msg.set_error_type((int32)err_type);
		session->send_to_client(&legion_error_msg, e_msgindex_s2c_legion_error);
	}

	bool legion_ws_mgr::is_two_player_in_the_same_legion(guid_64 player1_guid, guid_64 player2_guid)
	{
		legion_ws* player1_legion_ptr = get_unit_legion(player1_guid);
		if (nullptr == player1_legion_ptr)
		{
			return false;
		}
		legion_ws* player2_legion_ptr = get_unit_legion(player2_guid);
		if (nullptr == player2_legion_ptr)
		{
			return false;
		}

		guid_64 legion1_guid = player1_legion_ptr->get_legion_guid();
		guid_64 legion2_guid = player2_legion_ptr->get_legion_guid();

		if (legion1_guid.is_valid() == false || legion2_guid.is_valid() == false)
		{
			return false;
		}

		if (legion1_guid == legion2_guid)
		{
			return true;
		}

		return false;
	}

	void legion_ws_mgr::on_player_reconnect(client_session* player_session)
	{
		if (nullptr == player_session)
		{
			return;
		}
		if (false == player_session->is_self_server())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(player_session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			legion_proto_leave_legion_end leave_legion_end_to_client_msg;
			player_session->send_to_client(&leave_legion_end_to_client_msg, e_msgindex_s2c_leave_legion_end);
		}
		else
		{
			legion_ws_ptr->on_legion_member_reconnect(player_session);
		}
	}

	void legion_ws_mgr::set_unit_legion(const guid_64& unit_guid, const guid_64& legion_guid)
	{
		m_role_legion_pair_map.insert({ unit_guid.server_64, legion_guid });
	}

	legion_ws*	legion_ws_mgr::get_unit_legion(const guid_64& unit_guid)
	{
		auto ite = m_role_legion_pair_map.find(unit_guid.server_64);
		if (ite == m_role_legion_pair_map.end())
		{
			return nullptr;
		}

		return get_legion(ite->second);
	}

	void legion_ws_mgr::clear_unit_legion(const guid_64& unit_guid)
	{
		m_role_legion_pair_map.erase(unit_guid.server_64);
	}

	void legion_ws_mgr::add_name_in_legion_name_searcher(std::string legion_name)
	{
		for (int32 i = 0; i < max_legion_count; ++i)
		{
			if ("" == m_legion_name_searcher[i])
			{
				m_legion_name_searcher[i] = legion_name;
				return;
			}
		}
	}

	bool legion_ws_mgr::exist_name_in_legion_name_searcher(std::string legion_name)
	{
		for (int32 i = 0; i < max_legion_count; ++i)
		{
			if (legion_name == m_legion_name_searcher[i])
			{
				return true;
			}
		}
		return false;
	}

	void legion_ws_mgr::clear_name_in_legion_name_searcher(std::string legion_name)
	{
		for (int32 i = 0; i < max_legion_count; ++i)
		{
			if (legion_name == m_legion_name_searcher[i])
			{
				m_legion_name_searcher[i] = "";
				return;
			}
		}
	}

	bool fighting_power_legion_pair_comp(const fighting_power_legion_pair& pair_x, const fighting_power_legion_pair& pair_y)
	{
		return pair_x.fighting_power > pair_y.fighting_power;
	}

	void legion_ws_mgr::update_ordered_legion_list()
	{
		m_ordered_legion_list.clear();
		m_ordered_auto_accept_new_member_legion_list.clear();

		int32 ordered_list_size = m_legion_map.size();
		legion_ws_map::iterator ite;
		for (ite = m_legion_map.begin(); ite != m_legion_map.end(); ++ite)
		{
			m_ordered_legion_list.push_back({
				ite->second.get_legion_gs_value(),
				ite->second.get_legion_guid()
			});

			if (ite->second.get_legion_info(ELegionInfo_auto_accept_new_member) > 0)
			{
				m_ordered_auto_accept_new_member_legion_list.push_back({
					ite->second.get_legion_gs_value(),
					ite->second.get_legion_guid()
				});
			}
		}
		
		std::sort(m_ordered_legion_list.begin(), m_ordered_legion_list.end(), fighting_power_legion_pair_comp);
		std::sort(m_ordered_auto_accept_new_member_legion_list.begin(), m_ordered_auto_accept_new_member_legion_list.end(), fighting_power_legion_pair_comp);

		int32 rank_num = 1;
		for (ite = m_legion_map.begin(); ite != m_legion_map.end(); ++ite)
		{
			ite->second.set_legion_rank(rank_num);
			++rank_num;
		}
	}

	void legion_ws_mgr::add_into_ordered_legion_list(std::vector<fighting_power_legion_pair>& legion_list, int64 legion_fighting_power, guid_64 legion_guid)
	{
		if (legion_guid.is_valid() == false)
		{
			return;
		}
		legion_ws* legion_ws_ptr = get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		fighting_power_legion_pair new_pair;
		new_pair.fighting_power = legion_fighting_power;
		new_pair.legion_guid = legion_guid;

		int32 len = legion_list.size();

		// �մ����ľ���ֻ��һ����Ա,����ս�����Ƚϵ�,��Ϊ�б����ǰ�ս�����Ӵ�С��
		// ˳������,�������һ����ʼ�ȽϿ��Ը������ɲ������
		for (int32 i = len - 1; i >= 0; --i)
		{
			if (legion_fighting_power < legion_list[i].fighting_power)
			{
				legion_list.insert(legion_list.begin() + i + 1, new_pair);
				legion_ws_ptr->set_legion_rank(i + 1 + 1);
				return;
			}
		}

		legion_list.insert(legion_list.begin(), new_pair);
		legion_ws_ptr->set_legion_rank(1);
	}

	int32 legion_ws_mgr::get_legion_rank_in_list(guid_64 legion_guid, std::vector<fighting_power_legion_pair>* legion_list_ptr)
	{
		std::vector<fighting_power_legion_pair> legion_list;
		if (nullptr == legion_list_ptr)
		{
			legion_list = m_ordered_legion_list;
		}
		else
		{
			legion_list = *legion_list_ptr;
		}
		
		int count = legion_list.size();
		for (int i = 0; i < count; i++)
		{
			 if (legion_guid == legion_list[i].legion_guid)
			 {
				 return i + 1;
			 }
		}
		return -1;
	}

	void legion_ws_mgr::del_from_ordered_legion_list(std::vector<fighting_power_legion_pair>& legion_list, guid_64 legion_guid)
	{
		int32 len = legion_list.size();
		for (int32 i = 0; i < len; ++i)
		{
			if (legion_guid == legion_list[i].legion_guid)
			{
				legion_list.erase(legion_list.begin() + i);
				return;
			}
		}
	}

	bool legion_ws_mgr::get_legion_list(int32 start_index, int64 base_fighting_power, legion_proto_get_legion_list_end& get_legion_list_msg, bool only_accept_new_member_legion)
	{
		get_legion_list_msg.set_legion_list_len(0);

		// �����Ƿ�ֻ����Զ����˵�������ѡ����Ӧ�Ĳ�ѯ�б�
		std::vector<fighting_power_legion_pair>* legion_list_ptr = nullptr;
		if (only_accept_new_member_legion)
		{
			legion_list_ptr = &m_ordered_auto_accept_new_member_legion_list;
		}
		else
		{
			legion_list_ptr = &m_ordered_legion_list;
		}

		int32 legion_list_total_len = legion_list_ptr->size();
		int32 index = 0;
		int32 legion_list_entry_count = 0;
		for (int32 i = 0; i < once_legion_list_get_num; ++i)
		{
			index = start_index + i;
			if (index >= legion_list_total_len)
			{
				break;
			}
			fighting_power_legion_pair& fightpower_legion_pair = (*legion_list_ptr)[index];
			if (base_fighting_power > 0 && fightpower_legion_pair.fighting_power > base_fighting_power)
			{
				continue;
			}
			legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(fightpower_legion_pair.legion_guid);
			if (nullptr == legion_ws_ptr)
			{
				continue;
			}
			legion_proto_legion_brief_info* legion_brief_info_msg = get_legion_list_msg.add_legion_list();
			if (nullptr == legion_brief_info_msg)
			{
				continue;
			}
			legion_ws_ptr->fill_legion_brief_info_msg(*legion_brief_info_msg);
			legion_list_entry_count++;
		}

		if (legion_list_entry_count <= 0)
		{
			return false;
		}
		else
		{
			get_legion_list_msg.set_legion_list_len(legion_list_entry_count);
		}

		return true;
	}

	void legion_ws_mgr::add_into_auto_accept_new_member_ordered_legion_list(int64 legion_fighting_power, guid_64 legion_guid)
	{
		add_into_ordered_legion_list(m_ordered_auto_accept_new_member_legion_list, legion_fighting_power, legion_guid);
	}

	void legion_ws_mgr::del_from_auto_accept_new_member_ordered_legion_list(guid_64 legion_guid)
	{
		del_from_ordered_legion_list(m_ordered_auto_accept_new_member_legion_list, legion_guid);
	}
	
	bool legion_ws_mgr::is_in_legion_boss_game_time()
	{
		return world_server::getInstance().get_activity_sec_left(e_activity_type_legion_boss, e_activity_time_get_all) > 0;
	}

	bool legion_ws_mgr::check_reset_act_rank(e_legion_act_type act_type)
	{
		if (false == time_helper::is_diff_day(m_last_update_rank_stamp[act_type]))
		{
			return false;
		}
		m_last_update_rank_stamp[act_type] = time_helper::get_cur_time_new().second;
		m_legion_act_ranks[act_type].clear();
		return true;
	}

	void legion_ws_mgr::remove_legion_rank_one(legion_rank_list& rank_list, legion_ws* score_legion_ptr)
	{
		if (nullptr == score_legion_ptr)
		{
			return;
		}

		legion_rank_list& _rank_list = rank_list;
		legion_rank_list::iterator rank_pos_ite = _rank_list.end();
		for (legion_rank_list::iterator ite = _rank_list.begin(); ite != _rank_list.end(); ++ite)
		{
			const s_ranking_player_info& _rank_info = *ite;
			if (score_legion_ptr->get_legion_guid() == _rank_info.role_guid)
			{
				_rank_list.erase(ite);
				break;
			}
		}
	}

	void legion_ws_mgr::insert_legion_rank_one(legion_rank_list& rank_list, legion_ws* score_legion_ptr, int64 legion_score, e_legion_rank_type rank_type, int32 rank_sub_id)
	{
		if (nullptr == score_legion_ptr || 0 == legion_score)
		{
			return;
		}

		legion_rank_list& _rank_list = rank_list;
		legion_rank_list::iterator rank_pos_ite = _rank_list.end();
		for (legion_rank_list::iterator ite = _rank_list.begin(); ite != _rank_list.end(); ++ite)
		{
			const s_ranking_player_info& _rank_info = *ite;
			if (legion_score > _rank_info.ranking_value)
			{
				rank_pos_ite = ite;
				break;
			}
		}
		s_ranking_player_info new_rank_info;
		score_legion_ptr->gen_info_in_rank_list(new_rank_info, legion_score, rank_type, rank_sub_id);
		_rank_list.insert(rank_pos_ite, new_rank_info);
	}

	void legion_ws_mgr::update_legion_rank_one(legion_rank_list& rank_list, legion_ws* score_legion_ptr, int64 legion_score, e_legion_rank_type rank_type, int32 rank_sub_id)
	{
		remove_legion_rank_one(rank_list, score_legion_ptr);
		insert_legion_rank_one(rank_list, score_legion_ptr, legion_score, rank_type, rank_sub_id);
	}

	void legion_ws_mgr::update_act_rank_all(e_legion_act_type act_type)
	{
		legion_ws_map::iterator ite;
		for (ite = m_legion_map.begin(); ite != m_legion_map.end(); ++ite)
		{
			legion_ws& legion_ref = ite->second;
			insert_legion_rank_one(m_legion_act_ranks[act_type], &legion_ref, legion_ref.get_legion_info((ELegionInfo)(ELegionInfo_act_score_raid + act_type)), (e_legion_rank_type)(e_legion_rank_type_raid + act_type));
		}
	}

	void legion_ws_mgr::send_act_rank_reward(e_legion_act_type act_type)
	{
		ActivityCommonConfigTemplate* _act_config_ptr = get_activity_cfg_ptr(act_type);
		if (nullptr == _act_config_ptr)
		{
			return;
		}

		int32 _rank = 0;
		legion_rank_list& _rank_list = m_legion_act_ranks[act_type];
		legion_rank_list::const_iterator rank_pos_ite = _rank_list.end();
		for (legion_rank_list::const_iterator ite = _rank_list.begin(); ite != _rank_list.end(); ++ite)
		{
			_rank++;
			const s_ranking_player_info& _rank_info = *ite;
			legion_ws* _rank_legion = get_legion(_rank_info.role_guid);
			if (_rank_legion)
			{
				std::vector<s_item_template_info> _item_list;
				std::vector<int32>& rwd_data = _act_config_ptr->RankRewards;
				globle_data::get_instance().get_item_list_by_rank_rwd(_rank, rwd_data, _item_list, ranking_mgr_ws::get_world_level_last());

				for (auto& item_ite : _item_list)
				{
					_rank_legion->give_activity_reward(_act_config_ptr, _rank_legion->get_attend_act_mems_map(act_type), item_ite.m_item_id, item_ite.m_item_num);
				}
			}
		}
	}

	void legion_ws_mgr::send_act_rank_reward(e_legion_rank_type rank_type, int32 npc_template_id, const legion_rank_list& rank_list, const std::unordered_map<ui64, legion_act_attend_mems_map>& attend_mems_map_list)
	{
		NpcTemplate* _npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_template_id);
		if (nullptr == _npc_template_ptr)
		{
			return;
		}
		ActivityCommonConfigTemplate* _act_config_ptr = get_activity_cfg_ptr(rank_type);
		if (nullptr == _act_config_ptr)
		{
			return;
		}

		int32 _rank = 0;
		const legion_rank_list& _rank_list = rank_list;
		legion_rank_list::const_iterator rank_pos_ite = _rank_list.end();
		for (legion_rank_list::const_iterator ite = _rank_list.begin(); ite != _rank_list.end(); ++ite)
		{
			_rank++;
			const s_ranking_player_info& _rank_info = *ite;
			legion_ws* _rank_legion = get_legion(_rank_info.role_guid);
			if (nullptr == _rank_legion)
			{
				continue;
			}
			std::vector<s_item_template_info> _item_list;
			std::vector<int32>& rwd_data = _npc_template_ptr->RankRewards;
			globle_data::get_instance().get_item_list_by_rank_rwd(_rank, rwd_data, _item_list, ranking_mgr_ws::get_world_level_last());
			//_rank_legion->send_legion_welfare_by_npc(npc_template_id, _rank);
			std::unordered_map<ui64, legion_act_attend_mems_map>::const_iterator _attend_mems_map = attend_mems_map_list.find(_rank_info.role_guid.server_64);
			if (_attend_mems_map == attend_mems_map_list.end())
			{
				continue;
			}
			for (auto& item_ite : _item_list)
			{
				_rank_legion->give_activity_reward(_act_config_ptr, _attend_mems_map->second, item_ite.m_item_id, item_ite.m_item_num);
			}
		}
	}

	void legion_ws_mgr::send_act_boss_rank_notice(const legion_rank_list& rank_list, const int32& kill_npc_name)
	{  
		if (rank_list.size() <= 0)
		{
			return;
		}
		if ( 0 == kill_npc_name)
		{
			return;
		}			
		NpcTemplate* npc_ptr = GET_TEMPLATE(NpcTemplate, kill_npc_name);
		if (nullptr == npc_ptr)
		{
			return;
		} 


		int32 create_time = time_helper::get_cur_time_new().second;

		//���а��һ
		int32 top_notice_id = 93000189;
		const guid_64& top_sender_guid = rank_list.front().role_guid;
		std::vector<std::string> vec_notice_str_top;
		vec_notice_str_top.push_back(template_manager::get_instance().get_str_id_by_notice_id(top_notice_id));
		vec_notice_str_top.push_back(rank_list.front().role_name);
		const std::string& temp_string_first = template_manager::get_instance().get_str_by_string_template_id(npc_ptr->NpcName);
		vec_notice_str_top.push_back(temp_string_first);
		std::string top_notice_str = init_unit::implode(vec_notice_str_top);
		event_ws_mgr::get_instance().send_notice_to_all(top_notice_id, create_time, top_sender_guid, top_notice_str);


		int32 rank = 0;
		for (auto ite = rank_list.begin(); ite!= rank_list.end();ite++)
		{
			int32 notice_id = 93000192;
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			const std::string& temp_string_second = template_manager::get_instance().get_str_by_string_template_id(npc_ptr->NpcName);
			vec_notice_str.push_back(temp_string_second);
			vec_notice_str.push_back(template_manager::get_instance().int_to_string(++rank));
			std::string notice_str = init_unit::implode(vec_notice_str);
			event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time++, ite->role_guid, notice_str);
		}
	}

	void legion_ws_mgr::send_act_raid_rank_notice(e_legion_act_type& rank_type)
	{
		switch (rank_type)
		{
		case faith::e_legion_act_type_raid:
		{
			legion_rank_list& _rank_list = m_legion_act_ranks[e_legion_act_type_raid];
			int32 create_time = time_helper::get_cur_time_new().second;

			//��һ������
			MapTemplate* map_ptr = GET_TEMPLATE(MapTemplate, _rank_list.front().ranking_value / LegionBossRankingPara1 + legion_start_legion_boss_map_id);
			if (nullptr == map_ptr)
			{
				return;
			}
			int32 top_notice_id = 93000195;
			guid_64 top_sender_guid = _rank_list.front().role_guid;
			int32 pass_time = LegionBossRankingPara2 - ((_rank_list.front().ranking_value % LegionBossRankingPara1) / LegionBossRankingPara3);
			std::vector<std::string> vec_notice_str_top;
			vec_notice_str_top.push_back(template_manager::get_instance().get_str_id_by_notice_id(top_notice_id));
			vec_notice_str_top.push_back(_rank_list.front().role_name);
			vec_notice_str_top.push_back(template_manager::get_instance().int_to_string(pass_time));
			const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(map_ptr->MapName);
			vec_notice_str_top.push_back(temp_string);
			std::string top_notice_str = init_unit::implode(vec_notice_str_top);
			event_ws_mgr::get_instance().send_notice_to_all(top_notice_id, create_time, top_sender_guid, top_notice_str);

			//�ϼ������й���
			int32 _rank = 0;
			int32 notice_id = 93000196;
			legion_rank_list::const_iterator rank_pos_ite = _rank_list.end();
			for (legion_rank_list::const_iterator ite = _rank_list.begin(); ite != _rank_list.end(); ++ite)
			{
				_rank++; 
				const s_ranking_player_info& _rank_info = *ite;
				guid_64 sender_guid = _rank_info.supporting_guid;
				std::vector<std::string> vec_notice_str;
				vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
				vec_notice_str.push_back(template_manager::get_instance().int_to_string(_rank));
				std::string notice_str = init_unit::implode(vec_notice_str);
				event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time++, sender_guid, notice_str);
			}
			break;
		}
		case faith::e_legion_act_type_bonfire:
		{
			legion_rank_list& _rank_list = m_legion_act_ranks[e_legion_act_type_bonfire];
			int32 create_time = time_helper::get_cur_time_new().second;

			//��һ������
			int32 top_notice_id = 93000197;
			guid_64 top_sender_guid = _rank_list.front().role_guid;
			std::vector<std::string> vec_notice_str_top;
			vec_notice_str_top.push_back(template_manager::get_instance().get_str_id_by_notice_id(top_notice_id));
			vec_notice_str_top.push_back(_rank_list.front().role_name);
			vec_notice_str_top.push_back(template_manager::get_instance().int_to_string(_rank_list.front().ranking_value));
			std::string top_notice_str = init_unit::implode(vec_notice_str_top);
			event_ws_mgr::get_instance().send_notice_to_all(top_notice_id, create_time, top_sender_guid, top_notice_str);

			//�ϼ������й���
			int32 _rank = 0;
			int32 notice_id = 93000198;
			legion_rank_list::const_iterator rank_pos_ite = _rank_list.end();
			for (legion_rank_list::const_iterator ite = _rank_list.begin(); ite != _rank_list.end(); ++ite)
			{
				_rank++;
				const s_ranking_player_info& _rank_info = *ite;
				guid_64 sender_guid = _rank_info.supporting_guid;
				std::vector<std::string> vec_notice_str;
				vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
				vec_notice_str.push_back(template_manager::get_instance().int_to_string(_rank_info.ranking_value));
				vec_notice_str.push_back(template_manager::get_instance().int_to_string(_rank));
				std::string notice_str = init_unit::implode(vec_notice_str);
				event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time++, sender_guid, notice_str);
			}
			break;
		}
		case faith::e_legion_act_type_max:
			break;
		default:
			break;
		}
	}

	void legion_ws_mgr::clear_boss_rank_list()
	{
		m_legion_monster_rank_map.clear();
	}

	void legion_ws_mgr::send_act_special_reward(e_legion_rank_type rank_type, legion_ws* rwd_legion_ptr, legion_act_attend_mems_map& attend_mems_map)
	{
		if (nullptr == rwd_legion_ptr)
		{
			return;
		}
		ActivityCommonConfigTemplate* _act_config_ptr = get_activity_cfg_ptr(rank_type);
		if (nullptr == _act_config_ptr)
		{
			return;
		}
		
		std::vector<s_item_template_info> _item_list;
		globle_data::get_instance().get_item_list_by_drop_bg(_act_config_ptr->SpecialReward, _item_list, 0, ranking_mgr_ws::get_world_level_last());
		
		for (auto& item_ite : _item_list)
		{
			rwd_legion_ptr->give_activity_reward(_act_config_ptr, attend_mems_map, item_ite.m_item_id, item_ite.m_item_num);
		}
	}

	void legion_ws_mgr::settle_today_act_rank(e_legion_act_type act_type)
	{
		if (false == check_reset_act_rank(act_type))
		{
			return;
		}
		update_act_rank_all(act_type);

		legion_rank_list& _rank_list = m_legion_act_ranks[act_type];
		if (_rank_list.size() == 0)
		{
			return;
		}
		send_act_rank_reward(act_type);
		send_act_raid_rank_notice(act_type);
	}

	void legion_ws_mgr::tick_1_min_legion_activity(const uint64& tick_time)
	{
		struct tm* cur_date = time_helper::get_localtime();
		if (nullptr == cur_date)
		{
			return;
		}
		ActivityCommonConfigTemplate* _act_config_ptr = get_activity_cfg_ptr(e_legion_rank_type_raid);
		if (nullptr == _act_config_ptr)
		{
			return;
		}
		std::vector<int32> open_time = world_server::getInstance().get_activity_open_time(_act_config_ptr->ActivityType);
		if (open_time.size() >= once_activity_open_time_array_size
			&& cur_date->tm_hour == open_time[6] && cur_date->tm_min == open_time[7] + 1)
		{
			settle_today_act_rank(e_legion_act_type_raid);
		}

		_act_config_ptr = get_activity_cfg_ptr(e_legion_rank_type_bonfire);
		if (nullptr == _act_config_ptr)
		{
			return;
		}
		open_time = world_server::getInstance().get_activity_open_time(_act_config_ptr->ActivityType);
		if (open_time.size() >= once_activity_open_time_array_size
			&& cur_date->tm_hour == open_time[6] && cur_date->tm_min == open_time[7] + 1)
		{
			settle_today_act_rank(e_legion_act_type_bonfire);
		}
	}

	ActivityCommonConfigTemplate* legion_ws_mgr::get_activity_cfg_ptr(e_legion_rank_type rank_type)
	{
		int32 _activity_type = -1;
		switch (rank_type)
		{
		case e_legion_rank_type_world_elite:
			_activity_type = e_activity_type_world_elite;
			break;
		case e_legion_rank_type_world_boss:
			_activity_type = e_activity_type_world_boss;
			break;
		case e_legion_rank_type_raid:
			_activity_type = e_activity_type_legion_boss;
			break;
		case e_legion_rank_type_bonfire:
			_activity_type = e_activity_type_legion_bonfire;
			break;
		case e_legion_rank_type_cross_world_boss:
			_activity_type = e_activity_type_cross_server_world_boss;
			break;
		default:
			break;
		}
		ActivityCommonConfigTemplate* _act_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + _activity_type);
		return _act_config_ptr;
	}

	ActivityCommonConfigTemplate* legion_ws_mgr::get_activity_cfg_ptr(e_legion_act_type act_type)
	{
		int32 _rank_type = -1;
		switch (act_type)
		{
		case e_legion_act_type_raid:
			_rank_type = e_legion_rank_type_raid;
			break;
		case e_legion_act_type_bonfire:
			_rank_type = e_legion_rank_type_bonfire;
			break;
		default:
			break;
		}

		if (_rank_type < 0)
		{
			return nullptr;
		}
		return get_activity_cfg_ptr((e_legion_rank_type)_rank_type);
	}

	legion_rank_list* legion_ws_mgr::get_legion_rank(e_legion_rank_type rank_type, int32 rank_sub_id)
	{
		switch (rank_type)
		{
		case faith::e_legion_rank_type_fighting_power:
		{
			return &m_legion_fight_power_rank;
		}
		break;
		case faith::e_legion_rank_type_city_war:
		{
			return &m_legion_city_war_rank;
		}
		break;
		case faith::e_legion_rank_type_world_elite:
		case faith::e_legion_rank_type_world_boss:
		{
			if (rank_sub_id <= 0)
			{
				return nullptr;
			}
			return &m_legion_monster_rank_map[rank_sub_id];
		}
		break;
		case faith::e_legion_rank_type_raid:
		{
			return &m_legion_act_ranks[e_legion_act_type_raid];
		}
		break;
		case faith::e_legion_rank_type_bonfire:
		{
			return &m_legion_act_ranks[e_legion_act_type_bonfire];
		}
		break;
		case faith::e_legion_rank_type_cross_world_boss:
		{
			if (rank_sub_id <= 0)
			{
				return nullptr;
			}
			return &m_legion_act_ranks[rank_sub_id];
		}
		break;
		default:
			break;
		}

		return nullptr;
	}

	ranking_list* legion_ws_mgr::get_legion_ranking_by_ranking_type(e_RankingIndex ranking_type)
	{
		RankingShowTemplate* templates_ptr = template_manager::get_instance().get_ranking_template_by_ranking_type(ranking_type);
		if (nullptr == templates_ptr)
		{
			return nullptr;
		}

		switch (ranking_type)
		{
		case faith::e_RankingIndex_legion_core:
			return &m_legion_fight_power_rank;
			break;
		case faith::e_RankingIndex_legion_territory:
			return &m_legion_city_war_rank;
			break;
		case faith::e_RankingIndex_legion_boss:
			return &m_legion_act_ranks[e_legion_act_type_raid];
			break;
		case faith::e_RankingIndex_legion_bonfire:
			return &m_legion_act_ranks[e_legion_act_type_bonfire];
			break;
		case faith::e_RankingIndex_world_boss_one:
		case faith::e_RankingIndex_world_boss_two:
		case faith::e_RankingIndex_world_boss_three:
		case faith::e_RankingIndex_world_boss_four:
		case faith::e_RankingIndex_world_boss_five:
		case faith::e_RankingIndex_world_boss_six:
		case faith::e_RankingIndex_world_boss_seven:
		case faith::e_RankingIndex_world_elite_one:
		case faith::e_RankingIndex_world_elite_two:
		case faith::e_RankingIndex_world_elite_three:
		case faith::e_RankingIndex_world_elite_four:
		case faith::e_RankingIndex_world_elite_five:
		case faith::e_RankingIndex_world_elite_six:
		case faith::e_RankingIndex_world_elite_seven:
		case faith::e_RankingIndex_world_elite_eight:
		{
			return get_legion_rank(e_legion_rank_type_world_boss, templates_ptr->RankingBossID);
		}
			break;
		default:
			break;
		}
		return nullptr;
	}

	void legion_ws_mgr::create_all_legion_station_map()
	{
		legion_ws_map::iterator ite;
		for (ite = m_legion_map.begin(); ite != m_legion_map.end(); ++ite)
		{
			legion_ws& legion_ref = ite->second;
			legion_ref.create_legion_station_map();
		}
	}

	void legion_ws_mgr::calcu_all_legion_ranks()
	{
		legion_ws_map::iterator ite;
		for (ite = m_legion_map.begin(); ite != m_legion_map.end(); ++ite)
		{
			legion_ws& legion_ref = ite->second;
			insert_legion_rank_one(m_legion_fight_power_rank, &legion_ref, legion_ref.get_legion_gs_value(), e_legion_rank_type_fighting_power);
			insert_legion_rank_one(m_legion_act_ranks[e_legion_act_type_raid], &legion_ref, legion_ref.get_legion_info(ELegionInfo_act_score_raid), e_legion_rank_type_raid);
			insert_legion_rank_one(m_legion_act_ranks[e_legion_act_type_bonfire], &legion_ref, legion_ref.get_legion_info(ELegionInfo_act_score_bonfire), e_legion_rank_type_bonfire);
		}
	}

	void legion_ws_mgr::calcu_one_legion_ranks(legion_ws& rank_legion)
	{
		update_legion_rank_one(m_legion_fight_power_rank, &rank_legion, rank_legion.get_legion_gs_value(), e_legion_rank_type_fighting_power);
		update_legion_rank_one(m_legion_act_ranks[e_legion_act_type_raid], &rank_legion, rank_legion.get_legion_info(ELegionInfo_act_score_raid), e_legion_rank_type_raid);
		update_legion_rank_one(m_legion_act_ranks[e_legion_act_type_bonfire], &rank_legion, rank_legion.get_legion_info(ELegionInfo_act_score_bonfire), e_legion_rank_type_bonfire);
	}

	void legion_ws_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		//���ų�Ա
		auto iter_legion_ws = m_legion_map.begin();
		for (; iter_legion_ws != m_legion_map.end(); iter_legion_ws++)
		{
			iter_legion_ws->second.change_player_name_func(role_guid, role_name);
		}

		change_player_name_in_list(m_legion_fight_power_rank, role_guid, role_name);
		change_player_name_in_list(m_legion_city_war_rank, role_guid, role_name);

		for ( int32 i = 0; i < e_legion_act_type_max; ++i)
		{
			change_player_name_in_list(m_legion_act_ranks[i], role_guid, role_name);
		}
	}

	void legion_ws_mgr::change_player_name_in_list(legion_rank_list&  rank_list, const guid_64& role_guid, const xstring& role_name)
	{
		auto iter = rank_list.begin();
		for (; iter != rank_list.end(); iter++)
		{
			if (iter->role_guid == role_guid)
			{
				iter->set_role_name(role_name);
			}
			if (iter->supporting_guid == role_guid)
			{
				iter->set_supporting_name(role_name);
			}
		}
	}

	void legion_ws_mgr::send_message_to_all_legion(google::protobuf::Message* net_pro, uint32 header)
	{
		auto iter = m_legion_map.begin();
		for (; iter != m_legion_map.end(); ++iter)
		{
			iter->second.send_message_to_all_member(net_pro, header);
		}
	}

	bool legion_ws_mgr::is_need_refresh_overload_city_bid()
	{
		bool is_have_bid_info = city_war_territory_mgr::get_instance().is_have_overload_city_bid_info();
		if (is_have_bid_info == true)
		{
			return false;
		}
		ActivityCommonConfigTemplate* _activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_overlord_city_war);
		if (_activity_cfg_ptr == nullptr || 1 != _activity_cfg_ptr->IsOpen)
		{
			return false;
		}
		int32 ServerOpenDays = globle_data::get_instance().get_server_days();
		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 now_stamp = cur_time_info.second;
		int32 day_in_week = cur_time_info.day_in_week;

		bool is_today_open = false;
		std::vector<int32> open_data = world_server::getInstance().get_activity_open_data(e_activity_type_overlord_city_war);
		//if (ServerOpenDays == _activity_cfg_ptr->ParamInt1)
		//{
		//	is_today_open = true;
		//}
		for (int32 idx = 0; idx < open_data.size(); idx++)
		{
			if (day_in_week == open_data[idx])
			{
				is_today_open = true;
				break;
			}
		}
		if (false == is_today_open)
		{
			return false;
		}

		int32 refresh_stamp = -1;
		//std::vector<int32> refresh_time_arr = _activity_cfg_ptr->ActivityOpenTime1;
		if (_activity_cfg_ptr->ActivityOpenTime1.size() < 2)
		{
			return false;
		}
		refresh_stamp = time_helper::get_stamp_by_hour_min(_activity_cfg_ptr->ActivityOpenTime1[0], _activity_cfg_ptr->ActivityOpenTime1[1]);
		if (now_stamp > refresh_stamp)
		{
			return true;
		}
		return false;
	}

	legion_ws* legion_ws_mgr::find_legion_ws_with_chief_guid(guid_64 chief_guid)
	{
		legion_ws_map::iterator ite;
		for (ite = m_legion_map.begin(); ite != m_legion_map.end(); ++ite)
		{
			legion_ws& legion_ref = ite->second;
			if (legion_ref.get_chief_guid() == chief_guid)
			{
				return &legion_ref;
			}
		}
		return nullptr;
	}

	legion_ws* legion_ws_mgr::find_legion_ws_with_chief_name(xstring p_role_name)
	{
		legion_ws_map::iterator ite;
		for (ite = m_legion_map.begin(); ite != m_legion_map.end(); ++ite)
		{
			legion_ws& legion_ref = ite->second;
			if (legion_ref.get_chief_name() == p_role_name)
			{
				return &legion_ref;
			}
		}
		return nullptr;
	}

	void legion_ws_mgr::kick_out_chief_with_legion_guid(guid_64 legion_guid)
	{
		legion_ws* legion_ptr = get_legion(legion_guid);
		if (nullptr == legion_ptr)
		{
			return;
		}
		legion_ptr->gm_kick_out_chief();
	}

	void legion_ws_mgr::kick_out_chief_with_chief_account(xstring p_role_name)
	{
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(p_role_name);
		if (nullptr != client_session_ptr)
		{
			guid_64 legion_guid = client_session_ptr->get_legion_guid();
			legion_ws* legion_ws_ptr = get_legion(legion_guid);
			if (nullptr == legion_ws_ptr)
			{
				return;
			}
			if (legion_ws_ptr->get_chief_guid() != client_session_ptr->get_role_guid())
			{
				return;
			}
			legion_ws_ptr->gm_kick_out_chief();
		}
	}
	void legion_ws_mgr::deduct_maintain_territories() 
	{
		legion_ws_map_it ite;
		for (ite = m_legion_map.begin(); ite != m_legion_map.end(); ++ite)
		{
			legion_ws& legion_ws_ref = ite->second;
			legion_ws_ref.maintain_territories_cross_city();
		}
	}
	void legion_ws_mgr::change_gate_legion_name(guid_64 legion_guid, xstring legion_name)
	{
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross) == false)
		{
			return;
		}
		ws2ws_change_gate_legion_name msg;
		msg.legion_guid = legion_guid;
		msg.set_legion_name(legion_name);
		int32 server_id =  world_server::getInstance().get_cross_id();
		if (server_id <= 0)
		{
			return;
		}
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_change_gate_legion_name, &msg, sizeof(msg));
	}

	int32 legion_ws_mgr::get_active_legion_num()
	{
		int32 active_legion_num = 0;
		for (auto ite = m_legion_map.begin(); ite != m_legion_map.end(); )
		{
			// tick�����ɾ��legion Ҫ�����ַ�ʽ��ֹ������ʧЧ
			auto temp_it = ite++;
			//ͳ�ƻ�Ծ��������
			if (temp_it->second.week_is_have_player_active())
			{
				active_legion_num++;
			}
		}
		return active_legion_num;
	}

	void legion_ws_mgr::send_all_legion_world_boss_info()
	{
		legion_ws_map& legion_map = get_legion_map();
		legion_ws_map_it ite;
		for (ite = legion_map.begin(); ite != legion_map.end(); ++ite)
		{
			legion_ws& legion_ws_ref = ite->second;
			legion_ws_ref.send_legion_strategy_world_boss();
		}
	}

}