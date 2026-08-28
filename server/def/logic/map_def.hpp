/********************************************************************
  created: 2015年8月24日20:54:53
  file base: map_def
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _MAP_DEF_HPP_
#define _MAP_DEF_HPP_

#include "char_def.hpp"
#include "utility/cs_date.hpp"
#include "game_enum_def_s.h"

namespace hld 
{
#pragma pack(push,1)

	const int32 max_map_record_num = 300;
	const int32 big_world_map_id = 71050001;				// 世界地图id
	const int32 new_map_lock_id = 15001517;					//进入新世界的解锁任务id
	const int32 first_demons_tower_template_id = 71031001;	// 第一层万魔塔的模板ID
	const int32 first_battle_map_template_id = 71040001;	// 第一层战斗副本的模板ID
	const int32 wave_npc_num_max = 100;
	const int32 pk_king_map_template_id = 71140001;			// PK之王的待机地图模板ID
	const int32 pk_king_combat_map_template_id = 71140002;	// PK之王的战斗地图模板ID
	const int32 shui_jing_map_tempplate_id = 71160001;		// 水晶幻境的地图模板ID
	const int32 shui_jing_map_cross_temp_id = 71160000;		// 跨服水晶幻境地图模板ID
	const int32 broken_sky_map_template_id = 71130001;		// 破碎虚空的地图模板ID
	const int32 legion_bonfire_map_template_id = 71160004;	// 军团篝火的地图模板ID
	const int32 legion_city_war_map_temp_id = 71080001;		// 军团城战ID
	const int32	cross_server_world_boss_temp_id = 71080002; //跨服世界模板id
	const int32 max_extra_enter_map_count = 10;				// 能够通过花费特殊钱物的方式来进入某个地图(副本)的次数
	const int32 big_map_line_num = 100;
	const int32 battle_map_line_num = 4000;
	const int32 role_pk_line_num = 2000;
	const int32 city_war_line_num = 1;
	const int32 map_in_null_time = 3;						//地图处于无人状态时多长时间会删除(秒)
	const int32 hold_map_in_null_time = 3600;				//hold地图处于无人状态时多长时间会删除(秒)
	const int32 map_game_over_time = 60;					//游戏结束时玩家在场景内停留剩余时间(秒)
	const int32 temple_join_num = 5;						//神庙最多加入多少人

	const int32 first_temple_template_id = 71110001;			// 诅咒神殿一段的ID
	const int32 first_empire_treasure_template_id = 71120001;	// 帝国遗藏一段的ID
	const int32 first_broken_sky_template_id = 71130001;		// 破碎虚空一段的ID

	const int32 multiplayer_baohugongzhu_reach_distance = 150;		//判断到达公主坐标距离

	const int32 map_info_arr_max_num = 5;						//单个地图信息数组最大值

	const int32 map_load_npc_interval_max = 10;
	const int32 map_load_npc_interval_middle = 8;
	const int32 map_load_npc_interval_min = 6;
	//const int32 begin_join_time	= 15 * 3600 + 00 * 60 + 00;	// 破碎虚空的开始入场时间
	//const int32 end_join_time	= 15 * 3600 + 02 * 60 + 00;	// 破碎虚空的结束入场时间
	//const int32 begin_time		= 15 * 3600 + 02 * 60 + 00;	// 破碎虚空的开始时间
	//const int32 end_time			= 15 * 3600 + 15 * 60 + 00;	// 破碎虚空的结束时间
	//const int32 creat_boss_level_ranking_begion = 1;
	//const int32 creat_boss_level_ranking_end = 50;
	const int32 sync_damage_rank_count = 50;

	const int32 boss_island_temp_id = 71200001;	//永恒岛ID

	const int32 map_lind_max_num = 2;//有几个线

	const int32	doublie_line_max_num = 10;	//双倍经验最大地图数量

	//const int32 EmpireTreasureBeginJoinTime = 0;//可以加入的时间
	//const int32 EmpireTreasureEndJoinTime = 2;//结束加入的时间
	//const int32 EmpireTreasureBeginGameTime = 2;//开始游戏的时间
	//const int32 EmpireTreasureEndGameTime = 12;//结束游戏的时间
	//const int32 EmpireTreasureDiffTime = 30;//下一次的时间间隔
	//const int32 EmpireTreasureRefreshTime = 2;//需刷新怪物死后刷新间隔时间

	//const int32 TempleBeginJoinTime = 15;//可以加入的时间
	//const int32 TempleEndJoinTime = 17;//结束加入的时间
	//const int32 TempleBeginGameTime = 17;//开始游戏的时间
	//const int32 TempleEndGameTime = 25;//结束游戏的时间
	//const int32 TempleDiffTime = 30;//下一次的时间间隔

	//const int32 CrystalIllusionOpenTime0[4]{ 12,0,13,0 };
	//const int32 CrystalIllusionOpenTime1[4]{ 22,0,23,0 };

	const int32 appearance_map_id = 71220301;			// 捏脸地图Id

	const int32 assist_fight_npc = 76991158;			//助战引导Npc


	const int32 box_map_npc_count = 20;			//开箱子地图中Npc最大数量

	
	enum e_map_info
	{
		e_map_info_player_num,//当前地图的玩家人数
		e_map_info_boss_cur_hp,//当前地图BOSS的当前血量
		e_map_info_boss_max_hp,//当前地图BOSS的最大血量
	};

	enum e_map_open
	{
		e_map_open_new,//每次进入重新开启
		e_map_open_old,//每次进入先查找是否有旧的
	};

	enum e_map_close
	{
		e_map_close_close,//人都离开的时候要删除
		e_map_close_hold,//人离开后要保留
	};

	// 副本中的计时模式
	enum e_map_time_mode
	{
		e_map_time_mode_none,             // 无(比如大地图)
		e_map_time_mode_timeout_success,  // 到时后胜利
		e_map_time_mode_timeout_fail,     // 到时后失败
		e_map_time_mode_timeout_transfer, // 到时后传送
		e_map_time_mode_max
	};

	enum e_map_war_result
	{
		e_map_war_result_lost,		//失败
		e_map_war_result_peace,		//平局
		e_map_war_result_win,		//胜利
	};

	enum e_mopping_up_result
	{
		e_mopping_up_result_none,            // 无
		e_mopping_up_result_finish,          // 完成扫荡
		e_mopping_up_result_no_enter_count,  // 没有剩余扫荡次数了
		e_mopping_up_result_max
	};

	enum e_consume_enter_count_cond
	{
		e_consume_enter_count_cond_none,
		e_consume_enter_count_cond_enter,
		e_consume_enter_count_cond_win
	};

	// 副本的胜利条件
	enum e_map_win_cond
	{
		e_map_win_cond_none,              // 无限模式
		e_map_win_cond_kill_boss,         // 打死BOSS即为胜利
		e_map_win_cond_clear_all_wave,    // 清理所有波的怪物即为胜利
		e_map_win_cond_time_out,          // 到时即为胜利
		e_map_win_cond_max
	};

	// 副本结算评级
	enum e_map_settle_type
	{
		e_map_settle_type_finish_time,		// 通关时间
		e_map_settle_type_kill_boss_num,	// 击败BOSS数
		e_map_settle_type_protect_npc_hp,	// 保护NPC剩余血量
		e_map_settle_type_max
	};

	enum e_map_result_dialog_type
	{
		e_map_result_dialog_type_none,							// 无弹出(例如金币副本)
		e_map_result_dialog_type_lucky_draw,					// 抽奖结算界面(如剧情副本)
		e_map_result_dialog_type_lucky_draw_and_fixed_award,	// 抽奖+固定奖励结算界面(如多人副本)
		e_map_result_dialog_type_demons_tower,					// 万魔塔结算界面
		e_map_result_dialog_type_legion_activity,				// 军团活动结算界面
		e_map_result_dialog_type_score_and_rank_award,			// PK之王结算界面
		e_map_result_dialog_type_city_war_award,				// 城战结算界面
		e_map_result_dialog_type_arena,
		e_map_result_dialog_type_exp_raid,						//经验副本结算界面
		e_map_result_dialog_type_pk_king,						//PK之王结算界面
		e_map_result_dialog_type_max
	};

	enum e_map_result_dialog_pk_king_sub_type
	{
		e_map_result_dialog_pk_king_sub_type_death,				//死斗结算界面
		e_map_result_dialog_pk_king_sub_type_No_death,			//非死斗结算界面
	};

	enum e_map_lua_type
	{
		e_map_lua_type_init = 0,     //地图初始化
		e_map_lua_type_tick,         //地图tick
		e_map_lua_type_end,          //地图结束
		e_map_lua_type_unit_in,      //地图角色进入
		e_map_lua_type_unit_out,     //地图角色离开
		e_map_lua_type_unit_dead,    //地图角色死亡
	};
	struct s_map_record_info
	{
		int32 data_ary[e_map_record_info_max];

        s_map_record_info& operator=(const s_map_record_info& src)//重载运算符
        {
            for (int32 i = 0; i < e_map_record_info_max; i++)
            {
				data_ary[i] = src.data_ary[i];
            }
            return *this;
        }

        s_map_record_info(const s_map_record_info &src)  //拷贝构造函数
        {
            for (int32 i = 0; i < e_map_record_info_max; i++)
            {
				data_ary[i] = src.data_ary[i];
            }
        }

		s_map_record_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};

	struct s_map_record_info_db
	{
		guid_64           role_guid;
		s_map_record_info data_info;
	};

	struct s_demons_tower_info_db
	{
		guid_64 role_guid;
		int32   data_array[e_demons_tower_info_max];
	};

	struct s_demons_tower_info
	{
	public:
		int32 data_ary[e_demons_tower_info_max];

		bool is_valid()
		{
			if (data_ary[e_demons_tower_info_cur_challenge_tier] <= 0)
			{
				return false;
			}
			return true;
		}		
		s_demons_tower_info()
		{
			clear_data();
		}
		void clear_data()
		{
			data_ary[e_demons_tower_info_cur_challenge_tier] = 1;
			data_ary[e_demons_tower_info_left_mopping_up_count] = 1;
			data_ary[e_demons_tower_info_cur_mopping_up_tier] = 0;
			data_ary[e_demons_tower_info_can_get_award] = 0;
		}
		bool can_mopping_up()
		{
			if (data_ary[e_demons_tower_info_left_mopping_up_count] <= 0 && data_ary[e_demons_tower_info_cur_mopping_up_tier] <= 0)
			{
				return false; // 如果没有了剩余扫荡次数就不能进行扫荡
			}
			if (data_ary[e_demons_tower_info_can_get_award] > 0)
			{
				return false; // 如果上次扫荡的奖励还没有领取就不能开始下一次扫荡
			}
			return true;
		}

		// 获得当前正在扫荡的层的模板ID
		int32 get_cur_mopping_up_tier_template_id()
		{
			int32 cur_mopping_up_tier = data_ary[e_demons_tower_info_cur_mopping_up_tier];
			if (cur_mopping_up_tier <= 0)
			{
				return -1;
			}
			return first_demons_tower_template_id + cur_mopping_up_tier - 1;
		}

		//bool is_mopping_up_over()
		//{
		//	int32 mopping_up_tier = data_ary[e_demons_tower_info_cur_mopping_up_tier];
		//	if (mopping_up_tier < data_ary[e_demons_tower_info_cur_challenge_tier])
		//	{
		//		return false;
		//	}
		//	return true;
		//}

		// 返回值表示是否扫荡结束(true表示扫荡已经结束)
		bool mopping_up_tier_increase()
		{
			int32 mopping_up_tier = data_ary[e_demons_tower_info_cur_mopping_up_tier];
			if (mopping_up_tier + 1 < data_ary[e_demons_tower_info_cur_challenge_tier])
			{
				data_ary[e_demons_tower_info_cur_mopping_up_tier] = mopping_up_tier + 1;
				return true;
			}
			else
			{
				data_ary[e_demons_tower_info_can_get_award] = 1;
				return false;
			}
		}

		// 获得万魔塔扫荡奖励,如果当前不能获取就返回false
		bool get_award()
		{
			if (data_ary[e_demons_tower_info_can_get_award] <= 0)
			{
				return false;
			}
			else
			{
				data_ary[e_demons_tower_info_can_get_award] = 0;
				data_ary[e_demons_tower_info_cur_mopping_up_tier] = 0;
				return true;
			}
		}

		// 消耗一次扫荡次数
		void cost_left_mopping_up_count()
		{
			if (data_ary[e_demons_tower_info_left_mopping_up_count] > 0)
			{
				data_ary[e_demons_tower_info_left_mopping_up_count] -= 1;
			}
		}
	};

	struct score_record
	{
		guid_64	role_guid;
		xchar	role_name[max_name_size + 1];
		int32	role_level;
		int64	score;
		int32	server_id;
		guid_64 legion_guid;	//单人排行榜记录军团guid的
		int64	other_score;

		score_record()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(score_record));
		}

		void set_role_name(xstring _name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, _name.c_str(), _name.size() > sizeof(role_name) ? sizeof(role_name) : _name.size());
		}
	};
	enum e_doublie_line
	{
		e_double_line_map_template_id,
		e_double_line_open_time_hour,
		e_double_line_open_time_min,
		e_double_line_open_time_seconds,
		e_double_line_end_time_hour,
		e_double_line_end_time_min,
		e_double_line_end_time_seconds,
		e_double_line_buff_id,
		e_double_line_notice_id,
		e_double_line_max,
	};

	struct s_double_line_info
	{
		int32	data[hld::e_double_line_max];
		guid_64	map_guid;
		bool	is_in_time(tm time)
		{
			tm begin_time, end_time;
			memset(&begin_time, 0, sizeof(begin_time));
			memset(&end_time, 0, sizeof(end_time));
			begin_time.tm_hour = data[hld::e_double_line_open_time_hour];
			begin_time.tm_min = data[hld::e_double_line_open_time_min];
			begin_time.tm_sec = data[hld::e_double_line_open_time_seconds];
			end_time.tm_hour = data[hld::e_double_line_end_time_hour];
			end_time.tm_min = data[hld::e_double_line_end_time_min];
			end_time.tm_sec = data[hld::e_double_line_end_time_seconds];

			return time_helper::compare_tm(&time, &begin_time) > 0 && time_helper::compare_tm(&end_time, &time) > 0;
		}
		bool	is_valid()
		{
			return data[hld::e_double_line_map_template_id] > 0;
		}

		s_double_line_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_double_line_info));
		}
	};

	struct s_map_target_show_info	
	{
		guid_64	role_guid;
		xchar	role_name[max_name_size + 1];
		int32	image_index;

		s_map_target_show_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_map_target_show_info));
		}
		 
		void set_role_name(xstring _name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, _name.c_str(), _name.size() > sizeof(role_name) ? sizeof(role_name) : _name.size());
		}
	};

	enum e_map_target_image
	{
		e_map_target_image_element_red,		//元素争霸红
		e_map_target_image_element_blue,	//元素争霸蓝
	};
#pragma pack(pop)
}

#endif