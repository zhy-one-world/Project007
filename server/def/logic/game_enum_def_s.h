#ifndef _GAME_ENUM_DEF_S_HPP_
#define _GAME_ENUM_DEF_S_HPP_
 
namespace faith
{
     enum e_role_pk_map_state
     {
         e_role_pk_map_state_one_role, //第一个人玩家进入
         e_role_pk_map_state_wait_begin, //等待计时结束游戏开始
         e_role_pk_map_state_begin_war, //开始战斗了
         e_role_pk_map_state_end_war, //游戏结束了
         e_role_pk_map_state_max
     };
     enum e_map_type
     {
         e_map_type_ui, //ui地图
         e_map_type_new, //新手村
         e_map_type_main, //主城
         e_map_type_battle, //剧情副本
         e_map_type_field, //上古遗迹
         e_map_type_big_map, //大世界地图
         e_map_type_fly_battle, //空中战斗副本
         e_map_type_exp_fuben, //经验副本
         e_map_type_money_fuben, //绑金副本
         e_map_type_demon_tower_fuben, //万魔塔副本
         e_map_type_legion_boss, //军团BOSS副本
         e_map_type_city_war, //军团城战
         e_map_type_broken_sky, //破碎虚空
         e_map_type_king_of_pk, //PK之王活动
         e_map_type_arena, //竞技场
         e_map_type_temple, //诅咒神殿
         e_map_type_empire_treasure, //帝国遗藏
         e_map_type_crystak_dreamland, //水晶幻境
         e_map_type_legion_bonfire, //军团篝火
         e_map_type_boss_single, //打宝个人
         e_map_type_boss_home, //打宝boss之家
         e_map_type_boss_vip_home, //打宝VIPboss之家
         e_map_type_boss_island, //永恒岛
         e_map_type_belief_cloister, //信仰回廊
         e_map_type_princess_guard_war, //公主守卫战
         e_map_type_lava_hellfire, //熔岩地狱
         e_map_type_sea_of_hate, //无尽之海
         e_map_type_abyss_of_fear, //领主的试炼
         e_map_type_fallen_fantasy, //梦境的试炼
         e_map_type_star_test, //众星的试炼
         e_map_type_cross_server_pk, //跨服PK
         e_map_type_cross_server_harry, //跨服掠夺
         e_map_type_cross_ladder, //跨服天梯
         e_map_type_oracle_trial_map, //天启试炼
         e_map_type_legion_station, //军团驻地
         e_map_type_appearance, //捏脸地图
         e_map_type_single_land_boss, //单人地宫Boss
         e_map_type_single_assist_boss, //单人助战地图
         e_map_type_level_up_map, //升级副本
         e_map_type_element_war, //元素争霸
         e_map_type_attack_city, //攻城战
         e_map_type_single_monster, //单人刷怪奖励地图
         e_map_type_single_open_box, //单人开箱子奖励地图
         e_map_type_max
     };
     enum e_role_info
     {
         e_role_info_template_id, //模板id
         e_role_info_server_id, //服务器id
         e_role_info_img_id, //头像
         e_role_info_class_type, //职业
         e_role_info_class_branch, //转职
         e_role_info_gender, //角色性别
         e_role_info_exp_level, //经验等级
         e_role_info_vip_level, //vip等级
         e_role_info_vip_experience_level, //vip体验等级
         e_role_info_recharge_num, //充值点数
         e_role_info_main_map_id, //大地图模板id
         e_role_info_main_pos_x, //大地图坐标x
         e_role_info_main_pos_y, //10大地图坐标y
         e_role_info_main_pos_z, //大地图坐标z
         e_role_info_main_pos_yaw, //大地图旋转
         e_role_info_main_line_id, //15大地图线id
         e_role_info_move_map_id, //要到达地图模板id
         e_role_info_move_pos_x, //当前坐标x
         e_role_info_move_pos_y, //当前坐标y
         e_role_info_move_pos_z, //当前坐标z
         e_role_info_move_pos_yaw, //旋转
         e_role_info_move_line_id, //要到达地图线id
         e_role_info_GS_value, //25当前战斗力
         e_role_info_upgrade_id, //等级模板id
         e_role_info_daily_done_num, //日常完成数目
         e_role_info_against_done_num, //讨伐完成数目
         e_role_info_wing_showd_template_id, //30要显示的翅膀ID
         e_role_info_mount_showd_template_id, //要显示的坐骑ID
         e_role_info_element_heart_drop_group, //元素之心的抽取组
         e_role_info_arena_times, //竞技场次数
         e_role_info_arena_vip_times, //vip可挑战次数
         e_role_info_arena_vip_buy_times, //35vip已购买次数
         e_role_info_arena_winning_streak, //连胜纪录
         e_role_info_legion_donate_earn_by_money, //军团捐献金币获得的战功
         e_role_info_legion_donate_earn_by_item, //军团捐献物品获得的战功
         e_role_info_worship_other_time, //玩家崇拜次数
         e_role_info_get_func_unlock_award_record, //功能解锁奖励领取记录
         e_role_info_vip_item_geted_level, //VIP礼包领取到第几级了
         e_role_info_vip_free_revive_time, //免费复活次数
         e_role_info_element_heart_refine_time, //元素之心提炼次数
         e_role_info_total_login_days, //累计登陆天数
         e_role_info_continue_login_time, //45连续登录次数
         e_role_info_daily_activity_degree, //日活跃度
         e_role_info_daily_recharge_num, //日充值数
         e_role_info_total_diamond_cost_num, //累计消费钻石数
         e_role_info_daily_active_degree_get_flag, //每日活跃奖励领取标志位
         e_role_info_fund_recharge_num, //50成长基金充值基金充值额
         e_role_info_fund_payment_flag, //成长基金购买标记位
         e_role_info_fund_time_get_flag, //成长基金时光领取标记位
         e_role_info_fund_pride_get_flag, //成长基金充值领取标记位
         e_role_info_fund_reborn_get_flag, //成长基金重生领取标记位
         e_role_info_fund_cost_num, //55成长基金充值基金消费额
         e_role_info_lord_of_city_worship_time, //城主膜拜次数
         e_role_info_lord_of_war_worship_time, //PK之王膜拜次数
         e_role_info_lord_of_sanctuary_worship_time, //圣域之主膜拜次数
         e_role_info_lord_of_god_worship_time, //众神之王膜拜次数
         e_role_info_hope_value, //祈福祝福值
         e_role_info_del_time, //删除时间0未删除
         e_role_info_kill_player_num, //击杀玩家数量
         e_role_info_spirit_value, //精灵猎取保底
         e_role_info_watch_count, //查看的最大人数
         e_role_info_crystal_dreamland_chests, //水晶幻境采集宝箱个数
         e_role_info_protect_mark_slot_open_num, //守护印记槽位开启个数
         e_role_info_element_heart_slot_open_num, //元素之心槽位开启个数
         e_role_info_sprite_equip_slot_open_num, //精灵装备槽位开启个数
         e_role_info_exclusive_flag, //尊享用户标志
         e_role_info_red_package_geted_times, //每日红包已领取次数
         e_role_info_free_red_package_send_times, //每日免费发红包次数
         e_role_info_world_boss_chests, //世界BOSS掉落宝箱采集数
         e_role_info_leave_map_id, //要离开地图的id
         e_role_info_assist_chests, //助战宝箱获取次数
         e_role_info_next_side_mission_main_trigger_id, //下个待触发支线任务的主线任务ID
         e_role_info_show_fashion, //是否显示时装
         e_role_info_real_recharge_jewel_num, //冲入钻石数主要用于充值榜
         e_role_info_relic_kill_num, //上古遗迹今日杀怪数
         e_role_info_relic_exp_num, //上古遗迹今日获得经验数
         e_role_info_head_frame, //头像框
         e_role_info_fund_pride_cost_value, //成长基金消费点数
         e_role_info_tinder_prop_slot_open_num, //火种道具槽位开启个数
         e_role_info_show_sky_suit_shape, //是否显示天空套装外形
         e_role_info_move_server_id, //要到达服务器ID
         e_role_info_move_war_idex, //要到达地图位置
         e_role_info_goddess_slot_flag, //女神协战槽位开启标记
         e_role_info_daily_cost, //每日消费数值
         e_role_info_marry_done_num, //结婚任务完成数量
         e_role_info_marry_dati_id, //当前结婚答题id
         e_role_info_marry_dati_num, //结婚答题完成数量
         e_role_info_marry_dati_right_num, //结婚答题答对数量
         e_role_info_marry_dati_current_answer, //结婚答题当前答案
         e_role_info_legion_dance_exp_num, //军团共舞今日经验获取次数
         e_role_info_legion_donate_num, //军团捐赠次数
         e_role_info_appearance_id, //捏脸外形ID
         e_role_info_is_show_vip, //是否显示vip
         e_role_info_need_check_guide, //是否需要检查引导
         e_role_info_enchant_show_type, //附魔显示buff类型
         e_role_info_push_map_id, //推图的地图id
         e_role_info_max
     };
     enum e_role_logic_info
     {
         e_role_logic_info_bag_open, //背包开启格子数最大值由宏MAX_ITEMINST_SIZE_PROP定义
         e_role_logic_info_main_pk_mode, //在大世界时候的PK模式
         e_role_logic_info_jewel_first_one, //是否是第一次宝石单次寻宝
         e_role_logic_info_jewel_first_ten, //是否是第一次宝石十次寻宝
         e_role_logic_info_jewel_luck, //宝石幸运积分
         e_role_logic_info_jewel_free, //宝石免费寻宝
         e_role_logic_info_element_first_one, //是否是第一次符文单次寻宝
         e_role_logic_info_element_first_ten, //是否是第一次符文十次寻宝
         e_role_logic_info_element_luck, //符文幸运积分
         e_role_logic_info_element_free, //符文免费寻宝
         e_role_logic_info_spirit_first_one, //是否是第一次精灵单次寻宝
         e_role_logic_info_spirit_first_ten, //是否是第一次精灵十次寻宝
         e_role_logic_info_spirit_luck, //精灵幸运积分
         e_role_logic_info_spirit_free, //精灵免费寻宝
         e_role_logic_info_storage_open, //仓库背包开启格子数目
         e_role_logic_info_first_look_legion, //第一次查看军团页面
         e_role_logic_info_done_grade_mission, //完成的转职任务
         e_role_logic_info_god_hood_grade, //神格突破等级
         e_role_logic_info_first_join_legion_award, //第一次加入军团获得的奖励
         e_role_logic_info_is_get_kill_legion_boss_award, //今日是否获得军团boss奖励
         e_role_logic_info_done_grade_mission_1, //完成的转职任务每个可以表示6个大转职
         e_role_logic_info_done_grade_mission_2, //完成的转职任务
         e_role_logic_info_done_grade_mission_3, //完成的转职任务
         e_role_logic_info_done_grade_mission_4, //完成的转职任务
         e_role_logic_info_jewel_total_recharge, //钻石总充值
         e_role_logic_info_jewel_total_consume, //钻石总消费
         e_role_logic_info_cross_ladder_join_num, //跨服天梯每日参与次数
         e_role_logic_info_cross_ladder_win_num, //跨服天梯每日胜利次数
         e_role_logic_info_cross_ladder_reward_tag, //跨服天梯领奖标识
         e_role_logic_info_marry_times, //结婚次数
         e_role_logic_info_marry_divorce_times, //离婚次数
         e_role_logic_info_daily_lucky_times, //每日命中注定次数
         e_role_logic_info_zero_gs_up_start_value, //零点战力提升榜初始值
         e_role_logic_info_zero_mount_up_start_value, //零点刷新坐骑提升榜初始值
         e_role_logic_info_zero_wing_up_start_value, //零点刷新翅膀提升榜初始值
         e_role_logic_info_zero_equip_up_start_value, //零点刷新装备提升榜初始值
         e_role_logic_info_zero_spirit_up_start_value, //零点刷新精灵提升榜初始值
         e_role_logic_info_helper_reward_send_times, //助战奖励发放次数
         e_role_logic_info_boss_home_cur_used_times, //boss之家当前使用次数
         e_role_logic_info_gain_treasure_restore_num, //地宫boss恢复次数
         e_role_logic_info_gain_treasure_restore_time, //地宫boss恢复时间
         e_role_logic_info_sky_land_boss_restore_num, //天空岛boss恢复次数
         e_role_logic_info_sky_land_boss_restore_time, //天空岛boss恢复时间
         e_role_logic_info_boss_island_cur_kill_times, //天空岛当前击杀boss次数
         e_role_logic_info_help_reward_send_times, //被助战奖励发放次数
         e_role_logic_info_sprite_equip_slot_open_flag, //精灵包裹解锁标识
         e_role_logic_info_check_in_days, //签到累计天数
         e_role_logic_info_check_in_repetition_num, //签到循环机次数
         e_role_logic_info_exp_raid_add_damage_gold, //经验副本鼓舞金币次数
         e_role_logic_info_exp_raid_add_damage_diamond, //经验副本鼓舞钻石次数
         e_role_logic_info_element_war_mission_tag, //元素争霸每日奖励领取标记位
         e_role_logic_info_element_war_buy_ticket, //元素争霸每日购买次数
         e_role_logic_info_jewel_carve_lucky, //宝石纹刻幸运值
         e_role_logic_info_choose_fashion_id, //选择时装任务选择的时装id
         e_role_logic_info_help_tip_send_times, //累计发送助战请求次数
         e_role_logic_info_is_have_new_server_mail, //是否获取过新服邮件奖励
         e_role_logic_info_awaken_fetter, //觉醒大师等级
         e_role_logic_info_offline_award_time, //离线计算开启时间
         e_role_logic_info_offline_award_random, //离线计算随机种子
         e_role_logic_info_online_award_money_type, //在线奖励货币类型
         e_role_logic_info_online_award_money_num, //在线奖励货币数量
         e_role_logic_info_online_award_time, //在线奖励总时长
         e_role_logic_info_online_award_money_count, //在线奖励货币次数
         e_role_logic_info_online_award_item_count, //在线奖励物品次数
         e_role_logic_info_skill_level, //技能当前等级
         e_role_logic_info_search_value, //探索值
         e_role_logic_info_search_level, //玩家探索等级
         e_role_logic_info_max
     };
     enum e_role_box_map
     {
         e_role_box_map_next_award, //获取下一个开箱的npc
         e_role_box_map_search_time, //下一次搜索的时间间隔(秒)
         e_role_box_map_middle_open_compensate, //中级宝箱开启补偿值
         e_role_box_map_big_open_compensate, //大宝箱开启补偿次数
         e_role_box_map_special_refrush_compensate, //特殊宝箱刷新补偿次数
         e_role_box_map_special_open_compensate, //特殊宝箱开启补偿值
         e_role_box_map_max
     };
     enum e_role_element_map
     {
         e_role_element_map_mop_up_count, //扫荡剩余次数
         e_role_element_map_max
     };
     enum e_role_battle_map
     {
         e_role_battle_map_mop_up_count, //扫荡剩余次数
         e_role_battle_map_max
     };
     enum e_unit_attack_att
     {
         e_unit_attack_att_none,
         e_unit_attack_att_strength, //力量
         e_unit_attack_att_agility, //敏捷
         e_unit_attack_att_intellect, //智力
         e_unit_attack_att_stamina, //耐力
         e_unit_attack_att_attack_min, //最小攻击力
         e_unit_attack_att_attack_max, //最大攻击力
         e_unit_attack_att_armor, //防御力
         e_unit_attack_att_hp_max, //最大生命生命值
         e_unit_attack_att_hit_force, //命中精通
         e_unit_attack_att_dodge_force, //闪避精通
         e_unit_attack_att_critical_rate, //暴击精通
         e_unit_attack_att_critical_armor_rate, //暴击抵抗
         e_unit_attack_att_critical_attack_rate, //暴击伤害
         e_unit_attack_att_critical_attack_relief, //暴击减伤
         e_unit_attack_att_att_attack_1, //属性伤害1
         e_unit_attack_att_att_armor_1, //属性抵抗1
         e_unit_attack_att_att_attack_2, //属性伤害2
         e_unit_attack_att_att_armor_2, //属性抵抗2
         e_unit_attack_att_att_attack_3, //属性伤害3
         e_unit_attack_att_att_armor_3, //属性抵抗3
         e_unit_attack_att_att_attack_4, //属性伤害4
         e_unit_attack_att_att_armor_4, //属性抵抗4
         e_unit_attack_att_element_ability, //元素精通
         e_unit_attack_att_element_resist, //元素亲和
         e_unit_attack_att_effect_damage, //最终伤害
         e_unit_attack_att_effect_damage_relief, //最终伤害减免
         e_unit_attack_att_hit_prob, //命中率
         e_unit_attack_att_un_hit_prob, //闪避率
         e_unit_attack_att_crit_prob, //暴击率
         e_unit_attack_att_un_crit_prob, //暴抗率
         e_unit_attack_att_move_speed, //速度
         e_unit_attack_att_un_dizzy, //抗眩晕
         e_unit_attack_att_un_move_speed, //抗减速
         e_unit_attack_att_un_frozen, //抗冰冻
         e_unit_attack_att_un_no_skill, //抗沉默
         e_unit_attack_att_magic, //魔法值
         e_unit_attack_att_body_power, //体力值
         e_unit_attack_att_exp_addition, //打怪经验加成
         e_unit_attack_att_attack_npc, //打怪增伤
         e_unit_attack_att_attack_player, //打人增伤
         e_unit_attack_att_exp_add_all, //全局经验加成
         e_unit_attack_att_be_attack_player, //被人打增伤
         e_unit_attack_att_pure_attack, //纯粹伤害
         e_unit_attack_att_pure_armor, //纯粹防御
         e_unit_attack_att_attack_multiple, //攻击翻倍
         e_unit_attack_att_flight_energy, //飞行冲刺能力值
         e_unit_attack_att_max
     };
     enum e_unit_game_att
     {
         e_unit_game_att_movement, //当前运动状态
         e_unit_game_att_exp_only, //当前经验的倍率
         e_unit_game_att_suck_blood, //吸血比例
         e_unit_game_att_damage_reflect, //伤害反射
         e_unit_game_att_war_state, //战斗状态
         e_unit_game_att_damage_sub, //伤害减免
         e_unit_game_att_fight_lock, //锁定
         e_unit_game_att_jump, //跳跃
         e_unit_game_att_interaction, //交互
         e_unit_game_att_exp_add, //经验倍数累加
         e_unit_game_att_attack_num, //攻击次数
         e_unit_game_att_back_hp_percent, //回血百分比
         e_unit_game_att_back_hp_value, //回血固定值
         e_unit_game_att_no_back_hp_percent, //减少回血百分比
         e_unit_game_att_pure_hurt_percent, //纯粹伤害百分比
         e_unit_game_att_pure_hurt_value, //纯粹伤害固定值
         e_unit_game_att_max
     };
     enum e_war_state
     {
         e_war_state_peace, //和平状态
         e_war_state_attack_ready, //战斗准备
         e_war_state_attack_in, //战斗状态
         e_war_state_max
     };
     enum e_unit_attack_state
     {
         e_unit_attack_state_dead, //死亡
         e_unit_attack_state_dizzy, //眩晕
         e_unit_attack_state_frozen, //冰冻
         e_unit_attack_state_attack_back, //击退
         e_unit_attack_state_no_control, //是否免疫控制
         e_unit_attack_state_no_hurt, //是否免疫伤害
         e_unit_attack_state_stealth, //是否隐身 自己不可见
         e_unit_attack_state_no_dead, //hp低于0时是否死亡
         e_unit_attack_state_no_skill, //沉默
         e_unit_attack_state_adversity_aid, //逆境援助
         e_unit_attack_state_speed_down, //减速
         e_unit_attack_state_speed_up, //加速
         e_unit_attack_state_stealth_show_me, //是否隐身 自己可见
         e_unit_attack_state_bleeding, //是否流血
         e_unit_attack_state_burn, //是否灼烧
         e_unit_attack_state_adsorbent, //是否吸附
         e_unit_attack_state_crazy, //是否狂暴
         e_unit_attack_state_lock_cd, //锁定cd
         e_unit_attack_state_will_dead, //是否濒死无敌
         e_unit_attack_state_will_dead_percent, //百分比濒死无敌
         e_unit_attack_state_max
     };
     enum e_pawn_att_type
     {
         e_pawn_att_type_attack, //攻击
         e_pawn_att_type_game, //单一属性
         e_pawn_att_type_state, //战斗状态
         e_pawn_att_type_sign, //标记
         e_pawn_att_type_max
     };
     enum e_class_type
     {
         e_class_type_none,
         e_class_type_barserker, //狂战士
         e_class_type_wizard, //巫师
         e_class_type_guardian, //守护者
         e_class_type_assassinator, //暗杀者
         e_class_type_ranger, //游侠
         e_class_type_monster, //怪物
         e_class_type_max
     };
     enum e_branch_type
     {
         e_branch_type_common, //基础职业
         e_branch_type_ice, //冰
         e_branch_type_fire, //火
         e_branch_type_light, //光明
         e_branch_type_dark, //黑暗
         e_branch_type_max
     };
     enum e_map_record_info
     {
         e_map_record_info_map_template_id, //地图模板ID
         e_map_record_info_unlock, //地图解锁状态
         e_map_record_info_enter_count, //组地图进入次数
         e_map_record_info_fastest_time, //最速通关时间
         e_map_record_info_extra_enter_count, //通过花费特殊钱物进入的次数
         e_map_record_info_score_record, //分数记录
         e_map_record_info_real_enter_count, //本地图实际进入次数
         e_map_record_info_buy_count, //已购买次数
         e_map_record_info_use_things_count, //消耗物品获得次数
         e_map_record_info_first_join, //是否已经进入过副本了
         e_map_record_info_first_award, //是否领取过首次奖励
         e_map_record_info_next_refresh_time, //下次刷新时间
         e_map_record_info_mopping_num, //累计刷新次数
         e_map_record_info_all_enter_count, //历史累计进入次数
         e_map_record_info_max
     };
     enum e_big_player_type
     {
         e_big_player_type_lord_of_city, //城主
         e_big_player_type_lord_or_war, //pk之王
         e_big_player_type_lord_or_sanctuary, //圣域之主
         e_big_player_type_lord_or_god, //众神之王
         e_big_player_type_best_barserker, //最强狂战士
         e_big_player_type_best_wizard, //最强法师
         e_big_player_type_best_guardian, //最强骑士
         e_big_player_type_best_assassinator, //最强暗杀者
         e_big_player_type_overload_legion, //霸主军团
         e_big_player_type_gs_first_barserker, //战士战力榜第一
         e_big_player_type_gs_first_wizard, //法师战力榜第一
         e_big_player_type_gs_first_guardian, //骑士战力榜第一
         e_big_player_type_gs_first_assassinator, //刺客战力榜第一
         e_big_player_type_gs_first_player, //战力总排行第一
         e_big_player_type_money_first_player, //金钱第一
         e_big_player_type_worship_first_player, //威望第一
         e_big_player_type_arena_first_player, //竞技场第一
         e_big_player_type_max
     };
     enum e_map_state
     {
         e_map_state_join, //游戏开始时进人的阶段到一定时间无人进入会删除这个副本
         e_map_state_game, //游戏有人了可以开始
         e_map_state_in_game, //游戏正式开始
         e_map_state_over, //游戏已经结束开始传送玩家通知ws删除自己
         e_map_state_return_data, //等待资源回收
         e_map_state_max
     };
     enum e_bag_type
     {
         e_bag_type_none,
         e_bag_type_equip, //已装备
         e_bag_type_bag, //道具包
         e_bag_type_storage, //装备背包
         e_bag_type_equip_spirit, //已装备的精灵背包
         e_bag_type_awaken_item, //觉醒大师物品包
         e_bag_type_online_award, //在线奖励
         e_bag_type_wing, //翅膀包
         e_bag_type_mount, //坐骑包
         e_bag_type_protect, //未装备的守护之灵的背包
         e_bag_type_equip_protect, //已经装备的守护之灵的背包
         e_bag_type_mail, //邮件附件物品包
         e_bag_type_hope_item, //祈愿获得的物品的包
         e_bag_type_elemenet, //未装备的元素之心的背包
         e_bag_type_equip_elemenet, //已经装备的元素之心的背包
         e_bag_type_fasion, //时装
         e_bag_type_equip_fasion, //已装备时装
         e_bag_type_mount_beast_spirit, //坐骑兽魂包
         e_bag_type_wing_beast_spirit, //羽翼兽魂包
         e_bag_type_treasure, //宝藏抽取包
         e_bag_type_belief_rune, //未装备的信仰符文背包
         e_bag_type_belief_rune_equip, //已装备的信仰符文背包
         e_bag_type_equip_tinder_prop, //已装备的火种道具背包
         e_bag_type_sky_equip, //天空装备包
         e_bag_type_skill_book, //技能书
         e_bag_type_goddess_equip, //女神装备
         e_bag_type_wedding_equip, //结婚装备
         e_bag_type_core_element, //核心符文
         e_bag_type_spirit_beast_spirit, //精灵兽魂包
         e_bag_type_skygod_equip, //天神装备包
         e_bag_type_supreme_equip, //神王装备包
         e_bag_type_spirit_upgrade_promote, //星图精灵包
         e_bag_type_break_will, //心魔意志装备包
         e_bag_type_max
     };
     enum e_item_info
     {
         e_item_info_info_id, //模板id
         e_item_info_slot, //物品位置索引e_role_equip物品装备时代表物品在装备栏中的位置在物品栏时代表在物品栏中的位置
         e_item_info_equip_slot, //装配位置（背包，显示共用时）
         e_item_info_container_type, //所在包裹:e_bag_type
         e_item_info_stack_count, //物品堆叠数目
         e_item_info_locked, //是否绑定 1视为绑定0视为未绑定
         e_item_info_activate, //是否激活
         e_item_info_is_first, //5是否第一次激活
         e_item_info_upgrade_count, //装备等级坐骑升星数升阶翎羽升星数翎羽升阶注魂次数注灵次数精灵等级元素之心等级兽魂等级
         e_item_info_stargrade_count, //坐骑、羽翼碎片升星数
         e_item_info_starskill_flag, //坐骑、羽翼魂星技能开启标记
         e_item_info_add_on, //装备追加次数
         e_item_info_wing_exp, //坐骑当前升星经验元素之心当前经验
         e_item_info_wing_bless, //10坐骑祝福值
         e_item_info_random_had_flag, //拥有的随机属性标记位
         e_item_info_random_property1, //物品随机属性精灵天赋属性或卓越属性
         e_item_info_random_property2,
         e_item_info_random_property3,
         e_item_info_random_property4,
         e_item_info_random_property5,
         e_item_info_random_property6,
         e_item_info_succinct_property1, //装备洗练属性坐骑翅膀精灵技能激活时间
         e_item_info_succinct_property2,
         e_item_info_succinct_property3,
         e_item_info_succinct_property4,
         e_item_info_succinct_property5, //备用
         e_item_info_succinct_property6,
         e_item_info_showing_illusion_idex, //显示的幻化ID默认为-1
         e_item_info_put_in_bag_count, //放入背包计数主要用于确认是否是新获得的物品
         e_item_info_illusion_had_byte, //已获取幻化位标识符
         e_item_info_over_time, //物品到期时间
         e_item_info_succinct_level, //洗练等级
         e_item_info_jewel_slot_0, //宝石槽位0
         e_item_info_jewel_slot_1, //宝石槽位1
         e_item_info_jewel_slot_2, //宝石槽位2
         e_item_info_jewel_slot_3, //宝石槽位3
         e_item_info_jewel_slot_4, //宝石槽位4 
         e_item_info_jewel_vip_slot_0, //VIP宝石槽位0
         e_item_info_jewel_vip_slot_1, //VIP宝石槽位1
         e_item_info_prototype_id, //物品原型id 限时物品改变前id
         e_item_info_forge_property1, //物品重铸属性精灵天赋属性或卓越属性羽翼灵性值
         e_item_info_forge_property2          , //羽翼铸灵冷却时间 
         e_item_info_forge_property3          , //羽翼通灵技能id 
         e_item_info_forge_property4         , //羽翼清灵露使用次数
         e_item_info_forge_property5, //羽翼灵性值第一次大于等于80的值
         e_item_info_forge_property6, //羽翼灵性值到达110标记位
         e_item_info_jewel_carve_0           , //随机纹刻
         e_item_info_jewel_carve_1            , //宝石纹刻1
         e_item_info_jewel_carve_2            , //宝石纹刻2
         e_item_info_jewel_lucky_carve, //是否是幸运纹刻
         e_item_info_max
     };
     enum e_item_base
     {
         e_item_base_bag_type, //所在包裹:e_bag_type
         e_item_base_count, //物品堆叠数目
         e_item_base_locked, //是否绑定 1视为绑定0视为未绑定
         e_item_base_max
     };
     enum e_item_equip
     {
         e_item_equip_level, //装备等级
         e_item_equip_color, //装备品质
         e_item_equip_attack, //攻击
         e_item_equip_defense, //防御
         e_item_equip_hp, //血量
         e_item_equip_random_att_1, //随机高级属性条目1
         e_item_equip_random_att_2, //随机高级属性条目2
         e_item_equip_random_att_3, //随机高级属性条目3
         e_item_equip_random_att_4, //随机高级属性条目4
         e_item_equip_random_att_5, //随机高级属性条目5
         e_item_equip_max
     };
     enum e_item_sprite
     {
         e_item_sprite_equip_slot, //装备位置
         e_item_sprite_level, //强化等级
         e_item_sprite_max
     };
     enum e_item_prop
     {
         e_item_prop_param1, //暂时无用，占位
         e_item_prop_max
     };
     enum e_item_element
     {
         e_item_element_equip_slot, //装备位置
         e_item_element_level, //强化等级
         e_item_element_max
     };
     enum e_item_lock
     {
         e_item_lock_all, //全部获取
         e_item_lock_lock, //获取锁定物品
         e_item_lock_un_lock, //获取非锁定物品
         e_item_lock_max
     };
     enum e_item_operation
     {
         e_item_operation_none,
         e_item_operation_equipon, //装备上
         e_item_operation_equipoff, //脱下
         e_item_operation_weaponactivate, //激活武器
         e_item_operation_weaponfirst, //第一次激活武器
         e_item_operation_levelup, //物品强化
         e_item_operation_synthesis, //物品合成
         e_item_operation_sell, //物品出售
         e_item_operation_addon, //物品追加
         e_item_operation_recovery, //物品回收
         e_item_operation_use, //使用
         e_item_operation_wing_upgrade, //羽翼升级（以前的用法）坐骑升阶升星
         e_item_operation_wing_starupgrade, //羽翼碎片升星
         e_item_operation_wing_illusionupgrade, //羽翼幻化升级
         e_item_operation_wing_addsoul, //羽翼注魂
         e_item_operation_wing_addspirit, //羽翼注灵
         e_item_operation_featherupgrade, //翎羽升阶升星
         e_item_operation_tidy, //整理装备
         e_item_operation_inherit, //装备传承
         e_item_operation_advanced, //装备进阶
         e_item_operation_spirit_upgrade, //精灵升级
         e_item_operation_split, //物品拆分
         e_item_operation_spirit_send_out, //精灵送出背包
         e_item_operation_spirit_recovery, //精灵回收
         e_item_operation_godassembly, //神装再造
         e_item_operation_succinct_gold, //装备金币洗练
         e_item_operation_succinct_diamond, //装备钻石洗练
         e_item_operation_save_succinct_result, //保存洗练结果
         e_item_operation_show_this_wing, //显示该翅膀3
         e_item_operation_mount_unlock, //解锁坐骑
         e_item_operation_mount_upgrade, //坐骑升级
         e_item_operation_mount_starupgrade, //坐骑升星
         e_item_operation_mount_illusionupgrade, //坐骑幻化升级
         e_item_operation_mount_show_this_mount, //显示此坐骑;
         e_item_operation_mount_show_this_mount_illusion, //显示幻化
         e_item_operation_mount_get_illusion, //解锁幻化
         e_item_operation_retrieve_psyche, //回收精魄
         e_item_operation_put_into_bag, //放入背包
         e_item_operation_tidy_hope_item, //整理祈福物品包
         e_item_operation_featherupgrade_star, //羽翼升星
         e_item_operation_featherupgrade_grade, //羽翼升阶
         e_item_operation_spirit_had, //拥有精灵
         e_item_operation_wing_shape_upgrade, //羽翼外形升级
         e_item_operation_wing_shape_unlock, //羽翼外形解锁
         e_item_operation_wing_shape_illusion_unlock, //羽翼外形幻化解锁
         e_item_operation_one_key_use, //一键使用
         e_item_operation_quickly_hp, //保存快捷药剂
         e_item_operation_against_buy, //物品续费
         e_item_operation_buy_and_use, //购买并使用
         e_item_operation_use_beast_spirit, //使用兽魂
         e_item_operation_set_jewel_on, //镶嵌宝石
         e_item_operation_set_jewel_off, //取下宝石
         e_item_operation_jewel_upgrade, //宝石升级
         e_item_operation_tidy_treasure_item, //整理宝藏物品包
         e_item_operation_get_treasure_item, //取出宝藏物品包
         e_item_operation_godassembled, //神装合成
         e_item_operation_tidy_storage_item, //整理仓库物品包
         e_item_operation_fashion_upgrade, //时装升星
         e_item_operation_forge, //装备锻造
         e_item_operation_n_succinct_gold, //装备n次金币洗练
         e_item_operation_n_succinct_diamond, //装备n次钻石洗练
         e_item_operation_enchant, //装备附魔
         e_item_operation_unenchant, //装备反附魔
         e_item_operation_wing_zhuling, //羽翼铸灵
         e_item_operation_wing_lengque, //羽翼冷却
         e_item_operation_wing_xiling, //羽翼洗灵
         e_item_operation_spirit_upgrade_promote, //精灵升格
         e_item_operation_break_will_activate, //心魔意志
         e_item_operation_awaken_fetter, //觉醒大师激活
         e_item_operation_awaken_fetter_skill, //觉醒大师技能升级
         e_item_operation_max
     };
     enum e_item_composit_type
     {
         e_item_composit_type_none,
         e_item_composit_type_element, //符文
         e_item_composit_type_equip, //装备
         e_item_composit_type_sprite, //精灵
         e_item_composit_type_max
     };
     enum e_welfare_random_item
     {
         e_welfare_random_item_daily_online_0, //每日在线0
         e_welfare_random_item_daily_online_1, //每日在线1
         e_welfare_random_item_daily_online_2, //每日在线2
         e_welfare_random_item_daily_online_3, //每日在线3
         e_welfare_random_item_daily_online_4, //每日在线4
         e_welfare_random_item_daily_online_5, //每日在线5
         e_welfare_random_item_daily_online_6, //每日在线6
         e_welfare_random_item_daily_online_7, //每日在线7
         e_welfare_random_item_continue_login_0, //连续登陆0
         e_welfare_random_item_continue_login_1, //连续登陆1
         e_welfare_random_item_continue_login_2, //连续登陆2
         e_welfare_random_item_continue_login_3, //连续登陆3
         e_welfare_random_item_continue_login_4, //连续登陆4
         e_welfare_random_item_continue_login_5, //连续登陆5
         e_welfare_random_item_continue_login_6, //连续登陆6
         e_welfare_random_item_continue_login_7, //连续登陆7
         e_welfare_random_item_continue_login_8, //连续登陆8
         e_welfare_random_item_continue_login_9, //连续登陆9
         e_welfare_random_item_max
     };
     enum e_item_use_type
     {
         e_item_use_type_can_not_use, //不能使用
         e_item_use_type_can_use, //能使用
         e_item_use_type_can_batch_use, //能批量使用
         e_item_use_type_only_jump, //纯跳转
         e_item_use_type_max
     };
     enum e_item_color
     {
         e_item_color_white, //白色
         e_item_color_green, //绿色
         e_item_color_blue, //蓝色
         e_item_color_purple, //紫色
         e_item_color_orange, //橙色
         e_item_color_red, //紅色
         e_item_color_pink, //粉色
         e_item_color_max
     };
     enum e_tem_melting_sort_type
     {
         e_tem_melting_sort_type_gs, //最下层按战斗力对比
         e_tem_melting_sort_type_upgrade, //最下层按升级数对比
         e_tem_melting_sort_type_addon, //最下层按追加数对比
         e_tem_melting_sort_type_max
     };
     enum e_unit_type
     {
         e_unit_type_null,
         e_unit_type_player, //玩家
         e_unit_type_npc, //npc
         e_unit_type_monster, //怪物
         e_unit_type_drop_bag, //掉落包
         e_unit_type_trap, //陷阱
         e_unit_type_friend_npc, //友方npc
         e_unit_type_summoned, //召唤物
         e_unit_type_hide, //隐身暴风雪
         e_unit_type_award_mark, //奖励NPC
         e_unit_type_award_play, //玩法NPC
         e_unit_type_max
     };
     enum e_npc_type
     {
         e_npc_type_mission, //任务npc
         e_npc_type_grade_up, //转生npc
         e_npc_type_strong_hold, //军团战据点NPc
         e_npc_type_gather_crystal, //军团战水晶
         e_npc_type_lord_city, //城主膜拜
         e_npc_type_lord_pk, //Pk膜拜
         e_npc_type_lord_sanctuary, //圣域膜拜
         e_npc_type_lord_god, //众神膜拜
         e_npc_type_protect_npc, //要守护的NPC
         e_npc_type_interact_gather, //读条采集Npc
         e_npc_type_touch_gather, //不读条采集Npc
         e_npc_type_hit_and_get_npc, //碰撞采集Npc
         e_npc_type_marry_npc, //结婚npc
         e_npc_type_divorce_npc, //离婚npc
         e_npc_type_world_boss_chests, //世界BOSS掉落宝箱采集
         e_npc_type_island_big_chests, //永恒岛大神像采集
         e_npc_type_island_small_chests, //永恒岛小神像采集
         e_npc_type_cross_pk_chests, //跨服竞技宝箱采集
         e_npc_type_server_harry_chests, //跨服掠夺采集
         e_npc_type_server_harry_submit, //跨服掠夺提交
         e_npc_type_marry_task, //夫妻任务
         e_npc_type_marry_exam, //夫妻答题
         e_npc_type_attack_city_npc, //攻城战复活NPC
         e_npc_type_transfiguration_npc, //攻城战变身
         e_npc_type_transfer, //传送NPC
         e_npc_type_max
     };
     enum e_monster_type
     {
         e_monster_type_nomal, //普通怪物
         e_monster_type_elite, //精英怪物
         e_monster_type_boss, //boss
         e_monster_type_world_boss, //世界Boss
         e_monster_type_gold_army, //黄金部队
         e_monster_type_player_data, //使用玩家数据的怪
         e_monster_type_legion_bonfire, //军团篝火
         e_monster_type_robot_player, //机器人
         e_monster_type_award_boss, //宝箱Boss
         e_monster_type_max
     };
     enum e_hide_type
     {
         e_hide_type_null, //默认
         e_hide_type_fire, //火球
         e_hide_type_snowstorm, //暴风雪
         e_hide_type_max
     };
     enum e_award_mark_level
     {
         e_award_mark_level_small, //小奖
         e_award_mark_level_middle, //中奖
         e_award_mark_level_big, //大奖
         e_award_mark_level_special, //特殊奖励
         e_award_mark_level_max
     };
     enum e_award_mark_type
     {
         e_award_mark_type_nomal, //小奖
         e_award_mark_type_treasure_pic, //藏宝图
         e_award_mark_type_special, //特殊奖励
         e_award_mark_type_bank_robbery, //抢银行的交互NPC
         e_award_mark_type_refrush_monster, //开普通箱子出怪交互
         e_award_mark_type_finger_guess, //猜拳游戏交互NPC
         e_award_mark_type_smashing_bottle, //砸酒瓶交互NPC
         e_award_mark_type_transfer, //传送交互NPC
         e_award_mark_type_max
     };
     enum e_award_play_type
     {
         e_award_play_type_null,
         e_award_play_type_bank_robbery_box, //抢银行拿奖励的宝箱
         e_award_play_type_smashing_bottle, //酒瓶NPC
         e_award_play_type_smashing_bottle_guard, //砸酒瓶看守
         e_award_play_type_smashing_bottle_award, //砸酒瓶宝箱
         e_award_play_type_max
     };
     enum e_drop_bag_type
     {
         e_drop_bag_type_nomal, //普通掉落包
         e_drop_bag_type_award, //开奖掉落包
         e_drop_bag_type_bank_robbery_box, //抢银行开奖产生的掉落包
         e_drop_bag_type_smashing_bottle, //砸酒瓶产生的掉落包
         e_drop_bag_type_max
     };
     enum e_box_award_type
     {
         e_box_award_type_null, //无
         e_box_award_type_player_search_level, //根据玩家探索等级掉落
         e_box_award_type_self_drop, //根据自身掉落包掉落
         e_box_award_type_max
     };
     enum e_activity_type
     {
         e_activity_type_temple, //诅咒神庙
         e_activity_type_empire_treasure, //帝国遗藏
         e_activity_type_pk_king, //PK之王
         e_activity_type_legion_bonfire, //军团篝火
         e_activity_type_broken_sky, //破碎虚空
         e_activity_type_sky_treasure_box, //天空宝箱
         e_activity_type_legion_boss, //军团BOSS
         e_activity_type_city_war, //领土争夺
         e_activity_type_crystal_fairyland, //水晶幻境
         e_activity_type_overlord_city_war, //世界霸主
         e_activity_type_world_elite, //世界精英
         e_activity_type_world_boss, //世界boss
         e_activity_type_ancient_field, //古战场
         e_activity_type_boss_field, //打宝野外
         e_activity_type_boss_home, //打宝boss之家
         e_activity_type_boss_single, //打宝个人
         e_activity_type_cloud_shop, //云购
         e_activity_type_belief_cloister, //信仰回廊
         e_activity_type_cross_server_pk, //跨服PK
         e_activity_type_cross_server_harry, //跨服掠夺
         e_activity_type_cross_ladder, //跨服天梯
         e_activity_type_cross_server_world_boss, //跨服世界boss
         e_activity_type_city_war_cross_server, //跨服军团战
         e_activity_type_legion_answer, //军团答题
         e_activity_type_legion_dance, //军团共舞
         e_activity_type_element_war, //元素争霸
         e_activity_type_attack_city, //攻城战
         e_activity_type_max
     };
     enum e_widget_jump
     {
         e_widget_jump_img_id, //图片
         e_widget_jump_describe_id, //文字描述
         e_widget_jump_widget_id, //主界面名
         e_widget_jump_second_index, //二级界面index
         e_widget_jump_third_index, //三级界面index
         e_widget_jump_max
     };
     enum e_logout_result
     {
         e_logout_result_self, //自己主动下线
         e_logout_result_load_data_fail, //数据加载错误
         e_logout_result_save_data_fail, //数据存档错误
         e_logout_result_link_fail, //网络连接失败
         e_logout_result_time_out, //网络连接超时
         e_logout_result_net_lost, //网络通信丢失
         e_logout_result_ban_account, //账号被禁用
         e_logout_result_new_login, //踢出新上线账户
         e_logout_result_replaced_by_new_login, //被新登录账号替换
         e_logout_result_enter_scene_failed, //进入场景失败
         e_logout_result_leave_scene_failed, //离开场景失败
         e_logout_result_cs_crash, //CS当机
         e_logout_result_ls_crash, //LS当机
         e_logout_result_kick_by_rpc, //网页上踢的
         e_logout_result_no_find_player, //未找到这个玩家
         e_logout_result_fep_id_error, //发错fep了
         e_logout_result_connect_dis, //网络连接断开
         e_logout_result_begin_time, //时间未到不能进入
         e_logout_result_password_error, //密码错误
         e_logout_result_version_wrong, //通讯版本号错误
         e_logout_result_account_error, //账号格式错误
         e_logout_result_queue_full, //队列已满
         e_logout_result_login_full, //登陆人数已满
         e_logout_result_bi, //BI验证失败
         e_logout_result_fake_server_full, //假服务器爆满
         e_logout_result_reconnect_account, //重新连接账号错误
         e_logout_result_login_game_need_update, //登陆游戏检测需要更新
         e_logout_result_enter_game_server_maintenance, //登陆游戏服务器维护中
         e_logout_result_enter_game_no_net, //登陆进入游戏没网
         e_logout_result_online_time_limit, //防沉迷在线时间限制
         e_logout_result_online_duration_over, //防沉迷在线时长超时
         e_logout_result_only_create_role, //服务器未启动
         e_logout_result_max
     };
     enum e_reconnect_result
     {
         e_reconnect_result_win, //成功
         e_reconnect_result_fep_no_find, //fep未找到数据
         e_reconnect_result_ws_no_find, //ws未找到数据
         e_reconnect_result_cs_no_find, //cs未找到数据
         e_reconnect_result_max
     };
     enum e_demons_tower_info
     {
         e_demons_tower_info_cur_challenge_tier, //当前挑战层数
         e_demons_tower_info_left_mopping_up_count, //剩余扫荡次数
         e_demons_tower_info_cur_mopping_up_tier, //当前正在扫荡的层数
         e_demons_tower_info_can_get_award, //是否可以领取奖励
         e_demons_tower_info_max
     };
     enum e_map_category
     {
         e_map_category_common, //通用
         e_map_category_raid, //副本
         e_map_category_activity, //活动
         e_map_category_max
     };
     enum e_money_type
     {
         e_money_type_exp, //经验值
         e_money_type_treasure_pic, //宝藏图
         e_money_type_silver_bind, //绑定游戏币
         e_money_type_jewel, //宝石
         e_money_type_jewel_bind, //绑定宝石
         e_money_type_boss_progress, //boss进度
         e_money_type_magic_crystal, //魔晶
         e_money_type_battle_key, //副本进入货币
         e_money_type_skill_active, //技能激活
         e_money_type_skill_upgrade, //技能升级
         e_money_type_pound, //元素粉末
         e_money_type_guard_point, //守护点数
         e_money_type_achievement_point, //成就点数
         e_money_type_reputation, //声望
         e_money_type_hope_point, //祈福点数
         e_money_type_battle_achievement, //战功
         e_money_type_cross_honor, //荣耀点数
         e_money_type_talent, //天赋点数
         e_money_type_assist_fighting, //助战值
         e_money_type_treasure_score, //宝藏寻宝积分
         e_money_type_belief_rune_piece, //信仰符文分解碎片
         e_money_type_legion_warehouse_score, //军团仓库积分
         e_money_type_attribute_talent, //属性天赋点数
         e_money_type_world_essence, //世界精粹
         e_money_type_cross_server_money, //跨服币
         e_money_type_red_diamond, //红钻
         e_money_type_feather_piece, //神翎之尘
         e_money_type_cross_ladder_honor, //竞技币
         e_money_type_season_point, //赛季积分
         e_money_type_element_score, //勇士币
         e_money_type_element_point, //元素积分
         e_money_type_energy, //体力值
         e_money_type_max
     };
     enum e_team_type
     {
         e_team_type_none, //无
         e_team_type_one_dragon, //一条龙
         e_team_type_mission_daily, //日常任务
         e_team_type_mission_against, //讨伐任务
         e_team_type_act_world_boss, //世界BOSS
         e_team_type_element_war, //元素争霸
         e_team_type_raid_daily_exp, //经验副本
         e_team_type_raid_daily_gold, //金币副本
         e_team_type_raid_multi_0, //熔岩地狱
         e_team_type_raid_sky, //仇恨之海
         e_team_type_raid_temple, //恐惧梦魇
         e_team_type_raid_multi_1, //公主守卫战
         e_team_type_raid_treasure, //堕落幻象
         e_team_type_raid_multi_2, //恐惧深渊
         e_team_type_hang_up, //挂机
         e_team_type_ancient_hang_up, //古战场挂机
         e_team_type_act_world_elite, //世界精英
         e_team_type_max
     };
     enum e_skill_info
     {
         e_skill_info_template_id, //技能模板id
         e_skill_info_cd_cur_time, //冷却剩余时间
         e_skill_info_cd_max_time, //冷却时间最大值
         e_skill_info_activated, //是否激活了
         e_skill_info_skill_type, //技能类型
         e_skill_info_maturity, //技能熟练度
         e_skill_info_use_pos, //使用位置
         e_skill_info_level, //技能等级
         e_skill_info_max
     };
     enum e_mission_end_type
     {
         e_mission_end_type_npc_kill, //杀怪
         e_mission_end_type_dialog, //地点或对话
         e_mission_end_type_mission, //完成任务的任务章节任务
         e_mission_end_type_map, //完成某个地图
         e_mission_end_type_transfer, //瞬移传送
         e_mission_end_type_beat_npc, //击败NPC
         e_mission_end_type_kill_monster_get_item, //杀怪拿物品
         e_mission_end_type_get_item, //采集物品
         e_mission_end_type_equip_upgrade, //装备强化
         e_mission_end_type_equip_addon, //装备追加
         e_mission_end_type_rune_equip, //装备符文
         e_mission_end_type_map_clear_current, //通关副本（当前）
         e_mission_end_type_activity_clear_current, //通关活动（当前）
         e_mission_end_type_mount_unlock, //解锁坐骑
         e_mission_end_type_wing_upgrade, //翅膀升阶
         e_mission_end_type_pokedex_activation, //激活图鉴
         e_mission_end_type_sprite_activation, //精灵激活
         e_mission_end_type_character_level, //角色等级
         e_mission_end_type_map_clear_history, //通关副本（历史）
         e_mission_end_type_clear_daily_mission, //完成日常任务（数量）
         e_mission_end_type_clear_against_mission, //完成讨伐任务（数量）
         e_mission_end_type_clear_demon_tower, //万魔塔通关（层）
         e_mission_end_type_clear_arena, //挑战竞技场（次数）
         e_mission_end_type_cheer_worship, //助力崇拜
         e_mission_end_type_transform, //转换
         e_mission_end_type_equip_levelup, //装备进阶
         e_mission_end_type_add_friend, //添加好友
         e_mission_end_type_purchase_goods, //买东西
         e_mission_end_type_npc_attacked_num, //怪物受击次数
         e_mission_end_type_join_legion, //加入军团
         e_mission_end_type_rune_total_level, //符文总等级
         e_mission_end_type_upgrade_total_level, //强化总等级
         e_mission_end_type_add_on_total_level, //追加总等级
         e_mission_end_type_mount_total_grade, //坐骑总阶数
         e_mission_end_type_join_legion_boss_map, //参加军团BOSS副本
         e_mission_end_type_tinder_total_level, //火种总等级
         e_mission_end_type_equip_target_color, //穿戴指定颜色装备
         e_mission_end_type_legion_donate, //军团捐赠
         e_mission_end_type_boss_field, //打宝
         e_mission_end_type_grade_levelup, //转职
         e_mission_end_type_upgrade_level_num, //强化等级数量
         e_mission_end_type_addon_level_num, //追加等级数量
         e_mission_end_type_awaken_level_num, //觉醒等级数量
         e_mission_end_type_succinct_level_num, //洗练等级数量
         e_mission_end_type_marry_dance, //结婚动作
         e_mission_end_type_reach_level, //达到等级
         e_mission_end_type_learning_skill, //学习指定技能
         e_mission_end_type_arena_military_rank, //竞技场到达指定军衔
         e_mission_end_type_use_item, //使用指定道具
         e_mission_end_type_compound_item, //合成指定道具
         e_mission_end_type_spirit_baptism, //精灵洗礼次数
         e_mission_end_type_spirit_baptism_color, //精灵总洗礼颜色数
         e_mission_end_type_inset_genstone, //镶嵌宝石
         e_mission_end_type_belief_all_num, //信仰到达指定等级
         e_mission_end_type_kill_home_boss, //击杀地宫boss
         e_mission_end_type_new_map_finish, //完成地图(跳转方式不同)
         e_mission_end_type_mount_upgrade_num, //坐骑升级次数
         e_mission_end_type_spirit_upgrade_num, //精灵升级次数
         e_mission_end_type_wing_all_star, //羽翼总星数
         e_mission_end_type_install_equip, //装备指定道具
         e_mission_end_type_unlock_skill_by_mission, //完成任务解锁技能
         e_mission_end_type_auction_goods, //交易行上架物品
         e_mission_end_type_equip_spirit, //装备精灵数量
         e_mission_end_type_assis_fight_num, //进行助战次数
         e_mission_end_type_auction_shopping, //交易行购买物品
         e_mission_end_type_kill_any_home_boss, //击杀任意一只地宫boss
         e_mission_end_type_kill_any_single_boss, //击杀任意一只个人boss
         e_mission_end_type_service_goal, //累计完成指定七日目标次数
         e_mission_end_type_total_login, //累计登陆领取奖励次数
         e_mission_end_type_operatrion_raid, //累计扫荡副本次数
         e_mission_end_type_add_dot, //完成X次加点
         e_mission_end_type_gs_welfare_num, //累计领取战力礼包次数
         e_mission_end_type_level_welfare_num, //累计领取等级礼包次数
         e_mission_end_type_by_assis_fight_num, //被助战次数
         e_mission_end_type_intensify_add_gs, //变强战力提升
         e_mission_end_type_choose_a_fashion, //选择一件指定时装
         e_mission_end_type_active_break_will, //完成指定次数心魔意志激活
         e_mission_end_type_search_count, //探索次数
         e_mission_end_type_active_skill_count, //上阵主动技能个数
         e_mission_end_type_passive_skill_count, //上阵被动技能个数
         e_mission_end_type_active_skill_level, //主动技能达到的等级
         e_mission_end_type_max
     };
     enum e_unlock_func_type
     {
         e_unlock_func_type_skill_0, //普攻技能
         e_unlock_func_type_skill_1, //技能1
         e_unlock_func_type_skill_2, //技能2
         e_unlock_func_type_skill_3, //技能3
         e_unlock_func_type_skill_4, //被动技能1
         e_unlock_func_type_skill_5, //被动技能2
         e_unlock_func_type_auto_battle, //自动战斗
         e_unlock_func_type_mount, //坐骑功能
         e_unlock_func_type_equip_upgrade, //熔炼功能
         e_unlock_func_type_equip_addon, //装备追加
         e_unlock_func_type_wing, //翅膀功能
         e_unlock_func_type_activity, //活动功能
         e_unlock_func_type_convert, //转换功能
         e_unlock_func_type_ranking, //排行榜功能
         e_unlock_func_type_achievement, //成就功能
         e_unlock_func_type_raid_daily_exp, //经验日常副本
         e_unlock_func_type_raid_daily_money, //金钱日常副本
         e_unlock_func_type_raid_demons_tower, //万魔塔副本
         e_unlock_func_type_raid_multiplayer, //多人副本
         e_unlock_func_type_raid_sky_battle, //空战副本
         e_unlock_func_type_raid_drama, //剧情副本
         e_unlock_func_type_max
     };
     enum e_guide_type
     {
         e_guide_type_none, //无
         e_guide_type_operate, //操作引导
         e_guide_type_open_func, //开启功能
         e_guide_type_mask, //黑布遮罩
         e_guide_type_cinematics_anim, //过场动画
         e_guide_type_trigger_event, //触发事件
         e_guide_type_max
     };
     enum e_time_type
     {
         e_time_type_login_time, //最后一次登录时间
         e_time_type_logout_time, //最后一次登出时间
         e_time_type_login_time_all, //总的在线时间
         e_time_type_last_save_data_time, //最近一次存档时间
         e_time_type_last_refresh_time, //下次五点刷新时间
         e_time_type_spirit_last_free_hunt_time, //精灵上次免费猎取时间
         e_time_type_arena_last_update, //竞技场次数上一次刷新时间
         e_time_type_arena_colling_time, //竞技场挑战冷却时间
         e_time_type_hope_item_free_get_time, //上次免费物品祈福时间
         e_time_type_next_free_change_gold_time, //下次免费金币转化时间
         e_time_type_have_meditation_time, //已经冥想的时间
         e_time_type_month_card_activity_time, //月卡当前天数
         e_time_type_month_card_remain_time, //月卡剩余天数
         e_time_type_bag_time, //背包开启剩余时间
         e_time_type_last_daily_info_refresh_time, //上一次按日刷新内容的刷新时间
         e_time_type_ancient_battelfield, //本日古战场已进入时间
         e_time_type_last_map_record_update_time, //上一次地图记录的刷新时间
         e_time_type_cur_level_time, //当前等级时间
         e_time_type_legion_daily_award_flags, //军团功能各种每日领奖标记
         e_time_type_convert_last_update_time, //转换上次刷新时间
         e_time_type_arena_last_get_reward_time, //竞技场上次领奖时间
         e_time_type_map_auto_revive_time, //下次可自动复活时间
         e_time_type_exclusive_card_activity_time, //尊享当前天数
         e_time_type_last_month_info_refresh_time, //上一次按月刷新内容的刷新时间
         e_time_type_welfare_last_refresh_time, //福利刷新时间
         e_time_type_active_degree_last_refresh_time, //活跃度刷新时间
         e_time_type_meditation_full_send_mail, //是否向该玩家发送过冥想已满的邮件
         e_time_type_legion_bonus_award_flags, //分红领奖标记
         e_time_type_vip_experience_level_time, //vip体验等级到期时间
         e_time_type_treasure_jewel_free, //宝石抽奖免费时间
         e_time_type_treasure_element_free, //符文抽奖免费时间
         e_time_type_treasure_spirit_free, //精灵抽奖免费时间
         e_time_type_storage_time, //仓库开启剩余时间
         e_time_type_meditation_remaining_time, //冥想剩余时间
         e_time_type_last_refresh_by_zero_no_cross, //上一次非跨服零点刷新时间
         e_time_type_next_world_boss_can_count, //下次世界boss每日必做可计数时间戳
         e_time_type_last_harry_clear_time, //跨服掠夺上次全部清空时间
         e_time_type_last_refresh_by_zero_cross, //上一次跨服零点刷新时间
         e_time_type_last_refresh_gs_up_start_value_time, //上次零点刷新战力提升榜初始值时间
         e_time_type_frist_recharge_time, //玩家首充时间
         e_time_type_exp_raid_join_time, //每日经验副本已进入时间
         e_time_type_exp_raid_skill_cd, //每日经验副本进入时守护技CD
         e_time_type_exp_raid_extra_time, //每日经验副本额外增加时间
         e_time_type_service_goal_close_time, //七日王者关闭时间
         e_time_type_first_recharge_open_time, //首充界面打开时间
         e_time_type_fuben_level_buy_tip_time, //副本评级买提示时间
         e_time_type_money_buy_tip_time, //紫钻买提示时间
         e_time_type_red_convert_last_time, //红钻免费时间
         e_time_type_energy_back, //体力恢复时间
         e_time_type_offline_award_time, //离线奖励计算时间
         e_time_type_max
     };
     enum e_title_info
     {
         e_title_info_template_id, //称号模板id
         e_title_info_start_time, //称号开始时间
         e_title_info_end_time, //称号结束时间
         e_title_info_if_geted, //是否获取
         e_title_info_if_need_see, //是否需要查看
         e_title_info_if_need_equip, //是否需要装备
         e_title_info_max
     };
     enum e_title_end_type
     {
         e_title_end_type_forever, //永久性
         e_title_end_type_time, //时效性
         e_title_end_type_condition_1, //条件性1可确定结束时间
         e_title_end_type_condition_2, //条件性2不可确定结束时间
         e_title_end_type_max
     };
     enum e_buff_add_judge
     {
         e_buff_add_judge_non, //加不了
         e_buff_add_judge_has_it, //已有此效果
         e_buff_add_judge_has_stronger_one, //有更强的效果
         e_buff_add_judge_can_cover, //覆盖
         e_buff_add_judge_can_wrap, //叠加
         e_buff_add_judge_can_cumulative, //续命
         e_buff_add_judge_direct_add, //直接加
         e_buff_add_judge_can_percent, //增加百分比
         e_buff_add_judge_max
     };
     enum e_spirit_info
     {
         e_spirit_info_template_id, //模板id
         e_spirit_info_spirit_slot, //装备位置
         e_spirit_info_container_type, //背包类型
         e_spirit_info_spirit_is_equiped, //是否装备中
         e_spirit_info_spirit_is_fighting, //是否出战
         e_spirit_info_flair_property_1,
         e_spirit_info_flair_property_2,
         e_spirit_info_flair_property_3,
         e_spirit_info_flair_property_4,
         e_spirit_info_flair_property_5,
         e_spirit_info_flair_property_6,
         e_spirit_info_spirit_typ,
         e_spirit_info_showing_illusion_idex,
         e_spirit_info_illusion_had_byte,
         e_spirit_info_upgrade_count,
         e_spirit_info_star_upgrade_count,
         e_spirit_info_spirit_exp,
         e_spirit_info_random_has_flag,
         e_spirit_info_star_skill_flag,
         e_spirit_info_efficient_skill_id,
         e_spirit_info_efficient_skill_id_2,
         e_spirit_info_max
     };
     enum e_spirit_fetter_info
     {
         e_spirit_fetter_info_template_id, //模板id
         e_spirit_fetter_info_upgrade_level, //强化等级
         e_spirit_fetter_info_max
     };
     enum e_spirit_bag_type
     {
         e_spirit_bag_type_equiped, //装备包
         e_spirit_bag_type_spiritbag, //精灵仓库
         e_spirit_bag_type_itembag, //物品包
         e_spirit_bag_type_max
     };
     enum e_spirit_hunt_type
     {
         e_spirit_hunt_type_free, //免费
         e_spirit_hunt_type_by_voucher, //奖券
         e_spirit_hunt_type_by_ten_voucher, //奖券十连
         e_spirit_hunt_type_one_by_cost, //付费一次
         e_spirit_hunt_type_ten, //十连
         e_spirit_hunt_type_max
     };
     enum e_map_difficulty
     {
         e_map_difficulty_easy, //普通
         e_map_difficulty_normal, //精英
         e_map_difficulty_heard, //炼狱
         e_map_difficulty_max
     };
     enum e_ranking_worship_type
     {
         e_ranking_worship_type_normal, //免费崇拜
         e_ranking_worship_type_money, //金币崇拜
         e_ranking_worship_type_max
     };
     enum e_pk_mode
     {
         e_pk_mode_peace, //和平模式
         e_pk_mode_good_and_evil, //善恶模式
         e_pk_mode_team, //队伍模式
         e_pk_mode_family, //家族模式
         e_pk_mode_slaughter, //杀戮模式
         e_pk_mode_cross_sever, //跨服模式
         e_pk_mode_plunder, //掠夺模式
         e_pk_mode_max
     };
     enum e_pk_mode_type
     {
         e_pk_mode_type_cant, //不可PK
         e_pk_mode_type_allow, //可PK
         e_pk_mode_type_no_kill_point, //可PK但不加杀戮值
         e_pk_mode_type_max
     };
     enum e_recurit_type
     {
         e_recurit_type_none, //无招募
         e_recurit_type_legion_recurit, //军团招募
         e_recurit_type_world_recurit, //世界招募
         e_recurit_type_max
     };
     enum e_fruit_type
     {
         e_fruit_type_none, //非法果实
         e_fruit_type_strength, //力量果实
         e_fruit_type_agility, //敏捷果实
         e_fruit_type_intellect, //智力果实
         e_fruit_type_stamina, //耐力果实
         e_fruit_type_max
     };
     enum e_welfare_type
     {
         e_welfare_type_online_time, //每日在线
         e_welfare_type_total_login, //累计登陆
         e_welfare_type_continue_login, //连续登陆
         e_welfare_type_daily_activity_reward, //每日活跃
         e_welfare_type_monthcard_reward, //月卡返利 月卡签到
         e_welfare_type_daily_recharge_reward, //每日充值
         e_welfare_type_total_recharge_reward, //累计充值
         e_welfare_type_total_cost_reward, //累计消费
         e_welfare_type_exp_level_reward, //等级奖励
         e_welfare_type_gs_reward, //战斗力奖励
         e_welfare_type_first_recharge_reward, //首次充值
         e_welfare_type_first_recharge_geted, //首次充值额外奖励
         e_welfare_type_regular_check_in, //常规签到
         e_welfare_type_exclusive_check_in, //尊享签到
         e_welfare_type_vip_double_check_in, //vip翻倍签到
         e_welfare_type_is_today_check_in, //今天是否签到过
         e_welfare_type_replace_times, //补签次数
         e_welfare_type_time_limit_reward, //限时福利
         e_welfare_type_cumulative_sign_in, //累计签到
         e_welfare_type_weekend_recharge_reward, //周末加赠
         e_welfare_type_max
     };
     enum e_red_package_type
     {
         e_red_package_type_globel, //全服
         e_red_package_type_legion, //军团
         e_red_package_type_vip, //VIP
         e_red_package_type_max
     };
     enum e_role_ws_info
     {
         e_role_ws_info_save_time, //存储时间
         e_role_ws_info_free_red_package_count, //免费红包计数
         e_role_ws_info_max
     };
     enum e_power_up_prompt
     {
         e_power_up_prompt_mount_levelup, //坐骑升级
         e_power_up_prompt_mount_upgrade, //坐骑升阶
         e_power_up_prompt_mount_unlock, //坐骑解锁
         e_power_up_prompt_wing_levelup, //翅膀升星
         e_power_up_prompt_wing_upgrade, //翅膀升阶
         e_power_up_prompt_wing_shengling, //翅膀升灵
         e_power_up_prompt_wing_yuling, //翅膀羽灵
         e_power_up_prompt_wing_juhun, //翅膀聚魂
         e_power_up_prompt_equip_upgrade, //装备可强化
         e_power_up_prompt_equip_add_on, //装备可追加
         e_power_up_prompt_equip_jinjie, //装备可进阶
         e_power_up_prompt_equip_xilian, //装备可洗练
         e_power_up_prompt_equip_chongzhu, //装备可重铸
         e_power_up_prompt_equip_shenzhuangzaizao, //装备可神装再造
         e_power_up_prompt_exchange_amulet, //可兑换护符
         e_power_up_prompt_raise_achievement, //可提升成就
         e_power_up_prompt_raise_rank, //可提升军衔
         e_power_up_prompt_raise_sprite, //可培养精灵
         e_power_up_prompt_role_add_point, //人物加点
         e_power_up_prompt_add_tinder, //加火种
         e_power_up_prompt_mount_beast_spirit, //坐骑兽魂
         e_power_up_prompt_wing_beast_spirit, //翅膀精魂
         e_power_up_prompt_jewel_set, //宝石可镶嵌
         e_power_up_prompt_jewel_composit, //宝石可合成
         e_power_up_prompt_level_package, //领取等级礼包
         e_power_up_prompt_gs_package, //领取战力礼包
         e_power_up_prompt_god_assembled, //神装合成
         e_power_up_prompt_mount_illusion, //坐骑幻化
         e_power_up_prompt_mount_star_grade, //坐骑升魂
         e_power_up_prompt_wing_illusion, //翅膀幻化
         e_power_up_prompt_wing_star_grade, //翅膀升魂
         e_power_up_prompt_fighting_power_analysis, //战力分析
         e_power_up_prompt_element_power_up, //符文提升
         e_power_up_prompt_tinder_skill, //火种技能
         e_power_up_prompt_feather_activate, //翎羽激活
         e_power_up_prompt_feather_upgrade, //翎羽升级
         e_power_up_prompt_feather_skillbook_upgrade, //神翎技能书升级
         e_power_up_prompt_feather_skillbook_EquipOn, //神翎装备技能书
         e_power_up_prompt_Phantom_upgrade, //幻灵提升
         e_power_up_prompt_goddess, //神祈养成
         e_power_up_prompt_goddess_chain, //神祈星铭
         e_power_up_prompt_mount_star, //坐骑赋能
         e_power_up_prompt_wing_psychic, //羽翼通灵
         e_power_up_prompt_equip_awaken, //装备可觉醒
         e_power_up_prompt_max
     };
     enum e_sound_type
     {
         e_sound_type_sound, //背景音效
         e_sound_type_music, //背景音乐
         e_sound_type_max
     };
     enum e_pk_king_competition_stage
     {
         e_pk_king_competition_stage_preliminary, //预赛
         e_pk_king_competition_stage_final, //决赛
         e_pk_king_competition_stage_max
     };
     enum e_small_map_show_unit_type
     {
         e_small_map_show_unit_type_monster, //小怪
         e_small_map_show_unit_type_boss, //Boss
         e_small_map_show_unit_type_partner, //队友
         e_small_map_show_unit_type_target, //目标点
         e_small_map_show_unit_type_target_in_map_border, //目标点位于地图边缘
         e_small_map_show_unit_type_city_war_born_point, //军团城战出生点
         e_small_map_show_unit_type_world_boss, //世界boss
         e_small_map_show_unit_type_max
     };
     enum e_money_tuple
     {
         e_money_tuple_id, //钱的ID
         e_money_tuple_num, //钱的数量
         e_money_tuple_max
     };
     enum e_item_tuple
     {
         e_item_tuple_id, //物品的ID
         e_item_tuple_num, //物品的数量
         e_item_tuple_max
     };
     enum e_recruit_activites_type
     {
         e_recruit_activites_type_fuben, //副本
         e_recruit_activites_type_hunting, //狩猎活动
         e_recruit_activites_type_max
     };
     enum e_widget_show_mode
     {
         e_widget_show_mode_normal, //正常
         e_widget_show_mode_OnlyDeleteBySelf, //只能被自己关闭不能被CloseAll关闭
         e_widget_show_mode_HiddenInGuide, //引导时隐藏不关闭引导结束显示
         e_widget_show_mode_OnlyDeleteBySelfAndHiddenInGuide, //引导时隐藏不关闭引导结束显示
         e_widget_show_mode_max
     };
     enum e_arena_end_type
     {
         e_arena_end_type_failed, //失败
         e_arena_end_type_win, //胜利
         e_arena_end_type_draw, //平局
         e_arena_end_type_max
     };
     enum e_item_upgrade_effect_type
     {
         e_item_upgrade_effect_type_wing_add_star, //翅膀升星
         e_item_upgrade_effect_type_wing_upgrade, //翅膀升阶
         e_item_upgrade_effect_type_mount_upgrade, //坐骑升级
         e_item_upgrade_effect_type_mount_upgrade_class, //坐骑升阶
         e_item_upgrade_effect_type_fashion_upgrade, //时装升星
         e_item_upgrade_effect_type_max
     };
     enum e_map_info_panel_type
     {
         e_map_info_panel_type_none, //无
         e_map_info_panel_type_wave, //波次信息
         e_map_info_panel_type_damage_rank, //伤害排名
         e_map_info_panel_type_boss_buff, //boss提供的BUFF
         e_map_info_panel_type_final_boss, //最终BOSS
         e_map_info_panel_type_protect_npc, //守护NPC的状态
         e_map_info_panel_type_wave_timer, //波次倒计时
         e_map_info_panel_type_score_rank, //积分排名信息
         e_map_info_panel_type_score_get_reward, //积分达成奖励
         e_map_info_panel_type_aim_monster_killed_num, //目标怪物击杀数量
         e_map_info_panel_type_exp_raid, //经验副本专用
         e_map_info_panel_type_boss_home, //boss之家和vipboss之家专用
         e_map_info_panel_type_tower_reward, //万魔塔奖励显示控件
         e_map_info_panel_type_crystak_raid, //水晶环境显示控件
         e_map_info_panel_type_relic_raid, //上古遗迹显示控件
         e_map_info_panel_type_belief_home, //信仰回廊显示控件
         e_map_info_panel_type_answer_question, //军团答题显示控件
         e_map_info_panel_type_legion_station, //军团驻地控件
         e_map_info_panel_type_pk_king, //Pk之王专属控件
         e_map_info_panel_type_element_war, //元素争霸显示控件
         e_map_info_panel_type_max
     };
     enum e_legion_member_info
     {
         e_legion_member_info_is_online, //军团成员是否在线
         e_legion_member_info_level, //军团成员的等级
         e_legion_member_info_player_template_id, //军团成员的角色模板
         e_legion_member_info_fighting_power, //军团成员的战斗力
         e_legion_member_info_job_title, //职位
         e_legion_member_info_city_war_score, //在城战中获得的分数
         e_legion_member_info_donate_contribution, //捐赠的贡献度
         e_legion_member_info_add_fuel_times, //篝火活动拱火次数
         e_legion_member_info_last_logout_stamp, //上次离线时间戳
         e_legion_member_info_last_get_city_award, //城战领奖记录
         e_legion_member_info_agora_id, //AgoraID
         e_legion_member_info_join_time_stamp, //加入军团时间戳
         e_legion_member_info_vip_level, //军团成员的VIP等级
         e_legion_member_info_get_master_city_award, //霸主战领奖记录
         e_legion_member_info_week_contribution, //每周贡献度
         e_legion_member_info_is_show_vip, //是否显示vip
         e_legion_member_info_max
     };
     enum e_first_time_do_type
     {
         e_first_time_do_type_none, //无
         e_first_time_do_type_composit, //第一次合成
         e_first_time_do_type_pop_power_up_prompt_widget, //第一次弹出变强指引控件
         e_first_time_do_type_sky_raid, //第一次做空战副本
         e_first_time_do_type_close_raid_win_widget, //第一次关闭结算界面
         e_first_time_do_type_equip_upgrade, //第一次装备强化
         e_first_time_do_type_lucky_draw_sprite, //第一次抽精灵
         e_first_time_do_type_lucky_draw_item, //第一次抽装备
         e_first_time_do_type_get_equip, //第一次获得装备
         e_first_time_do_type_lucky_draw_elementheart, //第一次抽取元素之心
         e_first_time_do_type_killed, //第一次死亡
         e_first_time_do_type_open_legion_store, //第一次打开军团商店
         e_first_time_do_type_open_old_store, //第一次打开古战场商店
         e_first_time_do_type_open_hope_store, //第一次打开祈愿商店
         e_first_time_do_type_exp_raid, //第一次进入经验副本
         e_first_time_do_type_online_reward, //第一次点击领取在线福利
         e_first_time_do_type_spirit_forge, //第一次精灵洗礼
         e_first_time_do_type_auction_buy, //第一次拍卖购买
         e_first_time_do_type_auction_sell, //第一次拍卖出售
         e_first_time_do_type_max
     };
     enum e_move_type
     {
         e_move_type_stop, //停止
         e_move_type_front, //向前
         e_move_type_back, //向后
         e_move_type_left, //向左
         e_move_type_right, //向右
         e_move_type_max
     };
     enum e_revive_type
     {
         e_revive_type_transfer, //回城复活
         e_revive_type_born_pos, //出生点复活
         e_revive_type_free_revive_by_CD, //免费按CD原地复活
         e_revive_type_same_pos, //原地复活
         e_revive_type_cant_revive, //不可复活
         e_revive_type_max
     };
     enum e_team_state
     {
         e_team_state_none, //无状态
         e_team_state_waiting_join, //等待成员加入
         e_team_state_waiting_ready, //准备中中
         e_team_state_game, //游戏中
         e_team_state_max
     };
     enum e_pk_info
     {
         e_pk_info_pk_mode, //pk模式
         e_pk_info_community_type, //玩家阵营
         e_pk_info_max
     };
     enum e_region_type
     {
         e_region_type_zhongli, //中立区
         e_region_type_danger_zhongli, //危险中立区
         e_region_type_safe, //安全区
         e_region_type_protect, //受保护区
         e_region_type_max
     };
     enum e_npc_interact_state
     {
         e_npc_interact_state_none, //无
         e_npc_interact_state_begin, //开始交互
         e_npc_interact_state_stop, //停止交互
         e_npc_interact_state_finish, //完成交互
         e_npc_interact_state_max
     };
     enum e_mission_operate_type
     {
         e_mission_operate_type_accept, //接取
         e_mission_operate_type_finish, //交付
         e_mission_operate_type_refresh_star, //刷星
         e_mission_operate_type_dialog, //对话
         e_mission_operate_type_finish_all_daily, //一键完成
         e_mission_operate_type_special_finish, //钻石完成双倍经验
         e_mission_operate_type_create_npc, //创建任务需要的npc
         e_mission_operate_type_max
     };
     enum e_protect_mark_type
     {
         e_protect_mark_type_single_equip_upgrade_history_record, //单件装备强化等级历史最高纪录
         e_protect_mark_type_total_equip_upgrade_num_history_record, //装备强化总等级
         e_protect_mark_type_total_equip_addon_num_history_record, //装备追加总等级
         e_protect_mark_type_equip_succinct_max_highest_color, //装备洗练升满最高品质
         e_protect_mark_type_total_mount_grade, //坐骑总阶数
         e_protect_mark_type_mount_num, //激活的坐骑个数
         e_protect_mark_type_wing_and_feather_total_grade, //当前翅膀阶数+翎羽总阶数
         e_protect_mark_type_pokedex_num, //图鉴激活数
         e_protect_mark_type_total_sprite_equip_level_history_record, //精灵总等级
         e_protect_mark_type_total_belief_grade, //信仰总阶数
         e_protect_mark_type_role_exp_level, //等级
         e_protect_mark_type_role_military_rank, //军衔
         e_protect_mark_type_achievement, //成就
         e_protect_mark_type_total_element_heart_equip_level_history_record, //元素之心总等级
         e_protect_mark_type_max
     };
     enum e_patron_saint_info
     {
         e_patron_saint_info_template_id, //模板id
         e_patron_saint_info_last_update_time, //上一次刷新时间
         e_patron_saint_info_psyche_recovery_num, //精魄已经回收的数量
         e_patron_saint_info_psyche_oneday_limit, //精魄当日回收的上限
         e_patron_saint_info_psyche_today_exceed_num, //今天多吃掉的一个精魄超出上限无法激活的那部分保留到次日再使用
         e_patron_saint_info_upgrade_num, //升级次数
         e_patron_saint_info_max
     };
     enum e_role_history_highest_record
     {
         e_role_history_highest_record_single_equip_upgrade, //单件装备强化历史最高值
         e_role_history_highest_record_total_equip_upgrade_num, //全身装备强化次数和历史最高值
         e_role_history_highest_record_total_equip_addon_num, //全身装备追加次数和历史最高值
         e_role_history_highest_record_item_succinct_reach_mark_level, //洗练装备到印记升级要求第几层
         e_role_history_highest_record_total_sprite_equip_level, //同时装备精灵等级和历史最高值
         e_role_history_highest_record_total_element_heart_equip_level, //同时装备元素之心等级和历史最高值
         e_role_history_highest_record_arena_higest, //竞技场历史最高
         e_role_history_highest_record_demon_tower_highest_level, //万魔塔历史最高层
         e_role_history_highest_record_higest_friend_num, //拥有好友数量
         e_role_history_highest_record_higest_friendliness_value, //最高好友度
         e_role_history_highest_record_higest_weapon_upgrade_buff, //武器强化Buff历史最高值
         e_role_history_highest_record_higest_equip_upgrade_buff, //装备强化Buff历史最高值
         e_role_history_highest_record_rune_total_level, //符文最高总等级
         e_role_history_highest_record_addon_weapon, //追加武器最高等级
         e_role_history_highest_record_addon_equip, //追加装备最高等级
         e_role_history_highest_record_belief_spring, //火系信仰
         e_role_history_highest_record_belief_summer, //冰系信仰
         e_role_history_highest_record_belief_autumn, //光系信仰
         e_role_history_highest_record_belief_winter, //暗系信仰
         e_role_history_highest_record_weapon_full_succinct_grade, //最高满洗练武器阶数 真实阶数加1
         e_role_history_highest_record_equip_full_succinct_grade, //最低全身满洗练装备阶数 真实阶数加1
         e_role_history_highest_record_battle_achievement, //累计获得的最高军功值
         e_role_history_highest_record_gs_value, //总战力最高值
         e_role_history_highest_record_mount_value, //坐骑战力最高值
         e_role_history_highest_record_wing_value, //翅膀战力最高值
         e_role_history_highest_record_equip_value, //装备战力最高值
         e_role_history_highest_record_spirit_value, //精灵战力最高值
         e_role_history_highest_record_max_awaken_level, //装备觉醒最高值
         e_role_history_highest_record_max_forge_level, //装备重铸最高值
         e_role_history_highest_record_max_equip_enchant_level, //防具附魔最高值
         e_role_history_highest_record_max_jewelry_enchant_level, //首饰附魔最高值
         e_role_history_highest_record_max_spirit_composit_level, //精灵合成最高值
         e_role_history_highest_record_max
     };
     enum e_patron_saint_mark_operate_type
     {
         e_patron_saint_mark_operate_type_purchase_new_slot, //买新格子
         e_patron_saint_mark_operate_type_equip_mark, //装备
         e_patron_saint_mark_operate_type_unequip_mark, //卸下
         e_patron_saint_mark_operate_type_upgrade, //升级
         e_patron_saint_mark_operate_type_equip_prop, //装备火种道具
         e_patron_saint_mark_operate_type_unequip_prop, //卸下火种道具
         e_patron_saint_mark_operate_type_purchase_prop_new_slot, //购买火种道具新格子
         e_patron_saint_mark_operate_type_max
     };
     enum e_belief_info
     {
         e_belief_info_template_id, //模版ID
         e_belief_info_upgrade_count, //升级次数
         e_belief_info_try_upgrade_times, //尝试升级次数
         e_belief_info_max
     };
     enum e_talent_info
     {
         e_talent_info_template_id, //模板id
         e_talent_info_cast_money, //总共消耗多少点天赋在这个节点上
         e_talent_info_max
     };
     enum e_talent_layer
     {
         e_talent_layer_need_id, //需求的id<100层级>100talent_list_template_id
         e_talent_layer_need_num, //需求的数量
         e_talent_layer_template_id_0, //这一层的天赋id
         e_talent_layer_template_id_1, //这一层的天赋id
         e_talent_layer_template_id_2, //这一层的天赋id
         e_talent_layer_template_id_3, //这一层的天赋id
         e_talent_layer_max
     };
     enum e_relation_data
     {
         e_relation_data_type, //类型
         e_relation_data_level, //等级
         e_relation_data_template_id, //模板id
         e_relation_data_gs, //战斗力
         e_relation_data_relation_state, //关系人状态
         e_relation_data_last_login_time, //关系人最近一次登录时间
         e_relation_data_friendliness, //友好度
         e_relation_data_vip_title_template_id, //Vip称号等级
         e_relation_data_max
     };
     enum e_role_equip_slot
     {
         e_role_equip_slot_body, //人物身体骨骼模型
         e_role_equip_slot_weapon_1, //武器1
         e_role_equip_slot_hat, //帽子
         e_role_equip_slot_earring, //耳环
         e_role_equip_slot_necklace, //项链
         e_role_equip_slot_armor, //身体防具
         e_role_equip_slot_pants, //腿部,腿甲、裤子
         e_role_equip_slot_ring, //戒指
         e_role_equip_slot_glove, //手部
         e_role_equip_slot_boot, //脚部,靴子
         e_role_equip_slot_amulet, //护符
         e_role_equip_slot_magic_1, //魔法书1
         e_role_equip_slot_magic_2, //魔法书2
         e_role_equip_slot_wing, //翅膀
         e_role_equip_slot_mount, //坐骑
         e_role_equip_slot_sprite, //精灵
         e_role_equip_slot_mantle, //背部披风
         e_role_equip_slot_badge, //徽章
         e_role_equip_slot_head, //人物头部骨骼模型
         e_role_equip_slot_hair, //人物头发骨骼模型
         e_role_equip_slot_headdress, //人物头饰骨骼模型
         e_role_equip_slot_mouth, //人物口罩骨骼模型
         e_role_equip_slot_eye, //人物眼睛骨骼模型
         e_role_equip_slot_max
     };
     enum e_item_type
     {
         e_item_type_none,
         e_item_type_weapon, //武器
         e_item_type_armor, //防具
         e_item_type_mount, //坐骑
         e_item_type_expendable, //消耗品
         e_item_type_stone, //宝石类
         e_item_type_fashion, //时装类
         e_item_type_mission, //任务类
         e_item_type_spirit, //精灵
         e_item_type_wing, //翅膀
         e_item_type_element_heart, //元素之心
         e_item_type_protect, //守护之灵
         e_item_type_beast_spirit, //兽魂
         e_item_type_time_limit, //限时物品
         e_item_type_belief_rune, //信仰符文
         e_item_type_sky_equip, //天空装备
         e_item_type_skill_book, //技能书
         e_item_type_goddess_equip, //女神装备
         e_item_type_wedding_equip, //结婚装备
         e_item_type_skygod_equip, //天神装备
         e_item_type_mount_equip, //坐骑装备
         e_item_type_supreme_equip, //神王装备
         e_item_type_star_map_spirit, //星图精灵
         e_item_type_mount_star, //坐骑赋能
         e_item_type_wing_psychic, //羽翼通灵
         e_item_type_break_will, //心魔意志装备
         e_item_type_awaken_item, //觉醒羁绊装备
         e_item_type_max
     };
     enum e_armor_sub_type
     {
         e_armor_sub_type_none,
         e_armor_sub_type_hat, //头部防具
         e_armor_sub_type_earring, //耳环
         e_armor_sub_type_necklace, //项链
         e_armor_sub_type_armor, //身体防具
         e_armor_sub_type_pants, //腿部腿甲、裤子
         e_armor_sub_type_ring, //戒指
         e_armor_sub_type_glove, //手部
         e_armor_sub_type_boot, //脚部靴子
         e_armor_sub_type_amulet, //护符
         e_armor_sub_type_fashion, //时装
         e_armor_sub_type_wedding, //婚戒
         e_armor_sub_type_spirit, //精灵
         e_armor_sub_type_mantle, //背部披风
         e_armor_sub_type_max
     };
     enum e_prop_sub_type
     {
         e_prop_sub_type_none,
         e_prop_sub_type_fruit, //果实
         e_prop_sub_type_gife, //礼包
         e_prop_sub_type_money, //加钱的卡片
         e_prop_sub_type_hp_min, //小血瓶
         e_prop_sub_type_hp_mld, //中血瓶
         e_prop_sub_type_hp_max, //大血瓶
         e_prop_sub_type_wing_shap_card, //翅膀外形卡
         e_prop_sub_type_title_card, //称号卡
         e_prop_sub_type_hp_shop, //加大血瓶
         e_prop_sub_type_hp_essence, //超大血瓶
         e_prop_sub_type_store_medicine, //商城药剂
         e_prop_sub_type_gain_medicine, //增益药剂
         e_prop_sub_type_min_level_up_medicine, //升级丹
         e_prop_sub_type_mid_level_up_medicine, //超级升级丹
         e_prop_sub_type_max_level_up_medicine, //至尊升级丹
         e_prop_sub_type_wash_talent_water, //洗点水
         e_prop_sub_type_legion_donate, //军团捐赠物资
         e_prop_sub_type_reduce_Pk_value, //洗pk值
         e_prop_sub_type_mount_illusion_unlock, //坐骑幻化解锁道具
         e_prop_sub_type_wing_illusion_unlock, //羽翼幻化解锁道具
         e_prop_sub_type_element_package, //符文包裹
         e_prop_sub_type_vip_card, //VIP卡
         e_prop_sub_type_rand_equip, //随机装备
         e_prop_sub_type_globel_red_package, //世界红包
         e_prop_sub_type_legion_red_package, //军团红包
         e_prop_sub_type_mount_shap_card, //坐骑外形卡
         e_prop_sub_type_marry_prop, //结婚道具
         e_prop_sub_type_diamonds_package, //钻石礼包
         e_prop_sub_type_refresh_body_att, //洗属性
         e_prop_sub_type_time_limit_prop, //限时道具
         e_prop_sub_type_recharge_package, //充值礼包
         e_prop_sub_type_vip_experience_card, //VIP体验卡
         e_prop_sub_type_const_att_equip, //固定卓越属性装备
         e_prop_sub_type_exp_addition, //使用获得经验加成
         e_prop_sub_type_add_map_count, //增加地图进入次数
         e_prop_sub_type_add_exp_with_num, //按照系数动态增加经验
         e_prop_sub_type_wing_experience, //自动使用的翅膀限时卡（无视限制直接带上翅膀）
         e_prop_sub_type_wing_piece, //翅膀碎片
         e_prop_sub_type_change_class_card, //转职卡
         e_prop_sub_type_change_name_card, //改名卡
         e_prop_sub_type_choose_gife, //选择礼包
         e_prop_sub_type_meditation_medicine, //冥想药水
         e_prop_sub_type_fashion_prop, //时装升级道具
         e_prop_sub_type_tinder_skill_book, //火种技能书
         e_prop_sub_type_grade_up, //突破道具
         e_prop_sub_type_legion_change_name_card, //军团改名卡
         e_prop_sub_type_fireworks, //烟花
         e_prop_sub_type_horn, //喇叭
         e_prop_sub_type_area_horn, //全区喇叭
         e_prop_sub_type_add_map_time, //增加地图时间道具
         e_prop_sub_type_appearance_card, //捏脸卡
         e_prop_sub_type_add_already_time, //减少地宫boss疲劳值
         e_prop_sub_type_add_single_boss_times, //减少个人Boss Times
         e_prop_sub_type_awken_item, //觉醒消耗道具
         e_prop_sub_type_sprite_upgrade, //精灵升级道具
         e_prop_sub_type_max
     };
     enum e_stone_sub_type
     {
         e_stone_sub_type_none,
         e_stone_sub_type_equip_upgrade, //强化素材
         e_stone_sub_type_equip_addon, //追加素材
         e_stone_sub_type_mount, //坐骑素材
         e_stone_sub_type_addsoul, //注魂素材
         e_stone_sub_type_addspririt, //注灵素材
         e_stone_sub_type_refine, //洗炼素材
         e_stone_sub_type_upgrade_protect, //强化保护素材
         e_stone_sub_type_mount_upgrade, //坐骑升级材料
         e_stone_sub_type_mount_upstair, //坐骑升阶材料
         e_stone_sub_type_mount_unlock, //坐骑解锁材料
         e_stone_sub_type_mount_unlock_highgrade, //高级坐骑解锁材料
         e_stone_sub_type_wing_upgrade, //羽翼升级材料
         e_stone_sub_type_wing_upstair, //羽翼升阶材料
         e_stone_sub_type_wing_highgrade, //高级羽翼材料
         e_stone_sub_type_orangered_shard, //橙、红装碎片
         e_stone_sub_type_basic_synthetic_material, //基础合成材料
         e_stone_sub_type_spirit_shard, //精灵碎片
         e_stone_sub_type_resurection_stone, //复活石
         e_stone_sub_type_dragon_soul, //龙魂
         e_stone_sub_type_debris, //神器碎片
         e_stone_sub_type_psyche, //精魄
         e_stone_sub_type_jewel, //宝石
         e_stone_sub_type_book_card, //图鉴碎片
         e_stone_sub_type_tinder_prop, //火种道具
         e_stone_sub_type_mount_equip, //坐骑装备碎片
         e_stone_sub_type_max
     };
     enum e_feather_sub_type
     {
         e_feather_sub_type_none,
         e_feather_sub_type_feather1,
         e_feather_sub_type_feather2,
         e_feather_sub_type_feather3,
         e_feather_sub_type_feather4,
         e_feather_sub_type_max
     };
     enum e_wing_sub_type
     {
         e_wing_sub_type_none,
         e_wing_sub_type_wing, //翅膀本体
         e_wing_sub_type_feather, //翅膀翎羽
         e_wing_sub_type_soul, //翅膀注魂
         e_wing_sub_type_spirit, //翅膀注灵
         e_wing_sub_type_illusion, //翅膀幻化
         e_wing_sub_type_max
     };
     enum e_element_heart_sub_type
     {
         e_element_heart_sub_type_equip, //可佩带
         e_element_heart_sub_type_exp, //经验用
         e_element_heart_sub_type_max
     };
     enum e_store_type
     {
         e_store_type_non, //空
         e_store_type_recommend, //推荐商店
         e_store_type_daily, //日常商店
         e_store_type_expendables, //消耗品
         e_store_type_bind_jewel_0, //绑钻商店_0
         e_store_type_shape, //外形商店
         e_store_type_limit_buy, //限购商店
         e_store_type_sprite_first, //精灵兑换1
         e_store_type_sprite_second, //精灵兑换2
         e_store_type_sprite_third, //精灵兑换3
         e_store_type_magic_crystal_first, //魔晶1
         e_store_type_magic_crystal_second, //魔晶2
         e_store_type_magic_crystal_third, //魔晶3
         e_store_type_medicine, //药品
         e_store_type_prop, //道具
         e_store_type_hope_point_first, //祈福积分1
         e_store_type_hope_point_second, //祈福积分2
         e_store_type_hope_point_third, //祈福积分3
         e_store_type_legion, //军团商店
         e_store_type_hornor, //荣誉商店
         e_store_type_assist_fighting, //助战商店
         e_store_type_fashion, //时装
         e_store_type_bind_jewel_1, //绑钻商店_1
         e_store_type_bind_jewel_2, //绑钻商店_2
         e_store_type_treasure_royal, //宝藏宝石积分商店
         e_store_type_treasure_element, //宝藏符文积分商店
         e_store_type_cross_server_buy, //跨服商店
         e_store_type_red_diamond, //红钻商店
         e_store_type_cross_ladder_store, //跨服天梯荣誉商店
         e_store_type_non_29, //预留
         e_store_type_non_30, //预留
         e_store_type_non_31, //预留
         e_store_type_non_32, //预留
         e_store_type_non_33, //预留
         e_store_type_non_34, //预留
         e_store_type_non_35, //预留
         e_store_type_non_36, //预留
         e_store_type_non_37, //预留
         e_store_type_non_38, //预留
         e_store_type_non_39, //预留
         e_store_type_non_40, //预留
         e_store_type_limit_buy_41, //限时商店41
         e_store_type_limit_buy_42, //限时商店42
         e_store_type_limit_buy_43, //限时商店43
         e_store_type_limit_buy_44, //限时商店44
         e_store_type_limit_buy_45, //限时商店45
         e_store_type_limit_buy_46, //限时商店46
         e_store_type_limit_buy_47, //限时商店47
         e_store_type_limit_buy_48, //限时商店48
         e_store_type_limit_buy_49, //限时商店49
         e_store_type_limit_buy_50, //限时商店50
         e_store_type_limit_buy_51, //限时商店51
         e_store_type_limit_buy_52, //限时商店52
         e_store_type_limit_buy_53, //限时商店53
         e_store_type_limit_buy_54, //限时商店54
         e_store_type_limit_buy_55, //限时商店55
         e_store_type_limit_buy_56, //限时商店56
         e_store_type_limit_buy_57, //限时商店57
         e_store_type_limit_buy_58, //限时商店58
         e_store_type_limit_buy_59, //限时商店59
         e_store_type_limit_buy_60, //限时商店60
         e_store_type_limit_buy_61, //限时商店61
         e_store_type_limit_buy_62, //限时商店62
         e_store_type_limit_buy_63, //限时商店63
         e_store_type_limit_buy_64, //限时商店64
         e_store_type_limit_buy_65, //限时商店65
         e_store_type_limit_buy_66, //限时商店66
         e_store_type_limit_buy_67, //限时商店67
         e_store_type_limit_buy_68, //限时商店68
         e_store_type_limit_buy_69, //限时商店69
         e_store_type_limit_buy_70, //限时商店70
         e_store_type_limit_buy_71, //限时商店71
         e_store_type_limit_buy_72, //限时商店72
         e_store_type_limit_buy_73, //限时商店73
         e_store_type_limit_buy_74, //限时商店74
         e_store_type_limit_buy_75, //限时商店75
         e_store_type_limit_buy_76, //限时商店76
         e_store_type_limit_buy_77, //限时商店77
         e_store_type_limit_buy_78, //限时商店78
         e_store_type_limit_buy_79, //限时商店79
         e_store_type_limit_buy_80, //限时商店80
         e_store_type_limit_buy_81, //限时商店81
         e_store_type_limit_buy_82, //限时商店82
         e_store_type_limit_buy_83, //限时商店83
         e_store_type_limit_buy_84, //限时商店84
         e_store_type_limit_buy_85, //限时商店85
         e_store_type_limit_buy_86, //限时商店86
         e_store_type_limit_buy_87, //限时商店87
         e_store_type_limit_buy_88, //限时商店88
         e_store_type_limit_buy_89, //限时商店89
         e_store_type_limit_buy_90, //限时商店90
         e_store_type_limit_buy_91, //限时商店91
         e_store_type_limit_buy_92, //限时商店92
         e_store_type_limit_buy_93, //限时商店93
         e_store_type_limit_buy_94, //限时商店94
         e_store_type_limit_buy_95, //限时商店95
         e_store_type_limit_buy_96, //限时商店96
         e_store_type_limit_buy_97, //限时商店97
         e_store_type_limit_buy_98, //限时商店98
         e_store_type_limit_buy_99, //限时商店99
         e_store_type_limit_buy_100, //限时商店100
         e_store_type_limit_buy_101, //限时商店101
         e_store_type_limit_buy_102, //限时商店102
         e_store_type_limit_buy_103, //限时商店103
         e_store_type_limit_buy_104, //限时商店104
         e_store_type_limit_buy_105, //限时商店105
         e_store_type_limit_buy_106, //限时商店106
         e_store_type_limit_buy_107, //限时商店107
         e_store_type_limit_buy_108, //限时商店108
         e_store_type_limit_buy_109, //限时商店109
         e_store_type_limit_buy_110, //限时商店110
         e_store_type_limit_buy_111, //限时商店111
         e_store_type_limit_buy_112, //限时商店112
         e_store_type_limit_buy_113, //限时商店113
         e_store_type_limit_buy_114, //限时商店114
         e_store_type_limit_buy_115, //限时商店115
         e_store_type_limit_buy_116, //限时商店116
         e_store_type_limit_buy_117, //限时商店117
         e_store_type_limit_buy_118, //限时商店118
         e_store_type_limit_buy_119, //限时商店119
         e_store_type_limit_buy_120, //限时商店120
         e_store_type_limit_buy_121, //限时商店121
         e_store_type_limit_buy_122, //限时商店122
         e_store_type_limit_buy_123, //限时商店123
         e_store_type_limit_buy_124, //限时商店124
         e_store_type_limit_buy_125, //限时商店125
         e_store_type_limit_buy_126, //限时商店126
         e_store_type_limit_buy_127, //限时商店127
         e_store_type_limit_buy_128, //限时商店128
         e_store_type_limit_buy_129, //限时商店129
         e_store_type_limit_buy_130, //限时商店130
         e_store_type_limit_buy_131, //限时商店131
         e_store_type_limit_buy_132, //限时商店132
         e_store_type_limit_buy_133, //限时商店133
         e_store_type_limit_buy_134, //限时商店134
         e_store_type_limit_buy_135, //限时商店135
         e_store_type_limit_buy_136, //限时商店136
         e_store_type_limit_buy_137, //限时商店137
         e_store_type_limit_buy_138, //限时商店138
         e_store_type_limit_buy_139, //限时商店139
         e_store_type_limit_buy_140, //限时商店140
         e_store_type_max
     };
     enum e_element_heart_operate
     {
         e_element_heart_operate_get_one, //抽一个
         e_element_heart_operate_get_ten, //抽十个
         e_element_heart_operate_upgrade, //升级
         e_element_heart_operate_equip_on, //装备
         e_element_heart_operate_equip_off, //卸下
         e_element_heart_operate_summon, //召唤
         e_element_heart_operate_tidy, //整理
         e_element_heart_operate_purchase_equip_slot, //买格子
         e_element_heart_operate_recovery, //出售
         e_element_heart_operate_common, //普通
         e_element_heart_operate_superior, //精良
         e_element_heart_operate_rare, //稀有
         e_element_heart_operate_epic, //史诗
         e_element_heart_operate_legend, //传说
         e_element_heart_operate_merge, //符文融合
         e_element_heart_operate_core_unlock, //核心符文解锁
         e_element_heart_operate_core_upgrade, //核心符文升级
         e_element_heart_operate_max
     };
     enum e_top_tip_type
     {
         e_top_tip_type_normal, //普通消息
         e_top_tip_type_important, //重要通知
         e_top_tip_type_max
     };
     enum e_sprite_operate
     {
         e_sprite_operate_fight, //出战/召回
         e_sprite_operate_equip, //装备
         e_sprite_operate_unequip, //卸下
         e_sprite_operate_upgrade, //升级
         e_sprite_operate_sell, //出售
         e_sprite_operate_send_to_item_bag, //送进背包
         e_sprite_operate_tidy, //整理
         e_sprite_operate_auto_sell, //自动出售
         e_sprite_operate_purchase_equip_slot, //购买装备槽
         e_sprite_operate_purchase_illusion, //购买幻化
         e_sprite_operate_use_illusion, //启用幻化
         e_sprite_operate_star_upgrade, //魂星升级
         e_sprite_operate_max
     };
     enum e_chat_type
     {
         e_chat_type_none,
         e_chat_type_system, //系统信息
         e_chat_type_world, //世界信息
         e_chat_type_legion, //军团信息
         e_chat_type_team, //组队信息
         e_chat_type_private, //私聊信息
         e_chat_type_near, //附近aoi
         e_chat_type_total, //所有综合 不用于发送只用于显示
         e_chat_type_friend, //好友聊天
         e_chat_type_world_recruit, //世界招募
         e_chat_type_legion_recruit, //军团招募
         e_chat_type_radio_host, //主播频道
         e_chat_type_server, //跨服频道
         e_chat_type_all_server, //大跨服频道
         e_chat_type_max
     };
     enum e_recharge_type
     {
         e_recharge_type_none, //未充值
         e_recharge_type_vip, //vip
         e_recharge_type_month_card, //月卡
         e_recharge_type_exclusive, //尊享
         e_recharge_type_max
     };
     enum e_relationlist_type
     {
         e_relationlist_type_friend, //好友列表
         e_relationlist_type_black, //屏蔽列表
         e_relationlist_type_marry, //婚姻列表
         e_relationlist_type_hate, //仇恨列表
         e_relationlist_type_friendex, //跨服好友列表
         e_relationlist_type_other_req_add, //请求添加好友列表
         e_relationlist_type_chat, //最近聊天
         e_relationlist_type_lucky, //结缘
         e_relationlist_type_other_req_add_lucky, //请求结缘列表
         e_relationlist_type_max
     };
     enum e_first_kill_prize_type
     {
         e_first_kill_prize_type_cant_get, //首杀没出现，不能领取首杀奖励
         e_first_kill_prize_type_not_get, //没有领取首杀奖励
         e_first_kill_prize_type_was_get, //领取过首杀奖励
         e_first_kill_prize_type_max
     };
     enum e_service_goal_state
     {
         e_service_goal_state_unfinished, //未完成
         e_service_goal_state_finished, //完成
         e_service_goal_state_has_been_received, //已领取
         e_service_goal_state_max
     };
     enum e_service_goal_inst_data
     {
         e_service_goal_inst_data_id, //id
         e_service_goal_inst_data_schedule, //进度
         e_service_goal_inst_data_state, //状态
         e_service_goal_inst_data_active_state, //激活状态
         e_service_goal_inst_data_over_time, //到期时间
         e_service_goal_inst_data_max
     };
     enum e_service_goal_operate_type
     {
         e_service_goal_operate_type_none,
         e_service_goal_operate_type_receive, //领取
         e_service_goal_operate_type_max
     };
     enum e_service_goal_type
     {
         e_service_goal_type_role_level, //等级
         e_service_goal_type_role_gs, //战力
         e_service_goal_type_big_reward, //大奖
         e_service_goal_type_strength_any, //N件强化等级
         e_service_goal_type_addition_the_body, //全身追加等级
         e_service_goal_type_mount_level, //坐骑几级阶几星
         e_service_goal_type_mount_num, //拥有几个坐骑
         e_service_goal_type_friend_num, //拥有N个好友
         e_service_goal_type_friendliness, //好友度
         e_service_goal_type_clearance_fuben, //通关过XX副本
         e_service_goal_type_body_has_color_equip, //全身装备N件N阶紫色品质以上装备
         e_service_goal_type_legion_donate_num, //军团捐赠次数
         e_service_goal_type_soldier_rank_level, //军衔等级
         e_service_goal_type_arena_rank_level, //竞技场排名
         e_service_goal_type_actived_illustration_num, //激活N个图鉴
         e_service_goal_type_actived_any_chapter_illustration, //激活任意一章节图鉴
         e_service_goal_type_demons_tower_layer_num, //万魔塔层数
         e_service_goal_type_achievement_title, //成就称号
         e_service_goal_type_wing_level, //羽翼N阶N星
         e_service_goal_type_element_heart_total_level, //符文总等级
         e_service_goal_type_equiped_element_heart_num, //拥有X品质符文
         e_service_goal_type_equiped_spirit_total_level, //精灵总等级
         e_service_goal_type_equiped_spirit_num, //装备精灵数
         e_service_goal_type_belief_rank, //信仰阶数 区分职业
         e_service_goal_type_get_all_reward, //领取前六日所有进阶奖励
         e_service_goal_type_final, //最终大奖
         e_service_goal_type_god_hood_grade, //神格突破等级
         e_service_goal_type_seven_day_final, //七日最终大奖
         e_service_goal_type_no_limit_time_final, //非限时大奖
         e_service_goal_type_awaken_level, //觉醒装备等级
         e_service_goal_type_max
     };
     enum e_service_goal_active_type
     {
         e_service_goal_active_type_basis, //基础
         e_service_goal_active_type_advance, //进阶
         e_service_goal_active_type_final, //最终大奖
         e_service_goal_active_type_seven_day, //前7天目标
         e_service_goal_active_type_max
     };
     enum e_team_operate
     {
         e_team_operate_follow_cap, //跟随队长
         e_team_operate_urge_cap, //催促队长
         e_team_operate_call_together, //召集队员
         e_team_operate_urge_together, //催促队员
         e_team_operate_max
     };
     enum e_relation_operate_result
     {
         e_relation_operate_result_success, //成功
         e_relation_operate_result_offline, //对方不在线
         e_relation_operate_result_add_friend_success, //添加好友成功
         e_relation_operate_result_del_friend_success, //删除成功
         e_relation_operate_result_add_black_success, //添加黑名单成功
         e_relation_operate_result_del_black_success, //删除黑名单成功
         e_relation_operate_result_add_hate_success, //添加仇人成功
         e_relation_operate_result_add_friend_all_once, //一键添加成功
         e_relation_operate_result_cant_operate_self, //不允许操作自己
         e_relation_operate_result_friend_list_full, //好友列表满了
         e_relation_operate_result_hate_list_full, //仇恨列表满了
         e_relation_operate_result_black_list_full, //屏蔽列表满了
         e_relation_operate_result_friend_exist, //好友已存在
         e_relation_operate_result_black_exist, //屏蔽玩家已经存在
         e_relation_operate_result_hate_exist, //仇人已存在
         e_relation_operate_result_too_fast, //搜索过快
         e_relation_operate_result_req_friend_success,
         e_relation_operate_result_pass_req_friend,
         e_relation_operate_result_other_add_friend, //加入待同意列表
         e_relation_operate_result_other_is_cross_server, //对方是跨服玩家禁止添加好友
         e_relation_operate_result_max
     };
     enum e_item_wing_type
     {
         e_item_wing_type_non, //空
         e_item_wing_type_normal, //普通翅膀
         e_item_wing_type_shape_no_illusion, //无幻化外形翅膀
         e_item_wing_type_shape_illusion, //有幻化的外形翅膀
         e_item_wing_type_illusion, //幻化的翅膀
         e_item_wing_type_special_santo, //城主翅膀
         e_item_wing_type_max
     };
     enum e_convert_type
     {
         e_convert_type_bind_siliver, //绑金
         e_convert_type_exp, //经验
         e_convert_type_bind_diamond, //绑定钻石
         e_convert_type_bag_bind_siliver, //背包绑金
         e_convert_type_bag_siliver, //背包金币
         e_convert_type_max
     };
     enum e_belief_series_type
     {
         e_belief_series_type_spring, //火
         e_belief_series_type_summer, //冰
         e_belief_series_type_autumn, //光
         e_belief_series_type_winter, //暗
         e_belief_series_type_max
     };
     enum e_belief_buff_level
     {
         e_belief_buff_level_index_first, //一阶段
         e_belief_buff_level_index_second, //二阶段
         e_belief_buff_level_index_third, //三阶段
         e_belief_buff_level_max
     };
     enum e_legion_call_type
     {
         e_legion_call_type_call_legion_boss, //军团boss召集
         e_legion_call_type_call_city_war, //领土争夺召集
         e_legion_call_type_call_bonfire, //篝火召集
         e_legion_call_type_call_elite_boss, //精英讨伐召集
         e_legion_call_type_call_world_boss, //世界领主召集
         e_legion_call_type_max
     };
     enum e_reduce_pk_value_prop_quality
     {
         e_reduce_pk_value_prop_quality_low, //低品质
         e_reduce_pk_value_prop_quality_mid, //中品质
         e_reduce_pk_value_prop_quality_high, //高品质
         e_reduce_pk_value_prop_quality_max
     };
     enum e_spirit_operate_result
     {
         e_spirit_operate_result_unknown,
         e_spirit_operate_result_success, //成功
         e_spirit_operate_result_no_space, //包满
         e_spirit_operate_result_max
     };
     enum e_daily_must_do_typ
     {
         e_daily_must_do_typ_raid_exp, ////经验副本
         e_daily_must_do_typ_raid_daily, ////日常任务
         e_daily_must_do_typ_raid_devil_square, ////诅咒神殿
         e_daily_must_do_typ_raid_ancient_battlefield, ////上古战场
         e_daily_must_do_typ_raid_siliver, ////金币副本
         e_daily_must_do_typ_community_fight, ////阵营战
         e_daily_must_do_typ_raid_drame, ////剧情副本
         e_daily_must_do_typ_bloody_castle, ////帝国遗藏
         e_daily_must_do_typ_raid_multiple_princess_guard_war, ////女神保卫
         e_daily_must_do_typ_raid_multiple_lava_hellfire, ////熔岩炼狱
         e_daily_must_do_typ_raid_multiple_abyss_of_fear, ////恐惧深渊
         e_daily_must_do_typ_demon_tower, ////万魔塔
         e_daily_must_do_typ_angel_temple, ////破碎虚空
         e_daily_must_do_typ_pk_king, ////PK之王
         e_daily_must_do_typ_arena, ////竞技场
         e_daily_must_do_typ_crusade_mission, ////讨伐任务
         e_daily_must_do_typ_crystal_illusion, ////水晶幻境
         e_daily_must_do_typ_mingmie_xukong, ////无尽之海
         e_daily_must_do_typ_out_board, ////挂机
         e_daily_must_do_typ_convert, ////转换
         e_daily_must_do_typ_rank, ////排行榜
         e_daily_must_do_typ_gold_army, ////世界精英
         e_daily_must_do_typ_world_boss, ////世界Boss
         e_daily_must_do_typ_sky_treasure_box, ////天空宝箱
         e_daily_must_do_typ_field_treasure_box, ////野外宝箱
         e_daily_must_do_typ_legion_boss, ////战盟Boss
         e_daily_must_do_typ_meditation, ////冥想
         e_daily_must_do_typ_activaty_degree, ////活跃度
         e_daily_must_do_typ_city_war, ////城战
         e_daily_must_do_typ_cross_fight, ////跨服
         e_daily_must_do_typ_gold_store, ////金币商店
         e_daily_must_do_typ_magic_crystal_store, ////魔晶商店
         e_daily_must_do_typ_ancient_coin_store, ////古币商店
         e_daily_must_do_typ_hope_store, ////祈福商店
         e_daily_must_do_typ_legion_store, ////军团商店
         e_daily_must_do_typ_hope_item, ////祈福
         e_daily_must_do_typ_shop, ////商城
         e_daily_must_do_typ_legion_bonfire, ////军团篝火
         e_daily_must_do_typ_legion_war, ////军团战
         e_daily_must_do_typ_boss_home, ////打宝野外boss
         e_daily_must_do_typ_boss_island, ////天空岛
         e_daily_must_do_typ_belief_cloister, ////信仰回廊
         e_daily_must_do_typ_cross_pk, ////跨服竞技
         e_daily_must_do_typ_cross_rob, ////跨服掠夺
         e_daily_must_do_typ_legion_dance, ////军团共舞
         e_daily_must_do_typ_legion_answer, ////军团答题
         e_daily_must_do_typ_cross_ladder, ////跨服天梯
         e_daily_must_do_typ_single_boss, ////个人boss
         e_daily_must_do_typ_element_war, ////元素争霸
         e_daily_must_do_typ_vip_boss_home, ////boss之家
         e_daily_must_do_typ_cross_server_boss, ////跨服世界boss
         e_daily_must_do_typ_oracle_trial, ////天启试炼
         e_daily_must_do_typ_attack_city, ////攻城战
         e_daily_must_do_typ_max
     };
     enum e_daily_active_degree_type
     {
         e_daily_active_degree_type_cost_one_diamond, //商城消费1钻
         e_daily_active_degree_type_ten_daily_mission, //日常任务10
         e_daily_active_degree_type_three_normal_raid, //通关3次普通副本
         e_daily_active_degree_type_three_elite_raid, //通关3次精英副本
         e_daily_active_degree_type_three_hell_raid, //通关3次炼狱副本
         e_daily_active_degree_type_empire_raid, //参与1次帝国遗藏
         e_daily_active_degree_type_temple_raid, //参与1次诅咒神殿
         e_daily_active_degree_type_warp_raid, //参与1次空战本
         e_daily_active_degree_type_three_upgrade, //强化任意装备3次
         e_daily_active_degree_type_three_addon, //追加任意装备3次
         e_daily_active_degree_type_kill_200_monster, //击杀200个怪物
         e_daily_active_degree_type_kill_500_monster, //击杀500个怪物
         e_daily_active_degree_type_kill_1000_monster, //击杀1000个怪物
         e_daily_active_degree_type_login, //登录游戏
         e_daily_active_degree_type_online_30_minutes, //在线30分钟
         e_daily_active_degree_type_strengthing_one_spirit, //强化任意精灵1次
         e_daily_active_degree_type_strengthing_one_mount, //强化任意坐骑1次
         e_daily_active_degree_type_strengthing_one_wing, //强化任意羽翼1次
         e_daily_active_degree_type_upgrade_one_element_heart, //升级任意符文1次
         e_daily_active_degree_type_upgrade_one_patron_saint, //升级守护神1次
         e_daily_active_degree_type_upgrade_one_belief, //升级任意信仰1次
         e_daily_active_degree_type_submit_one_illustration, //提交1次图鉴
         e_daily_active_degree_type_attend_three_arena, //参加3次竞技场
         e_daily_active_degree_type_attend_one_city_war, //参加城战1
         e_daily_active_degree_type_attend_one_legion_war, //参加城战2，军团战
         e_daily_active_degree_type_max
     };
     enum e_notice_param
     {
         e_notice_param_go_to_location, //寻路
         e_notice_param_open_widget, //打开界面
         e_notice_param_item_info, //物品信息
         e_notice_param_max
     };
     enum e_RankingIndex
     {
         e_RankingIndex_gs, //战力总榜
         e_RankingIndex_level, //等级总榜
         e_RankingIndex_box_map_level, //大世界榜
         e_RankingIndex_demonstower, //万魔塔
         e_RankingIndex_wing, //羽翼总战力
         e_RankingIndex_mount, //坐骑总战力
         e_RankingIndex_worship, //个人 威望
         e_RankingIndex_arena, //竞技场
         e_RankingIndex_legion_core, //军团榜 总战力
         e_RankingIndex_gs_barserker, //战力 战士
         e_RankingIndex_gs_wizard, //战力 法师
         e_RankingIndex_gs_guardian, //战力 骑士
         e_RankingIndex_gs_assassinator, //战力 刺客
         e_RankingIndex_level_barserker, //等级 战士
         e_RankingIndex_level_wizard, //等级 法师
         e_RankingIndex_level_guardian, //等级 骑士
         e_RankingIndex_level_assassinator, //等级 刺客
         e_RankingIndex_equipment_main, //装备榜 总战力
         e_RankingIndex_equipment_weapon, //装备榜 武器
         e_RankingIndex_equipment_necklace, //装备榜 项链
         e_RankingIndex_equipment_armor, //装备榜 衣服
         e_RankingIndex_equipment_pants, //装备榜 裤子
         e_RankingIndex_equipment_hat, //装备榜 头饰
         e_RankingIndex_equipment_shoes, //装备榜 鞋子
         e_RankingIndex_equipment_glove, //装备榜 手套
         e_RankingIndex_equipment_ring, //装备榜 戒指
         e_RankingIndex_equipment_earring, //装备榜 耳环
         e_RankingIndex_mount_train, //坐骑榜 培养
         e_RankingIndex_wing_train, //羽翼榜 培养
         e_RankingIndex_elves_train, //精灵榜 培养
         e_RankingIndex_belief, //个人榜 信仰
         e_RankingIndex_element, //个人榜 符文
         e_RankingIndex_inspiration, //个人榜 启示之魂
         e_RankingIndex_military_rank, //个人榜 军衔
         e_RankingIndex_achievement, //个人榜 成就
         e_RankingIndex_illustrated, //个人榜 图鉴
         e_RankingIndex_legion_territory, //军团榜 领地排行
         e_RankingIndex_legion_boss, //军团榜 军团boss
         e_RankingIndex_legion_bonfire, //军团榜 军团篝火
         e_RankingIndex_world_boss_one, //世界BOSS boss1
         e_RankingIndex_world_boss_two, //世界BOSS boss2
         e_RankingIndex_world_boss_three, //世界BOSS boss3
         e_RankingIndex_world_boss_four, //世界BOSS boss4
         e_RankingIndex_world_boss_five, //世界BOSS boss5
         e_RankingIndex_world_boss_six, //世界BOSS boss6
         e_RankingIndex_world_boss_seven, //世界BOSS boss7
         e_RankingIndex_world_elite_one, //精英讨伐 boss1
         e_RankingIndex_world_elite_two, //精英讨伐 boss2
         e_RankingIndex_world_elite_three, //精英讨伐 boss3
         e_RankingIndex_world_elite_four, //精英讨伐 boss4
         e_RankingIndex_world_elite_five, //精英讨伐 boss5
         e_RankingIndex_world_elite_six, //精英讨伐 boss6
         e_RankingIndex_world_elite_seven, //精英讨伐 boss7
         e_RankingIndex_world_elite_eight, //精英讨伐 boss8
         e_RankingIndex_elves_gs, //精灵总战力
         e_RankingIndex_vip_level_one, //VIP榜 VIP1
         e_RankingIndex_vip_level_five, //VIP榜 VIP5
         e_RankingIndex_vip_level_seven, //VIP榜 VIP7
         e_RankingIndex_recharge_rmb, //充值榜
         e_RankingIndex_time_limit_activity_total_recharge, //限时活动累计充值排行榜
         e_RankingIndex_time_limit_activity_total_cost, //限时活动累计消费排行榜
         e_RankingIndex_time_limit_activity_treasure, //限时活动宝藏抽取排行榜
         e_RankingIndex_gs_all, //跨服战力榜
         e_RankingIndex_cross_server_plinder_guard_value, //跨服掠夺守卫值榜
         e_RankingIndex_legion_gs_all, //跨服军团战力榜
         e_RankingIndex_cross_ladder_score, //跨服天梯本服积分榜
         e_RankingIndex_feather_heart, //神翎榜
         e_RankingIndex_cross_boss_one, //跨服世界boss1
         e_RankingIndex_cross_boss_two, //`跨服世界boss2
         e_RankingIndex_cross_boss_three, //跨服世界boss3
         e_RankingIndex_cross_boss_four, //跨服世界boss4
         e_RankingIndex_cross_boss_five, //跨服世界boss5
         e_RankingIndex_cross_boss_six, //跨服世界boss6
         e_RankingIndex_cross_boss_seven, //跨服世界boss7
         e_RankingIndex_cross_boss_eight, //跨服世界boss8
         e_RankingIndex_cross_boss_nine, //跨服世界boss9
         e_RankingIndex_cross_boss_ten, //跨服世界boss10
         e_RankingIndex_cross_boss_eleven, //跨服世界boss11
         e_RankingIndex_cross_boss_tweleve, //跨服世界boss12
         e_RankingIndex_time_limit_activity_gs_up, //限时活动战力提升榜
         e_RankingIndex_time_limit_activity_mount_up, //限时活动坐骑提升榜
         e_RankingIndex_time_limit_activity_wing_up, //限时活动翅膀提升榜
         e_RankingIndex_time_limit_activity_equip_up, //限时活动装备提升榜
         e_RankingIndex_time_limit_activity_spirit_up, //限时活动精灵提升榜
         e_RankingIndex_time_limit_activity_cross_gs_up, //跨服限时活动战力提升榜
         e_RankingIndex_time_limit_activity_cross_mount_up, //跨服限时活动坐骑提升榜
         e_RankingIndex_time_limit_activity_cross_wing_up, //跨服限时活动翅膀提升榜
         e_RankingIndex_time_limit_activity_cross_equip_up, //跨服限时活动装备提升榜
         e_RankingIndex_time_limit_activity_cross_spirit_up, //跨服限时活动精灵提升榜
         e_RankingIndex_level_all, //跨服等级榜
         e_RankingIndex_couple, //夫妻榜
         e_RankingIndex_Oracle_Trial, //天启试炼
         e_RankingIndex_cross_ladder_last_seaon, //跨服天梯上赛季王者榜
         e_RankingIndex_cross_ladder_cur_season, //跨服天梯本赛季王者榜
         e_RankingIndex_pk_king, //Pk之王
         e_RankingIndex_element_rank_list, //元素争霸本服排行榜
         e_RankingIndex_element_war_cur_season, //元素争霸本赛季排行榜
         e_RankingIndex_element_war_last_season, //元素争霸上赛季排行榜
         e_RankingIndex_cross_time_limit_activity_total_recharge, //跨服限时活动累计充值排行榜
         e_RankingIndex_max
     };
     enum e_mount_sub_type
     {
         e_mount_sub_type_none,
         e_mount_sub_type_origin, //坐骑本体
         e_mount_sub_type_illusion, //坐骑幻化
         e_mount_sub_type_max
     };
     enum e_goods_purchase_limit_type
     {
         e_goods_purchase_limit_type_none, //不限购
         e_goods_purchase_limit_type_per_day, //每日限购
         e_goods_purchase_limit_type_forever, //永久限购
         e_goods_purchase_limit_type_per_week, //每周限购
         e_goods_purchase_limit_type_max
     };
     enum e_update_item_info_mode
     {
         e_update_item_info_mode_default, //默认
         e_update_item_info_mode_pick_up_new, //捡包
         e_update_item_info_mode_update_all, //同步所有数据
         e_update_item_info_mode_max
     };
     enum e_exhibition_actor_array
     {
         e_exhibition_actor_array_model, //3D模型展示
         e_exhibition_actor_array_item, //物品2D展示
         e_exhibition_actor_array_max
     };
     enum e_auction_operate_type
     {
         e_auction_operate_type_non,
         e_auction_operate_type_sell, //卖
         e_auction_operate_type_purchase, //买
         e_auction_operate_type_cancel_sell, //取消出售
         e_auction_operate_type_bid, //竞标
         e_auction_operate_type_read_record, //读纪录
         e_auction_operate_type_max
     };
     enum e_trading_type
     {
         e_trading_type_normal_sell, //普通玩家交易
         e_trading_type_legion, //军团拍卖
         e_trading_type_world, //世界拍卖
         e_trading_type_max
     };
     enum e_trading_source_type
     {
         e_trading_source_type_non,
         e_trading_source_type_world_boss, //世界精英
         e_trading_source_type_gold_army, //世界领主
         e_trading_source_type_bonfire, //篝火
         e_trading_source_type_legion_boss, //军团boss
         e_trading_source_type_max
     };
     enum e_excellentatt_quality_type
     {
         e_excellentatt_quality_type_green, //卓越属性绿色品质
         e_excellentatt_quality_type_blue, //卓越属性蓝色品质
         e_excellentatt_quality_type_purple, //卓越属性紫色品质
         e_excellentatt_quality_type_orange, //卓越属性橙色品质
         e_excellentatt_quality_type_red, //卓越属性红色品质
         e_excellentatt_quality_type_upper, //卓越属性品质上限
         e_excellentatt_quality_type_max
     };
     enum e_ranking_operate_result
     {
         e_ranking_operate_result_success, //成功
         e_ranking_operate_result_error, //未知错误
         e_ranking_operate_result_no_worship_value, //没有威望
         e_ranking_operate_result_ranking_no_player, //排行榜没有人
         e_ranking_operate_result_max
     };
     enum e_camera_move_info
     {
         e_camera_move_info_boom_length, //距离 负1为失效
         e_camera_move_info_field_of_view, //视角 负1为失效
         e_camera_move_info_camera_location_x, //摄像机位置X 负1为失效
         e_camera_move_info_camera_location_y, //摄像机位置Y 负1为失效
         e_camera_move_info_camera_location_z, //摄像机位置Z 负1为失效
         e_camera_move_info_camera_rotation_roll, //摄像机X旋转 361为失效
         e_camera_move_info_camera_rotation_pitch, //摄像机Y旋转 361为失效
         e_camera_move_info_camera_rotation_yaw, //摄像机Z旋转 361为失效
         e_camera_move_info_boom_location_x, //摄像机杆位置X 负1为失效
         e_camera_move_info_boom_location_y, //摄像机杆位置Y 负1为失效
         e_camera_move_info_boom_location_z, //摄像机杆位置Z 负1为失效
         e_camera_move_info_boom_rotation_roll, //摄像机杆X旋转 361为失效
         e_camera_move_info_boom_rotation_pitch, //摄像机杆Y旋转 361为失效
         e_camera_move_info_boom_rotation_yaw, //摄像机杆Z旋转 361为失效
         e_camera_move_info_ani_time, //目标动画时间
         e_camera_move_info_remain_time, //真实持续时间
         e_camera_move_info_max
     };
     enum e_notice_param_type
     {
         e_notice_param_type_unit_name, //人物名称类型
         e_notice_param_type_item_name, //物品名称类型
         e_notice_param_type_num_data, //数字类型字符串
         e_notice_param_type_item_data, //超链接类型
         e_notice_param_type_describe_str, //描述类型字符串
         e_notice_param_type_other_name, //其他名字
         e_notice_param_type_max
     };
     enum e_marry_operate_type
     {
         e_marry_operate_type_success, //成功
         e_marry_operate_type_unknow_error, //未知错误
         e_marry_operate_type_team_error, //组队错误
         e_marry_operate_type_level_error, //等级错误
         e_marry_operate_type_not_friend, //不是好友
         e_marry_operate_type_low_friendliness, //好友度过低
         e_marry_operate_type_one_is_cant_marry_state, //一个人不在结婚状态
         e_marry_operate_type_far_away, //距离npc过远
         e_marry_operate_type_lack_marry_item, //没有结婚道具
         e_marry_operate_type_one_refuse, //对方拒绝
         e_marry_operate_type_one_offline, //对方离线
         e_marry_operate_type_can_marry, //符合条件
         e_marry_operate_type_marry_success, //结婚成功
         e_marry_operate_type_check_marry_item, //检测结婚礼物
         e_marry_operate_type_mary_func_unlock, //结婚功能未解锁
         e_marry_operate_type_check_marry_success, //婚前检查成功
         e_marry_operate_type_single, //单身
         e_marry_operate_type_one_agree, //对方同意
         e_marry_operate_type_divorce_team_error, //离婚_组队错误
         e_marry_operate_type_divorce_isnt_marry, //没结过婚
         e_marry_operate_type_divorce_lack_money, //金币不足
         e_marry_operate_type_one_refuse_divorce, //拒绝离婚
         e_marry_operate_type_one_agree_divorce, //同意离婚
         e_marry_operate_type_divorce_success, //离婚成功
         e_marry_operate_type_check_divorce_success, //离婚检查成功
         e_marry_operate_type_offline_over_seven, //强制离婚连续离线超过7天
         e_marry_operate_type_check_force_divorce_success, //强制离婚检查
         e_marry_operate_type_had_getted_reward, //领过奖励了
         e_marry_operate_type_get_reward_success, //领取成功
         e_marry_operate_type_max
     };
     enum e_fashion_equip_slot
     {
         e_fashion_equip_slot_non, //空
         e_fashion_equip_slot_weapon_mesh, //武器模型
         e_fashion_equip_slot_body_mesh, //身体模型
         e_fashion_equip_slot_wedding_ring, //婚戒
         e_fashion_equip_slot_weapon_effect, //武器特效
         e_fashion_equip_slot_body_effect, //身体特效
         e_fashion_equip_slot_track_effect, //足迹特效
         e_fashion_equip_slot_head_effect, //头部特效
         e_fashion_equip_slot_breastplate_effect, //胸甲特效
         e_fashion_equip_slot_glove_effect, //护手特效
         e_fashion_equip_slot_legs_effect, //腿部特效
         e_fashion_equip_slot_feet_effect, //足部特效
         e_fashion_equip_slot_headdress, //头饰
         e_fashion_equip_slot_mouth, //口罩
         e_fashion_equip_slot_eye, //眼睛
         e_fashion_equip_slot_max
     };
     enum e_fashion_show_type
     {
         e_fashion_show_type_all, //全部
         e_fashion_show_type_weapon, //武器
         e_fashion_show_type_cloth, //衣服
         e_fashion_show_type_weapon_effect, //武器特效
         e_fashion_show_type_cloth_effect, //衣服特效
         e_fashion_show_type_foot_effect, //脚部特效
         e_fashion_show_type_head_frame, //头像框
         e_fashion_show_type_headdress, //头饰
         e_fashion_show_type_mouth, //口罩
         e_fashion_show_type_eye, //眼睛
         e_fashion_show_type_max
     };
     enum e_interaction_reply
     {
         e_interaction_reply_none,
         e_interaction_reply_reject, //拒绝
         e_interaction_reply_agree, //同意
         e_interaction_reply_timeout, //超时
         e_interaction_reply_be_invited, //被邀请中
         e_interaction_reply_is_interacting, //交互中
         e_interaction_reply_max
     };
     enum e_interaction_type
     {
         e_interaction_type_none,
         e_interaction_type_interrupted, //交互
         e_interaction_type_doubleride_front, //双人坐骑前
         e_interaction_type_doubleride_back, //双人坐骑后
         e_interaction_type_bao_da_tui, //抱大腿
         e_interaction_type_bei_bao_da_tui, //被抱大腿
         e_interaction_type_gong_zhu_bao, //公主抱
         e_interaction_type_bei_gong_zhu_bao, //被公主抱
         e_interaction_type_qiu_hun, //求婚
         e_interaction_type_bei_qiu_hun, //被求婚
         e_interaction_type_tiao_wu, //跳舞
         e_interaction_type_bei_tiao_wu, //被跳舞
         e_interaction_type_marry_raid_back_target_man, //结婚同骑后对方男
         e_interaction_type_marry_raid_back_target_woman, //结婚同骑后对方女
         e_interaction_type_marry_raid_front_target_man, //结婚同骑前对方男
         e_interaction_type_marry_raid_front_target_woman, //结婚同骑前对方女
         e_interaction_type_marry_raid, //结婚同骑
         e_interaction_type_yao_legion_dance, //邀请共舞
         e_interaction_type_bei_legion_dance, //被邀请共舞
         e_interaction_type_marry_dance_accord, //结婚动作主动
         e_interaction_type_marry_dance_passive, //结婚动作被动
         e_interaction_type_max
     };
     enum e_belief_upgrade_type
     {
         e_belief_upgrade_type_none, //满级
         e_belief_upgrade_type_upgrade_star, //升星
         e_belief_upgrade_type_upgrade_rank, //升阶
         e_belief_upgrade_type_max
     };
     enum e_belief_operate_type
     {
         e_belief_operate_type_upgrade_star, //升星
         e_belief_operate_type_upgrade_rank, //升阶
         e_belief_operate_type_max
     };
     enum e_exhibition_show_type
     {
         e_exhibition_show_type_player_self, //自己
         e_exhibition_show_type_player_other, //其他人
         e_exhibition_show_type_npc, //npc
         e_exhibition_show_type_other_slot, //某一装备槽
         e_exhibition_show_type_spirit, //精灵
         e_exhibition_show_type_protect_spirit, //守护神
         e_exhibition_show_type_self_by_info, //用具体信息显示自己
         e_exhibition_show_type_player_self_simple, //简化的自己 没翅膀啥的
         e_exhibition_show_type_player_other_on_mount, //在坐骑上的其他玩家
         e_exhibition_show_type_try_on_fashion, //时装试穿
         e_exhibition_show_type_spirit_composit, //精灵合成
         e_exhibition_show_type_max
     };
     enum e_pos_type
     {
         e_pos_type_pos_x, //坐标x
         e_pos_type_pos_y, //坐标y
         e_pos_type_pos_z, //坐标z
         e_pos_type_max
     };
     enum e_rotation_type
     {
         e_rotation_type_pitch, //旋转pitch
         e_rotation_type_yaw, //旋转yaw
         e_rotation_type_roll, //旋转roll
         e_rotation_type_max
     };
     enum e_week_day
     {
         e_week_day_monday, //周一
         e_week_day_tuesday, //周二
         e_week_day_wednesday, //周三
         e_week_day_thursday, //周四
         e_week_day_friday, //周五
         e_week_day_saturday, //周六
         e_week_day_sunday, //周日
         e_week_day_max
     };
     enum e_month
     {
         e_month_january, //一月
         e_month_february, //二月
         e_month_march, //三月
         e_month_april, //四月
         e_month_may, //五月
         e_month_june, //六月
         e_month_july, //七月
         e_month_august, //八月
         e_month_september, //九月
         e_month_october, //十月
         e_month_november, //十一月
         e_month_december, //十二月
         e_month_max
     };
     enum e_item_get_info
     {
         e_item_get_info_item_template_id, //表格ID
         e_item_get_info_item_num, //物品数量
         e_item_get_info_locked_state, //锁定状态
         e_item_get_info_max
     };
     enum e_addition_buff
     {
         e_addition_buff_upgrade, //装备强化
         e_addition_buff_quality, //装备卓越
         e_addition_buff_achievement, //成就
         e_addition_buff_rank, //军衔
         e_addition_buff_feather, //翅膀强化
         e_addition_buff_addon, //装备追加
         e_addition_buff_succinct, //装备洗练
         e_addition_buff_fahion_sum_level, //时装总星级
         e_addition_buff_sky_equip, //天空套装
         e_addition_buff_enchant, //附魔套装
         e_addition_buff_goddess, //女神套装
         e_addition_buff_couple_heart_value, //夫妻灵犀值
         e_addition_buff_skygod_equip, //天神套装
         e_addition_buff_supreme_equip, //神王套装
         e_addition_buff_max
     };
     enum e_item_upgrade_protect_type
     {
         e_item_upgrade_protect_type_non, //无
         e_item_upgrade_protect_type_low_level_undown, //低级不掉级
         e_item_upgrade_protect_type_high_level_undown, //高级不掉级
         e_item_upgrade_protect_type_add_success_percentage, //增加成功率
         e_item_upgrade_protect_type_max
     };
     enum e_item_succinct_buff_level
     {
         e_item_succinct_buff_level_none, //无
         e_item_succinct_buff_level_F, //F级
         e_item_succinct_buff_level_E, //E级
         e_item_succinct_buff_level_D, //D级
         e_item_succinct_buff_level_C, //C级
         e_item_succinct_buff_level_B, //B级
         e_item_succinct_buff_level_A, //A级
         e_item_succinct_buff_level_S, //S级
         e_item_succinct_buff_level_SS, //SS级
         e_item_succinct_buff_level_SSS, //SSS级
         e_item_succinct_buff_level_max
     };
     enum e_this_launch_already_do
     {
         e_this_launch_already_do_play_cg, //播放CG
         e_this_launch_already_do_show_notice, //弹出公告
         e_this_launch_already_do_sdk_game_start, //防止GameStart接口调用多次
         e_this_launch_already_do_max
     };
     enum e_tinder_info
     {
         e_tinder_info_current_level_template_id, //当前火种等级模板id
         e_tinder_info_current_unlock_template_id, //当前解锁火种模板id
         e_tinder_info_already_play_unlock_animation_id, //已经播放解锁动画的火种id
         e_tinder_info_total_star_level, //火种星级
         e_tinder_info_add_star_level, //火种道具增加星级
         e_tinder_info_max
     };
     enum e_tinder_operation_type
     {
         e_tinder_operation_type_levelup_tinder, //升级火种
         e_tinder_operation_type_unlock_tinder, //解锁火种
         e_tinder_operation_type_update_unlock_ani, //更新解锁动画
         e_tinder_operation_type_unlock_active_skill, //解锁主动技能
         e_tinder_operation_type_max
     };
     enum e_get_aoi_info_all_type
     {
         e_get_aoi_info_all_type_create, //创建
         e_get_aoi_info_all_type_simple_to_normal, //Simple转Normal
         e_get_aoi_info_all_type_normal_to_simple, //Normal转Simple
         e_get_aoi_info_all_type_max
     };
     enum e_service_rank_type
     {
         e_service_rank_type_level, //等级榜
         e_service_rank_type_mount, //坐骑
         e_service_rank_type_spirit, //精灵
         e_service_rank_type_element, //符文
         e_service_rank_type_wing, //翅膀
         e_service_rank_type_gs, //战力榜
         e_service_rank_type_max
     };
     enum e_mission_type
     {
         e_mission_type_main, //主线
         e_mission_type_daily, //日常
         e_mission_type_against, //讨伐
         e_mission_type_side, //支线
         e_mission_type_grade_up, //转职任务
         e_mission_type_marry, //夫妻任务
         e_mission_type_max
     };
     enum e_mission_slot
     {
         e_mission_slot_main, //主线
         e_mission_slot_daily, //日常
         e_mission_slot_against, //讨伐
         e_mission_slot_grade_up, //转职
         e_mission_slot_side_0, //支线_0
         e_mission_slot_side_1, //支线_1
         e_mission_slot_side_2, //支线_2
         e_mission_slot_side_3, //支线_3
         e_mission_slot_side_4, //支线_4
         e_mission_slot_side_5, //支线_5
         e_mission_slot_side_6, //支线_6
         e_mission_slot_extra_side_0, //额外支线_0（结婚任务）
         e_mission_slot_extra_side_1, //额外支线_1
         e_mission_slot_extra_side_2, //额外支线_2
         e_mission_slot_extra_side_3, //额外支线_3
         e_mission_slot_extra_side_4, //额外支线_4
         e_mission_slot_extra_side_5, //额外支线_5
         e_mission_slot_extra_side_6, //额外支线_6
         e_mission_slot_extra_side_7, //额外支线_7
         e_mission_slot_extra_side_8, //额外支线_8
         e_mission_slot_extra_side_9, //额外支线_9
         e_mission_slot_extra_side_10, //额外支线_10
         e_mission_slot_max
     };
     enum e_service_rank_reward_state_type
     {
         e_service_rank_reward_state_type_no_qualification, //没有资格
         e_service_rank_reward_state_type_have_qualification, //有资格
         e_service_rank_reward_state_type_already_receive, //已经领取
         e_service_rank_reward_state_type_max
     };
     enum e_service_rank_reward_type
     {
         e_service_rank_reward_type_first, //头奖
         e_service_rank_reward_type_second, //第二段
         e_service_rank_reward_type_third, //第三段
         e_service_rank_reward_type_fourth, //第四段
         e_service_rank_reward_type_fifth, //第五段
         e_service_rank_reward_type_max
     };
     enum e_beast_spirit_type
     {
         e_beast_spirit_type_mount, //坐骑兽魂
         e_beast_spirit_type_wing, //翅膀兽魂
         e_beast_spirit_type_spirit, //精灵兽魂
         e_beast_spirit_type_max
     };
     enum e_gain_treasure_info_type
     {
         e_gain_treasure_info_type_killed_field_boss, //已击杀野外boss次数
         e_gain_treasure_info_type_killed_simple_boss, //已击杀个人boss次数
         e_gain_treasure_info_type_buy_simple_boss, //个人boss已购买次数
         e_gain_treasure_info_type_max
     };
     enum e_jewel_type
     {
         e_jewel_type_non, //无效
         e_jewel_type_attack, //攻击
         e_jewel_type_defense, //防御
         e_jewel_type_max
     };
     enum e_drop_record_model_type
     {
         e_drop_record_model_type_no, //不记录
         e_drop_record_model_type_record, //记录
         e_drop_record_model_type_set_top, //置顶
         e_drop_record_model_type_max
     };
     enum e_treasure_record
     {
         e_treasure_record_time, //抽取时间
         e_treasure_record_treasure_type, //宝藏类型
         e_treasure_record_prize_type, //抽取类型
         e_treasure_record_prize_0, //奖品
         e_treasure_record_prize_1, //奖品
         e_treasure_record_prize_2, //奖品
         e_treasure_record_prize_3, //奖品
         e_treasure_record_prize_4, //奖品
         e_treasure_record_prize_5, //奖品
         e_treasure_record_prize_6, //奖品
         e_treasure_record_prize_7, //奖品
         e_treasure_record_prize_8, //奖品
         e_treasure_record_prize_9, //奖品
         e_treasure_record_max
     };
     enum e_time_limit_activity_type
     {
         e_time_limit_activity_type_total_charge, //累计充值排行
         e_time_limit_activity_type_total_cost, //累计消耗排行
         e_time_limit_activity_type_daily_charge, //原初宝箱
         e_time_limit_activity_type_treasure, //限时皇家宝藏
         e_time_limit_activity_type_shop, //限时商店
         e_time_limit_activity_type_time_welfare, //经验派送
         e_time_limit_activity_type_behavior_welfare, //启示之路
         e_time_limit_activity_type_word_desc, //加群有礼
         e_time_limit_activity_type_time_welfare2, //战力派送
         e_time_limit_activity_type_time_welfare3, //紫钻派送
         e_time_limit_activity_type_time_welfare4, //时装派送
         e_time_limit_activity_type_treasure_sprite, //限时精灵抽奖
         e_time_limit_activity_type_item_exchange, //兑换活动
         e_time_limit_activity_type_time_welfare5, //周一领奖
         e_time_limit_activity_type_daily_charge2, //每日充值
         e_time_limit_activity_type_total_charge2, //累计充值
         e_time_limit_activity_type_behavior_welfare_reward, //启示之路领奖
         e_time_limit_activity_type_discount_shop, //折扣商店
         e_time_limit_activity_type_small_charge, //小额充值
         e_time_limit_activity_type_total_landing, //累计登陆
         e_time_limit_activity_type_total_cost_jewel, //累计消费
         e_time_limit_activity_type_landing_reward, //登陆领奖
         e_time_limit_activity_type_added_drop, //额外掉落
         e_time_limit_activity_type_add_upgrade_level, //强化加级
         e_time_limit_activity_type_activity_notice_1, //公告板1
         e_time_limit_activity_type_activity_notice_2, //公告板2
         e_time_limit_activity_type_national_treasure_1, //全民聚宝-个人
         e_time_limit_activity_type_national_treasure_2, //全民聚宝-全服
         e_time_limit_activity_type_gs_up, //战力提升榜
         e_time_limit_activity_type_mount_up, //坐骑提升榜
         e_time_limit_activity_type_wing_up, //翅膀提升榜
         e_time_limit_activity_type_equip_up, //装备提升榜
         e_time_limit_activity_type_spirit_up, //精灵提升榜
         e_time_limit_activity_type_cross_gs_up, //跨服战力提升榜
         e_time_limit_activity_type_cross_mount_up, //跨服坐骑提升榜
         e_time_limit_activity_type_cross_wing_up, //跨服翅膀提升榜
         e_time_limit_activity_type_cross_equip_up, //跨服装备提升榜
         e_time_limit_activity_type_cross_spirit_up, //跨服精灵提升榜
         e_time_limit_activity_type_treasure_element, //符文宝藏
         e_time_limit_activity_type_cloud_shop, //云购
         e_time_limit_activity_type_cross_cloud_shop, //跨服云购
         e_time_limit_activity_type_daily_gift_charge, //每日直购礼包
         e_time_limit_activity_type_item_shop, //积分道具商店
         e_time_limit_activity_type_dragontrip, //萌龙奇遇
         e_time_limit_activity_type_special_exchange, //特殊兑换
         e_time_limit_activity_type_new_total_landing, //新累计登录
         e_time_limit_activity_type_new_behavior_welfare, //新启示之路
         e_time_limit_activity_type_new_behavior_welfare_reward, //新启示之路领奖
         e_time_limit_activity_type_sky_treasure, //天空宝库
         e_time_limit_activity_type_starark, //星海方舟
         e_time_limit_activity_type_support_for_promotion, //冲级助力
         e_time_limit_activity_type_weekend_top_up, //周末限时充值
         e_time_limit_activity_type_behavior_welfare1, //启示之路备用1
         e_time_limit_activity_type_behavior_welfare2, //启示之路备用2
         e_time_limit_activity_type_behavior_welfare3, //启示之路备用3
         e_time_limit_activity_type_treasure_royal1, //皇家宝藏备用1
         e_time_limit_activity_type_treasure_royal2, //皇家宝藏备用2
         e_time_limit_activity_type_treasure_royal3, //皇家宝藏备用3
         e_time_limit_activity_type_daily_charge3, //每日充值备用3
         e_time_limit_activity_type_daily_charge4, //每日充值备用4
         e_time_limit_activity_type_daily_charge5, //每日充值备用5
         e_time_limit_activity_type_daily_charge6, //每日充值备用6
         e_time_limit_activity_type_discount_shop1, //折扣商店备用1
         e_time_limit_activity_type_discount_shop2, //折扣商店备用2
         e_time_limit_activity_type_discount_shop3, //折扣商店备用3
         e_time_limit_activity_type_behavior_welfare_reward1, //启示之路领奖备用1
         e_time_limit_activity_type_behavior_welfare_reward2, //启示之路领奖备用2
         e_time_limit_activity_type_behavior_welfare_reward3, //启示之路领奖备用3
         e_time_limit_activity_type_item_shop1, //积分道具商店备用1
         e_time_limit_activity_type_item_shop2, //积分道具商店备用2
         e_time_limit_activity_type_item_shop3, //积分道具商店备用3
         e_time_limit_activity_type_lucky_card, //福牌活动
         e_time_limit_activity_type_cross_total_charge, //跨服累计充值排行榜
         e_time_limit_activity_type_new_total_charge_1, //新累计充值1
         e_time_limit_activity_type_new_total_charge_2, //新累计充值2
         e_time_limit_activity_type_new_total_charge_3, //新累计充值3
         e_time_limit_activity_type_new_total_cost_jewel_1, //新累计消费1
         e_time_limit_activity_type_new_total_cost_jewel_2, //新累计消费2
         e_time_limit_activity_type_new_total_cost_jewel_3, //新累计消费3
         e_time_limit_activity_type_new_server_treasure, //新开服宝藏
         e_time_limit_activity_type_new_server_mail, //新服邮件
         e_time_limit_activity_type_national_treasure_3, //全民聚宝本服_个人
         e_time_limit_activity_type_national_treasure_4, //全民聚宝本服_全服
         e_time_limit_activity_type_daily_free_reward, //每日免费礼包
         e_time_limit_activity_type_time_feed_back1, //时光回馈1
         e_time_limit_activity_type_time_feed_back2, //时光回馈2
         e_time_limit_activity_type_time_feed_back3, //时光回馈3
         e_time_limit_activity_type_limit_time_treasure2, //新限时抽奖
         e_time_limit_activity_type_limit_time_notice, //限时公告
         e_time_limit_activity_type_diamond_shop, //钻石商店
         e_time_limit_activity_type_star_trip, //异界之旅
         e_time_limit_activity_type_max
     };
     enum e_mail_type
     {
         e_mail_type_normal_mail, //常规邮件
         e_mail_type_server_merge_to_normal_player, //合服发给所有人的
         e_mail_type_server_merge_to_legion_del_player, //合服发给军团被删的人的
         e_mail_type_server_merge_to_be_del_player, //合服发给被删号人的
         e_mail_type_max
     };
     enum e_time_limit_activity_info
     {
         e_time_limit_activity_info_type, //类型
         e_time_limit_activity_info_start_time, //开始时间
         e_time_limit_activity_info_end_time, //结束时间
         e_time_limit_activity_info_guid, //主表唯一id
         e_time_limit_activity_info_first_branch_guid, //当前周期子表首唯一id
         e_time_limit_activity_info_max
     };
     enum e_unit_safe_map_info
     {
         e_unit_safe_map_info_pos_x, //玩家位置X
         e_unit_safe_map_info_pos_y, //玩家位置Y
         e_unit_safe_map_info_map_tempate_id, //地图Id
         e_unit_safe_map_info_map_have_not_safe_region, //该地图没有安全区
         e_unit_safe_map_info_safe_map_template_id, //安全区TemplateId
         e_unit_safe_map_info_safe_missiondone_id, //完成主线任务id
         e_unit_safe_map_info_safe_map_type, //安全区类型
         e_unit_safe_map_info_max
     };
     enum e_island_record
     {
         e_island_record_killed_island_boss, //击杀boss次数
         e_island_record_island_collect_count, //采集次数
         e_island_record_cross_pk_collect_count, //跨服竞技采集次数
         e_island_record_oracle_trial_count, //天启试炼扫荡次数
         e_island_record_max
     };
     enum e_login_type_new
     {
         e_login_type_new_account_and_password, //账号密码登录
         e_login_type_new_robot, //机器人登录
         e_login_type_new_token, //token登录
         e_login_type_new_zw, //中文
         e_login_type_new_facebook, //脸书
         e_login_type_new_google, //谷歌
         e_login_type_new_gate_server, //gate登陆获取服务器信息
         e_login_type_new_tw, //台湾版
         e_login_type_new_korea, //韩国版
         e_login_type_new_en, //英文版
         e_login_type_new_umisdk, //泰国版
         e_login_type_new_umisdk_vn, //越南版
         e_login_type_new_appstore, //苹果登录
         e_login_type_new_umisdk_tw, //繁体版
         e_login_type_new_umisdk_jp, //日本
         e_login_type_new_umisdk_rus, //俄罗斯
         e_login_type_new_umisdk_sg, //东南亚
         e_login_type_new_umisdk_new_tw, //新台湾
         e_login_type_new_max
     };
     enum e_payment_method
     {
         e_payment_method_test, //测试充值
         e_payment_method_zw, //中文充值
         e_payment_method_google, //谷歌充值
         e_payment_method_one_store, //onestore充值
         e_payment_method_app_store, //苹果appstore
         e_payment_method_tw_android, //台湾android
         e_payment_method_tw_ios, //台湾ios
         e_payment_method_tw_third, //台湾第三方
         e_payment_method_samsung, //三星商城
         e_payment_method_en_ios, //英文ios
         e_payment_method_en_android, //英文android
         e_payment_method_kr_gameloop, //韩国Gameloop
         e_payment_method_tl_umisdk_google, //泰国umisdk
         e_payment_method_tl_umisdk_ios, //泰国umisdk
         e_payment_method_vn_umisdk_google, //越南google
         e_payment_method_vn_umisdk_ios, //越南ios
         e_payment_method_tw_android_new, //新台湾android
         e_payment_method_tw_ios_new, //新台湾ios
         e_payment_method_tw_third_new, //新台湾第三方
         e_payment_method_tw_umisdk_android, //繁体umisdk_android
         e_payment_method_tw_umisdk_ios, //繁体umisdk_ios
         e_payment_method_tw_umisdk_third, //繁体umisdk_third
         e_payment_method_jp_umisdk_android, //日本Android
         e_payment_method_jp_uminsdk_ios, //日本IOS
         e_payment_method_rus_umisdk_android, //俄罗斯Android
         e_payment_method_rus_uminsdk_ios, //俄罗斯IOS
         e_payment_method_sg_umisdk_android, //东南亚Android
         e_payment_method_sg_uminsdk_ios, //东南亚IOS
         e_payment_method_umisdk_tw_android, //新台湾android
         e_payment_method_umisdk_tw_ios, //新台湾ios
         e_payment_method_umisdk_tw_third, //新台湾第三方
         e_payment_method_umisdk_tw_hw, //新台湾华为
         e_payment_method_max
     };
     enum e_treasure_prize_type
     {
         e_treasure_prize_type_one, //钻石单抽
         e_treasure_prize_type_ten, //钻石10抽
         e_treasure_prize_type_one_item, //物品单抽
         e_treasure_prize_type_ten_item, //物品10抽
         e_treasure_prize_type_max
     };
     enum e_treasure_type
     {
         e_treasure_type_item, //物品
         e_treasure_type_element, //符文
         e_treasure_type_spirit, //精灵
         e_treasure_type_new_server, //新服
         e_treasure_type_time_limit, //限时
         e_treasure_type_max
     };
     enum e_ranking_show_type
     {
         e_ranking_show_type_non, //空
         e_ranking_show_type_gs, //战力榜
         e_ranking_show_type_exp_level, //等级榜
         e_ranking_show_type_equip, //装备榜
         e_ranking_show_type_mount, //坐骑榜
         e_ranking_show_type_wing, //羽翼榜
         e_ranking_show_type_sprite, //精灵榜
         e_ranking_show_type_role, //个人榜
         e_ranking_show_type_challenge, //挑战榜
         e_ranking_show_type_vip, //vip榜
         e_ranking_show_type_legion, //军团榜
         e_ranking_show_type_world_boss, //世界Boss榜
         e_ranking_show_type_elite, //精英讨伐榜
         e_ranking_show_type_cross_service, //跨服榜
         e_ranking_show_type_max
     };
     enum e_time_limit_behavior_type
     {
         e_time_limit_behavior_type_non, //空
         e_time_limit_behavior_type_kill_boss, //击杀地宫boss
         e_time_limit_behavior_type_kill_monster, //杀死野外怪物
         e_time_limit_behavior_type_treasure_lottery, //宝藏抽奖
         e_time_limit_behavior_type_exp_stage, //通关经验副本
         e_time_limit_behavior_type_coin_stage, //通关金币副本
         e_time_limit_behavior_type_illusion_stage, //通关幻化副本
         e_time_limit_behavior_type_belief_stage, //通关信仰副本
         e_time_limit_behavior_type_sprite_stage, //通关精灵副本
         e_time_limit_behavior_type_recharge, //完成任意金额充值
         e_time_limit_behavior_type_auction_bid, //完成拍卖行竞标
         e_time_limit_behavior_type_buy_in_diamond_store, //钻石商店购买商品
         e_time_limit_behavior_type_telves_awaken, //精灵觉醒
         e_time_limit_behavior_type_kill_personal_boss, //击杀个人boss
         e_time_limit_behavior_type_legion_dance, //完成军团共舞
         e_time_limit_behavior_type_kill_world_boss, //击杀世界boss
         e_time_limit_behavior_type_kill_legion_boss, //击杀军团boss
         e_time_limit_behavior_type_arena_win, //竞技场获胜
         e_time_limit_behavior_type_ranking_worship, //排行榜崇拜
         e_time_limit_behavior_type_max
     };
     enum e_passive_skill_info
     {
         e_passive_skill_info_passive_skill_id, //被动技能ID
         e_passive_skill_info_equip_slot, //装配位置
         e_passive_skill_info_cd, //时间戳 秒
         e_passive_skill_info_max
     };
     enum e_legion_skill_info
     {
         e_legion_skill_info_legion_skill_id, //军团技能ID
         e_legion_skill_info_curr_level, //当前军团技能等级
         e_legion_skill_info_max
     };
     enum e_passive_skill_info_type
     {
         e_passive_skill_info_type_non, //默认
         e_passive_skill_info_type_tinder, //火种被动
         e_passive_skill_info_type_sprite, //精灵被动
         e_passive_skill_info_type_mount, //坐骑被动
         e_passive_skill_info_type_wing, //翅膀被动
         e_passive_skill_info_type_feather, //翎羽被动
         e_passive_skill_info_type_phantom, //幻灵被动
         e_passive_skill_info_type_sky_weapon, //天空武器被动
         e_passive_skill_info_type_mount_star, //坐骑赋能
         e_passive_skill_info_type_wing_psychic, //羽翼通灵
         e_passive_skill_info_type_break_will, //心魔意志被动
         e_passive_skill_info_type_direct_get, //直接获取
         e_passive_skill_info_type_max
     };
     enum e_map_order_type
     {
         e_map_order_type_other_raid, //其他类型
         e_map_order_type_story_raid, //剧情副本
         e_map_order_type_daily_raid, //日常副本
         e_map_order_type_multiplayer_raid, //多人副本
         e_map_order_type_max
     };
     enum e_pokedex_Info
     {
         e_pokedex_Info_template_id, //模板ID
         e_pokedex_Info_upgrade_num, //当前等级
         e_pokedex_Info_state, //激活状态
         e_pokedex_Info_fetter_state_1, //羁绊激活1
         e_pokedex_Info_fetter_state_2, //羁绊激活2
         e_pokedex_Info_fetter_state_3, //羁绊激活3
         e_pokedex_Info_psyche_num_1, //精魄转化数目1
         e_pokedex_Info_psyche_num_2, //精魄转化数目2
         e_pokedex_Info_psyche_num_3, //精魄转化数目3
         e_pokedex_Info_psyche_num_4, //精魄转化数目4
         e_pokedex_Info_psyche_num_5, //精魄转化数目5
         e_pokedex_Info_psyche_num_6, //精魄转化数目6
         e_pokedex_Info_psyche_num_7, //精魄转化数目7
         e_pokedex_Info_psyche_num_8, //精魄转化数目8
         e_pokedex_Info_psyche_num_9, //精魄转化数目9
         e_pokedex_Info_psyche_num_10, //精魄转化数目10
         e_pokedex_Info_psyche_num_11, //精魄转化数目11
         e_pokedex_Info_psyche_num_12, //精魄转化数目12
         e_pokedex_Info_psyche_num_13, //精魄转化数目13
         e_pokedex_Info_psyche_num_14, //精魄转化数目14
         e_pokedex_Info_psyche_num_15, //精魄转化数目15
         e_pokedex_Info_psyche_num_16, //精魄转化数目16
         e_pokedex_Info_psyche_num_17, //精魄转化数目17
         e_pokedex_Info_psyche_num_18, //精魄转化数目18
         e_pokedex_Info_psyche_num_19, //精魄转化数目19
         e_pokedex_Info_psyche_num_20, //精魄转化数目20
         e_pokedex_Info_max
     };
     enum e_pokedex_operation_type
     {
         e_pokedex_operation_type_submit_psyche, //精魄收集
         e_pokedex_operation_type_change, //精魄转化
         e_pokedex_operation_type_main_palace_upgrade, //天启殿堂升级
         e_pokedex_operation_type_oracle_activate, //神谕激活
         e_pokedex_operation_type_oracle_upgrade, //神谕升级
         e_pokedex_operation_type_mainland_activate, //地图激活
         e_pokedex_operation_type_mainland_upgrade, //地图升级
         e_pokedex_operation_type_max
     };
     enum e_goddess_operate_type
     {
         e_goddess_operate_type_none, //空
         e_goddess_operate_type_unlock, //女神解锁
         e_goddess_operate_type_equip, //装备女神
         e_goddess_operate_type_unequip, //卸下女神
         e_goddess_operate_type_fight, //女神出战
         e_goddess_operate_type_equip_upgrade, //女神装备升级
         e_goddess_operate_type_goddess_slot_unlock, //女神槽位解锁
         e_goddess_operate_type_max
     };
     enum e_cross_pk_state
     {
         e_cross_pk_state_none, //空
         e_cross_pk_state_16, //16强
         e_cross_pk_state_8, //8强
         e_cross_pk_state_4, //4强
         e_cross_pk_state_2, //决赛
         e_cross_pk_state_max
     };
     enum e_1v1_pk_state
     {
         e_1v1_pk_state_ready, //准备
         e_1v1_pk_state_begin, //开始
         e_1v1_pk_state_end, //结束
         e_1v1_pk_state_max
     };
     enum e_auction_state
     {
         e_auction_state_start_sell, //上架
         e_auction_state_cancel, //取消
         e_auction_state_deal, //成交
         e_auction_state_bid, //竞标
         e_auction_state_max
     };
     enum e_cloud_shop_record
     {
         e_cloud_shop_record_date_value, //活动日期记录
         e_cloud_shop_record_day_index, //活动第几天
         e_cloud_shop_record_template_id, //配置表id
         e_cloud_shop_record_total_number, //活动总购买次数
         e_cloud_shop_record_buy_count, //活动已购买次数
         e_cloud_shop_record_send_award_flag, //奖励领取状态
         e_cloud_shop_record_max
     };
     enum e_seven_day_goal_info
     {
         e_seven_day_goal_info_id, //id
         e_seven_day_goal_info_schedule, //进度
         e_seven_day_goal_info_state, //状态
         e_seven_day_goal_info_open_time, //开始时间
         e_seven_day_goal_info_max
     };
     enum e_harry_record
     {
         e_harry_record_harry_value, //掠夺值
         e_harry_record_defend_value, //守护值
         e_harry_record_normal_harry_count, //每日普通掠夺次数
         e_harry_record_special_harry_count, //每日特殊掠夺次数
         e_harry_record_kill_ct_num, //击杀保卫者个数
         e_harry_record_kill_terrorist_num, //击杀掠夺者个数
         e_harry_record_harry_item_server_id, //采集到的物品归属服务器id
         e_harry_record_max
     };
     enum e_quality_level
     {
         e_quality_level_non, //空
         e_quality_level_one, //一阶
         e_quality_level_two, //二阶
         e_quality_level_three, //三阶
         e_quality_level_four, //四阶
         e_quality_level_five, //五阶
         e_quality_level_six, //六阶
         e_quality_level_seven, //七阶
         e_quality_level_eight, //八阶
         e_quality_level_nine, //九阶
         e_quality_level_ten, //十阶
         e_quality_level_max
     };
     enum e_lua_item_info
     {
         e_lua_item_info_non, //空
         e_lua_item_info_info_id, //模板id
         e_lua_item_info_slot, //物品位置索引e_role_equip，物品装备时代表物品在装备栏中的位置，在物品栏时代表在物品栏中的位置
         e_lua_item_info_container_type, //所在包裹:e_bag_type
         e_lua_item_info_stack_count, //物品堆叠数目
         e_lua_item_info_activate, //是否激活
         e_lua_item_info_is_first, //5是否第一次激活
         e_lua_item_info_upgrade_count, //装备强化次数,坐骑升星数,升阶,翎羽升星数,翎羽升阶，注魂次数，注灵次数，精灵等级,元素之心等级,兽魂等级
         e_lua_item_info_stargrade_count, //坐骑、羽翼碎片升星数
         e_lua_item_info_starskill_flag, //坐骑、羽翼魂星技能开启标记
         e_lua_item_info_add_on, //装备追加次数
         e_lua_item_info_locked, //是否绑定 1视为绑定，0视为未绑定
         e_lua_item_info_wing_exp, //坐骑当前升星经验，元素之心当前经验
         e_lua_item_info_wing_bless, //10坐骑祝福值
         e_lua_item_info_random_had_flag, //拥有的随机属性标记位
         e_lua_item_info_random_property1, //物品随机属性，精灵天赋属性或卓越属性
         e_lua_item_info_random_property2,
         e_lua_item_info_random_property3,
         e_lua_item_info_random_property4,
         e_lua_item_info_random_property5,
         e_lua_item_info_random_property6,
         e_lua_item_info_succinct_property1, //装备洗练属性,坐骑，翅膀，精灵技能激活时间
         e_lua_item_info_succinct_property2,
         e_lua_item_info_succinct_property3,
         e_lua_item_info_succinct_property4,
         e_lua_item_info_succinct_property5, //备用
         e_lua_item_info_succinct_property6,
         e_lua_item_info_showing_illusion_idex, //显示的幻化ID，默认为-1
         e_lua_item_info_put_in_bag_count, //放入背包计数，主要用于确认是否是新获得的物品
         e_lua_item_info_illusion_had_byte, //已获取幻化位标识符
         e_lua_item_info_over_time, //物品到期时间
         e_lua_item_info_succinct_level, //洗练等级
         e_lua_item_info_jewel_slot_0, //宝石槽位0
         e_lua_item_info_jewel_slot_1, //宝石槽位1
         e_lua_item_info_jewel_slot_2, //宝石槽位2
         e_lua_item_info_jewel_slot_3, //宝石槽位3
         e_lua_item_info_jewel_slot_4, //宝石槽位4
         e_lua_item_info_jewel_vip_slot_0, //VIP宝石槽位0
         e_lua_item_info_jewel_vip_slot_1, //VIP宝石槽位1
         e_lua_item_info_prototype_id, //物品原型id 限时物品改变前id
         e_lua_item_info_forge_property1, //物品重铸属性，精灵天赋属性或卓越属性
         e_lua_item_info_forge_property2,
         e_lua_item_info_forge_property3,
         e_lua_item_info_forge_property4,
         e_lua_item_info_forge_property5,
         e_lua_item_info_forge_property6,
         e_lua_item_info_jewel_carve_0, //随机纹刻
         e_lua_item_info_jewel_carve_1, //宝石纹刻1
         e_lua_item_info_jewel_carve_2, //宝石纹刻2
         e_lua_item_info_max
     };
     enum e_money_array_type
     {
         e_money_array_type_cur, //当前钱数
         e_money_array_type_total_get, //总获得数
         e_money_array_type_total_cost, //总消费数
         e_money_array_type_max
     };
     enum e_summon_pet_type
     {
         e_summon_pet_type_normal_summon, //普通召唤物
         e_summon_pet_type_goddess, //女神
         e_summon_pet_type_boss_pet, //BOSS召唤物
         e_summon_pet_type_max
     };
     enum e_version_region_type
     {
         e_version_region_type_china_main, //大陆
         e_version_region_type_kr, //韩国
         e_version_region_type_china_tw, //港澳台
         e_version_region_type_en, //英文版
         e_version_region_type_tha, //泰国
         e_version_region_type_vn, //越南
         e_version_region_type_jp, //日本
         e_version_region_type_rus, //俄罗斯(Russia)
         e_version_region_type_sg, //东南亚(新加坡)
         e_version_region_type_max
     };
     enum e_manual_reward_type
     {
         e_manual_reward_type_add_item, //奖励物品
         e_manual_reward_type_manual_level, //提升手册等级
         e_manual_reward_type_add_point, //增加挑战积分奖励
         e_manual_reward_type_max
     };
     enum e_time_limit_shop_purchase_limit_type
     {
         e_time_limit_shop_purchase_limit_type_none, //不限购
         e_time_limit_shop_purchase_limit_type_day, //每日限购
         e_time_limit_shop_purchase_limit_type_cycle, //周期限购
         e_time_limit_shop_purchase_limit_type_max
     };
     enum e_role_i64_info
     {
         e_role_i64_info_gs_value, //战力值
         e_role_i64_info_history_best_gs_value, //零点战力提升榜初始值
         e_role_i64_info_zero_mount_up_start_value, //零点刷新坐骑提升榜初始值
         e_role_i64_info_zero_wing_up_start_value, //零点刷新翅膀提升榜初始值
         e_role_i64_info_zero_equip_up_start_value, //零点刷新装备提升榜初始值
         e_role_i64_info_zero_spirit_up_start_value, //零点刷新精灵提升榜初始值
         e_role_i64_info_create_time, //创建时间
         e_role_i64_info_max
     };
     enum e_cross_server_pk_sub_type
     {
         e_cross_server_pk_sub_type_berserker, //跨服职业竞技战士
         e_cross_server_pk_sub_type_caster, //跨服职业竞技法师
         e_cross_server_pk_sub_type_add_rider, //跨服职业竞技骑士
         e_cross_server_pk_sub_type_add_assassion, //跨服职业竞技刺客
         e_cross_server_pk_sub_type_word_boss, //跨服世界boss幻影领主
         e_cross_server_pk_sub_type_max
     };
     enum e_star_trip_target_type
     {
         e_star_trip_target_type_buy_goods, //购买商品
         e_star_trip_target_type_buy_single_boss, //购买个人boss
         e_star_trip_target_type_convert, //进行转换
         e_star_trip_target_type_legion_donate, //进行军团捐赠
         e_star_trip_target_type_join_activity, //参与限时活动
         e_star_trip_target_type_buy_today_gift, //购买每日礼包
         e_star_trip_target_type_map_finish, //完成指定副本
         e_star_trip_target_type_kill_single_boss, //击杀个人boss
         e_star_trip_target_type_kill_home_boss, //击杀地宫boss
         e_star_trip_target_type_treasure, //完成抽奖
         e_star_trip_target_type_recharge, //完成充值
         e_star_trip_target_type_bid, //完成竞标
         e_star_trip_target_type_max
     };
     enum e_map_play_type
     {
         e_map_play_type_non, //无操作，直接过
         e_map_play_type_kill_boss, //杀死boss
         e_map_play_type_max
     };
     enum e_map_search_boss_type
     {
         e_map_search_boss_type_boss_no_kill, //boss不可被攻击
         e_map_search_boss_type_boss_can_kill, //boss可以攻击了
         e_map_search_boss_type_boss_wait, //boss等待刷新
         e_map_search_boss_type_boss_refresh, //boss开始刷新
         e_map_search_boss_type_boss_killed, //boss死亡
         e_map_search_boss_type_be_killed, //被杀死了，boss重置
         e_map_search_boss_type_max
     };
     enum e_daily_play_count
     {
         e_daily_play_count_element_map, //符文副本
         e_daily_play_count_battle_map, //战斗副本
         e_daily_play_count_max
     };
     enum e_time_activity_type
     {
         e_time_activity_type_null, //无
         e_time_activity_type_wheel_plate, //转盘玩法
         e_time_activity_type_max
     };
     enum e_time_activity_score_type
     {
         e_time_activity_score_type_nomal_search, //探索到小宝箱
         e_time_activity_score_type_monster_search, //探索到宝箱怪
         e_time_activity_score_type_flip_box_search, //探索到地窟
         e_time_activity_score_type_finger_guess_search, //探索到猜拳
         e_time_activity_score_type_special_search, //探索到金布林
         e_time_activity_score_type_max
     };
     enum e_wheel_plate_info
     {
         e_wheel_plate_info_accumulate_num, //累积数量
         e_wheel_plate_info_open_index, //开启位置
         e_wheel_plate_info_award_index, //本次奖励位置
         e_wheel_plate_info_max
     };
}
#endif
