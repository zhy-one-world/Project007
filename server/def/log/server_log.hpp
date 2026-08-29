/********************************************************************
	created:	2016年12月5日12:00:20
	file base:	server_log
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _SERVER_LOG_HPP_
#define _SERVER_LOG_HPP_

#include "logic/type_def.hpp"
#include "Logic/char_def.hpp"

#include "Logic/login_def.hpp"
#include "Logic/item_def.hpp"
#include "logger_base.hpp"
#include "Logic/ranking_def.hpp"
#include "Logic/auction_def.h"
#include "Logic/gain_treasure_def.hpp"
#include "game_cfg/servers_config.h"


namespace faith
{
#define set_log_var(head)  s_log_common_head head;  

#define set_log_common_head_part1(head, game_id, channel_id, media_id, device_id) \
        head.gameId = game_id;                      \
        head.channelId = channel_id;                \
        head.mediaId = media_id;                     \
        head.deviceId = device_id;                   

#define set_log_common_head_part2(head, version_name,version_code) \
        head.versionName = version_name;                      \
	    head.versionCode = version_code;                       

#define set_log_common_head_part3(head, user_id, server_id, log_time) \
        head.userId = user_id;                                    \
	    head.serverId = init_unit::change_i64_to_string(server_id);                                \
        head.logTime = log_time;                                  

#define set_log_common_head_part4(head,role_id, role_name,role_level,role_gender) \
       head.roleId = init_unit::change_i64_to_string(role_id.server_64);                         \
	   head.roleName = role_name;                     \
       head.roleLevel = role_level;                    \
	   head.roleGender = role_gender;                                 

#define set_log_common_head_part5(head,battle_points ,role_type,vip_level) \
       head.battlePoints = battle_points;                \
       head.roleType = init_unit::change_i64_to_string(role_type);                       \
       head.vipLevel = vip_level;

	struct s_log_common_head
	{
		xstring  gameId;
		xstring  channelId;
		xstring  mediaId;
		xstring  deviceId;
		xstring  versionName;
		int32    versionCode;
		xstring  userId;
		xstring  serverId;
		int64    logTime;
		xstring  roleId;
		xstring  roleName;
		int32    roleLevel;
		int32    roleGender;
		int64    battlePoints;
		xstring  roleType;
		int32    vipLevel;
		s_log_common_head()
		{
			gameId = SERVERCONFIG->app_key;
			channelId = "0";
			mediaId = "0";
			deviceId = "null";
			versionName = "null";
			versionCode = 0;
			userId = "null";
			serverId = "null";
			logTime = 0;
			roleId = "null";
			roleName = "null";
			roleLevel = 0;
			roleGender = 0;
			battlePoints = 0;
			roleType = "null";
			vipLevel = 0;
		}
		void clear_data()
		{
			gameId = SERVERCONFIG->app_key;
			channelId = "0";
			mediaId = "0";
			deviceId = "null";
			versionName = "null";
			versionCode = 0;
			userId = "null";
			serverId = "null";
			logTime = 0;
			roleId = "null";
			roleName = "null";
			roleLevel = 0;
			roleGender = 0;
			battlePoints = 0;
			roleType = "null";
			vipLevel = 0;
		}
	};
	class server_log
	{
		typedef boost::function<void(const void*, size_t, int32)>	sender_handler_type;
	public:
		static sender_handler_type	log_db_sender_handler;
		static logger_base& get_game_log();
	public:
		static void init_new_log(int32 server_id, xstring process_name);

	public:
		static void init_log_db_sender(sender_handler_type handler);
		static void send_log_to_db(const xstring& table_name, const xstring& log_string);
		static void push_log_string(xstring& des_str, const xstring& src_str, const xstring& json_key = "", bool is_final = false);
		static void push_log_string(xstring& des_str, int32 src_num, const xstring& json_key = "", bool is_final = false);
		static void push_log_string(xstring& des_str, int64 src_num, const xstring& json_key = "", bool is_final = false);
		static void push_log_string(xstring& des_str, uint64 src_num, const xstring& json_key = "", bool is_final = false);
		static void push_log_string(xstring& des_str, double src_num, const xstring& json_key = "", bool is_final = false);
		static void push_hadooplog_string(xstring& des_str, const xstring& json_key = "");
		static xstring change_i32_to_string(int32 value_i32);
		static int32  change_string_to_i32(std::string str_i32);


		//login_role_log为token登录日志
		static void login_role_log(int32 server_id, std::string app_key, std::string client_version, xstring gamechannel, xstring adchannel, std::string account, std::string device_id, std::string ip, xstring model, xstring idfa, xstring imei, xstring android_id, xstring mac, xstring sn);
		static void login_create_role_log(int32 server_id, std::string app_key, std::string client_version, xstring gamechannel, xstring adchannel, std::string account, std::string device_id, std::string ip, xstring model, xstring idfa, xstring imei, xstring android_id, xstring mac, xstring sn, int32 login_type);
		static void create_role_log(int32 server_template_id, xstring deviceid, xstring appid, xstring version, xstring gamechannel, xstring adchannel, xstring model, xstring idfa, xstring imei, xstring android_id, xstring mac, xstring sn, const s_unit_info& role_info, xstring ip, int32 login_type);
		//enter_game_role_log为登陆日志 logout_role_log为登出日志
		static void enter_game_role_log(login_fixed_data& third_info, s_unit_info& role_info, s_money_info& money_info, int64 online_time, int64 create_time, int32 login_type);
		static void logout_role_log(login_fixed_data& third_info, s_unit_info& role_info, s_money_info& money_info, int64 online_time, int64 create_time, int32 login_type);
		static void across_day_log(login_fixed_data& third_info, s_unit_info& role_info, int64 online_time, int64 create_time, int32 login_type);
		static void level_up_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 old_level, int32 cur_level, int32 cur_level_time, int32 old_gs, int64 online_time, int32 login_type);
		static void money_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 action, int32 money_id, int64 change_value, int64 remain_value, int32 causeid, int32 location, int64 old_money, int64 new_money, int32 param = 0, int32 param2 = 0, int32 login_type = 0);
		static void item_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 action, int32 item_id, int32 change_value, int32 causeid, int32 location, guid_64 item_guid,int32 left_item_count, int32 login_type);
		static void item_operate_log(login_fixed_data& third_info, s_unit_info& role_info, const std::vector<guid_64>& item_guid, int32 item_slot, int32 operation_type, int32 use_up_item, int32 first_use_lock, int32 item_num, int32 force_do);
		static void buy_goods_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 shop_template_id, int32 good_template_id, int32 buy_num, int32 login_type, int32 last_login_time);
		static void role_mall_log(login_fixed_data& third_info, s_unit_info&role_info, int32 consumed_currency_type, int32 consumed_num, int32 store_id, int32 sold_num, int32 sold_id, int32 login_type);
		static void gettask_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 mission_template_id, int32 get_result, int32 task_type, int32 login_type);
		static void finishtask_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 mission_template_id, int32 finish_result, int32 task_type, int32 login_type);
		static void raid_fight_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 raid_template_id, int32 type, int32 result, int32 map_id, int32 star, int32 diffcult, int32 map_type, int32 relive_times);
		static void achievement_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 achievement_template_id);
		static void new_stages_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 new_stages_id, int32 login_type);
		static void activity_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 must_do_type, int32 activity_id);
		static void heart_beat_role_log(const xstring& appkey, int32 server_version, int32 server_id, int32 online_user, int32 queue_user);
		static void player_count_log(int32 server_id, int32 online_user);
		static void recharge_role_log(login_fixed_data& third_info, s_unit_info& role_info, s_money_info& money_info, int32 add_value, int32 recharge_id);
		static void server_event_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 event_id);
		static void upgrade_equip_log(login_fixed_data& third_info, s_unit_info& role_info, int32 equip_slot, int32 item_id, int32 quality_level, int32 item_color,
			int32 upgrade_total_num, int32 is_success, int32 money_type, int32 money_num, int32 bind_material_num, int32 no_bind_material_num,
			int32 material_one_id, int32 material_one_num, int32 material_two_id, int32 material_two_num, int32 material_three_id, int32 material_three_num, int32 level);
		static void addon_equip_log(login_fixed_data& third_info, s_unit_info& role_info, int32 equip_slot, int32 item_id, int32 quality_level, int32 item_color,int32 cur_addon_level,
			int32 is_success, int32 money_type, int32 money_num, int32 bind_material_num, int32 no_bind_material_num, int32 material_id, int32 material_num);
		static void succinct_equip_log(login_fixed_data& third_info, s_unit_info& role_info, int32 equip_slot, int32 item_id, int32 quality_level, int32 item_color, float culturing_percent, 
			int32 money_type, int32 money_num, int32 bind_material_num, int32 no_bind_material_num, int32 material_id, int32 material_num);
		static void succinct_equip_save_log(login_fixed_data& third_info, s_unit_info& role_info, int32 item_id);
		static void advance_equip_log(login_fixed_data& third_info, s_unit_info& role_info, int32 item_id, int32 quality_level, int32 item_color, int32 excellence_num, int32 upgrade_level, int32 addon_level, float culturing_percent,
										int32 first_material_id, int32 first_material_level, int32 first_material_color, int32 first_material_excellence_num, int32 first_material_upgrade_level, int32 first_material_addon_level, float first_material_culturing_percent,
										int32 second_material_id, int32 second_material_level, int32 second_material_color, int32 second_material_excellence_num, int32 second_material_upgrade_level, int32 second_material_addon_level, float second_material_culturing_percent, 
										int32 finish_item_id, int32 finish_item_quality_level, int32 finish_item_color, int32 finish_item_excellence_num, int32 finish_item_upgrade_level, int32 finish_item_addon_level, float finish_item_culturing_percent);
		static void resurrection_log(int32 map_id, int32 mode, int32 old_jewel_num, int32 new_jewel_num);
		static void send_mail_log(int64 target_player_guid, int64 sender_player_guid,std::string title,std::string content_text,int32 money_typ1,int32 money_num1,
									 int32 money_typ2,int32 money_num2, const s_item_info* item_list,int32 item_type_num);
		static void first_recharge_log(int64 player_guid, bool is_first_recharge);
		static void recharge_log(login_fixed_data& third_info, s_unit_info& role_info, int64 jewel_num, std::string order_id_str, float add_money_value, int32 add_jewel_value, int32 recharge_template_id, bool is_first_recharge, int32 fixed_payment_type = -1, int32 login_type = e_login_type_new_account_and_password);
		static void recharge_step_log(xstring order_id, guid_64 role_id, int32 goods_id, int32 step_num, login_fixed_data& third_info, int32 login_type, const s_unit_info * role_info_ptr=nullptr);
		static void recharge_error_log(xstring order_id, guid_64 role_id, int32 goods_id, int32 error_num);
		static void today_recharge_log(int32 recharge_num, int32 total_num, int32 vip_level);
		static void vip_log(int32 old_vip_level, bool is_get_old_welfare, int32 cur_vip_level);
		static void jewel_consume_log(int32 consume_num, int32 consume_type, int32 total_consume_num);
		static void arena_challenge_log(login_fixed_data& third_info, s_unit_info& role_info);

		static void chat_log(login_fixed_data& third_info, s_unit_info& sender_info, guid_64& addressee_guid, int32 chat_type, std::string chat_text, s_item_info* item_info, int32 item_num, int32 login_type);
		static void lucky_draw_log(login_fixed_data& third_info, s_unit_info& role_info, std::vector<int32>& item_get_array, int32 treasure_type, int32 cost_type, int32 is_first_time_do, int32 old_lucky_value, int32 new_lucky_value, int32 is_free, int32 login_type);
		static void player_mark_log(login_fixed_data& third_info, s_unit_info& role_info, int32 strength, int32 agility, int32 intellect, int32 stamina);
		static void player_pk_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 cur_pk_mode, int32 cur_pk_value); 
		static void player_dead_role_log(login_fixed_data& third_info, s_unit_info& killer_role_info, s_unit_info& dead_role_info, fvector dead_pos, int32 killer_pk_mode, int32 dead_pk_mode, int32 map_type, int32 dead_type);
		static void bag_total_num_log(login_fixed_data& third_info, s_unit_info& role_info, int32 cur_bag_total_num);
		static void bag_unused_num_log(int32 cur_bag_unused_num);
		static void month_card_log(xstring buy_time, int32 buy_num);
		static void element_recovery_log(int32 pound_num);
		static void world_boss_battle_from_player_log(int32 boss_id, int32 hurt_value, int64 battle_begin_time, int64 battle_end_time, int64 battle_continued_time, int32 boss_is_dead, int32 is_team, int32 pk_mode);
		static void world_boss_battle_from_boss_log(login_fixed_data& third_info, s_unit_info& role_info, int32 boss_id, int32 map_id, int32 player_num, int32 battle_begin_time, int32 battle_end_time, int32 battle_continued_time, int32 login_type);
		static void boss_kill_drop_log(login_fixed_data& third_info, s_unit_info& role_info, std::vector< s_item_template_info >& item_list, int32 boss_id, int32 login_type);
		static void elite_boss_battle_from_player_log(int32 boss_id, int32 hurt_value, int64 battle_begin_time, int64 battle_end_time, int64 battle_continued_time, int32 boss_is_dead, int32 is_team, int32 pk_mode);
		static void cross_server_boss_kill_log(int32 boss_id, guid_64 legion_id, xstring legion_name, int32 server_id, guid_64 legion_id_top1, xstring legion_name_top1, int32 legion_server_id_top1, guid_64 legion_id_top2, xstring legion_name_top2, int32 legion_server_id_top2, guid_64 legion_id_top3, xstring legion_name_top3, int32 legion_server_id_top3);
		static void elite_boss_battle_from_boss_log(int32 boss_id, int32 player_num, int64 battle_begin_time, int64 battle_end_time, int64 battle_continued_time);
		static void boss_home_battle_from_boss_log(const s_gain_treasure_record_info_one& record_info);
		static void activity_degree_log(s_unit_info& role_info, int32 new_activity_degree_value);
		static void resource_find_back_log(login_fixed_data& third_info, s_unit_info& role_info, int32 must_do_type, int32 find_back_type, int32 find_back_value);
		static void legion_burn_fire_over_log(guid_64 legion_id, int32 participate_number);
		static void legion_boss_over_log(guid_64 legion_id, int32 participate_number);
		static void legion_war_over_log(guid_64 legion_id, int32 participate_number);
		static void legion_info_log(guid_64 legion_id, xstring legion_name, int32 legion_level, int32 legion_role_num);
		static void create_team_log(guid_64 creater_role_guid, int32 team_aim);
		static void join_team_log(guid_64 creater_role_guid, int32 cur_member_num, int32 team_aim, guid_64 joiner_role_guid);
		static void quit_team_log(guid_64 creater_role_guid, int32 cur_member_num, int32 team_aim, guid_64 joiner_role_guid, int32 is_exist);
		static void chat_channel_log(int32 channel_id, xstring chat_content, guid_64 sender_guid, guid_64 receiver_guid, xstring voice_id, int32 voice_time);
		static void grow_up_log(login_fixed_data& third_info, s_unit_info& role_info, int32 login_type, int32 grow_up_id, int32 grow_up_cost);
		
		static void grade_up_log(s_unit_info& role_info, int32 old_class, int32 new_class);
		static void getmeditation_log(login_fixed_data& third_info, s_unit_info& role_info, int32 meditation_type, int32 meditation_time);
		static void getwelfare_log(login_fixed_data& third_info, s_unit_info& role_info, int32 in_template_type, int32 in_template_index);
		static void map_transfer_log(login_fixed_data& third_info, s_unit_info& role_info, int32 desc_type, int32 desc_map_template_id, int32 line_id, s_map_pos map_pos, guid_64 map_guid, int32 war_index, bool force, int32 dest_group_id);
		static void rank_list_log(ranking_list* rank_list, int32 rank_list_type);

		static void spawn_npc_log(int32 map_id, int32 npc_id, int32 spawn_id, int32 array_idx, guid_64 guid, fvector bornpos);
		static void remove_npc_log(int32 map_id, int32 npc_id, int32 spawn_id, int32 array_idx, guid_64 guid, fvector bornpos);
		static void find_npc_log(int32 map_id, int32 npc_id, int32 spawn_id, int32 array_idx, guid_64 guid, fvector bornpos);
		static void message_minor_header_verstion_error(int32 server_verstion, int32 client_verstion);
		static void dump_npc_log(int32 map_id, int32 npc_id, int32 spawn_id, int32 array_idx, guid_64 guid, fvector bornpos, int32 line_id);
		static void dump_location_unit(int32* unit_array);
		static void dump_location_watch(const unit_index_map& aoi_tower_watch);
		static void dump_msg_info(int32 msg_header, int32 msg_num, int64 msg_size);
		static void dump_msg_info_client(int32 msg_header, int32 msg_num, int64 msg_size);

		static void time_limit_activity_log_zw(login_fixed_data& third_info, s_unit_info& role_info, int32 activity_id, int32 activity_template_id, int32 activity_cycle, int32 activity_day, int32 activity_index, int32 money_type, int32 money_value, std::vector<int32> item_array);

		static void auctionbid_log(login_fixed_data& third_info, s_unit_info& role_info, const int32& money_type, const int32& money_value, const int32& item_template_id, const int32& data_num, const guid_64& buyer_guid);
		static void auction_buy_log(login_fixed_data& third_info, s_unit_info& role_info, int32 item_id, int32 item_num, int32 money_type, int32 item_price);
		static void auction_sell_log(int32 item_id, int32 item_num, int32 money_type, int32 item_price);
		static void legion_auction_log(int64 item_guid, int32 item_num, int32 money_type, int32 item_price);
		static void world_auction_log(int64 item_guid, int32 item_num, int32 money_type, int32 item_price);
		static void auction_db_log(const s_auction_info& auction_info, int32 auction_state, const s_unit_info& role_info = s_unit_info(), const login_fixed_data& third_info = login_fixed_data(), const int32& login_type = e_login_type_new_account_and_password);

		static void mail_get_content_log(const s_unit_info& role_info, const login_fixed_data& third_info, guid_64 mail_guid, int32 reward_id, int32 reward_num, std::string mail_title, std::string text_contents, int32 login_type);

		static void jewel_count_log(const s_unit_info& role_info, const s_logic_info& logic_info, const login_fixed_data& third_info, const s_money_info& money_info, const int32& login_type);
		static void time_limit_activity_log(const s_unit_info& role_info, const login_fixed_data& third_info, const int32& activity_id, const int32& activity_type, const int32& cur_value, const int32& target_value, const int32& reward_id, const int32& reward_num, int32 login_type, int32 cost_money_type,int32 cost_money_num,int32 limit_act_branch_temp_id);

		static void map_change_log(login_fixed_data& third_info, s_unit_info& role_info, int32 login_type, int32 map_id, int32 map_type, int32 map_sub_type, int32 change_type, int32 stage_type);
		static void cross_server_harry_log(login_fixed_data& third_info, s_unit_info& role_info, int32 login_type, int32 oper_type, int32 harry_type, int32 harry_server_id, int32 normal_harry_count, int32 sepcail_harry_count);
		static void occupation_pk_log(login_fixed_data& third_info, s_unit_info& role_info, int32 login_type, int32 class_type, int32 state_value, guid_64 oppo_guid, int32 pk_result, std::vector<s_item_template_info>& item_list);
		static void occupation_pk_log(guid_64 player_guid, int32 class_type, int32 state_value, guid_64 oppo_guid, int32 pk_result, std::vector<s_item_template_info>& item_list);

		static void serverAccountCreate(s_log_common_head &lg_common_head);
		static void serverRoleLogin(s_log_common_head &lg_common_head);
		static void serverRoleCreate(s_log_common_head &lg_common_head);
		static void serverRoleUpgrade(s_log_common_head &lg_common_head, int32 newRoleLevel, int32 spendSecond);
		static void serverVipUpgrade(s_log_common_head &lg_common_head, int32 vipLevel, int32 spendSecond);
		static void serverNewbie(s_log_common_head &lg_common_head, xstring step);
		static void serverTask(s_log_common_head &lg_common_head, xstring taskId, xstring taskType, int32 taskStatus, int32 spendSecond);
		static void serverDungeon(s_log_common_head &lg_common_head, xstring dungeonId, xstring  dungeonType, int32 dungeonLevel, int32 battleType, int32 dungeonResult, int32 resultStar, int32 spendSecond);
		static void serverCurrencyChange(s_log_common_head &lg_common_head, xstring currencyId, int32 changeType, int32 recharge, int32 changeDescribe, int32 changeCount, int32 residueCount, int32 changeLocation);
		static void serverPropChange(s_log_common_head &lg_common_head, xstring propId, int32 changeType, int32 changeDescribe, int32 changeCount, int32 residueCount, int32 changeLocation);
		static void serverPropUpgrade(s_log_common_head &lg_common_head, xstring propId, int32 changeType, xstring propQuality, xstring propStart, xstring propLevel);

		static void serverRechargeStep(s_log_common_head &lg_common_head, xstring orderId, xstring goodsId, int32 stepNum);
		static void serverPlayerCount(s_log_common_head &lg_common_head, int32 playerCount);
		static void serverRechargeSuccess(s_log_common_head &lg_common_head, int32 roleVip, xstring gameOrderId, xstring gameChannelOrderId,
				float orderAmount, float shareaMount, float noShareaMount, int32 payId, int32 loginType, xstring rechargeChannel,
				int32 addJewel, int32 totalJewel, int32 currencyType, int32 itemId, int32 isFirstRecharge, xstring deviceModel);
		static void serverLuckyDraw(s_log_common_head &lg_common_head, int32 itemId, int32 itemNum, int32 treasureType, int32 costType,
				int32 isFirstTimeDo, int32 oldLuckyValue, int32 newLuckyValue, int32 isFree);
		static void serverAuction(s_log_common_head &lg_common_head, xstring sellerAccountId, xstring sellerRoleGuid, xstring sellerRoleName, int32 sellerRoleLevel,
			xstring itemGuid, int32 itemId, int32 itemNum, int32 auctionType,
			xstring buyerAccountId, xstring buyerRoleGuid, xstring buyerRoleName, int32 buyerRoleLevel,
			int32 moneyType, xstring moneyName, int32 moneyValue, int32 auctionState);
		static void serverChat(s_log_common_head &lg_common_head, xstring targetId, int32 chatType, xstring chatContent);
		static void serverBuyGoods(s_log_common_head &lg_common_head, int32 goodsId, xstring goodsName, int32 buyNum, int32 itemId,
			xstring itemName, int32 itemNumPerGoods, int32 costMoneyType, xstring constMoneyName, int32 costMoneyNum);


		static void serverGetMailContent(s_log_common_head &lg_common_head, int32 itemId, int32 itemNum, int64 mailGuid ,xstring mailTitle);
		static void serverGrowthFund(s_log_common_head &lg_common_head, int32 purchaseGrowthFundId, int32 purchaseCost, int32 purchaseQuantity);
		static void serverCrossServerHarry(s_log_common_head &lg_common_head, int32 operType, int32  harryType, int32 harryServerId, int32 normalHarryCount, int32 sepcailHarryCount);

	


	    static void serverCrossServerBossKill(s_log_common_head &lg_common_head, int32 bossId, int64 killLegionId, xstring killLegionName, int64 top1Id, xstring top1Name,
				int64 top2Id, xstring top2Name, int64 top3Id, xstring top3Name);


		static void serverOccupationPk(s_log_common_head &lg_common_head, int32 classType, int32 stateValue, int64 oppoGuid, int32 pkResult, std::vector<s_item_template_info>& item_list);

		static void serverBossKillDrop(s_log_common_head &lg_common_head, int32 bossId, std::vector< s_item_template_info >& item_list);



		static void serverBossKill(s_log_common_head &lg_common_head, int32 bossId, int32 mapId, int32 playerNum, int64 battleBeginTime, int64 battleEndTime, int64 battleContinuedTime);

	

		static void serverMapChange(s_log_common_head &lg_common_head, int32 mapTemplateId, int32 mapType, int32 mapSubType, int32 changeType, int32 stateType);


		static void serverTimeLimitActivity(s_log_common_head &lg_common_head, int32 curValue, int32 targetValue, int32 rewardId, int32 rewardNum, int32 activityType);


		static void serverRoleMallLog(s_log_common_head &lg_common_head, int32 activityId, int32 rewardId, int32 rewardNum, int32 consumedCurrencyType, int32 consumedNum, xstring storeName, xstring storeId, int32 soldNum, xstring soldCurrecyId, int32 soldNumAmount);

		static void serverLegionLog(s_log_common_head &lg_common_head, xstring legionName, int32 legionLevel, int64  legionPower, int32 legionNum);
		
		static void reconnect_game_log(std::string str_server_type, std::string func_name, std::string context_);

		static void serverLegionWelfareGrantNumLog(s_log_common_head & lg_common_head, guid_64 legion_guid, int32 rewardNum, int32 activeNum);
		static void serverLegionActivrNumLog(s_log_common_head & lg_common_head, int32 activeNum);

		static void serverAssistantLog(s_log_common_head &lg_common_head, int32 mapType);

		static void serverRankList(s_log_common_head &lg_common_head, s_ranking_player_info rank_info, int32 rank_index);

		static void serverTreasureLog(s_log_common_head &lg_common_head, int32 type, int32 bossId, int32 num);
		
		static void serverBeatBoss(s_log_common_head &lg_common_head, int32 activeType, int32 type, int32 bossId);
	
		static void serverManorLog(s_log_common_head &lg_common_head, int32 activeType, int32 type);

		static void serverCrystalLog(s_log_common_head &lg_common_head, int32 type);

		static void serverLadderLog(s_log_common_head &lg_common_head, int32 type);

		static void serverPkKingLog(s_log_common_head &lg_common_head, int32 type);

		static void serverGodnessStar(s_log_common_head &lg_common_head, int32 level);

		static void serverEarlyWarning(s_log_common_head &lg_common_head, int32 onlineTime);
		
	};
#define PROJECT_RECORD_LOG(OBJ, MSG, ...)			PROJECT_RECORD_LOG_PRIVATE(OBJ, MSG, __VA_ARGS__)
#define SERVER_RUNING_LOG(MSG, ...)					SERVER_RUNING_LOG_PRIVATE(server_log::get_game_log(), MSG, __VA_ARGS__)
#define CONSOLE_INFO(MSG, ...)						{console_info("{}({})|| "##MSG, __FUNCTION__, __LINE__, __VA_ARGS__); SERVER_RUNING_LOG(MSG, __VA_ARGS__)}
#define CONSOLE_ERROR(MSG, ...)						{console_error("{}({})|| "##MSG, __FUNCTION__, __LINE__, __VA_ARGS__); SERVER_RUNING_LOG(MSG, __VA_ARGS__)}
}

#endif // _SERVER_LOG_HPP_
