#include "event_ws_mgr.h"
#include "utility/cs_date.hpp"
#include "utility/guid_gen.h"
#include "utility/init_unit.h"
#include "Logic/char_def.hpp"
#include "Logic/time_def.hpp"
#include <mail_msg.hpp>
#include <event_msg.hpp>
#include "chat.pb.h"
#include"template/template_manager.h"
#include "ws_client.hpp"
#include "server/mail/mail_event_ws.h"
#include "server/legion/legion_ws_mgr.h"
#include "server/team/team_ws_mgr.h"
#include "net.pb.h"

namespace faith
{
	event_ws_mgr::event_ws_mgr()
	{
		clear_data();
	}

	event_ws_mgr::~event_ws_mgr()
	{


	}

	void event_ws_mgr::clear_data()
	{
		m_timer_process_time = 0;
		m_mail_event_living_time = 0;
		m_refresh_time = 10;
		m_cur_refresh_time = 0;
		m_mail_event_living_time = GAMECONFIG->MailLiveTime;
		memset(m_globel_mail_array, 0, sizeof(m_globel_mail_array));
		memset(m_temp_mail_array, 0, sizeof(m_temp_mail_array));
		memset(m_temp_event_guid_array, 0, sizeof(m_temp_event_guid_array));
		m_globel_mail_item_array.clear();
	}

	void event_ws_mgr::tick(const int64& now_stamp)
	{
		if (now_stamp >= m_timer_process_time)
		{
			tick_1sec(now_stamp);
			m_timer_process_time = now_stamp + second_tick_time;
		}
	}

	void event_ws_mgr::tick_1sec(const int64&  now_stamp)
	{
		int32 temp_cur_time = time_helper::get_cur_time_new().second;
		bool mail_changed = false;
		for (int32 i = 0; i < max_globel_mail_in_same_time; ++i)
		{//检查全局邮件存活性
			if (!m_globel_mail_array[i].mail_guid.is_valid())
			{
				continue;
			}

			const s_mail_info& temp_info = m_globel_mail_array[i];

			if (temp_cur_time >= temp_info.data_ary[EMailInfo_IsNeedDelete])
			{
				mail_changed = true;
				guid_64 item_guid1 = guid_64(temp_info.data_ary[EMailInfo_ItemGuid11], temp_info.data_ary[EMailInfo_ItemGuid12]);
				guid_64 item_guid2 = guid_64(temp_info.data_ary[EMailInfo_ItemGuid21], temp_info.data_ary[EMailInfo_ItemGuid22]);
				guid_64 item_guid3 = guid_64(temp_info.data_ary[EMailInfo_ItemGuid31], temp_info.data_ary[EMailInfo_ItemGuid32]);
				guid_64 item_guid4 = guid_64(temp_info.data_ary[EMailInfo_ItemGuid41], temp_info.data_ary[EMailInfo_ItemGuid42]);
				guid_64 item_guid5 = guid_64(temp_info.data_ary[EMailInfo_ItemGuid51], temp_info.data_ary[EMailInfo_ItemGuid52]);

				m_globel_mail_item_array.erase(item_guid1.server_64);
				m_globel_mail_item_array.erase(item_guid2.server_64);
				m_globel_mail_item_array.erase(item_guid3.server_64);
				m_globel_mail_item_array.erase(item_guid4.server_64);
				m_globel_mail_item_array.erase(item_guid5.server_64);

				m_globel_mail_array[i].clear_data();
			}
		}

		if (mail_changed)
		{
			save_globel_mail_to_sql();
		}

		continued_notice_map_ite ite;
		for (ite = m_continued_notice_array.begin(); ite != m_continued_notice_array.end(); )
		{
			continued_notice& notice_ref = ite->second;
			
			if (now_stamp < notice_ref.m_begin_time)
			{
				++ite;
				continue;
			}
			if (notice_ref.notice_id <= 0)
			{
				xstring send_string = notice_ref.text;
				send_notice_to_all_only_string(send_string);
			}
			else
			{
				chat_proto_notice_info msg;
				msg.add_role_guid(notice_ref.sender_guid.A);
				msg.add_role_guid(notice_ref.sender_guid.B);
				msg.set_notice_id(notice_ref.notice_id);
				msg.set_create_time(temp_cur_time);
				msg.set_final_string(notice_ref.text);
				send_notice_to_everyone(&msg, e_msgindex_s2c_receive_notice);
			}
			notice_ref.m_begin_time = now_stamp + notice_ref.m_interval_time;
			if (notice_ref.m_begin_time > notice_ref.m_end_time)
			{
				ite = m_continued_notice_array.erase(ite);
			}
			else
			{
				++ite;
			}
		}
	}

	void event_ws_mgr::save_globel_mail_to_sql()
	{
		
		server2dp_proto_ws2dp_save_mail_event_to_db mail_info;
		int32 effect_mail_info_idnex = 0;
		for (int32 i = 0; i < max_globel_mail_in_same_time; ++i)
		{
			if (!m_globel_mail_array[i].mail_guid.is_valid())
			{
				continue;
			}
			server2dp_proto_s_mail_info * add_info = mail_info.add_mail_info();
			if (nullptr == add_info)
			{
				break;
			}
			set_mail_proto_info(add_info, m_globel_mail_array[i]);
			effect_mail_info_idnex++;
		}
		mail_info.set_mail_num(effect_mail_info_idnex);
		ws_client::getInstance().send_to_dp_new(&mail_info, e_msg_ws2dp_save_mail_event);


		//ws2dp_save_mail_event_to_db mail_info;
		//int32 effect_mail_info_idnex = 0;
		//for (int32 i = 0; i < max_globel_mail_in_same_time; ++i)
		//{
		//	if (!m_globel_mail_array[i].mail_guid.is_valid())
		//	{
		//		continue;
		//	}

		//	mail_info.mail_info[effect_mail_info_idnex] = m_globel_mail_array[i];
		//	effect_mail_info_idnex++;
		//}
		//mail_info.mail_num = effect_mail_info_idnex;
		//ws_client::getInstance().send_to_dp(&mail_info, mail_info.get_pak_length());


		server2dp_proto_ws2dp_save_mail_event_item_to_db mail_item_info;
		if (m_globel_mail_item_array.size() > 0)
		{
			int32 item_info_index = 0;
			for (std::unordered_map<ui64, s_item_info>::iterator it = m_globel_mail_item_array.begin(); it != m_globel_mail_item_array.end(); ++it)
			{
				if (item_info_index >= max_globel_mail_item_in_same_time)
				{
					break;
				}
				server2dp_proto_s_item_info * proto_item_info = mail_item_info.add_item_list();
				if (nullptr == proto_item_info)
				{
					break;
				}
				s_item_info _info = it->second;
				proto_item_info->set_item_guid(_info.item_guid.server_64);
				proto_item_info->set_param_str(_info.param_str);
				for (int32 index = 0; index < e_item_info_max; ++index)
				{
					proto_item_info->add_data_ary(_info.data_ary[index]);
				}
				item_info_index++;
			}
			mail_item_info.set_item_num(item_info_index);
		}
		ws_client::getInstance().send_to_dp_new(&mail_item_info, e_msg_ws2dp_save_mail_event_item);

		//ws2dp_save_mail_event_item_to_db mail_item_info;
		//if (m_globel_mail_item_array.size() > 0)
		//{
		//	int32 item_info_index = 0;
		//	for (std::unordered_map<ui64, s_item_info>::iterator it = m_globel_mail_item_array.begin(); it != m_globel_mail_item_array.end(); ++it)
		//	{
		//		if (item_info_index >= max_globel_mail_item_in_same_time)
		//		{
		//			break;
		//		}
		//		mail_item_info.item_list[item_info_index] = it->second;
		//		item_info_index++;
		//	}
		//	mail_item_info.item_num = item_info_index;
		//}

		//ws_client::getInstance().send_to_dp(&mail_item_info, mail_item_info.get_pak_length());
	}


	void event_ws_mgr::save_mail_event_end()
	{
	}

	void event_ws_mgr::load_globel_mail_by_sql()
	{
		server2dp_proto_ws2dp_load_mail_event load_mail;
		ws_client::getInstance().send_to_dp_new(&load_mail, e_msg_ws2dp_load_mail_event);

		//ws2dp_load_mail_event load_mail;
		//ws_client::getInstance().send_to_dp(&load_mail, sizeof(load_mail));
	}

	void event_ws_mgr::load_globel_mail_item_by_sql()
	{
		server2dp_proto_ws2dp_load_mail_event_item load_mail_item;
		ws_client::getInstance().send_to_dp_new(&load_mail_item, e_msg_ws2dp_load_mail_event_item);

		//ws2dp_load_mail_event_item load_mail_item;
		//ws_client::getInstance().send_to_dp(&load_mail_item, sizeof(load_mail_item));
	}

	void event_ws_mgr::load_mail_event_end(s_mail_info* mail_info_ptr, int32 mail_num)
	{
		memset(m_globel_mail_array, 0, sizeof(m_globel_mail_array));
		if (mail_info_ptr == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < mail_num; ++i)
		{
			if (i >= max_globel_mail_in_same_time)
			{
				break;
			}
			m_globel_mail_array[i] = mail_info_ptr[i];
		}
	}

	void event_ws_mgr::load_mail_event_item_end(s_item_info* item_info_ptr, int32 item_num)
	{
		m_globel_mail_item_array.clear();
		if (item_info_ptr == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < item_num; ++i)
		{
			m_globel_mail_item_array.insert({ item_info_ptr[i].item_guid.server_64, item_info_ptr[i] });
		}
	}

	void event_ws_mgr::load_all_event_info_to_ws()
	{
		load_globel_mail_by_sql();
		load_globel_mail_item_by_sql();
	}

	void event_ws_mgr::add_globel_mail(s_mail_info temp_mail_info, const s_item_info* temp_item_list, int32 item_num)
	{
		if (item_num > max_item_per_mail)
		{
			return;
		}
		int32 empty_mail_slot = get_empty_globel_mail_slot();
		if (empty_mail_slot < 0
			|| empty_mail_slot >= max_globel_mail_in_same_time)
		{
			return;
		}
		m_globel_mail_array[empty_mail_slot] = temp_mail_info;

		for (int32 i = 0; i < item_num; ++i)
		{
			m_globel_mail_item_array.insert({ temp_item_list[i].item_guid.server_64, temp_item_list[i] });
		}

		save_globel_mail_to_sql();
	}

	void event_ws_mgr::check_player_globel_mail(const guid_64* had_event_array, int32 data_num, guid_64 player_guid, int64 role_create_time)
	{
		if (had_event_array == nullptr
			|| data_num < 0
			|| data_num > max_globel_mail_in_same_time)
		{
			return;
		}
		bool event_state_changed = false;
		memcpy(m_temp_mail_array, m_globel_mail_array, sizeof(m_globel_mail_array));
		memset(m_temp_event_guid_array, 0, sizeof(m_temp_event_guid_array));
		memcpy(m_temp_event_guid_array, had_event_array, data_num * sizeof(guid_64));

		for (int32 i = 0; i < data_num; ++i)
		{//找出所有未接受过的全局邮件，并清空无效的标志位
			if (!m_temp_event_guid_array[i].is_valid())
			{
				continue;
			}

			bool is_this_event_req_delete = true;
			for (int32 j = 0; j < max_globel_mail_in_same_time; ++j)
			{
				if (!m_temp_mail_array[j].mail_guid.is_valid())
				{
					continue;
				}

				if (m_temp_event_guid_array[i] == m_temp_mail_array[j].mail_guid)
				{//已经有此邮件
				 //就把此邮件从 预接受邮件 列表删除
					m_temp_mail_array[j].clear_data();
					is_this_event_req_delete = false;
					break;
				}
			}

			if (is_this_event_req_delete)
			{//玩家身上的guid在服务器端找不到，就说明该邮件事件失效了，删除玩家身上的标志位
				m_temp_event_guid_array[i].clear_data();
				event_state_changed = true;
			}
		}

		for (int32 i = 0; i < max_globel_mail_in_same_time; ++i)
		{//最终发送邮件
			if (!m_temp_mail_array[i].mail_guid.is_valid()
				|| m_temp_mail_array[i].data_ary[EMailInfo_SendTime] < role_create_time)
			{//邮件发送日期在建号之前
				continue;
			}

			guid_64 item_guid1 = guid_64(m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid11], m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid12]);
			guid_64 item_guid2 = guid_64(m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid21], m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid22]);
			guid_64 item_guid3 = guid_64(m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid31], m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid32]);
			guid_64 item_guid4 = guid_64(m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid41], m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid42]);
			guid_64 item_guid5 = guid_64(m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid51], m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid52]);

			s_item_info temp_mail_items_info[max_item_per_mail];
			memset(temp_mail_items_info, 0, sizeof(temp_mail_items_info));

			int32 effect_item_index = 0;
			if (m_globel_mail_item_array.size() > 0)
			{
				for (int32 j = 0; j < max_item_per_mail; ++j)
				{
					guid_64 temp_item_guid;
					temp_item_guid.A = m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid11 + j * 2];
					temp_item_guid.B = m_temp_mail_array[i].data_ary[EMailInfo_ItemGuid12 + j * 2];

					if (temp_item_guid.is_valid())
					{
						std::unordered_map<ui64, s_item_info>::iterator temp_it = m_globel_mail_item_array.find(temp_item_guid.server_64);
						if (temp_it != m_globel_mail_item_array.end())
						{
							temp_mail_items_info[effect_item_index] = temp_it->second;

							//此处的物品信息是模板，需要对每个发送的物品重新生成guid
							temp_mail_items_info[effect_item_index].item_guid = guid_gen::make_guid(player_guid);
							effect_item_index++;
						}
					}
				}
			}

			s_mail_info temp_mail_info = m_temp_mail_array[i];
			temp_mail_info.mail_guid = guid_gen::make_guid(player_guid);
			mail_ws_mgr::get_instance().send_mail(player_guid, 0, temp_mail_info, temp_mail_items_info, effect_item_index, false);

			for (int32 j = 0; j < max_globel_mail_in_same_time; ++j)
			{//此处虽然没用 data_num 但不会有问题，因为data_num是有效长度不是总长度
				if (!m_temp_event_guid_array[j].is_valid())
				{
					m_temp_event_guid_array[j] = m_temp_mail_array[i].mail_guid;
					break;
				}
			}
			event_state_changed = true;
		}


		if (event_state_changed)
		{
			ws2cs_had_new_globel_mail msg;

			server2dp_proto_ws2dp_save_new_globel_mail_had_info dp_msg;


			//ws2dp_save_new_globel_mail_had_info dp_msg;

			msg.role_guid = player_guid;
			dp_msg.set_role_guid(player_guid.server_64);
			//dp_msg.role_guid = player_guid;

			int32 effect_index = 0;
			for (int32 i = 0; i < max_globel_mail_in_same_time; ++i)
			{
				dp_msg.add_data_info(m_temp_event_guid_array[i].server_64);
				if (!m_temp_event_guid_array[i].is_valid())
				{
					continue;
				}
				msg.data_info[effect_index] = m_temp_event_guid_array[i];
				//dp_msg.data_info[effect_index] = m_temp_event_guid_array[i];
				effect_index++;
			}
			msg.data_num = effect_index;

			client_session* client_session_ptr = client_session_mgr::getInstance().get_session(player_guid);
			if (client_session_ptr != nullptr)
			{
				bool is_use = proto_by_lua(e_msgindex_ws2cs_had_new_globel_mail);
				if (is_use == false)
				{
					client_session_ptr->send_to_cs(&msg, sizeof(msg));
				}
				else
				{
					faith::ws2cs_proto::had_new_globel_mail pro_msg;
					msg.to_proto(pro_msg);
					client_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_had_new_globel_mail);
				}
			}
			ws_client::getInstance().send_to_dp_new(&dp_msg, e_msg_ws2dp_save_new_globel_mail_had_info);
			//ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
		}
	}

	bool event_ws_mgr::add_delay_globle_notice(guid_64 notice_guid, xstring& notice_content, int64 startStamp, int64 endStamp, int64 marqueeInterval)
	{
		if (notice_guid.is_valid() == false)
		{
			return false;
		}
		continued_notice new_notice;
		new_notice.m_begin_time = startStamp;
		new_notice.m_end_time = endStamp;
		new_notice.m_interval_time = marqueeInterval;
		int32 text_len = notice_content.size();
		if (text_len > globel_message_max_size)
		{
			text_len = globel_message_max_size;
		}
		memcpy(new_notice.text, notice_content.c_str(), text_len);
		m_continued_notice_array[notice_guid.server_64] = new_notice;
		return true;
	}

	void event_ws_mgr::del_delay_globle_notice(guid_64 notice_guid)
	{
		continued_notice_map_ite ite = m_continued_notice_array.find(notice_guid.server_64);
		if (ite == m_continued_notice_array.end())
		{
			return;
		}
		m_continued_notice_array.erase(ite);
	}

	void event_ws_mgr::send_notice_to_all_only_string(xstring& string_content)
	{
		chat_proto_chat_text msg;
		int32 text_len = string_content.length();
		if (text_len > globel_message_max_size)
		{
			text_len = globel_message_max_size;
		}
		msg.set_text(string_content);
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_receive_globel_message);
	}

	void event_ws_mgr::send_notice_to_all(int32 notice_id, int32 notice_create_time, guid_64 sender_guid, std::string final_string,int32 sender_template_id)
	{
		// 封消息包
 		chat_proto_notice_info msg;
		msg.add_role_guid(sender_guid.A);
		msg.add_role_guid(sender_guid.B);
		msg.set_notice_id(notice_id);
		msg.set_create_time(notice_create_time);
		msg.set_final_string(final_string);
		msg.set_sender_template_id(sender_template_id);

		// 消息类型
		uint32 header = e_msgindex_s2c_receive_notice;

		// 判断发送对象
		NoticeTemplate* notice_template_ptr = GET_TEMPLATE(NoticeTemplate, notice_id);
		if (notice_template_ptr == nullptr)
		{
			return;
		}
		switch (notice_template_ptr->VisibleType)
		{
		case 0:// 全服
		case 5://世界
			send_notice_to_everyone(&msg, header);
			break;
		case 1:// 军团
			send_notice_to_legion(sender_guid, &msg, header);
			break;
		case 2:// 队伍
			send_notice_to_team(sender_guid, &msg, header);
			break;
		case 3:// 自己
			send_notice_to_self(sender_guid, &msg, header);
			break;
		default:
			break;
		}
	}

	void event_ws_mgr::send_notice_to_all(int32 notice_id, int32 notice_create_time, guid_64 sender_guid, std::string final_string, s_item_info* item_data, int32 item_num,int32 sender_template_id)
	{
		// 封消息包
		chat_proto_notice_info msg;
		msg.add_role_guid(sender_guid.A);
		msg.add_role_guid(sender_guid.B);
		msg.set_notice_id(notice_id);
		msg.set_create_time(notice_create_time);
		msg.set_final_string(final_string);
		msg.set_sender_template_id(sender_template_id);
		if (item_num >= 0 && item_num <= 4)
		{
			for (int i = 0; i <= item_num; i++)
			{
				for (int32 j = 0; j < e_item_info_max; ++j)
				{
					msg.add_item_data(item_data[i].data_ary[j]);
				}
			}
			item_data->clear_data();
		}

		if (msg.item_data().size() > e_item_info_max)
		{
			return;
		}

		// 消息类型
		uint32 header = e_msgindex_s2c_receive_notice;

		// 判断发送对象
		NoticeTemplate* notice_template_ptr = GET_TEMPLATE(NoticeTemplate, notice_id);
		if (notice_template_ptr == nullptr)
		{
			return;
		}
		switch (notice_template_ptr->VisibleType)
		{
		case 0:// 全服
		case 5:
			send_notice_to_everyone(&msg, header);
			break;
		case 1:// 军团
			send_notice_to_legion(sender_guid, &msg, header);
			break;
		case 2:// 队伍
			send_notice_to_team(sender_guid, &msg, header);
			break;
		case 3:// 自己
			send_notice_to_self(sender_guid, &msg, header);
			break;
		default:
			break;
		}
	}

	void event_ws_mgr::send_notice_to_everyone(google::protobuf::Message* net_pro, uint32 header)
	{
		client_session_mgr::getInstance().send_message_to_all_client(net_pro, header);
	}

	void event_ws_mgr::send_notice_with_text(std::string notice_text, int32 notice_create_time, int32 notice_show_typ)
	{


	}

	void event_ws_mgr::send_mail_to_player_by_system(guid_64 terget_player_guid,
		int32 target_server_id,
		std::string sender_name,
		std::string title,
		std::string content_text,
		int32 money_typ1,
		int32 money_num1,
		int32 money_typ2,
		int32 money_num2,
		const std::vector<s_item_info>& item_list)
	{
		if (!terget_player_guid.is_valid())
		{
			return;
		}

		s_mail_info temp_mail_info;

		temp_mail_info.mail_guid = guid_gen::make_guid(terget_player_guid);

		for (int32 i = 0; i < item_list.size(); ++i)
		{
			temp_mail_info.data_ary[i * 2 + EMailInfo_ItemGuid11] = item_list[i].item_guid.A;
			temp_mail_info.data_ary[i * 2 + EMailInfo_ItemGuid12] = item_list[i].item_guid.B;
		}
		temp_mail_info.data_ary[EMailInfo_MoneyTyp1] = money_typ1;
		temp_mail_info.data_ary[EMailInfo_MoneyNum1] = money_num1;
		temp_mail_info.data_ary[EMailInfo_MoneyTyp2] = money_typ2;
		temp_mail_info.data_ary[EMailInfo_MoneyNum2] = money_num2;

		temp_mail_info.data_ary[EMailInfo_ReadState] = 0;

		temp_mail_info.data_ary[EMailInfo_SenderGuid1] = 0;
		temp_mail_info.data_ary[EMailInfo_SenderGuid2] = 0;
		temp_mail_info.data_ary[EMailInfo_SendTime] = time_helper::get_cur_time_new().second;

		int32 string_len = 0;
		if (content_text.length() > max_text_contents_length)
		{
			string_len = max_text_contents_length;
		}
		else
		{
			string_len = content_text.length();
		}
		memcpy(temp_mail_info.text_contents, content_text.c_str(), string_len);

		if (title.length() > max_mail_title_length)
		{
			string_len = max_mail_title_length;
		}
		else
		{
			string_len = title.length();
		}
		memcpy(temp_mail_info.mail_title, title.c_str(), string_len);

		if (sender_name.length() > max_name_size)
		{
			string_len = max_name_size;
		}
		else
		{
			string_len = sender_name.length();
		}
		memcpy(temp_mail_info.sender_name, sender_name.c_str(), string_len);


		// nullptr说明不在线/不存在，cs_uid = 0 说明不在CS上（比如传送中）
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(terget_player_guid);
		if ((client_session_ptr != nullptr) && (client_session_ptr->get_cs_conn_index() >= 0))
		{
			ws2cs_send_mail request;
			request.addressee_guid = terget_player_guid;
			request.item_num = item_list.size();
			request.mail_info = temp_mail_info;
			for (int32 i = 0; i < item_list.size(); ++i)
			{
				if (request.item_num > max_item_per_mail)
				{
					break;
				}
				request.item_list[i] = item_list[i];
			}
			bool is_use = proto_by_lua(e_msgindex_ws2cs_recive_mail);
			if (is_use == false)
			{
				client_session_ptr->send_to_cs(&request, sizeof(request));
			}
			else
			{
				faith::ws2cs_proto::send_mail pro_msg;
				request.to_proto(pro_msg);
				client_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_recive_mail);
			}
		}

		ws2dp_add_mail_to_sql msg;
		msg.address_server_id = target_server_id;
		msg.mail_info = temp_mail_info;
		std::string role_guid_str = init_unit::change_i64_to_string(terget_player_guid.server_64);
		memcpy(msg.role_mark, role_guid_str.c_str(), role_guid_str.size());
		msg.mark_type = e_unit_guid_role_guid;
		msg.item_num = item_list.size();
		for (int32 i = 0; i < item_list.size(); ++i)
		{
			msg.item_list[i] = item_list[i];
		}

		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}


	void event_ws_mgr::send_notice_to_legion(guid_64 sender_guid, google::protobuf::Message* net_pro, uint32 header)
	{
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(sender_guid);
		if (nullptr == legion_ws_ptr)
		{
			legion_ws_ptr = legion_ws_mgr_ref.get_legion(sender_guid);
			if (nullptr == legion_ws_ptr)
			{
				return;
			}
		}
		legion_ws_ptr->send_message_to_all_member(net_pro, header);
	}

	void event_ws_mgr::send_notice_to_team(guid_64 sender_guid, google::protobuf::Message* net_pro, uint32 header)
	{
		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		team_ws* team_ws_ptr = team_ws_mgr_ref.get_unit_team(sender_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}
		team_ws_ptr->send_message_to_all_member(net_pro, header);
	}

	void event_ws_mgr::send_notice_to_self(guid_64 sender_guid, google::protobuf::Message* net_pro, uint32 header)
	{
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(sender_guid);
		if (client_session_ptr)
		{
			client_session_ptr->send_to_client(net_pro, header);
		}
	}

	void event_ws_mgr::set_mail_proto_info(server2dp_proto_s_mail_info * mail_info, s_mail_info _info)
	{
		if (nullptr == mail_info)
		{
			return;
		}
		mail_info->set_role_guid(_info.mail_guid.server_64);
		mail_info->set_text_contents(_info.text_contents);
		mail_info->set_sender_name(_info.sender_name);
		mail_info->set_mail_title(_info.mail_title);
		for (int32 i = 0; i < EMailInfo_max; ++i)
		{
			mail_info->add_data_ary(_info.data_ary[i]);
		}
	}

	void event_ws_mgr::send_notice_with_param(guid_64 sender_guid, const xchar* sender_name, const xchar* content_text, 
		const std::vector<int32>& param_array, int32 param_type, int32 chat_type, int32 sender_template_id, int32 notice_id)
	{
		chat_proto_notice_info_with_param msg;
		msg.set_sender_name(sender_name);
		msg.set_sender_guid(sender_guid.server_64);
		msg.set_content_text(content_text);
		for (int32 i = 0; i < param_array.size(); ++i)
		{
			msg.add_param_array(param_array[i]);
		}
		msg.set_param_type(param_type);
		msg.set_chat_type(chat_type);
		msg.set_sender_template_id(sender_template_id);
		msg.set_notice_id(notice_id);
		msg.set_create_time(time_helper::get_cur_time_new().second);
		switch (chat_type)
		{
		case e_chat_type_system:
		case e_chat_type_world:
		{
			client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_receive_notice_with_param);
		}
		break;
		case e_chat_type_legion:
		{
			legion_ws* temp_legion = legion_ws_mgr::get_instance().get_unit_legion(sender_guid);
			if (temp_legion != nullptr)
			{
				temp_legion->send_message_to_all_member(&msg, e_msgindex_s2c_receive_notice_with_param);
			}
		}
		break;
		case e_chat_type_team:
		{
			team_ws* temp_team = team_ws_mgr::get_instance().get_unit_team(sender_guid);
			if (temp_team != nullptr)
			{
				temp_team->send_message_to_all_member(&msg, e_msgindex_s2c_receive_notice_with_param);
			}
		}
		break;
		default:
			break;
		}
	}

	int32 event_ws_mgr::get_empty_globel_mail_slot()
	{
		for (int32 i = 0; i < max_globel_mail_in_same_time; ++i)
		{
			if (!m_globel_mail_array[i].mail_guid.is_valid())
			{
				return i;
			}
		}
		return -1;
	}
}
