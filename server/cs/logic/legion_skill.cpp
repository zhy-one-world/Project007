/********************************************************************
file base:	passive_skill
file ext:	cpp
author:		gy
purpose:
*********************************************************************/

#include "legion_def.hpp"
#include "legion_skill.h"
#include "logic/buff_man.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "internal/skill_msg.hpp"
#include "template/template_manager.h"
#include "internet/cs2dp.pb.h"
#include "internet/legion.pb.h"
#include "internet/net.pb.h"
#include "internal/core.hpp"
#include "utility/parse_msg.h"
#include "buff_def.hpp"

namespace hld
{
	legion_skill::legion_skill()
	{
		m_array_index = 0;
		clear_data();
	}

	legion_skill::~legion_skill()
	{

	}

	void legion_skill::save_legion_skill(int32 save_type_ex)
	{
		player& player_ref = unit_man::get_player(m_array_index);

		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_legion_skill);
		if (is_use == false)
		{
			cs2dp_save_char_legion_skill request;
			request.role_guid = player_ref.get_unit_guid();
			request.unit_array_index = m_array_index;
			request.save_type_ex = save_type_ex;
			legion_skill_map_it it;
			if (m_legion_skill_map.size() > 0)
			{
				int32 index = 0;
				for (it = m_legion_skill_map.begin(); it != m_legion_skill_map.end(); it++)
				{
					if (index >= max_legion_skill_num)
					{
						break;
					}
					s_legion_skill_info temp_info = it->second;

					for (int32 i = 0; i < e_legion_skill_info_max; ++i)
					{
						request.data_list[index].data_ary[i] = temp_info.data_ary[i];
					}

					index++;
				}
				request.data_num = index;
			}
			player_ref.send_message_to_dp(&request, sizeof(request));
		}
		else
		{
			hld::cs2dp_proto::save_role_legion_skill msg;

			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(save_type_ex);

			hld::cs2dp_proto::role_legion_skill_db * db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			int32 row_count = 0;
			for (legion_skill_map_it it = m_legion_skill_map.begin(); it != m_legion_skill_map.end(); it++)
			{
				if (row_count >= max_legion_skill_num)
				{
					break;
				}
				const s_legion_skill_info& temp_info = it->second;
				hld::cs2dp_proto::role_legion_skill_row * db_row = db_data->add_row_data();
				if (db_row == nullptr)
				{
					return;
				}
				for (int32 i = 0; i < e_legion_skill_info_max; ++i)
				{
					db_row->add_data_ary(temp_info.data_ary[i]);
				}
				row_count++;
			}
			db_data->set_row_count(row_count);
			if (row_count == 0)
			{
				db_data->add_row_data();
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_legion_skill);
		}
	}
	void legion_skill::clear_data()
	{
		m_legion_skill_map.clear();
	}
	void legion_skill::load_legion_skill(const s_legion_skill_info* legion_skill_data, int32 data_num)
	{
		if (legion_skill_data == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < data_num; ++i)
		{
			update_one_legion_skill_info(legion_skill_data[i]);
		}
		send_all_legion_skill_level_to_client();
	}

	bool legion_skill::load_legion_skill_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_legion_skill_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_legion_skill_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_legion_skill_info *p_row = (s_legion_skill_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			hld::cs2dp_proto::role_legion_skill_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_legion_skill((const s_legion_skill_info *)p_data, msg.row_count());
		return true;

	}
	void legion_skill::update_one_legion_skill_info(s_legion_skill_info temp_info)
	{
		
		int32 skill_template_id = temp_info.data_ary[e_legion_skill_info_legion_skill_id];
		LegionSkillTemplate* legion_skill_ptr = GET_TEMPLATE(LegionSkillTemplate, skill_template_id);
		if (nullptr == legion_skill_ptr)
		{
			return;
		}

		legion_skill_map_it it = m_legion_skill_map.find(skill_template_id);
		if (it == m_legion_skill_map.end())
		{
			m_legion_skill_map.insert({ skill_template_id ,temp_info});
		}
		else
		{
			it->second = temp_info;
		}
	}

	void legion_skill::send_all_legion_skill_level_to_client()
	{
		std::vector<int32> skill_level_arr = get_all_legion_skill_level();
		if (skill_level_arr.size() != (finally_legion_skill_id - first_legion_skill_id + 1))
		{
			return;
		}
		legion_proto_get_all_legion_skill_level_end level_msg;
		for (int32 i = 0; i < skill_level_arr.size(); i++)
		{
			level_msg.add_legion_skill_level(skill_level_arr[i]);
		}
		player& self_ref = unit_man::get_player(m_array_index);
		if (self_ref.is_valid() == false)
		{
			return;
		}
		self_ref.send_message_to_self(&level_msg, e_msgindex_s2c_get_legion_skill_level_end);
	}

	int32 legion_skill::get_skill_level_by_id(int32 legion_skill_id)
	{	
		int32 cur_skill_level = 0;
		legion_skill_map_it it = m_legion_skill_map.find(legion_skill_id);
		if (it != m_legion_skill_map.end())
		{
			cur_skill_level = it->second.data_ary[e_legion_skill_info_curr_level];
		}
		return cur_skill_level;
	}

	int32 legion_skill::legion_skill_level_up(int32 legion_skill_id)
	{
		int32 update_level = 0;
		legion_skill_map_it it = m_legion_skill_map.find(legion_skill_id);
		if (it == m_legion_skill_map.end())
		{
			s_legion_skill_info legion_skill_info;
			legion_skill_info.data_ary[e_legion_skill_info_legion_skill_id] = legion_skill_id;
			legion_skill_info.data_ary[e_legion_skill_info_curr_level] = ++update_level;
			m_legion_skill_map.insert({ legion_skill_id,legion_skill_info });
		}
		else
		{
			update_level = ++it->second.data_ary[e_legion_skill_info_curr_level];
		}
		return update_level;
	}

	void legion_skill::level_up_legion_skill_func(const void* data_ptr)
	{
		const ws2cs_level_up_legion_skill* packet = static_cast<const ws2cs_level_up_legion_skill*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (!player_ref.is_valid())
		{
			return;
		}


		int32 cur_player_level = player_ref.get_unit_info(e_role_info_exp_level);
		LegionSkillTemplate* legion_skill_ptr = GET_TEMPLATE(LegionSkillTemplate, packet->skill_template_id);
		if (nullptr == legion_skill_ptr)
		{
			return;
		}
		int32 cur_skill_level = get_skill_level_by_id(packet->skill_template_id);

		if (cur_skill_level >= cur_player_level)
		{
			return;
		}

		int32 unlock_contribution_size = legion_skill_ptr->UnlockContribution.size();

		for (int32 i = 0; i < unlock_contribution_size - 1; i += 2)
		{
			if (legion_skill_ptr->UnlockContribution[i] > cur_skill_level)
			{
				break;
			}
			if (cur_skill_level == legion_skill_ptr->UnlockContribution[i])
			{
				if (packet->donate_contribution < legion_skill_ptr->UnlockContribution[i + 1])
				{
					return;
				}
				break;
			}
		}
		if (legion_skill_ptr->UpLevelCost2.size() < 3)
		{
			return;
		}
		int32 level_up_need_money2 = legion_skill_ptr->UpLevelCost2[1] + legion_skill_ptr->UpLevelCost2[2] * cur_skill_level;
		if (!player_ref.can_cut_money((e_money_type)legion_skill_ptr->UpLevelCost2[0], level_up_need_money2))
		{
			return;
		}
		player_ref.cut_money((e_money_type)legion_skill_ptr->UpLevelCost2[0], level_up_need_money2, e_server_log_cut_money_legion_skill);
		int32 update_level = legion_skill_level_up(packet->skill_template_id);
		if (update_level > cur_skill_level)
		{
			legion_buff_change(packet->skill_template_id, update_level);
		}
		send_all_legion_skill_level_to_client();

		legion_proto_level_up_legion_skill_end level_up_legion_skill_end_msg;
		level_up_legion_skill_end_msg.set_skill_id(packet->skill_template_id);
		player_ref.send_message_to_self(&level_up_legion_skill_end_msg, e_msgindex_s2c_level_up_legion_skill_end);

	}

	void legion_skill::lua_level_up_legion_skill_func(const char * data_ptr, int32 data_len)
	{
		hld::ws2cs_proto::level_up_legion_skill msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_level_up_legion_skill packet;

		packet.from_proto(msg);

		level_up_legion_skill_func(&packet);

	}

	void legion_skill::init_legion_skill_buff()
	{
		if (m_legion_skill_map.empty())
		{
			return;
		}
		else
		{
			for (legion_skill_map_it it = m_legion_skill_map.begin(); it != m_legion_skill_map.end(); it++)
			{
				legion_buff_change(it->second.data_ary[e_legion_skill_info_legion_skill_id], it->second.data_ary[e_legion_skill_info_curr_level]);
			}
		}
	}

	void legion_skill::legion_buff_change(int32 legion_skill_id, int32 legion_skill_level)
	{
		LegionSkillTemplate* legion_skill_ptr = GET_TEMPLATE(LegionSkillTemplate, legion_skill_id);
		if (nullptr == legion_skill_ptr)
		{
			return;
		}
		BuffDataTemplate* buff_data_ptr = GET_TEMPLATE(BuffDataTemplate, legion_skill_ptr->Buff);
		if (buff_data_ptr == nullptr)
		{
			return;
		}
		int32 role_array_index = m_array_index;
		buff_ex_env_param penv;
		penv.buff_level = legion_skill_level;
		buff_man::add_buff_inst(m_array_index, m_array_index, buff_data_ptr->attribute_id, &penv);
	}




	std::vector<int32> legion_skill::get_all_legion_skill_level()
	{
		std::vector<int32> skill_level;
		skill_level.clear();
		player& self_ref = unit_man::get_player(m_array_index);
		if (self_ref.is_valid() == false)
		{
			return skill_level;
		}
		if (!self_ref.get_legion_guid().is_valid())
		{
			return skill_level;
		}
		for (int32 temp_id = first_legion_skill_id; temp_id <= finally_legion_skill_id; temp_id++)
		{
			legion_skill_map_it it = m_legion_skill_map.find(temp_id);
			if (it !=m_legion_skill_map.end())
			{
				int curr_level = it->second.data_ary[e_legion_skill_info_curr_level];
				skill_level.push_back(curr_level);
			}
			else
			{
				skill_level.push_back(0);
			}
		
		}
		return skill_level;
	}

}
	
