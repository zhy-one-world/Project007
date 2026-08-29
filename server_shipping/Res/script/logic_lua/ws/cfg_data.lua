local ws_t_files = {
	"common",
	"class",
	"functions",
	"template_manager",
	"init_unit",
	"proto_mgr",
	"const_define",
	"register_data",
	"data_flag",

	"message_mgr",
	"arena_mgr_ws",
	"ws2dp_send",
	"d2w_msgproc_lua",
	"c2ws_msgproc",
	"cs2ws_msg",
	"red_package",
	"red_package_mgr",
	"arena_mgr",
	"rename_mgr",
	"assist_fight_mgr",
	"auction_mgr",
	"gain_treasure_mgr",
	"best_record_mgr",
	"big_player_mgr"
}


g_t_files = ws_t_files

local m_TemplateName =
{
    "VIPTemplate",
    "MapTemplate",
    "RedPackageTemplate",
    "GameConfigTemplate",
    "NoticeTemplate",
    "ArenaRobotTemplate",
    "NpcTemplate",
    "ItemTemplate",
    "ActivityCommonConfigTemplate",
    "NpcSpawnPointTemplate",
}

g_TemplateName = m_TemplateName



local t_one_files = 
{
	class = 1,
    register_data = 1,
}



g_one_files = t_one_files


local t_one_init = 
{
	red_package_mgr = 1,
	arena_mgr = 1,
	rename_mgr = 1,
	assist_fight_mgr = 1,
	auction_mgr = 1,
	gain_treasure_mgr = 1,
	best_record_mgr = 1,
	big_player_mgr = 1,

}

g_one_init = t_one_init


local t_debug_files = 
{
   --"red_package",
   --red_package_mgr",
   --"arena_mgr",
   --"rename_mgr",
   --"assist_fight_mgr"
   --"auction_mgr",
   --"gain_treasure_mgr",
   "big_player_mgr"

}

g_debug_files = t_debug_files



local define_c = {}

define_C = define_c






