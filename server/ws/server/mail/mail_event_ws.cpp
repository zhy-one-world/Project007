#include "ws_client.hpp"
#include "world_server.hpp"
#include <mail_msg.hpp>
#include "utility/init_unit.h"
#include "cross_group/cross_transfer_logic.hpp"
#include "mail_event_ws.h"
#include "utility/guid_gen.h"
#include "lua/script_mgr.h"
#include "net.pb.h"



namespace hld
{
	mail_ws_mgr::mail_ws_mgr()
	{

	}
	mail_ws_mgr::~mail_ws_mgr()
	{

	}

	void mail_ws_mgr::tick(const int64& new_time)
	{
		//测试跨服邮件代码
		int a1 = 0;
		if (a1 > 0)
		{
			for (int32 i = 0; i < 301; i++)
			{
				guid_64 player_guid;
				player_guid.server_64 = 8589978316330962697;
				std::vector<s_item_template_info> drop_item_list;
				drop_item_list.push_back({ 31020164 , 1});
				std::string title = "title";
				std::string content = "content";
				send_mail_system(player_guid, 10180, drop_item_list, title, content);
			}
		}
		int a2 = 0;
		if (a2 > 0)
		{
			for (int32 i = 0; i < 301; i++)
			{
				guid_64 player_guid;
				player_guid.server_64 = 8589978316330962695;
				std::vector<s_item_template_info> drop_item_list;
				drop_item_list.push_back({ 31020164 , 1});
				std::string title = "title";
				std::string content = "content";
				send_mail_system(player_guid, 10174, drop_item_list, title, content);
			}
		}

		int b1 = 0;
		if (b1 > 0)
		{
			s_other_server_mail_record mail_record;
			mail_record.source_server_id = 10174;
			mail_record.target_server_id = 10180;
			del_other_server_mail_record(mail_record);
		}
		int b2 = 0;
		if (b2 > 0)
		{
			s_other_server_mail_record mail_record;
			mail_record.source_server_id = 10180;
			mail_record.target_server_id = 10174;
			del_other_server_mail_record(mail_record);
		}

		load_other_server_mail_tick(new_time);
		send_other_server_mail_tick(new_time);
	}

	void mail_ws_mgr::send_mail(guid_64 address_guid, int32 address_server_id, const s_mail_info& mail_info, const s_item_info* item_list, int32 item_num, bool is_only_to_sql)
	{
		client_session* temp_session = client_session_mgr::getInstance().get_session(address_guid);

		// nullptr说明不在线/不存在，cs_uid = 0 说明不在CS上（比如传送中）
		if (!is_only_to_sql
			&& (temp_session != nullptr)
			&& (temp_session->get_cs_conn_index() >= 0))
		{
			ws2cs_send_mail request;

			request.addressee_guid = address_guid;
			request.item_num = item_num;
			request.mail_info = mail_info;
			for (int32 i = 0; i < item_num; ++i)
			{
				if (i >= max_item_per_mail)
				{
					break;
				}
				request.item_list[i] = item_list[i];
				request.mail_info.data_ary[i * 2 + EMailInfo_ItemGuid11] = item_list[i].item_guid.A;
				request.mail_info.data_ary[i * 2 + EMailInfo_ItemGuid12] = item_list[i].item_guid.B;
			}
			bool is_use = proto_by_lua(e_msgindex_ws2cs_recive_mail);
			if (is_use == false)
			{
				temp_session->send_to_cs(&request, sizeof(request));
			}
			else
			{
				hld::ws2cs_proto::send_mail pro_msg;
				request.to_proto(pro_msg);
				temp_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_recive_mail);
			}
		}

		ws2dp_add_mail_to_sql msg;
		msg.address_server_id = address_server_id;
		msg.mail_info = mail_info;
		std::string role_guid_str = init_unit::change_i64_to_string(address_guid.server_64);
		memcpy(msg.role_mark, role_guid_str.c_str(), role_guid_str.size());
		msg.mark_type = e_unit_guid_role_guid;
		msg.item_num = item_num;
		msg.mail_info.data_ary[EMailInfo_ServerId] = 0;
		if (address_server_id != world_server::getInstance().get_server_id())
		{
			msg.mail_info.data_ary[EMailInfo_ServerId] = address_server_id;
		}
		for (int32 i = 0; i < item_num; ++i)
		{
			if (i >= max_item_per_mail)
			{
				break;
			}
			msg.item_list[i] = item_list[i];
			msg.mail_info.data_ary[i * 2 + EMailInfo_ItemGuid11] = item_list[i].item_guid.A;
			msg.mail_info.data_ary[i * 2 + EMailInfo_ItemGuid12] = item_list[i].item_guid.B;
		}

		ws_client::getInstance().send_to_dp(&msg, sizeof(msg), address_server_id);
	}

	void mail_ws_mgr::send_mail(guid_64 address_guid, int32 address_server_id, s_mail_info mail_info, const std::vector<s_item_template_info>& item_list, bool is_only_to_sql)
	{
		s_item_info item_info_list[max_item_per_mail];
		memset(item_info_list, 0, sizeof(item_info_list));

		int32 item_slot_index = 0;
		bool is_send_once = false;
		for (auto& it : item_list)
		{
			s_item_info temp_item_info;

			if (init_unit::init_item_data(temp_item_info, it.m_item_id, -1, it.m_item_num, e_bag_type_bag, 0, it.m_lock))
			{
				item_info_list[item_slot_index] = temp_item_info;
				item_slot_index++;
			}
			if (item_slot_index >= max_item_per_mail)
			{
				send_mail(address_guid, address_server_id, mail_info, item_info_list, item_slot_index, is_only_to_sql);
				item_slot_index = 0;
				is_send_once = true;
				mail_info.mail_guid = guid_gen::make_guid();
				memset(item_info_list, 0, sizeof(item_info_list));
			}
		}

		if (!is_send_once
			|| item_slot_index > 0)
		{
			send_mail(address_guid, address_server_id, mail_info, item_info_list, item_slot_index, is_only_to_sql);
		}
	}

	void mail_ws_mgr::send_mail_system(guid_64 player_guid, int32 player_server_id, const std::vector<s_item_template_info>& item_list, std::string title, std::string contentText, int32 money_typ1, int32 money_num1, int32 money_typ2, int32 money_num2)
	{
		if (!player_guid.is_valid())
		{
			return;
		}

		s_mail_info new_mail_info;
		std::vector<guid_64> temp_item_guid_array;

		init_unit::init_mail_data_array(new_mail_info, temp_item_guid_array, money_typ1, money_num1, money_typ2, money_num2, player_guid);

		int32 string_len = 0;
		if (contentText.length() > max_text_contents_length)
		{
			string_len = max_text_contents_length;
		}
		else
		{
			string_len = contentText.length();
		}
		memcpy(new_mail_info.text_contents, contentText.c_str(), string_len);

		if (title.length() > max_mail_title_length)
		{
			string_len = max_mail_title_length;
		}
		else
		{
			string_len = title.length();
		}
		memcpy(new_mail_info.mail_title, title.c_str(), string_len);

		send_mail(player_guid, player_server_id, new_mail_info, item_list);
	}

	void mail_ws_mgr::send_mail_by_mail_id(guid_64 address_guid, int32 address_server_id, int32 mail_tamplate_id, int32 show_send_time)
	{
		MailTemplate* mail_template_ptr = GET_TEMPLATE(MailTemplate, mail_tamplate_id);
		if (mail_template_ptr == nullptr)
		{
			return;
		}

		send_mail_by_mail_template_ptr(address_guid, address_server_id, mail_template_ptr, show_send_time);
	}

	void mail_ws_mgr::send_mail_by_mail_template_ptr(guid_64 address_guid, int32 address_server_id, MailTemplate* mail_template_ptr, int32 show_send_time)
	{
		if (mail_template_ptr == nullptr)
		{
			return;
		}
		std::vector<s_single_mail_info> mail_info_array = get_mail_info_array_by_mail_template_ptr(mail_template_ptr, show_send_time);
		for (int32 i = 0; i < mail_info_array.size(); ++i)
		{
			s_single_mail_info temp_info = mail_info_array[i];
			send_mail(address_guid, address_server_id, temp_info.mail_info, temp_info.item_array, temp_info.item_num, false);
		}
	}


	std::vector<s_single_mail_info> mail_ws_mgr::get_mail_info_array_by_mail_template_ptr(MailTemplate* mail_template_ptr, int32 show_send_time)
	{
		std::vector<s_single_mail_info> return_array;
		if (mail_template_ptr == nullptr
			|| mail_template_ptr->ItemArray.size() % 2 != 0
			|| mail_template_ptr->MoneyArray.size() % 2 != 0)
		{
			return return_array;
		}

		std::vector<s_item_info> item_array;
		std::vector<int32> money_array;
		int32 all_item_count = 0;
		int32 all_money_count = 0;

		item_array.reserve(mail_template_ptr->ItemArray.size() / 2);
		money_array.reserve(mail_template_ptr->MoneyArray.size() / 2);

		for (int32 i = 0; i < mail_template_ptr->ItemArray.size(); i+=2)
		{
			s_item_info temp_info;
			int32 item_id = mail_template_ptr->ItemArray[i];
			int32 item_num = mail_template_ptr->ItemArray[i + 1];

			init_unit::init_item_data(temp_info, item_id, -1, item_num);
			item_array.push_back(temp_info);
			all_item_count++;
		}

		for (int32 i = 0; i < mail_template_ptr->MoneyArray.size(); i += 2)
		{
			int32 money_id = mail_template_ptr->MoneyArray[i];
			int32 money_num = mail_template_ptr->MoneyArray[i + 1];
			money_array.push_back(money_id);
			money_array.push_back(money_num);
			all_money_count++;
		}

		int32 money_send_all_req_times = all_money_count / 2;
		if (all_money_count % 2 != 0)
		{
			money_send_all_req_times += 1;
		}

		int32 item_send_all_req_times = all_item_count / max_item_per_mail;
		if (all_item_count % max_item_per_mail != 0)
		{
			item_send_all_req_times += 1;
		}

		int32 total_send_times_req = 0;
		if (item_send_all_req_times > money_send_all_req_times)
		{
			total_send_times_req = item_send_all_req_times;
		}
		else
		{
			total_send_times_req = money_send_all_req_times;
		}


		return_array.reserve(total_send_times_req);
		s_mail_info empty_mail_info;
		
		empty_mail_info.set_sender_name(std::to_string(mail_template_ptr->SenderNameID));
		empty_mail_info.set_mail_title(std::to_string(mail_template_ptr->TitleID));
		empty_mail_info.set_mail_content_text(std::to_string(mail_template_ptr->ContentTextID));

		if(show_send_time > 0)
		{
			empty_mail_info.data_ary[EMailInfo_SendTime] = show_send_time;
		}
		else
		{
			empty_mail_info.data_ary[EMailInfo_SendTime] = time_helper::get_cur_time_new().second;
		}



		for (int32 i = 0; i < total_send_times_req; ++i)
		{
			s_single_mail_info temp_info;
			temp_info.mail_info = empty_mail_info;
			temp_info.mail_info.mail_guid = guid_gen::make_guid();

			for (int32 j = 0; j < 2; ++j)
			{
				int32 money_index = get_effect_index_from_money_array(money_array);
				if (money_index < 0
					|| money_index >= all_money_count)
				{
					break;
				}
				temp_info.mail_info.data_ary[EMailInfo_MoneyTyp1 + j * 2] = money_array[money_index * 2];
				temp_info.mail_info.data_ary[EMailInfo_MoneyNum1 + j * 2] = money_array[money_index * 2 + 1];

				money_array[money_index * 2] = -1;
			}


			for (int32 j = 0; j < max_item_per_mail; ++j)
			{
				if (temp_info.item_num >= max_item_per_mail)
				{
					break;
				}
				int32 item_index = get_effect_index_from_item_array(item_array);
				if (item_index >= 0
					&& item_index < item_array.size())
				{
					temp_info.item_array[temp_info.item_num] = item_array[item_index];
					temp_info.item_num++;
					item_array[item_index].item_guid.clear_data();
				}
			}

			return_array.push_back(temp_info);
		}
		return return_array;
	}

	int32 mail_ws_mgr::get_effect_index_from_money_array(std::vector<int32>& money_array)
	{
		int32 invalid_index = -1;
		if (money_array.size() % 2 != 0)
		{
			return invalid_index;
		}

		int32 data_num = money_array.size() / 2;
		for (int32 i = 0; i < data_num; ++i)
		{
			int32 money_id = money_array[i * 2];
			if (money_id >= 0
				&& money_id < e_money_type_max)
			{
				return i;
			}
		}
		return invalid_index;
	}

	int32 mail_ws_mgr::get_effect_index_from_item_array(std::vector<s_item_info>& item_array)
	{
		int32 invalid_index = -1;
		for (int32 i = 0; i < item_array.size(); ++i)
		{
			if (item_array[i].item_guid.is_valid())
			{
				return i;
			}
		}
		return invalid_index;
	}

	void mail_ws_mgr::check_id_mail(guid_64 role_guid, int32 server_id, int32 cur_time)
	{
		if (!role_guid.is_valid())
		{
			return;
		}

		server2dp_proto_ws2dp_check_id_mail msg;
		msg.set_role_guid(role_guid.server_64);
		msg.set_server_id(server_id);
		msg.set_cur_time(cur_time);
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_check_id_mail, server_id);

		//script_mgr::get_instance().call_func("ws2dp_send", "send_check_id_mail_proc_lua", 0, false, "%l%d%d", role_guid.server_64, server_id, cur_time);

	}

	void mail_ws_mgr::check_id_mail_end(guid_64 role_guid, int32 server_id, const s_id_mail_info* mail_array, int32 mail_num)
	{
		if (mail_array == nullptr)
		{
			return;
		}

		for (int32 i = 0; i < mail_num; ++i)
		{
			s_id_mail_info temp_info = mail_array[i];
			send_mail_by_mail_id(role_guid, server_id, temp_info.mail_id);
		}
	}

	void mail_ws_mgr::load_other_server_mail_record_end(const s_other_server_mail_record* mail_record, int32 data_num)
	{
		for (int32 i = 0; i < data_num; i++)
		{
			if (mail_record[i].is_valid() == true)
			{
				add_other_server_mail_record(mail_record[i]);
			}
		}
		other_server_mail_record_load_finish = true;
	}

	void mail_ws_mgr::save_other_server_mail_record(s_other_server_mail_record mail_record)
	{
		//ws2dp_save_other_server_mail_record msg;
		//msg.mail_record = mail_record;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg), world_server::getInstance().get_server_id());
	
		script_mgr::get_instance().call_func("ws2dp_send", "send_save_other_server_mail_record_lua", 0, false, "%d%d%l%l%d", mail_record.source_server_id, mail_record.target_server_id, mail_record.mail_guid.server_64, mail_record.role_guid.server_64, world_server::getInstance().get_server_id());
	}
	void mail_ws_mgr::add_other_server_mail_record(s_other_server_mail_record mail_record)
	{
		int32 add_index = 0;
		for (int32 i = 0;i < max_other_server_mail_record_num;i++)
		{
			if (m_other_server_mail_record[i].is_valid() == true && m_other_server_mail_record[i].is_same_server_id(mail_record) == true)
			{
				add_index = i;
				break;
			}
			add_index++;
		}
		if (add_index >= max_other_server_mail_record_num)
		{
			return;
		}

		m_other_server_mail_record[add_index] = mail_record;
	}

	void mail_ws_mgr::del_other_server_mail_record(s_other_server_mail_record mail_record)
	{
		for (int32 i = 0;i < max_other_server_mail_record_num;i++)
		{
			if (m_other_server_mail_record[i].is_valid() == true && m_other_server_mail_record[i].is_same_server_id(mail_record) == true)
			{
				m_other_server_mail_record[i].mail_guid.clear_data();
				m_other_server_mail_record[i].role_guid.clear_data();
				save_other_server_mail_record(m_other_server_mail_record[i]);
				break;
			}
		}
	}

	s_other_server_mail_record  mail_ws_mgr::find_other_server_mail_record(int32 source_server_id, int32 target_server_id)
	{
		for (int32 i = 0;i < max_other_server_mail_record_num;i++)
		{
			if (m_other_server_mail_record[i].source_server_id == source_server_id && m_other_server_mail_record[i].target_server_id == target_server_id)
			{
				return m_other_server_mail_record[i];
			}
		}
		return s_other_server_mail_record();
	}

	void mail_ws_mgr::send_other_server_mail(s_other_server_mail_info mail_info)
	{
		s_other_server_mail_record mail_record;
		mail_record.source_server_id = world_server::getInstance().get_server_id();
		mail_record.target_server_id = mail_info.mail_info_db.data_info.data_ary[EMailInfo_ServerId];
		mail_record.mail_guid = mail_info.mail_info_db.data_info.mail_guid;
		mail_record.role_guid = mail_info.mail_info_db.role_guid;
		add_other_server_mail_record(mail_record);
		save_other_server_mail_record(mail_record);

		ws2ws_send_other_server_mail msg;
		msg.mail_record = mail_record;
		msg.mail_info = mail_info;
		cross::send_msg_to_ws(guid_64(), mail_info.mail_info_db.data_info.data_ary[EMailInfo_ServerId], e_msgindex_ws2ws_send_other_server_mail, &msg, sizeof(msg));
	}


	void mail_ws_mgr::recv_other_server_mail(const s_other_server_mail_record& mail_record, const s_other_server_mail_info& mail_info)
	{
		if (other_server_mail_record_load_finish == false)
		{
			return;
		}

		s_other_server_mail_record find_mail_record = find_other_server_mail_record(mail_record.source_server_id, mail_record.target_server_id);
		if (find_mail_record.mail_guid != mail_record.mail_guid)
		{
			add_other_server_mail_record(mail_record);
			save_other_server_mail_record(mail_record);

			int32 item_num = 0;
			for (int32 i = 0; i < max_item_per_mail; i++)
			{
				if (mail_info.item_list[i].item_guid.is_valid() == true)
				{
					item_num++;
				}
			}
			send_mail(mail_info.mail_info_db.role_guid, mail_info.mail_info_db.data_info.data_ary[EMailInfo_ServerId], mail_info.mail_info_db.data_info, mail_info.item_list, item_num);
		}
		ws2ws_send_other_server_mail_end msg;
		msg.mail_record = mail_record;
		cross::send_msg_to_ws(guid_64(), mail_record.source_server_id, e_msgindex_ws2ws_send_other_server_mail_end, &msg, sizeof(msg));
	}

	void mail_ws_mgr::recv_other_server_mail_end(const s_other_server_mail_record& mail_record)
	{
		if (other_server_mail_record_load_finish == false)
		{
			return;
		}
		
		del_other_server_mail_record(mail_record);
		del_other_server_mail(mail_record);

		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_other_server_mail[i].server_id == mail_record.target_server_id)
			{
				m_other_server_mail[i].send_tick_time = -1;
				break;
			}
		}
	}

	void mail_ws_mgr::load_other_server_mail_tick(const int64& new_time)
	{
		if (other_server_mail_record_load_finish == false)
		{
			return;
		}
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_other_server_mail[i].mail_info[0].is_valid() == false && m_other_server_mail[i].server_id > 0)
			{
				if (m_other_server_mail[i].load_tick_time > new_time)
				{
					continue;
				}
				m_other_server_mail[i].load_tick_time = new_time + second_tick_time * 10;

				s_other_server_mail_record mail_record = find_other_server_mail_record(world_server::getInstance().get_server_id(), m_other_server_mail[i].server_id);
				//ws2dp_load_other_server_mail msg;
				//msg.server_id = m_other_server_mail[i].server_id;
				//msg.mail_guid = mail_record.mail_guid;
				//ws_client::getInstance().send_to_dp(&msg, sizeof(msg), world_server::getInstance().get_server_id());
			
				script_mgr::get_instance().call_func("ws2dp_send", "send_load_other_server_mail_lua", 0, false, "%d%l%d", m_other_server_mail[i].server_id, mail_record.mail_guid.server_64, world_server::getInstance().get_server_id());
			}
		}
	}

	void mail_ws_mgr::load_other_server_mail_end(int32 server_id, const s_other_server_mail_info* mail_record, int32 data_num)
	{
		if (data_num > max_other_server_mail_num)
		{
			return;
		}

		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_other_server_mail[i].server_id == server_id)
			{
				if (m_other_server_mail[i].mail_info[0].is_valid() == true)
				{
					return;
				}
				memcpy(m_other_server_mail[i].mail_info, mail_record, data_num * sizeof(s_other_server_mail_info));
				m_other_server_mail[i].send_tick_time = -1;
				m_other_server_mail[i].load_tick_time = -1;
				break;
			}
		}
	}

	void mail_ws_mgr::send_other_server_mail_tick(const int64& new_time)
	{
		if (other_server_mail_record_load_finish == false)
		{
			return;
		}

		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_other_server_mail[i].mail_info[0].is_valid() == false)
			{
				continue;
			}
			if (m_other_server_mail[i].send_tick_time > new_time)
			{
				continue;
			}
			m_other_server_mail[i].send_tick_time = new_time + second_tick_time * 10;

			send_other_server_mail(m_other_server_mail[i].mail_info[0]);
		}
	}

	void mail_ws_mgr::del_other_server_mail(s_other_server_mail_record mail_record)
	{
		int32 server_index = -1;
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_other_server_mail[i].server_id == mail_record.target_server_id)
			{
				server_index = i;
				break;
			}
		}
		if (server_index < 0)
		{
			return;
		}
		if (m_other_server_mail[server_index].mail_info[0].get_mail_guid() != mail_record.mail_guid)
		{
			return;
		}
		for (int32 i = 0; i < max_other_server_mail_num - 1; i++)
		{
			m_other_server_mail[server_index].mail_info[i] = m_other_server_mail[server_index].mail_info[i + 1];
		}
		m_other_server_mail[server_index].mail_info[max_other_server_mail_num - 1].clear_data();
		
		//ws2dp_del_other_server_mail msg;
		//msg.mail_record = mail_record;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg), world_server::getInstance().get_server_id());
	
		script_mgr::get_instance().call_func("ws2dp_send", "send_del_other_server_mail_lua", 0, false, "%d%d%l%l%d", mail_record.source_server_id, mail_record.target_server_id, mail_record.mail_guid.server_64, mail_record.role_guid.server_64, world_server::getInstance().get_server_id());

	}

	std::vector<s_item_template_info>* mail_ws_mgr::get_free_item_list()
	{
		free_item_list.clear();

		return &free_item_list;
	}

	void mail_ws_mgr::add_free_item(int32 item_id, int32 item_num)
	{
		free_item_list.push_back({item_id, item_num});
	}



}