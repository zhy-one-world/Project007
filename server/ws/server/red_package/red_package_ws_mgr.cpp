#include "red_package_ws_mgr.h"
#include "utility/cs_date.hpp"
#include "utility/init_unit.h"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "../mail/event_ws_mgr.h"
#include "ws_client.hpp"
#include "server/client_session.hpp"
#include "server/client_session_mgr.hpp"
#include "server/legion/legion_ws_mgr.h"
#include "../cache_ws_mgr.hpp"

#include "internet/character.pb.h"
#include "event_msg.hpp"
#include "net.pb.h"
namespace faith
{
	red_package_ws_mgr*	red_package_ws_mgr::s_red_package_ws_mgr_ptr = nullptr;

	red_package_ws_mgr::red_package_ws_mgr()
	{
		clear_data();
	}

	red_package_ws_mgr::~red_package_ws_mgr()
	{


	}
	void red_package_ws_mgr::clear_data()
	{
		tick_count = 0;
		auto_save_count = 0;
		memset(red_package_arr, 0, sizeof(red_package_arr));
	}

	void red_package_ws_mgr::tick(float dt)
	{
		if (is_use_lua())
		{
			return;
		}
		tick_count += dt;
		auto_save_count += dt;
		if (tick_count > 1000)
		{
			tick_count = 0;

			int32 cur_time = time_helper::get_cur_time_new().second;
			for (int32 i = 0; i < red_package_total_num; ++i)
			{
				if (red_package_arr[i] == nullptr)
				{
					continue;
				}
				if (!red_package_arr[i]->get_guid().is_valid())
				{
					continue;
				}
				if (red_package_arr[i]->get_remain_times() <= 0
					|| cur_time - red_package_arr[i]->get_create_time() > GAMECONFIG->RedPackageAutoDelTime)
				{
					character_proto_del_red_package msg;
					msg.add_red_package_guid(red_package_arr[i]->get_guid().A);
					msg.add_red_package_guid(red_package_arr[i]->get_guid().B);
					client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_del_red_bag_info);

					server2dp_proto_ws2dp_del_red_package_by_sql del_msg;
					del_msg.set_red_package_guid(red_package_arr[i]->get_guid().server_64);
					ws_client::getInstance().send_to_dp_new(&del_msg, e_msg_ws2dp_del_red_pack_by_sql);
					//ws2dp_del_red_package_by_sql del_msg;
					//del_msg.red_package_guid = red_package_arr[i]->get_guid();
					//ws_client::getInstance().send_to_dp(&del_msg, sizeof(ws2dp_del_red_package_by_sql));

					cache_ws_mgr::get_instance().back_red_package_ws_ptr(red_package_arr[i]);
					red_package_arr[i] = nullptr;
				}
			}


			if (auto_save_count > 600000)
			{
				save_info_to_db();
				auto_save_count = 0;
			}
		}
	}

	int32 red_package_ws_mgr::find_empty_slot()
	{
		for (int32 i = 0; i < red_package_total_num; ++i)
		{
			if (red_package_arr[i]==nullptr || red_package_arr[i]->get_template_id() == 0)
			{
				return i;
			}
		}
		return -1;
	}

	int32  red_package_ws_mgr::get_oldest_red_package_slot()
	{
		int32 oldest_create_time = 0;
		int32 solt = -1;
		for (int32 i = 0; i < red_package_total_num; ++i)
		{
			if (red_package_arr[i] != nullptr && red_package_arr[i]->get_template_id() != 0)
			{
				if (oldest_create_time == 0)
				{
					oldest_create_time = red_package_arr[i]->get_create_time();
					solt = i;
				}
				else if (red_package_arr[i]->get_create_time() < oldest_create_time)
				{
					oldest_create_time = red_package_arr[i]->get_create_time();
					solt = i;
				}	
			}
		}

		return solt;
	}

	int32 red_package_ws_mgr::get_suit_slot()
	{
		int32 suit_solt = find_empty_slot();
		if (-1 == suit_solt)
		{
			suit_solt = get_oldest_red_package_slot();
			if (suit_solt != -1 && red_package_arr[suit_solt] != nullptr)
			{
				character_proto_del_red_package msg;
				msg.add_red_package_guid(red_package_arr[suit_solt]->get_guid().A);
				msg.add_red_package_guid(red_package_arr[suit_solt]->get_guid().B);
				client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_del_red_bag_info);

				server2dp_proto_ws2dp_del_red_package_by_sql del_msg;
				del_msg.set_red_package_guid(red_package_arr[suit_solt]->get_guid().server_64);
				ws_client::getInstance().send_to_dp_new(&del_msg, e_msg_ws2dp_del_red_pack_by_sql);

				//ws2dp_del_red_package_by_sql del_msg;
				//del_msg.red_package_guid = red_package_arr[suit_solt]->get_guid();
				//ws_client::getInstance().send_to_dp(&del_msg, sizeof(ws2dp_del_red_package_by_sql));

				cache_ws_mgr::get_instance().back_red_package_ws_ptr(red_package_arr[suit_solt]);
				red_package_arr[suit_solt] = nullptr;
			}
		}

		return suit_solt;
	}
	int32 red_package_ws_mgr::add_red_package(int32 red_package_template_id, guid_64 sender_guid, xstring sender_name, xstring text)
	{
		if (is_use_lua())
		{
			return e_item_string_unkown;
		}
		RedPackageTemplate* red_pack_template_ptr = GET_TEMPLATE(RedPackageTemplate, red_package_template_id);
		if (red_pack_template_ptr == nullptr)
		{
			return e_item_string_unkown;
		}

		client_session* session = client_session_mgr::getInstance().get_session(sender_guid);
		if (session == nullptr)
		{
			return e_item_string_unkown;
		}

		int32 empty_slot = get_suit_slot();
		if (empty_slot < 0
			|| empty_slot >= red_package_total_num)
		{
			return e_red_bag_no_more_space;
		}
		red_package_ws* red_package_ws_ptr = cache_ws_mgr::get_instance().get_red_package_ws_ptr();
		if (nullptr == red_package_ws_ptr)
		{
			return e_red_bag_no_more_space;
		}
		red_package_arr[empty_slot] = red_package_ws_ptr;
		int32 add_result = red_package_ws_ptr->init_by_info(red_package_template_id, sender_guid, session->m_role_info.data_ary[e_role_info_vip_level],sender_name, text);
		if (add_result == e_red_bag_send_success ||
			add_result == e_red_bag_get_globel_success ||
			add_result == e_red_bag_get_legion_success ||
			add_result == e_red_bag_get_vip_success)
		{
			server2dp_proto_ws2dp_add_red_package_to_sql add_msg;
			set_red_package_base_proto(add_msg.mutable_red_info(), red_package_ws_ptr->get_complete_data().base_info);
			ws_client::getInstance().send_to_dp_new(&add_msg, e_msg_ws2dp_add_red_pack_to_sql);

			//ws2dp_add_red_package_to_sql add_msg;
			//add_msg.red_package_info = red_package_ws_ptr->get_complete_data().base_info;

			//ws_client::getInstance().send_to_dp(&add_msg, sizeof(ws2dp_add_red_package_to_sql));

			send_info_to_all(empty_slot);

			int32 notice_id = red_pack_template_ptr->NoTiceId;
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(add_msg.red_info().sender_name());
			vec_notice_str.push_back(add_msg.red_info().text());
			std::string notice_str = init_unit::implode(vec_notice_str);
			int32 create_time = time_helper::get_cur_time_new().second;
			event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, red_package_ws_ptr->get_complete_data().base_info.sender_guid, notice_str);
		}

		return add_result;
	}

	void red_package_ws_mgr::get_red_package(guid_64 red_package_guid, guid_64 role_guid, xstring role_name)
	{
		if (is_use_lua())
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (session == nullptr)
		{
			return;
		}

		int32 target_index = find_package_index(red_package_guid);
		if (target_index < 0
			|| target_index >= red_package_total_num
			|| red_package_arr[target_index] == nullptr)
		{
			ws2cs_get_red_package_end msg;
			msg.receiver_guid = role_guid;
			msg.get_result = e_red_bag_alread_done;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_get_red_pack_end);
			if (is_use == false)
			{
				session->send_to_cs(&msg, sizeof(ws2cs_get_red_package_end));
			}
			else
			{
				ws2cs_proto::get_red_package_end pro_msg;
				msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_get_red_pack_end);
			}
			return;
		}
		if (red_package_arr[target_index]->get_remain_times() <= 0)
		{
			ws2cs_get_red_package_end msg;
			msg.receiver_guid = role_guid;
			msg.get_result = e_red_bag_alread_done;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_get_red_pack_end);
			if (is_use == false)
			{
				session->send_to_cs(&msg, sizeof(ws2cs_get_red_package_end));
			}
			else
			{
				ws2cs_proto::get_red_package_end pro_msg;
				msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_get_red_pack_end);
			}
			return;
		}

		if (red_package_arr[target_index]->is_in_receiver_list(role_guid))
		{
			ws2cs_get_red_package_end msg;
			msg.receiver_guid = role_guid;
			msg.red_package_guid = red_package_arr[target_index]->get_guid();
			msg.get_result = e_red_bag_cant_draw_again;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_get_red_pack_end);
			if (is_use == false)
			{
				session->send_to_cs(&msg, sizeof(ws2cs_get_red_package_end));
			}
			else
			{
				ws2cs_proto::get_red_package_end pro_msg;
				msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_get_red_pack_end);
			}
			return;
		}

		int32 draw_num = red_package_arr[target_index]->draw_red_package(role_guid, role_name);
		if (draw_num > 0)
		{
			//send_info_to_all(target_index);
			send_new_draw_info(target_index, role_guid, role_name, draw_num);

			ws2cs_get_red_package_end msg;
			msg.receiver_guid = role_guid;
			msg.red_package_guid = red_package_arr[target_index]->get_guid();
			msg.get_result = e_red_bag_get_success;
			msg.money_type = red_package_arr[target_index]->get_money_type();
			msg.money_num = draw_num;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_get_red_pack_end);
			if (is_use == false)
			{
				session->send_to_cs(&msg, sizeof(ws2cs_get_red_package_end));
			}
			else
			{
				ws2cs_proto::get_red_package_end pro_msg;
				msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_get_red_pack_end);
			}
		}
	}

	void red_package_ws_mgr::send_new_draw_info(int32 index, guid_64 role_guid, xstring role_name, int32 money_num)
	{
		if (index < 0
			|| index >= red_package_total_num
			|| red_package_arr[index] == nullptr
			|| !red_package_arr[index]->get_guid().is_valid())
		{
			return;
		}
		character_proto_update_new_draw_info msg;
		msg.add_red_package_guid(red_package_arr[index]->get_guid().A);
		msg.add_red_package_guid(red_package_arr[index]->get_guid().B);
		msg.set_remain_money_num(red_package_arr[index]->get_remain_num());
		msg.set_remain_get_times(red_package_arr[index]->get_remain_times());
		msg.add_receiver_guid(role_guid.A);
		msg.add_receiver_guid(role_guid.B);
		msg.set_receiver_name(role_name);
		msg.set_draw_num(money_num);
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_update_new_draw_info);
	}

	void red_package_ws_mgr::send_info_to_all(int32 index)
	{
		if (index < 0
			|| index >= red_package_total_num)
		{
			return;
		}
		if (red_package_arr[index] == nullptr)
		{
			return;
		}
		int32 temp_template_id = red_package_arr[index]->get_template_id();
		if (temp_template_id > 0)
		{
			RedPackageTemplate* red_pack_template_ptr = GET_TEMPLATE(RedPackageTemplate, temp_template_id);
			if (red_pack_template_ptr != nullptr)
			{
				switch (red_pack_template_ptr->Type)
				{
				case e_red_package_type_globel:
				case e_red_package_type_vip:
				{
					character_proto_update_red_pack_info globel_msg;
					add_info_to_msg(globel_msg, index);
					client_session_mgr::getInstance().send_message_to_all_client(&globel_msg, e_msgindex_s2c_update_red_bag_info);
				}
				break;
				case e_red_package_type_legion:
				{
					character_proto_update_red_pack_info legion_msg;
					add_info_to_msg(legion_msg, index);
					legion_ws* temp_legion = legion_ws_mgr::get_instance().get_unit_legion(red_package_arr[index]->get_sender_guid());
					if (temp_legion != nullptr)
					{
						temp_legion->send_message_to_all_member(&legion_msg, e_msgindex_s2c_update_red_bag_info);
					}
				}
				break;
				default:
					break;
				}
			}
		}
	}

	void red_package_ws_mgr::send_info_to_one(guid_64 target_guid)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "red_package_send_info_to_one", 0, false, "%l", target_guid.server_64);
			return;

		}
		client_session* target_session = client_session_mgr::getInstance().get_session(target_guid);
		if (target_session == nullptr)
		{
			return;
		}
		character_proto_update_red_pack_info msg;
		for (int32 i = 0; i < red_package_total_num; ++i)
		{
			if (nullptr == red_package_arr[i])
			{
				continue;
			}
			int32 temp_template_id = red_package_arr[i]->get_template_id();
			RedPackageTemplate* red_pack_template_ptr = GET_TEMPLATE(RedPackageTemplate, temp_template_id);
			if (red_pack_template_ptr == nullptr)
			{
				continue;
			}
			switch (red_pack_template_ptr->Type)
			{
			case e_red_package_type_globel:
			case e_red_package_type_vip:
				add_info_to_msg(msg, i);
				if (msg.ByteSize() > packege_length_max)
				{
					target_session->send_to_client(&msg, e_msgindex_s2c_update_red_bag_info);
					msg.clear_info_list();
				}
				break;
			case e_red_package_type_legion:
			{
				//bool is_in_legion = false;
				guid_64 sender_guid = red_package_arr[i]->get_sender_guid();
				legion_ws* temp_legion = legion_ws_mgr::get_instance().get_unit_legion(sender_guid);
				if (temp_legion == nullptr)
				{
					continue;;
				}
				if (temp_legion->is_in_this_legion(target_guid))
				{
					add_info_to_msg(msg, i);
					if (msg.ByteSize() > packege_length_max)
					{
						target_session->send_to_client(&msg, e_msgindex_s2c_update_red_bag_info);
						msg.clear_info_list();
					}
				}
			}
				break;
			default:
				break;
			}
			if (msg.ByteSize() + red_package_msg_max_lenght > packege_length_max)
			{
				target_session->send_to_client(&msg, e_msgindex_s2c_update_red_bag_info);
				msg.clear_info_list();
			}
		}
		if (msg.info_list_size() > 0)
		{
			target_session->send_to_client(&msg, e_msgindex_s2c_update_red_bag_info);
		}
	}

	void red_package_ws_mgr::add_info_to_msg(character_proto_update_red_pack_info& msg, int32 index)
	{
		if (index < 0
			|| index >= red_package_total_num
			|| red_package_arr[index] == nullptr
			|| red_package_arr[index]->get_template_id() == 0)
		{
			return;
		}
		character_proto_red_pack_info* red_package_info_ptr = msg.add_info_list();
		red_package_info_ptr->add_red_pack_guid(red_package_arr[index]->get_guid().A);
		red_package_info_ptr->add_red_pack_guid(red_package_arr[index]->get_guid().B);

		red_package_info_ptr->set_template_id(red_package_arr[index]->get_template_id());

		red_package_info_ptr->add_sender_guid(red_package_arr[index]->get_sender_guid().A);
		red_package_info_ptr->add_sender_guid(red_package_arr[index]->get_sender_guid().B);

		red_package_info_ptr->set_create_time(red_package_arr[index]->get_create_time());
		red_package_info_ptr->set_sender_name(red_package_arr[index]->get_sender_name());
		red_package_info_ptr->set_send_text(red_package_arr[index]->get_send_text());
		red_package_info_ptr->set_remain_times(red_package_arr[index]->get_remain_times());
		red_package_info_ptr->set_remain_money_num(red_package_arr[index]->get_remain_num());

		for (int32 i = 0; i < red_package_max_receiver_num; ++i)
		{
			s_red_package_receiver* temp_receiver_list = red_package_arr[index]->get_receiver_list();
			s_red_package_receiver temp_receiver = temp_receiver_list[i];
			if (!temp_receiver.receiver_guid.is_valid())
			{
				continue;
			}
			character_proto_red_pack_receiver_info* temp_receiver_info = red_package_info_ptr->add_receiver_list();
			temp_receiver_info->add_receiver_guid(temp_receiver.receiver_guid.A);
			temp_receiver_info->add_receiver_guid(temp_receiver.receiver_guid.B);

			temp_receiver_info->set_receiver_name(temp_receiver.role_name);
			temp_receiver_info->set_money_num(temp_receiver.money_num);
		}
	}

	void red_package_ws_mgr::save_info_to_db()
	{
		server2dp_proto_ws2dp_save_red_package msg;
		int32 data_num_count = 0;
		for (int32 i = 0; i < red_package_total_num; ++i)
		{
			
			if (nullptr == red_package_arr[i])
			{
				continue;
			}
			if (red_package_arr[i]->get_guid().is_valid())
			{
				server2dp_proto_s_red_package* red_package = msg.add_data_ary();
				if (nullptr == red_package)
				{
					continue;
				}
				set_red_package_proto(red_package, red_package_arr[i]->get_complete_data());
				data_num_count++;
			}
		}
		msg.set_data_num(data_num_count);
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_red_pack);
		//ws2dp_save_red_package msg;
		//int32 data_num_count = 0;
		//for (int32 i = 0; i < red_package_total_num; ++i)
		//{
		//	if (nullptr == red_package_arr[i])
		//	{
		//		continue;
		//	}
		//	if (red_package_arr[i]->get_guid().is_valid())
		//	{
		//		msg.red_package_list[data_num_count] = red_package_arr[i]->get_complete_data();
		//		data_num_count++;
		//	}
		//}
		//msg.data_num = data_num_count;

		//ws_client::getInstance().send_to_dp(&msg, msg.get_pak_length());
	}

	void red_package_ws_mgr::load_info_by_db()
	{
		server2dp_proto_ws2dp_load_red_package msg;
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_red_pack);

		//ws2dp_load_red_package load_msg;
		//ws_client::getInstance().send_to_dp(&load_msg, sizeof(ws2dp_load_red_package));
	}

	void red_package_ws_mgr::load_receiver_by_db(guid_64 red_package_guid)
	{
		server2dp_proto_ws2dp_load_red_package_receiver msg;
		msg.set_red_package_guid(red_package_guid.server_64);
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_red_pack_receiver);

		//ws2dp_load_red_package_receiver load_msg;
		//load_msg.red_package_guid = red_package_guid;
		//ws_client::getInstance().send_to_dp(&load_msg, sizeof(ws2dp_load_red_package_receiver));
	}

	int32 red_package_ws_mgr::find_package_index(guid_64 red_package_guid)
	{
		for (int32 i = 0; i < red_package_total_num; ++i)
		{
			if (nullptr == red_package_arr[i])
			{
				continue;
			}
			if (red_package_arr[i]->get_guid() == red_package_guid)
			{
				return i;
			}
		}
		return -1;
	}

	void red_package_ws_mgr::load_red_package_end(const s_red_package_base* data_list, int32 data_num)
	{
		if (is_use_lua())
		{
			return;
		}
		for (int32 i = 0; i < data_num; ++i)
		{
			int32 target_index = find_package_index(data_list[i].red_package_guid);
			if (target_index >= 0)
			{
				continue;
			}
			int32 empty_slot = find_empty_slot();
			if (empty_slot < 0
				|| empty_slot >= red_package_total_num)
			{
				break;
			}
			red_package_ws* red_package_ws_ptr = cache_ws_mgr::get_instance().get_red_package_ws_ptr();
			if (nullptr == red_package_ws_ptr)
			{
				break;
			}
			red_package_arr[empty_slot] = red_package_ws_ptr;
			red_package_ws_ptr->set_base_info(data_list[i]);

			if (data_list[i].remain_times > 0)
			{
				load_receiver_by_db(data_list[i].red_package_guid);
			}
		}
	}
	void red_package_ws_mgr::load_red_package_receiver_end(const s_red_package_receiver* data_list, int32 data_num)
	{
		if (is_use_lua())
		{
			return;
		}
		if (data_num <= 0
			|| data_num >= red_package_max_receiver_num)
		{
			return;
		}
		guid_64 red_package_guid = data_list[0].red_package_guid;
		int32 target_index = find_package_index(red_package_guid);
		if (target_index < 0
			|| target_index >= red_package_total_num
			|| nullptr == red_package_arr[target_index] )
		{
			return;
		}

		for (int32 i = 0; i < data_num; ++i)
		{
			red_package_arr[target_index]->add_receiver(data_list[i]);
		}
	}

	void red_package_ws_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "red_package_change_player_name", 0, false, "%l%s", role_guid.server_64, role_name.c_str());
			return;
		}
		for (int32 i = 0; i < red_package_total_num; ++i)
		{
			if (nullptr == red_package_arr[i])
			{
				continue;
			}
			red_package_arr[i]->change_player_name_func(role_guid, role_name);
		}
	}
	void red_package_ws_mgr::set_red_package_base_proto(server2dp_proto_s_red_package_base * proto_info, s_red_package_base data_info)
	{
		if (nullptr == proto_info)
		{
			return;
		}
		proto_info->set_red_package_guid(data_info.red_package_guid.server_64);
		proto_info->set_red_package_template_id(data_info.red_package_template_id);
		proto_info->set_sender_guid(data_info.sender_guid.server_64);
		proto_info->set_create_time(data_info.create_time);
		proto_info->set_sender_name(data_info.sender_name);
		proto_info->set_text(data_info.text);
		proto_info->set_sender_vip_level(data_info.sender_vip_level);
		proto_info->set_remain_times(data_info.remain_times);
		proto_info->set_remain_money_num(data_info.remain_money_num);
	}
	void red_package_ws_mgr::set_red_package_receiver_proto(server2dp_proto_s_red_package_receiver * proto_info, s_red_package_receiver data_info)
	{
		if (nullptr == proto_info)
		{
			return;
		}
		proto_info->set_red_package_guid(data_info.red_package_guid.server_64);
		proto_info->set_receiver_guid(data_info.receiver_guid.server_64);
		proto_info->set_role_name(data_info.role_name);
		proto_info->set_money_num(data_info.money_num);
	}
	void red_package_ws_mgr::set_red_package_proto(server2dp_proto_s_red_package * proto_info, s_red_package data_info)
	{
		if (nullptr == proto_info)
		{
			return;
		}
		set_red_package_base_proto(proto_info->mutable_base_info(), data_info.base_info);
		for (int32 i = 0; i < data_info.get_receiver_num(); ++i)
		{
			if (i >= red_package_max_receiver_num)
			{
				break;
			}
			server2dp_proto_s_red_package_receiver* receiver_info = proto_info->add_info_list();
			if (nullptr == receiver_info)
			{
				continue;
			}
			set_red_package_receiver_proto(receiver_info, data_info.receiver_info_list[i]);
		}

	}

	bool red_package_ws_mgr::is_use_lua()
	{
		static int is_get = 0;
		static bool getRet = false;
		if (is_get == 0)
		{
			bool bRet = false;
			script_mgr::get_instance().call_func(nullptr, "is_red_use_lua", 1, false, ">%b", &bRet);
			getRet = bRet;
			is_get = 1;
			return bRet;
		}
		return getRet;
	}
}