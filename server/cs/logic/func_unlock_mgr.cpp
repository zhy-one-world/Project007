
#include "func_unlock_mgr.h"
#include "template/template_manager.h"
#include "logic/unit_man.h"
#include "logic/skill_manager.h"
#include "cell_server.hpp"
#include "logic/skill_set.h"
#include "logic/item_set.h"
#include "logic/player.hpp"

namespace faith
{
	func_unlock_mgr::func_unlock_mgr()
	{
		m_unit_index = 0;
		clear_data();
	}

	void func_unlock_mgr::clear_data()
	{
		m_func_unlock_template_map.clear();
		m_can_trigger_active_funcs.clear();
		m_func_unluck_enable = true;
	}
	func_unlock_mgr::~func_unlock_mgr(void)
	{

	}

	void func_unlock_mgr::load_func_unlock_template_map()
	{
		m_func_unlock_template_map.clear();
		faith::template_manager::template_type* table = template_manager::get_instance().get_templates(e_FuncUnlockTemplate);
		if (nullptr == table)
		{
			return;
		}

		faith::template_manager::template_type::iterator ite;
		FuncUnlockTemplate* func_unlock_template_ptr = nullptr;
		for (ite = table->begin(); ite != table->end(); ++ite)
		{
			func_unlock_template_ptr = (FuncUnlockTemplate*)(ite->second);
			if (nullptr == func_unlock_template_ptr || func_unlock_template_ptr->FuncName.size() <= 0)
			{
				continue;
			}

			m_func_unlock_template_map.insert({ func_unlock_template_ptr->FuncName, func_unlock_template_ptr });

			// 为了减少遍历的量,把能触发激活操作的条目单独放在另一个数组里
			if ((func_unlock_template_ptr->Activate.size() > 0) && (func_unlock_template_ptr->Activate.size() % e_finish_activate_info_max == 0) && (is_func_unlock(func_unlock_template_ptr->FuncName) == false || func_unlock_template_ptr->attribute_id == wing_func_unlock_id))
			{
				m_can_trigger_active_funcs.push_back(func_unlock_template_ptr);
			}
		}
		func_unlock_trigger_activate();
	}

	FuncUnlockTemplate* func_unlock_mgr::get_func_unlock_template_by_func_name(const std::string& func_name)
	{
		func_unlock_template_map_it ite = m_func_unlock_template_map.find(func_name);
		if (m_func_unlock_template_map.end() == ite)
		{
			return nullptr;
		}
		return ite->second;
	}

	FuncUnlockTemplate*	func_unlock_mgr::get_func_unlock_template_by_func_name_of_all_func(const std::string& func_name)
	{
		template_manager::template_type* func_unlock_template_table = template_manager::get_instance().get_templates(e_FuncUnlockTemplate);
		if (nullptr == func_unlock_template_table)
		{
			return nullptr;
		}
		for (auto ite : *func_unlock_template_table)
		{
			FuncUnlockTemplate* func_unlock_template_ptr = (FuncUnlockTemplate*)(ite.second);
			if (nullptr == func_unlock_template_ptr || func_unlock_template_ptr->FuncName != func_name)
			{
				continue;
			}
			return func_unlock_template_ptr;
		}
		return nullptr;
	}

	void func_unlock_mgr::func_unlock_trigger_activate()
	{
		player& player_ref = unit_man::get_player(m_unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		FuncUnlockTemplate* func_unlock_template_ptr = nullptr;
		for (auto it = m_can_trigger_active_funcs.begin(); it != m_can_trigger_active_funcs.end();)
		{
			auto temp_it = it++;
			func_unlock_template_ptr = *(temp_it);
			if (func_unlock_template_ptr == nullptr)
			{
				continue;
			}
			if (is_func_unlock(func_unlock_template_ptr->FuncName) == true)
			{
				activate(func_unlock_template_ptr->Activate);
				m_can_trigger_active_funcs.erase(temp_it);
			}
		}
	}

	void func_unlock_mgr::activate(std::vector<int32>& activate_arr)
	{
		player& player_ref = unit_man::get_player(m_unit_index);

		int32 cur_class = player_ref.get_unit_info(e_role_info_class_type);
		if (cur_class >= e_finish_activate_info_max)
		{
			return;
		}
		int32 activate_num = activate_arr.size() / e_finish_activate_info_max;
		for (int32 i = 0; i < activate_num; ++i)
		{
			int32 temp_type = activate_arr[i * e_finish_activate_info_max + e_finish_activate_info_type];
			int32 activate_id = activate_arr[i * e_finish_activate_info_max + cur_class];
			switch (temp_type)
			{
			case e_finish_activate_type_skill:
			{
				skill_set& temp_skill_set = skill_manager::get_skill_set(m_unit_index);
				temp_skill_set.activate_skill(activate_id);
			}
			break;
			case e_finish_activate_type_wing:
			{
				citem* temp_item = player_ref.get_item_set().get_item_by_template_id(activate_id);
				if (temp_item != nullptr && temp_item->get_data_info(e_item_info_activate) <= 0)
				{
					player_ref.get_item_set().set_wing_att(false);
					temp_item->set_data_info(e_item_info_activate, 1);
					player_ref.get_item_set().send_item_one(temp_item);
					player_ref.get_item_set().show_this_wing(temp_item, -1, true);
					temp_item->set_unit_skill(m_unit_index, true);
					/*if ((temp_item->get_data_info(e_item_info_slot) + 1) == FEATHER_EQUIP_OPEN)
					{
						player_ref.get_item_set().equip_all_feather_and_spirit_and_soul();
						player_ref.get_item_set().reset_wings_order();
						player_ref.get_item_set().send_wing_item_all();
					
					}*/

					player_ref.get_item_set().set_wing_att(true);
					player_ref.get_phantom_mgr().on_item_oper(temp_item, temp_item->get_data_info(e_item_info_info_id));
					if (temp_item->get_wing_template_ptr() != nullptr && temp_item->get_wing_template_ptr()->Type == e_item_wing_type_normal)
					{
						player_ref.get_achievement_mgr().item_changed(temp_item, e_item_operation_featherupgrade_grade);
					}
				}
			}
			break;
			case e_finish_activate_type_mount:
			{
				citem* temp_item = player_ref.get_item_set().get_item_by_template_id(activate_id);
				if (temp_item != nullptr && temp_item->get_data_info(e_item_info_activate) <= 0)
				{
					player_ref.get_item_set().set_mount_att(false);
					temp_item->set_data_info(e_item_info_activate, 1);
					player_ref.get_item_set().send_item_one(temp_item);
					player_ref.get_item_set().show_this_mount(temp_item, -1);
					temp_item->set_unit_skill(m_unit_index, true);
					player_ref.get_item_set().set_mount_att(true);
					player_ref.get_achievement_mgr().item_changed(temp_item, e_item_operation_mount_unlock);
					player_ref.get_phantom_mgr().on_item_oper(temp_item, temp_item->get_data_info(e_item_info_info_id));
					player_ref.refresh_service_goal(e_service_goal_type_mount_num);
				}
			}
			break;
			case e_finish_activate_type_item:
			{
				citem* temp_item = player_ref.get_item_set().get_item_by_template_id(activate_id);
				if (temp_item != nullptr && temp_item->get_data_info(e_item_info_activate) <= 0)
				{
					temp_item->set_data_info(e_item_info_activate, 1);
					player_ref.get_item_set().send_item_one(temp_item);
					player_ref.send_info_one(e_role_info_mount_showd_template_id);
					temp_item->set_unit_skill(m_unit_index, true);
					player_ref.get_phantom_mgr().on_item_oper(temp_item, temp_item->get_data_info(e_item_info_info_id));
				}
			}
			break;
			case e_finish_activate_type_sprite_jiban:
			{
				//player_ref.get_spirit_mgr().set_jiban_att_all();
			}
			break;
			case e_finish_activate_type_sprite_qiyuan:
			{
				//player_ref.get_spirit_mgr().set_qiyuan_att_all();
			}
			break;
			case e_finish_activate_type_wing_spirit://注灵
			{
				player_ref.get_item_set().set_wing_att(false);
				player_ref.get_item_set().equip_all_spirit();
				player_ref.get_item_set().reset_wings_order();
				player_ref.get_item_set().send_wing_item_all();
				player_ref.get_item_set().set_wing_att(true);
			}
			break;
			case e_finish_activate_type_wing_feather://翎羽
			{
				player_ref.get_item_set().set_wing_att(false);
				player_ref.get_item_set().reset_wings_order();
				player_ref.get_item_set().send_wing_item_all();
				player_ref.get_item_set().set_wing_att(true);
			}
			break;
			case e_finish_activate_type_wing_soul://注魂
			{
				player_ref.get_item_set().set_wing_att(false);
				player_ref.get_item_set().equip_all_soul();
				player_ref.get_item_set().reset_wings_order();
				player_ref.get_item_set().send_wing_item_all();
				player_ref.get_item_set().set_wing_att(true);
			}
			break;
			case e_finish_activate_type_meditation://冥想
			{
				player_ref.get_meditation_mgr().set_meditation_reward_time();
			}
			break;
			default:
				break;
			}
		}
	}

	bool func_unlock_mgr::is_func_unlock(const std::string& func_name, int32 recursion_num)
	{
		recursion_num++;
		if (recursion_num >= max_recursion_num)
		{
			return false;
		}

		if (m_func_unluck_enable == false)
		{
			return true;
		}

		FuncUnlockTemplate* func_unlock_template_ptr = get_func_unlock_template_by_func_name(func_name);
		if (nullptr == func_unlock_template_ptr)
		{//找不到就说明不存在这个 就是解锁了
			return true;
		}

		// 检查依赖的功能是否已经解锁
		if (func_unlock_template_ptr->PrecondFuncUnlock.length() > 0)
		{
			if (func_name == func_unlock_template_ptr->PrecondFuncUnlock)
			{
				return false;
			}
			if (is_func_unlock(func_unlock_template_ptr->PrecondFuncUnlock, recursion_num) == false)
			{
				return false;
			}
		}

		player& player_ref = unit_man::get_player(m_unit_index);
		if (false == player_ref.is_valid())
		{
			return false;
		}

		// 检查角色当前的等级是否可以开启相应的功能
		int32 cur_level = player_ref.get_unit_info(e_role_info_exp_level);
		if (cur_level < func_unlock_template_ptr->UnlockNeedLevel)
		{
			return false;
		}
		
		int32 open_server_days = globle_data::get_instance().get_server_on_days();
		if (open_server_days < func_unlock_template_ptr->NeedOpenServerDays)
		{
			return false;
		}

		// 检查开启相应的功能需要完成的任务是否已经完成
		int32 need_finish_mission_id = func_unlock_template_ptr->UnlockNeedMissionID;
		if (need_finish_mission_id > 0)
		{
			cmission_mgr& mission_mgr_ref = player_ref.get_mission_mgr();
			cmission* mission_ptr = mission_mgr_ref.find_mission_by_index(e_mission_slot_main);
			if (nullptr == mission_ptr)
			{
				return false;
			}
			MissionTemplate* mission_template_ptr = mission_ptr->get_mission_template_ptr();
			if (mission_template_ptr == nullptr)
			{
				return false;
			}
			int32 cur_mission_id = mission_template_ptr->attribute_id;
			if (cur_mission_id <= 0)
			{
				return false;
			}
			if (cur_mission_id < need_finish_mission_id)
			{
				return false;
			}
			else if (cur_mission_id == need_finish_mission_id && mission_ptr->get_mission_state() < e_mission_state_finished)
			{
				return false;
			}
		}

		//检测是否开启跨服
		int32 is_need_cross_server = func_unlock_template_ptr->IsNeedCrossServer;
		if (is_need_cross_server > 0)
		{
			if (cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross) == false)
			{
				return false;
			}
		}

		return true;
	}

	bool func_unlock_mgr::is_func_unlock(const int32 func_template_id)
	{
		FuncUnlockTemplate* func_unlock_template_ptr = GET_TEMPLATE(FuncUnlockTemplate, func_template_id);
		if (func_unlock_template_ptr == nullptr)
		{
			return true;
		}
		return is_func_unlock(func_unlock_template_ptr->FuncName);
	}

}