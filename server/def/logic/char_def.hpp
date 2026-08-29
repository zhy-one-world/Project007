/********************************************************************
created: 2014/07/16
created: 16:7:2014 11:14
file base: char_def
file ext: hpp
author: zhy

purpose:
*********************************************************************/
#ifndef _CHAR_DEF_HPP_
#define _CHAR_DEF_HPP_

#include "logic/type_def.hpp"
#include "internet/server2dp.pb.h"
#include "internet/st.pb.h"

namespace faith
{
#pragma pack(push,1)

	const int32 role_data_save_time = 310000;
	const xstring account_and_password_login_prefix = "Faith";
	const int32 max_time_str_size = 20;								// 时间字符串长度
	const int32 max_packege_in_one_second = 20;						// 一秒鐘內發送數據包的最大值
	const int32 max_role_name_size = 8;								// 角色名字的可用最大字符数
	const int32 max_name_size = 100;								// 角色名最大字符数
	const int32 min_name_size = 2;									// 角色名最小字符数
	const int32 max_name_size_one_team = max_name_size * 5 +10;		// 一个队伍角色名最大字符数
	const uint32 max_account_length = 100;							// 账户名的最大字符数
	const uint32 min_account_length = 4;							// 账户名的最大字符数
	const uint32 max_password_length = 100;							// 密码的最大字符数
	const int32	max_unit_guid_str_max = 64;							// guid转化为str最大长度
	const uint32 max_server_msg_length = 500;						// sdk返回的message
	const uint32 max_device_type_length = 100;						// 设备类型
	const ui64 invalid_character_id = 0;				  			// 角色id的非法值
	const uint32 max_character_num = 3;								// 角色数最大上限
	const int32 max_time_role_count = 6;							// 每小时存储几次角色数量
	const int32 diconnect_repeat_num = 5;							// 断线重复检测次数 
	const uint32 limit_call_back_val_count = 8;
	const uint32 max_secondary_passwd_len = 12;						// 二级密码最大长度
	const int32 last_server_list_max_length = 256;	
	const uint32 max_captcha_length = 10;
	const uint32 max_token_len = 64;								// 跨服传送token长度（定长）
	const uint32 limit_cluster_login_key_length = 32;
	const uint32 limit_disable_account_desc_length = 64;
	const int32	role_max_level = 1000;								// 角色最大等级
	const int32	role_branch_id_diff = 100;							// 角色分支中间id差距
	const int32 role_watch_unit_num = 100;							// 玩家可以看到周围玩家完整数据的数量
	const uint32 init_item_num = 60;								// 玩家初始的物品数量
	const uint32 init_skill_num = 100;								// 玩家初始的技能数量
	const uint32 init_belief_num = 4;								// 玩家初始的信仰数量
	const uint32 init_pokedex_num = 8;								// 玩家初始的图鉴数量
	const uint32 init_mission_num = 4;								// 玩家初始任务数量
	const uint32 init_achievement_num = 200;						// 玩家初始成就数量
	const uint32 init_spirit_num = 200;								// 玩家初始的精灵数量
	const uint32 init_map_record_num = 256;                         // 玩家初始的地图记录数量
	typedef uint32	character_data_version;							// 玩家数据版本
	const int32	search_level_template_id_start = 171000001;			// 探索升级表起始id
	const int32	attribute_template_id_start	= 20000000;				// 属性说明表起始id
	const int32 money_template_id_start = 30000000;					// 金钱表起始id
	const int32   player_achievement_template_id_start = 34000001;  // 初始成就id
	const int32 player_grade_up_level_interval = 100;               // 角色转生等级间隔(比如每100级转生一次这个常量的值就是100)
	const int32 player_grade_up_peakedness_num = 3;					// 巅峰等级的转职次数
	const int32 player_grade_up_stage_nums = 5;						// 转生的小阶段数量
	const int32 player_grade_up_one_mission_num = 30;				// 一个任务完成标识数能表示的转生数量
	const uint32	game_config_template_id = 99000001;				// 游戏配置文件读取数据id
	const uint32  bag_extend_template_first_id = 33000000;
	const uint32  default_gather_distance = 500;					//默认/最小 采集距离
	const uint32  sync_pos_diff_distance = 100;						//最小同步距离平方
	const int32	ROBOT_BUFF_TEMPLATE_ID = 10010019;					//机器人增加的buff
	const int32 ROBOT_BUFF_EXP_TEMPLATE_ID = 10010040;				//机器人1.5倍经验id
	const int32	ROBOT_COPY_EXP_LEVEL = 100;							//机器人拷贝其他数据的起始等级

	const int32	req_other_info_retry_num = 5;						//申请他人信息重试次数
	const int32	req_other_info_retry_interval = 5;					//申请他人信息重试CD

	const int32 resource_find_back_day = 3;
	
	const uint32 max_gm_sql_str_length = 1024;

	const uint32 max_recharge_order_length = 40;					// 充值订单号最大长度
	const uint32 max_recharge_order_token_length = 7000;			// 充值token最大长度
	const int32 item_template_id_length = 8;						// 物品模板id的字符长度

	const int32 recharge_unfinish_message_keep_time = 120;			// 分包发送的SDK充值信息保存时间

	const int32 server_merge_guid_load_max = 1000;					//合服时一次处理guid数量

	const int32 person_information_tag_num_max = 8;				//玩家个人标签最大个数

	const int32 relation_push_num_max = 100;						//结缘推送最大人数

	const int32 person_infor_personalized_signature_max = 100;		//玩家个性签名最大字符数
	const int32 person_infor_city_max = 100;				//玩家城市最大字符数
	const int32 person_infor_hometown_max = 100;			//玩家家乡最大字符数
	const int32 person_infor_online_state_max = 100;			//玩家在线状态最大字符数
	const int32 person_infor_gender_max = 100;		//玩家性别最大字符数

	const int32 role_competition_max = 100;		//玩家竞赛奖励最大个数

	const int32 role_competition_tag_num = 7;	//玩家竞赛奖励int个数

	const int32 competition_season_id_start = 153000000;	//竞赛奖励起始id
	const int32 normal_manual_goods_type = 32;				//普通精英手册充值类型
	const int32 special_manual_goods_type = 33;				//豪华精英手册充值类型
	const int32 add_special_manual_goods_type = 34;			//豪华精英手册差价充值类型

	const int32 element_normal_manual_goods_type = 40;				//普通精英手册充值类型
	const int32 element_special_manual_goods_type = 41;				//豪华精英手册充值类型
	const int32 element_add_special_manual_goods_type = 42;			//豪华精英手册差价充值类型

	const int32 assist_fight_max_num = 5;					//助战最大人数

	const int32 assist_fight_help_notice_id = 90204169;					
	const int32 assist_fight_helper_notice_id = 90204170;

	const int32 can_show_other_line_session_num = 100;		//超过多少session则显示另一个线

	const int32 max_first_kill_npc_id_str = 280;			//首杀NPCid记录长度
	const int32 max_first_kill_npc_id_num = 30;				//首杀npcid记录数组最大值

	const int32 wing_func_unlock_id = 117000037;			//羽翼解锁Id

	const int32 time_limit_gift_db_num = 100;				// 限时礼包状态最大存储数量
	//========================= 日志 =========================================================
	//const int32 max_log_char_num = 4096;							// log日志最大长度 
	//static const char server_log_separator = 0x01;
	//static const std::string server_log_normversion = "1.1.4";
	//static const std::string server_invaild_string = "null";
	//static const std::string server_common_head = "loghead";
	//=========================================================================================
	const int32 char_quick_call_mount_max = 3;
	const int32 aoi_watch_unit_length = 2250000;

	const uint32 max_hit_and_get_npc_dis_power = 500 * 500;			// 碰撞并采集Npc最大距离的乘方
	
	const int32 player_upgrade_cfg_first_id = 21000001;				// 玩家标准数中第一个id
	const int32	monster_upgrade_cfg_first_id = 21050001;			// 怪物标准数中第一个id
	const int32	new_monster_upgrade_cfg_first_id = 21060001;			// 怪物标准数中第一个id
	const int32 default_show_fashion = 1;							// 创建人物默认勾选时装
	const int32 invalid_payment_type = -1;							// 无效的支付类型

	const int32 is_belong_string_template_id = 90037226;			// %s的%s

	const int32 max_passive_skill_num = 250;						// 被动技能数量
	const int32 max_legion_passive_skill_num = 150;					// 军团被动技能数量

	const int32 max_legion_skill_num = 20;							//军团技能数量

	const int32 random_seed_num = 10;								//随机种子数量

	const int32 max_recursion_num = 20;								//递归层数上限

	const int32 cache_other_player_num = 30;					//缓存其他玩家数量
	const int32 cache_drop_box_num = 20;						//缓存掉落包数量
	const int32 cache_bot_num = 50;								//缓存npc数量

	const int32	max_appearance_size = 1000;						//外形数据最大长度
	enum e_data_flag
	{
		e_data_flag_none,
		e_data_flag_info,		// 角色基本信息
		e_data_flag_buff,		// 角色Buff
		e_data_flag_skill,		// 角色技能
		e_data_flag_time,		// 玩家时间
		e_data_flag_money,		// 角色货币
		e_data_flag_time_activity,// 限时活动
		e_data_flag_mission,		// 角色任务
		e_data_flag_att,		// 角色属性
		e_data_flag_goods,		// 角色购买记录
		e_data_flag_map,		// 角色地图记录
		e_data_flag_service_goal,		// 七日目标记录
		e_data_flag_special_name,		// 角色称号记录
		e_data_flag_achievement,		// 角色成就记录
		e_data_flag_pokedex,		// 角色图鉴记录
		e_data_flag_item_upgrade,		// 角色物品强化数据
		e_data_flag_mail,		// 角色邮件记录
		e_data_flag_tinder,		// 角色火种记录
		e_data_flag_worship,		// 玩家崇拜记录
		e_data_flag_yesterday_must_do_remain,		// 昨天没做的必做任务
		e_data_flag_convert,		// 玩家转换记录
		e_data_flag_welfare,		// 玩家福利
		e_data_flag_active_degree,		// 玩家活跃度
		e_data_flag_title,		// 玩家称号
		e_data_flag_guide,		// 新手引导
		e_data_flag_fist_time_do,		// 首次操作
		e_data_flag_sprite_fetter,		// 精灵羁绊
		e_data_flag_talent,		// 角色天赋
		e_data_flag_daily_must_do_count,		// 每日必做次数记录
		e_data_flag_highest_record,		// 角色历史最高纪录
		e_data_flag_auction_self_selling,		// 交易行自己的出售物
		e_data_flag_globel_mail_had,		// 已拥有的全局邮件
		e_data_flag_service_rank,		// 全民冲榜信息
		e_data_flag_logic_info,		// 玩家逻辑数据
		e_data_flag_gain_treasure,		// 打宝数据
		e_data_flag_time_limit_activity,		// 限时活动数据	
		e_data_flag_relation,		// 角色关系
		e_data_flag_boss_island,		// 永恒岛数据
		e_data_flag_chat,		// 聊天数据
		e_data_flag_passive_skill,		// 被动技能
		e_data_flag_seven_day_goal,		// 七日目标前七天记录
		e_data_flag_harry_record,		// 跨服掠夺玩家记录
		e_data_flag_phantom,		// 角色幻灵
		e_data_flag_oracle_trial,		// 天启试炼
		e_data_flag_chat_record,		// 
		e_data_flag_player_information,		// 个人信息
		e_data_flag_recycle,     // 回归信息
		e_data_flag_recycle_task,     // 回归任务
		e_data_flag_player_competition,		// 竞赛信息
		e_data_flag_recycle_invited,		// 回归招募
		e_data_flag_dragontrip,		// 萌龙奇遇
		e_data_flag_legion_skill,		// 军团技能
		e_data_flag_skytreasure,		// 天空宝库
		e_data_flag_starark,		// 星海方舟
		e_data_flag_element_competition,		// 元素手册
		e_data_flag_time_feed_back,		// 元购
		e_data_flag_time_limit_gift,		// 限时礼包
		e_data_flag_subscribe_daily_info,		// 活动预告
		e_data_flag_item = 100000,		// 角色物品(物品要多次存档，所以要保持物品的标记位再最后)
	};

//////////////////////////////////////////////////////////////////////////
	enum e_unit_res
	{
		e_unit_res_mesh,				// 模型
		e_unit_res_blueprint,			// 动画蓝图
		e_unit_res_spell,				// 吟唱动作
		e_unit_res_attack,				// 攻击动作
		e_unit_res_cast,				// 施法动作
		e_unit_res_channel,				// 通道技
		e_unit_res_rush,				// 冲
		e_unit_res_ghost,			    // 
		e_unit_res_hurt,				// 受伤动作
		e_unit_res_dead,				// 死亡动作
		e_unit_res_talk,				// 聊天动作
		e_unit_res_xuanyun,				// 眩晕动作
		e_unit_res_born,				// 出生动作
		e_unit_res_jump_to_mount,		// 跳上坐骑
		e_unit_res_wing,				// 召唤翅膀动作
		e_unit_res_flyup,				// 坐骑飞行上升
		e_unit_res_flydown,				// 坐骑飞行下降
		e_unit_res_change_mount,		// 换坐骑
		e_unit_res_sprint,				// 
		e_unit_res_double_mounts,		// 双人坐骑
		e_unit_res_wing_fly_up,			// 翅膀飞行上升
		e_unit_res_wing_fly_down,		// 翅膀飞行下降
		e_unit_res_channel_1,			// 冰channel
		e_unit_res_channel_2,			// 火channel
		e_unit_res_channel_3,			// 光channel
		e_unit_res_channel_4,			// 暗channel
		e_unit_res_channel_mount,		// 坐骑channel
		e_unit_res_channel_wing,		// 翅膀channel
		e_unit_res_max
	};

	enum e_unit_mesh_data
	{
		e_unit_mesh_data_body,									//身体模型
		e_unit_mesh_data_head,									//头部模型
		e_unit_mesh_data_hair,									//头发模型
		e_unit_mesh_data_headdress,								//头饰模型
		e_unit_mesh_data_mouth,									//嘴部模型
		e_unit_mesh_data_eye,									//眼部模型	
		e_unit_mesh_data_max									
	};

	enum e_save_role_data_type							// 角色数据保存类型
	{
		e_save_data_type_exit_game,						// 退出游戏
		e_save_data_type_leave_scene,					// 传送时离开场景相关数据保存
		e_save_data_type_update,						// 定时存盘
		e_save_data_type_cross,							// 传送到其他服务器
		e_save_data_type_max,
	};
	enum e_be_dead_reason								// 死亡原因
	{
		e_be_dead_self,									//自杀
		e_be_dead_other,								//被他人杀死
	};

	enum e_monster_born
	{
		e_monster_born_nomal,									//无动画
		e_monster_born_anim,									//动画
		e_monster_born_soluble,									//溶解
		e_monster_born_soluble_anim,							//溶解+动画
	};
	// 查找角色返回结果
	enum get_char_result
	{
		e_enum_result_failed_common_error,		// 没有login就发送该消息
		e_enum_result_success_exist_char,		// 查找到角色
		e_enum_result_success_no_exist_char,	// 查找成功未查找到角色
		e_enum_result_failed_no_exist_char,		// 查找失败未查找到角色
		e_enum_result_max,
	};

	// 创建角色返回结果
	enum create_char_result
	{
		e_create_result_unknow,
		e_create_result_success,
		e_create_result_failed,
		e_create_result_role_name_invalid = 4,
		e_create_role_name_duplicate = 6,
		e_create_result_max,
	};

	// 角色转生操作的返回结果
	enum e_grade_up_result
	{
		e_grade_up_result_unknown,                      // 未知错误
		e_grade_up_result_success,                      // 转生成功
		e_grade_up_result_level_not_enough,             // 等级不足
		e_grade_up_result_silver_not_enough,            // 金币不足
		e_grade_up_result_magic_crystal_not_enough,     // 魔晶不足
		e_grade_up_result_full_grade_num,               // 已达到最大转生次数
		e_grade_up_result_max
	};

	enum e_character_error
	{
		e_character_error_unknwon,
		e_character_error_exp_full,    // 经验值已不能再继续累加
		e_character_error_max
	};

	enum e_session_status
	{
		e_ss_invalid,	                    // 非法值
		e_session_status_created_session,   // session创建完毕
		e_session_status_create_player,	    // player 创建完毕
		e_session_status_dp_loading,	    // session加入到管理器，开始从 DP 读取数据
		e_session_status_client_loading,    // DP加载完毕后，向client同步数据
		e_session_status_in_gaming,	        // 进入到场景，游戏状态
		e_session_status_switching_loading,	// 跨loading区域数据加载
		e_session_status_logout_connect,	// 在断线重连状态
		e_session_status_max,	            // 
	};

	enum e_role_gender
	{//性別類型
		e_role_gender_male = 0,						//	男性
		e_role_gender_female = 1,					//	女性
	};
	//enum e_revive_mode
	//{//復活類型
	//	e_revive_mode_same_pos,				//原地
	//	e_revive_mode_transfer,				//回城
	//	e_revive_mode_born_pos,				//出生点复活
	//	e_revive_mode_max,
	//};
	enum e_warning_type
	{
		e_warning_type_find,				//发现攻击者
		e_warning_type_use_skill			//使用技能前奏
	};
	enum green_channel_type
	{
		e_gct_none,
		e_gct_user,
		e_gct_manager,
		e_gct_none_token_crosser,
	};




	enum e_sync_cs2ws_data_type
	{
		e_sync_cs2ws_data_gs_value,
		e_sync_cs2ws_data_cur_hp,
		e_sync_cs2ws_data_max_hp,
		e_sync_cs2ws_data_exp_level,
		//e_sync_cs2ws_data_money,
		//e_sync_cs2ws_data_demonstower,
		//e_sync_cs2ws_data_wing,
		//e_sync_cs2ws_data_mount,
		//e_sync_cs2ws_data_worship,
		e_sync_cs2ws_military_rank,
		e_sync_cs2ws_data_player_template,
		e_sync_cs2ws_data_dead_to_addhate,
		e_sync_cs2ws_data_vip_level,
		e_sync_cs2ws_data_is_show_vip,
		e_sync_cs2ws_data_max
	};

	struct s_logic_info
	{
		int32 data_ary[e_role_logic_info_max];
		s_logic_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct s_logic_info_db
	{
		guid_64			role_guid;
		int32			data_ary[e_role_logic_info_max];
		s_logic_info_db()
		{
			role_guid.server_64 = 0;
			memset(data_ary, 0, sizeof(data_ary));
		}
	};

	struct s_time_info
	{
		i64 data_ary[e_time_type_max];
		s_time_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct s_time_info_db
	{
		guid_64			role_guid;
		i64				data_ary[e_time_type_max];
		s_time_info_db()
		{
			role_guid.server_64 = 0;
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	const xchar* const e_role_info_str[] = 
	{
		"e_role_info_template_id",						//模板id
		"e_role_info_server_id",						//服务器id
		"e_role_info_img_id",							//头像
		"e_role_info_class_type",						//职业
		"e_role_info_class_branch",						//分支
		"e_role_info_gender",							//角色性别
		"e_role_info_exp_level",						//经验等级
		"e_role_info_vip_level",						//vip等级
		"e_role_info_vip_experience_level",				//vip体验等级
		"e_role_info_recharge_num",						//充值点数
		"e_role_info_main_map_id",						//大地图模板id
		"e_role_info_main_pos_x",						//大地图坐标x
		"e_role_info_main_pos_y",						//10大地图坐标y
		"e_role_info_main_pos_z",						//大地图坐标z
		"e_role_info_main_pos_yaw",						//大地图旋转
		"e_role_info_main_line_id",						//15大地图线id
		"e_role_info_move_map_id",						//要到达地图模板id
		"e_role_info_move_pos_x",						//当前坐标x
		"e_role_info_move_pos_y",						//当前坐标y
		"e_role_info_move_pos_z",						//当前坐标z
		"e_role_info_move_pos_yaw",						//旋转
		"e_role_info_move_line_id",						//要到达地图线id
		"e_role_info_GS_value",							//25当前战斗力值
		"e_role_info_upgrade_id",						//等级模板id
		"e_role_info_daily_done_num",					//日常完成数目
		"e_role_info_against_done_num",					//讨伐完成数目
		"e_role_info_wing_showd_template_id",			//30要显示的翅膀ID
		"e_role_info_mount_showd_template_id",			//要显示的坐骑ID
		"e_role_info_element_heart_drop_group",			//元素之心的抽取组
		"e_role_info_arena_times",						//竞技场次数
		"e_role_info_arena_vip_times",					//vip可挑战次数
		"e_role_info_arena_vip_buy_times",				//35vip已购买次数
		"e_role_info_arena_winning_streak",				//连胜纪录
		"e_role_info_legion_donate_earn_by_money",		//军团捐献金币获得的战功
		"e_role_info_legion_donate_earn_by_item",		//军团捐献物品获得的战功
		"e_role_info_worship_other_time",				//玩家崇拜次数
		"e_role_info_get_func_unlock_award_record",		//解锁奖励的领取
		"e_role_info_vip_item_geted_level",				// VIP礼包领取到第几级了
		"e_role_info_vip_free_revive_time",				//免费复活次数
		"e_role_info_element_heart_refine_time",		//元素之心提炼次数
		"e_role_info_total_login_days",					//累计登陆天数
		"e_role_info_continue_login_time",				//45连续登录次数
		"e_role_info_daily_activity_degree",			//日活跃度
		"e_role_info_daily_recharge_num",				//日充值数
		"e_role_info_total_diamond_cost_num",			//累计消费钻石数
		"e_role_info_daily_active_degree_get_flag",		//每日活跃奖励领取标志位
		"e_role_info_fund_recharge_num",				//50成长基金，充值基金充值额
		"e_role_info_fund_payment_flag",				//成长基金购买标记位
		"e_role_info_fund_time_get_flag",				//成长基金，时光领取标记位
		"e_role_info_fund_pride_get_flag",				//成长基金，充值领取标记位
		"e_role_info_fund_reborn_get_flag",				//成长基金，重生领取标记位
		"e_role_info_fund_cost_num",					//55成长基金，充值基金消费额
		"e_role_info_lord_of_city_worship_time",		//城主膜拜次数
		"e_role_info_lord_of_war_worship_time",			//PK之王膜拜次数
		"e_role_info_lord_of_sanctuary_worship_time",	//圣域之主膜拜次数
		"e_role_info_lord_of_god_worship_time",			//众神之王膜拜次数
		"e_role_info_hope_value",						//祈福祝福值
		"e_role_info_del_time",							//删除时间
		"e_role_info_kill_player_num",					//击杀玩家数量
		"e_role_info_spirit_value",						//精灵猎取保底
		"e_role_info_watch_count",						//查看的最大人数
		"e_role_info_crystal_dreamland_chests",			//水晶幻境采集宝箱个数
		"e_role_info_protect_mark_slot_open_num",		//守护印记槽位开启个数
		"e_role_info_element_heart_slot_open_num",		//元素之心槽位开启个数
		"e_role_info_sprite_equip_slot_open_num",		//精灵装备槽位开启个数
		"e_role_info_exclusive_flag",
		"e_role_info_red_package_geted_times",			//每日红包领取次数
		"e_role_info_free_red_package_send_times",		//每日免费发红包次数
		"e_role_info_world_boss_chests",				//世界BOSS掉落宝箱采集
		"e_role_info_leave_map_id",						//要离开的地图id
		"e_role_info_assist_chests",					//助战宝箱采集
		"e_role_info_next_side_mission_main_trigger_id",	//下个待触发支线任务的主线任务ID
		"e_role_info_show_fashion",						//是否显示时装
		"e_role_info_real_recharge_jewel_num",			//冲入钻石数主要用于充值榜
		"e_role_info_relic_kill_num",					//上古遗迹今日杀怪数
		"e_role_info_relic_exp_num",					//上古遗迹今日获得经验数
		"e_role_info_head_frame",						//头像框
		"e_role_info_fund_pride_cost_value",			//成长基金消费点数
		"e_role_info_tinder_prop_slot_open_num",			//火种道具槽位开启个数
		"e_role_info_show_sky_suit_shape", //是否显示天空套装外形
		"e_role_info_move_server_id",					//要到达服务器
		"e_role_info_move_war_idex",					//要到达地图位置
		"e_role_info_goddess_slot_flag",				//女神协战槽位开启标记
		"e_role_info_daily_cost",						//每日消费数值
		"e_role_info_marry_done_num",				//结婚任务每日完成次数
		"e_role_info_marry_dati_id",				//当前结婚答题id
		"e_role_info_marry_dati_num",				//结婚答题完成数量
		"e_role_info_marry_dati_right_num",			//结婚答题答对数量
		"e_role_info_marry_dati_current_answer",	//结婚答题当前答案
		"e_role_info_legion_dance_exp_num",			//军团共舞今日经验获取次数
		"e_role_info_legion_donate_num",			//军团捐赠次数
		"e_role_info_appearance_id",				//捏脸外形ID
		"e_role_info_is_show_vip",					//是否显示vip
		"e_role_info_need_check_guide",				//是否需要检查引导
		"e_role_info_enchant_show_type",			//附魔显示buff类型
		"e_role_info_push_map_id",					//推图的地图id
	};

	const xchar* const e_role_i64_info_str[] =
	{
		"e_role_i64_info_gs_value",				//战力
		"e_role_i64_info_history_best_gs_value",//历史最高战力
		"e_role_i64_info_zero_mount_up_start_value", //零点刷新坐骑提升榜初始值
		"e_role_i64_info_zero_wing_up_start_value", //零点刷新翅膀提升榜初始值
		"e_role_i64_info_zero_equip_up_start_value", //零点刷新装备提升榜初始值
		"e_role_i64_info_zero_spirit_up_start_value", //零点刷新精灵提升榜初始值
		"e_role_i64_info_create_time", //创建时间
	};

	const xchar* const e_role_ws_info_str[] =
	{
		"save_time",										//保存时间
		"red_package_free_count",							//免费红包计数
	};

	enum e_grow_up_fund_type
	{
		e_grow_up_fund_type_reborn,					//重生基金
		e_grow_up_fund_type_time,					//时光基金
		e_grow_up_fund_type_pride,					//充值基金
		e_grow_up_fund_type_max,
	};

	enum e_relivepoint_read
	{
		e_relivepoint_read_x,		//复活点X值
		e_relivepoint_read_y,		//复活点Y值
		e_relivepoint_read_z,		//复活点Z值
		e_relivepoint_read_pitch,	//绕X轴转
		e_relivepoint_read_yaw,		//绕Y轴转
		e_relivepoint_read_roll,	//绕Z轴转
		e_relivepoint_read_max		//复活点数据总个数
	};

	//enum e_powerup_target_typ
	//{
	//	e_powerup_target_non,
	//	e_powerup_target_wing_grade_num,						//翅膀阶数
	//	e_powerup_target_wing_star_num_in_cur_grade,			//翅膀当前阶数下的星级
	//	e_powerup_target_skill_level,							//技能等级
	//	e_powerup_target_equips_average_upgrade_level,			//装备平均强化等级
	//	e_powerup_target_equips_average_addon_level,			//装备平均追加等级
	//	e_powerup_target_amulet_level,							//护身符等级
	//	e_powerup_target_VIP_level,								//VIP等级
	//	e_powerup_target_achievement_level,						//成就等级
	//	e_powerup_target_rank_level,							//军衔等级
	//	e_powerup_target_activity_belief_level,					//信仰激活总等级
	//	e_powerup_target_succinct,								//培养属性达到上限的 人物转生阶 装备数
	//	e_powerup_target_fighting_spirit_level,					//出战宠物等级达到10级
	//	e_powerup_target_element_heart_total_level,				//元素之心总等级达到10级
	//	e_powerup_target_equips_average_quality_level,			//装备平均等阶达到10级
	//	e_powerup_target_reincarnation_num,						//转生
	//	e_powerup_target_protect_spirit_level,					//守护神阶数
	//	e_powerup_target_wing_feather,							//翅膀翎羽
	//	e_powerup_target_wing_add_spirit,						//翅膀注灵
	//	e_powerup_target_wing_add_soul,							//翅膀注魂
	//	e_powerup_target_mount_grade_num,						//坐骑阶数
	//	e_powerup_target_mount_level_num,						//坐骑等级
	//	e_powerup_target_pokedex_actived_num,					//图鉴激活总数
	//	e_powerup_target_max
	//};
	enum e_unit_guid
	{
		e_unit_guid_account,
		e_unit_guid_role_guid,
		e_unit_guid_role_name
	};
	enum e_check_type
	{
		e_check_type_ch,
		e_check_type_tw,
		e_check_type_kr,
		e_check_type_max
	};

	struct s_unit_info
	{
		guid_64		role_guid;
		xchar		account[max_account_length + 1];
		xchar		role_name[max_name_size + 1];
		xchar		role_appearance[max_appearance_size + 1];
		xchar		role_first_kill_npc_id_arr[max_first_kill_npc_id_str + 1];
		guid_64		map_guid;
		int64		data_i64_ary[e_role_i64_info_max];
		int32		data_ary[e_role_info_max];
		s_unit_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			data_ary[e_role_info_show_fashion] = 1;
		}
		void set_role_name(xstring temp_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, temp_name.c_str(), temp_name.size() > max_name_size ? max_name_size : temp_name.size());
		}
		void set_appearance(xstring temp_appearance)
		{
			memset(role_appearance, 0, sizeof(role_appearance));
			memcpy(role_appearance, temp_appearance.c_str(), temp_appearance.size() > max_appearance_size ? max_appearance_size : temp_appearance.size());
		}
		void set_role_account(xstring temp_account)
		{
			memset(account, 0, sizeof(account));
			memcpy(account, temp_account.c_str(), temp_account.size() > max_account_length ? max_account_length : temp_account.size());
		}
		const int64 get_gs_value()
		{
			return data_i64_ary[e_role_i64_info_gs_value];
		}
		void set_first_kill_npc_id_arr(xstring temp_arr)
		{
			memset(role_first_kill_npc_id_arr, 0, sizeof(role_first_kill_npc_id_arr));
			memcpy(role_first_kill_npc_id_arr, temp_arr.c_str(), temp_arr.size() > max_first_kill_npc_id_str ? max_first_kill_npc_id_str : temp_arr.size());
		}
		void set_proto_info(server2dp_proto_s_unit_info * _info) const
		{
			if (nullptr == _info)
			{
				return;
			}
			_info->set_role_guid(role_guid.server_64);
			_info->set_account(account);
			_info->set_role_name(role_name);
			_info->set_role_appearance(role_appearance);
			_info->set_first_kill_npc_arr(role_first_kill_npc_id_arr);
			for (int32 i = 0; i < e_role_i64_info_max; ++i)
			{
				_info->add_data_i64_ary(data_i64_ary[i]);
			}
			for (int32 i = 0; i < e_role_info_max; ++i)
			{
				_info->add_data_ary(data_ary[i]);
			}
		}
		void proto_set_info(server2dp_proto_s_unit_info _info)
		{
			role_guid.server_64 = _info.role_guid();
			set_role_account(_info.account());
			set_role_name(_info.role_name());
			set_appearance(_info.role_appearance());
			set_first_kill_npc_id_arr(_info.first_kill_npc_arr());
			for (int32 i = 0; i < e_role_i64_info_max; ++i)
			{
				if (_info.data_i64_ary_size() > i)
				{
					data_i64_ary[i] = _info.data_i64_ary(i);
				}
			}
			for (int32 i = 0; i < e_role_info_max; ++i)
			{
				if (_info.data_ary_size() > i)
				{
					data_ary[i] = _info.data_ary(i);
				}
			}
		}
		bool to_proto(faith::st_proto::st_unit_info *st_unit_ptr)
		{
			if (st_unit_ptr == nullptr)
			{
				return false;
			}
			st_unit_ptr->set_role_guid(role_guid.server_64);
			st_unit_ptr->set_account(account);
			st_unit_ptr->set_role_name(role_name);
			st_unit_ptr->set_role_appearance(role_appearance);
			st_unit_ptr->set_role_first_kill_ary(role_first_kill_npc_id_arr);
			for (int32 i = 0; i < e_role_i64_info_max; ++i)
			{
				st_unit_ptr->add_data_i64_ary(data_i64_ary[i]);
			}
			for (int32 i = 0; i < e_role_info_max; ++i)
			{
				st_unit_ptr->add_data_ary(data_ary[i]);
			}
			return true;
		}
		void from_proto(const faith::st_proto::st_unit_info& st_unit_ref)
		{
			role_guid.server_64 = st_unit_ref.role_guid();
			set_role_account(st_unit_ref.account());
			set_role_name(st_unit_ref.role_name());
			set_appearance(st_unit_ref.role_appearance());
			set_first_kill_npc_id_arr(st_unit_ref.role_first_kill_ary());
			for (int32 i = 0; i < e_role_i64_info_max && i < st_unit_ref.data_i64_ary_size(); ++i)
			{
				data_i64_ary[i] = st_unit_ref.data_i64_ary(i);			
			}
			for (int32 i = 0; i < e_role_info_max && i < st_unit_ref.data_ary_size(); ++i)
			{
				data_ary[i] = st_unit_ref.data_ary(i);
			}
		}
	};

	struct s_unit_info_db_check
	{
		guid_64		role_guid;
		xchar		account[max_account_length + 1];
		xchar		role_name[max_name_size + 1];
		xchar		role_appearance[max_appearance_size + 1];
		xchar		role_first_kill_npc_id_arr[max_first_kill_npc_id_str + 1];
		guid_64		map_guid;
		int64		data_i64_ary[e_role_i64_info_max];
		int32		data_ary[e_role_info_max];
		int32		server_id;
		s_unit_info_db_check()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			data_ary[e_role_info_show_fashion] = 1;
		}
		void set_unit_info(const s_unit_info& _unit_info)
		{
			role_guid = _unit_info.role_guid;
			set_role_name(_unit_info.role_name);
			set_role_appearance(_unit_info.role_appearance);
			set_role_account(_unit_info.account);
			set_first_kill_npc_id_arr(_unit_info.role_first_kill_npc_id_arr);
			map_guid = _unit_info.map_guid;
			for (int32 i = 0; i < e_role_i64_info_max; i++)
			{
				data_i64_ary[i] = _unit_info.data_i64_ary[i];
			}
			for (int32 i = 0; i < e_role_info_max; i++)
			{
				data_ary[i] = _unit_info.data_ary[i];
			}
		}
		void set_role_name(xstring temp_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, temp_name.c_str(), temp_name.size() > max_name_size ? max_name_size : temp_name.size());
		}
		void set_role_appearance(xstring temp_appearance)
		{
			memset(role_appearance, 0, sizeof(role_appearance));
			memcpy(role_appearance, temp_appearance.c_str(), temp_appearance.size() > max_appearance_size ? max_appearance_size : temp_appearance.size());
		}
		void set_role_account(xstring temp_account)
		{
			memset(account, 0, sizeof(account));
			memcpy(account, temp_account.c_str(), temp_account.size() > max_account_length ? max_account_length : temp_account.size());
		}
		const int64 get_gs_value()
		{
			return data_i64_ary[e_role_i64_info_gs_value];
		}
		void set_first_kill_npc_id_arr(xstring temp_arr)
		{
			memset(role_first_kill_npc_id_arr, 0, sizeof(role_first_kill_npc_id_arr));
			memcpy(role_first_kill_npc_id_arr, temp_arr.c_str(), temp_arr.size() > max_first_kill_npc_id_str ? max_first_kill_npc_id_str : temp_arr.size());
		}
	};

	struct s_last_server_list_db
	{
		int32 server_id;
		xchar last_server_list[last_server_list_max_length];
		s_last_server_list_db()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		void set_last_server_list(xstring t_last_server_list)
		{
			memset(last_server_list, 0, sizeof(last_server_list));
			memcpy(last_server_list, t_last_server_list.c_str(), t_last_server_list.size() > last_server_list_max_length ? last_server_list_max_length : t_last_server_list.size());
		}
	};

	struct s_unit_ws_info
	{
		int32		data_ary[e_role_ws_info_max];
		s_unit_ws_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_char_show_info
	{
		guid_64		role_guid;
		int32		showed_mount_template_id;
		int32		showed_wing_template_id;
		guid_64		showed_mount_guid;
		guid_64		showed_wing_guid;
		guid_64		quick_call_mount_guid_array[char_quick_call_mount_max];
		guid_64		showed_quickly_hp_guid;

		s_char_show_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	enum e_meditation_reward_type
	{
		e_meditation_reward_usual,					//一倍奖励
		e_meditation_reward_double,                 //两倍奖励
		e_meditation_reward_four,                  //四倍奖励
	};

	enum e_meditation_get_reward_endtype
	{
		e_meditation_get_reward_success = 0,		//成功
		e_meditation_reward_vip,					//vip等级不足
		e_meditation_reward_money_lack,             //货币不足
		e_meditation_reward_less_one_munite,		//少于一分钟
		e_meditation_reward_unknown,				//未知错误
	};


	struct s_unit_yesterday_must_do_remain
	{
		guid_64		role_guid;
		int32		role_exp_level_yesterday;
		int32		save_date;
		int32		role_vip_level_yesterday;
		int32       distance_today_day_count;
		int32       role_arena_rank;
		int32		data_ary[e_daily_must_do_typ_max];
		s_unit_yesterday_must_do_remain()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			role_arena_rank = -1;
		}
	};


	enum e_daily_must_do_sub_tap_typ
	{
		e_daily_must_do_sub_tap_typ_exp,
		e_daily_must_do_sub_tap_typ_siliver,
		e_daily_must_do_sub_tap_typ_magic_crystal,
		e_daily_must_do_sub_tap_typ_equips,
		e_daily_must_do_sub_tap_typ_crystal_stone,
		e_daily_must_do_sub_tap_typ_feather,
		e_daily_must_do_sub_tap_typ_achievement,
		e_daily_must_do_sub_tap_typ_prestige,
		e_daily_must_do_sub_tap_typ_diamond,
		e_daily_must_do_sub_tap_typ_pound,
		e_daily_must_do_sub_tap_typ_max
	};

	enum e_vip_item_get
	{
		e_vip_item_get_non	= 0x00000000,		// vip奖励没领过
		e_vip_item_get_0	= 0x00000001,		// 0级vip奖励
		e_vip_item_get_1	= 0x00000002,		// 1级vip奖励
		e_vip_item_get_2	= 0x00000004,		// 2级vip奖励
		e_vip_item_get_3	= 0x00000008,		// 3级vip奖励
		e_vip_item_get_4	= 0x00000010,		// 4级vip奖励
		e_vip_item_get_5	= 0x00000020,		// 5级vip奖励
		e_vip_item_get_6	= 0x00000040,		// 6级vip奖励
		e_vip_item_get_7	= 0x00000080,		// 7级vip奖励
		e_vip_item_get_8	= 0x00000100,		// 8级vip奖励
		e_vip_item_get_9	= 0x00000200,		// 9级vip奖励
		e_vip_item_get_10	= 0x00000400,		// 10级vip奖励
		e_vip_item_get_11	= 0x00000800,		// 11级vip奖励
		e_vip_item_get_12	= 0x00001000,		// 12级vip奖励
		e_vip_item_get_13	= 0x00002000,		// 13级vip奖励
		e_vip_item_get_14	= 0x00004000,		// 14级vip奖励
		e_vip_item_get_15	= 0x00008000,		// 15级vip奖励
	};

	enum e_recharge_typ
	{
		e_recharge_typ_non,
		e_recharge_typ_month_card,
		e_recharge_typ_month_money,
	};

	enum e_recharge_step
	{
		e_recharge_step_begin,								// 充值开始
		e_recharge_step_ws_save_recharge,					// ws保存充值记录
		e_recharge_step_sp_save_recharge,					// dp保存充值记录
		e_recharge_step_sp_save_recharge_end,				// dp保存充值记录结束
		e_recharge_step_ws_save_recharge_end,				// ws保存充值记录结束
		e_recharge_step_ws_to_cs_send_recharge,				// ws发送充值到cs
		e_recharge_step_ws_to_cs_send_recharge_end,			// cs收到ws的充值记录
		e_recharge_step_cs_recharge_by_template_id,			// cs开始充值
		e_recharge_step_cs_recharge_by_template_id_end,		// cs充值成功
		e_recharge_step_ws_recharge_send_mail,				// 玩家离线发送邮件
		e_recharge_step_ws_send_mail_end,					// 玩家离线发送邮件成功
	};

	enum e_recharge_error
	{
		e_recharge_error_none,						//无错误
		e_recharge_error_begin_order_id_is_null,
		e_recharge_error_send_save_recharge_order_id_is_null,
		e_recharge_error_send_save_recharge_dp_is_null,
		e_recharge_error_sp_save_recharge_pData_is_null,
		e_recharge_error_sp_save_recharge_end_error,
		e_recharge_error_dp_to_ws_packet_is_null,
		e_recharge_error_db_save_recharge_order_id_is_null,
		e_recharge_error_db_save_recharge_order_id_len_less,
		e_recharge_error_db_save_recharge_order_id_len_bigger,
		e_recharge_error_db_save_recharge_role_id_is_not_valid,
		e_recharge_error_db_save_recharge_game_goods_id_less,
		e_recharge_error_db_save_recharge_success_less,								//已存在的账单
		e_recharge_error_ws2cs_recharge_proc_packet_is_null,
		e_recharge_error_ws2cs_recharge_proc_data_len_error,
		e_recharge_error_ws2cs_recharge_proc_player_not_valid,
		e_recharge_error_recharge_by_template_id_recharge_template_ptr_is_null,
		e_recharge_error_recharge_by_template_id_order_id_is_null,
		e_recharge_error_cs_dp_recharge_end_order_id_is_null,
		e_recharge_error_cs_dp_recharge_end_order_id_len_error,
		e_recharge_error_recharge_by_template_id_MoneyGetArray_error,
		e_recharge_error_recharge_by_template_id_send_mail_recharge_template_ptr_is_null,
		e_recharge_error_send_save_recharge_end_order_id_is_null,
		e_recharge_error_save_recharge_end_pData_is_null,
		e_recharge_error_cs2dp_req_save_recharge_end_pData_is_null,
		e_recharge_error_client_seesion_save_recharge_order_id_is_null,
		e_recharge_error_cs2ws_recharge_end_proc_package_is_null,
		e_recharge_error_cs2ws_recharge_end_proc_package_len_error,
		e_recharge_error_dp2ws_send_recharge_end_result_proc_packet_is_null,
		e_recharge_error_cs2ws_recharge_end_send_mail_proc_package_is_null,
		e_recharge_error_cs2ws_recharge_end_send_mail_proc_package_len_error,
		e_recharge_error_recharge_send_mail_order_id_is_null,
		e_recharge_error_recharge_send_mail_order_id_len_less,
		e_recharge_error_recharge_send_mail_order_id_len_bigger,
		e_recharge_error_recharge_send_mail_role_id_is_not_valid,
		e_recharge_error_recharge_send_mail_game_goods_id_less,
		e_recharge_error_check_role_name_pdata_is_null,
		e_recharge_error_change_role_name_pdata_is_null,
		e_recharge_error_http_verify_failed,
		e_recharge_error_json_parse_failed,
		e_recharge_error_sub_package_lost,
		e_recharge_error_un_consume,
	};

	enum e_create_role_result
	{
		e_failed_common_error = 0,
		e_create_role_success = 1,
		e_failed_internal_error,
		e_failed_create_error,
		e_failed_role_name_invalid,
		e_failed_invalid_param_other,
		e_failed_role_name_duplicate,
		e_failed_role_name_no_in_use,
		e_failed_role_name_in_no_use,
		e_failed_role_num_max,
		e_failed_role_name_size_too_short,
		e_failed_role_name_size_too_long,
		e_failed_role_create_level_limit,
		e_failed_role_item_not_enough
	};

	struct s_money_info
	{	
		i64						cur_data_ary[e_money_type_max];				// 财产实例数据
		i64						all_data_ary[e_money_type_max];				// 总获得数据
		i64						cost_data_ary[e_money_type_max];			// 总消费数据
		s_money_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(cur_data_ary, 0, sizeof(cur_data_ary));
			memset(all_data_ary, 0, sizeof(all_data_ary));
		}
	};
	struct s_money_info_db
	{
		guid_64			role_guid;
		int32			money_type;
		i64				data_ary[e_money_type_max];
		s_money_info_db()
		{
			money_type = 0;
			role_guid.server_64 = 0;
			memset(data_ary, 0, sizeof(data_ary));
		}
	};

	struct s_money_tuple
	{
		int32 money_id;
		int64 money_num;

		s_money_tuple()
		{
			money_id = 0;
			money_num = 0;
		}
	};

	enum e_role_right
	{
		e_role_right_allow_login,//是否允许登录
		e_role_right_allow_chat,//是否允许发言
		e_role_right_radio_host,//是否主播
		e_role_right_vip_point,//vip点数
		e_role_right_max
	};
	struct s_role_right
	{
		int32 data_ary[e_role_right_max];
		s_role_right()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_role_history_high_record_info
	{
		int64 data_ary[e_role_history_highest_record_max];
		s_role_history_high_record_info()
		{
			clear_data();
		}
		void clear_data()
		{
			for (int32 i = 0; i < faith::e_role_history_highest_record_max; ++i)
			{
				data_ary[i] = -1;
			}
		}
	};

	struct s_role_history_high_record_info_db
	{
		guid_64 role_guid;
		s_role_history_high_record_info data_info;

		s_role_history_high_record_info_db()
		{
			clear_data();
		}
		void clear_data()
		{
			role_guid.clear_data();
			data_info.clear_data();
		}
	};

	enum e_role_data_ws_ex
	{
		e_role_data_ws_ex_cur_hp,
		e_role_data_ws_ex_max_hp,
		e_role_data_ws_ex_max,
	};

	struct s_role_daily_must_do_count
	{
	public:
		guid_64		role_guid;
		int32 data_ary[e_daily_must_do_typ_max];
		s_role_daily_must_do_count()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct s_role_person_information
	{
	public:
		guid_64     role_guid;
		int32		infor_state;
		int32		reward_state;
		xchar		gender[person_infor_gender_max + 1];
		xchar	    personalized_signature[person_infor_personalized_signature_max + 1];
		int32		birthday_year;
		int32		birthday_mouth;
		int32		birthday_day;
		xchar		city[person_infor_city_max + 1];
		xchar		hometown[person_infor_hometown_max + 1];
		xchar		online_state[person_infor_online_state_max + 1];
		int32		tags[person_information_tag_num_max];

		s_role_person_information()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		void set_gender(xstring m_gender)
		{
			memset(gender, 0, sizeof(gender));
			memcpy(gender, m_gender.c_str(), m_gender.size() > person_infor_gender_max ? person_infor_gender_max : m_gender.size());
		}
		void set_personalized_signature(xstring m_personalized_signature)
		{
			memset(personalized_signature, 0, sizeof(personalized_signature));
			memcpy(personalized_signature, m_personalized_signature.c_str(), m_personalized_signature.size() > person_infor_personalized_signature_max ? person_infor_personalized_signature_max : m_personalized_signature.size());
		}
		void set_city(xstring m_city)
		{
			memset(city, 0, sizeof(city));
			memcpy(city, m_city.c_str(), m_city.size() > person_infor_city_max ? person_infor_city_max : m_city.size());
		}
		void set_hometown(xstring m_hometown)
		{
			memset(hometown, 0, sizeof(hometown));
			memcpy(hometown, m_hometown.c_str(), m_hometown.size() > person_infor_hometown_max ? person_infor_hometown_max : m_hometown.size());
		}
		void set_online_state(xstring m_online_state)
		{
			memset(online_state, 0, sizeof(online_state));
			memcpy(online_state, m_online_state.c_str(), m_online_state.size() > person_infor_online_state_max ? person_infor_online_state_max : m_online_state.size());
		}
		void set_tags(int32 m_tags[])
		{
			for (int32 i = 0 ; i < person_information_tag_num_max;i++)
			{
				tags[i] = m_tags[i];
			}
		}
		void clear_tags()
		{
			for (int32 i = 0; i < person_information_tag_num_max; i++)
			{
				tags[i] = 0;
			}
		}
		void add_tag(int32 tag)
		{
			for (int32 i = 0; i < person_information_tag_num_max; i++)
			{
				if (tags[i] != 0)
				{
					continue;
				}
				tags[i] = tag;
				break;
			}
		}
	};

	enum e_competition_type
	{
		e_competition_type_ladder,	//天梯手册
		e_competition_type_element,	//元素手册
		e_competition_type_element_max,
	};
	struct s_role_competition_info
	{
	public:
		guid_64     role_guid;
		int32		have_type;
		int32		is_buy;//是否购买精英手册
		int32		buy_manual_type;
		int32		point_add_percent;
		int32		season;//赛季
		int32		competition_begin_time;//赛季开始时间
		int32		competition_end_time;//赛季结束时间
		int32		level;//等级
		int64		cur_exp;//当前经验		
		int32		need_reset;
		guid_64     assist_fight_guid;
		int32       reward_state[role_competition_tag_num];

		s_role_competition_info()
		{
			clear_data();
			point_add_percent = 0;
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		void clear_reward_data()
		{
			memset(reward_state, 0, sizeof(reward_state));
		}
		int32 get_reward_tag(int32 type,int32 idx)
		{
			int32 m_index = idx + role_competition_max * type;
			int32 array_idx = m_index / 32;
			int32 pos = m_index % 32;
			return ((reward_state[array_idx] & 1 << pos) != 0) ? 1 : 0;
		}
		void set_reward_tag(int32 type, int32 idx)
		{
			int32 m_index = idx + role_competition_max * type;
			int32 array_idx = m_index / 32;
			int32 pos = m_index % 32;
			reward_state[array_idx] = reward_state[array_idx] | 1 << pos;
		}
	};
	struct s_competition_time
	{
	public:
		int32   season;
		int32	begin_time;
		int32	end_time;
		s_competition_time()
		{
			clear_data();
			season = 0;
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct s_assist_fight_info
	{
	public:
		int32	    assist_fight_type;//助战类型  0：军团  1：全服
		guid_64		asssit_fight_guid;
		xchar		role_name[max_name_size + 1];
		int32		role_lv;
		int32       role_professional;
		int32       role_head_icon;
		int32		boss_id;
		guid_64     boss_guid;
		int32		map_id;
		int64		map_guid;
		s_map_pos   map_pos;
		guid_64     assist_guid_list[assist_fight_max_num];
		int32       assist_fight_tag;//0：boss  1：多人副本
		guid_64     legion_guid;
		int32       start_assist_time;//助战发起时间  用于客户端排序
		s_assist_fight_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		bool insert_assist_list(guid_64 role_guid)
		{
			for (int32 i = 0; i < assist_fight_max_num; i++)
			{
				if (assist_guid_list[i].is_valid() && assist_guid_list[i] == role_guid)
				{
					return true;
				}
			}
			for (int32 i = 0 ; i < assist_fight_max_num;i++)
			{
				if (!assist_guid_list[i].is_valid())
				{
					assist_guid_list[i] = role_guid;
					return true;
				}
			}
			return false;
		}
		void remove_assist_list(guid_64 role_guid)
		{
			for (int32 i = 0; i < assist_fight_max_num; i++)
			{
				if (assist_guid_list[i].is_valid() && assist_guid_list[i] == role_guid)
				{
					assist_guid_list[i].clear_data();
					break;
				}
			}
		}
		void set_info(const s_assist_fight_info & m_in_info)
		{
			asssit_fight_guid = m_in_info.asssit_fight_guid;
			memcpy(role_name, m_in_info.role_name, max_name_size);
			role_lv = m_in_info.role_lv;
			role_professional = m_in_info.role_professional;
			role_head_icon = m_in_info.role_head_icon;
			assist_fight_type = m_in_info.assist_fight_type;
			boss_id = m_in_info.boss_id;
			boss_guid = m_in_info.boss_guid;
			map_id = m_in_info.map_id;
			map_guid = m_in_info.map_guid;
			map_pos = m_in_info.map_pos;
			assist_fight_tag = m_in_info.assist_fight_tag;
			legion_guid = m_in_info.legion_guid;
		}
		void set_role_name(xstring temp_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, temp_name.c_str(), temp_name.size() > max_name_size ? max_name_size : temp_name.size());
		}
		bool to_proto(faith::st_proto::st_assist_fight_info* st_assist_ptr)
		{
			if (st_assist_ptr == nullptr)
			{
				return false;
			}

			st_assist_ptr->set_assist_fight_type(assist_fight_type);	  //助战类型  0：军团  1：全服
			st_assist_ptr->set_asssit_fight_guid(asssit_fight_guid.server_64);
			st_assist_ptr->set_role_name(role_name);
			st_assist_ptr->set_role_lv(role_lv);
			st_assist_ptr->set_role_professional(role_professional);
			st_assist_ptr->set_role_head_icon(role_head_icon);
			st_assist_ptr->set_boss_id(boss_id);
			st_assist_ptr->set_boss_guid(boss_guid.server_64);
			st_assist_ptr->set_map_id(map_id);
			st_assist_ptr->set_map_guid(map_guid);
			st_assist_ptr->add_unit_location(map_pos.unit_location.x);
			st_assist_ptr->add_unit_location(map_pos.unit_location.y);
			st_assist_ptr->add_unit_location(map_pos.unit_location.z);
			st_assist_ptr->add_unit_rotation(map_pos.unit_rotation.roll);
			st_assist_ptr->add_unit_rotation(map_pos.unit_rotation.pitch);
			st_assist_ptr->add_unit_rotation(map_pos.unit_rotation.Yaw);

			for (int32 i = 0 ; i < assist_fight_max_num; i++)
			{
				st_assist_ptr->add_assist_guid_list(assist_guid_list[i]);
			}
			st_assist_ptr->set_assist_fight_tag(assist_fight_tag);//0：boss  1：多人副本
			st_assist_ptr->set_legion_guid(legion_guid.server_64);
			st_assist_ptr->set_start_assist_time(start_assist_time);//助战发起时间  用于客户端排序

			return true;
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

		void from_proto(const faith::st_proto::st_assist_fight_info& st_assist_ref)
		{
			if (st_assist_ref.unit_location_size() < 3 || st_assist_ref.unit_rotation_size() < 3)
			{
				return;
			}

			assist_fight_type = st_assist_ref.assist_fight_type();	  //助战类型  0：军团  1：全服
			asssit_fight_guid.server_64 = st_assist_ref.asssit_fight_guid();
			my_memcopy_string(role_name, max_name_size, st_assist_ref.role_name());
			role_lv = st_assist_ref.role_lv();
			role_professional = st_assist_ref.role_professional();
			role_head_icon =st_assist_ref.role_head_icon();
			boss_id = st_assist_ref.boss_id();
			boss_guid.server_64 = st_assist_ref.boss_guid();
			map_id = st_assist_ref.map_id();
			map_guid= st_assist_ref.map_guid();

			map_pos.unit_location.x = st_assist_ref.unit_location(0);
			map_pos.unit_location.y = st_assist_ref.unit_location(1);
			map_pos.unit_location.z = st_assist_ref.unit_location(2);
			map_pos.unit_rotation.roll = st_assist_ref.unit_rotation(0);
			map_pos.unit_rotation.pitch = st_assist_ref.unit_rotation(1);
			map_pos.unit_rotation.yaw  = st_assist_ref.unit_rotation(2);

			for (int32 i = 0; i < assist_fight_max_num &&  i <  st_assist_ref.assist_guid_list_size(); i++)
			{
				assist_guid_list[i] = st_assist_ref.assist_guid_list(i);
			}
			assist_fight_tag = st_assist_ref.assist_fight_tag();//0：boss  1：多人副本
			legion_guid.server_64 = st_assist_ref.legion_guid();
			start_assist_time =  st_assist_ref.start_assist_time();//助战发起时间  用于客户端排序

		}
	};

	struct  s_assist_reward_info
	{
	public:
		guid_64 assist_fight_guid;
		bool is_main_player;
		int32 reward1;
		int32 reward2;
		int32 map_id;
		guid_64 helper_list[assist_fight_max_num];
		s_assist_reward_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		bool is_valid()
		{
			return assist_fight_guid.is_valid();
		}
		void add_helper(guid_64 helper_guid)
		{
			for (int32 i = 0; i < assist_fight_max_num; i++)
			{
				if (!helper_list[i].is_valid())
				{
					helper_list[i].A = helper_guid.A;
					helper_list[i].B = helper_guid.B;
					break;
				}
			}
		}
	};

	struct s_daemon_callback_info
	{
		int32 listen_port;
		int64 handle_index;

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		};

		s_daemon_callback_info()
		{
			clear_data();
		};
	};

	enum e_lucky_info_req_type
	{
		e_lucky_info_req_type_vec,
		e_lucky_info_req_type_special_info,
		e_lucky_info_req_type_buy_special_times,
	};

	enum e_competition_manual_type
	{
		e_competition_manual_type_none,
		e_competition_manual_type_normal,
		e_competition_manual_type_special,
		e_competition_manual_type_add_special,
	};
	enum e_competition_manual_get_reward_type
	{
		e_competition_manual_get_reward_type_none,
		e_competition_manual_get_reward_type_normal,
		e_competition_manual_get_reward_type_special,
		e_competition_manual_get_reward_type_all,
	};
	enum e_competition_manual_state
	{
		e_competition_manual_state_before_begining,
		e_competition_manual_state_begining,
		e_competition_manual_state_over,
	};

	enum e_assist_fight_type
	{
		e_assist_fight_type_legion,
		e_assist_fight_type_server,
		e_assist_fight_type_robot,
	};
	enum e_assist_fight_state_type
	{
		e_assist_fight_state_type_none,
		e_assist_fight_state_type_main_player,
		e_assist_fight_state_type_helper,
	};

	enum e_boss_damage_list_check_type
	{
		e_boss_damage_list_check_none,
		e_boss_damage_list_check_true,
		e_boss_damage_list_check_time_limit,
		e_boss_damage_list_check_level_limit,
	};

	enum e_pop_plot_tip_type
	{
		e_pop_plot_tip_type_str,
		e_pop_plot_tip_type_guide,
	};

	enum e_buy_tip_type
	{
		e_buy_tip_type_fuben,
		e_buy_tip_type_money,

	};

	enum e_login_promptbox_type
	{

		e_login_promptbox_type_meditation,					//冥想
		e_login_promptbox_type_resource_recovery,			//资源找回
		e_login_promptbox_type_timed_mounts,				//限时坐骑
		e_login_promptbox_type_timed_Wings,				     //限时翅膀
		e_login_promptbox_type_firstrecharge,				//首冲领取提示
		e_login_promptbox_type_vipexperiencecard,			//限时Vip卡到期
		e_login_promptbox_type_firstflush,					//首冲提示
		e_login_promptbox_type_combatpower,				    //战力礼包提示
		e_login_promptbox_type_experience,					//经验礼包提示
		e_login_promptbox_type_monthlycard,				    //月卡提示
		e_login_promptbox_type_fund,						//基金提示
		e_login_promptbox_type_exclusive,                   //至尊卡提示
		e_login_promptbox_type_support_for_promotion,       //冲级助力
		e_login_promptbox_type_max,						   
	};


	struct s_time_limit_gift_info
	{
		int32 template_id;													// 表Id
		int32 state_info;													// 进行状态
		int32 begin_time;													// 触发开始时间
		int32 end_time;														// 结束时间
		int32 trigger_num;													// 触发次数
		int32 buy_num;														// 购买次数

		s_time_limit_gift_info()
		{
			memset(this, 0, sizeof(*this));
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		bool is_valid()
		{
			return template_id > 0;
		}
	};

	struct s_time_limit_gift_db_info
	{
		guid_64					role_guid;
		s_time_limit_gift_info	_info;
		s_time_limit_gift_db_info()
		{
			memset(this, 0, sizeof(*this));
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	enum e_time_limit_gift_state
	{
		e_time_limit_gift_state_non,			// 未激活
		e_time_limit_gift_state_begin,			// 已激活
		e_time_limit_gift_state_end,			// 已结束
	};
	
	enum e_time_limit_gift_operation_type
	{
		e_time_limit_gift_operation_type_activate,		// 开始激活
		e_time_limit_gift_operation_type_buy,			// 开始购买
	};

	enum e_time_limit_gift_operation_end_type
	{
		e_time_limit_gift_operation_end_type_activate_succeed,	// 激活礼包成功
		e_time_limit_gift_operation_end_type_activate_fail,		// 激活礼包失败
		e_time_limit_gift_operation_end_type_template_error,	// 表格错误
		e_time_limit_gift_operation_end_type_buy_succeed,		// 购买成功
		e_time_limit_gift_operation_end_type_money_error,		// 购买错误 - 货币不足
		e_time_limit_gift_operation_end_type_item_error,		// 购买错误 - 物品创建失败
		e_time_limit_gift_operation_end_type_time_error,		// 领取错误 - 超出购买时间
		e_time_limit_gift_operation_end_typed_error_01,			// 领取错误 - 错误码01
	};

	struct s_subscribe_daily_info
	{
		int32				must_do_type;
		int32				info_state;
		s_subscribe_daily_info()
		{
			memset(this, 0, sizeof(*this));
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	enum e_subscribe_daily_info_state
	{
		e_subscribe_daily_info_state_open,
		e_subscribe_daily_info_state_end,
	};

	enum e_subscribe_daily_end_type
	{
		e_subscribe_daily_end_type_succeed,	// 成功
		e_subscribe_daily_end_type_have,	// 已经报名了
		e_subscribe_daily_end_type_not_can1,// 该活动无法报名
	};

	struct s_subscribe_daily_db_info
	{
		guid_64					role_guid;
		s_subscribe_daily_info	_info;
		s_subscribe_daily_db_info()
		{
			memset(this, 0, sizeof(*this));
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};



#pragma pack(pop)
}

#endif
