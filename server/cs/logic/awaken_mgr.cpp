#include "awaken_mgr.h"
#include "internet/item.pb.h"
#include "internet/net.pb.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "lua/script_mgr.h"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "template/AwakenTemplate_S.h"
#include "template/template_manager.h"


namespace faith
{
	awaken_mgr::awaken_mgr()
	{
		m_array_index = 0;
		clear_data();
	}
	awaken_mgr::~awaken_mgr()
	{

	}
	void awaken_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;

	}
	void awaken_mgr::clear_data()
	{

	}
	void awaken_mgr::item_awaken_operate_lua(guid_64 main_guid, const std::vector<guid_64>& need_item_guid)
	{
		xstring need_item_guid_str = item_guid_array_to_string(need_item_guid);
		xstring main_guid_str = main_guid.to_string();

		script_mgr::get_instance().call_func(nullptr, "awaken_mgr_item_operate", 0, false, "%d%s%s", m_array_index, main_guid_str.c_str(), need_item_guid_str.c_str());

	}
	bool awaken_mgr::is_use_lua()
	{
		//script_mgr::get_instance().call_func("reload", 0);
		bool bRet = false;
		script_mgr::get_instance().call_func(nullptr, "awaken_mgr_is_use_lua", 1, false, ">%b", &bRet);
		return bRet;
	}
	void awaken_mgr::operate_msg(guid_64 main_guid, int32 result)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		item_proto_item_awaken_end msg;
		msg.add_item_guid(main_guid.A);
		msg.add_item_guid(main_guid.B);
		msg.set_result(result);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_item_awaken_end);
	}
	void awaken_mgr::syc_client_update_item(citem* item_ptr)
	{
		if (item_ptr == nullptr)
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		temp_player.get_item_set().send_item_one(item_ptr);

	}

	bool awaken_mgr::is_exist_item_guid_array(const std::vector<guid_64>& need_item_guid)
	{
	     for (int32 i = 0; i < need_item_guid.size(); i++)
	     {
			 if (!is_item(need_item_guid[i]))
			 {
				 return false;
			 }
	     }
		 return true;
	}
	bool awaken_mgr::is_item(guid_64 item_guid)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return false;
		}
	    citem * item_ptr = temp_player.get_item_set().get_item(item_guid);
	    if (item_ptr == nullptr)
	    {
		   return false;
	    }
	    return true;
	}
	int32 awaken_mgr::get_item_power(guid_64 item_guid)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return 0;
		}
		citem * item_ptr = temp_player.get_item_set().get_item(item_guid);
		if (item_ptr == nullptr)
		{
			return 0;
		}
		ItemTemplate *config_ptr = item_ptr->get_item_info_ptr();
		if (config_ptr == nullptr)
		{
			return 0;
		}
		return config_ptr->AwakenPower;
	}
	int32 awaken_mgr::get_all_item_power(const std::vector<guid_64>& need_item_guid)
	{
		int32 total_power = 0;
		for (int32 i = 0; i < need_item_guid.size(); i++)
		{
			total_power += get_item_power(need_item_guid[i]);
		}
		return total_power;
	}

	citem * awaken_mgr::get_item(guid_64 item_guid)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return nullptr;
		}
		return temp_player.get_item_set().get_item(item_guid);
	}

	AwakenTemplate *awaken_mgr::get_awaken_template_by_item(citem *item_ptr)
	{
		if (item_ptr == nullptr)
		{
			return nullptr;
		}
		ItemTemplate *item_info_ptr = item_ptr->get_item_info_ptr();
		if (item_info_ptr == nullptr)
		{
			return nullptr;
		}
		int32 awaken_id = item_info_ptr->AwakenID;
		return GET_TEMPLATE(AwakenTemplate, awaken_id + item_ptr->get_data_info(e_item_info_wing_exp));
	}

	AwakenTemplate * awaken_mgr::get_next_awaken_template_by_item(citem * item_ptr)
	{
		if (item_ptr == nullptr)
		{
			return nullptr;
		}
		ItemTemplate *item_info_ptr = item_ptr->get_item_info_ptr();
		if (item_info_ptr == nullptr)
		{
			return nullptr;
		}
		int32 awaken_id = item_info_ptr->AwakenID;
		return GET_TEMPLATE(AwakenTemplate, awaken_id + item_ptr->get_data_info(e_item_info_wing_exp) + 1);
	}

	bool awaken_mgr::is_type(int32 type,int32 sub_type)
	{
		if (type == e_item_type_weapon)
		{
			return true;
		}
		if (type != e_item_type_armor)
		{
			return false;
		}
		if (sub_type < e_armor_sub_type_hat || sub_type > e_armor_sub_type_fashion) //  2 防具 10 为魔法书
		{
			return false;
		}
		return true;
	}
	int32 awaken_mgr::get_grade_num()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return 0;
		}
		return temp_player.get_grade_num();
	}

	bool awaken_mgr::is_material(int32 material_id, int32 material_num)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return false;
		}

		return item_system::can_cost_item(&temp_player, e_bag_type_bag, material_id, material_num);
	}

	void awaken_mgr::del_material(int32 material_id, int32 material_num)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		item_system::cost_item_from_bag(&temp_player, e_bag_type_bag, material_id, material_num);

	}

	void awaken_mgr::set_awaken_attr(citem *item_ptr, bool is_add)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		if (item_ptr == nullptr)
		{
			return;
		}
		ItemTemplate *item_info_ptr = item_ptr->get_item_info_ptr();
		if (item_info_ptr == nullptr)
		{
			return;
		}
		if (item_info_ptr->AwakenID == 0)
		{
			return;
		}
		AwakenTemplate *awaken_template_ptr = get_awaken_template_by_item(item_ptr);
		if (awaken_template_ptr == nullptr)
		{
			return;
		}
		
		temp_player.get_pawn_att().apply_att_change_by_array(awaken_template_ptr->AttArray, is_add);	
		if (item_ptr->get_data_info(e_item_info_wing_bless) <= 0)
		{
			return;
		}
		// 增加属性属性比例是当前觉醒等级
		float add_count = (float)item_ptr->get_data_info(e_item_info_wing_bless) / (float)awaken_template_ptr->NeedPower;
		AwakenTemplate *next_awaken_template_ptr = get_next_awaken_template_by_item(item_ptr);
		if (next_awaken_template_ptr == nullptr)
		{
			return;
		}
		
		std::vector<float> AttArray = next_awaken_template_ptr->AttArray;
		if (AttArray.size() / e_att_one_max <= 0)
		{
			return;
		}
		for (int32 i = 0; i < AttArray.size() / e_att_one_max; ++i)
		{
			for (int32 j = 0; j < awaken_template_ptr->AttArray.size() / e_att_one_max; ++j)
			{
				if (AttArray[i * e_att_one_max + e_att_one_att_id] == awaken_template_ptr->AttArray[j * e_att_one_max + e_att_one_att_id])
				{
					AttArray[i * e_att_one_max + e_att_one_value] -= awaken_template_ptr->AttArray[j * e_att_one_max + e_att_one_value];
				}
			}
		}
		temp_player.get_pawn_att().apply_att_change_by_array(AttArray, is_add, add_count);
	}

	bool awaken_mgr::is_can_inherit(citem* strip_item_ptr, citem* inherited_item_ptr)
	{
		if (strip_item_ptr == nullptr || inherited_item_ptr == nullptr)
		{
			return false;
		}
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return false;
		}
		ItemTemplate *strip_item_info_ptr = strip_item_ptr->get_item_info_ptr();
		ItemTemplate *inherited_item_info_ptr = inherited_item_ptr->get_item_info_ptr();
		if (strip_item_info_ptr == nullptr || inherited_item_info_ptr == nullptr)
		{
			return false;
		}
		if (strip_item_info_ptr->AwakenID == 0 || inherited_item_info_ptr->AwakenID == 0)
		{
			return false;
		}
		if (!is_type(strip_item_info_ptr->item_type, strip_item_info_ptr->sub_type) ||
			!is_type(inherited_item_info_ptr->item_type, inherited_item_info_ptr->sub_type))
		{
			return false;
		}

		int32 strip_level = strip_item_ptr->get_data_info(e_item_info_wing_exp);
		int32 inherited_level = inherited_item_ptr->get_data_info(e_item_info_wing_exp);
		if (strip_level <= inherited_level)
		{
			return false;
		}
		return true;
	}
	void awaken_mgr::set_inherit(citem* strip_item_ptr, citem* inherited_item_ptr)
	{
		if (!is_can_inherit(strip_item_ptr, inherited_item_ptr))
		{
			return;
		}

		int32 strip_level = strip_item_ptr->get_data_info(e_item_info_wing_exp);
		int32 inherited_level = inherited_item_ptr->get_data_info(e_item_info_wing_exp);
		if (strip_level <= inherited_level)
		{
			return;
		}
		strip_item_ptr->set_data_info(e_item_info_wing_exp, inherited_level);
		inherited_item_ptr->set_data_info(e_item_info_wing_exp, strip_level);
	}

	void  awaken_mgr::sync_fight_power(citem *item_ptr)
	{
		if (item_ptr == nullptr)
		{
			return;
		}

		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(item_ptr);
		temp_player.get_item_set().sync_whole_euip_fighting_power();	 
	}

	xstring awaken_mgr::item_guid_array_to_string(const std::vector<guid_64>& need_item_guid)
	{
		std::stringstream buff;
		xstring ret = "";
		int32 num = need_item_guid.size();
		for (int32 i = 0 ; i < num; i++)
		{
			buff << need_item_guid[i].to_string();
			if (i != num - 1)
			buff << "|";
		}
		buff >> ret;
		return ret;
	}
	citem *awaken_mgr::get_item_id_str(xstring item_guid_str)
	{
		guid_64 item_guid;
		item_guid.parse_guid(item_guid_str);
		return get_item(item_guid);
	}
	
	void awaken_mgr::item_str_to_item_guid_array_del(xstring item_array_str)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (false == temp_player.is_valid())
		{
			CONSOLE_ERROR("player is invalid m_array_index:{}", m_array_index);
			return;
		}
		xstring  temp;
		char *p_start = (char *)item_array_str.c_str();
		char *p_end = p_start;
		while (*p_end != '\0')
		{
			if (*p_end == '|')
			{
				xstring temp(p_start,  p_end - p_start);

				guid_64 temp_guid;
				temp_guid.parse_guid(temp);
				auto item_ent = get_entity(temp_guid);
				if (item_ent)
				{
					item_system::cost_item_by_ent(item_ent);
				}
				p_start = p_end + 1;
			}
			p_end++;
		}
		if (p_start != p_end)
		{
			xstring temp(p_start, p_end - p_start);

			guid_64 temp_guid;
			temp_guid.parse_guid(temp);
			auto item_ent = get_entity(temp_guid);
			if (item_ent)
			{
				item_system::cost_item_by_ent(item_ent);
			}
		}
		if (temp_player.is_valid())
		{
		}
	}
	void awaken_mgr::operate_msg_str(xstring main_guid_str, int32 result)
	{
		guid_64 main_guid;
		main_guid.parse_guid(main_guid_str);
		operate_msg(main_guid, result);
	}

	


}