/********************************************************************
created: 2019年5月24日
file base: cross_server_world_boss_ws_mgr
file ext: hpp
author: zhaoyuming
purpose:
*********************************************************************/

#ifndef _CROSS_SERVER_WORLD_BOSS_WS_MGR_WS_H_
#define _CROSS_SERVER_WORLD_BOSS_WS_MGR_WS_H_

#include "server/client_session_mgr.hpp"
#include <Logic/cross_server_world_boss_def.hpp>
#include <cross_server_world_boss_msg.hpp>
#include "utility/score_indicator.h"


namespace hld
{
	class cross_server_world_boss_ws_mgr
	{
	public:
		static cross_server_world_boss_ws_mgr& get_instance()
		{
			static cross_server_world_boss_ws_mgr instance;
			return instance;
		}
		explicit cross_server_world_boss_ws_mgr();
		//活动初始化
		void						init_cross_server_world_boss_map();
		void						clear_data();
		void						init_manager();
		//时间处理
		void						tick(int64 time_new);
		bool						is_in_game_time();								//是否在跨服世界boss活动时间
		bool						is_in_game_ready_time();						//是否在活动的准备时间
		bool						is_in_game_all_time();
		bool						is_have_world_boss_map();						//跨服世界boss活动地图是否存在
		bool						is_in_cross_boss_map(guid_64 map_guid);
		//地图传送
		e_error_code					is_can_join_with_cross(guid_64 role_guid, int32 map_type);
		e_error_code					is_can_join_with_cross_gate(int32 map_type);
		void						recv_other_ws_transger_gate_map(guid_64 role_guid, int32 server_id, int32 map_type);
		void						transfer_to_map_with_gate_msg(guid_64 role_guid, int32 error_ret, int32 map_temp_id, guid_64 map_guid, int32 war_idex, int32 server_id);
		//数据库相关		
		void						send_get_boss_show_info_all();	//向服务器发送获得所有boss信息
		void						save_boss_info_to_dp(int32 boss_template_id);
		void						set_kill_boss_show_info_all(const cross_boss_info * boss_info, int32 data_num);
		void						set_kill_boss_show_info(const cross_boss_info& boss_info);
		void						send_boss_dead_info_all_to_all_server(int32 server_id);
		//通用函数
		void						get_world_boss_map_guid(guid_64& map_guid, int32& war_idex, int32 map_type);			//获得跨服世界boss活动地图id
		int32						get_cross_server_world_boss_map_temp_id(int32 map_type);								//获得跨服世界boss活动地图模板id
		void						set_boss_level();	//设置boss等级
		void						locd_boss_show_info(const cross_world_boss_info_to_db * dp_info, int32 data_num);
		//发送消息到cs
		void						sync_create_boos_to_cs(guid_64 from_map_guid);
		//跨服消息的处理
		int32						get_true_boss_level();	//获得boss等级
		int32						get_false_boss_level();	//获得boss等级
		void						get_with_save_server_level();//获取服务器组的平均等级
		void						send_server_level(int32 server_id);//发送服务器等级
		void						server_level_func(int32 true_server_level, int32 false_server_level, int32 server_id);//处理服务器等级
		//军团奖励和公告相关
		void						make_cross_world_boss_legion_award(guid_64 legion_guid, int32 boss_id, int32 rank_num, guid_64* play_guid_list, int32 play_num); //发放军团奖励
		void						send_legion_act_rank(const cs2ws_make_cross_server_world_legion_award& act_rank);//向跨服组里的所有服务器发送军团伤害列表
		void						make_kill_boss_legion_awaed(guid_64 kill_boss_legion_guid, int32 kill_boss_tmp_id, guid_64* play_guid_list, int32 play_num);
		void						send_boss_dead_notice(int32 boss_template, int32 notice_id, int32 item_num, s_item_info * data_ary, std::string notice_string);
		//游戏结束
		void						cross_boss_map_game_over(int32 map_template);
		//接收客户端消息的处理	
		void						send_kill_boss_show(client_session* session);
		//接收cs消息的处理
		void						cross_boss_on_dead(guid_64 kill_boss_play_guid, int32 be_kill_boss_id);
		//发送消息到客户端
		void						sync_kill_boss_to_all_map_inst(int32 map_template_id, int32 killer_unit_index, int32 be_kill_boss_guid, guid_64 killer_guid);
		void						send_message_to_all_cs(const void* data_ptr, size_t data_len);//广播消息	
		void						send_cross_boss_ranking_list(int32 boss_id);	
		void						set_cross_boss_ranking_list(int32 boss_template_id, s_ranking_player_info* ranking_info, int32 data_num);
		void						send_boss_dead_info_to_all_server(int32 boss_temp_id ,int32 server_id);
		void						send_cross_boss_info_all_to_other_ws(int32 server_id);
		void						change_legion_name(guid_64 legion_guid, const xstring& legion_name);
		int32						get_world_boss_id_with_type(int32 idex);
		
		bool						is_have_live_boss();
		void						send_legion_welfare_rank();
	private:

		bool							m_game_over_flag;							//活动是否结束
		bool							m_is_cur_map_end[map_max];					//用于记录地图是否存在0
		bool							m_ready_notice;								//是否发放过活动准备公告
		bool							m_game_notice;								//是否发放过活动开始公告
		bool							m_is_need_clear;							//是否需要清理资源
		bool							m_is_send_legion_welfare;					//是否发放军团福利
		int32							m_boss_level_true;							//真boss等级
		int32							m_boss_level_false;							//假boss等级
		int32							m_boss_id[m_boss_max_num];					//用于记录活动boss模板id
		guid_64							m_cross_world_boss_map_id[map_max];			//用于记录跨服世界boss活动地图id
		int64							m_boss_all_damage[m_boss_max_num];			//boss受到的总伤害
		score_indicator					m_boss_damage_list[m_boss_max_num];			//boss伤害列表
		score_indicator					m_all_boss_damage_list;						//所有boss总伤害列表
		e_map_state						m_map_state[map_max];						//活动地图状态
		cross_boss_info					m_boss_kill_all_info[m_boss_max_num];
		cross_boss_level				m_boss_level[max_server_num];	
	};


}

#endif