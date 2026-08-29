#include "logic/unit_man.h"
#include "logic/player.hpp"
#include "logic/item_set.h"
#include "person_information_mgr.h"
#include "server_log.hpp"
#include "invalid_word/invalid_ansi_word.h"
#include "internal/char_msg.hpp"
#include "internet/character.pb.h"
#include "internet/net.pb.h"
#include "connection_mgr.hpp"
#include "utility/parse_msg.h"
#include "internet/cs2dp.pb.h"

namespace faith
{
	person_information_mgr::person_information_mgr()
	{
		m_array_index = 0;
	}
	person_information_mgr::~person_information_mgr()
	{

	}
	void person_information_mgr::clear_data()
	{
		m_cur_person_info.clear_data();
	}
	bool person_information_mgr::check_person_information(xstring personalized_signature,xstring city, xstring hometown)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (invalid_ansi_word::is_valid_ansi_str(hometown) == false || invalid_ansi_word::include_invalid_ansi_str(hometown))
		{
			player_ref.send_notice("90204180");
			return false;
		}
		if (invalid_ansi_word::is_valid_ansi_str(city) == false || invalid_ansi_word::include_invalid_ansi_str(city))
		{
			player_ref.send_notice("90204181");
			return false;
		}
		if (invalid_ansi_word::is_valid_ansi_str(personalized_signature) == false || invalid_ansi_word::include_invalid_ansi_str(personalized_signature))
		{
			player_ref.send_notice("90204182");
			return false;
		}
		if (personalized_signature.size() > 60)
		{
			player_ref.send_notice("90096799");
			return false;
		}
		if (city.size() > 24)
		{
			player_ref.send_notice("90096799");
			return false;
		}
		if (hometown.size() > 24)
		{
			player_ref.send_notice("90096799");
			return false;
		}
		return true;
	}
	void person_information_mgr::mofify_person_information_lua(xstring gender, xstring personalized_signature, int32 birthday_year, int32 birthday_mouth, int32 birthday_day, xstring city, xstring hometown, xstring online_state)
	{
		m_cur_person_info.set_gender(gender);
		m_cur_person_info.set_personalized_signature(personalized_signature);
		m_cur_person_info.birthday_year = birthday_year;
		m_cur_person_info.birthday_mouth = birthday_mouth;
		m_cur_person_info.birthday_day = birthday_day;
		m_cur_person_info.set_city(city);
		m_cur_person_info.set_hometown(hometown);
		m_cur_person_info.set_online_state(online_state);

		if (m_cur_person_info.infor_state == 0)
		{
			m_cur_person_info.infor_state = 1;//已修改
		}
		if (m_cur_person_info.reward_state == 0)
		{
			m_cur_person_info.reward_state = 1;//可领取
		}
	}
	void person_information_mgr::clear_person_information_tags()
	{
		m_cur_person_info.clear_tags();
	}
	void person_information_mgr::add_person_information_tag(int32 tag_id)
	{
		m_cur_person_info.add_tag(tag_id);
	}
	void person_information_mgr::get_person_information_to_dp()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		cs2dp_get_person_information request;
		request.role_guid = player_ref.get_unit_guid();
		request.unit_array_index = m_array_index;
		player_ref.send_message_to_dp(&request, sizeof(request));
	}
	void person_information_mgr::get_other_person_information_to_dp(guid_64 target_guid, bool is_ask_player_base_info)
	{

		player& target_ref = unit_man::get_player(target_guid);
		if (true == target_ref.is_valid() && target_ref.get_session_state() == e_session_status_in_gaming)
		{
			//在线
			s_role_person_information role_info = target_ref.get_person_infor_mgr().get_person_info();
			faith::character_proto_sync_person_information msg;
			msg.set_is_main_player(false);
			msg.set_infor_state(role_info.infor_state);
			msg.set_reward_state(role_info.reward_state);
			msg.set_gender(role_info.gender);
			msg.set_personalized_signature(role_info.personalized_signature);
			msg.set_birthday_year(role_info.birthday_year);
			msg.set_birthday_mouth(role_info.birthday_mouth);
			msg.set_birthday_day(role_info.birthday_day);
			msg.set_city(role_info.city);
			msg.set_hometown(role_info.hometown);
			msg.set_online_state(role_info.online_state);
			msg.set_in_role_info_panel(is_ask_player_base_info);
			for (int32 i = 0; i < person_information_tag_num_max; i++)
			{
				msg.add_tags(role_info.tags[i]);
			}
			player& player_ref = unit_man::get_player(m_array_index);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_sync_person_information);

			cs2ws_get_person_couple couple_msg;
			couple_msg.role_guid = player_ref.get_unit_guid();
			couple_msg.target_guid = target_guid;
			connection_mgr::getInstance().send_to_ws(&couple_msg, sizeof(cs2ws_get_person_couple));
			return;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		cs2dp_get_other_person_information request;
		request.role_guid = player_ref.get_unit_guid();
		request.target_guid = target_guid;
		request.unit_array_index = m_array_index;
		request.is_ask_player_base_info = is_ask_player_base_info;
		player_ref.send_message_to_dp(&request, sizeof(request));
	}
	void person_information_mgr::get_other_person_information_to_dp_end(guid_64 target_guid, s_role_person_information role_info,bool is_ask_player_base_info)
	{
		faith::character_proto_sync_person_information msg;
		msg.set_is_main_player(false);
		msg.set_infor_state(role_info.infor_state);
		msg.set_reward_state(role_info.reward_state);
		msg.set_gender(role_info.gender);
		msg.set_personalized_signature(role_info.personalized_signature);
		msg.set_birthday_year(role_info.birthday_year);
		msg.set_birthday_mouth(role_info.birthday_mouth);
		msg.set_birthday_day(role_info.birthday_day);
		msg.set_city(role_info.city);
		msg.set_hometown(role_info.hometown);
		msg.set_online_state(role_info.online_state);
		msg.set_in_role_info_panel(is_ask_player_base_info);

		for (int32 i = 0; i < person_information_tag_num_max; i++)
		{
			msg.add_tags(role_info.tags[i]);
		}
		player& player_ref = unit_man::get_player(m_array_index);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_sync_person_information);

		cs2ws_get_person_couple couple_msg;
		couple_msg.role_guid = player_ref.get_unit_guid();
		couple_msg.target_guid = target_guid;
		connection_mgr::getInstance().send_to_ws(&couple_msg, sizeof(cs2ws_get_person_couple));
	}

	void person_information_mgr::update_person_infomation(s_role_person_information role_info)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		m_cur_person_info = role_info;
		m_cur_person_info.role_guid = player_ref.get_unit_guid();
		sync_to_ws();
	}

	bool person_information_mgr::load_role_person_infomation_by_db_lua(const char *data_ptr, int32 data_len)
	{

		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_person_information_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_role_person_information db_data;
		db_data.role_guid.server_64 = msg.role_guid();
		db_data.infor_state = msg.infor_state();
		db_data.reward_state = msg.reward_state();

		parse_msg::getInstance().my_memcopy_string(db_data.gender, person_infor_gender_max, msg.gender());
		parse_msg::getInstance().my_memcopy_string(db_data.personalized_signature, person_infor_personalized_signature_max, msg.personalized_signature());
		db_data.birthday_year = msg.birthday_year();
		db_data.birthday_mouth = msg.birthday_mouth();
		db_data.birthday_day = msg.birthday_day();
		parse_msg::getInstance().my_memcopy_string(db_data.city, person_infor_city_max, msg.city());
		parse_msg::getInstance().my_memcopy_string(db_data.hometown, person_infor_hometown_max, msg.hometown());
		parse_msg::getInstance().my_memcopy_string(db_data.online_state, person_infor_online_state_max, msg.online_state());
	
		for (int32 i = 0; i < msg.tags_size(); i++)
		{
			db_data.tags[i] = msg.tags(i);
		}

		update_person_infomation(db_data);

		return true;


	}

	void person_information_mgr::save_to_db(e_save_role_data_type eType)
	{
		player& player_ref = unit_man::get_player(m_array_index);

		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_person_information_to_db);
		if (is_use == false)
		{
			cs2dp_save_person_information_to_db  request;
			request.role_guid = player_ref.get_unit_guid();
			request.unit_array_index = m_array_index;
			request.person_info = m_cur_person_info;
			request.save_type_ex = eType;
			player_ref.send_message_to_dp(&request, sizeof(request));
		}
		else
		{
			faith::cs2dp_proto::save_role_person_information msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(eType);
			faith::cs2dp_proto::role_person_information_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			db_data->set_role_guid(m_cur_person_info.role_guid.server_64);
			db_data->set_infor_state(m_cur_person_info.infor_state);
			db_data->set_reward_state(m_cur_person_info.reward_state);
			db_data->set_gender(m_cur_person_info.gender);
			db_data->set_personalized_signature(m_cur_person_info.personalized_signature);
			db_data->set_birthday_year(m_cur_person_info.birthday_year);
			db_data->set_birthday_mouth(m_cur_person_info.birthday_mouth);
			db_data->set_birthday_day(m_cur_person_info.birthday_day);
			db_data->set_city(m_cur_person_info.city);
			db_data->set_hometown(m_cur_person_info.hometown);
			db_data->set_online_state(m_cur_person_info.online_state);
			for (int32 i = 0; i < person_information_tag_num_max; i++)
			{
				db_data->add_tags(m_cur_person_info.tags[i]);
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_person_information_to_db);
		}

		if (eType == e_save_data_type_exit_game)
		{
			delete_from_ws();
		}
		
	}
	void person_information_mgr::sync_to_client()
	{
		faith::character_proto_sync_person_information msg;
		msg.set_is_main_player(true);
		msg.set_infor_state(m_cur_person_info.infor_state);
		msg.set_reward_state(m_cur_person_info.reward_state);
		msg.set_gender(m_cur_person_info.gender);
		msg.set_personalized_signature(m_cur_person_info.personalized_signature);
		msg.set_birthday_year(m_cur_person_info.birthday_year);
		msg.set_birthday_mouth(m_cur_person_info.birthday_mouth);
		msg.set_birthday_day(m_cur_person_info.birthday_day);
		msg.set_city(m_cur_person_info.city);
		msg.set_hometown(m_cur_person_info.hometown);
		msg.set_online_state(m_cur_person_info.online_state);
		for (int32 i = 0; i < person_information_tag_num_max; i++)
		{
			msg.add_tags(m_cur_person_info.tags[i]);
		}
		player& player_ref = unit_man::get_player(m_array_index);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_sync_person_information);
	}
	void person_information_mgr::get_reward()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (m_cur_person_info.reward_state == 0)
		{
			//不可领取

		}
		else if (m_cur_person_info.reward_state == 1)
		{
			//可领取
			if (GAMECONFIG->PersonInforReward.size() < 2)
			{//开启主线未完成
				return;
			}
			//物品奖励
			std::vector<citem*> item_array;
			std::vector<s_item_template_info> promp_item_data;
			for (int32 i = 0; i * 2 + 1 < GAMECONFIG->PersonInforReward.size(); i++)
			{
				int32 item_template_id = GAMECONFIG->PersonInforReward[i * 2];
				int32 item_num = GAMECONFIG->PersonInforReward[i * 2 + 1];

				ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_template_id);
				if (nullptr == ItemTemplatePtr)
				{
					continue;
				}
				citem* temp_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_item_person_information, 0, item_template_id, item_num, 1);
				if (nullptr != temp_item)
				{
					item_array.push_back(temp_item);
					promp_item_data.push_back({ item_template_id ,item_num ,temp_item->get_data_info(e_item_info_locked) });
				}
			}
			player_ref.get_item_set().put_in_bag(item_array);
			if (promp_item_data.size() > 0)
			{
				player_ref.get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
			}

			m_cur_person_info.reward_state = 2;
			sync_to_client();

		}
		else if (m_cur_person_info.reward_state == 2)
		{
			//已领取
		}
	}

	void person_information_mgr::sync_to_ws()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (!player_ref.is_self_server())
		{
			return;
		}
		cs2ws_sync_person_information msg;
		msg.role_guid = player_ref.get_unit_guid();
		msg.person_info = m_cur_person_info;
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_sync_person_information));
	}
	void person_information_mgr::delete_from_ws()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (!player_ref.is_self_server())
		{
			return;
		}
		cs2ws_delete_person_information msg;
		msg.role_guid = player_ref.get_unit_guid();
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_delete_person_information));
	}
}