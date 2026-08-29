/********************************************************************
  created: 2014/07/30
  created: 30:7:2014 22:11
  file base: npc_def
  file ext: hpp
  author: Locke
  
  purpose: 
*********************************************************************/
#ifndef _NPC_DEF_HPP_
#define _NPC_DEF_HPP_

#include "logic/type_def.hpp"
#include "Logic/att_def.hpp"

namespace faith
{
	enum e_boss_state
	{
		e_boss_state_none,      // BOSS没出现
		e_boss_state_appear,    // BOSS出现
		e_boss_state_killed,    // BOSS被杀死
		e_boss_state_disappear, // BOSS消失
		e_boss_state_max
	};

	enum e_boss_type
	{
		e_boss_type_world,			// 世界BOSS
		e_boss_type_cross_world,	// 跨服世界BOSS
		e_boss_type_legion,			// 军团BOSS
		e_boss_type_attack_city,	// 攻城战boss
		e_boss_type_max
	};

	enum e_protect_npc_state
	{
		e_protect_npc_state_none,       // 被保护的NPC没出现
		e_protect_npc_state_appear,     // 被保护的NPC出现
		e_protect_npc_state_killed,     // 被保护的NPC被杀掉
		e_protect_npc_state_disappear,  // 被保护的NPC消失
		e_protect_npc_state_max
	};

	enum e_npc_respawn_type
	{
		e_npc_respawn_type_non,					//空
		e_npc_respawn_type_duration_time,		//按CD重生
		e_npc_respawn_type_target_time,			//时间点重生
		e_npc_respawn_type_in_certain_time,		//特定时间段重生
	};

	enum e_kill_npc_exp_mode
	{
		e_kill_npc_exp_mode_npc_lv,
		e_kill_npc_exp_mode_player_lv,
	};
	
	enum e_init_npc_level_type
	{
		e_npc_level_type_none,
		e_npc_level_type_player_lv,//创建地图时的玩家等级
		e_npc_level_type_server_lv,//服务器动态等级
		e_npc_level_type_legion_average_lv,//军团动态等级
		e_npc_level_type_cross_server_lv,//跨服动态等级
		e_npc_level_type_cur_player_lv,//地图内玩家实时平均等级
	};

	const int32 notice_id_boss = 93000026;				// boss和黄金战队的noticeid
	const int32 notice_id_boss_add_legion = 93000199;	// boss和黄金战队带军团名字的noticeId
	const int32 notice_id_curse = 93000028;				// 诅咒神殿的noticeid
	const int32 notice_id_broken_sky = 93000053;		// 破碎虚空
	const int32 notice_id_legion_boss = 93000056;		// 军团boss
	const int32 notice_id_gain_treasure_boss_set_top = 93000240;	//打宝boss置顶
	const int32 notice_id_gain_treasure_boss_record = 93000241;		//打宝boss正常
	const int32 first_boss_spawn_id = 82010001;			//第一个世界bossID
	const int32 first_daily_boss_spawn_id = 82010001;	//第一个世界精英ID
	const int32 last_boss_spawn_id = 82010012;			//最后一个世界bossID
	const int32 first_elite_ready_notice_id = 93000205; //第一个世界精英准备公告的ID
	const int32 first_boss_free_notice_id = 93000215;	//第一个无聊的boss公告ID
	const int32 notice_id_boss_island_record = 93000263;//永恒岛公告ID
	const int32 notice_id_cross_server_kill_player = 93000266;//跨服杀人公告
	const int32 notice_id_cross_boss_award_id = 93000278;//跨服世界击杀boss公告有军团
	const int32 notice_id_cross_boss_award_id_not_legion = 93000291;//跨服世界击杀boss公告没有军团
	const int32 high_level_broken_sky_spawn_id = 82039003;//高等级破碎虚空bossid

	const int32 first_boss_ready_npc_id = 82013000;	//第一个BOSS准备时刷新的NPCID
	const int32 end_boss_ready_npc_id = 82013006;	//最后一个BOSS准备室刷新的NPCID
	const int32 boss_rage_buff_id = 10010255;//Boss到时间以后狂暴buffID

	const int32 area_npc_start_id = 76980200;
	const int32 area_npc_end_id = 76980239;
	const int32 area_db_npc_id = 76980009;
}

#endif
