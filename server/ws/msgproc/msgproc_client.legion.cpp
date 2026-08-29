#include "msgproc_client.hpp"
#include <Utility/parse_msg.h>
#include "ws_client.hpp"
#include "../server/client_session_mgr.hpp"
#include "../server/legion/legion_ws_mgr.h"
#include "../server/legion/city_war_territory_mgr.h"
#include "invalid_word/invalid_ansi_word.h"
#include "template/template_manager.h"
#include "Logic/time_def.hpp"
#include "Logic/activity_def.hpp"
#include "server_log.hpp"
#include "utility/init_unit.h"
#include "../server/mail/event_ws_mgr.h"
#include "template/StringConst_S.h"
#include "world_server.hpp"
#include "../server/legion/legion_ws_answer.h"
#include "../server/activity/cross_server_city_war_ws_mgr.h"
#include "../server/activity/cross_server_world_boss_ws_mgr.h"
#include "globle_data.h"
#include "base/ecs_world.h"
#include "components/scene/cs_map_component.h"
#include "goods.pb.h"
#include "net.pb.h"
#include "world_boss_def.hpp"
#include "game.pb.h"

namespace faith
{
	void c2ws_req_create_legion(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_create_legion req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许创建
		if (false == session->is_self_server())
		{
			return;
		}

		bool auto_accept_new_member = (req.auto_accept_new_member() > 0 ? true : false);
		std::string legion_name = req.legion_name();
		int32 new_legion_level = req.legion_level();
		int32 name_len = legion_name.size();
		if (name_len >= max_name_size)
		{
			return;
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws_mgr_ref.create_legion_begin(session->get_role_guid(), legion_name.c_str(), legion_name.size(), new_legion_level, auto_accept_new_member);
	}

	void c2ws_req_leave_legion(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_leave_legion req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		//跨服状态下不允许退出
		if (false == session->is_self_server())
		{
			return;
		}
		// 如果角色不在任何军团中就不能进行退出军团操作
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		if (false == legion_ws_mgr_ref.check_can_leave_legion(*legion_ws_ptr, session->get_role_guid()))
		{ 
			return;
		}

		// 向玩家发送离队消息
		legion_proto_leave_legion_end leave_legion_end_to_client_msg;
		session->send_to_client(&leave_legion_end_to_client_msg, e_msgindex_s2c_leave_legion_end);

		s_legion_member_info* member_info_ptr = legion_ws_ptr->get_member(session->get_role_guid());
		if (member_info_ptr != nullptr && member_info_ptr->data_ary[e_legion_member_info_agora_id] != 0)
		{
			legion_ws_ptr->leave_voie_channel(session->get_role_guid());
		}

		//从军团中删除玩家的步骤放在最后，因为玩家被删除后所有信息会取不到
		legion_ws_ptr->del_member(session->get_role_guid(), session->m_login_third_data, session->m_login_type);
	}

	void c2ws_req_disslove_legion(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_dissolve_legion req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		// 如果角色不在任何军团中就不能解散军团
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		
		// 如果角色不是团长就不能解散军团
		if (legion_ws_ptr->is_chief(session->get_role_guid()) == false)
		{
			return;
		}
		
		// 如果是城战的参战军团且城战已经开始,那么城战期间不能解散军团
		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
		if (city_war_mgr.is_during_city_war(legion_ws_ptr->get_legion_guid()) == true)
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, e_legion_error_can_not_dissolve_during_city_war);
			return;
		}
		if (cross_server_city_war_ws_mgr::get_instance().is_in_game_time() && cross_server_city_war_ws_mgr::get_instance().get_cur_legion_in_rank(legion_ws_ptr->get_legion_guid()) >= 0)
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, e_legion_error_can_not_dissolve_during_city_war);
			return;
		}
		
		// 如果是战争之王且正在举办王城舞会就不能解散军团
		// PS: 和城战进行时不同的是,舞会期间王城占领军团的非城主成员是可以退出军团的
		if (legion_ws_ptr->get_chief_guid() == city_war_mgr.get_city_master_guid()
			&& city_war_mgr.is_banquet_holding() == true)
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, e_legion_error_can_not_dissolve_during_banquet);
			return;
		}

		guid_64 legion_guid = legion_ws_ptr->get_legion_guid();

		legion_ws_mgr::get_instance().del_legion(legion_guid, true, session->get_role_guid(), session->m_login_third_data, session->m_login_type);
	}

	void c2ws_req_get_legion_list(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_get_legion_list req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		int32 start_index = req.start_index();
		int64 base_fighting_power = req.base_fighting_power();
		int32 only_get_auto_accpet_new_member_legion = req.only_get_auto_accept_new_member();


		c2ws_req_get_legion_list_logic(session, start_index, base_fighting_power, only_get_auto_accpet_new_member_legion);

		
		/*faith::legion_proto_get_legion_list_end get_legion_list_end_msg;
		if (legion_ws_mgr::get_instance().get_legion_list(start_index, base_fighting_power, get_legion_list_end_msg, only_get_auto_accpet_new_member_legion))
		{
			session->send_to_client(&get_legion_list_end_msg, e_msgindex_s2c_get_legion_list_end);
		}*/
	}
	void c2ws_req_get_legion_list_logic(client_session* session,  int32  start_index, int64 base_fighting_power, int32 only_get_auto_accpet_new_member_legion)
	{
		if (nullptr == session)
		{
			return;
		}

		faith::legion_proto_get_legion_list_end get_legion_list_end_msg;
		if (legion_ws_mgr::get_instance().get_legion_list(start_index, base_fighting_power, get_legion_list_end_msg, only_get_auto_accpet_new_member_legion))
		{
			session->send_to_client(&get_legion_list_end_msg, e_msgindex_s2c_get_legion_list_end);
		}

	}

	void c2ws_req_apply_to_join_legion(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_apply_to_join_legion req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws_mgr_ref.join_to_legion(session, req.legion_guid(), true);

	}

	void c2ws_req_get_legion_applicant_list(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_get_applicant_info_list req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		// 请求获取入团申请名单的角色必须要在某个军团中才可以
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		legion_ws_ptr->send_applicant_info_list(session);
	}

	void c2ws_req_process_legion_apply(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_process_apply req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		// 如果要处理入团申请的角色不在军团中就不能进行这个操作
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		e_legion_job_title op_member_job_title = legion_ws_ptr->get_job_title(session->get_role_guid());
		// 没有任何职位的
		if (op_member_job_title <= e_legion_job_title_none)
		{
			return;
		}

		// 如果申请者的申请信息不在申请列表中,说明这个申请者的申请已经被处理过了
		legion_ws_applicant_list& applicant_list = legion_ws_ptr->get_applicant_list();
		legion_ws_applicant* applicant_info = applicant_list.get_applicant(req.applicant_guid());
		if (nullptr == applicant_info)
		{
			return;
		}
		
		client_session* applicant_session = client_session_mgr::getInstance().get_session(req.applicant_guid());

		// 根据接受或拒绝入团申请来进行不同的操作
		bool is_accept = (req.is_accept() > 0 ? true : false);

		if (is_accept && legion_ws_mgr::get_instance().check_can_join_legion(*legion_ws_ptr, req.applicant_guid(), session))
		{
			// 创建军团成员信息
			s_legion_member_info new_member_info;
			legion_ws_mgr::get_instance().generate_player_legion_info(applicant_session, new_member_info, applicant_info);

			if (false == legion_ws_ptr->add_member(new_member_info))
			{
				return;
			}
			legion_ws_applicant_list& applicant_list_ref = legion_ws_ptr->get_applicant_list();
			applicant_list_ref.del_applicant_with_join_legion(req.applicant_guid());
		}

		applicant_list.del_applicant(req.applicant_guid()); // 删除放这里 上面会用到
		
		if (applicant_session)
		{
			legion_proto_apply_to_join_legion_end apply_to_join_legion_msg;
			apply_to_join_legion_msg.set_apply_stage(is_accept ? e_legion_apply_stage_is_accepted : e_legion_apply_stage_is_refused);
			applicant_session->send_to_client(&apply_to_join_legion_msg, e_msgindex_s2c_apply_to_join_legion_end);
		}

		legion_proto_process_apply_end process_apply_end_msg;
		process_apply_end_msg.set_applicant_guid(req.applicant_guid());
		process_apply_end_msg.set_is_accept(is_accept ? 1 : 0);
		session->send_to_client(&process_apply_end_msg, e_msgindex_s2c_process_legion_apply_end);
	}

	void c2ws_req_legion_construction_level_up(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_construction_level_up req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		int32 construction_index = req.construction_index();
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

		// 不在任何军团中的角色不能进行军团建筑升级操作
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		// 只有职位至少是副军团长级别的成员才能够升级军团建筑
		if (legion_ws_ptr->get_job_title(session->get_role_guid()) < e_legion_job_title_assistant_chief)
		{
			return;
		}

		legion_ws_ptr->construction_level_up(construction_index);
	}

	void c2ws_req_get_construction_buff(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//s_client_uid client_uid;
		//parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		//legion_proto_get_construction_buff req;
		//parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		//client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		//if (nullptr == session)
		//{
		//	return;
		//}

		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//int32 construction_index = req.construction_index();
		//if (construction_index < ELegionInfo_construction_level_main || construction_index > ELegionInfo_construction_level_3)
		//{
		//	return;
		//}
		//int32 construction_level = legion_ws_ptr->get_legion_info((ELegionInfo)construction_index);

		//ws2cs_get_construction_buff get_construction_buff_msg;
		//get_construction_buff_msg.role_guid = session->get_role_guid();
		//get_construction_buff_msg.construction_level = construction_level;
		//get_construction_buff_msg.construction_index = req.construction_index();
		//session->send_to_cs(&get_construction_buff_msg, sizeof(get_construction_buff_msg));
	}

	void c2ws_req_legion_donate_money(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_donate_money req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_legion_donate_money_logic(session, req.donate_times());
		//int32 donate_times = req.donate_times();
		//if (donate_times <= 0)
		//{
		//	return;
		//}

		//// 不在任何军团中的角色不能进行军团捐献操作
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//ws2cs_donate_legion_money donate_legion_money_msg;
		//donate_legion_money_msg.role_guid = session->get_role_guid();
		//donate_legion_money_msg.donate_times = donate_times;
		//bool is_use = proto_by_lua(e_msg_index_ws2cs_donate_legion_money);
		//if (is_use == false)
		//{
		//	session->send_to_cs(&donate_legion_money_msg, sizeof(donate_legion_money_msg));
		//}
		//else
		//{
		//	faith::ws2cs_proto::donate_legion_money pro_msg;
		//	donate_legion_money_msg.to_proto(pro_msg);
		//	session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_donate_legion_money);
		//}
	}

	void c2ws_req_legion_donate_money_logic(client_session* session, int32 donate_times)
	{
		if (nullptr == session)
		{
			return;
		}
		if (donate_times <= 0)
		{
			return;
		}

		// 不在任何军团中的角色不能进行军团捐献操作
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		ws2cs_donate_legion_money donate_legion_money_msg;
		donate_legion_money_msg.role_guid = session->get_role_guid();
		donate_legion_money_msg.donate_times = donate_times;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_donate_legion_money);
		if (is_use == false)
		{
			session->send_to_cs(&donate_legion_money_msg, sizeof(donate_legion_money_msg));
		}
		else
		{
			faith::ws2cs_proto::donate_legion_money pro_msg;
			donate_legion_money_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_donate_legion_money);
		}

	}

	void c2ws_req_legion_donate_item(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_donate_item req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_legion_donate_item_logic(session, req.donate_item_index(), req.donate_times());
		//int32 donate_item_index = req.donate_item_index();
		//if (donate_item_index < 0 || donate_item_index >= 4)
		//{
		//	return;
		//}
		//
		////only can donate 1 or 10 times
		//int32 donate_item_num = req.donate_times();
		///*if (donate_item_num != 1 && donate_item_num != 10)
		//{
		//	return;
		//}*/

		////捐赠物品数量小于0则不可以操作
		//if (donate_item_num == 0)
		//{
		//	return;
		//}
		////捐赠物品数量最多捐赠10
		//if (donate_item_num >= 10)
		//{
		//	donate_item_num = 10;
		//}
		//// 不在任何军团中的角色不能进行军团捐献操作
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}
		//ws2cs_donate_legion_item donate_legion_item_msg;
		//donate_legion_item_msg.role_guid = session->get_role_guid();
		//donate_legion_item_msg.donate_item_index = donate_item_index;
		//donate_legion_item_msg.donate_item_num = donate_item_num;
		//bool is_use = proto_by_lua(e_msg_index_ws2cs_donate_legion_item);
		//if (is_use == false)
		//{
		//	session->send_to_cs(&donate_legion_item_msg, sizeof(donate_legion_item_msg));
		//}
		//else
		//{
		//	faith::ws2cs_proto::donate_legion_item pro_msg;
		//	donate_legion_item_msg.to_proto(pro_msg);
		//	session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_donate_legion_item);
		//}
	}

	void c2ws_req_legion_donate_item_logic(client_session* session, int32 donate_item_index, int32	donate_times)
	{
		if (nullptr == session)
		{
			return;
		}
		//int32 donate_item_index = req.donate_item_index();
		if (donate_item_index < 0 || donate_item_index >= 4)
		{
			return;
		}

		//only can donate 1 or 10 times
		int32 donate_item_num = donate_times;
		/*if (donate_item_num != 1 && donate_item_num != 10)
		{
		return;
		}*/

		//捐赠物品数量小于0则不可以操作
		if (donate_item_num == 0)
		{
			return;
		}
		//捐赠物品数量最多捐赠10
		if (donate_item_num >= 10)
		{
			donate_item_num = 10;
		}
		// 不在任何军团中的角色不能进行军团捐献操作
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		ws2cs_donate_legion_item donate_legion_item_msg;
		donate_legion_item_msg.role_guid = session->get_role_guid();
		donate_legion_item_msg.donate_item_index = donate_item_index;
		donate_legion_item_msg.donate_item_num = donate_item_num;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_donate_legion_item);
		if (is_use == false)
		{
			session->send_to_cs(&donate_legion_item_msg, sizeof(donate_legion_item_msg));
		}
		else
		{
			faith::ws2cs_proto::donate_legion_item pro_msg;
			donate_legion_item_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_donate_legion_item);
		}
	}

	void c2ws_req_update_legion_announcement(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_update_announcement req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		// 如果角色不在任何军团中就不能进行修改军团公告操作
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		// 如果角色在军团没有任何职位就不能修改军团公告
		if (legion_ws_ptr->get_job_title(session->get_role_guid()) <= e_legion_job_title_none)
		{
			return;
		}

		// 新公告内容的长度不能超过指定长度
		std::string announcement_content = req.content();
		if (announcement_content.length() > max_legion_announcement_size)
		{
			return;
		}
		// 检查是否可用
		if (invalid_ansi_word::is_valid_ansi_str(announcement_content) == false)
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, e_legion_error_error_legion_name_illegal);
			return;
		}
		if (invalid_ansi_word::include_invalid_ansi_str(announcement_content))
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, e_legion_error_error_legion_name_illegal);
			return;
		}
		legion_ws_ptr->set_announcement(announcement_content.c_str(), announcement_content.size(), true, true, session->get_role_guid(), session->m_login_third_data, session->m_login_type);
		legion_ws_ptr->send_legion_announcement();
	}

	void c2ws_req_get_event_list(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_get_legion_event_list req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_get_event_list_logic(session, req.from_happen_time());
		//// 如果角色不在任何军团中就不用获取军团事件
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//uint32 form_happen_time = req.from_happen_time();
		//legion_ws_event_logger& event_logger = legion_ws_ptr->get_event_logger();
		//legion_proto_get_legion_event_list_end get_event_list_end_msg;
		//if (event_logger.fill_get_event_list_msg(get_event_list_end_msg, form_happen_time) != 1)
		//{
		//	return;
		//}
		//
		//session->send_to_client(&get_event_list_end_msg, e_msgindex_s2c_get_legion_event_list_end);
	}
	void c2ws_req_get_event_list_logic(client_session* session, int32 from_happen_time)
	{
		if (nullptr == session)
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		legion_ws_event_logger& event_logger = legion_ws_ptr->get_event_logger();
		legion_proto_get_legion_event_list_end get_event_list_end_msg;
		if (event_logger.fill_get_event_list_msg(get_event_list_end_msg, from_happen_time) != 1)
		{
			return;
		}

		session->send_to_client(&get_event_list_end_msg, e_msgindex_s2c_get_legion_event_list_end);

	}

	void c2ws_req_kick_out_legion_member(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_kickout_legion_member req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		
		guid_64 leaver_guid(req.leaver_guid());

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		// 没在任何军团中的角色不能发起踢人操作
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		if (false == legion_ws_mgr::get_instance().check_can_leave_legion(*legion_ws_ptr, leaver_guid, session))
		{ 
			return;
		}

		// 如果被踢出的成员当前在线就通他已经被T了
		client_session* leaver_session = client_session_mgr::getInstance().get_session(leaver_guid);
		if (nullptr != leaver_session)
		{
			legion_proto_kickout_legion_member_end kickout_legion_member_end_msg;
			kickout_legion_member_end_msg.set_leaver_guid(leaver_guid.server_64);
			leaver_session->send_to_client(&kickout_legion_member_end_msg, e_msgindex_s2c_kickout_legion_member_end);
		}

		legion_ws_ptr->del_member(leaver_guid, session->m_login_third_data, session->m_login_type);
	}

	void c2ws_req_appoint_legion_member_job_title(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_appoint_job_title req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		// 如果发起任命请求的角色没有在任何军团中就不能进行任命操作
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		// 只有军团长才能进行任命操作
		if (legion_ws_ptr->get_job_title(session->get_role_guid()) != e_legion_job_title_chief)
		{
			return;
		}

		guid_64 member_guid(req.member_guid());
		if (member_guid.is_valid() == false || member_guid == legion_ws_ptr->get_chief_guid())
		{
			return;
		}
		int32 job_title = req.job_title();
		
		int32 ret = legion_ws_ptr->appoint(member_guid, (e_legion_job_title)job_title, session->m_login_third_data, session->m_login_type);
		switch (ret)
		{
		case 1:
			{
				// 给军团长发送一条委任结果反馈
				legion_proto_appoint_job_title_end appoint_job_title_end;
				appoint_job_title_end.set_result(ret);
				appoint_job_title_end.set_job_title(job_title);
				appoint_job_title_end.set_is_to_chief(true);
				session->send_to_client(&appoint_job_title_end, e_msgindex_s2c_appoint_legion_member_job_title_end);

				// 给被委任的成员发送一条被指派的结果反馈
				client_session* appoint_session = client_session_mgr::getInstance().get_session(member_guid);
				if (nullptr != appoint_session)
				{
					appoint_job_title_end.set_is_to_chief(false);
					appoint_session->send_to_client(&appoint_job_title_end, e_msgindex_s2c_appoint_legion_member_job_title_end);
				}
			}
			break;

		case -1: // 要委任的职位不存在
			break;
		case -2: // 要委任的角色不在军团中
			break;
		case -3: // 要委托的职位已达到上限
			legion_ws_mgr_ref.send_legion_error_to_session(session, e_legion_error_job_title_num_max);
			return;
			break;
		default: // 其他错误
			break;
		}
	}

	void c2ws_req_enter_legion_boss_map(uint32 conn_index, const void* data_ptr, size_t data_len)//停用函数
	{
		return;
		//s_client_uid client_uid;
		//parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		//legion_proto_enter_legion_boss_map req;
		//parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		//client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		//if (nullptr == session)
		//{
		//	return;
		//}
		////跨服状态下不允许
		//if (false == session->is_self_server())
		//{
		//	return;
		//}
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//if (!legion_ws_ptr->is_can_join_LegionBoss_raid(session->get_role_guid()))
		//{
		//	return;
		//}
		//int32 bosstemplateID = globle_data::get_instance().get_legion_boss_temp_id();
		//if (bosstemplateID < 0)
		//{
		//	return;
		//}	
		//guid_64 LegionBoss_map_guid = legion_ws_ptr->get_valid_LegionBoss_map(bosstemplateID);
		//if (false == LegionBoss_map_guid.is_valid())
		//{
		//	cs_map_mgr_system::send_map_error_to_client(session, e_error_code_map_init_map_err);
		//	return;
		//}
		//legion_ws_boss_record_set& legion_boss_record = legion_ws_ptr->get_boss_record_set();
		//if (legion_ws_ptr->get_kill_legion_boss_name() != "")
		//{
		//	return;
		//}
		//s_legion_boss_record* record = legion_boss_record.get_boss_record(bosstemplateID);
		//if (nullptr == record)
		//{
		//	legion_boss_record.add_boss_record(bosstemplateID);
		//}
		//
		//cs_map_mgr_system::transfer_to_map(session, bosstemplateID, LegionBoss_map_guid);
		
	}

	void c2ws_req_get_legion_boss_award(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_get_legion_boss_award req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_get_legion_boss_award_logic(session);
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//// 查看BOSS是否已经被击杀
		//int32 boss_map_id = globle_data::get_instance().get_legion_boss_temp_id();
		////legion_ws_boss_record_set& legion_boss_record = legion_ws_ptr->get_boss_record_set();
		//if (legion_ws_ptr->get_kill_legion_boss_name()=="")
		//{
		//	return;
		//}

		//// 检查角色是否已经领取过该BOSS的奖励
		//legion_ws_boss_award_get_log& boss_award_get_log = legion_ws_mgr::get_instance().get_boss_award_get_log();
		//if (boss_award_get_log.exist_role_legion_boss_award_get_log(session->get_role_guid(), boss_map_id) == true)
		//{
		//	return;
		//}
		//boss_award_get_log.add_role_legion_boss_award_get_log(session->get_role_guid(), boss_map_id);
		//
		//ws2cs_get_legion_boss_award get_legion_boss_award_msg;
		//get_legion_boss_award_msg.role_guid = session->get_role_guid();
		//get_legion_boss_award_msg.boss_map_id = boss_map_id;

		//bool is_use = proto_by_lua(e_msg_index_ws2cs_get_legion_boss_award);
		//if (is_use == false)
		//{
		//	session->send_to_cs(&get_legion_boss_award_msg, sizeof(get_legion_boss_award_msg));
		//}
		//else
		//{
		//	faith::ws2cs_proto::get_legion_boss_award pro_msg;
		//	get_legion_boss_award_msg.to_proto(pro_msg);
		//	session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_get_legion_boss_award);
		//}
	}

	void c2ws_req_get_legion_boss_award_logic(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		// 查看BOSS是否已经被击杀
		int32 boss_map_id = globle_data::get_instance().get_legion_boss_temp_id();
		//legion_ws_boss_record_set& legion_boss_record = legion_ws_ptr->get_boss_record_set();
		if (legion_ws_ptr->get_kill_legion_boss_name() == "")
		{
			return;
		}

		// 检查角色是否已经领取过该BOSS的奖励
		legion_ws_boss_award_get_log& boss_award_get_log = legion_ws_mgr::get_instance().get_boss_award_get_log();
		if (boss_award_get_log.exist_role_legion_boss_award_get_log(session->get_role_guid(), boss_map_id) == true)
		{
			return;
		}
		boss_award_get_log.add_role_legion_boss_award_get_log(session->get_role_guid(), boss_map_id);

		ws2cs_get_legion_boss_award get_legion_boss_award_msg;
		get_legion_boss_award_msg.role_guid = session->get_role_guid();
		get_legion_boss_award_msg.boss_map_id = boss_map_id;

		bool is_use = proto_by_lua(e_msg_index_ws2cs_get_legion_boss_award);
		if (is_use == false)
		{
			session->send_to_cs(&get_legion_boss_award_msg, sizeof(get_legion_boss_award_msg));
		}
		else
		{
			faith::ws2cs_proto::get_legion_boss_award pro_msg;
			get_legion_boss_award_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_get_legion_boss_award);
		}

	}

	void c2ws_req_get_banquet_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_get_banquet_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_get_banquet_info_logic(session);
		//legion_ws_city_war& legion_city_war = legion_ws_mgr::get_instance().get_legion_city_war();
		//guid_64 occupy_legion_guid = legion_city_war.get_overlord_legion();
		//int32 banquet_level = legion_city_war.get_banquet_level();
		//
		//legion_proto_get_banquet_info_end get_banquet_info_end_msg;
		//legion_city_war.fill_get_banquet_info_msg(get_banquet_info_end_msg, session->get_role_guid());
		//session->send_to_client(&get_banquet_info_end_msg, e_msgindex_s2c_get_banquet_info_end);
	}

	void c2ws_req_get_banquet_info_logic(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}

		legion_ws_city_war& legion_city_war = legion_ws_mgr::get_instance().get_legion_city_war();
		guid_64 occupy_legion_guid = legion_city_war.get_overlord_legion();
		int32 banquet_level = legion_city_war.get_banquet_level();

		legion_proto_get_banquet_info_end get_banquet_info_end_msg;
		legion_city_war.fill_get_banquet_info_msg(get_banquet_info_end_msg, session->get_role_guid());
		session->send_to_client(&get_banquet_info_end_msg, e_msgindex_s2c_get_banquet_info_end);
		 
	}

	void c2ws_req_request_hold_banquet(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_request_hold_banquet req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//// 只有军团长才能举办王城舞会
		//if (legion_ws_ptr->get_chief_guid() != session->get_role_guid())
		//{
		//	return;
		//}

		//// 如果申请者的军团不是当前王城的占领军团就不能举行宴会
		//legion_ws_city_war& legion_city_war = legion_ws_mgr::get_instance().get_legion_city_war();
		//if (legion_ws_ptr->get_legion_guid() != legion_city_war.get_overlord_legion())
		//{
		//	return;
		//}
		//if (legion_city_war.is_banquet_holding())
		//{
		//	return;
		//}

		//e_banquet_level banquet_level = (e_banquet_level)(req.banquet_level());
		//if (banquet_level >= e_banquet_level_max)
		//{
		//	return;
		//}
		//e_legion_error err = legion_city_war.request_hold_banquet(banquet_level);
		//if (e_legion_error_none != err)
		//{
		//	legion_ws_mgr::get_instance().send_legion_error_to_session(session, err);
		//	return;
		//}

		//// 通知cs扣去城主的举办花费
		//ws2cs_request_hold_banquet_end request_hold_banquet_end_msg;
		//request_hold_banquet_end_msg.role_guid = session->get_role_guid();
		//request_hold_banquet_end_msg.banquet_level = banquet_level;
		//session->send_to_cs(&request_hold_banquet_end_msg, sizeof(request_hold_banquet_end_msg));

		//// 把成功举办舞会的消息发送给举办人
		//legion_proto_request_hold_banquet_end request_hold_banquet_end_to_c_msg;
		//session->send_to_client(&request_hold_banquet_end_to_c_msg, e_msgindex_s2c_request_hold_banquet_end);
	}
	void c2ws_req_request_hold_banquet_logic(client_session* session, int32	in_banquet_level)
	{
		if (nullptr == session)
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		// 只有军团长才能举办王城舞会
		if (legion_ws_ptr->get_chief_guid() != session->get_role_guid())
		{
			return;
		}

		// 如果申请者的军团不是当前王城的占领军团就不能举行宴会
		legion_ws_city_war& legion_city_war = legion_ws_mgr::get_instance().get_legion_city_war();
		if (legion_ws_ptr->get_legion_guid() != legion_city_war.get_overlord_legion())
		{
			return;
		}
		if (legion_city_war.is_banquet_holding())
		{
			return;
		}

		e_banquet_level banquet_level = (e_banquet_level)(in_banquet_level);
		if (banquet_level >= e_banquet_level_max)
		{
			return;
		}
		e_legion_error err = legion_city_war.request_hold_banquet(banquet_level);
		if (e_legion_error_none != err)
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, err);
			return;
		}

		// 通知cs扣去城主的举办花费
		ws2cs_request_hold_banquet_end request_hold_banquet_end_msg;
		request_hold_banquet_end_msg.role_guid = session->get_role_guid();
		request_hold_banquet_end_msg.banquet_level = banquet_level;
		session->send_to_cs(&request_hold_banquet_end_msg, sizeof(request_hold_banquet_end_msg));

		// 把成功举办舞会的消息发送给举办人
		legion_proto_request_hold_banquet_end request_hold_banquet_end_to_c_msg;
		session->send_to_client(&request_hold_banquet_end_to_c_msg, e_msgindex_s2c_request_hold_banquet_end);

	}

	void c2ws_req_take_part_in_banquet(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_take_part_in_banquet req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws_city_war& legion_city_war = legion_ws_mgr::get_instance().get_legion_city_war();
		if (legion_city_war.is_banquet_holding() == false)
		{
			return;
		}

		legion_city_war.take_part_in_banquet(session);
	}

	void c2ws_req_get_city_war_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_get_city_war_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			city_war_territory_mgr::get_instance().req_cross_server_legion_city_war_info(session->get_role_guid());
			return;
		}

		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();

		//legion_proto_get_city_war_info_end get_city_war_info_end_msg;
		//if (city_war_mgr.fill_get_city_war_info_msg(get_city_war_info_end_msg) == false)
		//{
		//	return;
		//}
		//session->send_to_client(&get_city_war_info_end_msg, e_msgindex_s2c_get_city_war_info_end);
		city_war_mgr.send_city_war_info_all_msg(session);
	}
	//竞价
	void c2ws_req_apply_city_war_bid(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_apply_city_war_bid req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		//跨服状态下不允许申请
		if (false == session->is_self_server())
		{
			return;
		}
		
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

		// 如果申请参加城战的角色没有加入任何军团就不能申请
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			
			return;
		}
		guid_64 legion_guid = legion_ws_ptr->get_legion_guid();

		// 如果申请参加城战的角色不是军团长或者副军团长就不能申请
		if (legion_ws_ptr->is_chief(session->get_role_guid()) == false && false == legion_ws_ptr->is_assistant_chief(session->get_role_guid()))
		{
			return;
		}
		int32 terr_id = req.terr_id();
		int32 in_terr_idx = req.in_terr_idx();
		
		if (!legion_ws_mgr_ref.get_legion_city_war().is_during_bid_time(terr_id))
		{
			return;
		}

		if (false == legion_ws_ptr->is_can_bid())
		{
			return;
		}

		legion_ws_ptr->set_next_bid_time();
		//如果不是gate服且开启了跨服城战
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			//像gate请求验证
			//
			city_war_territory_mgr::get_instance().req_cross_server_apply_city_war_bid(legion_guid,terr_id, in_terr_idx, session->get_role_guid());
			return;
		}

		int32 ret = city_war_territory_mgr::get_instance().bid_one_territory(legion_guid, terr_id, in_terr_idx, session->get_role_guid());
		
		legion_proto_apply_city_war_bid_end apply_city_war_bid_end_msg;
		apply_city_war_bid_end_msg.set_result(ret);
		session->send_to_client(&apply_city_war_bid_end_msg, e_msgindex_s2c_apply_city_war_bid_end);
	}
	//参战
	void c2ws_req_enter_city_war_map(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_entry_city_war_map req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		//跨服状态下不允许申请
		if (false == session->is_self_server())
		{
			return;
		}	

		c2ws_req_enter_city_war_map_logic(session, req.terr_id());
		
		//legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		//legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}
		//guid_64 legin_guid = legion_ws_ptr->get_legion_guid();
		////如果不是gate服且开启了跨服城战
		//if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		//{
		//	//像gate请求验证
		//	city_war_territory_mgr::get_instance().req_cross_server_enter_city_war_map(session->get_role_guid(), req.terr_id(), legin_guid);
		//	return;
		//}
		//int32 terr_id = req.terr_id();

		//// 能够参加城战的只能是竞标成功的军团的成员 
		//legion_ws_city_war& city_war_mgr = legion_ws_mgr_ref.get_legion_city_war();
		//if (city_war_mgr.is_city_war_begin(terr_id) == false)
		//{
		//	// 改为服务器tick中统一创建副本
		//	// city_war_mgr.begin_war(terr_id);
		//	legion_ws_mgr_ref.send_legion_error_to_session(session, e_legion_error_city_war_not_begin);
		//	return;
		//}

		//if (city_war_mgr.exist_bid_info(terr_id, legion_ws_ptr->get_legion_guid()) == false)
		//{
		//	legion_ws_mgr_ref.send_legion_error_to_session(session, e_legion_error_no_city_war_qualification);
		//	return;
		//}

		//guid_64 city_war_map_guid = city_war_mgr.get_city_war_map_guid(terr_id);
		//if (!city_war_map_guid.is_valid())
		//{
		//	session->send_notice("90209394");
		//	return;
		//}

		//ws2cs_enter_city_war_map enter_city_war_map;
		//enter_city_war_map.role_guid = session->get_role_guid();
		//enter_city_war_map.map_guid = city_war_map_guid;
		//enter_city_war_map.war_index = city_war_mgr.get_legion_bid_index(terr_id, legion_ws_ptr->get_legion_guid()) + 1;
		//bool is_use = proto_by_lua(e_msg_index_ws2cs_enter_city_war_map);
		//if (is_use == false)
		//{
		//	session->send_to_cs(&enter_city_war_map, sizeof(enter_city_war_map));
		//}
		//else
		//{
		//	faith::ws2cs_proto::enter_city_war_map pro_msg;
		//	enter_city_war_map.to_proto(pro_msg);
		//	session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_enter_city_war_map);

		//}

		//legion_proto_entry_city_war_map_end entry_city_war_map_end_msg;
		//entry_city_war_map_end_msg.set_terr_id(terr_id);
		//session->send_to_client(&entry_city_war_map_end_msg, e_msgindex_s2c_entry_city_war_map_end);
	}

	void c2ws_req_enter_city_war_map_logic(client_session* session, int32	in_terr_id)
	{
		if (nullptr == session)
		{
			return;
		}

		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		guid_64 legin_guid = legion_ws_ptr->get_legion_guid();
		//如果不是gate服且开启了跨服城战
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			//像gate请求验证
			city_war_territory_mgr::get_instance().req_cross_server_enter_city_war_map(session->get_role_guid(), in_terr_id, legin_guid);
			return;
		}
		int32 terr_id = in_terr_id;

		// 能够参加城战的只能是竞标成功的军团的成员 
		legion_ws_city_war& city_war_mgr = legion_ws_mgr_ref.get_legion_city_war();
		if (city_war_mgr.is_city_war_begin(terr_id) == false)
		{
			// 改为服务器tick中统一创建副本
			// city_war_mgr.begin_war(terr_id);
			legion_ws_mgr_ref.send_legion_error_to_session(session, e_legion_error_city_war_not_begin);
			return;
		}

		if (city_war_mgr.exist_bid_info(terr_id, legion_ws_ptr->get_legion_guid()) == false)
		{
			legion_ws_mgr_ref.send_legion_error_to_session(session, e_legion_error_no_city_war_qualification);
			return;
		}

		guid_64 city_war_map_guid = city_war_mgr.get_city_war_map_guid(terr_id);
		if (!city_war_map_guid.is_valid())
		{
			session->send_notice("90209394");
			return;
		}

		ws2cs_enter_city_war_map enter_city_war_map;
		enter_city_war_map.role_guid = session->get_role_guid();
		enter_city_war_map.map_guid = city_war_map_guid;
		enter_city_war_map.war_index = city_war_mgr.get_legion_bid_index(terr_id, legion_ws_ptr->get_legion_guid()) + 1;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_enter_city_war_map);
		if (is_use == false)
		{
			session->send_to_cs(&enter_city_war_map, sizeof(enter_city_war_map));
		}
		else
		{
			faith::ws2cs_proto::enter_city_war_map pro_msg;
			enter_city_war_map.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_enter_city_war_map);

		}

		legion_proto_entry_city_war_map_end entry_city_war_map_end_msg;
		entry_city_war_map_end_msg.set_terr_id(terr_id);
		session->send_to_client(&entry_city_war_map_end_msg, e_msgindex_s2c_entry_city_war_map_end);
	}

	void c2ws_req_city_war_operate(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_city_war_operate req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		c2ws_req_city_war_operate_logic(session, req.operate_type(), req.prop_npc_guid(), req.terr_id());

		//int32 operate_type = req.operate_type();
		//guid_64 prop_npc_guid(req.prop_npc_guid());
		//int32 terr_id = req.terr_id();
		//if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war) && terr_id != overlord_war_territory_flag)
		//{
		//	//跨服状态下不允许
		//	if (false == session->is_self_server())
		//	{
		//		return;
		//	}
		//	legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

		//	// 角色必须是已经加入了军团的角色
		//	legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		//	if (nullptr == legion_ws_ptr)
		//	{
		//		return;
		//	}

		//	// 只有参加了城战的军团的成员才能继续操作
		//	legion_ws_city_war& city_war_mgr = legion_ws_mgr_ref.get_legion_city_war();
		//	if (city_war_mgr.is_city_war_begin(terr_id) == false)
		//	{
		//		return;
		//	}
		//	if (city_war_mgr.exist_bid_info(terr_id, legion_ws_ptr->get_legion_guid()) == false)
		//	{
		//		return;
		//	}
		//	guid_64 city_war_map_guid = city_war_mgr.get_city_war_map_guid(terr_id);
		//	if (!city_war_map_guid.is_valid())
		//	{
		//		session->send_notice("90209394");
		//		return;
		//	}

		//	ws2cs_city_war_operate city_war_operate_msg;
		//	city_war_operate_msg.city_war_map_guid = city_war_map_guid;
		//	city_war_operate_msg.legion_guid = legion_ws_ptr->get_legion_guid();
		//	city_war_operate_msg.role_guid = session->get_role_guid();
		//	city_war_operate_msg.operate_type = operate_type;
		//	city_war_operate_msg.prop_npc_guid = prop_npc_guid;
		//	bool is_use = proto_by_lua(e_msg_index_ws2cs_city_war_operate);
		//	if (is_use == false)
		//	{
		//		session->send_to_cs(&city_war_operate_msg, sizeof(city_war_operate_msg));
		//	}
		//	else
		//	{
		//		faith::ws2cs_proto::city_war_operate pro_msg;
		//		city_war_operate_msg.to_proto(pro_msg);
		//		session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_city_war_operate);
		//	}
		//}
		//else
		//{
		//	guid_64 legion_guid = session->get_legion_guid();
		//	guid_64 map_guid = guid_64();
		//	if (terr_id == overlord_war_territory_flag)
		//	{
		//		if (cross_server_city_war_ws_mgr::get_instance().get_cur_legion_in_rank(legion_guid) < 0)
		//		{
		//			return;//小于0说明该军团没有资格
		//		}
		//		map_guid = cross_server_city_war_ws_mgr::get_instance().get_map_guid_with_legion_guid(legion_guid);
		//		if (!map_guid.is_valid())
		//		{
		//			return;
		//		}
		//		if (cross_server_city_war_ws_mgr::get_instance().get_map_is_end_with_legion_guid(legion_guid))
		//		{
		//			return;
		//		}
		//	}
		//	else
		//	{
		//		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		//		legion_ws_city_war& city_war_mgr = legion_ws_mgr_ref.get_legion_city_war();
		//		if (terr_id > overlord_war_territory_flag && !city_war_mgr.exist_bid_info(terr_id, legion_guid))
		//		{
		//			return;
		//		}
		//		map_guid = city_war_mgr.get_city_war_map_guid(terr_id);
		//		if (!map_guid.is_valid())
		//		{
		//			return;
		//		}
		//	}

		//	ws2cs_city_war_operate city_war_operate_msg;
		//	city_war_operate_msg.city_war_map_guid = map_guid;
		//	city_war_operate_msg.legion_guid = legion_guid;
		//	city_war_operate_msg.role_guid = session->get_role_guid();
		//	city_war_operate_msg.operate_type = operate_type;
		//	city_war_operate_msg.prop_npc_guid = prop_npc_guid;

		//	bool is_use = proto_by_lua(e_msg_index_ws2cs_city_war_operate);
		//	if (is_use == false)
		//	{
		//		session->send_to_cs(&city_war_operate_msg, sizeof(city_war_operate_msg));
		//	}
		//	else
		//	{
		//		faith::ws2cs_proto::city_war_operate pro_msg;
		//		city_war_operate_msg.to_proto(pro_msg);
		//		session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_city_war_operate);
		//	}
		//	//session->send_to_cs(&city_war_operate_msg, sizeof(city_war_operate_msg));
		//}
	}

	void c2ws_req_city_war_operate_logic(client_session* session, int32	in_operate_type, int64 in_prop_npc_guid, int32	in_terr_id)
	{
		if (session == nullptr)
		{
			return;
		}

		int32 operate_type = in_operate_type;
		guid_64 prop_npc_guid(in_prop_npc_guid);
		int32 terr_id = in_terr_id;
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war) && terr_id != overlord_war_territory_flag)
		{
			//跨服状态下不允许
			if (false == session->is_self_server())
			{
				return;
			}
			legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

			// 角色必须是已经加入了军团的角色
			legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
			if (nullptr == legion_ws_ptr)
			{
				return;
			}

			// 只有参加了城战的军团的成员才能继续操作
			legion_ws_city_war& city_war_mgr = legion_ws_mgr_ref.get_legion_city_war();
			if (city_war_mgr.is_city_war_begin(terr_id) == false)
			{
				return;
			}
			if (city_war_mgr.exist_bid_info(terr_id, legion_ws_ptr->get_legion_guid()) == false)
			{
				return;
			}
			guid_64 city_war_map_guid = city_war_mgr.get_city_war_map_guid(terr_id);
			if (!city_war_map_guid.is_valid())
			{
				session->send_notice("90209394");
				return;
			}

			ws2cs_city_war_operate city_war_operate_msg;
			city_war_operate_msg.city_war_map_guid = city_war_map_guid;
			city_war_operate_msg.legion_guid = legion_ws_ptr->get_legion_guid();
			city_war_operate_msg.role_guid = session->get_role_guid();
			city_war_operate_msg.operate_type = operate_type;
			city_war_operate_msg.prop_npc_guid = prop_npc_guid;
			bool is_use = proto_by_lua(e_msg_index_ws2cs_city_war_operate);
			if (is_use == false)
			{
				session->send_to_cs(&city_war_operate_msg, sizeof(city_war_operate_msg));
			}
			else
			{
				faith::ws2cs_proto::city_war_operate pro_msg;
				city_war_operate_msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_city_war_operate);
			}
		}
		else
		{
			guid_64 legion_guid = session->get_legion_guid();
			guid_64 map_guid = guid_64();
			if (terr_id == overlord_war_territory_flag)
			{
				if (cross_server_city_war_ws_mgr::get_instance().get_cur_legion_in_rank(legion_guid) < 0)
				{
					return;//小于0说明该军团没有资格
				}
				map_guid = cross_server_city_war_ws_mgr::get_instance().get_map_guid_with_legion_guid(legion_guid);
				if (!map_guid.is_valid())
				{
					return;
				}
				if (cross_server_city_war_ws_mgr::get_instance().get_map_is_end_with_legion_guid(legion_guid))
				{
					return;
				}
			}
			else
			{
				legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
				legion_ws_city_war& city_war_mgr = legion_ws_mgr_ref.get_legion_city_war();
				if (terr_id > overlord_war_territory_flag && !city_war_mgr.exist_bid_info(terr_id, legion_guid))
				{
					return;
				}
				map_guid = city_war_mgr.get_city_war_map_guid(terr_id);
				if (!map_guid.is_valid())
				{
					return;
				}
			}

			ws2cs_city_war_operate city_war_operate_msg;
			city_war_operate_msg.city_war_map_guid = map_guid;
			city_war_operate_msg.legion_guid = legion_guid;
			city_war_operate_msg.role_guid = session->get_role_guid();
			city_war_operate_msg.operate_type = operate_type;
			city_war_operate_msg.prop_npc_guid = prop_npc_guid;

			bool is_use = proto_by_lua(e_msg_index_ws2cs_city_war_operate);
			if (is_use == false)
			{
				session->send_to_cs(&city_war_operate_msg, sizeof(city_war_operate_msg));
			}
			else
			{
				faith::ws2cs_proto::city_war_operate pro_msg;
				city_war_operate_msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_city_war_operate);
			}
		}

	}

	void c2ws_req_set_auto_accept_new_legion_member(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_set_auto_accept_new_member_setting req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		
		// 只有军团长才可以设置自动收人的配置
		if (legion_ws_ptr->get_job_title(session->get_role_guid()) != e_legion_job_title_chief)
		{
			return;
		}

		bool is_auto_accept = req.is_auto_accept();
		if (true == is_auto_accept)
		{
			legion_ws_ptr->set_legion_info(ELegionInfo_auto_accept_new_member, 1);
			legion_ws_mgr_ref.add_into_auto_accept_new_member_ordered_legion_list(legion_ws_ptr->get_legion_gs_value(), legion_ws_ptr->get_legion_guid());
		}
		else
		{
			legion_ws_ptr->set_legion_info(ELegionInfo_auto_accept_new_member, 0);
			legion_ws_mgr_ref.del_from_auto_accept_new_member_ordered_legion_list(legion_ws_ptr->get_legion_guid());
		}

		legion_ws_ptr->send_legion_attr_one(ELegionInfo_auto_accept_new_member);
	}

	void c2ws_req_get_city_master_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_get_city_master_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
		guid_64 city_master_guid = city_war_mgr.get_city_master_guid();
		guid_64 occupy_legion_guid = city_war_mgr.get_overlord_legion();

		legion_proto_get_city_master_info_end get_city_master_info_end_msg;
		get_city_master_info_end_msg.set_master_guid(city_master_guid.server_64);
		get_city_master_info_end_msg.set_master_name(city_war_mgr.get_city_master_name());
		get_city_master_info_end_msg.set_legion_guid(occupy_legion_guid.server_64);
		get_city_master_info_end_msg.set_legion_name(city_war_mgr.get_overlord_legion_name());
		session->send_to_client(&get_city_master_info_end_msg, e_msgindex_s2c_get_city_master_info_end);
	}

	void c2ws_req_stimulate_bonus(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_send_stimulate_bonus req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_stimulate_bonus_logic(session, req);
		/*legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		s_legion_member_info* member_info = legion_ws_ptr->get_member(session->get_role_guid());
		if (nullptr == member_info)
		{
			return;
		}
		int64 cur_sec = time_helper::get_cur_time_new().second;
		int64 last_sec = legion_ws_ptr->get_last_stimulated_stamp_by_guid(session->get_role_guid());
		int32 mission_type = req.mission_id();
		xstring member_name = member_info->role_name;
		if (cur_sec - last_sec >= 60)
		{
			legion_ws_ptr->insert_last_stimulated_stamp(session->get_role_guid(), cur_sec);
			int32 temp_id = mission_type + first_legion_bonus_template_id;
			LegionSalaryTemplate* bonus_cfg = GET_TEMPLATE(LegionSalaryTemplate, temp_id);
			if (nullptr == bonus_cfg)
			{
				return;
			}
			const xstring& mission_name = template_manager::get_instance().get_str_by_string_template_id(bonus_cfg->AimTypeName);
			int32 notice_id = legion_stimulate_bonus_notice_id;
			std::vector<std::string> context_text_arr;
			context_text_arr.push_back("90095259");
			context_text_arr.push_back(member_name);
			context_text_arr.push_back(mission_name);
			xstring context_text;
			context_text = init_unit::implode(context_text_arr);

			std::vector<int32> param_arr;
			param_arr.reserve(e_widget_jump_max);
			param_arr.push_back(-1);
			param_arr.push_back(-1);
			param_arr.push_back(92330051);
			param_arr.push_back(-1);
			param_arr.push_back(-1);
			event_ws_mgr::get_instance().send_notice_with_param(session->get_role_guid(), member_info->role_name, context_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000231);
		}*/
	}

	void c2ws_req_stimulate_bonus_logic(client_session * session, legion_proto_send_stimulate_bonus & req)
	{
		if (nullptr == session)
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		s_legion_member_info* member_info = legion_ws_ptr->get_member(session->get_role_guid());
		if (nullptr == member_info)
		{
			return;
		}
		int64 cur_sec = time_helper::get_cur_time_new().second;
		int64 last_sec = legion_ws_ptr->get_last_stimulated_stamp_by_guid(session->get_role_guid());
		int32 mission_type = req.mission_id();
		xstring member_name = member_info->role_name;
		if (cur_sec - last_sec >= 60)
		{
			legion_ws_ptr->insert_last_stimulated_stamp(session->get_role_guid(), cur_sec);
			int32 temp_id = mission_type + first_legion_bonus_template_id;
			LegionSalaryTemplate* bonus_cfg = GET_TEMPLATE(LegionSalaryTemplate, temp_id);
			if (nullptr == bonus_cfg)
			{
				return;
			}
			const xstring& mission_name = template_manager::get_instance().get_str_by_string_template_id(bonus_cfg->AimTypeName);
			int32 notice_id = legion_stimulate_bonus_notice_id;
			std::vector<std::string> context_text_arr;
			context_text_arr.push_back("90095259");
			context_text_arr.push_back(member_name);
			context_text_arr.push_back(mission_name);
			xstring context_text;
			context_text = init_unit::implode(context_text_arr);

			std::vector<int32> param_arr;
			param_arr.reserve(e_widget_jump_max);
			param_arr.push_back(-1);
			param_arr.push_back(-1);
			param_arr.push_back(92330051);
			param_arr.push_back(-1);
			param_arr.push_back(-1);
			event_ws_mgr::get_instance().send_notice_with_param(session->get_role_guid(), member_info->role_name, context_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000231);
		}
	}

	void c2ws_req_stimulate_bonus_lua(client_session * session, const char * msg, int32 msg_len)
	{
		 PROTO_C_WS_COMMON(legion_proto_send_stimulate_bonus, c2ws_req_stimulate_bonus)
	}

	void c2ws_req_get_bonus_rewards(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_get_legion_bonus_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_get_bonus_rewards_logic(session, req);
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}
		//int32 bonus_enum = req.mission_id();
		//if (bonus_enum < e_legion_bonus_type_donate_count || bonus_enum >= e_legion_rank_type_max)
		//{
		//	return;
		//}
		//int32 job_title = legion_ws_ptr->get_job_title(session->get_role_guid());
		//if (job_title >= e_legion_job_title_max)
		//{
		//	return;
		//}
		//int32 sub_type_id = req.sub_type_id();
		//LegionSalaryTemplate*salary_temp_ptr = GET_TEMPLATE(LegionSalaryTemplate, first_legion_bonus_template_id + bonus_enum);
		//if (salary_temp_ptr == nullptr)
		//{
		//	return;
		//}
		//s_legion_bonus_info& bonus_info = legion_ws_ptr->get_bonus_info_ins().get_bonus_one(bonus_enum);

		//std::vector<int32> need_count_arr = salary_temp_ptr->SubTypeId;
		////如果选择获取全部 走这里
		//if (req.get_all() == true)
		//{
		//	ws2cs_get_bonus_award get_bonus_award_msg;

		//	for (int32 i = 0; i < e_legion_bonus_type_max; i++)
		//	{
		//		s_legion_bonus_info& bonus_info = legion_ws_ptr->get_bonus_info_ins().get_bonus_one(i);
		//		get_bonus_award_msg.finish_count[i] = bonus_info.finish_count;//将所有的当前完成次数传给CS
		//	}	
		//	get_bonus_award_msg.role_guid = session->get_role_guid();
		//	get_bonus_award_msg.get_all = req.get_all();
		//	get_bonus_award_msg.legion_job = job_title;

		//	bool is_use = proto_by_lua(e_msg_index_ws2cs_get_bonus_award);
		//	if (is_use == false)
		//	{
		//		session->send_to_cs(&get_bonus_award_msg, sizeof(get_bonus_award_msg));
		//	}
		//	else
		//	{
		//		faith::ws2cs_proto::get_bonus_award pro_msg;
		//		get_bonus_award_msg.to_proto(pro_msg);
		//		session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_get_bonus_award);
		//	}
		//	return;
		//}
		////  否则走这里      判断条件是否达成
		//if (need_count_arr.size() <= sub_type_id||need_count_arr[sub_type_id]>bonus_info.finish_count)
		//{
		//	return;
		//}
		//ws2cs_get_bonus_award get_bonus_award_msg;
		//get_bonus_award_msg.role_guid = session->get_role_guid();
		//get_bonus_award_msg.mission_enum = bonus_enum;
		//get_bonus_award_msg.sub_type_id = sub_type_id;
		//get_bonus_award_msg.legion_job = job_title;
		//get_bonus_award_msg.get_all = req.get_all();

		//bool is_use = proto_by_lua(e_msg_index_ws2cs_get_bonus_award);
		//if (is_use == false)
		//{
		//	session->send_to_cs(&get_bonus_award_msg, sizeof(get_bonus_award_msg));
		//}
		//else
		//{
		//	faith::ws2cs_proto::get_bonus_award pro_msg;
		//	get_bonus_award_msg.to_proto(pro_msg);
		//	session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_get_bonus_award);
		//}
		//session->send_to_cs(&get_bonus_award_msg, sizeof(get_bonus_award_msg));
	}
	void c2ws_req_get_bonus_rewards_logic(client_session * session, legion_proto_get_legion_bonus_info & req)
	{
		if (nullptr == session)
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		int32 bonus_enum = req.mission_id();
		if (bonus_enum < e_legion_bonus_type_donate_count || bonus_enum >= e_legion_rank_type_max)
		{
			return;
		}
		int32 job_title = legion_ws_ptr->get_job_title(session->get_role_guid());
		if (job_title >= e_legion_job_title_max)
		{
			return;
		}
		int32 sub_type_id = req.sub_type_id();
		LegionSalaryTemplate*salary_temp_ptr = GET_TEMPLATE(LegionSalaryTemplate, first_legion_bonus_template_id + bonus_enum);
		if (salary_temp_ptr == nullptr)
		{
			return;
		}
		s_legion_bonus_info& bonus_info = legion_ws_ptr->get_bonus_info_ins().get_bonus_one(bonus_enum);

		std::vector<int32> need_count_arr = salary_temp_ptr->SubTypeId;
		//如果选择获取全部 走这里
		if (req.get_all() == true)
		{
			ws2cs_get_bonus_award get_bonus_award_msg;

			for (int32 i = 0; i < e_legion_bonus_type_max; i++)
			{
				s_legion_bonus_info& bonus_info = legion_ws_ptr->get_bonus_info_ins().get_bonus_one(i);
				get_bonus_award_msg.finish_count[i] = bonus_info.finish_count;//将所有的当前完成次数传给CS
			}
			get_bonus_award_msg.role_guid = session->get_role_guid();
			get_bonus_award_msg.get_all = req.get_all();
			get_bonus_award_msg.legion_job = job_title;

			bool is_use = proto_by_lua(e_msg_index_ws2cs_get_bonus_award);
			if (is_use == false)
			{
				session->send_to_cs(&get_bonus_award_msg, sizeof(get_bonus_award_msg));
			}
			else
			{
				faith::ws2cs_proto::get_bonus_award pro_msg;
				get_bonus_award_msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_get_bonus_award);
			}
			return;
		}
		//  否则走这里      判断条件是否达成
		if (need_count_arr.size() <= sub_type_id || need_count_arr[sub_type_id] > bonus_info.finish_count)
		{
			return;
		}
		ws2cs_get_bonus_award get_bonus_award_msg;
		get_bonus_award_msg.role_guid = session->get_role_guid();
		get_bonus_award_msg.mission_enum = bonus_enum;
		get_bonus_award_msg.sub_type_id = sub_type_id;
		get_bonus_award_msg.legion_job = job_title;
		get_bonus_award_msg.get_all = req.get_all();

		bool is_use = proto_by_lua(e_msg_index_ws2cs_get_bonus_award);
		if (is_use == false)
		{
			session->send_to_cs(&get_bonus_award_msg, sizeof(get_bonus_award_msg));
		}
		else
		{
			faith::ws2cs_proto::get_bonus_award pro_msg;
			get_bonus_award_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_get_bonus_award);
		}
	}
	void c2ws_req_get_bonus_rewards_lua(client_session * session, const char * msg, int32 msg_len)
	{
		 PROTO_C_WS_COMMON(legion_proto_get_legion_bonus_info, c2ws_req_get_bonus_rewards)
	}
	//领奖
	void c2ws_req_get_occupation_daily_award(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_get_occupation_daily_award req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		return;//取消领奖

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		s_legion_member_info* member_info_ptr = legion_ws_ptr->get_member(session->get_role_guid());
		if (nullptr == member_info_ptr)
		{
			return;
		}

		int32 legion_city_war_award_flag_data = member_info_ptr->data_ary[e_legion_member_info_last_get_city_award];
		int32 terr_id = req.terr_id();
		int32 rwd_flag_idx = terr_id - overlord_war_territory_flag + 1;

		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
		// 如果角色所在的军团不是当前占领王城的军团就不能领取每日奖励
		if (terr_id != overlord_war_territory_flag)
		{
			if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
			{
				//像gate请求验证
				city_war_territory_mgr::get_instance().req_get_occupation_daily_award(session->get_role_guid(),legion_ws_ptr->get_legion_guid(),req.terr_id());
				return;
			}
			if (legion_ws_ptr->get_legion_guid() != city_war_mgr.get_occupy_legion_guid(terr_id))
			{
				session->send_notice("90090403");
				return;
			}
		}
		if (terr_id == overlord_war_territory_flag)
		{
			if (legion_ws_ptr->get_legion_guid() != city_war_mgr.get_overlord_legion())
			{
				session->send_notice("90090403");
				return;
			}
		}

		//如果处于城战期间则不能领取奖励
		if (true == city_war_mgr.is_during_city_war_time(terr_id) && terr_id != overlord_war_territory_flag)
		{
			session->send_notice("90202127");
			return;
		}
		if (terr_id == overlord_war_territory_flag && false == legion_ws_mgr::get_instance().get_legion_city_war().get_overlord_legion().is_valid())
		{
			session->send_notice("90202127");
			return;
		}

		//s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(terr_id);
		//if (nullptr == terr_info_ptr)
		//{
		//	return;
		//}
		//s_city_war_bid_info* bid_info_ptr = terr_info_ptr->get_bid_info(territory_occupier_bid_idx);
		//if (nullptr == bid_info_ptr )
		//{
		//	return;
		//}
		//if (!time_helper::is_diff_day(bid_info_ptr->occupy_time_stamp) && terr_id != overlord_war_territory_flag)
		//{
		//	legion_ws_mgr::get_instance().send_legion_error_to_session(session, e_legion_error_next_day_get_occupation_daily_award);
		//	return;
		//}

		if (overlord_war_territory_flag == terr_id)
		{
			int32 legion_master_city_awar_flag = member_info_ptr->data_ary[e_legion_member_info_get_master_city_award];
			if (0 == legion_master_city_awar_flag)
			{
				session->send_notice("90095160");
				return;
			}
			legion_ws_ptr->update_member_info_one(session->get_role_guid(), e_legion_member_info_get_master_city_award, 0);
		}

		else
		{
			int32 is_got_rwd = init_unit::get_flag_data(legion_city_war_award_flag_data, rwd_flag_idx);
			if (1 == is_got_rwd)
			{
				session->send_notice("90095160");
				return;
			}

			init_unit::mark_flag_data(legion_city_war_award_flag_data, rwd_flag_idx);
			legion_ws_ptr->update_member_info_one(session->get_role_guid(), e_legion_member_info_last_get_city_award, legion_city_war_award_flag_data);
		}

		// 通知CS获取奖励
		ws2cs_get_occupation_daily_award get_occupation_daily_award_msg;
		get_occupation_daily_award_msg.role_guid = session->get_role_guid();
		get_occupation_daily_award_msg.territory_id = terr_id;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_get_occupation_daily_award);
		if (is_use == false)
		{
			session->send_to_cs(&get_occupation_daily_award_msg, sizeof(get_occupation_daily_award_msg));
		}
		else
		{
			faith::ws2cs_proto::get_occupation_daily_award pro_msg;
			get_occupation_daily_award_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_get_occupation_daily_award);
		}
	}

	void c2ws_req_get_legion_rank(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_get_legion_rank req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		int32 legion_rank = legion_ws_ptr->get_legion_rank();
		legion_proto_get_legion_rank_end get_legion_rank_end_msg;
		get_legion_rank_end_msg.set_rank(legion_rank);
		session->send_to_client(&get_legion_rank_end_msg, e_msgindex_s2c_get_legion_rank_end);
	}

	void c2ws_req_fire_bonfire(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//s_client_uid client_uid;
		//parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		//legion_proto_create_bonfire_npc req;
		//parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		//client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		//if (nullptr == session)
		//{
		//	return;
		//}
		////跨服状态下不允许
		//if (false == session->is_self_server())
		//{
		//	return;
		//}
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}
		//if (legion_ws_ptr->get_job_title(session->get_role_guid()) < e_legion_job_title_assistant_chief)
		//{
		//	return;
		//}
		//if (!legion_ws_ptr->is_bonfire_valid(true))
		//{
		//	return;
		//}
		//
		//int32 cur_legion_asset_money = legion_ws_ptr->get_legion_info(ELegionInfo_asset_money);
		//ActivityCommonConfigTemplate* activity_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_legion_bonfire);
		//if (nullptr == activity_template_ptr)
		//{
		//	return;
		//}
		//int32 fire_cost_money = activity_template_ptr->ParamInt1;
		//if (cur_legion_asset_money < fire_cost_money)
		//{
		//	return;
		//}

		//if ((time_helper::get_cur_time_new().second - legion_ws_ptr->get_creat_bonfire_cd()) < 10)
		//{
		//	return;
		//}

		//ws2cs_fire_bonfire fire_bonfire_msg;
		//fire_bonfire_msg.role_guid = session->get_role_guid();
		//session->send_to_cs(&fire_bonfire_msg, sizeof(fire_bonfire_msg));
		//legion_ws_ptr->set_creat_bonfire_cd();
	}

	void c2ws_req_add_fuel_to_fire(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_add_fuel_to_fire req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_add_fuel_to_fire_logic(session);
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//
		//guid_64 map_guid = legion_ws_ptr->get_valid_legion_station_map();
		//auto map_ptr = get_entity(map_guid);
		//if (nullptr == map_ptr)
		//{
		//	return;
		//}
		//if (session->get_cs_conn_index() != map_ptr->get_cs_uid())
		//{
		//	return;
		//}
		//
		//ws2cs_bonfire_add_fuel bonfire_add_fuel_msg;
		//bonfire_add_fuel_msg.role_guid = session->get_role_guid();
		//bonfire_add_fuel_msg.bonfire_map_guid = map_guid;
		//bonfire_add_fuel_msg.add_fuel_times = legion_ws_ptr->get_bonfire_add_fuel_times();
		//// world_server::getInstance().send_by_uid(map_ptr->get_cs_uid(), &bonfire_add_fuel_msg, sizeof(bonfire_add_fuel_msg));
		//bool is_use = proto_by_lua(e_msg_index_ws2cs_add_fuel_to_bonfire);
		//if (is_use == false)
		//{
		//	session->send_to_cs(&bonfire_add_fuel_msg, sizeof(bonfire_add_fuel_msg));
		//}
		//else
		//{
		//	faith::ws2cs_proto::bonfire_add_fuel pro_msg;
		//	bonfire_add_fuel_msg.to_proto(pro_msg);
		//	session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_add_fuel_to_bonfire);
		//}
	}
	void c2ws_req_add_fuel_to_fire_logic(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}


		guid_64 map_guid = legion_ws_ptr->get_valid_legion_station_map();
		auto map_ent = get_entity(map_guid);
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("map_ent is nullptr map_guid:{}", map_guid.server_64);
			return;
		}
		auto map_cp = map_ent->get_component<cs_map_component>();
		if (session->get_cs_conn_index() != map_cp->m_cs_uid)
		{
			CONSOLE_ERROR("session->get_cs_conn_index() != map_cp->m_cs_uid session->get_cs_conn_index:{} map_cp->m_cs_uid:{}", session->get_cs_conn_index(), map_cp->m_cs_uid);
			return;
		}

		ws2cs_bonfire_add_fuel bonfire_add_fuel_msg;
		bonfire_add_fuel_msg.role_guid = session->get_role_guid();
		bonfire_add_fuel_msg.bonfire_map_guid = map_guid;
		bonfire_add_fuel_msg.add_fuel_times = legion_ws_ptr->get_bonfire_add_fuel_times();
		// world_server::getInstance().send_by_uid(map_ptr->get_cs_uid(), &bonfire_add_fuel_msg, sizeof(bonfire_add_fuel_msg));
		bool is_use = proto_by_lua(e_msg_index_ws2cs_add_fuel_to_bonfire);
		if (is_use == false)
		{
			session->send_to_cs(&bonfire_add_fuel_msg, sizeof(bonfire_add_fuel_msg));
		}
		else
		{
			faith::ws2cs_proto::bonfire_add_fuel pro_msg;
			bonfire_add_fuel_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_add_fuel_to_bonfire);
		}


	}
	
	void c2ws_req_send_legion_recruit_in_world(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_recruit_in_world req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_send_legion_recruit_in_world_logic(session, req.is_rm_cd());
		/*	legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
			if (nullptr == legion_ws_ptr)
			{
				return;
			}
			const LegionConstructionsTemplate* legion_tmpl_ptr = legion_ws_ptr->get_legion_cfg();
			if (nullptr == legion_tmpl_ptr)
			{
				return;
			}

			if (req.is_rm_cd() && !legion_ws_ptr->can_world_recruit_now())
			{
				ws2cs_legion_remove_recruit_cd rm_recruit_cd_msg;
				rm_recruit_cd_msg.role_guid = session->get_role_guid();
				bool is_use = proto_by_lua(e_msg_index_ws2cs_legion_rm_recruit_cd);
				if (is_use == false)
				{
					session->send_to_cs(&rm_recruit_cd_msg, sizeof(rm_recruit_cd_msg));
				}
				else
				{
					faith::ws2cs_proto::legion_remove_recruit_cd pro_msg;
					rm_recruit_cd_msg.to_proto(pro_msg);
					session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_legion_rm_recruit_cd);
				}
				return;
			}

			legion_ws_ptr->req_send_legion_recruit_in_world(session->get_role_guid());
			legion_proto_recruit_in_world_end recruit_in_world_end_msg;
			int32 next_recruit_stamp_2c = legion_ws_ptr->get_last_recruit_in_world_stamp() / second_tick_time + legion_tmpl_ptr->RecruitInWorldCD;
			recruit_in_world_end_msg.set_next_recruit_stamp(next_recruit_stamp_2c);
			session->send_to_client(&recruit_in_world_end_msg, e_msgindex_s2c_legion_recruit_in_world_end);*/
	}
	void c2ws_req_send_legion_recruit_in_world_logic(client_session* session, bool in_is_rm_cd)
	{
		if (nullptr == session)
		{
			return;
		}


		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		const LegionConstructionsTemplate* legion_tmpl_ptr = legion_ws_ptr->get_legion_cfg();
		if (nullptr == legion_tmpl_ptr)
		{
			return;
		}

		if (in_is_rm_cd && !legion_ws_ptr->can_world_recruit_now())
		{
			ws2cs_legion_remove_recruit_cd rm_recruit_cd_msg;
			rm_recruit_cd_msg.role_guid = session->get_role_guid();
			bool is_use = proto_by_lua(e_msg_index_ws2cs_legion_rm_recruit_cd);
			if (is_use == false)
			{
				session->send_to_cs(&rm_recruit_cd_msg, sizeof(rm_recruit_cd_msg));
			}
			else
			{
				faith::ws2cs_proto::legion_remove_recruit_cd pro_msg;
				rm_recruit_cd_msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_legion_rm_recruit_cd);
			}
			return;
		}

		legion_ws_ptr->req_send_legion_recruit_in_world(session->get_role_guid());
		legion_proto_recruit_in_world_end recruit_in_world_end_msg;
		int32 next_recruit_stamp_2c = legion_ws_ptr->get_last_recruit_in_world_stamp() / second_tick_time + legion_tmpl_ptr->RecruitInWorldCD;
		recruit_in_world_end_msg.set_next_recruit_stamp(next_recruit_stamp_2c);
		session->send_to_client(&recruit_in_world_end_msg, e_msgindex_s2c_legion_recruit_in_world_end);

	}

	void c2ws_req_get_city_war_terr_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_req_territory_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

		// 如果申请参加城战的角色没有加入任何军团就不能申请
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		guid_64 legion_guid = legion_ws_ptr->get_legion_guid();

		int32 terr_id = req.terr_id();
		
		legion_ws_mgr_ref.get_legion_city_war().send_territory_info_one(session, terr_id);
	}

	void c2ws_req_get_city_war_legion_rank_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_req_territory_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_get_city_war_legion_rank_info_logic(session);
		//legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

		//legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//legion_proto_req_city_war_rank_info_end req_rank_infos_msg;
		//legion_ws_mgr_ref.get_legion_city_war().fill_rank_info_msg(req_rank_infos_msg);
		//session->send_to_client(&req_rank_infos_msg, e_msgindex_s2c_get_city_war_rank_info_end);
	}

	void  c2ws_req_get_city_war_legion_rank_info_logic(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}

		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		legion_proto_req_city_war_rank_info_end req_rank_infos_msg;
		legion_ws_mgr_ref.get_legion_city_war().fill_rank_info_msg(req_rank_infos_msg);
		session->send_to_client(&req_rank_infos_msg, e_msgindex_s2c_get_city_war_rank_info_end);
	}

	//优先维护
	void c2ws_req_set_territory_prior_maintain(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_set_territory_prior_maintain req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		if (legion_ws_ptr->is_chief(session->get_role_guid()) == false)
		{
			return;
		}
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			int array_length = req.terr_ids_size();
			if (array_length > cross_server_territory_city_max) {
				array_length = cross_server_territory_city_max;
			}
			int32 territoryIds[cross_server_territory_city_max] = {0,0,0,0,0};
			for (int32 idx = 0; idx < array_length; idx++)
			{
				int32 terr_id = req.terr_ids(idx);
				territoryIds[idx] = terr_id;
			}
			//像gate请求验证
			city_war_territory_mgr::get_instance().req_cross_server_territory_prior_maintain(legion_ws_ptr->get_legion_guid(), territoryIds, session->get_role_guid(), array_length);
			return;
		}
		legion_proto_set_territory_prior_maintain_end set_territory_prior_maintain_end_msg;

		guid_64 legion_guid = legion_ws_ptr->get_legion_guid();
		for (int32 idx = 0; idx < req.terr_ids_size(); idx++)
		{
			int32 terr_id = req.terr_ids(idx);
			bool prior_ret = city_war_territory_mgr::get_instance().set_is_prior_maintain(legion_guid, terr_id);
			set_territory_prior_maintain_end_msg.add_terr_ids(terr_id);
			set_territory_prior_maintain_end_msg.add_is_priors(prior_ret);
		}
		session->send_to_client(&set_territory_prior_maintain_end_msg, e_msgindex_s2c_set_territory_prior_maintain_end);
	}

	void c2ws_req_invite_player_to_join_legion(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_invite_player req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		client_session* new_member_session = nullptr;
		guid_64 invite_guid;
		if (req.has_invite_guid())
		{
			invite_guid = req.invite_guid();
			if (invite_guid.is_valid() == false)
			{
				return;
			}
			new_member_session = client_session_mgr::getInstance().get_session(invite_guid);
		}
		else if (req.has_new_member_name())
		{
			const std::string new_member_name = req.new_member_name();
			int32 name_len = new_member_name.length();
			if (name_len <= 0 || name_len > max_name_size)
			{
				return;
			}
			new_member_session = client_session_mgr::getInstance().get_session_by_role_name(new_member_name);
		}

		if (nullptr == new_member_session)
		{
			legion_ws_mgr_ref.send_legion_error_to_session(session, e_legion_error_player_added_not_online);
			return;
		}
		invite_guid = new_member_session->get_role_guid();

		// 如果要添加的玩家已经加入军团就不能再添加他
		if (legion_ws_mgr_ref.get_unit_legion(invite_guid) != nullptr)
		{
			legion_ws_mgr_ref.send_legion_error_to_session(session, e_legion_error_player_added_is_already_join_other_legion);
			return;
		}

		legion_proto_invite_player_end invite_player_to_join_legion_end_msg;
		legion_proto_legion_brief_info* legion_brief_info = invite_player_to_join_legion_end_msg.mutable_invite_legion_info();;
		if (nullptr == legion_brief_info)
		{
			return;
		}
		invite_player_to_join_legion_end_msg.set_invite_role_guid(req.role_guid());
		invite_player_to_join_legion_end_msg.set_invite_role_name(session->m_role_info.role_name);
		invite_player_to_join_legion_end_msg.set_invite_role_level(session->m_role_info.data_ary[e_role_info_exp_level]);
		legion_ws_ptr->fill_legion_brief_info_msg(*legion_brief_info);
		new_member_session->send_to_client(&invite_player_to_join_legion_end_msg, e_msgindex_s2c_invite_player_to_join_legion_end);

		legion_proto_invite_player_succeed invite_player_to_join_legion_succeed_msg;
		session->send_to_client(&invite_player_to_join_legion_succeed_msg, e_msgindex_s2c_invite_player_succeed);
	}

	void c2ws_req_draw_bonfire_daily_reward(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_draw_bonfire_daily_award req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_draw_bonfire_daily_reward_logic(session);
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}
		//guid_64 map_guid = legion_ws_ptr->get_valid_legion_station_map();
		//auto map_ptr = get_entity(map_guid);
		//if (nullptr == map_ptr)
		//{
		//	return;
		//}
		//if (session->get_cs_conn_index() != map_ptr->get_cs_uid())
		//{
		//	return;
		//}
		//
		//ws2cs_draw_bonfire_daily_rwd bonfire_daily_rwd_msg;
		//bonfire_daily_rwd_msg.role_guid = session->get_role_guid();
		//bonfire_daily_rwd_msg.bonfire_map_guid = map_guid;
		////world_server::getInstance().send_by_uid(map_ptr->get_cs_uid(), &bonfire_add_fuel_msg, sizeof(bonfire_add_fuel_msg));
		//bool is_use = proto_by_lua(e_msg_index_ws2cs_draw_bonfire_daily_rwd);
		//if (is_use == false)
		//{
		//	session->send_to_cs(&bonfire_daily_rwd_msg, sizeof(bonfire_daily_rwd_msg));
		//}
		//else
		//{
		//	faith::ws2cs_proto::draw_bonfire_daily_rwd pro_msg;
		//	bonfire_daily_rwd_msg.to_proto(pro_msg);
		//	session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_draw_bonfire_daily_rwd);
		//}
	}

	void c2ws_req_draw_bonfire_daily_reward_logic(client_session * session)
	{
		if (nullptr == session)
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		guid_64 map_guid = legion_ws_ptr->get_valid_legion_station_map();
		auto map_ent = get_entity(map_guid);
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("map_ent is nullptr map_guid:{}", map_guid.server_64);
			return;
		}
		auto map_cp = map_ent->get_component<cs_map_component>();
		if (session->get_cs_conn_index() != map_cp->m_cs_uid)
		{
			CONSOLE_ERROR("session->get_cs_conn_index() != map_cp->m_cs_uid session->get_cs_conn_index:{} map_cp->m_cs_uid:{}", session->get_cs_conn_index(), map_cp->m_cs_uid);
			return;
		}

		ws2cs_draw_bonfire_daily_rwd bonfire_daily_rwd_msg;
		bonfire_daily_rwd_msg.role_guid = session->get_role_guid();
		bonfire_daily_rwd_msg.bonfire_map_guid = map_guid;
		//world_server::getInstance().send_by_uid(map_ptr->get_cs_uid(), &bonfire_add_fuel_msg, sizeof(bonfire_add_fuel_msg));
		bool is_use = proto_by_lua(e_msg_index_ws2cs_draw_bonfire_daily_rwd);
		if (is_use == false)
		{
			session->send_to_cs(&bonfire_daily_rwd_msg, sizeof(bonfire_daily_rwd_msg));
		}
		else
		{
			faith::ws2cs_proto::draw_bonfire_daily_rwd pro_msg;
			bonfire_daily_rwd_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_draw_bonfire_daily_rwd);
		}
	}
	//召集
	void c2ws_req_legion_call_operate(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_legion_call_operate req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		guid_64 operate_role_guid(req.role_guid());

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		c2ws_req_legion_call_operate_logic(session, req);
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//const s_legion_member_info* legion_member_info_ptr = legion_ws_ptr->get_member(operate_role_guid);
		//if (nullptr == legion_member_info_ptr)
		//{
		//	return;
		//}

		//e_legion_call_type operate_type = (e_legion_call_type)req.call_type();

		//std::vector<std::string> content_text_arr;
		//std::vector<int32> param_arr;
		//param_arr.reserve(e_widget_jump_max);
		//std::string content_text;

		//switch (operate_type)
		//{
		//case faith::e_legion_call_type_call_legion_boss:
		//{
		//	if (legion_ws_ptr->get_job_title(operate_role_guid) < e_legion_job_title_assistant_chief)
		//	{
		//		return;
		//	}
		//	MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, req.operate_params(0));
		//	if (nullptr == map_template_ptr)
		//	{
		//		return;
		//	}

		//	content_text_arr.push_back("90300093");
		//	content_text_arr.push_back(init_unit::change_i64_to_string(map_template_ptr->MapName));
		//	content_text = init_unit::implode(content_text_arr);

		//	param_arr.push_back(-1);
		//	param_arr.push_back(-1);
		//	param_arr.push_back(92330007);
		//	int32 boss_index = req.operate_params(0) - legion_start_legion_boss_map_id;
		//	param_arr.push_back(boss_index);
		//	param_arr.push_back(-1);

		//	event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000140);
		//	break;
		//}
		//case faith::e_legion_call_type_call_city_war: 
		//{
		//	if (legion_ws_ptr->get_job_title(operate_role_guid) < e_legion_job_title_assistant_chief)
		//	{
		//		return;
		//	}
		//	if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		//	{
		//		content_text_arr.push_back("90203702");
		//	}
		//	else
		//	{
		//		content_text_arr.push_back("90300097");
		//	}
		//	
		//	content_text = init_unit::implode(content_text_arr);

		//	param_arr.push_back(-1);
		//	param_arr.push_back(-1);
		//	param_arr.push_back(92330038);
		//	param_arr.push_back(-1);
		//	param_arr.push_back(-1);

		//	event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000144);
		//	break;
		//}
		//case faith::e_legion_call_type_call_bonfire:
		//{
		//	if (legion_ws_ptr->get_job_title(operate_role_guid) < e_legion_job_title_assistant_chief)
		//	{
		//		return;
		//	}
		//	content_text_arr.push_back("90300096");
		//	content_text = init_unit::implode(content_text_arr);

		//	param_arr.push_back(-1);
		//	param_arr.push_back(-1);
		//	param_arr.push_back(92330036);
		//	param_arr.push_back(-1);
		//	param_arr.push_back(-1);

		//	event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000143);
		//	break;
		//}
		//case faith::e_legion_call_type_call_elite_boss:
		//{
		//	NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, req.operate_params(0));
		//	if (npc_spawn_point_template_ptr == nullptr)
		//	{
		//		return;
		//	}
		//	NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, req.operate_params(1));
		//	if (npc_template_ptr == nullptr)
		//	{
		//		return;
		//	}

		//	content_text_arr.push_back("90300094");
		//	content_text_arr.push_back(init_unit::change_i64_to_string(npc_template_ptr->NpcName));
		//	content_text = init_unit::implode(content_text_arr);

		//	param_arr.push_back(faith::world_boss_line);
		//	param_arr.push_back(npc_spawn_point_template_ptr->MapId);
		//	param_arr.push_back(npc_spawn_point_template_ptr->PosX);
		//	param_arr.push_back(npc_spawn_point_template_ptr->PosY);
		//	param_arr.push_back(npc_spawn_point_template_ptr->PosZ);
		//	event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_go_to_location, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000141);
		//	break;
		//}
		//case faith::e_legion_call_type_call_world_boss:
		//{
		//	if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss) && (cross_server_world_boss_ws_mgr::get_instance().is_in_game_time() || cross_server_world_boss_ws_mgr::get_instance().is_in_game_ready_time()))
		//	{
		//		if (legion_ws_ptr->get_job_title(operate_role_guid) < e_legion_job_title_assistant_chief)
		//		{
		//			return;
		//		}
		//		content_text_arr.push_back("90203728");
		//		content_text = init_unit::implode(content_text_arr);

		//		param_arr.push_back(-1);
		//		param_arr.push_back(-1);
		//		param_arr.push_back(92810013);
		//		int32 boss_index = req.operate_params(0);
		//		param_arr.push_back(boss_index);
		//		param_arr.push_back(-1);

		//		event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000290);
		//		break;
		//	}
		//	else
		//	{

		//		NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, req.operate_params(0));
		//		if (npc_spawn_point_template_ptr == nullptr)
		//		{
		//			return;
		//		}
		//		NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, req.operate_params(1));
		//		if (npc_template_ptr == nullptr)
		//		{
		//			return;
		//		}
		//		content_text_arr.push_back("90300095");
		//		content_text_arr.push_back(init_unit::change_i64_to_string(npc_template_ptr->NpcName));
		//		content_text = init_unit::implode(content_text_arr);

		//		param_arr.push_back(faith::world_boss_line);
		//		param_arr.push_back(npc_spawn_point_template_ptr->MapId);
		//		param_arr.push_back(npc_spawn_point_template_ptr->PosX);
		//		param_arr.push_back(npc_spawn_point_template_ptr->PosY);
		//		param_arr.push_back(npc_spawn_point_template_ptr->PosZ);
		//		event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_go_to_location, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000142);
		//		break;
		//	}
		//}
		//case faith::e_legion_call_type_max:
		//	break;
		//default:
		//	break;
		//}

		//legion_proto_legion_call_operate_end legion_call_operate_end_msg;
		//legion_call_operate_end_msg.set_role_guid(operate_role_guid.server_64);
		//legion_call_operate_end_msg.set_call_type(operate_type);

		//int32 paramas_size = req.operate_params_size();

		//for (int32 i = 0; i < paramas_size; i++)
		//{
		//	legion_call_operate_end_msg.add_operate_params(req.operate_params(i));
		//}

		//legion_ws_ptr->send_message_to_all_member(&legion_call_operate_end_msg, e_msgindex_s2c_legion_call_operate_end);
	}

	void c2ws_req_legion_call_operate_logic(client_session * session, legion_proto_legion_call_operate & req)
	{
		guid_64 operate_role_guid(req.role_guid());
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		const s_legion_member_info* legion_member_info_ptr = legion_ws_ptr->get_member(operate_role_guid);
		if (nullptr == legion_member_info_ptr)
		{
			return;
		}

		e_legion_call_type operate_type = (e_legion_call_type)req.call_type();

		std::vector<std::string> content_text_arr;
		std::vector<int32> param_arr;
		param_arr.reserve(e_widget_jump_max);
		std::string content_text;

		switch (operate_type)
		{
		case faith::e_legion_call_type_call_legion_boss:
		{
			if (legion_ws_ptr->get_job_title(operate_role_guid) < e_legion_job_title_assistant_chief)
			{
				return;
			}
			MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, req.operate_params(0));
			if (nullptr == map_template_ptr)
			{
				return;
			}

			content_text_arr.push_back("90300093");
			content_text_arr.push_back(init_unit::change_i64_to_string(map_template_ptr->MapName));
			content_text = init_unit::implode(content_text_arr);

			param_arr.push_back(-1);
			param_arr.push_back(-1);
			param_arr.push_back(92330007);
			int32 boss_index = req.operate_params(0) - legion_start_legion_boss_map_id;
			param_arr.push_back(boss_index);
			param_arr.push_back(-1);

			event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000140);
			break;
		}
		case faith::e_legion_call_type_call_city_war:
		{
			if (legion_ws_ptr->get_job_title(operate_role_guid) < e_legion_job_title_assistant_chief)
			{
				return;
			}
			if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
			{
				content_text_arr.push_back("90203702");
			}
			else
			{
				content_text_arr.push_back("90300097");
			}

			content_text = init_unit::implode(content_text_arr);

			param_arr.push_back(-1);
			param_arr.push_back(-1);
			param_arr.push_back(92330038);
			param_arr.push_back(-1);
			param_arr.push_back(-1);

			event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000144);
			break;
		}
		case faith::e_legion_call_type_call_bonfire:
		{
			if (legion_ws_ptr->get_job_title(operate_role_guid) < e_legion_job_title_assistant_chief)
			{
				return;
			}
			content_text_arr.push_back("90300096");
			content_text = init_unit::implode(content_text_arr);

			param_arr.push_back(-1);
			param_arr.push_back(-1);
			param_arr.push_back(92330036);
			param_arr.push_back(-1);
			param_arr.push_back(-1);

			event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000143);
			break;
		}
		case faith::e_legion_call_type_call_elite_boss:
		{
			NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, req.operate_params(0));
			if (npc_spawn_point_template_ptr == nullptr)
			{
				return;
			}
			NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, req.operate_params(1));
			if (npc_template_ptr == nullptr)
			{
				return;
			}

			content_text_arr.push_back("90300094");
			content_text_arr.push_back(init_unit::change_i64_to_string(npc_template_ptr->NpcName));
			content_text = init_unit::implode(content_text_arr);

			param_arr.push_back(faith::world_boss_line);
			param_arr.push_back(npc_spawn_point_template_ptr->MapId);
			param_arr.push_back(npc_spawn_point_template_ptr->PosX);
			param_arr.push_back(npc_spawn_point_template_ptr->PosY);
			param_arr.push_back(npc_spawn_point_template_ptr->PosZ);
			event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_go_to_location, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000141);
			break;
		}
		case faith::e_legion_call_type_call_world_boss:
		{
			if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss) && (cross_server_world_boss_ws_mgr::get_instance().is_in_game_time() || cross_server_world_boss_ws_mgr::get_instance().is_in_game_ready_time()))
			{
				if (legion_ws_ptr->get_job_title(operate_role_guid) < e_legion_job_title_assistant_chief)
				{
					return;
				}
				content_text_arr.push_back("90203728");
				content_text = init_unit::implode(content_text_arr);

				param_arr.push_back(-1);
				param_arr.push_back(-1);
				param_arr.push_back(92810013);
				int32 boss_index = req.operate_params(0);
				param_arr.push_back(boss_index);
				param_arr.push_back(-1);

				event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000290);
				break;
			}
			else
			{

				NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, req.operate_params(0));
				if (npc_spawn_point_template_ptr == nullptr)
				{
					return;
				}
				NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_spawn_point_template_ptr->TemplateId);
				if (npc_template_ptr == nullptr)
				{
					return;
				}
				if (GAMECONFIG->WorldBossRandomPosArr.size() < 54)
				{
					return;
				}
				if (req.operate_params(1) * 6 + 5 >= 54)
				{
					return;
				}

				content_text_arr.push_back("90300095");
				content_text_arr.push_back(init_unit::change_i64_to_string(npc_template_ptr->NpcName));
				content_text = init_unit::implode(content_text_arr);

				param_arr.push_back(faith::world_boss_line);
				param_arr.push_back(npc_spawn_point_template_ptr->MapId);
				param_arr.push_back(GAMECONFIG->WorldBossRandomPosArr[req.operate_params(1) * 6]);
				param_arr.push_back(GAMECONFIG->WorldBossRandomPosArr[req.operate_params(1) * 6 + 1]);
				param_arr.push_back(GAMECONFIG->WorldBossRandomPosArr[req.operate_params(1) * 6 + 2]);
				event_ws_mgr::get_instance().send_notice_with_param(operate_role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_go_to_location, e_chat_type_legion, session->get_role_info_data(e_role_info_template_id), 93000142);
				break;
			}
		}
		case faith::e_legion_call_type_max:
			break;
		default:
			break;
		}

		legion_proto_legion_call_operate_end legion_call_operate_end_msg;
		legion_call_operate_end_msg.set_role_guid(operate_role_guid.server_64);
		legion_call_operate_end_msg.set_call_type(operate_type);

		int32 paramas_size = req.operate_params_size();

		for (int32 i = 0; i < paramas_size; i++)
		{
			legion_call_operate_end_msg.add_operate_params(req.operate_params(i));
		}

		legion_ws_ptr->send_message_to_all_member(&legion_call_operate_end_msg, e_msgindex_s2c_legion_call_operate_end);
	}

	void c2ws_req_legion_call_operate_lua(client_session * session, const char * msg, int32 msg_len)
	{

		PROTO_C_WS_COMMON(legion_proto_legion_call_operate, c2ws_req_legion_call_operate)

	}

	void c2ws_req_level_up_legion_skill(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_level_up_legion_skill req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(req.role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		legion_ws_ptr->level_up_legion_skill(req.role_guid(), req.skill_id());
	}

	void c2ws_req_transfer_to_bonfire_map(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_transfer_to_pk_king_map req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		legion_ws_ptr->join_legion_station_map(session->get_role_guid());
	}
	void c2ws_req_goods_operate(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		int32 back_string = 0;
		goods_proto_goods_operate req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session = client_session_mgr::getInstance().get_session(req.role_guid());
		if (nullptr == session)
		{
			return;
		}
		c2ws_req_goods_operate_logic(session, req);
		//StoreTemplate* temp_store_ptr = template_manager::get_instance().get_store_template_by_id(req.store_id());
		//if (temp_store_ptr == nullptr)
		//{
		//	return;
		//}

		//ws2cs_buy_good buy_good;
		//buy_good.role_guid = session->get_role_guid();
		//buy_good.goods_id = req.goods_id();
		//buy_good.goods_num = req.goods_num();
		//buy_good.store_id = req.store_id();
		//buy_good.is_auto_buy = req.is_auto_buy();
		//buy_good.back_string = back_string;

		//bool is_legion_gooed = false;
		//if (temp_store_ptr->StoreType == e_store_type_legion
		//	|| temp_store_ptr->StoreType == e_store_type_sprite_third
		//	|| temp_store_ptr->StoreType == e_store_type_magic_crystal_first)
		//{
		//	is_legion_gooed = true;
		//}
		//if (is_legion_gooed)
		//{
		//	GoodsTemplate* temp_goods_ptr = GET_TEMPLATE(GoodsTemplate, req.goods_id());
		//	if (temp_goods_ptr == nullptr)
		//	{
		//		return;
		//	}
		//	legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		//	if (nullptr == legion_ws_ptr)
		//	{
		//		back_string = e_buy_goods_end_role_not_in_legion;
		//	}
		//	else
		//	{
		//		guid_64 legion_guid = legion_ws_ptr->get_legion_guid();
		//		if (false == legion_guid.is_valid())
		//		{
		//			back_string = e_buy_goods_end_role_not_in_legion;
		//		}
		//		else
		//		{
		//			// 如果购买改物品需要军团等级达到一定的等级才行,就在这里判断当前军团的等级是否足够
		//			if (temp_goods_ptr->NeedLegionLevel > 0)
		//			{
		//				int32 legion_level = legion_ws_ptr->get_legion_level();
		//				if (legion_level < temp_goods_ptr->NeedLegionLevel)
		//				{
		//					back_string = e_buy_goods_end_legion_level_limit;
		//				}
		//			}
		//			//如果需要军团贡献度，在这里判断是否足够
		//			int64 contribution = legion_ws_ptr->get_member(session->get_role_guid())->data_ary[e_legion_member_info_donate_contribution];
		//			if (contribution < temp_goods_ptr->NeedContribution)
		//			{
		//				back_string = e_buy_goods_end_need_contribution_not_enough;
		//			}
		//		}
		//	}
		//	buy_good.back_string = back_string;
		//	//session->send_to_cs(&buy_good, sizeof(buy_good));
		//} 
		////else
		////{
		////	session->send_to_cs(&buy_good, sizeof(buy_good));
		////}
		// bool is_use = proto_by_lua(e_msgindex_cs2ws_buy_good);
		// if (is_use == false)
		// {
		//	 session->send_to_cs(&buy_good, sizeof(buy_good));
		// }
		// else
		// {
		//	 faith::ws2cs_proto::buy_good pro_msg;
		//	 buy_good.to_proto(pro_msg);
		//	 session->send_to_cs_lua(&pro_msg, e_msgindex_cs2ws_buy_good);
		// }
	}

	void c2ws_req_goods_operate_logic(client_session * session, goods_proto_goods_operate & req)
	{
		if (nullptr == session)
		{
			return;
		}

		int32 back_string = 0;

		StoreTemplate* temp_store_ptr = template_manager::get_instance().get_store_template_by_id(req.store_id());
		if (temp_store_ptr == nullptr)
		{
			return;
		}

		ws2cs_buy_good buy_good;
		buy_good.role_guid = session->get_role_guid();
		buy_good.goods_id = req.goods_id();
		buy_good.goods_num = req.goods_num();
		buy_good.store_id = req.store_id();
		buy_good.is_auto_buy = req.is_auto_buy();
		buy_good.back_string = back_string;

		bool is_legion_gooed = false;
		if (temp_store_ptr->StoreType == e_store_type_legion
			|| temp_store_ptr->StoreType == e_store_type_sprite_third
			|| temp_store_ptr->StoreType == e_store_type_magic_crystal_first)
		{
			is_legion_gooed = true;
		}
		if (is_legion_gooed)
		{
			GoodsTemplate* temp_goods_ptr = GET_TEMPLATE(GoodsTemplate, req.goods_id());
			if (temp_goods_ptr == nullptr)
			{
				return;
			}
			legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
			if (nullptr == legion_ws_ptr)
			{
				back_string = e_buy_goods_end_role_not_in_legion;
			}
			else
			{
				guid_64 legion_guid = legion_ws_ptr->get_legion_guid();
				if (false == legion_guid.is_valid())
				{
					back_string = e_buy_goods_end_role_not_in_legion;
				}
				else
				{
					// 如果购买改物品需要军团等级达到一定的等级才行,就在这里判断当前军团的等级是否足够
					if (temp_goods_ptr->NeedLegionLevel > 0)
					{
						int32 legion_level = legion_ws_ptr->get_legion_level();
						if (legion_level < temp_goods_ptr->NeedLegionLevel)
						{
							back_string = e_buy_goods_end_legion_level_limit;
						}
					}
					//如果需要军团贡献度，在这里判断是否足够
					int64 contribution = legion_ws_ptr->get_member(session->get_role_guid())->data_ary[e_legion_member_info_donate_contribution];
					if (contribution < temp_goods_ptr->NeedContribution)
					{
						back_string = e_buy_goods_end_need_contribution_not_enough;
					}
				}
			}
			buy_good.back_string = back_string;
			//session->send_to_cs(&buy_good, sizeof(buy_good));
		}
		//else
		//{
		//	session->send_to_cs(&buy_good, sizeof(buy_good));
		//}
		bool is_use = proto_by_lua(e_msgindex_cs2ws_buy_good);
		if (is_use == false)
		{
			session->send_to_cs(&buy_good, sizeof(buy_good));
		}
		else
		{
			faith::ws2cs_proto::buy_good pro_msg;
			buy_good.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msgindex_cs2ws_buy_good);
		}
	}

	void c2ws_req_goods_operate_lua(client_session * session, const char * msg, int32 msg_len)
	{
		 PROTO_C_WS_COMMON(goods_proto_goods_operate, c2ws_req_goods_operate)
	}

	void c2ws_req_get_legion_question_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		int32 back_string = 0;
		legion_proto_get_legion_answer_question_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		legion_ws_answer::get_instance().get_legion_question_info(session->get_role_guid());
	}

	void c2ws_req_get_legion_warehouse_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		// check
		legion_proto_req_legion_warehouse_item_info req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		// check
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		// 发送数据
		legion_ws_ptr->get_warehouse().send_item_list_to_client(session->get_role_guid());
	}
	
	void c2ws_req_get_legion_warehouse_log(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		// check
		legion_proto_req_legion_warehouse_log req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		// check
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		// 发送数据
		legion_ws_ptr->get_warehouse().send_log_list_to_client(session->get_role_guid());
	}

	void c2ws_req_change_legion_name(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		legion_proto_req_change_legion_name req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (false == session->is_self_server())
		{
			legion_proto_req_change_legion_name_end msg;
			msg.set_ret(e_legion_name_error_cross_server);
			session->send_to_client(&msg, e_msgindex_s2c_change_legion_name_end);
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			legion_proto_req_change_legion_name_end msg;
			msg.set_ret(e_legion_name_error_player_not_legion);
			session->send_to_client(&msg, e_msgindex_s2c_change_legion_name_end);
			return;
		}
		std::string legion_name = req.legion_name();
		legion_ws_ptr->change_legion_name(session->get_role_guid(), legion_name.c_str(), legion_name.size());

	}
	void c2ws_req_confirm_change_legion_name(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		legion_proto_req_confirm_change_legion_name req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		if (req.confirm_type())
		{
			std::string legion_name = req.legion_name();
			legion_ws_ptr->confirm_change_legion_name(session->get_role_guid(), legion_name.c_str(), legion_name.size());
		}	
	}
}

