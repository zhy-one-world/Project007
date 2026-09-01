/********************************************************************
  created: 2017年3月3日11:32:50
  file base: time_def
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_TIME_DEF_HPP_
#define _FAITH_TIME_DEF_HPP_

#include "logic/type_def.hpp"
#include "time.hpp"

/************************************************************************/
/*        所有服务器刷新时间间隔(毫秒)								    */
/************************************************************************/

namespace faith 
{
#pragma pack(push,1)
	const int32 day_tick_time = 86400000;//一天的毫秒数
	const int32 day_time_second = 86400;//一天的秒数
	const int32 hour_tick_time = 3600000;//一小时的毫秒数
	const int32 half_hour_tick_time = 1800000;//半小时的毫秒数
	const int32 minute_tick_time = 60000;//一分钟的毫秒数
	const int32 thrity_second_tick_time = 30000;//30秒钟的毫秒数
	const int32 second_tick_time = 1000;//一秒钟的毫秒数
	const int32 half_second_tick_time = 500;//半秒钟的毫秒数
	const int32 deci_second_tick_time = 100;//十分之一秒钟的毫秒数
	const int32 interval_retry_connect2dp = 10000;//服务器重连dp间隔时间
	const int32 dp_interval_serverloop = 200;//dp内循环一次的时间间隔
	const int32 server_console_time = 60 * second_tick_time;//控制台输出一次的时间间隔
	const int32 server_player_to_ws = 15 * second_tick_time;//每15s同步一次cs玩家数量到ws
	const int32 server_log_tick_time = 10 * second_tick_time;//日志刷新间隔
	const int32 map_record_tick_time = hour_tick_time;//地图信息刷新间隔
	const int32 sync_pos_walk_tick_time = 300;//服务器地面移动同步位置时间间隔
	const int32 sync_pos_fly_tick_time = 1 * second_tick_time;//npc,小怪位置同步
	const int32 sync_pos_simple_update_time = 5 * second_tick_time;//简模玩家位置同步
	const int32 globel_mail_check_time = 30;// 全局邮件定时检查时间
	const int32 welfare_send_time = 600 * second_tick_time;//福利同步消息的时间间隔
	const int32 meditation_begin_time = 30 * second_tick_time;//冥想开启的时间间隔
	//const int32 meditation_time_max = 43200 * second_tick_time;// 冥想最大时间
	const int32 pk_value_cut_time = minute_tick_time;// pk每隔1分钟减少一次
	const int32 ai_nomal_tick_time = 3 * second_tick_time;// ai平常状态3秒钟一次
	const int32 ai_move_tick_time = 1 * second_tick_time;// ai随机移动提高到1秒钟一次
	const int32 ai_attack_tick_time = 0.5 * second_tick_time;// ai被攻击之后提高到0.5秒钟一次
	const int32 ai_next_attack_tick_time = 0.5 * second_tick_time;// AI攻击间隔
	const int32 client_session_heart_time = 120 * second_tick_time;// 客户端心跳时长
	const int32 client_session_login_time = 600 * second_tick_time;// 客户端在登陆界面时长
	const int32 client_session_logout_time = 180 * second_tick_time;// 客户端掉线后保留多久的数据
	const int32	recalcu_legion_fighting_power_interval = 120 * second_tick_time;	// 重新计算并推送军团总战斗力的事件间隔
	const int32 legion_apply_out_of_data_time = 172800 * second_tick_time;			// 一个入团申请的到期时间(目前是48个小时)
	const int32 auction_check_time = 60 * second_tick_time;//拍卖行检测时间
	const int32 back_hp_magic_time = 10 * second_tick_time;//30s tick
	const int32 all_day_in_week = 7;//一周的天数
	const int32 gate_fet_state = 5 * second_tick_time;//gate上fep的状态同步
	const int32 legion_answer_interval_time = 60 * second_tick_time;			//军团答题间隔时间
	const int32 legion_answer_fisrt_notice_time = 60;							//军团答题第一次准备公告时间
	const int32 legion_answer_second_notice_time = 30;							//军团答题第二次准备公告时间
	const int32 legion_answer_third_notice_time = 10;							//军团答题第三次准备公告时间
	const uint32 chat_interval_time = 20 * second_tick_time;					//聊天间隔
	const uint32 chat_same_str_time = 60 * second_tick_time;					//相同字符聊天间隔

	const int32 send_server_info_to_daemon_interval = 2000;						//向daemon同步服务器状态间隔时间

	const int32 sec_five_hour_tick_time = 18000;//0点到5点的秒数
	const int32 client_fdate_sec_ticks = 10000000;//客户端刷新时间
	const int32 auction_time_out_interval = 60 * second_tick_time;//拍卖行检测时间
	const int32 legion_city_war_bid_time_begin = 5;//城战竞标开始时间

	const int32 reload_csv_constraint_time = 300 * second_tick_time;//强制全服执行reloadcsv时间
#pragma pack(pop)
}

#endif
