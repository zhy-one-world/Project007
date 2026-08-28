/***********************************************
created:2019年5月23日
file base: cross_ladder_msg
file ext: hpp
author: pengbiao

purpose: 跨服天梯
************************************************/

#ifndef _FAITH_CROSS_LADDER_MSG_HPP_
#define _FAITH_CROSS_LADDER_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/cross_ladder_def.hpp"
#include "internet/ws2cs.pb.h"

namespace hld
{
#pragma pack(push, 1)

	enum
	{
		e_msgindex_ws2cs_cross_ladder_refresh_ticket_info = hld::e_msg_base_cross_ladder,
		e_msgindex_cs2ws_cross_ladder_req_buy_ticket,
		e_msgindex_cs2ws_cross_ladder_send_game_result,
		e_msgindex_ws2ws_cross_ladder_send_match_success,
		e_msgindex_ws2ws_cross_ladder_send_game_result,
		e_msgindex_ws2ws_cross_ladder_req_role_info,
		e_msgindex_ws2ws_cross_ladder_send_sign_up,
		e_msgindex_ws2ws_cross_ladder_sync_game_result,
		e_msgindex_ws2dp_cross_ladder_load_info,
		e_msgindex_ws2dp_cross_ladder_add_role_info,
		e_msgindex_ws2dp_cross_ladder_update_info,
		e_msgindex_ws2dp_cross_ladder_update_ticket_info,
		e_msgindex_ws2dp_cross_ladder_update_role_info,
		e_msgindex_ws2dp_cross_ladder_refresh_ticket_info,
		e_msgindex_ws2dp_cross_ladder_refresh_season_info,
		e_msgindex_dp2ws_cross_ladder_load_role_info,
		e_msgindex_ws2ws_cross_ladder_req_create_map,
		e_msgindex_ws2ws_cross_ladder_create_map_failed,
		e_msgindex_cs2ws_cross_ladder_req_last_score,
		e_msgindex_ws2ws_cross_ladder_send_last_score,
		e_msgindex_ws2cs_cross_ladder_send_last_score,
		e_msgindex_ws2ws_cross_ladder_notify_get_season_reward,
	};

	//ws向cs同步可购买次数
	struct ws2cs_cross_ladder_refresh_ticket_info : public hld::packet_base
	{
		guid_64		role_guid;
		int32		can_buy_ticket_num;
		ws2cs_cross_ladder_refresh_ticket_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_cross_ladder_refresh_ticket_info;
		}
		void to_proto(hld::ws2cs_proto::cross_ladder_refresh_ticket_info& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_can_buy_ticket_num(can_buy_ticket_num);

		}
		void from_proto(const hld::ws2cs_proto::cross_ladder_refresh_ticket_info& msg)
		{
			role_guid.server_64 = msg.role_guid();
			can_buy_ticket_num = msg.can_buy_ticket_num();

		}
	};

	//cs向ws请求购买（已经扣款）
	struct cs2ws_cross_ladder_req_buy_ticket : public hld::packet_base
	{
		guid_64		role_guid;
		cs2ws_cross_ladder_req_buy_ticket()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_cross_ladder_req_buy_ticket;
		}
		int32 get_len()
		{
			return sizeof(cs2ws_cross_ladder_req_buy_ticket);
		}
	};

	//cs向ws发送比赛结果
	struct cs2ws_cross_ladder_send_game_result : public hld::packet_base
	{
		guid_64		map_guid;
		guid_64		winner_guid;
		cs2ws_cross_ladder_send_game_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_cross_ladder_send_game_result;
		}
	};

	//比赛服ws向原区ws发送比赛匹配成功
	struct ws2ws_cross_ladder_send_match_success : public hld::packet_base
	{
		int32		server_id;
		guid_64		map_id;
		int32		war_index;
		xchar		opponent_name[max_name_size + 1];
		xchar		opponent_server_name[max_name_size + 1];
		int32		opponent_server_id;
		int32		opponent_template_id;
		int32		opponent_ladder_score;
		int32		opponent_total_join_num;
		int32		opponent_total_win_num;
		int32		opponent_last_win_num;
		s_game_info	target_game_info;
		ws2ws_cross_ladder_send_match_success()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_ladder_send_match_success;
		}
		void set_opponent_name(xstring _name)
		{
			memset(opponent_name, 0, sizeof(opponent_name));
			memcpy(opponent_name, _name.c_str(), _name.size() > sizeof(opponent_name) ? sizeof(opponent_name) : _name.size());
		}
		void set_server_name(xstring _name)
		{
			memset(opponent_server_name, 0, sizeof(opponent_server_name));
			memcpy(opponent_server_name, _name.c_str(), _name.size() > sizeof(opponent_server_name) ? sizeof(opponent_server_name) : _name.size());
		}
	};

	//比赛服务区向gate服ws发送比赛结果
	struct ws2ws_cross_ladder_send_game_result : public hld::packet_base
	{
		bool		draw;
		guid_64		winner_guid;
		guid_64		loser_guid;
		ws2ws_cross_ladder_send_game_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_ladder_send_game_result;
		}
	};

	//原区ws向gate服ws发送请求角色信息
	struct ws2ws_cross_ladder_req_role_info : public hld::packet_base
	{
		xchar		role_name[max_name_size + 1];
		xchar		server_name[max_name_size + 1];
		int32		server_id;
		int32		template_id;
		int64		role_gs;
		int32		vip_level;
		bool		is_online;
		ws2ws_cross_ladder_req_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_ladder_req_role_info;
		}
		void set_role_name(xstring _name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, _name.c_str(), _name.size() > max_name_size ? max_name_size : _name.size());
		}
		void set_server_name(xstring _name)
		{
			memset(server_name, 0, sizeof(server_name));
			memcpy(server_name, _name.c_str(), _name.size() > max_name_size ? max_name_size : _name.size());
		}
	};

	//原区ws向gate服发送报名请求
	struct ws2ws_cross_ladder_sign_up : public hld::packet_base
	{
		bool		is_join;
		int32		server_id;
		xchar		server_ip[max_ip_address_length + 1];
		int32		server_port;
		int32		online_num;
		ws2ws_cross_ladder_sign_up()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_ladder_send_sign_up;
		}
	};

	//gate服向原区发送比赛结果
	struct ws2ws_cross_ladder_sync_game_result : public hld::packet_base
	{
		xchar		role_name[max_name_size + 1];
		int32		ladder_score;
		int32		vip_leve;
		int32		ticket_num;
		bool		is_upgrade;
		ws2ws_cross_ladder_sync_game_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_ladder_sync_game_result;
		}
	};

	//gate服向dp请求数据
	struct ws2dp_cross_ladder_load_info : public hld::packet_base
	{
		ws2dp_cross_ladder_load_info()
		{
			wheader = e_msgindex_ws2dp_cross_ladder_load_info;
		}
	};

	//gate服向dp更新数据（新建数据后)
	struct ws2dp_cross_ladder_add_role_info : public hld::packet_base
	{
		s_cross_ladder_role_info	role_info;
		ws2dp_cross_ladder_add_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_cross_ladder_add_role_info;
		}
	};

	//gate服向dp更新数据（比赛后）
	struct ws2dp_cross_ladder_update_info : public hld::packet_base
	{
		guid_64		role_guid;
		int32		total_win_num;
		int32		last_win_num;
		int32		ladder_score;
		int32		ticket_num;
		int32		history_score;
		int32		cur_season_join_num;
		ws2dp_cross_ladder_update_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_cross_ladder_update_info;
		}
	};
	//gate服向dp更新门票数据（购买后）
	struct ws2dp_cross_ladder_update_ticket_info : public hld::packet_base
	{
		guid_64		role_guid;
		int32		ticket_num;
		int32		can_buy_num;
		ws2dp_cross_ladder_update_ticket_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_cross_ladder_update_ticket_info;
		}
	};

	//gate服向dp刷新role_name
	struct ws2dp_cross_ladder_update_role_info : public hld::packet_base
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		xchar		server_name[max_name_size + 1];
		int64		role_gs;
		int32		vip_level;
		int32		server_id;
		int32		template_id;
		ws2dp_cross_ladder_update_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_cross_ladder_update_role_info;
		}
	};

	//gate服通知dp刷新每日数据
	struct ws2dp_cross_ladder_refresh_ticket_info : public hld::packet_base
	{
		int32		ticket_num;
		int32		can_buy_num;
		ws2dp_cross_ladder_refresh_ticket_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_cross_ladder_refresh_ticket_info;
		}
	};

	struct dp2ws_cross_ladder_load_role_info : public hld::packet_base
	{
		bool		is_load_all;
		int32		cur_data_num;
		s_cross_ladder_role_info	data_array[CROSS_LADDER_MAX_LOAD_DB_NUM];
		dp2ws_cross_ladder_load_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_cross_ladder_load_role_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_array - (ULONG_PTR)&wheader;
			return (basic_len + cur_data_num * sizeof(s_cross_ladder_role_info));
		}
		void clear_data()
		{
			is_load_all = false;
			cur_data_num = 0;
			memset(data_array, 0, sizeof(data_array));
		}
	};

	struct ws2ws_cross_ladder_create_map : public hld::packet_base
	{
		s_cross_ladder_match_info match_info;
		ws2ws_cross_ladder_create_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_ladder_req_create_map;
		}
	};

	struct ws2ws_cross_ladder_create_map_failed : public hld::packet_base
	{
		guid_64		first_role_guid;
		guid_64		second_role_guid;
		ws2ws_cross_ladder_create_map_failed()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_ladder_create_map_failed;
		}
	};

	struct ws2dp_cross_ladder_refresh_season_info : public hld::packet_base
	{
		guid_64		role_guid;
		int32		ladder_score;
		int32		history_score;
		int32		last_score;
		int32		season_reward;
		int32		cur_season_join_num;
		int32		last_season_join_num;
		ws2dp_cross_ladder_refresh_season_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_cross_ladder_refresh_season_info;
		}
	};

	struct cs2ws_cross_ladder_req_last_score : public hld::packet_base
	{
		guid_64		role_guid;
		int32		server_id;
		cs2ws_cross_ladder_req_last_score()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_cross_ladder_req_last_score;
		}
	};

	struct ws2ws_cross_ladder_send_last_score : public hld::packet_base
	{
		guid_64		role_guid;
		int32		last_score;
		int32		last_ranking;
		ws2ws_cross_ladder_send_last_score()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_ladder_send_last_score;
		}
	};

	struct ws2cs_cross_ladder_send_last_score : public hld::packet_base
	{
		guid_64		role_guid;
		int32		last_score;
		int32		last_ranking;
		ws2cs_cross_ladder_send_last_score()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_cross_ladder_send_last_score;
		}
		void to_proto(hld::ws2cs_proto::cross_ladder_send_last_score& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_last_score(last_score);
			msg.set_last_ranking(last_ranking);

		}
		void from_proto(const hld::ws2cs_proto::cross_ladder_send_last_score& msg)
		{
			role_guid.server_64 = msg.role_guid();
			last_score = msg.last_score();
			last_ranking = msg.last_ranking();
		}
	};

	struct ws2ws_cross_ladder_notify_season_reward : public hld::packet_base
	{
		ws2ws_cross_ladder_notify_season_reward()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_ladder_notify_get_season_reward;
		}
	};

#pragma pack(pop)
}
#endif
