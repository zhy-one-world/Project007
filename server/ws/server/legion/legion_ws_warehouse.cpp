#include "legion_ws_warehouse.h"
#include "../client_session_mgr.hpp"
#include <legion_msg.hpp>
#include "./time.hpp"
#include "ws_client.hpp"
#include "template/template_manager.h"
#include "../mail/event_ws_mgr.h"
#include "utility/init_unit.h"
#include "net.pb.h"

namespace faith
{
	legion_ws_warehouse::legion_ws_warehouse()
	{
		clear_data();
	}

	legion_ws_warehouse::~legion_ws_warehouse()
	{
		clear_data();
	}

	void legion_ws_warehouse::clear_data()
	{
		m_legion_warehouse_list.clear();
		m_legion_warehouse_log.clear();
	}

	// 添加日志
	void legion_ws_warehouse::add_legion_log(const s_legion_warehouse_log& log, bool is_from_db) 
	{
		if (is_from_db)
		{
			m_legion_warehouse_log.push_back(log);
		}
		else
		{
			//判断容量 超过容量 先删除时间最早的那个 再添加
			if(m_legion_warehouse_log.size() >= max_legion_warehouse_log)
			{
				int64 min_time = utility::get_tick_count();
				for (s_legion_warehouse_log& log : m_legion_warehouse_log){
					if (log.add_time < min_time){
						min_time = log.add_time;
					}
				}
				//删除时间最小的那个元素
				std::list<s_legion_warehouse_log>::iterator ite;
				for (ite = m_legion_warehouse_log.begin(); ite != m_legion_warehouse_log.end();){
					if (ite->add_time == min_time){
						ite = m_legion_warehouse_log.erase(ite);
						break;
					}
					else{
						++ite;
					}
				}
				m_legion_warehouse_log.push_back(log);
			}
			else
			{
				m_legion_warehouse_log.push_back(log);
			}
		}
	}

	// 添加道具或者从数据库中加载道具
	bool legion_ws_warehouse::add_legion_warehouse(const s_legion_warehouse_info& legion_warehouse_info, bool is_from_db)
	{
		//仓库已满
		if (get_legion_warehouse_count() == max_legion_warehouse_info)
			return false;
		if(is_from_db)
		{
			m_legion_warehouse_list.push_back(legion_warehouse_info);
		}
		else
		{
			//需要查重
			if (!is_in_legion_warehouse(legion_warehouse_info.item_info.item_guid))
			{
				m_legion_warehouse_list.push_back(legion_warehouse_info);
			}
		}
		return true;
	}

	// 删除仓库里面的道具
	void legion_ws_warehouse::del_legion_warehouse(const guid_64& item_guid)
	{
		std::list<s_legion_warehouse_info>::iterator ite;
		for (ite = m_legion_warehouse_list.begin(); ite != m_legion_warehouse_list.end();)
		{
			if (ite->item_info.item_guid == item_guid)
			{
				ite = m_legion_warehouse_list.erase(ite);
				break;
			}
			else
			{
				++ite;
			}
		}
	}

	s_legion_warehouse_info legion_ws_warehouse::get_item(const guid_64& item_guid)
	{
		s_legion_warehouse_info item;
		for (s_legion_warehouse_info& legion_member_info : m_legion_warehouse_list)
		{
			if (item_guid == legion_member_info.item_info.item_guid)
			{
				item = legion_member_info;
			}
		}
		return item;
	}

	// 检查仓库里面是否已经存在该道具
	bool legion_ws_warehouse::is_in_legion_warehouse(const guid_64& item_guid)
	{
		for (s_legion_warehouse_info& legion_warehouse_info : m_legion_warehouse_list)
		{
			if (legion_warehouse_info.item_info.item_guid == item_guid)
			{
				return true;
			}
		}
		return false;
	}

	// 发送仓库道具到客户端
	void legion_ws_warehouse::send_item_list_to_client(uint64 role_guid)
	{
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			return;
		}
		legion_proto_req_legion_warehouse_item_info_end msg;
		msg.set_is_final(0);// 0:没有结束 1:结束
		msg.set_is_clean(0);// 0:需要清空 1:不需要
		int32 need_clear = 0;
		for (s_legion_warehouse_info& legion_warehouse_info : m_legion_warehouse_list)
		{
			legion_proto_legion_warehouse_item_info* temp_info = msg.add_item_list();
			make_package_info(temp_info, legion_warehouse_info);

			int32 pak_size = msg.ByteSize();
			if (pak_size > packege_length_max)
			{
				msg.set_is_clean(need_clear);
				need_clear = 1;
				session_ptr->send_to_client(&msg, e_msgindex_s2c_get_legion_warehouse_item_info_end);
				msg.clear_item_list();
			}
		}
		msg.set_is_clean(need_clear);
		msg.set_is_final(1);
		session_ptr->send_to_client(&msg, e_msgindex_s2c_get_legion_warehouse_item_info_end);
	}

	//发送日志
	void legion_ws_warehouse::send_log_list_to_client(uint64 role_guid)
	{
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			return;
		}
		legion_proto_req_legion_warehouse_log_end msg;
		for (s_legion_warehouse_log& legion_warehouse_log : m_legion_warehouse_log)
		{
			legion_proto_legion_warehouse_log* temp_info = msg.add_log_list();
			temp_info->set_add_time(legion_warehouse_log.add_time);
			temp_info->set_job(legion_warehouse_log.job);
			temp_info->set_role_name(legion_warehouse_log.role_name);
			temp_info->set_operation_type(legion_warehouse_log.operation_type);
			temp_info->set_eiteminfo_info_id(legion_warehouse_log.item_template_id);
		}
		session_ptr->send_to_client(&msg, e_msgindex_s2c_get_legion_warehouse_log_end);
	}

	//存储操作
	void legion_ws_warehouse::operation_store_legion_warehouse(const cs2ws_legion_warehouse_operation& info)
	{
		int32 error_id = operation_store(info);

		//从新将仓库的数据发送到前端
		if (error_id <= 0) {
			send_item_list_to_client(info.role_guid);
		}

		// 将结果发送给cs
		ws2cs_legion_warehouse_operation msg;
		msg.error_id = error_id;
		msg.role_guid = info.role_guid;
		msg.operation_type = info.operation_type;
		msg.legion_warehouse_info = info.legion_warehouse_info;
		client_session* player_session = client_session_mgr::getInstance().get_session(info.role_guid);
		if (nullptr == player_session)
		{
			return;
		}
		player_session->send_to_cs(&msg, sizeof(msg));
	}
	// 存储道具
	int32 legion_ws_warehouse::operation_store(const cs2ws_legion_warehouse_operation& info)
	{
		int32 error_id = 0;
		// 1.仓库容量已满
		if (get_legion_warehouse_count() == max_legion_warehouse_info)
			return e_legion_warehouse_is_full;

		// 添加到内存
		m_legion_warehouse_list.push_back(info.legion_warehouse_info);
		// 添加日志到内存
		add_legion_log(info.legion_warehouse_log,false);

		// 发送公告
		send_notice((e_legion_warehouse_operation_type)info.operation_type, info.role_guid, info.legion_warehouse_info.item_info.data_ary[e_item_info_info_id]);


		save_legion_warehouse(info.legion_warehouse_info);
		save_legion_warehouse_log(info.legion_warehouse_log);
		// 添加到数据库
		return error_id;
	}

	void legion_ws_warehouse::save_legion_warehouse(const s_legion_warehouse_info& info)
	{
		ws2dp_store_legion_warehouse_info msg;
		msg.legion_info = info;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	void legion_ws_warehouse::save_legion_warehouse_log(const s_legion_warehouse_log& log)
	{
		ws2dp_store_legion_warehouse_log msg;
		msg.log = log;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}
	//获取操作
	void legion_ws_warehouse::operation_out_legion_warehouse(const cs2ws_legion_warehouse_operation& info)
	{
		s_legion_warehouse_info temp_item = get_item(info.legion_warehouse_info.item_info.item_guid);

		int32 error_id = operation_out(info);

		//从新将仓库的数据发送到前端
		if (error_id <= 0) {
			send_item_list_to_client(info.role_guid);
		}

		// 将结果发送给cs
		ws2cs_legion_warehouse_operation msg;
		msg.error_id = error_id;
		msg.role_guid = info.role_guid;
		msg.operation_type = info.operation_type;
		msg.legion_warehouse_info = temp_item;

		client_session* player_session = client_session_mgr::getInstance().get_session(info.role_guid);
		if (nullptr == player_session)
		{
			return;
		}
		player_session->send_to_cs(&msg, sizeof(msg));
	}
	//获取道具
	int32 legion_ws_warehouse::operation_out(const cs2ws_legion_warehouse_operation& info)
	{
		int32 error_id = 0;
		//1.检查是否存在该道具 e_legion_not_has_item_error
		if (!is_in_legion_warehouse(info.legion_warehouse_info.item_info.item_guid))
			return e_legion_not_has_item_error;

		// 从军团仓库的内存中移除该道具
		s_legion_warehouse_info temp_item = get_item(info.legion_warehouse_info.item_info.item_guid);
		del_legion_warehouse(info.legion_warehouse_info.item_info.item_guid);
		// 添加日志到内存
		add_legion_log(info.legion_warehouse_log, false);

		// 发送公告
		send_notice((e_legion_warehouse_operation_type)info.operation_type, info.role_guid, info.legion_warehouse_log.item_template_id);

		// 从数据库中删除
		ws2dp_role_get_legion_warehouse_info msg;
		msg.role_guid = info.role_guid;
		msg.legion_info = temp_item;
		msg.log = info.legion_warehouse_log;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
		return error_id;
	}


	void legion_ws_warehouse::destroy_item(const cs2ws_legion_warehouse_operation& info)
	{
		ws2dp_destroy_legion_item msg;
		msg.log = info.legion_warehouse_log;
		msg.del_num = info.del_num;
		// 记录要删除的道具的guid
		int32 has_del_num = 0;//用于记录存在的guid
		for (int32 i = 0; i < info.del_num; i++)
		{
			if (is_in_legion_warehouse(info.del_array[i])){
				msg.del_guid_array[has_del_num] = info.del_array[i];
				has_del_num++;
			}
		}
		// 一次性插入删除日志 最多20条
		int32 max_log_del = has_del_num;
		if (max_log_del >= max_legion_warehouse_log)
		{
			max_log_del = max_legion_warehouse_log;
		}
		for (int32 i = 0; i < max_log_del; i++)
		{
			s_legion_warehouse_info item_info = get_item(msg.del_guid_array[i]);
			msg.del_item_id[i] = item_info.item_info.data_ary[e_item_info_info_id];
		}

		//添加销毁日志到内存
		for (int32 i = 0; i < max_log_del; i++)
		{
			s_legion_warehouse_info item_info = get_item(msg.del_guid_array[i]);
			s_legion_warehouse_log log = info.legion_warehouse_log;
			log.item_guid = item_info.item_info.item_guid;
			log.item_template_id = item_info.item_info.data_ary[e_item_info_info_id];
			add_legion_log(log, false);
		}
		// 从内存中删除道具
		for (int32 i = 0; i < has_del_num; i++)
		{
			del_legion_warehouse(msg.del_guid_array[i]);
		}
		msg.del_log_num = max_log_del;
		// 从数据库中删除道具
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
		//从新将仓库的数据发送到前端
		send_item_list_to_client(info.role_guid);

		// 将结果发送给cs
		ws2cs_legion_warehouse_operation response_cs;
		response_cs.error_id = 0;
		response_cs.role_guid = info.role_guid;
		response_cs.operation_type = info.operation_type;
		client_session* player_session = client_session_mgr::getInstance().get_session(info.role_guid);
		if (nullptr == player_session)
		{
			return;
		}
		player_session->send_to_cs(&response_cs, sizeof(response_cs));
	}

	// 当删除军团时 需要清空数据
	void legion_ws_warehouse::clear_legion_warehouse_info_in_db(const guid_64& legion_guid)
	{
		// 从内存中删除道具
		std::list<s_legion_warehouse_info>::iterator ite;
		for (ite = m_legion_warehouse_list.begin(); ite != m_legion_warehouse_list.end();)
		{
			if (ite->legion_guid == legion_guid)
			{
				ite = m_legion_warehouse_list.erase(ite);
				break;
			}
			else
			{
				++ite;
			}
		}
		// 从数据库中删除
		ws2dp_clear_legion_warehouse msg;
		msg.legion_guid = legion_guid;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	void legion_ws_warehouse::send_notice(e_legion_warehouse_operation_type operation_type, const guid_64& role_guid,int32 item_id)
	{
		client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == player_session)
			return;

		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
		if (item_template_ptr == nullptr)
			return;

		int32 notice_id = 0;
		int32 score = 0;
		switch (operation_type)
		{
		case faith::e_legion_warehouse_operation_store:
			notice_id = legion_warehouse_store_notice_id;
			score = item_template_ptr->LegionDonateAddIntegral;
			break;
		case faith::e_legion_warehouse_operation_out:
			notice_id = legion_warehouse_get_notice_id;
			score = item_template_ptr->LegionDonateReduceIntegral;
			break;
		case faith::e_legion_warehouse_operation_destroy:
			break;
		default:
			break;
		}
		if (notice_id == 0)
			return;

		std::vector<std::string> vec_notice_str;
		// 人名
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(player_session->get_role_name());
		// 道具名
		vec_notice_str.push_back(template_manager::get_instance().int_to_string(item_template_ptr->ItemName));
		// 积分数量
		vec_notice_str.push_back(template_manager::get_instance().int_to_string(score));
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, role_guid, notice_str);
	}

	void legion_ws_warehouse::make_package_info(legion_proto_legion_warehouse_item_info* target_info, const s_legion_warehouse_info& source_info)
	{
		target_info->set_item_guid_a(source_info.item_info.item_guid.A);
		target_info->set_item_guid_b(source_info.item_info.item_guid.B);
		target_info->set_add_time(source_info.add_time);
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			target_info->add_item_info(source_info.item_info.data_ary[i]);
		}
	}
}