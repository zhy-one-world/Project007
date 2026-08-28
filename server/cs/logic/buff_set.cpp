/********************************************************************
created:	2010/03/01
created:	1:3:2010   14:43
filename: 	d:\worksite\skill\LowServer\octopus\app\cs\Low\New_Buff\buff_set.cpp
file path:	d:\worksite\skill\LowServer\octopus\app\cs\Low\New_Buff
file base:	buff_set
file ext:	cpp
author:		zhy

purpose:
*********************************************************************/

#include "buff_set.h"
#include "cache_mgr.hpp"
#include "internal/buff_msg.hpp"
#include "internet/cs2dp.pb.h"
#include "internet/net.pb.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "utility/parse_msg.h"

namespace hld
{
	buff_set::buff_set()
	{
		m_owner_array_index = 0;
		clear_data();
	}
	buff_set::~buff_set()
	{
		clear_data();
	}
	void buff_set::set_owner_array_index(int32 unit_array_index)
	{
		m_owner_array_index = unit_array_index;
	}
	void buff_set::clear_data()
	{
		for (buff_item_map_it it = m_buff_inst_vector.begin(); it != m_buff_inst_vector.end(); ++it)
		{
			if (*it)
			{
				cache_mgr::get_instance().back_buff_item_ptr(*it);
			}
		}
		m_buff_inst_vector.clear();
		m_buff_inst_vector_len = 0;
	}
	buff_item* buff_set::add_buff_item(const int32& sender_index, const int32& buff_template_id, int32 skill_template_id, bool is_ignore_att, int32 effect_index, int32 buff_level)
	{
		int32 empty_index = -1;
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			if (m_buff_inst_vector[i] == nullptr)
			{
				empty_index = i;
				break;
			}
		}
		buff_item* buff_item_ref = cache_mgr::get_instance().get_buff_item_ptr();
		if (empty_index >= 0)
		{
			m_buff_inst_vector[empty_index] = buff_item_ref;
		}
		else
		{
			m_buff_inst_vector.push_back(buff_item_ref);
			empty_index = m_buff_inst_vector_len++;
		}
		guid_64 new_guid(m_owner_array_index + buff_guid_owner_up + sender_index, empty_index);
		buff_item_ref->init_data(m_owner_array_index, sender_index, new_guid, buff_template_id, skill_template_id, is_ignore_att, effect_index, buff_level);
		buff_item_ref->set_valid_state(true);
		return buff_item_ref;
	}
	void buff_set::remove_buff_item(const guid_64& buff_guid)
	{
		if (!buff_guid.is_valid())
		{
			return;
		}

		if (buff_guid.B < 0 || buff_guid.B >= m_buff_inst_vector_len)
		{
			return;
		}
		buff_item* buff_item_ref = m_buff_inst_vector[buff_guid.B];
		if (buff_item_ref)
		{
			m_buff_inst_vector[buff_guid.B] = nullptr;
			cache_mgr::get_instance().back_buff_item_ptr(buff_item_ref);
		}
	}
	buff_item* buff_set::get_buff_item_by_guid(const guid_64& buff_guid, int32 sender_index, bool same_sender_del)
	{
		if (buff_guid.B < 0 || buff_guid.B >= m_buff_inst_vector_len)
		{
			return nullptr;
		}
		buff_item* buff_item_ref = m_buff_inst_vector[buff_guid.B];
		if (nullptr == buff_item_ref)
		{
			return nullptr;
		}
		if (same_sender_del && buff_item_ref->get_sender_index() != sender_index)
		{
			return nullptr;
		}
		return buff_item_ref;
	}
	buff_item* buff_set::get_buff_item_by_series(const int32& sender_index, const int32& series_type, bool same_sender_del)
	{
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			buff_item* buff_item_ref = m_buff_inst_vector[i];
			if (nullptr == buff_item_ref)
			{
				continue;
			}
			if (buff_item_ref->get_series() == series_type)
			{
				if (same_sender_del)
				{
					if (buff_item_ref->get_sender_index() == sender_index)
					{
						return buff_item_ref;
					}
				}
				else
				{
					return buff_item_ref;
				}
			}
		}
		return nullptr;
	}
	buff_item* buff_set::get_buff_item_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del)
	{
		if (m_buff_inst_vector_len <= 0 || buff_template_id <= 0)
		{
			return nullptr;
		}
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			buff_item* buff_item_ref = m_buff_inst_vector[i];
			if (nullptr == buff_item_ref)
			{
				continue;
			}
			if (buff_item_ref->get_buff_template_id() == buff_template_id)
			{
				if (same_sender_del)
				{
					if (buff_item_ref->get_sender_index() == sender_index)
					{
						return buff_item_ref;
					}
				}
				else
				{
					return buff_item_ref;
				}
			}
		}
		return nullptr;
	}

	bool buff_set::save_buff(int32 save_type_ex)
	{
		player& player_ref = unit_man::get_player(m_owner_array_index);
		buff_proto_buff_save_load request_save;
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			buff_item* buff_item_ref = m_buff_inst_vector[i];
			if (nullptr == buff_item_ref)
			{
				continue;
			}
			if (buff_item_ref->get_save_type() == e_buff_svt_not_save)
			{
				continue;
			}
			buff_item_ref->save(request_save);
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_buff);
		if (is_use == false)
		{
			player_ref.send_message_to_dp(&request_save, e_msgindex_cs2dp_save_char_buff, save_type_ex);
		}
		else
		{
			hld::cs2dp_proto::save_role_db_data msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(player_ref.get_array_index());
			msg.set_save_type_ex(save_type_ex);
			packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&request_save);
			if (p_db == nullptr)
			{
				return false;
			}
			msg.set_db_data(p_db->google_data, p_db->google_data_len);

			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_buff);
		}
		return true;
	}
	void buff_set::load_buff(const buff_proto_buff_save_load& load_data)
	{
		int32 data_num = load_data.data_ary_size();
		buff_ex_env_param env;
		env.reload = true;
		for (int32 idx = 0; idx < data_num; ++idx)
		{
			const buff_proto_buff_info&  buff_info = load_data.data_ary(idx);
			if (buff_info.data_ary(EBuffInfo_template_id) == 0 || buff_info.data_ary(EBuffInfo_load_type) != e_buff_svt_save_load)
			{
				continue;
			}
			env.buff_level = buff_info.data_ary(EBuffInfo_pile_count);
			guid_64 temp_sender_guid = buff_info.sender_guid();
			unit& unit_ref = unit_man::get_unit(temp_sender_guid);
			buff_item* buff_item_ptr = add_buff(unit_ref.get_array_index(), buff_info.data_ary(EBuffInfo_template_id), &env);
			if (buff_item_ptr)
			{
				int64 life_time = buff_info.data_ary(EBuffInfo_time) + utility::get_tick_count();
				buff_item_ptr->set_buff_time(life_time);
			}
		}
		sync_to_client(m_owner_array_index);
	}
	void buff_set::heart_tick(const int64& new_time)
	{
		if (m_buff_inst_vector.empty())
		{
			return;
		}
		unit& owner_ptr = unit_man::get_unit(m_owner_array_index);
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			buff_item* buff_item_ref = m_buff_inst_vector[i];
			if (nullptr == buff_item_ref)
			{
				continue;
			}
			if (owner_ptr.is_dead() && buff_item_ref->is_dead_del())
			{
				buff_item_ref->dec_all_pile();
				remove_buff_item(buff_item_ref->get_buff_guid());
			}
			else
			{
				int32 nret = buff_item_ref->heart_tick(new_time);
				if (nret == buff_ret_kill)
				{
					if (buff_item_ref->is_keep() == false)
					{
						buff_item_ref->dec_all_pile(true);
					}
					remove_buff_item(buff_item_ref->get_buff_guid());
				}
			}
		}
		if (m_buff_update_time < new_time)
		{
			m_buff_update_time = new_time + second_tick_time;
			update_all_buff_need_refresh();
		}
		
	}
	buff_item*  buff_set::add_buff(const int32& sender_index, const int32& buff_template_id, buff_ex_env_param* pEnv, bool is_ignore_att, int32 effect_index)
	{
		return add_buff_inst(sender_index, buff_template_id, pEnv, is_ignore_att, effect_index);
	}
	buff_item* buff_set::add_buff_inst(const int32& sender_index, const int32& buff_template_id, buff_ex_env_param* pEnv, bool is_ignore_att, int32 effect_index)
	{
		unit& unit_ref = unit_man::get_unit(m_owner_array_index);
		if (unit_ref.is_valid() == false)
		{
			CONSOLE_ERROR("m_owner_array_index:{}", m_owner_array_index);
			return nullptr;
		}
		BuffDataTemplate* buff_template_ptr = GET_TEMPLATE(BuffDataTemplate, buff_template_id);
		if (buff_template_ptr == nullptr)
		{
			CONSOLE_ERROR("buff_template_ptr is null buff_template_id :{}", buff_template_id);
			return nullptr;
		}
		if (buff_template_ptr->InvalidBoss > 0)
		{
			if (unit_ref.get_unit_type() == e_unit_type_monster && init_unit::is_boss(unit_ref.get_unit_sub_type()) ||
				(unit_ref.get_unit_type() == e_unit_type_summoned && unit_ref.get_unit_sub_type() == e_summon_pet_type_boss_pet))
			{
				CONSOLE_ERROR("buff_template_id :{} InvalidBoss:{} unit_type:{} sub_type:{}", buff_template_id, buff_template_ptr->InvalidBoss, (int32)unit_ref.get_unit_type(), unit_ref.get_unit_sub_type());
				return nullptr;
			}
		}
		buff_item* buff_item_ptr = get_buff_item_by_series(sender_index, buff_template_ptr->Series, false);
		if (buff_item_ptr)
		{
			e_buff_add_judge judge_result = buff_item_ptr->add_buff_type(sender_index, buff_template_id);
			switch (judge_result)
			{
			case hld::e_buff_add_judge_can_cover:
				return cover_buff(sender_index, buff_item_ptr->get_buff_guid(), buff_template_id, pEnv, effect_index);
				break;
			case hld::e_buff_add_judge_can_wrap:
				return wrap_buff(sender_index, buff_item_ptr->get_buff_guid(), buff_template_id, pEnv);
				break;
			case hld::e_buff_add_judge_can_cumulative:
				return cumulative_buff(sender_index, buff_item_ptr->get_buff_guid(), buff_template_id, pEnv);
				break;
			case hld::e_buff_add_judge_direct_add:
				return tile_buff(sender_index, buff_template_id, pEnv, is_ignore_att, effect_index);
				break;
			case hld::e_buff_add_judge_can_percent:
				return percent_buff(sender_index, buff_item_ptr->get_buff_guid(), buff_template_id, pEnv);
				break;
			default:
				CONSOLE_INFO("buff_template_id :{} judge_result:{}", buff_template_id, (int32)judge_result);
				return buff_item_ptr;
				break;
			}
		}
		else
		{
			return tile_buff(sender_index, buff_template_id, pEnv, is_ignore_att, effect_index);
		}
		return nullptr;
	}

	buff_item* buff_set::add_buff_pile(const guid_64& buff_guid, buff_ex_env_param* pEnv)
	{
		if (!buff_guid.is_valid())
		{
			return nullptr;
		}
		buff_item* buff_item_ptr = get_buff_item_by_guid(buff_guid);
		if (nullptr == buff_item_ptr)
		{
			return nullptr;
		}
		buff_item_ptr->pile_one(pEnv, true);
		return buff_item_ptr;
	}

	bool buff_set::dec_buff_pile(const int32& sender_index, const guid_64& buff_guid, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_guid(buff_guid, sender_index, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return true;
		}
		int32 res_dec = buff_item_ptr->dec_pile();
		if (res_dec == buff_ret_kill)
		{
			remove_buff_item(buff_item_ptr->get_buff_guid());
		}
		return true;
	}


	bool buff_set::del_buff_pile_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_template_id(sender_index, buff_template_id, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return true;
		}
		return dec_buff_pile(sender_index, buff_item_ptr->get_buff_guid(), same_sender_del);
	}

	bool buff_set::del_buff_by_inst_id(const int32& sender_index, const guid_64& buff_guid, bool server, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_guid(buff_guid, sender_index, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return true;
		}
		kill_buff(buff_guid, server);
		return true;
	}

	bool buff_set::del_buff_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_template_id(sender_index, buff_template_id, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return true;
		}
		kill_buff(buff_item_ptr->get_buff_guid());
		return true;
	}

	bool buff_set::del_buff_by_template_id_one(const int32& sender_index, const int32& buff_template_id)
	{
		buff_item* buff_item_ptr = get_buff_item_by_template_id(sender_index, buff_template_id, false);
		if (nullptr == buff_item_ptr)
		{
			return true;
		}
		kill_buff(buff_item_ptr->get_buff_guid());
		return true;
	}

	bool buff_set::del_buff_by_series_id(const int32& sender_index, const int32& series_id, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_series(sender_index, series_id, false);
		if (nullptr == buff_item_ptr)
		{
			return true;
		}
		kill_buff(buff_item_ptr->get_buff_guid());
		return true;
	}
	bool buff_set::del_buff_when_map_end()
	{
		if (m_buff_inst_vector_len <= 0)
		{
			return true;
		}
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			buff_item* buff_item_ref = m_buff_inst_vector[i];
			if (nullptr == buff_item_ref)
			{
				continue;
			}
			if (false == buff_item_ref->is_map_end())
			{
				continue;
			}
			kill_buff(buff_item_ref->get_buff_guid());
		}
		return true;
	}
	bool buff_set::del_all_buff(const int32& sender_index, bool same_sender_del)
	{
		if (m_buff_inst_vector_len <= 0)
		{
			return true;
		}
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			buff_item* buff_item_ref = m_buff_inst_vector[i];
			if (nullptr == buff_item_ref)
			{
				continue;
			}
			if (same_sender_del && sender_index != buff_item_ref->get_sender_index())
			{
				continue;
			}
			kill_buff(buff_item_ref->get_buff_guid());
		}
		return true;
	}
	bool buff_set::del_buff_by_type(const int32& buff_type)
	{
		if (m_buff_inst_vector_len <= 0)
		{
			return true;
		}
		if (buff_type > e_buff_type_max)
		{
			for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
			{
				buff_item* buff_item_ref = m_buff_inst_vector[i];
				if (nullptr == buff_item_ref)
				{
					continue;
				}
				if (buff_type == buff_item_ref->get_buff_template_id())
				{
					kill_buff(buff_item_ref->get_buff_guid());
				}
			}

		}
		else
		{
			for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
			{
				buff_item* buff_item_ref = m_buff_inst_vector[i];
				if (nullptr == buff_item_ref)
				{
					continue;
				}
				if (buff_type == buff_item_ref->get_type())
				{
					kill_buff(buff_item_ref->get_buff_guid());
				}
			}
		}
		return true;
	}

	bool buff_set::is_have_buff_by_inst_id(const int32& sender_index, const guid_64& buff_guid, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_guid(buff_guid, sender_index, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return false;
		}
		return true;
	}

	bool buff_set::is_have_buff_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_template_id(sender_index, buff_template_id, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return false;
		}
		return true;
	}

	bool buff_set::is_have_buff_by_serise_id(const int32& sender_index, int32 serise_id, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_series(sender_index, serise_id, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return false;
		}
		return true;
	}
	int32 buff_set::get_buff_pile_by_inst_id(const int32& sender_index, const guid_64& buff_guid, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_guid(buff_guid, sender_index, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return 0;
		}
		return buff_item_ptr->get_cur_pile_count();
	}

	int32 buff_set::get_buff_pile_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_template_id(sender_index, buff_template_id, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return 0;
		}
		return buff_item_ptr->get_cur_pile_count();
	}

	int32 buff_set::get_buff_pile_by_series_id(const int32& sender_index, const int32& series_id, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_series(sender_index, series_id, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return 0;
		}
		return buff_item_ptr->get_cur_pile_count();
	}
	void buff_set::offline_buffs()
	{
		if (m_buff_inst_vector_len <= 0)
		{
			return;
		}
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			buff_item* buff_item_ref = m_buff_inst_vector[i];
			if (nullptr == buff_item_ref)
			{
				continue;
			}
			if (buff_item_ref->is_offline())
			{
				kill_buff(buff_item_ref->get_buff_guid());
			}
		}
	}
	guid_64 buff_set::find_buff_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del)
	{
		buff_item* buff_item_ptr = get_buff_item_by_template_id(sender_index, buff_template_id, same_sender_del);
		if (nullptr == buff_item_ptr)
		{
			return 0;
		}
		return buff_item_ptr->get_buff_guid();
	}

	buff_item* buff_set::tile_buff(const int32& sender_index, const int32& buff_template_id, buff_ex_env_param* pEnv, bool is_ignore_att, int32 effect_index)
	{
		buff_item* buff_item_ptr = add_buff_item(sender_index, buff_template_id, pEnv ? pEnv->skill_template_id : 0, is_ignore_att, effect_index, pEnv ? pEnv->buff_level : 1);
		if (nullptr == buff_item_ptr)
		{
			CONSOLE_ERROR("buff_item_ptr is null");
			return nullptr;
		}
		buff_item_ptr->pile_one(pEnv);
		return buff_item_ptr;
	}

	buff_item*  buff_set::cover_buff(const int32& sender_index, const guid_64& buff_guid, const int32& buff_template_id, buff_ex_env_param* pEnv, int32 effect_index)
	{
		if (!buff_guid.is_valid())
		{
			return nullptr;
		}
		if (kill_buff(buff_guid))
		{
			return tile_buff(sender_index, buff_template_id, pEnv, false, effect_index);
		}
		return nullptr;
	}


	buff_item*  buff_set::wrap_buff(const int32& sender_index, const guid_64& buff_guid, const int32& buff_template_id, buff_ex_env_param* pEnv)
	{
		if (!buff_guid.is_valid())
		{
			return nullptr;
		}
		return add_buff_pile(buff_guid, pEnv);
	}
	buff_item* buff_set::cumulative_buff(const int32& sender_index, const guid_64& buff_guid, const int32& buff_template_id, buff_ex_env_param* pEnv)
	{
		if (!buff_guid.is_valid())
		{
			return nullptr;
		}
		buff_item* buff_item_ptr = get_buff_item_by_guid(buff_guid);
		if (nullptr == buff_item_ptr)
		{
			return nullptr;
		}
		BuffDataTemplate* buff_data_template_ptr = GET_TEMPLATE(BuffDataTemplate, buff_template_id);
		if (buff_data_template_ptr)
		{
			buff_item_ptr->add_life_time(buff_data_template_ptr->DuringTime);
		}
		return buff_item_ptr;
	}
	buff_item* buff_set::percent_buff(const int32& sender_index, const guid_64& buff_guid, const int32& buff_template_id, buff_ex_env_param* pEnv)
	{
		if (!buff_guid.is_valid())
		{
			return nullptr;
		}
		buff_item* buff_item_ptr = get_buff_item_by_guid(buff_guid);
		if (nullptr == buff_item_ptr)
		{
			return nullptr;
		}
		BuffDataTemplate* buff_data_template_ptr = GET_TEMPLATE(BuffDataTemplate, buff_template_id);
		if (buff_data_template_ptr)
		{
			buff_item_ptr->change_buff_percent(buff_data_template_ptr->BuffPercent);
		}
		buff_item_ptr->begin_buff(nullptr);
		return buff_item_ptr;
	}
	bool buff_set::kill_buff(const guid_64& buff_guid, bool server)
	{
		if (!buff_guid.is_valid())
		{
			return true;
		}
		buff_item* buff_item_ptr = get_buff_item_by_guid(buff_guid);
		if (nullptr == buff_item_ptr)
		{
			return true;
		}
		if (server == false && buff_item_ptr->can_cannel() == false)
		{
			return false;
		}
		buff_item_ptr->dec_all_pile();
		remove_buff_item(buff_item_ptr->get_buff_guid());
		return true;
	}

	void buff_set::sync_to_client(const int32& recv_id)
	{
		if (m_buff_inst_vector_len <= 0)
		{
			return;
		}
		unit& target_unit_ref = unit_man::get_unit(m_owner_array_index);
		buff_proto_buff_info_all buff_info_all_msg;
		buff_info_all_msg.set_target(target_unit_ref.get_unit_guid().server_64);
		bool send_to = false;
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			buff_item* buff_item_ref = m_buff_inst_vector[i];
			if (nullptr == buff_item_ref)
			{
				continue;
			}
			if (buff_item_ref->is_sync() == false || buff_item_ref->get_buff_data_template()->OtherPlayerCanSee <= 0)
			{
				continue;
			}
			buff_item_ref->get_buff_info_msg(buff_info_all_msg);
			buff_info_all_msg.set_target(target_unit_ref.get_unit_guid().server_64);
			if (i % 700 == 0)
			{
				if (recv_id > 0)
				{
					unit& unit_ref = unit_man::get_unit(recv_id);
					unit_ref.send_message_to_self(&buff_info_all_msg, e_msgindex_s2c_buff_info_all);
				}
				else
				{
					unit& unit_ref = unit_man::get_unit(m_owner_array_index);
					unit_ref.send_message_to_aoi(&buff_info_all_msg, e_msgindex_s2c_buff_info_all, false);

				}
				buff_info_all_msg.Clear();
				send_to = false;
				continue;
			}
			send_to = true;
		}
		if (send_to)
		{
			if (recv_id > 0)
			{
				unit& unit_ref = unit_man::get_unit(recv_id);
				unit_ref.send_message_to_self(&buff_info_all_msg, e_msgindex_s2c_buff_info_all);
			}
			else
			{
				unit& unit_ref = unit_man::get_unit(m_owner_array_index);
				unit_ref.send_message_to_aoi(&buff_info_all_msg, e_msgindex_s2c_buff_info_all, false);

			}
		}
	}
	void buff_set::mod_buff_during_time_by_series_id(const int32& series_id, const int32& during_time_fix)
	{
		buff_item* buff_item_ptr = get_buff_item_by_series(m_owner_array_index, series_id, false);
		if (nullptr == buff_item_ptr)
		{
			return;
		}
		buff_item_ptr->change_life_time(during_time_fix);

	}
	int32 buff_set::init_fake_buff_info(s_buff_info * fake_buff_info, int32 max_num)
	{
		int32 buff_num = 0;
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			buff_item* buff_item_ref = m_buff_inst_vector[i];
			if (nullptr == buff_item_ref)
			{
				continue;
			}
			if (i >= buff_num)
			{
				break;
			}
			fake_buff_info[i] = buff_item_ref->get_fake_buff_info_inst();
			buff_num++;
		}
		return buff_num;
	}
	void buff_set::update_all_buff_need_refresh()
	{
		unit& owner_ptr = unit_man::get_unit(m_owner_array_index);
		for (uint32 i = 0; i < m_buff_inst_vector_len; ++i)
		{
			buff_item* buff_item_ref = m_buff_inst_vector[i];
			if (nullptr == buff_item_ref)
			{
				continue;
			}
			if (owner_ptr.is_dead())
			{
				continue;
			}
			buff_item_ref->check_need_update();
		}
	}
}
