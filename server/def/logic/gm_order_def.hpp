
/********************************************************************
created: 2016/07/29
file base: gm_order_def
file ext: hpp
author: 张宇翔

purpose:
*********************************************************************/
#ifndef _GM_ORDER_DEF_HPP_
#define _GM_ORDER_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"

namespace faith
{
#pragma pack(push,1)
	const int32  GM_DAMAGE_BUFF_ID = 10010013;
	const int32  GM_HEAL_BUFF_ID = 10010001;
	const int32  GM_ORDER_PARAMETER_MAX_SIZE_ON_WS = 10;
	enum e_gm_order_type
	{
		egot_role_level,//人物等级,转生等级
		egot_role_vip_point,//充值点数
		egot_role_career,//人物职业
		egot_role_info,//显示人物信息
		egot_skill_level,//技能等级
		egot_heal,//回满
		egot_god,//无敌
		egot_no_god,//无敌失效
		egot_add_money,//加钱
		egot_reset_cd,//重置技能冷却
		egot_create_item,//创建物品
		egot_create_mail,//发邮件
		egot_transport,//传送指定位置
		egot_set_damage,//造成伤害
		egot_fuben_win,//副本胜利
		egot_create_npc,//创建单位
		egot_clean_wave,//清楚波次
		egot_mission_main,//主线任务
		egot_mission_daily,//日常任务
		egot_mission_taofa,//讨伐任务
		egot_buff,//buff
		egot_activate_all,//解锁所有已有的翅膀、坐骑、技能
		egot_activate_new_stage,//解锁初阶翅膀、坐骑、和全部技能
		egot_dump_npc_info, //打印指定id的npc信息
		egot_dump_aoi_info, //打印玩家附件的aoi信息
		egot_unlock_all_raid, // 解锁全部副本地图
		egot_clear_enter_count, // 清零全部地图的进入次数
		egot_activate_skill,//解锁技能
		egot_add_legion_coin,//增加军团资源
		egot_load_config,//读取表格配置
		egot_clean_bag,//清理背包
		egot_reset_highest_record,//重置最大记录
		egot_control_robot_chat,//关闭或打开机器人聊天
		egot_set_self_hp_point,//设置自己血量
		egot_clear_arena,//重置竞技场次数
		egot_open_mobile_gm_web_require,//客户端请求打开GM指令网页
		egot_open_mobile_gm_web_response,//Server回传客户端打开GM指令网页
		egot_close_speed,//关闭速度限制
		egot_grade_up,   //转职
		egot_send_mail,	//发邮件
		egot_load_svr_lua,
		egot_set_recycle,// 回归系统设置
		egot_max,
	};
#pragma pack(pop)
}
#endif