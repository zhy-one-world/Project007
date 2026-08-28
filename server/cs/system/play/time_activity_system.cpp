#include "components/play/time_activity_component.h"
#include "internal/char_msg.hpp"
#include "internet/time_activity.pb.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "internet/net.pb.h"
#include "template/template_manager.h"
#include "time_activity_system.h"
#include "wheel_plate_system.h"

using namespace hld;

bool time_activity_system::start_up()
{
	auto sta_cp = g_world_ent->add_component<system_time_activity_component>();
	auto time_activity_table = template_manager::get_instance().get_templates(e_TimeActivityTemplate);
	auto current_time = utility::get_tick_count() / 1000;
	for (auto ite = time_activity_table->begin(); ite != time_activity_table->end(); ++ite)
	{
		auto template_ptr = (TimeActivityTemplate*)(ite->second);
		if (template_ptr->attribute_id <= 0)
		{
			continue;
		}
		if (template_ptr->OpenType <= 0)
		{
			continue;
		}
		if (template_ptr->OpenType == 1)
		{
			tm activity_open_time;
			time_helper::get_time_by_string(template_ptr->NaturalOpenTime.c_str(), &activity_open_time);
			auto open_time_stamp = mktime(&activity_open_time);
			tm activity_end_time;
			time_helper::get_time_by_string(template_ptr->NaturalEndTime.c_str(), &activity_end_time);
			auto end_time_stamp = mktime(&activity_end_time);
			if (current_time < open_time_stamp)
			{
				auto info = std::make_shared<system_time_activity_info>();
				info->activity_id = template_ptr->attribute_id;
				info->open_time = open_time_stamp;
				info->end_time = end_time_stamp;
				bool is_insert = false;
				sta_cp->m_begin_activity.push_back(info);
				sta_cp->m_begin_activity.sort([](const std::shared_ptr<system_time_activity_info>& a, const std::shared_ptr<system_time_activity_info>& b) { return a->open_time < b->open_time; });
			}
			else if (current_time < end_time_stamp)
			{
				auto info = std::make_shared<system_time_activity_info>();
				info->activity_id = template_ptr->attribute_id;
				info->open_time = open_time_stamp;
				info->end_time = end_time_stamp;
				sta_cp->m_run_activity.push_back(info);
				sta_cp->m_run_activity.sort([](const std::shared_ptr<system_time_activity_info>& a, const std::shared_ptr<system_time_activity_info>& b) { return a->end_time < b->end_time; });
			}
		}
	}
	return true;
}
void time_activity_system::shut_down(player* player_ptr)
{
	player_ptr->m_time_activity_component = nullptr;
}
void time_activity_system::heart_tick(const int64& new_time)
{
	auto sta_cp = g_world_ent->get_component<system_time_activity_component>();
	auto current_time = utility::get_tick_count() / 1000;
	bool data_ditry = false;
	for (auto it = sta_cp->m_begin_activity.begin(); it != sta_cp->m_begin_activity.end();)
	{
		auto temp_it = it++;
		if (current_time < (*temp_it)->open_time)
		{
			break;
		}
		auto info = *temp_it;
		sta_cp->m_begin_activity.erase(temp_it);
		sta_cp->m_run_activity.push_back(info);
		sta_cp->m_run_activity.sort([](const std::shared_ptr<system_time_activity_info>& a, const std::shared_ptr<system_time_activity_info>& b) { return a->end_time < b->end_time; });
		data_ditry = true;
	}
	for (auto it = sta_cp->m_run_activity.begin(); it != sta_cp->m_run_activity.end();)
	{
		auto temp_it = it++;
		if (current_time < (*temp_it)->end_time)
		{
			break;
		}
		sta_cp->m_run_activity.erase(temp_it);
		data_ditry = true;
	}
	if (data_ditry)
	{
		unit_man::refresh_all_time_activity();
	}
}
void time_activity_system::load_data_from_db(player* player_ptr, const time_activity_s2s_sl_activity_info& msg)
{
	auto sta_cp = g_world_ent->get_component<system_time_activity_component>();
	player_ptr->m_time_activity_component = std::make_shared<time_activity_component>();
	for (auto& it : msg.data_list())
	{
		for (auto& system_it : sta_cp->m_run_activity)
		{
			if (it.activity_id() == system_it->activity_id)
			{
				auto time_activity = std::make_shared<c_time_activity>();
				time_activity->load_data(it, system_it->end_time);
				player_ptr->m_time_activity_component->m_data_map[time_activity->get_activity_id()] = time_activity;
				break;
			}
		}
	}
	generate_new_activity(player_ptr);
	send_time_activity_info_all(player_ptr);
}
void time_activity_system::save_data_to_db(player* player_ptr, int32 save_type_ex)
{
	if (player_ptr->m_time_activity_component == nullptr)
	{
		CONSOLE_ERROR("time_activity_component is nullptr");
		return;
	}
	time_activity_s2s_sl_activity_info msg;
	for (auto& it : player_ptr->m_time_activity_component->m_data_map)
	{
		auto msg_data = msg.add_data_list();
		it.second->generate_db_data(msg_data);
	}
	player_ptr->send_message_to_dp(&msg, e_msgindex_dp2cs_save_char_data, e_data_flag_time_activity, save_type_ex);
}
void time_activity_system::send_time_activity_info_all(player* player_ptr)
{
	if (player_ptr->m_time_activity_component == nullptr)
	{
		CONSOLE_ERROR("time_activity_component is nullptr");
		return;
	}
	time_activity_s2c_activity_info_all msg;
	for (auto& it : player_ptr->m_time_activity_component->m_data_map)
	{
		if (it.second->is_end_del() == false)
		{
			auto msg_data = msg.add_data_list();
			it.second->generate_client_data(msg_data);
		}
	}
	player_ptr->send_message_to_self(&msg, e_msgindex_s2c_activity_info_all);
}
void time_activity_system::send_time_activity_info_one(player* player_ptr, int32 activity_id)
{
	if (player_ptr->m_time_activity_component == nullptr)
	{
		CONSOLE_ERROR("time_activity_component is nullptr");
		return;
	}
	time_activity_s2c_activity_info msg;
	auto it = player_ptr->m_time_activity_component->m_data_map.find(activity_id);
	if (it != player_ptr->m_time_activity_component->m_data_map.end())
	{
		it->second->generate_client_data(&msg);
		player_ptr->send_message(&msg, e_msgindex_s2c_activity_info_one);
	}
}
void time_activity_system::send_time_activity_info_del(player* player_ptr, int32 activity_id)
{
	time_activity_s2c_activity_info_del msg;
	msg.set_activity_id(activity_id);
	player_ptr->send_message(&msg, e_msgindex_s2c_activity_info_del);
}

std::shared_ptr<c_time_activity> time_activity_system::get_time_activity(player* player_ptr, int32 activity_id)
{
	if (player_ptr->m_time_activity_component == nullptr)
	{
		CONSOLE_ERROR("time_activity_component is nullptr");
		return nullptr;
	}
	auto it = player_ptr->m_time_activity_component->m_data_map.find(activity_id);
	if (it != player_ptr->m_time_activity_component->m_data_map.end())
	{
		return it->second;
	}
	return nullptr;
}
void time_activity_system::clear_old_activity(player* player_ptr)
{
	if (player_ptr->m_time_activity_component == nullptr)
	{
		CONSOLE_ERROR("time_activity_component is nullptr");
		return;
	}
	auto sta_cp = g_world_ent->get_component<system_time_activity_component>();
	for (auto it = player_ptr->m_time_activity_component->m_data_map.begin(); it != player_ptr->m_time_activity_component->m_data_map.end();)
	{
		auto temp_it = it++;
		bool is_find = false;
		int32 activity_id = temp_it->first;
		for (auto& find_it : sta_cp->m_run_activity)
		{
			if (activity_id == find_it->activity_id)
			{
				is_find = true;
				break;
			}
		}
		if (is_find == false)
		{
			player_ptr->m_time_activity_component->m_data_map.erase(temp_it);
			send_time_activity_info_del(player_ptr, activity_id);
		}
	}
}
void time_activity_system::generate_new_activity(player* player_ptr)
{
	auto time_activity_cp = player_ptr->m_time_activity_component;
	if (time_activity_cp == nullptr)
	{
		CONSOLE_ERROR("time_activity_component is nullptr");
		return;
	}
	auto sta_cp = g_world_ent->get_component<system_time_activity_component>();

	for (auto& it : sta_cp->m_run_activity)
	{
		auto find_it = time_activity_cp->m_data_map.find(it->activity_id);
		if (find_it != time_activity_cp->m_data_map.end())
		{
			continue;
		}
		auto template_ptr = GET_TEMPLATE(TimeActivityTemplate, it->activity_id);
		auto time_activity = std::make_shared<c_time_activity>();
		time_activity->set_activity_id(it->activity_id, it->end_time);
		time_activity_cp->m_data_map[it->activity_id] = time_activity;
		send_time_activity_info_one(player_ptr, it->activity_id);
	}
}
void time_activity_system::gm_clear_time_activity(player* player_ptr)
{
	if (player_ptr->m_time_activity_component == nullptr)
	{
		CONSOLE_ERROR("time_activity_component is nullptr");
		return;
	}
	for (auto& it : player_ptr->m_time_activity_component->m_data_map)
	{
		it.second->clear_data();
	}
	send_time_activity_info_all(player_ptr);
}
void time_activity_system::gm_add_time_activity_score(player* player_ptr, int32 activity_id, int32 add_score)
{
	if (player_ptr->m_time_activity_component == nullptr)
	{
		CONSOLE_ERROR("time_activity_component is nullptr");
		return;
	}
	auto time_activity = get_time_activity(player_ptr, activity_id);
	if (time_activity)
	{
		time_activity->add_data_info(e_wheel_plate_info_accumulate_num, add_score);
		time_activity_system::send_time_activity_info_one(player_ptr, activity_id);
	}
}