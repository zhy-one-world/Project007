#ifndef _PK_KING_MGR_WS_H_
#define _PK_KING_MGR_WS_H_

#include "server/client_session_mgr.hpp"
#include <Logic/pk_king_def.hpp>
#include <pk_king_msg.hpp>

namespace hld
{
	class pk_king_mgr
	{
	public:
		static pk_king_mgr& get_instance()
		{
			static pk_king_mgr instance;
			return instance;
		}
	public:
		void						tick(int64 time_new);
		void						clear_data();
		//清除排行榜数据
		void						clear_rank_list_data();
		//增加排行榜数据
		void						add_rank_list_data(guid_64 role_guid);
		//创建交战地图
		void						create_pk_king_combat_map(guid_64 from_map_guid, int32 map_num);
		//判断是否可以进入
		e_error_code					is_can_join(guid_64 role_guid);
		//判断是否在活动时间内
		bool						is_in_game_time();
		//判断是否在准备时间内
		bool						is_in_ready_time();
		//判断是否在公告时间内
		bool						is_in_notice_time();
		//初始化地图
		void						init_pkking_map();
		//获得待机地图
		guid_64						get_pk_king_map_guid() { return m_pk_king_map_guid; }
		//设置待机地图
		void						set_pk_king_map_guid(guid_64 map_guid) { m_pk_king_map_guid = map_guid; }
		//判断地图人数
		void						get_valid_pkking_map(int32& map_war_idx, guid_64& map_guid);
		//发送当前地图人数
		void						send_player_num_in_map_to_client(guid_64 role_guid);
		//传送进入地图
		void						recv_other_ws_transfer_gate_map(guid_64 role_guid, int32 server_id);
		
		//记录离线玩家
		void						add_break_line_player(s_break_line_login_info break_info);
		//检查离线玩家
		void						check_break_line_player();
		//发送断线重连消息			
		void						send_break_line_msg(guid_64 role_guid, int32 map_template_id);
		//进行重连
		void						break_line_login_msg(guid_64 role_guid, bool is_login);
		//发送重连消息
		void						send_to_break_login_transfer_map(guid_64 role_guid, guid_64 map_guid, int32 activity_type, int32 server_id = 0);
		//检查是否可以重连
		void						check_can_break_login_map(guid_64 role_guid, guid_64 map_guid,int32 server_id);
		//检查重连地图是否可进
		e_error_code					check_login_map_can_join(guid_64 role_guid, guid_64 map_guid);
		//重连入地图
		void						transfer_break_login_map(guid_64 role_guid, guid_64 map_guid, int32 error_ret, int32 map_template_id, int32 war_idex, int32 server_id);
		//是否是pk之王
		bool						player_is_pk_king(guid_64 role_guid);
		//获取重连地图信息
		s_break_line_login_info		get_player_break_login_info(guid_64 role_guid);
		//Old
		guid_64						get_pk_king_player() { return m_pk_king_player; }
		void						set_pk_king_player(guid_64 role_guid);


	private:
		explicit pk_king_mgr();

		guid_64						m_pk_king_player;

		int64						m_last_tick_time;
		int64						m_timer;

		bool						m_is_clear_rank;

		guid_64						m_pk_king_map_guid;
		std::vector<s_break_line_login_info>		m_break_line_login_list;
	};
}

#endif
