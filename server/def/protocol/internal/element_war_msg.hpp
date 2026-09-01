/***********************************************
* created	: 2020年8月18日
* file base : element_war_msg
* file ext  : hpp
* author    : 赵玉明
* purpose   : 元素争霸
************************************************/

#ifndef _ELEMENT_WAR_MSG_HPP_
#define _ELEMENT_WAR_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/element_war_def.hpp"
#include "pk_king_msg.hpp"

namespace faith
{
#pragma pack(push, 1)
	enum
	{
		e_msgindex_ws2dp_element_war_add_role_info = e_msg_base_element_war,	//添加新角色
		e_msgindex_ws2dp_element_war_update_info,								//更新角色比赛信息
		e_msgindex_ws2dp_element_war_refresh_season_info,						//刷新赛季信息
		e_msgindex_ws2ws_element_war_req_create_map,							//创建地图
		e_msgindex_ws2ws_element_war_req_role_info,								//角色服向匹配服请求玩家数据
		e_msgindex_ws2dp_element_war_update_role_info,							//更新角色显示信息
		e_msgindex_ws2dp_element_war_load_info,									//从数据库拉取玩家信息请求
		e_msgindex_dp2ws_element_war_load_role_info_end,						//从数据库拉取数据结果
		e_msgindex_ws2ws_element_war_send_sign_up,								//发送匹配请求
		e_msgindex_ws2ws_element_war_close_sign_up,								//发送取消匹配申请
		e_msgindex_ws2ws_element_war_create_map_failed,							//战斗服地图创建失败结果
		e_msgindex_ws2ws_element_war_send_match_success,						//发送地图创建成功
		e_msgindex_ws2cs_element_war_send_create_robot,							//发送创建机器人消息
		e_msgindex_ws2dp_element_war_refresh_ticket_info,						//刷新每日数据
		e_msgindex_ws2ws_element_war_notify_get_season_reward,					//发送赛季更新提示
		e_msgindex_cs2ws_element_war_map_game_end_role_info,					//发送比赛结果
		e_msgindex_ws2dp_element_war_refresh_mission_info,						//刷新任务信息
		e_msgindex_ws2dp_element_war_clear_mission_info,						//清除任务信息
		e_msgindex_ws2ws_element_war_sync_game_result,							//发送比赛结果到自己服务器
		e_msgindex_ws2ws_element_war_check_can_get_mission_reward,				//角色服向匹配服发送是否可以领取任务奖励
		e_msgindex_ws2ws_element_war_check_can_get_mission_reward_end,			//匹配服回复任务奖励的领取结果
		e_msgindex_ws2cs_element_war_check_can_get_mission_reward_end,			//ws向cs发送领取结果
		e_msgindex_ws2cs_element_war_refresh_ticket_info,						//刷新每日次数
		e_msgindex_ws2cs_element_war_send_last_score,							//发送上赛季排名
		e_msgindex_cs2ws_element_war_buy_ticket,								//通知gate服购买次数
		e_msgindex_cs2ws_element_ladder_req_last_score,							//获取赛季分数
		e_msgindex_ws2ws_element_ladder_send_last_score,						//发送上赛季分数
		e_msgindex_ws2ws_element_war_send_math_tip,								//发送匹配提示
		e_msgindex_cs2ws_element_war_gm_send_add_ticket,						//Gm增加次数
	};
	


	//gate服向dp更新数据（比赛后）
	struct ws2dp_element_war_update_info : public faith::packet_base
	{
		guid_64		role_guid;
		int32		total_win_num;
		int32		last_win_num;
		int32		score_num;
		int32		ticket_num;
		int32		history_score;
		int32		have_buy_num;
		int32		cur_season_join_num;
		int32		join_num;
		ws2dp_element_war_update_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_element_war_update_info;
		}
	};

	struct ws2dp_element_war_refresh_season_info : public faith::packet_base
	{
		guid_64		role_guid;
		int32		score_num;
		int32		history_score;
		int32		last_score;
		int32		last_season;
		int32		season_reward;
		int32		cur_season_join_num;
		int32		last_season_join_num;
		ws2dp_element_war_refresh_season_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_element_war_refresh_season_info;
		}
	};

	//原区ws向gate服ws发送请求角色信息
	struct ws2ws_element_war_req_role_info : public faith::packet_base
	{
		xchar		role_name[max_name_size + 1];
		xchar		server_name[max_name_size + 1];
		int32		server_id;
		int32		template_id;
		int64		role_gs;
		int32		vip_level;
		int32		role_level;
		bool		is_online;
		ws2ws_element_war_req_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_req_role_info;
		}
	};

	//gate服向dp更新数据（新建数据后)
	struct ws2dp_element_war_add_role_info : public faith::packet_base
	{
		s_element_war_role_info	role_info;
		ws2dp_element_war_add_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_element_war_add_role_info;
		}
	};

	//gate服向dp刷新role_name
	struct ws2dp_element_war_update_role_info : public faith::packet_base
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		xchar		server_name[max_name_size + 1];
		int64		role_gs;
		int32		vip_level;
		int32		role_level;
		int32		server_id;
		int32		template_id;
		int32		can_buy_num;
		ws2dp_element_war_update_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_element_war_update_role_info;
		}
	};

	//gate服向dp请求数据
	struct ws2dp_element_war_load_info : public faith::packet_base
	{
		ws2dp_element_war_load_info()
		{
			wheader = e_msgindex_ws2dp_element_war_load_info;
		}
	};

	struct dp2ws_element_war_load_role_info : public faith::packet_base
	{
		bool		is_load_all;
		int32		cur_data_num;
		s_element_war_role_info	data_array[ELEMENT_WAR_MAX_LOAD_DB_NUM];
		dp2ws_element_war_load_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_element_war_load_role_info_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_array - (ULONG_PTR)&wheader;
			return (basic_len + cur_data_num * sizeof(s_element_war_role_info));
		}
		void clear_data()
		{
			is_load_all = false;
			cur_data_num = 0;
			memset(data_array, 0, sizeof(data_array));
		}
	};

	//原区ws向gate服发送报名请求
	struct ws2ws_element_war_sign_up : public faith::packet_base
	{
		guid_64		role_guid_list[ELEMENT_WAR_PLAY_NUM];
		guid_64		team_guid;
		int32		server_id;
		xchar		server_ip[max_ip_address_length + 1];
		int32		server_port;
		int32		online_num;
		ws2ws_element_war_sign_up()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_send_sign_up;
		}
	};

	//原区ws向gate服发送取消报名请求
	struct ws2ws_element_war_close_sign_up : public faith::packet_base
	{
		guid_64		role_guid;
		int32		server_id;
		ws2ws_element_war_close_sign_up()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_close_sign_up;
		}
	};
	//地图创建失败重置角色信息
	struct ws2ws_element_war_create_map_failed : public faith::packet_base
	{
		guid_64		first_role_guid[ELEMENT_WAR_PLAY_NUM];
		guid_64		second_role_guid[ELEMENT_WAR_PLAY_NUM];
		ws2ws_element_war_create_map_failed()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_create_map_failed;
		}
	};

	struct ws2ws_element_war_create_map : public faith::packet_base
	{
		s_element_war_match_info match_info;
		ws2ws_element_war_create_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_req_create_map;
		}
	};

	//比赛服ws向原区ws发送比赛匹配成功
	struct ws2ws_element_war_send_match_success : public faith::packet_base
	{
		int32		server_id;
		guid_64		map_id;
		int32		war_index;
		s_element_war_match_success_info match_info;
		ws2ws_element_war_send_match_success()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_send_match_success;
		}
	};

	//比赛服ws向cs发送创建机器人消息
	struct ws2cs_element_war_send_create_robot : public faith::packet_base
	{
		guid_64		map_guid;
		s_element_war_match_info match_info;
		ws2cs_element_war_send_create_robot()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_element_war_send_create_robot;
		}
		bool to_proto(faith::ws2cs_proto::element_war_send_create_robot& msg)
		{
			msg.set_map_guid(map_guid.server_64);
			faith::st_proto::st_element_war_match_info * st_match_ptr = msg.mutable_match_info();
			if (st_match_ptr == nullptr)
			{
				return false;
			}
			match_info.to_proto(st_match_ptr);
			return true;
		}
		void from_proto(const faith::ws2cs_proto::element_war_send_create_robot& msg)
		{

		}
	};

	//gate服通知dp刷新每日数据
	struct ws2dp_element_war_refresh_ticket_info : public faith::packet_base
	{
		int32		ticket_num;
		int32		have_buy_num;
		ws2dp_element_war_refresh_ticket_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_element_war_refresh_ticket_info;
		}
	};

	//ws服通知cs刷新每日数据
	struct ws2cs_element_war_refresh_ticket_info : public faith::packet_base
	{
		guid_64		role_guid;
		int32		can_buy_ticket_num;
		ws2cs_element_war_refresh_ticket_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_element_war_refresh_ticket_info;
		}
	};

	//发送上赛季排名
	struct ws2cs_element_war_send_last_score : public faith::packet_base
	{
		guid_64		role_guid;
		int32		last_score;
		int32		last_ranking;
		ws2cs_element_war_send_last_score()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_element_war_send_last_score;
		}
		void to_proto(faith::ws2cs_proto::element_war_send_last_score& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_last_score(last_score);
			msg.set_last_ranking(last_ranking);

		}
		void from_proto(const faith::ws2cs_proto::element_war_send_last_score& msg)
		{
			role_guid.server_64 = msg.role_guid();
			last_score = msg.last_score();
			last_ranking = msg.last_ranking();
		}
	};

	//发送赛季更新
	struct ws2ws_element_war_notify_season_reward : public faith::packet_base
	{
		ws2ws_element_war_notify_season_reward()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_notify_get_season_reward;
		}
	};

	struct cs2ws_element_war_map_game_end_role_info : public faith::packet_base
	{
		guid_64		role_guid;
		int32		add_score;
		int32		score_rank;
		bool		is_win;
		bool		is_escape;
		int32		kill_player;
		int32		kill_monster;
		int32		kill_elite;
		int32		kill_boss;
		cs2ws_element_war_map_game_end_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_element_war_map_game_end_role_info;
		}

	};

	struct ws2dp_element_war_clear_mission_info : public faith::packet_base
	{
		ws2dp_element_war_clear_mission_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_element_war_clear_mission_info;
		}

	};

	struct ws2dp_element_war_refresh_mission_info : public faith::packet_base
	{
		guid_64 role_guid;
		int32 mission1;
		int32 mission2;
		int32 mission3;
		int32 mission4;
		int32 mission1_have_num;
		int32 mission2_have_num;
		int32 mission3_have_num;
		int32 mission4_have_num;
		ws2dp_element_war_refresh_mission_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_element_war_refresh_mission_info;
		}

	};

	struct ws2ws_element_war_sync_game_result : public faith::packet_base
	{
		guid_64 role_guid;
		xchar role_name[max_name_size + 1];
		int32 new_score;
		int32 hight_score;
		int32 server_id;
		int32 vip_level;
		ws2ws_element_war_sync_game_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_sync_game_result;
		}

	};

	struct ws2ws_element_war_check_can_get_mission_reward : public faith::packet_base
	{
		guid_64 role_guid;
		int32 mission_index;
		ws2ws_element_war_check_can_get_mission_reward()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_check_can_get_mission_reward;
		}
	};

	struct ws2ws_element_war_check_can_get_mission_reward_end : public faith::packet_base
	{
		guid_64 role_guid;
		int32 ret_index;
		int32 mission_index;
		int32 mission_id;
		ws2ws_element_war_check_can_get_mission_reward_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_check_can_get_mission_reward_end;
		}
	};

	struct ws2cs_element_war_check_can_get_mission_reward_end : public faith::packet_base
	{
		guid_64 role_guid;
		int32 ret_index;
		int32 mission_index;
		int32 mission_id;
		ws2cs_element_war_check_can_get_mission_reward_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_element_war_check_can_get_mission_reward_end;
		}
		void to_proto(faith::ws2cs_proto::element_war_check_can_get_mission_reward_end& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_ret_index(ret_index);
			msg.set_mission_index(mission_index);
			msg.set_mission_id(mission_id);

		}
		void from_proto(const faith::ws2cs_proto::element_war_check_can_get_mission_reward_end& msg)
		{
			role_guid.server_64 = msg.role_guid();
			ret_index = msg.ret_index();
			mission_index = msg.mission_index();
			mission_id = msg.mission_id();
		}
	};

	//扣除货币成功通知gate服增加次数
	struct cs2ws_element_war_buy_ticket : public faith::packet_base
	{
		guid_64 role_guid;
		cs2ws_element_war_buy_ticket()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_element_war_buy_ticket;
		}
	};
	//前期上赛季分数
	struct cs2ws_element_ladder_req_last_score : public faith::packet_base
	{
		guid_64		role_guid;
		int32		server_id;
		cs2ws_element_ladder_req_last_score()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_element_ladder_req_last_score;
		}
	};
	//发送上赛季分数
	struct ws2ws_element_ladder_send_last_score : public faith::packet_base
	{
		guid_64		role_guid;
		int32		last_score;
		int32		last_ranking;
		ws2ws_element_ladder_send_last_score()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_ladder_send_last_score;
		}
	};

	//发送匹配提示Tip
	struct ws2ws_element_war_send_math_tip : public faith::packet_base
	{
		guid_64		role_guid;
		ws2ws_element_war_send_math_tip()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_element_war_send_math_tip;
		}
	};
	
	struct cs2ws_element_war_gm_send_add_ticket : public faith::packet_base
	{
		guid_64 role_guid;
		cs2ws_element_war_gm_send_add_ticket()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_element_war_gm_send_add_ticket;
		}
	};
#pragma pack(pop)
}
#endif