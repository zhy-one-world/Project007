#pragma once
 
#include "logic/type_def.hpp"
#include "template_base.h"
 
namespace faith
{
	//////////////////////////////////item提示信息////////////////////////////////////////
	const int32 e_item_string_bind = 90028001;									// 已绑定
	const int32 e_item_string_bind_no = 90028002;								// 未绑定
	const int32 e_item_string_upgrade = 90028003;								// "+","追"
	const int32 e_item_string_mount_max_high = 90028004;						// 已达坐骑最大飞行高度
	const int32 e_item_string_push_upgrade = 90010077;							// 请放入要强化的装备
	const int32 e_item_string_exchange_success = 90090122;						// 兑换成功

	//////////////////////////////////item错误信息////////////////////////////////////////
	const int32 e_item_string_unkown = 90029001;								// 未知错误
	const int32 e_item_string_succeed = 90029002;								// 成功
	const int32 e_item_string_can_not_use = 90029003;							// 不能使用
	const int32 e_item_string_no_item = 90029004;								// 没有该物品
	const int32 e_item_string_no_target = 90029005;								// nTargetRoleID没有找到
	const int32 e_item_string_dead = 90029006;									// 死亡状态下无法使用
	const int32 e_item_string_spelling = 90029007;								// 正在执行另一个动作
	const int32 e_item_string_cd = 90029008;									// cd未好
	const int32 e_item_string_in_combat = 90029009;								// 战斗状态下无法使用
	const int32 e_item_string_quest = 90029010;									// 任务物品使用错误
	const int32 e_item_string_target = 90029011;								// 目标错误
	const int32 e_item_string_regin = 90029012;									// 没有在指定区域内使用
	const int32 e_item_string_level = 90029013;									// 未满足等级要求
	const int32 e_item_string_class = 90029014;									// 未满足职业要求
	const int32 e_item_string_camp = 90029015;									// 未满足阵营要求
	const int32 e_item_string_race = 90029016;									// 未满足种族要求
	const int32 e_item_string_gender = 90029017;								// 未满足性别要求
	const int32 e_item_string_spell_learn = 90029018;							// 不能学习该技能
	const int32 e_item_string_specialty_learn = 90029019;						// 不能学习该生活技能
	const int32 e_item_string_pet_spell_learn = 90029020;						// 指定兵魂不能学习该技能
	const int32 e_item_string_compse_learn = 90029021;							// 不能学习配方
	const int32 e_item_string_not_identified = 90029022;						// 物品需要鉴定后才能使用
	const int32 e_item_string_not_enough_item = 90029023;						// 所需合成物品缺少
	const int32 e_item_string_target_too_far = 90029024;						// 目标太远
	const int32 e_item_string_cannot_open_gift_bag = 90029025;					// 背包满无法打开礼品包
	const int32 e_item_string_cannot_use_qust_point_bag = 90029026;				// 任务点数达到上限无法增加任务点
	const int32 e_item_string_cannot_use_prestige_bag = 90029027;				// 势力声望达到最大值无法增加
	const int32 e_item_string_cannot_use_presting_bag = 90029028;				// 无法完成物品合成操作，背包空间不足
	const int32 e_item_string_no_catch_pet_skill = 90029029;					// 还没有学习吸魂技能,不能使用兵魂栏位扩充道具
	const int32 e_item_string_all_pet_slot_opened = 90029030;					// 所有的兵魂栏位已被全部打开
	const int32 e_item_string_error_metemp_sychosis_times = 90029031;			// 错误的转生次数
	const int32 e_item_string_spell_learned = 90029032;							// 已经学会了
	const int32 e_item_string_success_hc = 90029033;							// 使用道具合成
	const int32 e_item_string_pk_cannot_use_item1 = 90029034;					// 此物品不需使用
	const int32 e_item_string_item_is_using = 90029035;							// 物品正在使用中，操作失败
	const int32 e_item_string_invalid = 90029036;								// 原堆物品不合法
	const int32 e_item_string_new_position = 90029037;							// 新位置错误
	const int32 e_item_string_bag_capacity = 90029038;							// 背包容量变化非法
	const int32 e_item_string_never_can_be_discard = 90029039;					// 物品不能销毁
	const int32 e_item_string_ca_not_enough = 90029040;							// 战力值不足
	const int32 e_item_string_noneed_item = 90029041;							// 使用失败，缺少必要的物品
	const int32 e_item_string_script_trigger_error = 90029042;					// 脚本函数返回错误
	const int32 e_item_string_peer_level = 90029043;							// 贵族等级不足
	const int32 e_item_string_vip_level = 90029044;								// vip等级不足
	const int32 e_item_string_yuanbao = 90029045;								// 玩家元宝不足
	const int32 e_item_string_yuanbao_bind = 90029046;							// 玩家绑定元宝不足
	const int32 e_item_string_inherit = 90029047;								// 传承失败
	const int32 e_item_string_advance = 90029048;								// 进阶失败
	const int32 e_item_string_upgrade_no_down = 90029049;						// 强化失败,不降级
	const int32 e_item_string_upgrade_down = 90029050;							// 强化失败,降级
	const int32 e_item_string_item_use_in_cd = 90029051;						// 道具CD中，无法使用
	const int32 e_item_string_money = 90029052;									// 金币不足
	const int32 e_item_string_jewel = 90029053;									// 钻石不足
	const int32 e_item_string_tinder_point_not_enough = 90095310;				// 火种点数不足
	const int32 e_item_string_full_level = 90029054;							// 等级已满
	const int32 e_item_string_matirial_not_enough = 90029055;					// 素材不足
	const int32 e_item_string_chant_show_unactivate_wing = 90029514;			// 无法启用未激活翅膀
	const int32 e_item_string_had_this_shape_wing = 90029515;					// 已拥有此外形翅膀
	const int32 e_item_string_wing_block_full = 90029516;						// 翅膀栏已满
	const int32 e_item_string_auto_use_money_not_enough = 90305019;				// 自动购买材料的钱币不足
	const int32 e_item_string_warlord_mount_cant_upgrade = 90305020;			// 城主坐骑无法升级
	const int32 e_item_string_unlock_requirement_not_reach = 90305021;			// 解锁条件未满足
	const int32 e_item_string_cant_unlock_again = 90305022;						// 不能重复解锁
	const int32 e_item_string_have_no_illusion = 90305023;						// 无此幻化
	const int32 e_item_string_cant_show_unlock_mount = 90305024;				// 无法启用未解锁坐骑
	const int32 e_item_string_mount_unlock = 90305025;							// 此坐骑未解锁
	const int32 e_item_string_mount_illusion_unlock = 90305026;					// 此坐骑幻化未解锁
	const int32 e_item_string_cant_free_get_hope_item = 90305003;				// 免费祈福时间没到
	const int32 e_item_string_bag_full = 90305038;								// 背包已满
	const int32 e_item_string_storage_full = 90209300;							// 仓库已满
	const int32 e_item_string_hopbag_full = 90090136;							// 祈愿仓库已满
	const int32 e_item_element_heart_refind_one_no_time = 90090195;				// 元素之心剩余提炼次数不足,1次
	const int32 e_item_element_heart_refind_ten_no_time = 90090196;				// 元素之心剩余提炼次数不足,10次
	const int32 e_item_get_new_wing_by_card = 90305049;							// 通过翅膀卡获得新翅膀
	const int32 e_item_inherit_point_lose_notice = 90029619;					// 自动传承培养点数损失提示
	const int32 e_item_rear_item_get_notice = 90300020;							// 稀有物品（红色）获得提示
	const int32 e_item_sprite_equip_slot_full = 90305006;						// 精灵装备槽已满
	const int32 e_item_add_on_failed = 90090682;								// 追加失败
	const int32 e_item_string_not_belong_level = 90091304;					// 只可购买当前人物对应阶段的药水
	const int32 e_buff_cover_confirm = 90090512;								// buff覆盖确认
	const int32 e_buff_has_stronger = 90090513;									// 有更强效果了
	const int32 e_buff_has_same = 90090514;										// 有同类效果了

	const int32 e_system_recruit_need_legion = 90090597;						// 军团招募需要军团
	const int32 e_item_tinder_star_level_not_enough = 90202996;					// 火种星级不足
	const int32 e_item_tinder_skill_slot_not_enough = 90096416;					// 火种技能槽位不足

	///////////////////////////////物品获得提示前缀///////////////////////////////////////////
	//const int32 e_item_get_first_string_weapon = 90305001;						// 武器
	//const int32 e_item_get_first_string_armor = 90305001;						// 防具
	//const int32 e_item_get_first_string_mount = 90305001;						// 坐骑
	//const int32 e_item_get_first_string_expendable = 90305001;					// 消耗品
	//const int32 e_item_get_first_string_stone = 90305001;						// 宝石
	//const int32 e_item_get_first_string_fashion = 90305001;						// 时装
	//const int32 e_item_get_first_string_quest = 90305001;						// 任务
	//const int32 e_item_get_first_string_spirit = 90305001;						// 精灵
	//const int32 e_item_get_first_string_wing = 90305002;						// 翅膀
	const int32 e_item_get_first_string_common = 90090227;						// 通用

	////////////////////////////////////////转职/////////////////////////////////////////////////
	const int32 e_item_gradeup_use_item_not_enough = 90096509;				   // 突破道具不足
	const int32 e_item_gradeup_use_level_not_enough = 90096510;				   // 未达到突破等级
	const int32 e_item_gradeup_use_mission_not_enough = 90096511;			  // 需要先完成突破任务

///////////////////////////////邮件操作结果提示///////////////////////////////////////////
	const int32 e_mail_bag_full_cant_get = 90303001;							// 背包已满无法提取（单个领取用）
	const int32 e_mail_element_bag_full_cant_get = 90096110;					// 符文背包已满无法提取（单个领取用）
	const int32 e_mail_get_success = 90305004;									// 提取成功
	const int32 e_mail_force_delete_confirm = 90303003;							// 无附件邮件已删除，剩余邮件中有未领取奖励，是否全部删除
	const int32 e_mail_delete_sucess = 90303004;								// 邮件已删除
	const int32 e_mail_no_selected_mail = 90303005;								// 无选中邮件
	const int32 e_mail_no_item_or_money = 90303006;								// 无附件
	const int32 e_mail_not_exist = 90303007;									// 邮件不存在
	const int32 e_mail_bag_full_cant_continue_get = 90305005;					// 背包已满无法继续提取（自动领取用）
	const int32 e_mail_has_no_readed_mail = 90303008;							// 无已读邮件

///////////////////////////////聊天提示///////////////////////////////////////////
	const int32 e_chat_no_in_player = 90081001;									// 请输入私聊玩家
	const int32 e_chat_is_empty = 90081002;										// 消息为空
	const int32 e_chat_cant_find_player = 90081003;								// 私聊对象不在线或名称错误
	const int32 e_chat_illegal_string = 90081004;								// 含有非法字符
	const int32 e_chat_cant_send_chat = 90096617;								// 还不能发言
	const int32 e_chat_is_baning = 90081006;									// 已被禁言
	const int32 e_chat_player_cant_receive_personal_chat = 90081007;			// 该玩家不接受私聊
	const int32 e_chat_player_state_cant_chat = 90081008;						// 该玩家状态无法私聊
	const int32 e_chat_player_no_team = 90081009;								// 该玩家没队伍
	const int32 e_chat_player_no_legion = 90081010;								// 该玩家没军团
	const int32 e_chat_too_long = 90081011;										// 消息太长了
	const int32 e_chat_sender_no_legion = 90090419;								// 没有军团
	const int32 e_chat_my_position_text = 90090565;								// 我的位置
	const int32 e_chat_open_widget = 90090910;									// 打开界面
	const int32	e_chat_near_channel_level_limit = 90090687;						// 角色达到某某等级可在附近频道发言
	const int32	e_chat_world_channel_level_limit = 90090688;					// 角色达到某某等级可在世界频道发言
	const int32	e_chat_radio_host_channel_level_limit = 90091005;				// 角色达到某某等级可在主播频道发言
	const int32 e_chat_is_same_str = 90096616;									// 相同字符串不允许发送
	const int32 e_chat_voice_message_forbidden_world = 90096659;				// 语音消息包含敏感字符 不能发送

	const int32 e_chat_fight_power_req = 90090566;								// 战力需求
	const int32 e_chat_fight_power_no_req = 90090567;							// 无战力需求
	const int32 e_chat_level_no_limit = 90090568;								// 无限制
	const int32 e_chat_hunt_recruit = 90090569;									// 狩猎开组
	const int32 e_chat_fuben_recruit = 90090570;								// 副本开组

	const int32 e_chat_addressee_offline = 90300073;								//信息无法送达，对方不在线

	const int32 e_chat_line_str = 90091025;										// 线
	////////////////////////////////////////VIP提示/////////////////////////////////////////////
	const int32 e_vip_this_level_not_open = 90090186;							// VIP等级未开启
	const int32 e_vip_item_alread_get = 90090187;								// VIP礼包已领取过
	const int32 e_vip_open_daily_mission_limit = 90090193;						// VIP开启日常任务一键完成等级提示
	const int32 e_vip_open_area_transfer = 90090194;							// VIP开启区域传送等级提示
	const int32 e_vip_item_get_sucess = 90090188;								// VIP礼包领取成功
	const int32 e_recharge_sucess = 90090189;									// 充值成功

	////////////////////////////////////////福利提示/////////////////////////////////////////////
	const int32 e_welfare_no_empty_slot = 90090165;								// 福利，包格子不够
	const int32 e_welfare_cant_get = 90090197;									// 福利，未满足领取条件
	const int32 e_welfare_already_geted = 90090198;								// 福利，已领取过
	const int32 e_welfare_replacement_success = 90090167;						// 福利，月卡补领成功

	///////////////////////////////////////地图提示//////////////////////////////////////////////////
	const int32 e_map_exp_level_cant = 90090228;								// 地图，等级不足，无法进入
	const int32 e_map_ancient_map_no_time = 90090229;							// 地图，古战场时间已耗尽
	const int32 e_raid_map_leave_prompt = 90095171;								// 地图  确定传出将离开副本 丢失副本进度 是否传出

	///////////////////////////////////////世界Boss提示//////////////////////////////////////////////////
	const int32 e_world_boss_create_1 = 90090339;								// 已经刷新在了
	const int32 e_world_boss_create_2 = 90090340;								// 线，勇士们请速去击杀
	const int32 e_world_boss_killed_1 = 90090341;								// 已在
	const int32 e_world_boss_killed_2 = 90090342;								// 线被
	const int32 e_world_boss_killed_3 = 90090343;								// 击杀

	///////////////////////////////////////成长基金提示//////////////////////////////////////////////////
	const int32 e_fund_already_purchased = 90090400;							// 不能重复购买
	const int32 e_fund_money_not_enough = 90090401;								// 货币不足
	const int32 e_fund_already_geted = 90090402;								// 已领取过
	const int32 e_fund_cant_get = 90090403;										// 未满足领取条件
	const int32 e_fund_not_purchase = 90090404;									// 没买
	const int32 e_fund_purchase_success = 90090405;								// 购买基金成功

	///////////////////////////////////////称号提示//////////////////////////////////////////////////
	const int32 e_title_have_geted = 90090336;									// 称号，已经拥有该永久性称号，无法使用
	const int32 e_title_get_title = 90090227;									// 称号，恭喜获得:

	///////////////////////////////////////膜拜提示//////////////////////////////////////////////////
	const int32 e_worship_gold_not_enough = 90090415;							// 膜拜，金币不足
	const int32 e_worship_diamond_not_enough = 90090416;						// 膜拜，钻石不足
	const int32 e_worship_has_worship = 90090417;								// 膜拜，膜拜次数用尽
	const int32 e_worship_success = 90090418;									// 膜拜，膜拜成功

	//////////////////////////////////////商店/////////////////////////////////////////////////////
	const int32 e_buy_goods_end_goods_num_is_zero = 90029522;						// 购买物品数量为0
	const int32 e_buy_goods_end_role_not_in_legion = 90090419;						// 玩家不在军团中
	const int32 e_buy_goods_end_legion_level_limit = 90090420;						// 军团等级不足
	const int32 e_buy_goods_end_state_num_limit = 90029521;							// 售卖数量已达上限
	const int32 e_buy_goods_end_state_bag = 90090165;								// 背包格子不够
	const int32 e_buy_goods_end_need_item_not_enough = 90029524;					// 所需前置物品不足
	const int32 e_buy_goods_end_state_money = 90029519;								// 货币不足
	const int32 e_buy_goods_end_success = 90029517;									// 购买成功
	const int32 e_buy_goods_end_need_contribution_not_enough = 90029526;			// 贡献度不足
	const int32 e_buy_goods_end_in_time_limit = 90200900;							// 不在限时购买时间


	const int32 e_skill_unactivate = 90069110;										// 技能未解锁
	const int32 e_mount_unactivate = 90069111;										// 坐骑未解锁
	const int32 e_pk_mode_cant_change_back = 90090519;								// 不能切换PK模式的后半段（前半段是地图名）

	//////////////////////////////////////红包/////////////////////////////////////////////////////
	const int32 e_red_bag_send_success = 90090549;									// 红包发送成功
	const int32 e_red_bag_no_more_space = 90090550;									// 服务端红包容量已满
	const int32 e_red_bag_refund_mail_text = 90090551;								// 红包发送失败退款邮件正文
	const int32 e_red_bag_no_more_free = 90090552;									// 免费红包次数已耗尽
	const int32 e_red_bag_alread_done = 90090553;									// 红包已被领光
	const int32 e_red_bag_get_success = 90090554;									// 红包领取成功
	const int32 e_red_bag_cant_draw_again = 90090557;								// 红包不可重复领取
	const int32 e_red_bag_get_legion_success = 90095205;							// 军团红包发送成功
	const int32 e_red_bag_get_globel_success = 90095206;							// 全服红包发送成功
	const int32 e_red_bag_get_vip_success = 90095220;								// VIP红包发送成功


	//////////////////////////////////////竞技场/////////////////////////////////////////////////////
	const int32 e_arena_match_success = 90090573;									// 匹配成功
	const int32 e_arena_no_player = 90090574;										// 玩家不存在
	const int32 e_arena_challenge_times_not_enough = 90090575;						// 挑战次数不足
	const int32 e_arena_challenge_is_colling = 90090576;							// 挑战冷却中
	const int32 e_arena_target_is_challenging = 90090577;							// 该玩家正在挑战中
	const int32 e_arena_target_pos_is_changed = 90090580;							// 该玩家排名已改变，请刷新后重试
	const int32 e_arena_self_is_challenging = 90095251;								// 自己正在被挑战中
	const int32 e_arena_self_pos_is_changed = 90095250;								// 自己排名已改变，请刷新后重试
	const int32 e_arena_mopping_no_times = 90305028;								// 次数已满无法扫荡


	//////////////////////////////////////元素之心/////////////////////////////////////////////////////
	const int32 e_element_heart_no_pound = 90305045;								// 元素粉末不足
	const int32 e_element_heart_equip_same_type = 90305046;							// 装备相同类型
	const int32 e_element_heart_equip_full = 90305047;								// 装备位已满
	const int32 e_element_heart_summon_high = 90305048;								// 抽取组数过高
	const int32 e_element_heart_no_summon_num = 90305050;							// 召唤次数用尽
	const int32	e_element_heart_bag_full = 90090917;								// 元素之心背包已满

	//////////////////////////////////////信仰系統/////////////////////////////////////////////////////
	const int32 e_belief_string_ji_huo_fail = 90060059;								// 激活失敗
	const int32 e_belief_string_ji_huo_succeed = 90060058;							//激活成功

	//////////////////////////////////////职业名称/////////////////////////////////////////////////////
	const int32 e_class_type_string_all = 90060040;										// 全职业
	const int32 e_class_type_string_barserker = 90060041;									// 战士
	const int32 e_class_type_string_wizard = 90060042;										// 法师
	const int32 e_class_type_string_guardian = 90060043;									// 骑士
	const int32 e_class_type_string_assassinator = 90060044;								// 刺客
	const int32 e_class_type_string_ranger = 90060045;										// 游侠

	//////////////////////////////////////图鉴/////////////////////////////////////////////////////
	const int32 e_psyche_cant_sell_before_submit_finish = 90090653;							// 精魄在回收够数前不得出售

	const int32 e_active_has_not_start = 90090360;										// 活动尚未开启

	//////////////////////////////////////离线语音提示/////////////////////////////////////////////////////
	const int32 e_voice_record_time_not_enough = 90090831;								// 语音时间不足1s 发送失败
	const int32 e_voice_record_time_over = 90090832;									// 语音时间超过15s 自动发送
	const int32 e_voice_user_canel = 90090839;											// 手指滑开 语音发送取消
	const int32 e_voice_not_valid = 90090835;											// 未检测到有效声音 发送失败
	const int32 e_voice_send_too_fast = 90090841;										// 语音发送过快 请稍等片刻
	const int32 e_world_message_send_too_fast = 90095172;										// 消息发送过快  请 s后再试

	//////////////////////////////////////交易行/////////////////////////////////////////////////////
	const int32 e_auction_sell_success = 90095107;										// 上架成功
	const int32 e_auction_sell_failed = 90095108;										// 上架失败
	const int32 e_auction_sell_slot_full = 90095109;									// 出售槽已满
	const int32 e_auction_sell_finish_success = 90095100;								// 物品已成功售出

	const int32 e_auction_purchase_failed = 90095101;									// 购买失败
	const int32 e_auction_purchase_already_purchased = 90095106;						// 物品已售出
	const int32 e_auction_purchase_success = 90095099;									// 购买成功

	const int32 e_auction_cancel_sell_already_sell = 90095116;							// 取消失败已售出
	const int32 e_auction_cancel_sell_success = 90095120;								// 取消成功

	const int32 e_auction_bid_money_changed = 90095149;									// 竞标价格变动
	const int32 e_auction_bid_success = 90095148;										// 竞标成功
	const int32 e_auction_bid_are_overtaken = 90300214;									// 竞标被超过

	//////////////////////////////////////状态切换限制/////////////////////////////////////////////////////
	const int32 e_equip_off_mount_limite = 90095246;									// 坐骑解除限制
	const int32 e_equip_off_wing_limite = 90095247;										// 羽翼解除限制

	//////////////////////////////////////状态切换限制/////////////////////////////////////////////////////
	const int32 e_target_cannot_path_find_toptip = 90090999;							// 目标位置无法自动寻路 请手动前往
	const int32 e_path_finding = 90091000;												// 自动寻路中
	const int32 e_target_cant_path_find_mp_fly_up = 90091001;							// 自动寻路中 目标位置无法到达			起飞受阻下方提示
	const int32 e_target_cant_path_find_mp_fly_down = 90091022;							// 自动寻路中 目标位置无法到达			下降受阻下方提示
	const int32 e_setting_level_cannot_lower_than_target = 90091002;					// 设置等级不能低于当前等级
	const int32 e_follow_captain_can_not_do_this = 90091002;							// 正在跟随队长 无法进行该操作

	//////////////////////////////////////切线/////////////////////////////////////////////////////
	const int32 e_is_chane_line_cannot_do_this = 90091024;								// 正处于切换线路中 无法这样做
	const int32 e_not_same_line_need_change_line = 90070061;							// 目标所处不在当前线路 是否切换线路
	const int32 e_team_call_boss_not_same_one_tip = 90070058;							// 目标所在地为混乱之地 是否更换到混乱之地


	//////////////////////////////////////交互/////////////////////////////////////////////////////
	const int32 e_target_not_in_range = 90091024;										// 目标不在范围内
	const int32 e_target_can_not_accept_request = 90070061;								// 对方暂时无法接收邀请
	const int32 e_ride_state_can_not_interaction = 90070058;							// 骑坐骑状态下不能交互
	const int32 e_can_not_path_find_to_target = 90091024;								// 无法正常寻路至目标
	const int32 e_interaction_state_can_not_do_this = 90070061;							// 交互状态下不能这么做
	const int32 e_double_state_can_not_do_this = 90070058;								// 双骑状态下不能这么做
	const int32 e_interaction_cd_please_wait = 90091024;								// 邀请冷却中 请稍后再试
	const int32 e_target_is_accept_other_request = 90070061;							// 对方正在接收他人邀请
	const int32 e_target_reject_request = 90070058;										// 对方拒绝邀请
	const int32 e_target_not_notice_your_request = 90091024;							// 对方未注意你的邀请
	const int32 e_can_not_send_request = 90070061;										// 无法发送邀请

	//////////////////////////////////////语音/////////////////////////////////////////////////////
	const int32 e_agora_in_other_channel_can_not_change = 90091134;						// 在其他语音聊天中，无法切换
	const int32 e_agora_someone_join_channel = 90091135;								// 某人加入了实时语音
	const int32 e_agora_someone_leave_channel = 90091136;								// 某人离开了实时语音
	const int32 e_agora_in_voice_can_not_do_it = 90091137;								// 聊天中不能这样做
	const int32 e_agora_inteive_join_legion_channel_noteice = 90091138;					// 是否加入军团频道实时语音
	const int32 e_agora_inteive_join_team_channel_noteice = 90091139;					// 是否加入队伍频道实时语音
	const int32 e_agora_channel_become_radio = 90202807;								// 已将您语音频道切换为主播频道
	const int32 e_agora_channel_become_legion = 90202808;								// 已将您语音频道切换为军团频道
	const int32 e_agora_channel_become_team = 90202809;									// 已将您语音频道切换为队伍频道
	//////////////////////////////////////其他/////////////////////////////////////////////////////
	const int32 e_operate_too_fast = 90095223;											// 操作过快
	const int32 e_copy_finish = 90305344;												// 已复制
	const int32 e_paste_finish = 90305613;												// 已粘贴
} 