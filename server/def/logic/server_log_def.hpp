/********************************************************************
created:	2014/08/01
created:	01:08:2014   12:45
file base: player_def
file ext:	hpp
author:	Locke

purpose:	
*********************************************************************/
#include <cassert>
#ifndef _SERVER_LOG_DEF_HPP_
#define _SERVER_LOG_DEF_HPP_

namespace faith
{

	const int32 max_log_char_num = 4096;							// log日志最大长度 
	const int32 max_log_int64_size = 21;							// log日志最大长度
	static const char server_log_separator = 0x01;
	static const std::string server_log_normversion = "1.1.4";
	static const std::string server_invaild_string = "null";
	static const std::string server_common_head = "loghead";

	enum e_server_log_cut_money
	{
		e_server_log_cut_money_grade_up, //转生
		e_server_log_cut_money_back_to_life, //复活
		e_server_log_cut_money_unlock_bag_slot, //解锁背包格子
		e_server_log_cut_money_legion_donate, //军团捐献
		e_server_log_cut_money_resource_back, //资源找回
		e_server_log_cut_money_grow_up_fund, //成长基金
		e_server_log_cut_money_red_package, //红包
		e_server_log_cut_money_worship_player, //膜拜
		e_server_log_cut_money_learn_skill, //学习技能
		e_server_log_cut_money_upgrade_skill, //升级技能
		e_server_log_cut_money_send_mail_content, //通过邮件发钱
		e_server_log_cut_money_convert_main_ui, //主界面的转换
		e_server_log_cut_money_convert_in_bag, //背包里的转换
		e_server_log_cut_money_patron_add_star, //图鉴守护神升星
		e_server_log_cut_money_patron_upgrade, //图鉴守护神升级
		e_server_log_cut_money_belief_head_up, //信仰点亮
		e_server_log_cut_money_belief_upgrade_up, //信仰升级
		e_server_log_cut_money_create_legion, //创建军团
		e_server_log_cut_money_get_banquet_reward, //舞会领奖
		e_server_log_cut_money_create_banquet, //举办舞会
		e_server_log_cut_money_legion_buff, //军团buff
		e_server_log_cut_money_legion_rm_recruit_cd, //军团清除世界招募cd
		e_server_log_cut_money_welfare_replacement, //补领福利
		e_server_log_cut_money_map_over_gift, //副本完成奖励
		e_server_log_cut_money_sprite_upgrade, //j精灵升级
		e_server_log_cut_money_sprite_hunt_one, //精灵单个猎取
		e_server_log_cut_money_sprite_hunt_ten, //精灵十连猎取
		e_server_log_cut_money_meditation, //冥想
		e_server_log_cut_money_arena_challenge, //竞技场付费挑战
		e_server_log_cut_money_finish_mission, //完成任务
		e_server_log_cut_money_refresh_mission_star, //任务刷星
		e_server_log_cut_money_teleport, //传送
		e_server_log_cut_money_element_heart_get_one, //元素之心单个获取
		e_server_log_cut_money_element_heart_summon, //元素之心精炼
		e_server_log_cut_money_element_heart_upgrade, //元素之心升级
		e_server_log_cut_money_map_mopping, //副本扫荡
		e_server_log_cut_money_buy_goods, //买物品
		e_server_log_cut_money_item_add_on, //物品追加
		e_server_log_cut_money_item_advance, //物品进阶
		e_server_log_cut_money_item_assembly, //神装再造
		e_server_log_cut_money_composit, //合成
		e_server_log_cut_money_item_upgrade, //物品升级
		e_server_log_cut_money_wing_upgrade_auto_buy, //翅膀升级自动买
		e_server_log_cut_money_wing_upgrade, //翅膀升级
		e_server_log_cut_money_wing_add_star_auto_buy, //翅膀升星自动买
		e_server_log_cut_money_wing_add_star, //翅膀升星
		e_server_log_cut_money_add_soul, //注魂
		e_server_log_cut_money_add_sprite, //注灵
		e_server_log_cut_money_feather_upgrade_auto_buy, //翎羽升级自动买
		e_server_log_cut_money_feather_upgrade, //翎羽升级
		e_server_log_cut_money_feather_add_star_auto_buy, //翎羽升星自动买
		e_server_log_cut_money_feather_add_star, //翎羽升星
		e_server_log_cut_money_item_succinct, //物品洗练
		e_server_log_cut_money_mount_upgrade_auto_buy, //坐骑升级自动买
		e_server_log_cut_money_mount_upgrade, //坐骑升级
		e_server_log_cut_money_mount_unlock, //坐骑解锁
		e_server_log_cut_money_get_mount_illusion, //坐骑幻化
		e_server_log_cut_money_hope_item_one, //祈福单次
		e_server_log_cut_money_hope_item_ten, //祈福十连
		e_server_log_cut_money_sepcial_name_upgrade, //称号升级
		e_server_log_cut_money_gm_order, //gm指令
		e_server_log_cut_money_get_wing_illusion, //翅膀幻化
		e_server_log_cut_money_element_diamond_recovery, //符文保值回收
		e_server_log_cut_money_chat, //花费金币聊天
		e_server_log_cut_money_item_against_buy, //物品续费
		e_server_log_cut_money_item_package_buy, //物品礼包购买
		e_server_log_cut_money_item_medical_buy, //购买buff药
		e_server_log_cut_money_jewel_upgrade, //宝石升级
		e_server_log_cut_money_auction_buy, //拍卖购买
		e_server_log_cut_money_auction_bid, //拍卖竞标
		e_server_log_cut_money_item_recovery, //物品回收
		e_server_log_cut_money_legion_bonfire, //军团加火
		e_server_log_cut_money_legion_skill, //军团提升技能
		e_server_log_cut_money_patron_saint_buy_slot, //火种购买标志位
		e_server_log_cut_money_patron_saint_upgrade, //火种升级
		e_server_log_cut_money_patron_saint_tinder_upgrade, //火种位置升级
		e_server_log_cut_money_map_add_buff, //副本增加buff
		e_server_log_cut_money_spirit_slot, //精灵开格子
		e_server_log_cut_money_spirit_illusion, //精灵幻化
		e_server_log_cut_money_talent_open, //开启天赋
		e_server_log_cut_money_divorce, //离婚
		e_server_log_cut_money_treasure_prize, //宝藏抽奖
		e_server_log_cut_money_item_assembled, //神装合成
		e_server_log_cut_money_mopping_arena, //竞技场扫荡
		e_server_log_cut_money_cloud_shop, //云购购买
		e_server_log_cut_money_unlock_storage_slot, //解锁仓库格子
		e_server_log_cut_money_belief_rune_levelup, //信仰符文升级
		e_server_log_cut_money_belief_rune_diamond_recover, //信仰符文保值分解
		e_server_log_cut_money_legion_warehouse_score,//军团仓库积分
		e_server_log_cut_money_vip_get_item,//购买VIP特权物品
		e_server_log_cut_money_pokedex_upgrade,//图鉴升级
		e_server_log_cut_money_time_limit_small_charge,//限时小额充值
		e_server_log_cut_money_cross_ladder_buy_ticket,//购买跨服天梯次数
		e_server_log_cut_money_feather_heart_upgrade, //神翎之心升级
		e_server_log_cut_money_skill_book_diamond_recovery, //技能书保值回收
		e_server_log_cut_money_couple_bless, //夫妻榜祝福
		e_server_log_cut_money_far_speak,  //千里传音
		e_server_log_cut_money_buy_special_lucky_times, //购买命中注定次数
		e_sereve_log_cut_money_recycle_buy, //回归系统购买
		e_server_log_cut_money_buy_competition_lv, //购买赛季手册等级
		e_server_cut_add_money_competition, //跨服天梯赛季手册
		e_server_log_cut_money_time_limit_shop, //限时活动商店购买
		e_server_log_cut_money_buy_single_boss,	//购买个人boss次数
		e_server_log_cut_money_dragontrip,      // 萌龙奇遇
		e_server_log_cut_money_wing_lengque, //羽翼铸灵冷却
		e_server_log_cut_money_wing_xiling, //羽翼洗灵
		e_server_log_cut_money_element_war_buy_ticket, //购买元素争霸次数
		e_server_log_cut_money_skytreasure_buy_key,  //天空宝库购买钥匙
		e_server_log_cut_money_starark_buy_item,   // 星海方舟购买消耗道具
		e_server_log_cut_money_buy_element_competition_lv, //购买元素手册等级
		e_server_log_cut_money_activity_shop,//限时商店购买
		e_server_log_cut_money_discount_shop,//折扣商店购买
		e_server_log_cut_money_item_shop,//积分商店购买
		e_server_log_cut_money_weekend_top_up, //周末充值
		e_server_log_cut_money_support_for_promotion,//冲击助力
		e_server_log_cut_money_time_feed_back,// 时空馈赠
		e_server_log_cut_money_time_limit_gift,// 新限时礼包
		e_server_log_cut_money_time_limit_buy_item, //限时活动购买道具
		e_server_log_cut_money_time_refresh_diamond_shop, //刷新钻石商店
		e_server_log_cut_money_time_buy_diamond_shop,	// 钻石商店购买
		e_server_log_cut_money_map_search,	// 地图刷新
		e_server_log_cut_money_upgrade_search_level,	// 探索等级升级
		e_server_log_cut_money_get_award,	// 获取场景中的箱子
		e_server_log_cut_money_treasure_pic,	// 开启宝藏图
		e_server_log_cut_money_finger_guess,	// 猜拳失败
		e_server_log_cut_money_finger_guess_offline,	// 猜拳中掉线
		e_server_log_cut_money_smashing_bottle,	// 砸酒瓶失败
		e_server_log_cut_money_smashing_bottle_offline,	// 砸酒瓶中掉线
		e_server_log_cut_money_max
	};
	enum e_server_log_add_money
	{
		e_server_log_add_money_add_exp, //加经验
		e_server_log_add_money_worship, //膜拜
		e_server_log_add_money_raid_over_reward, //副本结算
		e_server_log_add_money_legion_donate_money, //军团捐钱
		e_server_log_add_money_legion_donate_item, //军团捐物
		e_server_log_add_money_legion_boss, //军团Boss
		e_server_log_add_money_legion_add_fuel_to_bonfire, //军团篝火加火
		e_server_log_add_money_resource_back, //资源找回
		e_server_log_add_money_recharge, //充值
		e_server_log_add_money_grow_up_fund, //成长基金
		e_server_log_add_money_red_package, //红包
		e_server_log_add_money_achievement, //成就
		e_server_log_add_money_convert_main_ui, //主界面的转换
		e_server_log_add_money_convert_in_bag, //背包里的转换
		e_server_log_add_money_get_mail, //领邮件
		e_server_log_add_money_banquet, //舞会
		e_server_log_add_money_codex_del, //图鉴回收
		e_server_log_add_money_codex_refresh, //图鉴刷新
		e_server_log_add_money_sprite_sell, //精灵回收
		e_server_log_add_money_hunt_sprite, //精灵猎取加点
		e_server_log_add_money_meditation, //冥想
		e_server_log_add_money_finish_mission, //任务结算
		e_server_log_add_money_map_mopping, //地图扫荡
		e_server_log_add_money_money_card, //钱卡
		e_server_log_add_money_item_sell, //卖东西
		e_server_log_add_money_item_recovery, //物品回收
		e_server_log_add_money_hope_item, //祈福加点
		e_server_log_add_money_welfare, //领取福利
		e_server_log_add_money_arena_rank, //竞技场排名奖励
		e_server_log_add_money_finish_all_daily_mission, //完成全部日常
		e_server_log_add_money_finish_all_hard_mission, //完成全部讨伐
		e_server_log_add_money_gm_order, //gm指令
		e_server_log_add_money_buff, //buff增加
		e_server_log_add_money_element_heart_recovery, //元素之心回收
		e_server_log_add_money_daily_legion_reward, //每日未领取的军团福利
		e_server_log_add_money_talent_recovery, //天赋回收
		e_server_log_add_money_exp_level_up, //等级提升
		e_server_log_add_money_treasure_prize, //宝藏抽奖
		e_server_log_add_money_arena_mopping, //扫荡竞技场
		e_server_log_add_money_belief_rune_piece, //信仰符文分解碎片
		e_server_log_add_legion_warehouse_score, //军团仓库积分
		e_server_log_add_money_harry,//增加跨服掠夺币
		e_server_log_add_money_skill_book_recovery, //技能书回收
		e_server_log_add_money_competition, //跨服天梯赛季手册
		e_server_log_add_money_cross_ladder_task, //跨服天梯每日任务
		e_server_log_add_money_captain_reward, // 军团副本队长
		e_server_log_add_money_assist_reward,  //队员奖励
		e_server_log_add_money_element_competition, //元素手册
		e_server_log_add_money_time_feed_back, //时光馈赠
		e_server_log_add_money_offline_award,	//离线奖励领取
		e_server_log_add_money_online_award,	//在线奖励领取
		e_server_log_add_money_energy_tick,	//体力自然恢复
		e_server_log_add_money_map_search,	//地图寻宝
		e_server_log_add_money_player_init,	//玩家初始化
		e_server_log_add_money_finger_guess,	//猜拳胜利
		e_server_log_add_money_smashing_bottle,	//砸酒瓶胜利
		e_server_log_add_money_put_into_bag,	//放入背包后自动使用
		e_server_log_add_money_max
	};
	enum e_server_log_del_item
	{
		e_server_log_del_item_put_in_bag_not_create, //只是换个包而不是创建新的
		e_server_log_del_item_del_mail, //邮件删除
		e_server_log_del_item_advance_cost, //物品进阶消耗
		e_server_log_del_item_assembly_cost, //神装再造消耗
		e_server_log_del_item_item_use, //物品使用掉了
		e_server_log_del_item_item_sell, //物品出售
		e_server_log_del_item_limit_item_auto_del, //限制物品自动删除
		e_server_log_del_item_city_war, //城战
		e_server_log_del_item_gm_order, //gm指令
		e_server_log_del_item_auction_sell, //拍卖行卖出删除
		e_server_log_del_item_clearn_talent, //清空天赋扣除
		e_server_log_del_item_set_jewel, //镶宝石
		e_server_log_del_item_assembled_cost, //神装合成消耗
		e_server_log_del_item_rename_cost,	//改名消耗
		e_server_log_del_item_add_on,	//追加
		e_server_log_del_item_treasure,	//抽奖
		e_server_log_del_item_map_mopping, //副本扫荡
		e_server_log_del_item_teleport, //传送
		e_server_log_del_item_friend_gift, //好友送礼品
		e_server_log_del_item_submit_psyche,//图鉴提交魂魄
		e_server_log_del_item_spirit_illusion, //精灵幻化
		e_server_log_del_item_spirit_voucher, //精灵抽奖
		e_server_log_del_item_shop_buy, //商店购买替换
		e_server_log_del_item_assembly,//神装再造
		e_server_log_del_item_assembled, //神装合成
		e_server_log_del_item_composit, //合成
		e_server_log_del_item_upgrade, //物品升级
		e_server_log_del_item_wing_upgrade, //翅膀升级
		e_server_log_del_item_wing_unlock, //翅膀解锁
		e_server_log_del_item_wing_shape_upgrade, //翅膀外形升级
		e_server_log_del_item_wing_illusionupgrade, //翅膀幻化升级
		e_server_log_del_item_wing_starupgrade, //翅膀升星
		e_server_log_del_item_legion_donate, //军团捐物
		e_server_log_del_item_hope_item_one, //祈福单次
		e_server_log_del_item_hope_item_ten, //祈福十连
		e_server_log_del_item_mount_illusion, //坐骑幻化
		e_server_log_del_item_mount_unlock, //坐骑解锁
		e_server_log_del_item_mount_illusionupgrade, //坐骑幻化升级
		e_server_log_del_item_mount_starupgrade, //坐骑升星
		e_server_log_del_item_mount_upgrade, //坐骑升级
		e_server_log_del_item_succinct, //物品洗练
		e_server_log_del_item_feather_add_star, //翎羽升星
		e_server_log_del_item_add_soul, //注魂
		e_server_log_del_item_add_sprite, //注灵
		e_server_log_del_item_wing_iilusion, //翅膀幻化
		e_server_log_del_item_grade_up,		//转职
		e_server_log_del_item_legion_warehouse,//军团仓库捐献
		e_server_log_del_item_mainpalace_upgrade,//图鉴殿堂升级
		e_server_log_del_item_oracle_activate,//图鉴神谕激活
		e_server_log_del_item_auction,//拍卖行上架
		e_server_log_del_item_change_class,//转职
		e_server_log_del_item_back_life,//复活
		e_server_log_del_item_marry,//求婚
		e_server_log_del_item_send_mail,//邮件发送物品
		e_server_log_del_item_red_package,//发送红包
		e_server_log_del_item_bag_zero,//放包时数量为0
		e_server_log_del_item_bag_merge,//放包时合并后数量为0
		e_server_log_del_item_storage_zero,//放仓库时数量为0
		e_server_log_del_item_storage_merge,//放包时合并后数量为0
		e_server_log_del_item_use_beast,//使用兽魂
		e_server_log_del_item_equip_on_spirit,//装备精灵
		e_server_log_del_item_bag_error,//放包失败
		e_server_log_del_item_template_null,//指针为空
		e_server_log_del_item_patron_saint_activate,//守护神激活
		e_server_log_del_item_skill_unlock,//技能解锁
		e_server_log_del_item_time_limit_exchange,//限时兑换
		e_server_log_del_item_split, //拆分的物品
		e_server_log_del_item_pokedex_upgrade, //图鉴升级
		e_server_log_del_item_local_from_mail,//删除来自邮件里的本地物品
		e_server_log_del_item_activate_tinder_skill,//激活火种技能
		e_server_log_del_item_spirit_upgrade,//精灵升级
		e_server_log_del_item_spirit_skill,//精灵技能
		e_server_log_del_item_passive_skill_unlock,//被动技能解锁
		e_server_log_del_item_wing_feather_unlock,//翅膀翎羽解锁
		e_server_log_del_item_forge, //装备重铸
		e_server_log_del_item_re_legion_name,	//军团改名消耗
		e_server_log_del_item_enchant, //装备附魔
		e_server_log_del_item_set_skill_book, //装备技能书
		e_server_log_del_item_upgrade_goddess_equip,//升级女神装备
		e_server_log_del_item_goddess_slot_unlock,//解锁协战槽位
		e_server_log_del_item_wedding_ring_upgrade,//婚戒升级
		e_server_log_del_item_element_merge,//符文融合
		e_server_log_del_item_element_unlock,//符文解锁
		e_server_log_del_item_chat_far_speak, // 千里传音
		e_server_log_del_item_new_tinder_active,  //新火种激活
		e_server_log_del_item_awaken,   // 觉醒消耗
		e_server_log_del_item_unlock_spirit_slot,   // 解锁精灵槽位
		e_server_log_del_item_star_upgrade_goddess,//女神升星
		e_server_log_del_item_dragontrip,  //萌龙奇遇
		e_server_log_del_item_mount_equip_off,   //坐骑装备
		e_server_log_del_item_mount_equip_star,  //坐骑装备升星
		e_server_log_del_item_wing_zhuling,   //羽翼铸灵
		e_server_log_del_item_treasure_clear,  // 宝库清空
		e_server_log_del_item_jewel_carve,     // 宝石纹刻
		e_server_log_del_item_skytreasure,     //天空宝库 
		e_server_log_del_item_starark,        //星海方舟
		e_server_log_del_item_spirit_promote, //精灵升格消耗
		e_server_log_del_item_lucky_card, //福牌开启消耗
		e_server_log_del_item_break_will, // 心魔意志激活消耗
		e_server_log_del_item_cloud_shop, // 云购消耗
		e_server_log_del_item_awaken_skill,//觉醒羁绊技能升级
		e_server_log_del_item_full_bag,//背包满了，直接删除
		e_server_log_del_item_skill_up_level,//技能等级提升
		e_server_log_del_item_max
	};
	enum e_server_log_add_item
	{
		e_server_log_add_item_put_in_bag_not_create, //只是换个包而不是创建新的
		e_server_log_add_item_item_advance, //物品进阶
		e_server_log_add_item_item_assembly, //神装再造
		e_server_log_add_item_item_use, //使用物品
		e_server_log_add_item_item_split, //物品拆分
		e_server_log_add_item_item_composit, //合成
		e_server_log_add_item_create_sprite_stone, //创建精灵石
		e_server_log_add_item_mount_illusion, //坐骑幻化
		e_server_log_add_item_codex_open, //开图鉴
		e_server_log_add_item_raid_reward, //副本结算
		e_server_log_add_item_get_welfare, //领福利
		e_server_log_add_item_activate_code, //物品码
		e_server_log_add_item_finish_mission, //完成任务
		e_server_log_add_item_map_mopping, //扫荡
		e_server_log_add_item_buy_goods, //买东西
		e_server_log_add_item_pick_up_drop_box, //捡掉落包
		e_server_log_add_item_luck_draw, //幸运抽奖
		e_server_log_add_item_item_recovery, //物品回收
		e_server_log_add_item_get_vip_item, //vip礼包
		e_server_log_add_item_func_lock, //功能解锁
		e_server_log_add_item_finish_all_daily_mission, //完成全部日常
		e_server_log_add_item_finish_all_hard_mission, //完成全部讨伐
		e_server_log_add_item_city_war, //城战
		e_server_log_add_item_gm_order, //gm指令
		e_server_log_add_item_wing_iilusion, //翅膀幻化
		e_server_log_add_item_set_jewel_off, //拆宝石
		e_server_log_add_item_treasure_prize, //宝藏抽奖
		e_server_log_add_item_daily_activity_reward, //每日未领取的活跃度福利
		e_server_log_add_item_daily_legion_reward, //每日未领取的军团福利
		e_server_log_add_item_item_assembled, //神装合成
		e_server_log_add_item_mail, //邮件中获取
		e_server_log_add_item_spirit_lock, //魂石解锁
		e_server_log_add_item_cancel_sell, //物品下架
		e_server_log_add_item_rename_fail, //改名失败
		e_server_log_add_item_legion_warehouse,//军团仓库获取
		e_server_log_add_item_service_rank, //全民冲榜
		e_server_log_add_item_spirit_bag, //精灵放回背包
		e_server_log_add_item_load_db, //从数据库读取
		e_server_log_add_item_load_create, //数据增加
		e_server_log_add_item_bag_split, //背包拆分
		e_server_log_add_item_storage_split, //仓库拆分
		e_server_log_add_item_open_package_bag,//物品开包
		e_server_log_add_item_arena_reward,//竞技场奖励
		e_server_log_add_item_time_limit,//限时活动
		e_server_log_add_item_treasure,//抽奖
		e_server_log_add_item_service_goal,//24H
		e_server_log_add_item_seven_day,//7日
		e_server_log_add_item_first_kill,//首杀
		e_server_log_add_item_upgrade_fail_offset,//强化失败补偿
		e_server_log_add_item_add_item_to_bag,//添加物件到背包
		e_server_log_add_item_splite_element_heart,//分解元素之心
		e_server_log_add_item_tidy_belife_rune,//整理信仰符文
		e_server_log_add_item_tidy_element_heart,	//获得元素之心
		e_server_log_add_item_gm_create_item,//GM命令创建物品
		e_server_log_add_item_first_kill_boss_welfare,//世界boss首杀福利
		e_server_log_add_item_notice_info,//添加公告信息
		e_server_log_add_item_by_dtop_items, //通过物品列表添加物品
		e_server_log_add_item_unlock_award,//解锁得到的奖品
		e_server_log_add_item_unenchant,//附魔拆分
		e_server_log_add_item_element_merge,//符文融合
		e_server_log_add_item_person_information,//个性标签
		e_server_log_add_item_marry_dati,//结婚答题
		e_server_log_add_item_recycle, //老友回归
		e_server_log_add_item_competition,//赛季奖励
		e_server_log_add_item_cumulative_sign_in,//累计签到
		e_server_log_add_item_dragontrip,         //萌龙奇遇
		e_server_log_add_item_auction,            //拍卖行购买物品
		e_server_log_add_item_skytreasure,        //天空宝库
		e_server_log_add_item_starark,
		e_server_log_add_item_lucky_card, //福牌奖励
		e_server_log_add_item_element_competition,//元素手册奖励
		e_server_log_add_item_time_feed_back,	//时光馈赠
		e_server_log_add_item_time_limit_gift,	//限时礼包
		e_server_log_add_item_time_limit_buy,	//限时活动购买
		e_server_log_add_buy_diamond_shop,		//钻石商店购买
		e_server_log_add_buy_star_trip,			//星界异旅兑换
		e_server_log_add_item_offline_award,	//离线奖励领取
		e_server_log_add_item_online_award,		//在线奖励领取
		e_server_log_add_item_search_map,		//探索地图奖励
		e_server_log_add_item_treasure_pic,		//藏宝图奖励
		e_server_log_add_item_element_mission,	//符文战任务奖励
		e_server_log_add_item_harry,//增加跨服掠夺物品
		e_server_log_add_item_pk_king,//pk之王奖励
		e_server_log_add_item_legion_station,//军团站
		e_server_log_add_item_broken_sky,//破碎虚空
		e_server_log_add_item_bonfire,//篝火
		e_server_log_add_item_married_reward,//结婚奖励
		e_server_log_add_item_stat_num,//
		e_server_log_add_item_daily_must_do_arena, //每日必做竞技场
		e_server_log_add_item_oracle_trial,
		e_server_log_add_item_npc_damage_rewards,//npc伤害奖励
		e_server_log_add_item_legion_occupation_daily,//军团占领日常
		e_server_log_add_item_legion_draw_mem_daily,//
		e_server_log_add_item_legion_draw_bonfire_daily,//
		e_server_log_add_item_legion_answer_question_right,//
		e_server_log_add_item_max
	};

	enum e_map_change_type
	{
		e_map_change_leave,
		e_map_change_enter
	};

	enum e_cross_server_harry_oper_type
	{
		e_cross_server_harry_oper_get,
		e_cross_server_harry_oper_commit,
		e_cross_server_harry_oper_dead_drop
	};

	enum e_occupation_pk_state_type
	{
		e_occupation_pk_state_out_16,
		e_occupation_pk_state_16,
		e_occupation_pk_state_8,
		e_occupation_pk_state_4,
		e_occupation_pk_state_2,
		e_occupation_pk_state_1,
	};

	enum e_server_beat_boss_type
	{
		e_server_beat_boss_type_world_boss,
		e_server_beat_boss_type_legion_boss,
		e_server_beat_boss_type_broken_boss,
	};
}

#endif
