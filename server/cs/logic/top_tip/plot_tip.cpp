
#include "plot_tip.h"
#include "server_log.hpp"
#include "logic/player.hpp"
#include "utility/init_unit.h"
#include "internet/game.pb.h"
#include "internet/net.pb.h"

namespace faith
{
	plot_tip::plot_tip()
	{
		clear_data();
	}

	plot_tip::~plot_tip()
	{
		pop_destroy_plot_tip();
	}

	void plot_tip::clear_data()
	{
		m_end_time = 0;

		for (int32 i = 0; i < max_plot_tip_entry_num; ++i)
		{
			s_plot_tip_entry& entry = m_plot_tips[i];
			entry.clear_data();
		}
	}

	void plot_tip::pop_destroy_plot_tip()
	{
		for (int32 i = 0; i < max_plot_tip_entry_num; ++i)
		{
			s_plot_tip_entry& entry = m_plot_tips[i];
			if (e_plot_type_when_destroy == entry.type)
			{
				on_pop_plot_tip(entry.str_id);
				entry.clear_data();
			}
		}
	}

	bool plot_tip::init(const std::vector<int32>& plot_tips_arr, guid_64 map_obj_guid, float end_time)
	{
		int32 temp_len = plot_tips_arr.size();
		if (temp_len <= 0 || temp_len % e_plot_tip_arr_tuple_index_max != 0)
		{
			CONSOLE_ERROR("plot_tip::init(): plot_tips_arr.size() = %d", temp_len);
			return false;
		}

		m_map_obj_guid = map_obj_guid;
		m_end_time = init_unit::get_end_time(end_time);

		int32 index = 0;
		for (int32 i = 0; i < temp_len; i += e_plot_tip_arr_tuple_index_max)
		{
			if (index >= max_plot_tip_entry_num)
			{
				// 如果进入了这个分支就说明表格中的数据太多了,目前每个plot_tip最多支持提示5段剧情文字,
				// 如果想要让支持的数目更多,只需将max_plot_tip_entry_num的值改大即可.
				CONSOLE_ERROR("plot_tip::init(): index:{} max_plot_tip_entry_num:{}", index, max_plot_tip_entry_num);
				return false;
			}

			e_plot_type plot_tip_pop_type = (e_plot_type)(plot_tips_arr[i + e_plot_tip_arr_tuple_index_type]);
			int32 str_id = plot_tips_arr[i + e_plot_tip_arr_tuple_index_str_id];

			if (e_plot_type_when_create == plot_tip_pop_type)
			{
				on_pop_plot_tip(str_id);
			}
			else
			{
				s_plot_tip_entry& entry = m_plot_tips[index];
				entry.type = plot_tip_pop_type;
				switch (plot_tip_pop_type)
				{
				case e_plot_type_after_begin:
					entry.time = init_unit::get_end_time(plot_tips_arr[i + e_plot_tip_arr_tuple_index_time]);
					break;

				case e_plot_type_before_end:
					entry.time = m_end_time - (plot_tips_arr[i + e_plot_tip_arr_tuple_index_time] * 1000);
					break;
				}
				entry.str_id = str_id;
				++index;
			}
		}

		return true;
	}

	void plot_tip::tick(int32 new_time)
	{
		for (int32 i = 0; i < max_plot_tip_entry_num; ++i)
		{
			s_plot_tip_entry& entry = m_plot_tips[i];
			if (entry.is_valid() == false)
			{
				continue;
			}
			switch (entry.type)
			{
			case e_plot_type_after_begin:
			case e_plot_type_before_end:
				if (new_time >= entry.time)
				{
					on_pop_plot_tip(entry.str_id);
					entry.clear_data();
				}
				break;
			}
		}
	}

	void plot_tip::send_pop_plot_tip(player& player_ref, int32 str_id)
	{
		game_proto_pop_plot_tip pop_plot_tip_msg;
		pop_plot_tip_msg.set_str_id(str_id);
		player_ref.send_message_to_self(&pop_plot_tip_msg, e_msgindex_s2c_pop_plot_tip);
	}

	void plot_tip::on_pop_plot_tip(int32 str_id)
	{
		if (m_map_obj_guid.is_valid() == false || str_id <= 0)
		{
			return;
		}

		//auto map_object_ptr = world_cs::get_map_by_guid<map_object>(m_map_obj_guid);
		//if (map_object_ptr)
		//{
		//	map_object_ptr->on_pop_plot_tip(str_id);
		//}
	}
}