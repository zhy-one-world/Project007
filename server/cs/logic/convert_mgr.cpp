/********************************************************************
created: 2016年7月14日13:56:28
file base: convert_mgr
file ext: cpp
author: zhangminghai

purpose:about role's convert_mgr
*********************************************************************/

#include "convert_mgr.h"
#include "logic/player.hpp"
#include "template/template_manager.h"
#include "internet/cs2dp.pb.h"
#include "utility/parse_msg.h"
#include "internet/convert.pb.h"
#include "internet/net.pb.h"
#include "internal/convert_msg.hpp"
#include "utility/init_unit.h"

namespace faith
{

	convert_mgr::convert_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();
	}

	convert_mgr::~convert_mgr(void)
	{
		clear_data();
	}

	void convert_mgr::heart_tick(const int64& new_time)
	{

	}
	
	void convert_mgr::clear_data()
	{
		for (int32 i = 0; i < faith::e_convert_type_max;++i)
		{
			m_all_convert_time[i] = 0;
			m_vip_convert_time[i] = 0;
		}
	}
	void convert_mgr::reset_convert_data(bool is_begin)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(m_player_ptr->get_vip_level(false));
		if (vip_template_ptr == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < e_convert_type_max; ++i)
		{
			if (i == e_convert_type_bag_siliver)
			{
				continue;
			}
			int32 MaxLimit = 0;
			switch (i)
			{
			case e_convert_type_bind_diamond:
			{
				MaxLimit = vip_template_ptr->DiamondChangeTimes;
			}
			break;
			case e_convert_type_exp:
			{
				MaxLimit = vip_template_ptr->BindDiamondChangeTimes;
			}
			break;
			case e_convert_type_bind_siliver:
			{
				MaxLimit = vip_template_ptr->SiliverChangeTimes;
			}
			break;
			case e_convert_type_bag_bind_siliver: 
			{
				ConvertTemplate* template_ptr = template_manager::get_instance().get_convert_template_by_type(e_convert_type_bag_bind_siliver);
				if (template_ptr)
				{
					int32 vip_level = m_player_ptr->get_vip_level(false);
					if (!(template_ptr->ExperienceConversionTimes.size() % 2))
					{
						for (int32 i = 0; i < template_ptr->ExperienceConversionTimes.size() / 2; ++i)
						{
							if (template_ptr->ExperienceConversionTimes[i * 2] == vip_level)
							{
								MaxLimit = template_ptr->ExperienceConversionTimes[i * 2 + 1];
								break;
							}
						}
					}
				}

			}
			break;
			default:
				break;
			}
			if (is_begin)
			{
				m_vip_convert_time[i] = MaxLimit;
				m_all_convert_time[i] = MaxLimit;
			}
			else
			{
				if (m_vip_convert_time[i] > 0)
				{
					int32 diff_num = MaxLimit - m_vip_convert_time[i];
					m_vip_convert_time[i] = MaxLimit;
					m_all_convert_time[i] += diff_num;
				}
				else
				{
					m_vip_convert_time[i] = MaxLimit;
				}
			}
		}
		send_info_all();
	}

	void convert_mgr::load_data_from_db(const int32* alltime)
	{
		for (int32 i = 0; i < faith::e_convert_type_max; i++)
		{
			m_all_convert_time[i] = alltime[i];
		}
		reset_convert_data(false);
		force_refresh();
	}

	bool convert_mgr::load_convert_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_convert_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		int32 data_ary[e_convert_type_max] = { 0 };

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			data_ary[i] = msg.data_ary(i);
		}

		load_data_from_db(data_ary);

		return true;

	}

	void convert_mgr::send_info_all()
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		convert_proto_convert_info_all msg;
		for (int32 i = 0; i < faith::e_convert_type_max; i++)
		{
			msg.add_convert_times(m_all_convert_time[i]);
		}

		for (int32 i = 0; i < faith::e_convert_type_max; i++)
		{
			msg.add_convert_max_times(m_vip_convert_time[i]);
		}
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_convert_update_all);
	}

	void convert_mgr::send_info_one(int32 convert_type)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (convert_type < 0 || convert_type >= e_convert_type_max)
		{
			return;
		}

		convert_proto_convert_info_one msg;
		msg.set_convert_type(convert_type);
		msg.set_convert_times(m_all_convert_time[convert_type]);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_convert_update_one);
	}

	void convert_mgr::save_data_to_db(int32 save_type)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_convert);
		if (is_use == false)
		{
			cs2dp_save_char_convert req;
			req.role_guid = m_player_ptr->get_unit_guid();
			req.unit_array_index = m_player_ptr->get_array_index();
			req.save_type_ex = save_type;
			for (int32 i = 0; i < e_convert_type_max; i++)
			{
				req.data_list[i] = m_all_convert_time[i];
			}

			m_player_ptr->send_message_to_dp(&req, sizeof(req));
		}
		else
		{
			faith::cs2dp_proto::save_role_convert msg;
			msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
			msg.set_unit_array_index(m_player_ptr->get_array_index());
			msg.set_save_type_ex(save_type);
			faith::cs2dp_proto::role_convert_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			for (int32 i= 0; i < e_convert_type_max; i++)
			{
				db_data->add_data_ary(m_all_convert_time[i]);
			}

			m_player_ptr->send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_convert);
		}
	}
	int32 convert_mgr::get_convert_time(e_convert_type convert_type)
	{
		if (convert_type < e_convert_type_max)
		{
			return m_vip_convert_time[convert_type] - m_all_convert_time[convert_type];
		}
		else
		{
			return 0;
		}
	}

	int32 convert_mgr::get_m_all_convert_time(e_convert_type convert_type) 
	{
		if (convert_type < e_convert_type_max)
		{
			return m_all_convert_time[convert_type];
		}
		else
		{
			return 0;
		}
	}

	int32 convert_mgr::get_m_vip_convert_time(e_convert_type convert_type)
	{
		if (convert_type < e_convert_type_max)
		{
			return m_vip_convert_time[convert_type];
		}
		else
		{
			return 0;
		}
	}

	void convert_mgr::set_m_all_convert_time(e_convert_type convert_type , int32 new_m_all_convert_time)
	{
		if (convert_type < e_convert_type_max)
		{
			m_all_convert_time[convert_type] = new_m_all_convert_time;
		}
		else
		{
			return;
		}
	}
	xstring convert_mgr::lua_get_time_to_str(e_time_type time_type)
	{
		if (nullptr == m_player_ptr)
		{
			return "";
		}
		return init_unit::change_i64_to_string(m_player_ptr->get_time_data(time_type)).c_str();
	}

	void convert_mgr::force_refresh() //用m_all_convert_time[4]，十进制不同位，存储第一次免费刷新状态（0未刷新，1已刷新），1234位分别表示不同货币转换，第五位用来强制刷新转换次数	
	{
		int32 num = m_all_convert_time[4];
		if (num	< pow(10,5))
		{
			m_all_convert_time[4] += pow(10, 5);
			reset_convert_data(true);
		}
		else
		{
			num = num / pow(10, 5);
			num = num % 10;
			if (num == 0)
			{
				m_all_convert_time[4] += pow(10, 5);
				reset_convert_data(true);
			}
		}
	}
}