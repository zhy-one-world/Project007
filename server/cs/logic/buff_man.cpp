#include "logic/buff_man.h"
#include "server_log.hpp"
#include "logic/unit_man.h"
#include "Logic/buff_def.hpp"
#include "buff_set.h"
#include "event_processer.h"
#include "logic/unit.hpp"
#include "utility/parse_msg.h"

using namespace faith;

buff_set m_empty_buff_set;
buff_set* m_buff_player_ary[init_player_max];
buff_set* m_buff_npc_ary[init_npc_max];

bool buff_man::init_manager()
{
	ZoneScoped;
	memset(m_buff_player_ary, 0, sizeof(m_buff_player_ary));
	memset(m_buff_npc_ary, 0, sizeof(m_buff_npc_ary));
	return true;
}

buff_set& buff_man::get_buff_set_by_index(const int32& unit_array_index)
{
	ZoneScoped;
	if (unit_array_index < init_player_max)
	{
		if (unit_array_index > 0)
		{
			if (m_buff_player_ary[unit_array_index] == nullptr)
			{
				m_buff_player_ary[unit_array_index] = new buff_set();
				m_buff_player_ary[unit_array_index]->set_owner_array_index(unit_array_index);
			}
			m_buff_player_ary[unit_array_index]->clear_data();
			return *m_buff_player_ary[unit_array_index];
		}
	}
	else
	{
		int32 npc_array_index = unit_array_index - npc_arrary_index_begin;
		if (npc_array_index > 0 && npc_array_index < init_npc_max)
		{
			if (m_buff_npc_ary[npc_array_index] == nullptr)
			{
				m_buff_npc_ary[npc_array_index] = new buff_set();
				m_buff_npc_ary[npc_array_index]->set_owner_array_index(unit_array_index);
			}
			m_buff_npc_ary[npc_array_index]->clear_data();
			return *m_buff_npc_ary[npc_array_index];
		}
	}
	return m_empty_buff_set;
}

void buff_man::load(const int32& unit_array_index, const buff_proto_buff_save_load& load_data)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(unit_array_index);
	temp_buff_set.clear_data();
	temp_buff_set.set_owner_array_index(unit_array_index);
	temp_buff_set.load_buff(load_data);
}
bool buff_man::load_buff_by_db_lua(int32 unit_array_index, const char *data_ptr, int32 data_len)
{
	ZoneScoped;
	buff_proto_buff_save_load load_proto;

	bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&load_proto, data_ptr, data_len, false);

	if (!is_sucess)
	{
		return false;
	}

	load(unit_array_index, load_proto);
	return true;

}
void buff_man::save(const int32& unit_array_index, const int32& save_type_ex)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(unit_array_index);
	temp_buff_set.save_buff(save_type_ex);
}
void buff_man::heart_tick(const int32& unit_array_index, const int64& new_time)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(unit_array_index);
	temp_buff_set.heart_tick(new_time);
}

bool buff_man::on_event(s_trigger_param& trigger_param)
{
	//if (EGameEventType_TriggerBuf != event_ptr->m_type)
	//{
	//	return false;
	//}
	//buff_set it = m_buff_list.find(unit_id);
	//if (it != m_buff_list.end())
	//{
	//	(*it).second->on_event(event_ptr);
	//	++it;
	//}
	return true;
}

void buff_man::handle_msg(const int32& unit_array_index, buff_proto_buff_operation const& msg)
{
	ZoneScoped;
	switch (msg.operation_type())
	{
	case faith::e_buff_operation_type_del:
	{
		guid_64 buff_guid(msg.buff_guid());
		del_buff_by_inst_id(-1, unit_array_index, buff_guid, false, false);
	}
	break;

	default:
		break;
	}
}
guid_64 buff_man::add_buff_inst(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, buff_ex_env_param* penv, bool is_ignore_att, int32 effect_index)
{
	ZoneScoped;
	if (buff_template_id <= 0)
	{
		//CONSOLE_ERROR("buff_template_id is invalid buff_template_id:{}", buff_template_id);
		return BUFF_INVALID;
	}

	unit& sender_unit = unit_man::get_unit(sender_array_index);
	unit& target_unit = unit_man::get_unit(target_array_index);
	if (sender_unit.is_valid() == false || target_unit.is_valid() == false)
	{
		CONSOLE_ERROR("sender_array_index:{} target_array_index:{}", sender_array_index, target_array_index);
		return BUFF_INVALID;
	}
	/*	if (target_unit.is_dead())
	{
	return BUFF_INVALID;
	}*/
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	buff_item* buff_item_ptr = temp_buff_set.add_buff(sender_array_index, buff_template_id, penv, is_ignore_att, effect_index);
	if (buff_item_ptr)
	{
		//auto map_object_ptr = world_cs::get_map_by_guid<map_object>(target_unit.get_map_guid());
		//if (nullptr != map_object_ptr)
		//{
		//	map_object_ptr->add_buff(target_array_index, buff_template_id);
		//}
		return buff_item_ptr->get_buff_guid();
	}
	else
	{
		CONSOLE_ERROR("create fail buff_template_id:{}", buff_template_id);
	}
	return BUFF_INVALID;
}

bool buff_man::add_buff_pile(const int32&target_array_index, const guid_64& buff_guid)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.add_buff_pile(buff_guid, NULL);
}

bool buff_man::dec_buff_pile(const int32&sender_array_index, const int32& target_array_index, const guid_64& buff_guid, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.dec_buff_pile(sender_array_index, buff_guid, same_sender_del);
}


bool buff_man::del_buff_pile_by_template_id(const int32&sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.del_buff_pile_by_template_id(sender_array_index, buff_template_id, same_sender_del);
}

bool buff_man::del_buff_by_inst_id(const int32&sender_array_index, const int32& target_array_index, const guid_64& buff_guid, bool server, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.del_buff_by_inst_id(sender_array_index, buff_guid, server, same_sender_del);
}

bool buff_man::del_buff_by_template_id(const int32&sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.del_buff_by_template_id(sender_array_index, buff_template_id, same_sender_del);
}

bool buff_man::del_buff_by_template_id_one(const int32&sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.del_buff_by_template_id_one(sender_array_index, buff_template_id);
}

bool buff_man::del_buff_by_type(const int32& sender_array_index, const int32& target_array_index, const int32& buff_type)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return	temp_buff_set.del_buff_by_type(buff_type);
}

bool buff_man::del_buff_by_series_id(const int32&sender_array_index, const int32& target_array_index, const int32& series_id, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.del_buff_by_series_id(sender_array_index, series_id, same_sender_del);
}
bool buff_man::del_buff_when_map_end(const int32& target_array_index)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return	temp_buff_set.del_buff_when_map_end();
}
bool buff_man::del_all_buff(const int32& sender_array_index, const int32& target_array_index, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	temp_buff_set.clear_data();
	return	true;
}
bool buff_man::is_have_buff_by_inst_id(const int32& sender_array_index, const int32& target_array_index, const guid_64& buff_guid, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.is_have_buff_by_inst_id(sender_array_index, buff_guid, same_sender_del);
}

bool buff_man::is_have_buff_by_template_id(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.is_have_buff_by_template_id(sender_array_index, buff_template_id, same_sender_del);
}

bool buff_man::is_have_buff_by_serise_id(const int32& sender_array_index, const int32& target_array_index, const int32& serise_id, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.is_have_buff_by_serise_id(sender_array_index, serise_id, same_sender_del);
}
int32 buff_man::get_buff_pile_by_inst_id(const int32& sender_array_index, const int32& target_array_index, const guid_64& buff_guid, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.get_buff_pile_by_inst_id(sender_array_index, buff_guid, same_sender_del);
}

int32 buff_man::get_buff_pile_by_template_id(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.get_buff_pile_by_template_id(sender_array_index, buff_template_id, same_sender_del);
}

buff_item* buff_man::get_buff_item_by_series_id(const int32& sender_array_index, const int32& target_array_index, const int32& series_id, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.get_buff_item_by_series(sender_array_index, series_id, same_sender_del);
}

buff_item*	buff_man::get_buff_item_by_template_id(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.get_buff_item_by_template_id(sender_array_index, buff_template_id, same_sender_del);
}
void buff_man::unit_destroy(const int32&unit_array_index)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(unit_array_index);
	temp_buff_set.clear_data();
}

void buff_man::offline_buffs(const int32&unit_array_index)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(unit_array_index);
	temp_buff_set.offline_buffs();
}

guid_64 buff_man::find_buff_by_template_id(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	return temp_buff_set.find_buff_by_template_id(sender_array_index, buff_template_id, same_sender_del);
}

void buff_man::sync_buff_set_info(int32 sender_array_index, int32 target_array_index)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(sender_array_index);
	temp_buff_set.sync_to_client(target_array_index);
}
void buff_man::mod_buff_during_time_by_series_id(const int32& target_array_index, const int32& series_id, const int32& during_time_fix)
{
	ZoneScoped;
	buff_set& temp_buff_set = get_buff_set_by_index(target_array_index);
	temp_buff_set.mod_buff_during_time_by_series_id(series_id, during_time_fix);
}
