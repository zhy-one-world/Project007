/********************************************************************
	created:	2015Äê12ÔÂ28ÈÕ16:11:23
	file base:	msgproc_gm
	file ext:	cpp
	author:		zhy

	purpose:
*********************************************************************/

#include "server_log.hpp"
#include "msgproc_gm.hpp"
#include "logic/player.hpp"
#include "logic/world_cs.h"
#include "logic/buff_man.h"
#include "logic/unit_man.h"
#include "base/ecs_world.h"
#include "lua/script_mgr.h"
#include "system/scene/wave_map_system.h"
#include "logic/skill_manager.h"
#include "system/scene/base_map_system.h"
#include "system/scene/box_map_system.h"
#include "logic/skill_set.h"
#include "system/play/time_activity_system.h"
#include "system/item/item_system.h"
#include "gm_order_def.hpp"
#include "logic/npc.hpp"

namespace hld
{
	typedef boost::function<void(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)>	gm_function;

	std::unordered_map<std::string, gm_function> g_gm_function_map;

	void init_gm_function_map()
	{
		g_gm_function_map["~additem"] = gm_add_item;
		g_gm_function_map["~addmoney"] = gm_add_money;
		g_gm_function_map["~levelup"] = gm_level_up;
		g_gm_function_map["~leveldown"] = gm_level_down;
		g_gm_function_map["~setvippoint"] = gm_set_vip_point;
		g_gm_function_map["~modifyclass"] = gm_modify_class;
		g_gm_function_map["~addbuff"] = gm_add_buff;
		g_gm_function_map["~delbuff"] = gm_del_buff;
		g_gm_function_map["~createnpc"] = gm_create_npc;
		g_gm_function_map["~clearbag"] = gm_clear_bag;
		g_gm_function_map["~clearwave"] = gm_clear_wave;
		g_gm_function_map["~addofflinetime"] = gm_add_offline_time;
		g_gm_function_map["~addonlinetime"] = gm_add_online_time;
		g_gm_function_map["~killself"] = gm_kill_self;
		g_gm_function_map["~missiondone"] = gm_mission_done;
		g_gm_function_map["~dailymissiondone"] = gm_mission_done;
		g_gm_function_map["~taofamissiondone"] = gm_mission_done;
		g_gm_function_map["~activateall"] = gm_activate_all;
		g_gm_function_map["~killall"] = gm_kill_all;
		g_gm_function_map["~transfermap"] = gm_transfer_map;
		g_gm_function_map["~showatt"] = gm_show_att;
		g_gm_function_map["~shownpcpos"] = gm_show_npc_pos;
		g_gm_function_map["~showallnpc"] = gm_show_all_npc;
		g_gm_function_map["~addtreasurepic"] = gm_add_treasure_pic;
		g_gm_function_map["~reload"] = gm_reload_data;
		g_gm_function_map["~cleartimeactivity"] = gm_clear_time_activity;
		g_gm_function_map["~addtimeactivitysco"] = gm_add_time_activity_score;
		g_gm_function_map["~onlinetime"] = gm_change_online_time;
	}
	void do_gm_order_logic(player& player_ref, const std::string& gm_command, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		auto iter = g_gm_function_map.find(gm_command);
		if (iter != g_gm_function_map.end())
		{
			iter->second(player_ref, arg_int, arg_string);
		}
		else
		{
			CONSOLE_ERROR("gm_command:{} not found", gm_command);
		}
	}
	void gm_add_item(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 3)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 3, arg_int.size());
			return;
		}
		std::vector<s_item_template_info> item_array;
		item_array.push_back(s_item_template_info(arg_int[0], arg_int[1], arg_int[2]));
		item_system::put_item_into_bag(&player_ref, item_array);
	}
	void gm_add_money(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 2)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 2, arg_int.size());
			return;
		}
		int64 add_money_num = arg_int[1];
		if (add_money_num < 0)
		{
			add_money_num = 0;
		}
		player_ref.add_money_or_exp((e_money_type)arg_int[0], add_money_num, e_server_log_add_money_gm_order);
		player_ref.send_money_one((e_money_type)arg_int[0]);
	}

	void gm_level_up(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		player_ref.gm_set_level_and_grade(arg_int[0]);

	}
	void gm_level_down(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
	}
	void gm_set_vip_point(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
	}
	void gm_modify_class(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
	}
	void gm_add_buff(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		buff_ex_env_param penv;
		penv.buff_level = 1;
		buff_man::add_buff_inst(player_ref.get_array_index(), player_ref.get_array_index(), arg_int[0], &penv);
		
	}
	void gm_del_buff(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		buff_man::del_buff_by_template_id(player_ref.get_array_index(), player_ref.get_array_index(), arg_int[0]);
	}
	void gm_create_npc(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		auto pos = player_ref.get_new_map_pos();
		s_unit_identifier empty_owner;
		world_cs::spawn_npc(arg_int[0], pos, empty_owner, player_ref.get_map_ent(), &player_ref);
	}
	void gm_clear_bag(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size()  == 0)
		{
			item_system::clear_bag(&player_ref, e_bag_type_bag);
		}
		else
		{
			item_system::clear_bag(&player_ref, (e_bag_type)arg_int[0]);
		}
	}
	void gm_clear_wave(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		auto entity = player_ref.get_map_ent();
		if (entity)
		{
			wave_map_system::kill_cur_wave(entity);
		}
	}
	void gm_add_offline_time(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		player_ref.set_logic_data(e_role_logic_info_offline_award_time, arg_int[0]);
	}
	void gm_add_online_time(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		auto online_time = player_ref.get_logic_data(e_role_logic_info_online_award_time);
		online_time += arg_int[0];
		player_ref.set_logic_data(e_role_logic_info_online_award_time, online_time);
	}
	void gm_kill_self(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		buff_ex_env_param penv;
		penv.damage_num = int32_MAX_NUM;
		penv.buff_level = 1;
		buff_man::add_buff_inst(player_ref.get_array_index(), player_ref.get_array_index(), GM_DAMAGE_BUFF_ID, &penv);
	}
	void gm_mission_done(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		player_ref.get_mission_mgr().gm_mission(arg_int[0]);;
	}
	void gm_activate_all(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		auto& skill_set = skill_manager::get_skill_set(player_ref.get_array_index());
		skill_set.activate_all_skill();
		player_ref.get_func_unlock_mgr().set_func_unluck_enable(false);
	}
	void gm_kill_all(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		base_map_system::kill_all_monster(player_ref.get_map_ent(), player_ref.get_array_index());
	}
	void gm_transfer_map(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		player_ref.get_transfer_control().teleport_by_template(arg_int[0], 0, 0, empty_guid_64, 0);
	}
	void gm_show_att(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		player_ref.get_pawn_att().show_att(arg_int[0]);
	}
	void gm_show_npc_pos(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		auto& npc_ref = unit_man::get_npc(arg_int[0]);
		if (npc_ref.is_valid() == false)
		{
			CONSOLE_ERROR("npc not find npc_id:{}", 1, arg_int[0]);
		}
		auto pos = npc_ref.get_new_map_pos().unit_location;
		CONSOLE_INFO("npc_inde:{} npc_id:{} spawn_id:{} x:{}  y:{}  z:{} ", arg_int[0], npc_ref.get_npc_template_id(), npc_ref.get_spawn_point_id(), pos.x, pos.y, pos.z);
	}
	void gm_show_all_npc(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		base_map_system::gm_show_all_npc(player_ref.get_map_ent());
	}
	void gm_add_treasure_pic(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		player_ref.add_money_or_exp(e_money_type_treasure_pic, arg_int[0], e_server_log_add_money_gm_order);
		player_ref.send_money_one(e_money_type_treasure_pic);
	}
	void gm_reload_data(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	}
	void gm_clear_time_activity(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		time_activity_system::gm_clear_time_activity(&player_ref);
	}
	void gm_add_time_activity_score(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 2)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 2, arg_int.size());
			return;
		}
		time_activity_system::gm_add_time_activity_score(&player_ref, e_time_activity_score_type(arg_int[0]), arg_int[1]);
	}
	void gm_change_online_time(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string)
	{
		if (arg_int.size() < 1)
		{
			CONSOLE_ERROR("arg_int size error need:{}, msg:{}", 1, arg_int.size());
			return;
		}
		auto online_time = player_ref.get_logic_data(e_role_logic_info_online_award_time);
		online_time = arg_int[0];
		player_ref.set_logic_data(e_role_logic_info_online_award_time, online_time);
		player_ref.send_logic_one(e_role_logic_info_online_award_time);
	}
}