#include "components/scene/base_map_component.h"
#include "components/scene/flip_box_component.h"
#include "flip_box_system.h"
#include "internet/map.pb.h"
#include "internet/net.pb.h"
#include "logic/map_record.h"
#include "logic/player.hpp"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "template/template_manager.h"
#include "utility/random.h"

using namespace faith;

void flip_box_system::start_up(Entity* map_ent, int32 flip_box_id, player* player_ptr)
{
	ZoneScoped;
	auto flip_box_template = GET_TEMPLATE(FlipBoxTemplate, flip_box_id);
	if (nullptr == flip_box_template)
	{
		CONSOLE_ERROR("flip_box_template is invalid flip_box_id:{}", flip_box_id);
		return;
	}
	if (flip_box_template->RandomItem.size() != g_flip_box_count * 2)
	{
		CONSOLE_ERROR("flip_box_template size error size:{}", flip_box_template->RandomItem.size());
		return;
	}

	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(base_map_cp->m_map_id);
	auto box_map_data = map_record_ptr->get_map_record_data<s_box_map_info>();
	if (nullptr == box_map_data)
	{
		CONSOLE_ERROR("box_map_data is invalid");
		return;
	}

	box_map_data->m_flip_box.clear();
	auto random_num = random_gen::get_random(1, 1000000);
	auto random_sum = 0;
	auto end_index = 0;
	for (int32 i = 0; i < g_flip_box_count; i++)
	{
		if (random_num <= random_sum + flip_box_template->RandomItem[i * 2 + 1])
		{
			end_index = i;
			break;
		}
		else
		{
			random_sum += flip_box_template->RandomItem[i * 2 + 1];
		}
	}
	for (int32 i = 0; i < g_flip_box_count; i++)
	{
		if (i == end_index)
		{
			continue;
		}
		int32 add_count = random_gen::get_random(0, g_flip_box_win_count - 1);
		for (size_t j = 0; j < add_count; j++)
		{
			int32 random_insert = random_gen::get_random(0, box_map_data->m_flip_box.m_award_list.size());
			box_map_data->m_flip_box.m_award_list.insert(box_map_data->m_flip_box.m_award_list.begin() + random_insert, i);
		}
	}
	for (size_t i = 0; i < g_flip_box_win_count - 1; i++)
	{
		int32 random_insert = random_gen::get_random(0, box_map_data->m_flip_box.m_award_list.size());
		box_map_data->m_flip_box.m_award_list.insert(box_map_data->m_flip_box.m_award_list.begin() + random_insert, end_index);
	}
	box_map_data->m_flip_box.m_award_list.push_back(end_index);
	box_map_data->m_flip_box.m_flip_box_id = flip_box_id;

	auto flip_box_cp = map_ent->add_component<flip_box_component>();
	flip_box_cp->m_flip_box_template = flip_box_template;

	send_box_info(map_ent, player_ptr);
	return;
}
void flip_box_system::shut_down(Entity* map_ent)
{
	map_ent->remove_component<flip_box_component>();
}
void flip_box_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
}
void flip_box_system::load_data_from_db(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(base_map_cp->m_map_id);
	auto box_map_data = map_record_ptr->get_map_record_data<s_box_map_info>();
	if (nullptr == box_map_data)
	{
		CONSOLE_ERROR("box_map_data is invalid");
		return;
	}
	if (box_map_data->m_flip_box.m_award_list.empty())
	{
		return;
	}
	auto flip_box_cp = map_ent->add_component<flip_box_component>();
	flip_box_cp->m_flip_box_template = GET_TEMPLATE(FlipBoxTemplate, box_map_data->m_flip_box.m_flip_box_id);;
}
void flip_box_system::send_box_info(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	auto flip_box_cp = map_ent->get_component<flip_box_component>();
	if (false == flip_box_cp.isValid())
	{
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(base_map_cp->m_map_id);
	auto box_map_data = map_record_ptr->get_map_record_data<s_box_map_info>();
	if (nullptr == box_map_data)
	{
		CONSOLE_ERROR("box_map_data is invalid");
		return;
	}
	map_s2c_flip_box_info msg;
	msg.set_flip_box_id(box_map_data->m_flip_box.m_flip_box_id);
	for (auto& award : box_map_data->m_flip_box.m_award_list)
	{
		msg.add_award_list(award);
	}
	for (auto& award : box_map_data->m_flip_box.m_open_list)
	{
		msg.add_open_list(award);
	}
	player_ptr->send_message_to_self(&msg, e_msgindex_s2c_flip_box_info);
}
int32 flip_box_system::open_flip_box(Entity* map_ent, player* player_ptr, int32 open_index)
{
	ZoneScoped;
	auto flip_box_cp = map_ent->get_component<flip_box_component>();
	if (false == flip_box_cp.isValid())
	{
		return -1;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(base_map_cp->m_map_id);
	auto box_map_data = map_record_ptr->get_map_record_data<s_box_map_info>();
	if (nullptr == box_map_data)
	{
		CONSOLE_ERROR("box_map_data is invalid");
		return -1;
	}
	if (box_map_data->m_flip_box.m_open_list.find(open_index) != box_map_data->m_flip_box.m_open_list.end())
	{
		CONSOLE_ERROR("open_index is invalid open_index:{}", open_index);
		return -1;
	}
	box_map_data->m_flip_box.m_open_list.insert(open_index);
	if (box_map_data->m_flip_box.m_open_list.size() >= box_map_data->m_flip_box.m_award_list.size())
	{
		auto end_index = box_map_data->m_flip_box.m_award_list[box_map_data->m_flip_box.m_award_list.size() - 1];
		auto item_id = flip_box_cp->m_flip_box_template->RandomItem[end_index * 2];
		std::vector<s_item_template_info> item_list;
		item_list.push_back({item_id, 1, 1});
		item_system::put_item_into_bag(player_ptr, item_list);
		shut_down(map_ent);
	}
	return open_index;
}