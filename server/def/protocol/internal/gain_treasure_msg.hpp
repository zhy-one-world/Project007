/********************************************************************
created: 2018年5月5日
file base: gain_treasure_msg
file ext: hpp
author: wangdi

purpose: 打宝相关
*********************************************************************/

#ifndef _FAITH_GAIN_TREASURE_HPP_
#define _FAITH_GAIN_TREASURE_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/gain_treasure_def.hpp"
#include "internet/dp2s.pb.h"

namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_gain_treasure_info = faith::e_msg_base_gain_treasure,
		e_msgindex_cs2dp_save_gain_treasure_info,

		e_msgindex_ws2dp_load_all_gain_treasure_record_info,
		e_msgindex_dp2ws_load_all_gain_treasure_record_info,
		e_msgindex_ws2dp_save_gain_treasure_record_info,

		e_msgindex_cs2ws_update_gain_treasure_boss_info,
		e_msgindex_cs2ws_add_gain_treasure_record_info,
	};

	struct dp2cs_load_gain_treasure_info : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		bool								is_over;
		s_gain_treasure_info				info_data;
		dp2cs_load_gain_treasure_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_gain_treasure_info;
		}
	};

	struct cs2dp_save_gain_treasure_info : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		s_gain_treasure_info				info_data;
		cs2dp_save_gain_treasure_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_gain_treasure_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&info_data - (ULONG_PTR)&wheader;
			return (basic_len + sizeof(s_gain_treasure_info));
		}
	};

	struct ws2dp_load_all_gain_treasure_record_info : public faith::packet_base
	{
		int32								boss_spawn_point_id;
		ws2dp_load_all_gain_treasure_record_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_all_gain_treasure_record_info;
		}
	};

	struct dp2ws_load_all_gain_treasure_record_info : public faith::packet_base
	{
		int32								data_num;
		s_gain_treasure_record_info_one_db	record_infos[gain_treasure_killed_record_one_boss_max_size];

		dp2ws_load_all_gain_treasure_record_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_gain_treasure_record_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&record_infos - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_gain_treasure_record_info_one_db));
		}
		bool  to_proto(faith::dp2s_proto::load_all_gain_treasure_record_info& msg)
		{
			msg.set_data_num(data_num);
			for (int32 i = 0; i < data_num &&  i < gain_treasure_record_info_max_size; i++)
			{
				faith::st_proto::st_gain_treasure_record_info_one_db * st_treasure_ptr = msg.add_record_infos();
				if (st_treasure_ptr == nullptr)
				{
					return false;
				}
				record_infos[i].to_proto(st_treasure_ptr);
			}
			return true;
		}
		void  from_proto(const faith::dp2s_proto::load_all_gain_treasure_record_info& msg)
		{
			data_num = msg.data_num();
			for (int32 i = 0;  i < gain_treasure_record_info_max_size && i < msg.record_infos_size(); i++)
			{
				record_infos[i].from_proto(msg.record_infos(i));
			}
		}
	};

	struct ws2dp_save_gain_treasure_record_info : public faith::packet_base
	{
		int32								del_stamp;
		s_gain_treasure_record_info_one_db	record_info;
				
		ws2dp_save_gain_treasure_record_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_gain_treasure_record_info;
		}
	};

	struct cs2ws_update_gain_treasure_boss_info : public faith::packet_base
	{
		s_gain_treasure_boss_info			boss_info;

		cs2ws_update_gain_treasure_boss_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_update_gain_treasure_boss_info;
		}
	};
	
	struct cs2ws_add_gain_treasure_record_info : public faith::packet_base
	{
		//s_gain_treasure_record_info_one		record_info;
		int32 boss_spawn_point_temp_id;
		int32 killed_stamp;
		int32 data_num;
		s_gain_treasure_record_player_info_one_things		player_info[gain_treasure_gain_item_max_num];
		cs2ws_add_gain_treasure_record_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_add_gain_treasure_record_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&player_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_gain_treasure_record_player_info_one_things));
		}
	};

#pragma pack(pop)
}

#endif
