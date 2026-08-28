#include "components/play/time_activity_component.h"
#include "components/play/wheel_plate_component.h"
#include "internet/aoi.pb.h"
#include "internet/net.pb.h"
#include "internet/time_activity.pb.h"
#include "logic/item_set.h"
#include "logic/npc.hpp"
#include "logic/player.hpp"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "template/template_manager.h"
#include "time_activity_system.h"
#include "utility/random.h"
#include "wheel_plate_system.h"

using namespace hld;

void wheel_plate_system::start_up(player* player_ptr, npc* npc_ptr)
{
	ZoneScoped;
	if (nullptr == player_ptr)
	{
		return;
	}
	if (player_ptr->m_time_activity_component == nullptr)
	{
		return;
	}
	auto unit_type = npc_ptr->get_unit_type();
	auto sub_type = npc_ptr->get_unit_sub_type();
	if (unit_type != e_unit_type_award_mark)
	{
		return;
	}
	auto score_type = get_activity_score_type(sub_type);
	if (score_type == e_time_activity_score_type_max)
	{
		return;
	}
	for (auto& it : player_ptr->m_time_activity_component->m_data_map)
	{
		if (it.second->is_end_del() == false)
		{
			wheel_plate_create_score(npc_ptr, it.first, score_type);
		}
	}
}
void wheel_plate_system::shut_down(npc* npc_ptr)
{
	ZoneScoped;
	npc_ptr->m_wheel_plate_component = nullptr;
}
void wheel_plate_system::aoi_msg(npc* npc_ptr, aoi_proto_unit_aoi_all& msg)
{
	ZoneScoped;
	if (npc_ptr->m_wheel_plate_component == nullptr)
	{
		return;
	}
	for (auto& it : npc_ptr->m_wheel_plate_component->m_score_map)
	{
		msg.add_extra_award_list(it.second.m_item_id);
		msg.add_extra_award_list(it.second.m_item_num);
		msg.add_extra_award_list(it.second.m_lock);
	}
}
void wheel_plate_system::wheel_plate_begin(player* player_ptr, int32 activity_id)
{
	ZoneScoped;
	auto time_activity_cp = player_ptr->m_time_activity_component;
	if (time_activity_cp == nullptr)
	{
		CONSOLE_ERROR("time_activity_cp is nullptr");
		return;
	}
	auto activity_time_template = GET_TEMPLATE(TimeActivityTemplate, activity_id);
	if (nullptr == activity_time_template)
	{
		CONSOLE_ERROR("activity_time_template is nullptr activity_id:{}", activity_id);
		return;
	}
	if (activity_time_template->ActivityType != e_time_activity_type_wheel_plate)
	{
		CONSOLE_ERROR("activity_id:{} ActivityType:{}", activity_id, activity_time_template->ActivityType);
		return;
	}
	auto time_activity = time_activity_system::get_time_activity(player_ptr, activity_id);
	if (time_activity == nullptr)
	{
		CONSOLE_ERROR("time_activity is nullptr activity_id:{}", activity_id);
		return;
	}
	auto own_score = time_activity->get_data_info(e_wheel_plate_info_accumulate_num);
	auto open_index = time_activity->get_data_info(e_wheel_plate_info_open_index);
	auto award_index = time_activity->get_data_info(e_wheel_plate_info_award_index);

	if (open_index >= activity_time_template->ActivityLogic.size() / 2)
	{
		CONSOLE_ERROR("open_index:{} ActivityLogic.size:{}", open_index, activity_time_template->ActivityLogic.size());
		return;
	}
	auto plate_id = activity_time_template->ActivityLogic[open_index * 2];
	auto need_score = activity_time_template->ActivityLogic[open_index * 2 + 1];
	if (own_score < need_score)
	{
		CONSOLE_ERROR("own_score:{} need_score:{}", own_score, need_score);
		return;
	}
	auto wheel_plate_template = GET_TEMPLATE(WheelPlateTemplate, plate_id);
	if (nullptr == wheel_plate_template)
	{
		CONSOLE_ERROR("wheel_plate_template is nullptr activity_id:{} plate_id:{}", activity_id, plate_id);
		return;
	}
	auto random_num = random_gen::get_random(1, 1000000);
	auto sum_random = 0;
	auto win_index = 0;
	time_activity_s2c_wheel_plate_begin msg;
	for (int32 i = 0; i < wheel_plate_template->CurItem.size() / 3; i++)
	{
		auto item_id = wheel_plate_template->CurItem[i * 3];
		auto item_num = wheel_plate_template->CurItem[i * 3 + 1];
		auto item_random = wheel_plate_template->CurItem[i * 3 + 2];
		msg.add_award_list(item_id);
		msg.add_award_list(item_num);
		sum_random += item_random;
		if (random_num <= sum_random && win_index == 0)
		{
			win_index = i + 1;
		}
	}
	if (award_index > 0)
	{
		win_index = award_index;
	}
	else
	{
		time_activity->set_data_info(e_wheel_plate_info_award_index, win_index);
	}
	msg.set_win_index(win_index);
	player_ptr->send_message_to_self(&msg, e_msgindex_s2c_wheel_plate_begin);
}
void wheel_plate_system::wheel_plate_end(player* player_ptr, int32 activity_id)
{
	ZoneScoped;
	auto time_activity_cp = player_ptr->m_time_activity_component;
	if (time_activity_cp == nullptr)
	{
		CONSOLE_ERROR("time_activity_cp is nullptr");
		return;
	}
	auto activity_time_template = GET_TEMPLATE(TimeActivityTemplate, activity_id);
	if (nullptr == activity_time_template)
	{
		CONSOLE_ERROR("activity_time_template is nullptr activity_id:{}", activity_id);
		return;
	}
	if (activity_time_template->ActivityType != e_time_activity_type_wheel_plate)
	{
		CONSOLE_ERROR("activity_id:{} ActivityType:{}", activity_id, activity_time_template->ActivityType);
		return;
	}
	auto time_activity = time_activity_system::get_time_activity(player_ptr, activity_id);
	if (time_activity == nullptr)
	{
		CONSOLE_ERROR("time_activity is nullptr activity_id:{}", activity_id);
		return;
	}
	auto own_score = time_activity->get_data_info(e_wheel_plate_info_accumulate_num);
	auto open_index = time_activity->get_data_info(e_wheel_plate_info_open_index);
	auto award_index = time_activity->get_data_info(e_wheel_plate_info_award_index) - 1;

	if (award_index < 0)
	{
		CONSOLE_ERROR("award_index:{} activity_id:{}", award_index, activity_id);
		return;
	}
	if (open_index >= activity_time_template->ActivityLogic.size() / 2)
	{
		CONSOLE_ERROR("open_index:{} ActivityLogic.size:{}", open_index, activity_time_template->ActivityLogic.size());
		return;
	}
	auto plate_id = activity_time_template->ActivityLogic[open_index * 2];
	auto need_score = activity_time_template->ActivityLogic[open_index * 2 + 1];
	if (own_score < need_score)
	{
		CONSOLE_ERROR("own_score:{} need_score:{}", own_score, need_score);
		return;
	}
	auto wheel_plate_template = GET_TEMPLATE(WheelPlateTemplate, plate_id);
	if (nullptr == wheel_plate_template)
	{
		CONSOLE_ERROR("wheel_plate_template is nullptr activity_id:{} plate_id:{}", activity_id, plate_id);
		return;
	}
	if (award_index * 3 > wheel_plate_template->CurItem.size())
	{
		CONSOLE_ERROR("award_index:{} CurItem.size:{}", award_index, wheel_plate_template->CurItem.size());
		return;
	}
	auto item_id = wheel_plate_template->CurItem[award_index * 3];
	auto item_num = wheel_plate_template->CurItem[award_index * 3 + 1];
	std::vector<s_item_template_info> item_list;
	item_list.push_back({ item_id, item_num, 1 });
	item_system::put_item_into_bag(player_ptr, item_list);

	time_activity->set_data_info(e_wheel_plate_info_open_index, open_index + 1);
	time_activity->set_data_info(e_wheel_plate_info_award_index, 0);
	time_activity_system::send_time_activity_info_one(player_ptr, activity_id);

	time_activity_s2c_wheel_plate_end msg;
	msg.set_item_id(item_id);
	msg.set_item_count(item_num);
	player_ptr->send_message_to_self(&msg, e_msgindex_s2c_wheel_plate_end);

	if (time_activity->is_end_del())
	{
		time_activity_system::send_time_activity_info_del(player_ptr, activity_id);
	}
}
void wheel_plate_system::wheel_plate_add_score(player* player_ptr, npc* npc_ptr)
{
	ZoneScoped;
	auto time_activity_cp = player_ptr->m_time_activity_component;
	if (time_activity_cp == nullptr)
	{
		return;
	}
	auto wheel_plate_cp = npc_ptr->m_wheel_plate_component;
	if (wheel_plate_cp == nullptr)
	{
		return;
	}
	for (auto& it : wheel_plate_cp->m_score_map)
	{
		auto time_activity = time_activity_system::get_time_activity(player_ptr, it.first);
		if (time_activity == nullptr)
		{
			CONSOLE_ERROR("time_activity is nullptr activity_id:{}", it.first);
			continue;
		}
		time_activity->add_data_info(e_wheel_plate_info_accumulate_num, it.second.m_item_num);
		time_activity_system::send_time_activity_info_one(player_ptr, it.first);
	}
}
void wheel_plate_system::wheel_plate_create_score(npc* npc_ptr, int32 activity_id, e_time_activity_score_type score_type)
{
	ZoneScoped;

	auto activity_time_template = GET_TEMPLATE(TimeActivityTemplate, activity_id);
	if (nullptr == activity_time_template)
	{
		CONSOLE_ERROR("activity_time_template is nullptr activity_id:{}", activity_id);
		return;
	}
	if (activity_time_template->ActivityType != e_time_activity_type_wheel_plate)
	{
		CONSOLE_ERROR("activity_id:{} ActivityType:{}", activity_id, activity_time_template->ActivityType);
		return;
	}
	int32 sum_score = 0;
	for (auto& it : activity_time_template->PlayScoreType)
	{
		auto play_add_activity_template = GET_TEMPLATE(PlayAddActivityTemplate, it);
		if (nullptr == play_add_activity_template)
		{
			CONSOLE_ERROR("play_add_activity_template is nullptr activity_id:{} play_add_activity_id:{}", activity_id, it);
			continue;
		}
		if (play_add_activity_template->PlayType != score_type)
		{
			continue;
		}
		auto add_score = play_add_activity_template->AddScore;
		auto random_num = random_gen::get_random(1, 1000000);
		if (random_num > play_add_activity_template->AddProbability)
		{
			continue;
		}
		sum_score += add_score;
	}
	if (sum_score > 0)
	{
		if (npc_ptr->m_wheel_plate_component == nullptr)
		{
			npc_ptr->m_wheel_plate_component = std::make_shared<wheel_plate_component>();
		}
		npc_ptr->m_wheel_plate_component->m_score_map[activity_id] = { activity_time_template->ScoreItem , sum_score, 1 };
	}
}
e_time_activity_score_type wheel_plate_system::get_activity_score_type(int32 sub_type)
{
	switch (sub_type)
	{
	case e_award_mark_type_nomal:
	{
		return e_time_activity_score_type_nomal_search;
	}
	break;
	case e_award_mark_type_refrush_monster:
	{
		return e_time_activity_score_type_monster_search;
	}
	break;
	case e_award_mark_type_bank_robbery:
	{
		return e_time_activity_score_type_flip_box_search;
	}
	break;
	case e_award_mark_type_finger_guess:
	{
		return e_time_activity_score_type_finger_guess_search;
	}
	break;
	case e_award_mark_type_special:
	{
		return e_time_activity_score_type_special_search;
	}
	break;
	default:
	{
		CONSOLE_ERROR("sub_type:{}", sub_type);
		return e_time_activity_score_type_max;
	}
		break;
	}
}