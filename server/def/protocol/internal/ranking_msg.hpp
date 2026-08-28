
/********************************************************************
created: 2016/08/23
created: 23:8:2016 14:11
file base: ranking 排行榜
file ext: hpp
author: XingLong

purpose:
*********************************************************************/

#ifndef _FAITH_RANKING_HPP_
#define _FAITH_RANKING_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/ranking_def.hpp"
#include "Logic/arena_def.hpp"

namespace hld
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_ws2dp_load_all_ranking_info = hld::e_msg_base_ranking,
		e_msgindex_dp2ws_load_all_ranking_info,

		e_msgindex_ws2dp_save_ranking_worship,
		e_msgindex_dp2ws_save_ranking_worship,

		e_msgindex_ws2dp_add_ranking_worship,

		e_msgindex_cs2dp_save_worship_target,
		e_msgindex_dp2cs_load_worship_target,

		e_msgindex_dp2ws_load_ranking_info_one,
		//e_msgindex_cs2dp_save_ranking_info,
		e_msgindex_ws2dp_save_ranking_info_one,
		e_msgindex_cs2ws_sync_ranking_value,

		e_msgindex_ws2dp_get_worship_value,
		e_msgindex_dp2ws_get_worship_value_end,
		e_msgindex_ws2dp_update_ranking_vip_level,
		e_msgindex_ws2dp_save_one_total_ranking_info,

		e_msgindex_ws2dp_save_one_service_rank_info,
		e_msgindex_ws2dp_load_all_service_rank_info,
		e_msgindex_dp2ws_load_all_service_rank_info,

		e_msgindex_cs2ws_sync_oracle_trial_rank,
		e_msgindex_ws2ws_sync_oracle_trial_rank,
		e_msgindex_ws2ws_get_oracle_trial_rank,
		e_msgindex_ws2ws_get_oracle_trial_rank_end,

	};


	//***********/
	//ws2dp 请求读取排行榜记录
	struct ws2dp_load_all_ranking_info : public hld::packet_base
	{
		int32								ranking_type;
		ws2dp_load_all_ranking_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_all_ranking_info;
		}

		uint32 get_pak_length() const
		{
			return sizeof(ws2dp_load_all_ranking_info);
		}
	};

	struct ws2dp_save_one_total_ranking_info : public hld::packet_base
	{
		int32							data_num;
		int32							data_type;
		s_ranking_top_info				data_info;

		ws2dp_save_one_total_ranking_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_one_total_ranking_info;
		}
		 
	};

	struct ws2dp_save_one_service_rank_info : public hld::packet_base
	{
		int32							data_num;
		s_service_rank_info				data_info;

		ws2dp_save_one_service_rank_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_one_service_rank_info;
		}

	};

	struct dp2ws_load_all_service_rank_info : public hld::packet_base
	{
		int32							data_num;
		s_service_rank_info				data_info;

		dp2ws_load_all_service_rank_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_service_rank_info;
		}

		uint32 get_pak_length() const
		{
			return sizeof(dp2ws_load_all_service_rank_info);
		}
	};

	struct dp2ws_load_all_ranking_info : public hld::packet_base
	{
		int32							data_num;
		int32							ranking_type;
		s_ranking_top_info				data_info;

		dp2ws_load_all_ranking_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_ranking_info;
		}

		uint32 get_pak_length() const
		{
			return sizeof(dp2ws_load_all_ranking_info);
		}
	};

	struct ws2dp_save_ranking_info_one : public hld::packet_base
	{
		s_ranking_player_info				ranking_info;
		ws2dp_save_ranking_info_one()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_ranking_info_one;
		}

		uint32 get_pak_length() const
		{
			return sizeof(ws2dp_save_ranking_info_one);
		}
	};

	struct dp2ws_load_one_ranking_info : public hld::packet_base
	{
		guid_64								sender_guid;
		int32								ranking_type;
		int32								worship_add_value;
		int32								worship_type;
		int32								end_result;
		s_ranking_player_info				data_info;
		dp2ws_load_one_ranking_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_ranking_info_one;
		}
	};

	struct ws2dp_save_ranking_worship : public hld::packet_base
	{
		guid_64						sender_guid;
		s_ranking_player_info		target_data;
		int64						worship_add_value;
		int32						worship_type;

		ws2dp_save_ranking_worship()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_ranking_worship;
		}
	};

	struct cs2ws_sync_ranking_value : public hld::packet_base
	{
		s_ranking_player_info				data_info;
		cs2ws_sync_ranking_value()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_sync_ranking_value;
		}
	};

	struct ws2dp_get_worship_value : public hld::packet_base
	{
		guid_64				sender;
		guid_64				target;

		ws2dp_get_worship_value()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_get_worship_value;
		}

		uint32 get_pak_length() const
		{
			return sizeof(ws2dp_get_worship_value);
		}
	};


	struct dp2ws_get_worship_value_end : public hld::packet_base
	{
		guid_64						sender;
		s_ranking_player_info		target;
		bool						is_load;

		dp2ws_get_worship_value_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_get_worship_value_end;
		}

		uint32 get_pak_length() const
		{
			return sizeof(dp2ws_get_worship_value_end);
		}
	};


	struct ws2dp_add_robot_worship : public hld::packet_base
	{
		int32						data_num;
		s_ranking_worship_info		data_array[rank_max];
		ws2dp_add_robot_worship()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_add_ranking_worship;
		}

		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_array - (ULONG_PTR)&wheader;
			return basic_len + data_num * sizeof(s_ranking_worship_info);
		}
	};


	struct cs2dp_save_char_worship_target : public hld::packet_base
	{
		guid_64					role_guid;
		int32					unit_array_index;
		int32					save_type_ex;
		int32					data_num;
		worship_record			data_list[DaliyWorshipTime];
		cs2dp_save_char_worship_target()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_worship_target;
		}

		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(worship_record));
		}
	};


	struct dp2cs_load_worship_target : public hld::packet_base
	{
		guid_64								role_guid;
		int32							unit_array_index;
		bool								is_over;
		int32								data_num;
		worship_record						data_list[DaliyWorshipTime];
		dp2cs_load_worship_target()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_worship_target;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(worship_record));
		}
	};

	struct ws2dp_load_all_service_rank_info : public hld::packet_base
	{
		int32								rank_type;
		ws2dp_load_all_service_rank_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_all_service_rank_info;
		}

		uint32 get_pak_length() const
		{
			return sizeof(ws2dp_load_all_service_rank_info);
		}
	};

	struct cs2ws_sync_oracle_trial_rank : public hld::packet_base
	{
		guid_64 role_guid;
		int32	rank_value;
		cs2ws_sync_oracle_trial_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_sync_oracle_trial_rank;
		}
	};

	
	struct ws2ws_sync_oracle_trial_rank : public hld::packet_base
	{
		s_ranking_player_info				play_info;
		ws2ws_sync_oracle_trial_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_sync_oracle_trial_rank;
		}
	};

	struct ws2ws_get_oracle_trial_rank : public hld::packet_base
	{
		int32				server_id;
		guid_64				play_guid;
		ws2ws_get_oracle_trial_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_get_oracle_trial_rank;
		}
	};

	struct ws2ws_get_oracle_trial_rank_end: public hld::packet_base
	{
		guid_64					play_guid;
		int32					data_num;
		s_ranking_player_info	play_info[oracle_trial_rank_info];
		ws2ws_get_oracle_trial_rank_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_get_oracle_trial_rank_end;
		}
	};

#pragma pack(pop)
}
#endif