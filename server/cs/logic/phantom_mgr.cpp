#include "phantom_mgr.h"
#include "logic/unit_man.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "template/template_manager.h"
#include "internal/phantom_msg.hpp"
#include "internet/cs2dp.pb.h"
#include "internet/phantom.pb.h"
#include "internet/net.pb.h"
#include "utility/parse_msg.h"

namespace faith
{
	cphantom_config cphantom_mgr::m_phantom_config;

	 cphantom_mgr::cphantom_mgr()
	{
		 m_array_index = 0;
		 clear_data();
	}
	cphantom_mgr::~cphantom_mgr()
	{

	}
	void cphantom_mgr::clear_data()
	{
		m_data_map.clear();
	}
	void cphantom_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}
	void cphantom_mgr::save_phantom_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_phantom);
		if (!is_use)
		{
			cs2dp_save_char_phantom req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_array_index;

			int32 data_count = 0;
			for (phantom_map_it it = m_data_map.begin(); it != m_data_map.end(); it++)
			{
				if (data_count >= MAX_PHANTOM_NUM)
				{
					break;
				}
				cphantom &temp_phantom = it->second;
				if (temp_phantom.get_inst_data(e_phm_config_id) <= 0)
				{
					continue;
				}
				req.data_list[data_count] = temp_phantom.get_phantom_info_all();
				data_count++;
			}
			req.data_num = data_count;
			player_ref.send_message_to_dp(&req, req.get_pak_length());
			return;
		}

		faith::cs2dp_proto::save_role_phantom msg;
		msg.set_role_guid(player_ref.get_unit_guid().server_64);
		msg.set_unit_array_index(m_array_index);
		msg.set_save_type_ex(save_type);

		faith::cs2dp_proto::role_phantom_db* db_data = msg.mutable_db_data();
		if (db_data == nullptr)
		{
			return;
		}

		int32 data_count = 0;
		for (phantom_map_it it = m_data_map.begin(); it != m_data_map.end(); it++)
		{
			if (data_count >= MAX_PHANTOM_NUM)
			{
				break;
			}
			cphantom &temp_phantom = it->second;
			if (temp_phantom.get_inst_data(e_phm_config_id) <= 0)
			{
				continue;
			}

			faith::cs2dp_proto::role_phantom_row *db_row = db_data->add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			for (int32 i = 0; i < e_phm_max ; i ++)
			{
				db_row->add_data_ary(temp_phantom.get_inst_data(i));

			}
			data_count++;
		}
		db_data->set_row_count(data_count);
		if (data_count == 0)
		{
			db_data->add_row_data();
		}

		player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_phantom);





	}
	void cphantom_mgr::load_phantom_by_db(const s_phantom_info * phantom_data, int32 load_num)
	{
		 for (int32 i = 0; i < load_num; ++i)
		 {
			 const s_phantom_info& temp_info = phantom_data[i];
			 if (temp_info.data_ary[e_phm_config_id] <= 0)
			 {
				 continue;
			 }
			 create_phantom_by_info(temp_info);
		 }
		 sync_all_message_to_client();
	}

	bool cphantom_mgr::load_phantom_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_phantom_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_phantom_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_phantom_info *p_row = (s_phantom_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_phantom_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_phantom_by_db((const s_phantom_info *)p_data, msg.row_count());

		return true;

	}
	void cphantom_mgr::create_phantom_by_info(const s_phantom_info & phantom_info)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		if (m_data_map.size() >= MAX_PHANTOM_NUM)
		{
			return;
		}
	
		cphantom new_phantom;
		if (!new_phantom.init_phantom_by_info(phantom_info))
		{
			return;
		}
		
		int32 phantom_id = new_phantom.get_inst_data(e_phm_config_id);
		phantom_map_it it = m_data_map.find(phantom_id);
		if(it != m_data_map.end())
		{
			return;
		}
		m_data_map[phantom_id] = new_phantom;
	}
	void cphantom_mgr::sync_all_message_to_client()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		phantom_proto_phantom_all  msg_all;
		msg_all.add_role_guid(temp_player.get_unit_guid().A);
		msg_all.add_role_guid(temp_player.get_unit_guid().B);
		for (phantom_map_it it = m_data_map.begin(); it != m_data_map.end(); it++)
		{
			cphantom& temp_phantom = it->second;
			if (!temp_phantom.get_config_ptr())
			{
				continue;
			}
			phantom_proto_phantom_one* one_msg = msg_all.add_phantom_array();

			for (int32 i = e_phm_config_id; i < e_phm_max; i++)
			{
				one_msg->add_phantom_info(temp_phantom.get_inst_data(i));
			}
		}
		temp_player.send_message_to_self(&msg_all, e_msgindex_s2c_phantom_all);
	}

	void cphantom_mgr::sync_one_message_to_client(cphantom &temp_phantom)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		phantom_proto_phantom_one one_msg;
		for (int32 i = e_phm_config_id; i < e_phm_max;  i++)
		{
			one_msg.add_phantom_info(temp_phantom.get_inst_data(i));
		}
		temp_player.send_message_to_self(&one_msg, e_msgindex_s2c_phantom_one);
	}
	int32 cphantom_mgr::get_player_class()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return 0;
		}
		return temp_player.get_unit_info(e_role_info_class_type);
	}

	void cphantom_mgr::oper_active(int32 phantom_id)
	{
		phantom_map_it it = m_data_map.find(phantom_id);
		if (it == m_data_map.end())
		{
			oper_msg(phantom_id, phantom_error_id_not_exist);
			return;
		}

		int32 class_type = get_player_class();

		cphantom& temp_phantom = it->second;

		if (!temp_phantom.is_finish(class_type))
		{
			oper_msg(phantom_id, phantom_error_not_finish);
			return;
		}
		if (temp_phantom.get_inst_data(e_phm_state) == 1)
		{
			oper_msg(phantom_id, phantom_error_is_active);
			return;
		}

		temp_phantom.set_inst_data(e_phm_state, 1);

		int32 skillid = temp_phantom.get_skill_id();
		active_skill(skillid);

		sync_one_message_to_client(temp_phantom);

		oper_msg(phantom_id, phantom_error_sucess);

	}
	void cphantom_mgr::active_skill(int32 skillid)
	{
		if (skillid == 0)
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		temp_player.get_passive_skill().passive_skill_special(skillid, true);

	}
	void cphantom_mgr::oper_msg(int32 phantom_id, int32 result)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		phantom_proto_phantom_operate_end msg;
		msg.set_phantom_id(phantom_id);
		msg.set_oper_type(0);
		msg.set_result(result);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_phantom_operate_end);
	}


	void cphantom_mgr::on_item_oper(citem* changed_item_ptr, int32 item_template_id)
	{
		if (!is_item_con(changed_item_ptr))
		{
			return;
		}
		std::vector<s_id_pos>& ret_list = m_phantom_config.get_id_pos_list(item_template_id);
		if (ret_list.size() == 0)
		{
			return;
		}
		for (int32 i = 0; i < ret_list.size(); i++)
		{
			const s_id_pos& temp_node = ret_list[i];
			phantom_event(temp_node.id, temp_node.pos);
		}

	}
	bool cphantom_mgr::phantom_event(int32 phantom_id, int32 pos)
	{
		phantom_map_it it = m_data_map.find(phantom_id);
		if (it == m_data_map.end())
		{
			if (m_data_map.size() >= MAX_PHANTOM_NUM)
			{
				return false;
			}
			cphantom new_phantom;
			if (!new_phantom.init_phantom_by_template(phantom_id))
			{
				return false;
			}
			m_data_map[phantom_id] = new_phantom;
		}
		it = m_data_map.find(phantom_id);
		cphantom& temp_phantom = it->second;
		if (temp_phantom.is_item_finish(pos))
		{
			return false;
		}

		temp_phantom.set_item_value(pos, 1);
		sync_one_message_to_client(temp_phantom);
		return true;

	}
	void cphantom_mgr::on_item_load_end()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		config_map & temp_config_map = m_phantom_config.get_all_config();

		for (config_map_it it = temp_config_map.begin(); it != temp_config_map.end(); it++)
		{
			int32 item_id = it->first;

			if (is_item_finish(it->second))
			{
				continue;
			}
			citem* item_ptr = temp_player.get_item_set().get_item_by_template_id(item_id);
			if (item_ptr == nullptr || !is_item_con(item_ptr))
			{
				continue;
			}
			on_item_oper(item_ptr, item_id);
		}
		
	}
	bool cphantom_mgr::is_item_finish(std::vector<s_id_pos> & in_vec)
	{
		if (in_vec.size() == 0)
		{
			return true;
		}
		for (int32 i = 0; i < in_vec.size(); i++)
		{
			s_id_pos& temp_node = in_vec[i];
			if (!is_phantom_finish(temp_node.id, temp_node.pos))
			{
				return false;
			}
		}
		return true;

	}
	bool cphantom_mgr::is_phantom_finish(int32 phantom_id, int32 pos)
	{
		phantom_map_it it = m_data_map.find(phantom_id);
		if (it == m_data_map.end())
		{
			return false;
		}
		cphantom & temp_phantom = it->second;
		return temp_phantom.is_item_finish(pos);
	}
	bool cphantom_mgr::is_item_con(citem *item_ptr)
	{
		ItemTemplate *item_template_ptr = item_ptr->get_item_info_ptr();
		if (item_template_ptr == nullptr)
		{
			return false;
		}
		if (item_template_ptr->item_type == e_item_type_spirit)
		{
			return true;
		}
		int32 active = item_ptr->get_data_info(e_item_info_activate);
		if (active == 0)
		{
			return false;
		}
		return true;
	}
	void cphantom_mgr::init_all_skill()
	{
		for (phantom_map_it it = m_data_map.begin(); it != m_data_map.end(); it++)
		{
			cphantom & temp_phantom = it->second;
			if (!temp_phantom.is_acitve())
			{
				continue;
			}
			active_skill(temp_phantom.get_skill_id());
		}
	}
	cphantom& cphantom_mgr::lua_find_object(int32 phantom_id)
	{
		phantom_map_it it = m_data_map.find(phantom_id);
		if (it == m_data_map.end())
		{	
			return m_empty_obj;
		}
		return it->second;
	}

	////////////////////////////////////////////////////////////////////////////
	cphantom_config::cphantom_config()
	{
		m_config_map.clear();
		m_empty_list.clear();
	}

	std::vector<s_id_pos>& cphantom_config::get_id_pos_list(int32 item_template_id)
	{
		if (!m_is_read)
		{
			build();
			m_is_read = true;
		}
		config_map_it it = m_config_map.find(item_template_id);
		if (it == m_config_map.end())
		{
			return m_empty_list;
		}
		return it->second;

	}

	config_map & cphantom_config::get_all_config()
	{
		if (!m_is_read)
		{
			build();
			m_is_read = true;
		}
		return m_config_map;
	}

	void cphantom_config::build()
	{
		template_manager::template_type* tempalte_table_ptr = template_manager::get_instance().get_templates(e_IllusionTemplate);
		if (tempalte_table_ptr == nullptr)
		{
			return;
		}
		template_manager::template_type::iterator it = tempalte_table_ptr->begin();
		for (; it != tempalte_table_ptr->end(); it++)
		{
			int32 id = it->first;
			IllusionTemplate* config_ptr = (IllusionTemplate*)it->second;

			bool can_show = true;
			std::vector<int32> region_arr = config_ptr->RegionArr;
			if (region_arr.size() > 0)
			{
				int32 cur_region = e_version_region_type_china_main;
				RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
				if (region_template_ptr != nullptr)
				{
					cur_region = region_template_ptr->RegionCode;
				}
				for (int32 i = 0; i < region_arr.size(); i++)
				{
					if (cur_region == region_arr[i])
					{
						can_show = false;
						break;
					}
				}
			}
			if (!can_show)
			{
				continue;
			}

			for (int32 i = 0; i < config_ptr->WarriorItemSet.size(); i++)
			{
				add(config_ptr->WarriorItemSet[i], id, i);
			}
			for (int32 i = 0; i < config_ptr->MageItemSet.size(); i++)
			{
				add(config_ptr->MageItemSet[i], id, i);
			}
			for (int32 i = 0; i < config_ptr->KnightItemSet.size(); i++)
			{
				add(config_ptr->KnightItemSet[i], id, i);
			}
			for (int32 i = 0; i < config_ptr->AssassinItemSet.size(); i++)
			{
				add(config_ptr->AssassinItemSet[i], id, i);
			}

		}
	}
	void cphantom_config::add(int32 item_template_id, int32 id, int32 pos)
	{
		config_map_it it = m_config_map.find(item_template_id);
		if (it == m_config_map.end())
		{
			std::vector<s_id_pos> new_list;
			m_config_map.insert(std::make_pair(item_template_id, new_list));
		}
		it = m_config_map.find(item_template_id);
		std::vector<s_id_pos>& temp_list = it->second;
		if (is_exist_id_pos(temp_list, id, pos))
		{
			return;
		}	
		temp_list.push_back(s_id_pos(id, pos));
	}

	bool cphantom_config::is_exist_id_pos(std::vector<s_id_pos>& vec_list, int32 id, int32 pos)
	{
		if (vec_list.size() == 0)
		{
			return false;
		}
		for (int32 i = 0; i < vec_list.size(); i++)
		{
			s_id_pos& temp = vec_list[i];
			if (temp.id == id && temp.pos == pos)
			{
				return true;
			}
		}
		return false;
	}


	
}
