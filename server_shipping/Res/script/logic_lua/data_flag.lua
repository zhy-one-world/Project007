 local data_flag = {}

function data_flag:init( )
	-- body
end
e_data_flag_none =							0x0000000000000000;
e_data_flag_info =							0x0000000000000001;		--角色基本信息
e_data_flag_buff =							0x0000000000000002;		--角色Buff
e_data_flag_skill =							0x0000000000000004;		--角色技能

e_data_flag_time =							0x0000000000000008;		--玩家时间
e_data_flag_money =							0x0000000000000010;		--角色货币
e_data_flag_belief =						0x0000000000000020;		--角色信仰
e_data_flag_spirit =						0x0000000000000040;		--角色精灵
e_data_flag_mission =						0x0000000000000080;		--角色任务
e_data_flag_att =							0x0000000000000100;		--角色属性

e_data_flag_goods =							0x0000000000000200;		--角色购买记录
e_data_flag_map =							0x0000000000000400;		--地图记录
e_data_flag_service_goal =					0x0000000000000800;		--七日目标记录
e_data_flag_special_name =					0x0000000000001000;		--角色称号记录
e_data_flag_achievement =					0x0000000000002000;		--角色成就记录
e_data_flag_pokedex =						0x0000000000004000;		--角色图鉴记录
--const int64	e_data_flag_demons_tower =					0x0000000000008000;		-- 角色万魔塔记录
e_data_flag_mail =							0x0000000000010000;		--角色邮件记录
e_data_flag_tinder =						0x0000000000020000;		--角色火种记录
e_data_flag_worship =						0x0000000000040000;		--玩家崇拜记录
e_data_flag_yesterday_must_do_remain =		0x0000000000080000;		--昨天没做的必做任务
e_data_flag_convert =						0x0000000000100000;		--玩家转换记录
e_data_flag_item =							0x0000000000200000;	    --角色物品
e_data_flag_welfare =						0x0000000000400000;		--玩家福利
e_data_flag_active_degree =					0x0000000000800000;		--玩家活跃度

e_data_flag_title =							0x0000000001000000;		--玩家称号
e_data_flag_guide =							0x0000000002000000;		--新手引导
e_data_flag_fist_time_do =					0x0000000004000000;		--首次操作
--const int64	e_data_flag_skill_use =						0x0000000008000000;		-- 技能使用位置
e_data_flag_talent =						0x0000000010000000;		--角色天赋
e_data_flag_daily_must_do_count =			0x0000000020000000;		--每日必做次数记录
e_data_flag_highest_record =				0x0000000040000000;		--角色历史最高纪录

e_data_flag_auction_self_selling =			0x0000000080000000;		--交易行自己的出售物
e_data_flag_globel_mail_had =				0x0000000100000000;		--已拥有的全局邮件ws
e_data_flag_service_rank	=				0x0000000200000000;		--全民冲榜信息
e_data_flag_logic_info =					0x0000000400000000;		--玩家逻辑数据
e_data_flag_gain_treasure =					0x0000000800000000;		--打宝数据
e_data_flag_time_limit_activity =			0x0000001000000000;		--限时活动数据


e_data_flag_relation =						0x0000002000000000;		--角色关系ws
e_data_flag_boss_island =					0x0000004000000000;		--永恒岛数据
e_data_flag_chat =							0x0000008000000000;		--聊天数据ws
e_data_flag_passive_skill =					0x0000010000000000;		--被动技能
e_data_flag_seven_day_goal =				0x0000020000000000;		--七日目标前七天记录
e_data_flag_harry_record =					0x0000040000000000;		--跨服掠夺玩家记录




e_data_flag_phantom =                       0x0000080000000000;		--角色幻灵
e_data_flag_oracle_trial=					0x0000100000000000;		--天启试炼
e_data_flag_chat_record =                   0x0000200000000000;		
e_data_flag_player_information =			0x0000400000000000;		--个人信息
e_data_flag_recycle =                       0x0000800000000000;     --回归信息
e_data_flag_recycle_task =                  0x0001000000000000;     --回归任务
e_data_flag_player_competition =			0x0002000000000000;		--竞赛信息
e_data_flag_recycle_invited =               0x0004000000000000;		--回归招募
e_data_flag_dragontrip      =               0x0008000000000000;		--萌龙奇遇
e_data_flag_legion_skill	=			    0x0010000000000000;		--军团技能
e_data_flag_skytreasure =                   0x0020000000000000;		--天空宝库
e_data_flag_starark         =               0x0040000000000000;		--星海方舟





e_role_info_str = 
{
	[1] = "e_role_info_template_id",						--模板id
	[2] = "e_role_info_server_id",						--服务器id
	[3] = "e_role_info_img_id",							--头像
	[4] = "e_role_info_class_type",						--职业
	[5] = "e_role_info_class_branch",						--分支
	[6] =  "e_role_info_gender",							--角色性别
	[7] =   "e_role_info_exp_level",						--经验等级
	[8] =   "e_role_info_vip_level",						--vip等级
	[9] =   "e_role_info_vip_experience_level",				--vip体验等级
	[10] =  "e_role_info_recharge_num",						--充值点数
	[11] =  "e_role_info_main_map_id",						--大地图模板id
	[12] =  "e_role_info_main_pos_x",						--大地图坐标x
	[13] =	"e_role_info_main_pos_y",						--10大地图坐标y
	[14] =	"e_role_info_main_pos_z",						--大地图坐标z
	[15] =	"e_role_info_main_pos_yaw",						--大地图旋转
	[16] =	"e_role_info_main_line_id",						--15大地图线id
	[17] =	"e_role_info_move_map_id",						--要到达地图模板id
	[18] =	"e_role_info_move_pos_x",						--当前坐标x
	[19] =	"e_role_info_move_pos_y",						--当前坐标y
	[20] =	"e_role_info_move_pos_z",						--当前坐标z
	[21] =	"e_role_info_move_pos_yaw",						--旋转
	[22] =	"e_role_info_move_line_id",						--要到达地图线id
	[23] =	"e_role_info_GS_value",							--25当前战斗力值
	[24] =	"e_role_info_upgrade_id",						--等级模板id
	[25] =	"e_role_info_daily_done_num",					--日常完成数目
	[26] =	"e_role_info_against_done_num",					--讨伐完成数目
	[27] =	"e_role_info_wing_showd_template_id",			--30要显示的翅膀ID
	[28] =	"e_role_info_mount_showd_template_id",			--要显示的坐骑ID
	[29] =	"e_role_info_element_heart_drop_group",			--元素之心的抽取组
	[30] =	"e_role_info_arena_times",						--竞技场次数
	[31] =	"e_role_info_arena_vip_times",					--vip可挑战次数
	[32] =	"e_role_info_arena_vip_buy_times",				--35vip已购买次数
	[33] =	"e_role_info_arena_winning_streak",				--连胜纪录
	[34] =	"e_role_info_legion_donate_earn_by_money",		--军团捐献金币获得的战功
	[35] =	"e_role_info_legion_donate_earn_by_item",		--军团捐献物品获得的战功
	[36] =	"e_role_info_worship_other_time",				--玩家崇拜次数
	[37] =	"e_role_info_get_func_unlock_award_record",		--解锁奖励的领取
	[38] =	"e_role_info_vip_item_geted_level",				-- VIP礼包领取到第几级了
	[39] =	"e_role_info_vip_free_revive_time",				--免费复活次数
	[40] =	"e_role_info_element_heart_refine_time",		--元素之心提炼次数
	[41] =	"e_role_info_total_login_days",					--累计登陆天数
	[42] =	"e_role_info_continue_login_time",				--45连续登录次数
	[43] =	"e_role_info_daily_activity_degree",			--日活跃度
	[44] =	"e_role_info_daily_recharge_num",				--日充值数
	[45] =	"e_role_info_total_diamond_cost_num",			--累计消费钻石数
	[46] =	"e_role_info_daily_active_degree_get_flag",		--每日活跃奖励领取标志位
	[47] =	"e_role_info_fund_recharge_num",				--50成长基金，充值基金充值额
	[48] =	"e_role_info_fund_payment_flag",				--成长基金购买标记位
	[49] =	"e_role_info_fund_time_get_flag",				--成长基金，时光领取标记位
	[50] =	"e_role_info_fund_pride_get_flag",				--成长基金，充值领取标记位
	[51] =	"e_role_info_fund_reborn_get_flag",				--成长基金，重生领取标记位
	[52] =	"e_role_info_fund_cost_num",					--55成长基金，充值基金消费额
	[53] =	"e_role_info_lord_of_city_worship_time",		--城主膜拜次数
	[54] =	"e_role_info_lord_of_war_worship_time",			--PK之王膜拜次数
	[55] =	"e_role_info_lord_of_sanctuary_worship_time",	--圣域之主膜拜次数
	[56] =	"e_role_info_lord_of_god_worship_time",			--众神之王膜拜次数
	[57] =	"e_role_info_hope_value",						--祈福祝福值
	[58] =	"e_role_info_del_time",							--删除时间
	[59] =	"e_role_info_kill_player_num",					--击杀玩家数量
	[60] =	"e_role_info_spirit_value",						--精灵猎取保底
	[61] =	"e_role_info_watch_count",						--查看的最大人数
	[62] =	"e_role_info_crystal_dreamland_chests",			--水晶幻境采集宝箱个数
	[63] =	"e_role_info_protect_mark_slot_open_num",		--守护印记槽位开启个数
	[64] =	"e_role_info_element_heart_slot_open_num",		--元素之心槽位开启个数
	[65] =	"e_role_info_sprite_equip_slot_open_num",		--精灵装备槽位开启个数
	[66] =	"e_role_info_exclusive_flag",
	[67] =	"e_role_info_red_package_geted_times",			--每日红包领取次数
	[68] =	"e_role_info_free_red_package_send_times",		--每日免费发红包次数
	[69] =	"e_role_info_world_boss_chests",				--世界BOSS掉落宝箱采集
	[70] =	"e_role_info_leave_map_id",						--要离开的地图id
	[71] =	"e_role_info_assist_chests",					--助战宝箱采集
	[72] =	"e_role_info_next_side_mission_main_trigger_id",	--下个待触发支线任务的主线任务ID
	[73] =	"e_role_info_show_fashion",						--是否显示时装
	[74] =	"e_role_info_real_recharge_jewel_num",			--冲入钻石数主要用于充值榜
	[75] =	"e_role_info_relic_kill_num",					--上古遗迹今日杀怪数
	[76] =	"e_role_info_relic_exp_num",					--上古遗迹今日获得经验数
	[77] =	"e_role_info_head_frame",						--头像框
	[78] =	"e_role_info_fund_pride_cost_value",			--成长基金消费点数
	[79] =	"e_role_info_tinder_prop_slot_open_num",			--火种道具槽位开启个数
	[80] =	"e_role_info_show_sky_suit_shape", --是否显示天空套装外形
	[81] =	"e_role_info_move_server_id",					--要到达服务器
	[82] =	"e_role_info_move_war_idex",					--要到达地图位置
	[83] =	"e_role_info_goddess_slot_flag",				--女神协战槽位开启标记
	[84] =	"e_role_info_daily_cost",						--每日消费数值
	[85] =	"e_role_info_marry_done_num",				--结婚任务每日完成次数
	[86] =	"e_role_info_marry_dati_id",				--当前结婚答题id
	[87] =	"e_role_info_marry_dati_num",				--结婚答题完成数量
	[88] =	"e_role_info_marry_dati_right_num",			 --结婚答题答对数量
	[89] =	"e_role_info_marry_dati_current_answer",	 --结婚答题当前答案
	[90] =	"e_role_info_legion_dance_exp_num",			--军团共舞今日经验获取次数
	[91] =	"e_role_info_legion_donate_num",			--军团捐赠次数
	[92] =	"e_role_info_appearance_id",				--捏脸外形ID
	[93] =	"e_role_info_is_show_vip",						--是否显示vip
}


e_role_i64_info_str  =
{
	[1]	= "e_role_i64_info_gs_value",				--战力
	[2] = "e_role_i64_info_history_best_gs_value",--历史最高战力
	[3]	= "e_role_i64_info_zero_mount_up_start_value", --零点刷新坐骑提升榜初始值
	[4]	= "e_role_i64_info_zero_wing_up_start_value", --零点刷新翅膀提升榜初始值
	[5]	= "e_role_i64_info_zero_equip_up_start_value", --零点刷新装备提升榜初始值
	[6]	= "e_role_i64_info_zero_spirit_up_start_value", --零点刷新精灵提升榜初始值
};




e_ws_flag_none =										    0x0000000000000000;
e_ws_flag_big_player =										0x0000000000000001;--名人堂
e_ws_flag_cs_connect =										0x0000000000000002;--CS连接成功
e_ws_flag_gm_common =										0x0000000000000004;--GM指令load完毕
e_ws_flag_harry =											0x0000000000000008;--跨服掠夺
e_ws_flag_cross_pk =										0x0000000000000010;--跨服职业竞技
e_ws_flag_overload_war =									0x0000000000000020;--跨服霸主战
e_ws_flag_city_war =										0x0000000000000040;--跨服城战
e_ws_flag_cross_boss =										0x0000000000000080;--跨服BOSS
e_ws_flag_server_cross_time =								0x0000000000000100;--服务器跨服时间
e_ws_flag_server_refresh =									0x0000000000000200;--服务器server_refresh表中的数据，包含各种刷新时间以及服务器等级
e_ws_flag_time_limit_info =									0x0000000000000400;--限时活动数据
e_ws_flag_time_limit_template =								0x0000000000000800;--限时活动表格数据
e_ws_flag_ranking_info =									0x0000000000001000;--排行榜数据
e_ws_flag_load_legion_info =								0x0000000000002000;--军团数据
e_ws_flag_cross_server_state =								0x0000000000004000;--跨服状态
e_ws_flag_init_time_limit_template =						0x0000000000008000;--限时活动表格数据初始化完成
e_ws_flag_load_cloud_shop_info =							0x0000000000010000;--云购数据读取
e_ws_flag_fep_connect =										0x0000000000020000;--fep连接成功







g_data_flag = data_flag


return data_flag
