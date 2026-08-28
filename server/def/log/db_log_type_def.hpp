#ifndef _DB_LOG_TYPE_DEF_H_
#define _DB_LOG_TYPE_DEF_H_

#include "logic/type_def.hpp"
// role_logout_log
const xstring TABLE_NAME_LOGOUT = "role_logout_log";
const xstring TABLE_FILED_LOGOUT[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"channel_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"op_type",
	"create_time",
	"online_time",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// create_role_log
const xstring TABLE_NAME_CREATE = "create_role_log";
const xstring TABLE_FILED_CREATE[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	"career",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// level_change_log
const xstring TABLE_NAME_LEVELCHANGE = "level_change_log";
const xstring TABLE_FILED_LEVELCHANGE[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"old_level",
	"new_level",
	"online_time",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// role_task_log
const xstring TABLE_NAME_TASK = "role_task_log";
const xstring TABLE_FILED_TASK[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"op_type",
	"task_type",
	"task_id",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// money_change_log
const xstring TABLE_NAME_MONEYCHANGE = "money_change_log";
const xstring TABLE_FILED_MONEYCHANGE[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"role_vip",
	"money_type",
	"old_money",
	"new_money",
	"op_type",
	"cause_id",
	"location",
	"param2",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// item_change_log
const xstring TABLE_NAME_ITEMCHANGE = "item_change_log";
const xstring TABLE_FILED_ITEMCHANGE[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	

	"role_vip",
	"item_id",
	"change_num",
	"left_num",
	"op_type",
	"cause_id",
	"location",
	"item_guid",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// new_guide_log
const xstring TABLE_NAME_NEWGUIDE = "new_guide_log";
const xstring TABLE_FILED_NEWGUIDE[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"step_id",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// recharge_step_log
const xstring TABLE_NAME_RECHARGE_STEP = "recharge_step_log";
const xstring TABLE_FILED_RECHARGE_STEP[] = {
	"server_id",
	"log_time",
	"order_id",
	"role_id",
	"goods_id",
	"step_num",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// player_count_log
const xstring TABLE_NAME_PLAYERCOUNT = "player_count_log";
const xstring TABLE_FILED_PLAYERCOUNT[] = {
	"server_id",
	"log_time",
	"player_count",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// recharge_log
const xstring TABLE_NAME_RECHARGE_SUCCESS = "recharge_success_log";
const xstring TABLE_FILED_RECHARGE_SUCCESS[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"role_vip",
	"game_order_id",
	"game_channel_order_id",
	"order_amount",
	"share_amount",
	"no_share_amount",
	"pay_id",
	"recharge_channel",
	"add_jewel",
	"total_jewel",
	"currency_type",
	"item_id",
	"is_first_recharge",
	"device_model",
	"android_id",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// grow_up_log
const xstring TABLE_NAME_GROW_UP = "growth_fund";
const xstring TABLE_FILED_GROW_UP[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"role_vip",
	"purchase_growth_fund_id",
	"purchase_cost",
	"purchase_quantity",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};
// account_create_log
const xstring TABLE_NAME_ACCOUNT_CREATE = "account_create_log";
const xstring TABLE_FILED_ACCOUNT_CREATE[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};


// lucky_draw_log
const xstring TABLE_NAME_LUCKY_DRAW_LOG = "role_lucky_draw_log";
const xstring TABLE_FILED_LUCKY_DRAW_LOG[] = {
	"server_id",
	"log_time",
	"account_id",
	"role_id",
	"role_name",
	"role_level",
	
	"item_id",
	"item_num",
	"treasure_type",
	"cost_type",
	"is_first_time_do",
	"old_lucky_value",
	"new_lucky_value",
	"is_free",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};


// auction_log
const xstring TABLE_NAME_AUCTION_LOG = "auction_log";
const xstring TABLE_FILED_AUCTION_LOG[] = {
	"server_id",
	"log_time",
	"seller_account_id",
	"seller_role_guid",
	"seller_role_name",
	"seller_role_level",
	"item_guid",
	"item_id",
	"item_num",
	"auction_type",
	"buyer_account_id",
	"buyer_role_guid",
	"buyer_role_name",
	"buyer_role_level",
	"money_type",
	"money_value",
	"auction_state",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};


// goods_log
const xstring TABLE_NAME_GOODS_LOG = "role_buy_goods_log";
const xstring TABLE_FILED_GOODS_LOG[] = {
	"server_id",
	"log_time",
	"account_id",
	"role_id",
	"role_name",
	"role_level",
	
	"goods_id",
	"buy_num",
	"item_id",
	"item_num_per_goods",
	"cost_money_type",
	"cost_money_num",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};


// mail_log
const xstring TABLE_NAME_MAIL_GET_CONTENT_LOG = "role_get_mail_content_log";
const xstring TABLE_FILED_MAIL_GET_CONTENT_LOG[] = {
	"server_id",
	"log_time",
	"account_id",
	"role_id",
	"role_name",
	"role_level",
	
	"reward_id",
	"reward_num",
	"mail_guid",
	"mail_title",
	"text_contents",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// jewel_count_log
const xstring TABLE_NAME_JEWEL_COUNT_LOG = "role_jewel_count";
const xstring TABLE_FILED_JEWEL_COUNT_LOG[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"role_vip",
	"total_recharge_jewel",
	"total_consume_jewel",
	"total_recharge_bind_jewel",
	"total_consume_bind_jewel",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// time_limit_activity_count_log
const xstring TABLE_NAME_TIME_LIMIT_ACTIVITY_LOG = "role_time_limit_activity_log";
const xstring TABLE_FILED_TABLE_NAME_TIME_LIMIT_ACTIVITY_LOG[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"role_vip",
	"activity_id",//限时活动id
	"activity_type",//限时活动类型
	"cur_value",//当前指
	"target_value",//目标值
	"reward_id",//奖励的物品id
	"reward_num",//奖励的物品数量

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

const xstring TABLE_NAME_BOSS_KILL_LOG = "boss_kill_log";
const xstring TABLE_FILED_TABLE_NAME_BOSS_KILL_LOG[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"role_vip",
	"boss_id",
	"map_id",
	"player_num",
	"battle_begin_time",
	"battle_end_time",
	"battle_continued_time",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};
const xstring TABLE_NAME_BOSS_KILL_DROP_LOG = "boss_kill_drop_log";
const xstring TABLE_FILED_TABLE_NAME_BOSS_KILL_DROP_LOG[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"role_vip",
	"boss_id",
	"drop_item_id1",
	"drop_item_num1",
	"drop_item_id2",
	"drop_item_num2",
	"drop_item_id3",
	"drop_item_num3",
	"drop_item_id4",
	"drop_item_num4",
	"drop_item_id5",
	"drop_item_num5",
	"drop_item_id6",
	"drop_item_num6",
	"drop_item_id7",
	"drop_item_num7",
	"drop_item_id8",
	"drop_item_num8",
	"drop_item_id9",
	"drop_item_num9",
	"drop_item_id10",
	"drop_item_num10",
	"drop_item_id11",
	"drop_item_num11",
	"drop_item_id12",
	"drop_item_num12",
	"drop_item_id13",
	"drop_item_num13",
	"drop_item_id14",
	"drop_item_num14",
	"drop_item_id15",
	"drop_item_num15",
	"drop_item_id16",
	"drop_item_num16",
	"drop_item_id17",
	"drop_item_num17",
	"drop_item_id18",
	"drop_item_num18",
	"drop_item_id19",
	"drop_item_num19",
	"drop_item_id20",
	"drop_item_num20",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

const xstring TABLE_NAME_CROSS_SERVER_BOSS_KILL_LOG = "cross_server_boss_kill_log";
const xstring TABLE_FILED_TABLE_NAME_CROSS_SERVER_BOSS_KILL_LOG[] = {
	"server_id",
	"log_time",
	"boss_id",
	"kill_legion_id",
	"kill_legion_name",
	"top1_id",
	"top1_name",
	"top1_server_id",
	"top2_id",
	"top2_name",
	"top2_server_id",
	"top3_id",
	"top3_name",
	"top3_server_id",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

// map_change_log
const xstring TABLE_NAME_MAP_CHANGE = "map_change_log";
const xstring TABLE_FILED_MAP_CHANGE[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
    "role_power",
	
    "role_vip",

    "map_template_id",
    "map_type",
    "map_sub_type",
    "change_type",
    "stage_type",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

const xstring TABLE_NAME_CROSS_SERVER_HARRY = "cross_server_harry_log";
const xstring TABLE_FILED_CROSS_SERVER_HARRY[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"role_vip",

	"oper_type",
	"harry_type",
	"harry_server_id",
	"normal_harry_count",
	"sepcail_harry_count",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

const xstring TABLE_NAME_OCCUPATION_PK = "occupation_pk_log";
const xstring TABLE_FILED_OCCUPATION_PK[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"role_vip",

	"class_type",
	"state_value",
	"oppo_guid",
	"pk_result",
	"item_id_1",
	"item_num_1",
	"item_id_2",
	"item_num_2",
	"item_id_3",
	"item_num_3",
	"item_id_4",
	"item_num_4",
	"item_id_5",
	"item_num_5",
	"item_id_6",
	"item_num_6",
	"item_id_7",
	"item_num_7",
	"item_id_8",
	"item_num_8",
	"item_id_9",
	"item_num_9",
	"item_id_10",
	"item_num_10",

	"login_type",
	"appid",
	"version",
	"stepnumid",
	"normversion",
	"gamechannel",
	"ip",
	"adchannel",
	"cross_service",
	"os_version",
	"model",
	"idfa",
	"imei",
	"androidId",
	"mac",
	"sn",
};

const xstring TABLE_NAME_RANK_LIST = "rank_list_log";
const xstring TABLE_FILED_RANK_LIST[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	"role_vip",

	"ranking_type",
	"ranking_value",
	"ranking_time",
	"supporting_guid",
	"supporting_name",
	"vip_level",
	"peak",
	"game_channel",
	"ranking_index",
};

const xstring TABLE_NAME_MALL = "role_mall_log";
const xstring TABLE_FILED_MALL[] = {
	"server_id",
	"log_time",
	"account_id",
	"device_id",
	"role_id",
	"role_name",
	"role_level",
	"role_power",
	
	"role_vip",
	"activity_id",
	"reward_id",
	"reward_num",
	"consumed_currency_type",
	"consumed_num",
	"store_name",
	"store_id",
	"sold_num",
	"sold_id",
	"sold_currency_id",
	"sold_num_amount",
	"channel_id",
	"login_type",
};

const xstring TABLE_NAME_LEGION_GRANT = "legion_grant_log";
const xstring TABLE_FILED_LEGION_GRANT[] = {
	"server_id",
	"log_time",
	"legion_id",
	"legionRewardNum",
	"legionActiveNum",
};

const xstring TABLE_NAME_LEGION_ACTIVE = "legion_active_log";
const xstring TABLE_FILED_LEGION_ACTIVE[] = {
	"server_id",
	"log_time",
	"legionActiveNum",
};

const xstring TABLE_NAME_ROLE_LOGIN = "role_login";

const xstring TABLE_NAME_OPERATE_ITEM = "operate_item";

const xstring TABLE_NAME_ROLE_FIGHT_RAID = "role_fight_raid";

const xstring TABLE_NAME_ROLE_ACHIEVEMENT = "role_achievement";

const xstring TABLE_NAME_ROLE_ACTIVITY = "role_activiity";

const xstring TABLE_NAME_ROLE_HEART_BEAT = "role_heart_beat";

const xstring TABLE_NAME_ROLE_RECHARGE = "role_recharge";

const xstring TABLE_NAME_ROLE_SERVER_EVENT = "role_server_event";

const xstring TABLE_NAME_UPGRADE_EQUIP = "upgrade_equip";

const xstring TABLE_NAME_ADDON_EQUIP = "addon_equip";

const xstring TABLE_NAME_SUCCINCT_EQUIP = "succinct_equip";

const xstring TABLE_NAME_SUCCINCT_EQUIP_SAVE = "succinct_equip_save";

const xstring TABLE_NAME_ADVANCE_EQUIP = "advance_equip";

const xstring TABLE_NAME_RESURRECTION = "resurrection";

const xstring TABLE_NAME_SEND_MAIL = "send_mail";

const xstring TABLE_NAME_FIRST_RECHARGE = "first_recharge";

const xstring TABLE_NAME_TODAY_RECHARGE = "today_recharge";

const xstring TABLE_NAME_RECHARGE_ERROR = "recharge_error";

const xstring TABLE_NAME_VIP = "vip";

const xstring TABLE_NAME_JEWEL_CONSUME_LOGGER = "jewel_consume_logger";

const xstring TABLE_NAME_ARENA_CHALLENGE = "arena_challenge";

const xstring TABLE_NAME_CHAT = "chat";

const xstring TABLE_NAME_ROLE_PLAYER_PK = "role_player_pk";

const xstring TABLE_NAME_PLAYER_MARK = "player_mark";

const xstring TABLE_NAME_ROLE_PLAYER_DEAD = "role_payer_dead";

const xstring TABLE_NAME_NUM_BAG_TOTAL = "num_bag_total";

const xstring TABLE_NAME_NUM_BAG_UNUSED = "num_bag_unused";

const xstring TABLE_NAME_MONTH_CARD = "month_card";

const xstring TABLE_NAME_ELEMENT_RECOVERY = "element_recovery";

const xstring TABLE_NAME_WORLD_BOSS_BATTLE_FROM_PLAYER = "world_boss_battle_from_player";

const xstring TABLE_NAME_ELITE_BOSS_BATTLE_FROM_PLAYER = "elite_boss_battle_from_player";

const xstring TABLE_NAME_ELITE_BOSS_BATTLE_FROM_BOSS = "elite_boss_battle_from_boss";

const xstring TABLE_NAME_ACTIVITY_DEGREE = "activity_degree";

const xstring TABLE_NAME_RESOURCE_FIND_BACK = "resource_find_back";

const xstring TABLE_NAME_LEGION_BURN_FIRE_OVER = "legion_burn_fire_over";

const xstring TABLE_NAME_LEGION_BOSS_OVER = "legion_boss_over";

const xstring TABLE_NAME_LEGION_WAR_OVER = "legion_war_over";

const xstring TABLE_NAME_LEGION_INFO = "legion_info";

const xstring TABLE_NAME_CREATE_TEAM = "create_team";

const xstring TABLE_NAME_JONIN_TEAM = "join_team";

const xstring TABLE_NAME_QUIT_TEAM = "quit_team";

const xstring TABLE_NAME_CHAT_CHANNEL = "chat_channel";

const xstring TABLE_NAME_GRADE_UP = "grade_up";

const xstring TABLE_NAME_GETMEDITATION = "getmeditation";

const xstring TABLE_NAME_GETWELFARE = "getwelfare";

const xstring TABLE_NAME_MAP_TRANSFER = "map_transfer";

const xstring TABLE_NAME_TIME_LIMIT_ACTIVITY = "time_limit_activity";

const xstring TABLE_NAME_AUCTIONBID = "auctionbid";

const xstring TABLE_NAME_AUTION_BUY = "auction_buy";

const xstring TABLE_NAME_AUCTION_SELL = "auction_sell";

const xstring TABLE_NAME_LEGION_AUCTION = "legion_auction";

const xstring TABLE_NAME_WORLD_AUCTION = "world_auction";

const xstring TABLE_NAME_RECONNECT_GAME = "reconnect_game";


#endif