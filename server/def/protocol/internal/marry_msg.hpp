/********************************************************************
  created: 2017/12/06 
  file base: marry
  file ext: hpp
  author: Allen_lei
  
  purpose: 
*********************************************************************/
#ifndef _MARRY_MSG_HPP_
#define _MARRY_MSG_HPP_

#include "logic/type_def.hpp" 
#include "base.hpp"   
#include "Logic/marry_def.hpp"
#include "internet/ws2cs.pb.h"

namespace faith
{

#pragma pack(push,1)

	enum
	{
		// Enum marry
		e_msgindex_ws2dp_save_marry_info = e_msg_base_marry,
		e_msgindex_dp2ws_save_marry_info_end,
		e_msgindex_ws2dp_load_marry_info,
		e_msgindex_dp2ws_load_marry_info_end,
		e_msgindex_ws2dp_del_couple_info,
		e_msgindex_ws2cs_check_item_enough,
		e_msgindex_ws2cs_marry_result,
		e_msgindex_cs2ws_operate_result_end,
		e_msgindex_ws2cs_cut_marry_item,
		e_msgindex_ws2dp_marry_get_last_login_time,
		e_msgindex_dp2ws_marry_get_last_login_time_end,
		e_msgindex_ws2cs_marry_get_reward,
		e_msgindex_cs2ws_marry_get_reward_end,
		e_msgindex_ws2cs_update_wedding_ring_state,
		e_msgindex_cs2ws_update_wedding_ring_level,
		e_msgindex_ws2cs_update_wedding_ring_level,
		e_msgindex_ws2dp_load_marry_info_cross_server,
		e_msgindex_dp2ws_load_marry_info_cross_server_end,
		e_msgindex_ws2cs_marry_get_marry_task,
		e_msgindex_ws2cs_sync_marry_task,
		e_msgindex_cs2ws_add_heart_value,
		e_msgindex_ws2cs_refresh_heart_value,
		e_msgindex_ws2cs_req_cost_bless_money,
		e_msgindex_cs2ws_cost_bless_money_result,
		e_msgindex_cs2ws_send_lucky_info_to_client,
		e_msgindex_ws2cs_return_special_item_times,
	};
	/************************************************************************/
	/*           Enum                                                      */
	/***********************************************************************/
	struct ws2dp_save_marry_couple_info : public packet_base
	{
		marry_record_data	data_one;
		bool				is_divorce;
		ws2dp_save_marry_couple_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_marry_info;
		}
	};

	struct ws2dp_load_marry_couple_info : public packet_base
	{
		ws2dp_load_marry_couple_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_marry_info;
		}

		int32 get_pak_length()
		{
			const int32 basic_len = sizeof(*this);
			return (basic_len);
		}

	};

	struct dp2ws_load_marry_couple_info_end : public packet_base
	{
		marry_record_data	load_data[max_load_num];
		int32				load_num;
		dp2ws_load_marry_couple_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_marry_info_end;
		}
	};

	struct ws2cs_check_item_enough : public packet_base
	{
		guid_64				main_guid;
		guid_64				sub_guid;
		int32				item_id;
		bool				is_cost_item;
		bool				is_divorce;
		ws2cs_check_item_enough()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_check_item_enough;
		}
		void to_proto(faith::ws2cs_proto::check_item_enough& msg)
		{
			msg.set_main_guid(main_guid.server_64);
			msg.set_sub_guid(sub_guid.server_64);
			msg.set_item_id(item_id);
			msg.set_is_cost_item(is_cost_item);
			msg.set_is_divorce(is_divorce);
		}
		void from_proto(const faith::ws2cs_proto::check_item_enough& msg)
		{
			main_guid.server_64 = msg.main_guid();
			sub_guid.server_64 = msg.sub_guid();
			item_id = msg.item_id();
			is_cost_item = msg.is_cost_item();
			is_divorce = msg.is_divorce();
		}
	};

	struct cs2ws_operate_result_end : public packet_base
	{
		guid_64				main_guid;
		guid_64				sub_guid;
		int32				result_type;
		int32				item_id;
		bool				is_cost_item;
		bool				is_divorce;
		cs2ws_operate_result_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_operate_result_end;
		}
	};

	struct ws2cs_marry_result_end : public packet_base
	{
		guid_64				main_guid;
		xchar				main_name[max_name_size + 1];
		guid_64				sub_guid;
		xchar				sub_name[max_name_size + 1];
		int32				result_type;
		bool				is_marry;
		int32				marry_item_id;
		ws2cs_marry_result_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_marry_result;
		}
		void to_proto(faith::ws2cs_proto::marry_result_end& msg)
		{
			msg.set_main_guid(main_guid.server_64);
			msg.set_main_name(main_name);
			msg.set_sub_guid(sub_guid.server_64);
			msg.set_sub_name(sub_name);
			msg.set_result_type(result_type);
			msg.set_is_marry(is_marry);
			msg.set_marry_item_id(marry_item_id);
		}
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::ws2cs_proto::marry_result_end& msg)
		{
			main_guid.server_64 = msg.main_guid();
			my_memcopy_string(main_name, max_name_size, msg.main_name());
			sub_guid.server_64 = msg.sub_guid();
			my_memcopy_string(sub_name, max_name_size, msg.sub_name());
			result_type = msg.result_type();
			is_marry = msg.is_marry();
			marry_item_id = msg.marry_item_id();
		}

	};

	struct ws2dp_marry_get_last_login_time : public packet_base
	{
		guid_64				sender_guid;
		guid_64				target_guid;

		ws2dp_marry_get_last_login_time()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_marry_get_last_login_time;
		}
	};

	struct dp2ws_marry_get_last_login_time_end : public packet_base
	{
		guid_64				sender_guid;
		int64				last_login;

		dp2ws_marry_get_last_login_time_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_marry_get_last_login_time_end;
		}
	};

	struct ws2cs_marry_get_reward : public packet_base
	{
		guid_64				sender_guid;
		int32               old_marry_state;

		ws2cs_marry_get_reward()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_marry_get_reward;
		}
		void to_proto(faith::ws2cs_proto::marry_get_reward& msg)
		{
			msg.set_sender_guid(sender_guid.server_64);
			msg.set_old_marry_state(old_marry_state);
		}
		void from_proto(const faith::ws2cs_proto::marry_get_reward& msg)
		{
			sender_guid.server_64 = msg.sender_guid();
			old_marry_state = msg.old_marry_state();
		}
	};
	struct ws2cs_marry_get_reward_end : public packet_base
	{
		guid_64				sender_guid;
		int32				get_result;
		int32               old_marry_state;
		ws2cs_marry_get_reward_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_marry_get_reward_end;
		}
	};

	struct ws2cs_update_wedding_ring_state : public packet_base
	{
		guid_64				role_guid;
		bool				is_marry;
		int32				marry_item_id;
		int32				mate_ring_level;
		int32				heart_value;
		ws2cs_update_wedding_ring_state()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_update_wedding_ring_state;
		}
		void to_proto(faith::ws2cs_proto::update_wedding_ring_state& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_is_marry(is_marry);
			msg.set_marry_item_id(marry_item_id);
			msg.set_mate_ring_level(mate_ring_level);
			msg.set_heart_value(heart_value);
		}
		void from_proto(const faith::ws2cs_proto::update_wedding_ring_state& msg)
		{
			role_guid.server_64 = msg.role_guid();
			is_marry = msg.is_marry();
			marry_item_id = msg.marry_item_id();
			mate_ring_level = msg.mate_ring_level();
			heart_value = msg.heart_value();
		}
	};

	struct cs2ws_update_wedding_ring_level : public packet_base
	{
		guid_64				role_guid;
		int32				ring_level;
		cs2ws_update_wedding_ring_level()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_update_wedding_ring_level;
		}
	};

	struct ws2cs_update_wedding_ring_level : public packet_base
	{
		guid_64				role_guid;
		int32				mate_ring_level;
		ws2cs_update_wedding_ring_level()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_update_wedding_ring_level;
		}
		void to_proto(faith::ws2cs_proto::update_wedding_ring_level& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_mate_ring_level(mate_ring_level);
		}
		void from_proto(const faith::ws2cs_proto::update_wedding_ring_level& msg)
		{
			role_guid.server_64 = msg.role_guid();
			mate_ring_level = msg.mate_ring_level();
		}
	};

	struct ws2dp_load_marry_info_cross_server : public packet_base
	{
		guid_64				role_guid;
		//int32				server_id;
		ws2dp_load_marry_info_cross_server()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_marry_info_cross_server;
		}
	};

	struct dp2ws_load_marry_info_cross_server_end : public packet_base
	{
		guid_64				role_guid;
		marry_record_data	data_one;
		dp2ws_load_marry_info_cross_server_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_marry_info_cross_server_end;
		}
	};
	
	struct ws2cs_marry_get_marry_task : public packet_base
	{
		guid_64				role_guid;
		ws2cs_marry_get_marry_task()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_marry_get_marry_task;
		}
		void to_proto(faith::ws2cs_proto::marry_get_marry_task& msg)
		{
			msg.set_role_guid(role_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::marry_get_marry_task& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};
	
	struct ws2cs_sync_marry_task : public packet_base
	{
		guid_64				role_guid;
		bool				is_marry;
		ws2cs_sync_marry_task()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_sync_marry_task;
		}
		void to_proto(faith::ws2cs_proto::sync_marry_task& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_is_marry(is_marry);
		}
		void from_proto(const faith::ws2cs_proto::sync_marry_task& msg)
		{
			role_guid.server_64 = msg.role_guid();
			is_marry = msg.is_marry();
		}
	};

	struct cs2ws_add_heart_value : public packet_base
	{
		guid_64				role_guid;
		int32				add_value;
		int32				send_type;
		cs2ws_add_heart_value()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_add_heart_value;
		}
	};
	struct ws2cs_refresh_heart_value : public packet_base
	{
		guid_64				role_guid;
		int32				heart_value;
		ws2cs_refresh_heart_value()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_refresh_heart_value;
		}
		void to_proto(faith::ws2cs_proto::refresh_heart_value& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_heart_value(heart_value);
		}
		void from_proto(const faith::ws2cs_proto::refresh_heart_value& msg)
		{
			role_guid.server_64 = msg.role_guid();
			heart_value = msg.heart_value();
		}
	};

	struct ws2cs_req_cost_bless_money : public packet_base
	{
		guid_64				role_guid;
		guid_64				target_guid;
		ws2cs_req_cost_bless_money()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_req_cost_bless_money;
		}
		void to_proto(faith::ws2cs_proto::req_cost_bless_money& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_target_guid(target_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::req_cost_bless_money& msg)
		{
			role_guid.server_64 = msg.role_guid();
			target_guid.server_64 = msg.target_guid();
		}
	};

	struct cs2ws_cost_bless_money_result : public packet_base
	{
		xchar				role_name[max_name_size + 1];
		guid_64				target_guid;
		cs2ws_cost_bless_money_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_cost_bless_money_result;
		}
	}; 

	struct cs2ws_send_lucky_info_to_client : public packet_base
	{
		guid_64				target_guid;
		int32				req_type;
		cs2ws_send_lucky_info_to_client()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_lucky_info_to_client;
		}
	};

	struct ws2cs_return_special_item_times : public packet_base
	{
		guid_64				target_guid;
		ws2cs_return_special_item_times()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_return_special_item_times;
		}
		void to_proto(faith::ws2cs_proto::return_special_item_times& msg)
		{
			msg.set_target_guid(target_guid);
		}
		void from_proto(const faith::ws2cs_proto::return_special_item_times& msg)
		{
			target_guid.server_64 = msg.target_guid();
		}
	};
 #pragma pack(pop)

}

#endif 
