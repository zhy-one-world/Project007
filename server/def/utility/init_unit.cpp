/********************************************************************
	created:	2014/07/26
	created:	26:7:2014   16:22
	file base:	guid_gen
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/

#include "time.hpp"
#include "template/template_manager.h"
#include "init_unit.h"
#include "utility/random.h"
#include "guid_gen.h"
#include "globle_data.h"
#include "Logic/game_enum_def_s.h"
#include "Logic/mission_def.hpp"
#include "cs_date.hpp"
#include "time_def.hpp"
#include "arena_def.hpp"
#include "mail_def.hpp"
#include "title_def.hpp"
#include "server_log.hpp"
#include "Logic/time_limit_activity_def.hpp"

namespace faith
{
	init_unit::init_unit()
	{

	}

	init_unit::~init_unit()
	{

	}

	int init_unit::get_left_shift_operation_result(int left_operator, int right_operator)
	{
		return left_operator << right_operator;
	}

	int init_unit::get_bitwise_and_operation_result(int left_operator, int right_operator)
	{
		return left_operator & right_operator;
	}

	int init_unit::get_bitwise_or_operation_result(int left_operator, int right_operator)
	{
		return left_operator | right_operator;
	}

	int32 init_unit::get_flag_data(int32 _flag_datas, int32 _idx)
	{
		if (_idx <= 0 || _idx > 32)
		{
			return 0;
		}
		return (_flag_datas >> (_idx - 1)) & 1;
	}

	void init_unit::mark_flag_data(int32& _flag_datas, int32 _idx)
	{
		if (_idx <= 0 || _idx > 32)
		{
			return;
		}
		_flag_datas |= (1 << (_idx - 1));
	}

	void init_unit::erase_flag_data(int32& _flag_datas, int32 _idx)
	{
		if (_idx <= 0 || _idx > 32)
		{
			return;
		}
		_flag_datas &= ~(1 << (_idx - 1));
	}
	
	int32 init_unit::get_flag_data(int64 _flag_datas, int32 _idx)
	{
		if (_idx <= 0 || _idx > 64)
		{
			return 0;
		}
		return (_flag_datas >> (_idx - 1)) & 1;
	}

	void init_unit::mark_flag_data(int64& _flag_datas, int32 _idx)
	{
		if (_idx <= 0 || _idx > 64)
		{
			return;
		}
		_flag_datas |= ((i64)1 << (_idx - 1));
	}

	void init_unit::erase_flag_data(int64& _flag_datas, int32 _idx)
	{
		if (_idx <= 0 || _idx > 64)
		{
			return;
		}
		_flag_datas &= ~((i64)1 << (_idx - 1));
	}

	int32 init_unit::get_flag_data_2bit(int32 _flag_datas, int32 _idx)
	{
		if (_idx <= 0 || _idx > 16)
		{
			return 0;
		}
		return (_flag_datas >> ((_idx - 1) * 2)) & 3;
	}

	void init_unit::set_flag_data_2bit(int32& _flag_datas, int32 _idx, uint32 flag_val)
	{
		if (_idx <= 0 || _idx > 16)
		{
			return;
		}
		if (flag_val > 3)
		{
			return;
		}
		erase_flag_data_2bit(_flag_datas, _idx);
		_flag_datas |= (flag_val << ((_idx - 1) * 2));
	}

	void init_unit::erase_flag_data_2bit(int32& _flag_datas, int32 _idx)
	{
		if (_idx <= 0 || _idx > 16)
		{
			return;
		}
		_flag_datas &= ~(3 << ((_idx - 1) * 2));
	}

	bool init_unit::is_had_this_flag(int32 _flag_datas, int32 _idx)
	{
		if (_idx < 0
			|| _idx >= 32)
		{
			return false;
		}

		int32 temp_value = 1 << _idx;

		return (_flag_datas & temp_value) > 0;
	}

	string init_unit::implode(const vector<string>& v_src, string sep)
	{
		string ret_ = "";
		if (v_src.size() <= 0)
		{
			return ret_;
		}

		ret_ = v_src[0];
		for (int32 str_idx = 1; str_idx < v_src.size(); str_idx++)
		{
			ret_ = ret_ + sep + v_src[str_idx];
		}

		return ret_;
	}

	void init_unit::explode(const string& str_src, vector<string>& v_ret, string sep)
	{
		v_ret.clear();
		if (str_src.empty())
		{
			return;
		}

		string tmp;
		string::size_type pos_begin = str_src.find_first_not_of(sep);
		string::size_type comma_pos = 0;
		int32 loop_times = 0;

		while (pos_begin != string::npos && loop_times < 300)
		{
			loop_times++;
			comma_pos = str_src.find(sep, pos_begin);
			if (comma_pos != string::npos)
			{
				tmp = str_src.substr(pos_begin, comma_pos - pos_begin);
				pos_begin = comma_pos + sep.length();
			}
			else
			{
				tmp = str_src.substr(pos_begin);
				pos_begin = comma_pos;
			}

			if (!tmp.empty())
			{
				v_ret.push_back(tmp);
				tmp.clear();
			}
		}
	}
	char* init_unit::get_host_ip(void) 
	{
		char cHost[256];

		gethostname(cHost, 256);

		hostent *pHost = gethostbyname(cHost);
		in_addr addr;

		char *p = pHost->h_addr_list[0];
		memcpy(&addr.S_un.S_addr, p, pHost->h_length);

		return inet_ntoa(addr);
	}
	void init_unit::format_str(char* szDest, int32 nDestSize, const char* szText, const vector<string>& vtVar)
	{
		memset(szDest, 0, nDestSize);
		string strTemp(szText);
		
		if (vtVar.empty() || nDestSize < strTemp.length())
		{
			strncpy(szDest, strTemp.c_str(), nDestSize - 1);
			return;
		}
 		
		string strSub;
		const char* szSpn = "%s";
		int32 nBeginPos = 0;
		int32 nEndPos   = 0;
		int32 nDestLenCur = 0;
 		
		vector<string>::const_iterator iter_begin = vtVar.begin();
		vector<string>::const_iterator iter_end   = vtVar.end();
		for (; iter_begin != iter_end; ++iter_begin)
		{
			string strParam = *iter_begin;
			strSub.clear();
			nEndPos = strTemp.find(szSpn);

			if (nEndPos == string::npos)
			{
				break;
			}

			strSub = strTemp.substr(nBeginPos, nEndPos - nBeginPos + 2); // 截取前面的字串XXX%s
			strTemp = strTemp.substr(nEndPos + 2, strTemp.length() - nEndPos - 2); // 缩短strTemp
			
			nDestLenCur = strlen(szDest);
			if (nDestSize <= nDestLenCur + strSub.length() + strParam.length() - 2)
			{
				return;
			}
			sprintf(szDest + nDestLenCur, strSub.c_str(), strParam.c_str()); // 按格式存入
		}

		nDestLenCur = strlen(szDest);
		if (strTemp.empty() || nDestSize <= nDestLenCur + strTemp.length())
		{
			return;
		}

		// 如果为XXX%sYYY %s以前的字串前面已经处理过 剩余的字串还需要拼起来
		strncpy(szDest + nDestLenCur, strTemp.c_str(), strTemp.length());
	}

	bool init_unit::get_map_need_add_hate(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_king_of_pk:
		case e_map_type_cross_server_pk:
		case e_map_type_cross_ladder:
		case e_map_type_element_war:
		case e_map_type_attack_city:
		{
			return false;
		}
		default:
			return true;
		}
	}

	bool init_unit::is_rank_type_cross_server(int32 ranking_type)
	{
		switch (ranking_type)
		{
		case e_RankingIndex_gs_all:
		case e_RankingIndex_legion_gs_all:

		case e_RankingIndex_cross_boss_one:
		case e_RankingIndex_cross_boss_two:
		case e_RankingIndex_cross_boss_three:
		case e_RankingIndex_cross_boss_four:
		case e_RankingIndex_cross_boss_five:
		case e_RankingIndex_cross_boss_six:
		case e_RankingIndex_cross_boss_seven:
		case e_RankingIndex_cross_boss_eight:
		case e_RankingIndex_cross_boss_nine:
		case e_RankingIndex_cross_boss_ten:
		case e_RankingIndex_cross_boss_eleven:
		case e_RankingIndex_cross_boss_tweleve:
		case e_RankingIndex_time_limit_activity_cross_gs_up:
		case e_RankingIndex_time_limit_activity_cross_mount_up:
		case e_RankingIndex_time_limit_activity_cross_wing_up:
		case e_RankingIndex_time_limit_activity_cross_equip_up:
		case e_RankingIndex_time_limit_activity_cross_spirit_up:
		case e_RankingIndex_level_all:
		case e_RankingIndex_Oracle_Trial:
		case e_RankingIndex_cross_ladder_cur_season:
		case e_RankingIndex_cross_ladder_last_seaon:
		case e_RankingIndex_element_war_cur_season:
		case e_RankingIndex_element_war_last_season:
		case e_RankingIndex_cross_time_limit_activity_total_recharge:
			return true;
		default:
			return false;
		}
	}

	bool init_unit::is_levelup_add_hp(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_cross_server_pk:
		{
			return false;
		}
		break;
		default:
			return true;
		}
	}

	bool init_unit::is_send_clear_title_with_clear_big_player(int32 big_player)
	{
		switch (big_player)
		{
		case e_big_player_type_lord_or_war:
		case e_big_player_type_best_barserker:
		case e_big_player_type_best_wizard:
		case e_big_player_type_best_guardian:
		case e_big_player_type_best_assassinator:
			return true;
			break;
		default:
			return false;
		}
	}

	bool init_unit::is_cross_server_title(int32 title_type, bool is_begin_cross_activity)
	{
		switch (title_type)
		{
		case e_title_type_best_barserker:
		case e_title_type_best_wizard:
		case e_title_type_best_guardian:
		case e_title_type_best_assassinator:
			return true;
			break;
		default:
			return false;
		}
	}

	bool init_unit::is_cross_server_big_player(int32 player_type, bool is_begin_cross_activity, bool is_begin_cross_city_war)
	{
		if (true == is_begin_cross_activity)
		{
			if (player_type == e_big_player_type_lord_or_war)
			{
				return true;
			}
		}
		if (true == is_begin_cross_city_war)
		{
			if (player_type == e_big_player_type_lord_of_city
				|| player_type == e_big_player_type_overload_legion)
			{
				return true;
			}
		}
		switch (player_type)
		{
		case e_big_player_type_best_barserker:
		case e_big_player_type_best_wizard:
		case e_big_player_type_best_guardian:
		case e_big_player_type_best_assassinator:
			return true;
			break;
		default:
			return false;
		}
	}

	// 判断是否是公用地图,如果地图是公用地图,则进入场景无需额外条件,游戏内容结束时
	// 也不会弹出结算界面
	bool init_unit::get_map_public(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_big_map:
		case e_map_type_new:
		case e_map_type_main:
		case e_map_type_field:
		case e_map_type_crystak_dreamland:
		case e_map_type_boss_home:
		case e_map_type_boss_vip_home:
		case e_map_type_boss_island:
		case e_map_type_belief_cloister:
		case e_map_type_cross_server_pk:
		case e_map_type_cross_server_harry:
		case e_map_type_legion_station:
		case e_map_type_attack_city:
		case e_map_type_single_open_box:
		{
			return true;
		}
		default:
			return false;
		}
	}

	bool init_unit::is_all_server_npc(int32 npc_type)
	{
		if (npc_type == e_npc_type_strong_hold
			|| npc_type == e_npc_type_gather_crystal)
		{
			return true;
		}
		return false;
	}

	bool init_unit::is_cross_server_npc(int32 npc_type)
	{
		switch (npc_type)
		{
		case e_npc_type_island_big_chests:
		case e_npc_type_island_small_chests:
		case e_npc_type_cross_pk_chests:
		case e_npc_type_server_harry_chests:
		{
			return true;
		}
		default:
			return false;
		}
	}

	bool init_unit::get_map_level_suppress(int32 map_type, int32 map_order)
	{
		switch (map_type)
		{
		case e_map_type_big_map:
		case e_map_type_new:
		case e_map_type_main:
		case e_map_type_field:
		case e_map_type_crystak_dreamland:
		case e_map_type_boss_home:
		case e_map_type_boss_vip_home:
		case e_map_type_boss_island:
		case e_map_type_oracle_trial_map:
		{
			return true;
		}
		default:
			return false;
		}
	}

	bool init_unit::is_team_exp_map(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_field:
		case e_map_type_big_map:
		case e_map_type_fly_battle:
		case e_map_type_main:
		case e_map_type_new:
		case e_map_type_legion_boss:
		case e_map_type_broken_sky:
		case e_map_type_boss_home:
		case e_map_type_boss_vip_home:
		case e_map_type_boss_island:
		case e_map_type_belief_cloister:
		case e_map_type_princess_guard_war:
		case e_map_type_lava_hellfire:
		case e_map_type_sea_of_hate:
		case e_map_type_legion_station:
		{
			return true;
		}
		default:
			return false;
		}
	}

	//打宝多人地图才需要显示掉落归属
	bool init_unit::get_map_gain_treasure_multiplayer(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_boss_home:
		case e_map_type_boss_vip_home:
		case e_map_type_boss_island:
		case e_map_type_single_land_boss:
		{
			return true;
		}
		default:
			return false;
		}
	}
	
	// 判断是否需要创建新的地图,返回false表示不需要
	bool init_unit::get_map_open_new(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_big_map:
		case e_map_type_new:
		case e_map_type_main:
		case e_map_type_field:
		case e_map_type_broken_sky:
		case e_map_type_temple:
		case e_map_type_empire_treasure:
		case e_map_type_king_of_pk:
		case e_map_type_crystak_dreamland:
		case e_map_type_boss_vip_home:
		case e_map_type_boss_home:
		case e_map_type_boss_island:
		case e_map_type_belief_cloister:
		case e_map_type_cross_server_harry:
		case e_map_type_legion_station:
		{
			return false;
		}
		break;
		default:
			return true;
		}
	}
	
	// 当地图满人后是否创建新的地图
	bool init_unit::get_map_create_full(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_temple:
		case e_map_type_empire_treasure:
		case e_map_type_broken_sky:
		case e_map_type_belief_cloister:
		{
			return true;
		}
		break;
		default:
			return false;
		}
	}
	
	// 是否始终保持某个地图的引用,用来在地图中[没有角色时]回收地图的内存.
	// 比如:
	//     大世界类的地图[即使没人]也不应该清理掉它,所以大世界的地图会返回true
	bool init_unit::get_map_hold(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_big_map:
		case e_map_type_new:
		case e_map_type_main:
		case e_map_type_field:
		case e_map_type_crystak_dreamland:
		case e_map_type_legion_boss:
		case e_map_type_city_war:
		case e_map_type_king_of_pk:
		case e_map_type_legion_bonfire:
		case e_map_type_boss_island:
		case e_map_type_belief_cloister:
		case e_map_type_cross_server_pk:
		case e_map_type_broken_sky:
		case e_map_type_cross_server_harry:
		case e_map_type_legion_station:
		case e_map_type_element_war:
		case e_map_type_attack_city:
		{
			return true;
		}
		break;
		default:
			return false;
		}
	}

	bool init_unit::is_map_gain_treasure(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_boss_home:
		case e_map_type_boss_vip_home:
		{
			return true;
		}
		break;
		default:
			return false;
		}
	}

	bool init_unit::is_map_boss_island(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_boss_island:
		{
			return true;
		}
		break;
		default:
			return false;
		}
	}
	bool init_unit::is_can_change_pk_mode(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_city_war:
		case e_map_type_king_of_pk:
		case e_map_type_crystak_dreamland:
		case e_map_type_attack_city:
			return false;
		default:
			return true;
		}
	}
	bool init_unit::is_map_belief_cloister(int32 map_type)
	{
		switch (map_type)
		{
		case e_map_type_belief_cloister:
		{
			return true;
		}
		break;
		default:
			return false;
	}
	}

	bool init_unit::is_map_other_server(int32 map_temp_id)
	{
		MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, map_temp_id);
		if (nullptr == map_temp_ptr)
		{
			return false;
		}
		int32 map_type = map_temp_ptr->Type;
		switch (map_type)
		{
		case e_map_type_big_map:
		case e_map_type_cross_server_harry:
		case e_map_type_cross_ladder:
		case e_map_type_element_war:
		case e_map_type_city_war:
		case e_map_type_attack_city:
			return true;
		case e_map_type_cross_server_pk:
		if (map_temp_ptr->SubType == 4)
		{
			return true;
		}
		break;
		default:
			break;
		}
		return false;
	}

	bool init_unit::is_map_cross_server(int32 map_temp_id, bool is_cross_activity)
	{
		MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, map_temp_id);

		if (nullptr == map_temp_ptr)
		{
			return false;
		}
		int32 map_type = map_temp_ptr->Type;
		if (true == is_cross_activity)
		{
			if (map_type == e_map_type_king_of_pk
				|| map_type == e_map_type_crystak_dreamland)
			{
				return true;
			}
		}
		switch (map_type)
		{
		case e_map_type_boss_island:
		case e_map_type_belief_cloister:
		case e_map_type_cross_server_pk:
		case e_map_type_element_war:
		case e_map_type_attack_city:
		{
			return true;
		}
		break;
		default:
			return false;
		}
	}
	bool init_unit::is_map_all_server(int32 map_temp_id)
	{
		MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, map_temp_id);
		if (nullptr == map_temp_ptr)
		{
			return false;
		}
		int32 map_type = map_temp_ptr->Type;
		switch (map_type)
		{
		case e_map_type_city_war:
		case e_map_type_element_war:
		case e_map_type_attack_city:
			return true;
			break;
		}
		return false;
	}
	bool init_unit::is_tansfer_server(int32 map_temp_id, bool is_gate_server, bool is_cross_activity)
	{
		bool is_cross_server = is_map_cross_server(map_temp_id, is_cross_activity);
		return is_gate_server == is_cross_server;
	}
	bool init_unit::is_boss(int32 boss_type)
	{
		switch (boss_type)
		{
		case e_monster_type_boss:
		case e_monster_type_world_boss:
		case e_monster_type_gold_army:
		{
			return true;
		}
		break;
		default:
			break;
		}
		return false;
	}
	int32 init_unit::get_max_player_level()
	{
		int32 region_code = (int32)e_version_region_type_china_main;
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr != nullptr)
		{
			region_code = region_template_ptr->RegionCode;
		}
		std::vector<int32> player_level_max_arr = GAMECONFIG->NewGameLevelMax;
		if (player_level_max_arr.size() <= 0 || player_level_max_arr.size() % 2 != 0)
		{
			return 0;
		}
		for (int32 i = 0; i < player_level_max_arr.size() / 2; i++)
		{
			if (player_level_max_arr[i * 2] == region_code)
			{
				return player_level_max_arr[i * 2 + 1];
			}
		}
		return 0;
	}
	int32 init_unit::get_max_grade_level()
	{
		int32 region_code = (int32)e_version_region_type_china_main;
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr != nullptr)
		{
			region_code = region_template_ptr->RegionCode;
		}

		std::vector<int32> grade_level_max_arr = GAMECONFIG->NewGradeMaxLevel;
		if (grade_level_max_arr.size() <= 0 || grade_level_max_arr.size() % 2 != 0)
		{
			return 0;
		}
		for (int32 i = 0; i < grade_level_max_arr.size() / 2; i++)
		{
			if (grade_level_max_arr[i * 2] == region_code)
			{
				return grade_level_max_arr[i * 2 + 1];
			}
		}
		return 0;
	}

	bool init_unit::is_world_boss(int32 boss_type)
	{
		switch (boss_type)
		{
		case e_monster_type_world_boss:
		case e_monster_type_gold_army:
		{
			return true;
		}
		break;
		default:
			break;
		}
		return false;
	}
	bool init_unit::is_goto_same_map(int32 map_id_to, int32 line_id_to, int32 map_id_cur, int32 line_id_cur)
	{
		if (map_id_cur != map_id_to)
		{
			return false;
		}

		if (line_id_to != 0 && line_id_cur != line_id_to)
		{
			return false;
		}
		
		return true;
	}
	bool init_unit::is_can_same_map_transfer(int32 map_type)
	{
		if (map_type == e_map_type_ui
			||map_type == e_map_type_new
			||map_type == e_map_type_main
			||map_type == e_map_type_big_map
			//||map_type == e_map_type_field
			/*||map_type == e_map_type_boss_home
			||map_type == e_map_type_boss_vip_home
			||map_type == e_map_type_boss_island
			||map_type == e_map_type_belief_cloister*/)
		{
			return true;
		}
		return false;
	}
	bool init_unit::is_fake_player(int32 unit_type, int32 sub_type)
	{
		if ((unit_type == e_unit_type_npc && (sub_type == e_npc_type_lord_city || sub_type == e_npc_type_lord_pk || sub_type == e_npc_type_lord_sanctuary || sub_type == e_npc_type_lord_god || sub_type == e_npc_type_server_harry_chests))
			|| (unit_type == e_unit_type_monster && sub_type == e_monster_type_player_data))
		{
			return true;
		}
		return false;
	}
	e_role_equip_slot init_unit::get_slot_by_item_type(int32 item_type, int32 sub_type)
	{
		switch (item_type)
		{
		case e_item_type_weapon:
			return e_role_equip_slot_weapon_1;
			break;
		case e_item_type_armor:
			return (e_role_equip_slot)(sub_type + e_role_equip_slot_weapon_1);
			break;
		case e_item_type_mount:
			return e_role_equip_slot_mount;
			break;
		}
		return e_role_equip_slot_body;
	}

	bool init_unit::is_equip_type(int32 item_type)
	{
		switch (item_type)
		{
		case faith::e_item_type_weapon:
		case faith::e_item_type_armor:
		case faith::e_item_type_fashion:
		case faith::e_item_type_sky_equip: 
		case faith::e_item_type_skygod_equip:
		case faith::e_item_type_supreme_equip:
			return true;
		default:
			return false;
		}
	}

	bool init_unit::is_spirit_type(int32 item_type)
	{
		if (faith::e_item_type_spirit == item_type)
		{
			return true;
		}
		return false;
	}

	int64 init_unit::get_distance(const int64& x, const int64& y, const int64& z, const int64& x1, const int64& y1, const int64& z1)
	{
		return (x1 - x)*(x1 - x) + (y1 - y)*(y1 - y) + (z1 - z)*(z1 - z);
	}

	int64 init_unit::get_distance(const fvector& pos1, const fvector& pos2)
	{
		return get_distance(pos1.x, pos1.y, pos2.x, pos2.y);
	}

	int64 init_unit::get_distance(const int64& x, const int64& y, const int64& x1, const int64& y1)
	{
		return (x1 - x)*(x1 - x) + (y1 - y)*(y1 - y);
	}

	bool init_unit::is_in_distance_with_high(const fvector& pos1, const fvector& pos2, const int64& need_length, const float& ratio, const bool& only_compare_high)
	{
		if (!only_compare_high)
		{
			double z_diff = fabs(pos1.z - pos2.z);
			if (ratio * need_length < z_diff)
			{
				return false;
			}
			return is_in_distance(pos1, pos2, need_length);
		}
		else
		{
			double z_diff = fabs(pos1.z - pos2.z);
			if (ratio * need_length < z_diff)
			{
				return false;
			}
			return true;
		}
	}

	bool init_unit::is_in_distance(const fvector& pos1, const fvector& pos2, const int64& need_length)
	{
		double x_diff = fabs(pos1.x - pos2.x);
		double y_diff = fabs(pos1.y - pos2.y);
		if (need_length >= x_diff + y_diff)
		{
			return true;
		}
		return false;
	}
	bool init_unit::init_item_data(faith::s_item_info& item_data, int32 template_id, int32 slot, int32 stack_count, e_bag_type container_type, int32 is_activate, int32 locked, int32 item_over_time, int32 item_const_att)
	{
		if (template_id == 0)
		{
			return false;
		}

		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, template_id);

		if (NULL == item_template_ptr)
		{
			return false;
		}

		item_data.item_guid = guid_gen::make_guid();
		item_data.data_ary[e_item_info_info_id] = template_id;

		if (container_type == e_bag_type_bag 
			|| container_type == e_bag_type_wing 
			|| container_type == e_bag_type_mount 
			|| container_type == e_bag_type_protect
			|| container_type == e_bag_type_hope_item
			|| container_type == e_bag_type_treasure
			|| container_type == e_bag_type_elemenet
			|| container_type == e_bag_type_equip_elemenet
			|| container_type == e_bag_type_fasion
			|| container_type == e_bag_type_belief_rune
			|| container_type == e_bag_type_equip_spirit
			|| container_type == e_bag_type_belief_rune_equip)
		{
			item_data.data_ary[e_item_info_slot] = slot;
		}
		else if(container_type == e_bag_type_equip)
		{
			item_data.data_ary[e_item_info_slot] = get_slot_by_item_type(item_template_ptr->item_type, item_template_ptr->sub_type);
		}

		item_data.data_ary[e_item_info_container_type] = container_type;
		item_data.data_ary[e_item_info_stack_count] = stack_count;

		// 如果ItemTemplate模板中的locked字段是1，那么不论怎么样改物品都是绑定的(强制绑定)
		if (item_template_ptr->locked > 0)
		{
			item_data.data_ary[e_item_info_locked] = 1;
		}
		else if(locked > 0)
		{
			item_data.data_ary[e_item_info_locked] = 1;
		}
		else
		{
			item_data.data_ary[e_item_info_locked] = 0;
		}

		item_data.data_ary[e_item_info_activate] = is_activate;


		if (item_template_ptr->EffectiveTime > 0)
		{
			item_data.data_ary[e_item_info_over_time] = time_helper::get_cur_time_new().second + item_template_ptr->EffectiveTime;
		}

		switch ((e_item_type)(item_template_ptr->item_type))
		{
			// 如果是装备类的物品要看看它是否需要生成卓越属性
			case e_item_type_weapon:
			case e_item_type_armor:
			{
				EquipTemplate* equip_tmpl_ptr = GET_TEMPLATE(EquipTemplate, item_template_ptr->logic_id);
				if (nullptr == equip_tmpl_ptr)
				{
					return false;
				}
				make_new_excellent_att(equip_tmpl_ptr, item_data, item_const_att);
			}
			break;
			case e_item_type_mount:
			case e_item_type_wing:
				//坐骑、翅膀需要一上来设置不用幻化
				item_data.data_ary[e_item_info_showing_illusion_idex] = -1;				
				break;
			case e_item_type_spirit:
			{
				//精灵需要一上来设置不用幻化
				item_data.data_ary[e_item_info_showing_illusion_idex] = -1;
				
				//random_sprite_flair_property(item_data);
				SpiritTemplate* spirit_template_ptr = GET_TEMPLATE(SpiritTemplate, item_template_ptr->logic_id);
				if (nullptr == spirit_template_ptr)
				{
					return false;
				}
				make_new_spirit_eccellent_att(spirit_template_ptr, item_data, item_const_att);
			}
				break;
			default:
				break;
		}

		return true;
	}

	bool init_unit::init_spirit_data(faith::s_spirit_info& msg_data, int32 template_id, int32 slot, e_spirit_bag_type container_type, int32 is_activate)
	{
		if (template_id == 0)
		{
			return false;
		}

		SpiritTemplate* spirit_template_ptr = GET_TEMPLATE(SpiritTemplate, template_id);

		if (NULL == spirit_template_ptr)
		{
			return false;
		}
		msg_data.spirit_guid = guid_gen::make_guid();
		msg_data.data_ary[e_spirit_info_template_id] = template_id;
		msg_data.data_ary[e_spirit_info_spirit_slot] = slot;
		msg_data.data_ary[e_item_info_container_type] = container_type;
		msg_data.data_ary[e_spirit_info_spirit_is_equiped] = 0;
		msg_data.data_ary[e_spirit_info_spirit_is_fighting] = 0;
		if (is_activate > 0)
		{
			msg_data.data_ary[e_spirit_info_spirit_is_equiped] = 1;
			msg_data.data_ary[e_spirit_info_spirit_is_fighting] = 1;
		}
		msg_data.data_ary[e_spirit_info_spirit_typ] = spirit_template_ptr->Type;

		return true;
	}
	bool init_unit::init_skill_data(faith::s_skill_info& msg_data, int32 template_id, int32 activated)
	{
		if (template_id == 0)
		{
			return false;
		}

		SkillTemplate* template_data = GET_TEMPLATE(SkillTemplate, template_id);

		if (NULL == template_data)
		{
			return false;
		}
		msg_data.data_ary[e_skill_info_template_id] = template_id;
		msg_data.data_ary[e_skill_info_cd_cur_time] = 0;
		msg_data.data_ary[e_skill_info_cd_max_time] = template_data->SelfCd * 1000;
		msg_data.data_ary[e_skill_info_skill_type] = template_data->Type;
		msg_data.data_ary[e_skill_info_maturity] = 0;
		if (template_data->IsUseCreate > 0)
		{
			msg_data.data_ary[e_skill_info_use_pos] = template_data->Type;
		}
		else
		{
			msg_data.data_ary[e_skill_info_use_pos] = -1;
		}
		if (activated == 0)
		{
			if (template_data->LearnConditionLevel == 0)
			{//填0就是不能通过正常逻辑获得，一般都是玩家技能，需要通过任务解锁
				msg_data.data_ary[e_skill_info_activated] = 0;
			}
			else
			{
				msg_data.data_ary[e_skill_info_activated] = 1;
			}
		}
		else
		{
			msg_data.data_ary[e_skill_info_activated] = 1;
		}
		return true;
	}
	bool init_unit::is_in_safe(const std::vector<fvector>& PointAry, fvector unit_pos)
	{
		int32 count = PointAry.size();

		if (count < 3)
		{
			return false;
		}

		bool result = false;

		for (int32 i = 0, j = count - 1; i < count; i++)
		{
			const fvector& p1 = PointAry[i];
			const fvector& p2 = PointAry[j];

			if ((p1.y < unit_pos.y && p2.y >= unit_pos.y) || (p2.y < unit_pos.y && p1.y >= unit_pos.y))
			{
				if (FLOAT_IS_ZERO(p2.y - p1.y))
				{
					continue;
				}
				if (p1.x + (unit_pos.y - p1.y) / (p2.y - p1.y) * (p2.x - p1.x) < unit_pos.x)
				{
					result = !result;
				}
			}
			j = i;
		}
		return result;
	}

	int32 init_unit::calcu_fighting_power_by_att_array(const std::vector<float>& att_array,const double* unit_att, const f32& att_ratio,bool AddPercentAtt)
	{
		if (nullptr == unit_att)
		{
			return 0;
		}
		int32 len = att_array.size();
		if (len <= 0 || len % e_att_one_max != 0)
		{
			return 0;
		}
		int32 fighting_power = 0;
		for (int32 i = 0; i < len; i += e_att_one_max)
		{
			int32 temp_value = att_array[i + e_att_one_att_id];
			int32 att_id = temp_value + attribute_template_id_start;
			float att_value = att_array[i + e_att_one_value] * att_ratio;
			float att_percent = att_array[i + e_att_one_percent] * att_ratio;

			if (FLOAT_IS_ZERO(att_value) && FLOAT_IS_ZERO(att_percent))
			{
				continue;
			}
			AttributeTemplate* att_template_ptr = nullptr;
			att_template_ptr = GET_TEMPLATE(AttributeTemplate, att_id);
			
			if (nullptr == att_template_ptr)
			{
				continue;
			}
			// 计算该属性固定数值增值提供的战斗力
			fighting_power += (att_template_ptr->AttGs * att_value);

			// 计算该属性百分比增值提供的战斗力
			if (AddPercentAtt)
			{
				f32 this_att_now_final_val = 0;
				if (temp_value > e_unit_attack_att_none && temp_value < e_unit_attack_att_max)
				{
					this_att_now_final_val = unit_att[temp_value];
				}

				fighting_power += (this_att_now_final_val * (att_percent / 100) * att_template_ptr->AttGs);
			}
			
		}

		return fighting_power;
	}

	fvector init_unit::in_skill_move(fvector boss_location, fvector target_location, int32 move_length)
	{
		fvector move_location = boss_location - target_location;
		move_location.Z = 0;
		if (move_location.x == 0)
		{
			if (move_location.y > 0)
			{
				boss_location.y -= move_length;
			}
			else
			{
				boss_location.y += move_length;
			}
			return boss_location;
		}
		if (move_location.y == 0)
		{
			if (move_location.x > 0)
			{
				boss_location.x -= move_length;
			}
			else
			{
				boss_location.x += move_length;
			}
			return boss_location;
		}
		if (fabs(move_location.Y) > fabs(move_location.X))
		{//注意调换位置，先除没改变的数值
			if (FLOAT_IS_ZERO(move_location.Y))
			{
				if (move_location.x > 0)
				{
					boss_location.x -= move_length;
				}
				else
				{
					boss_location.x += move_length;
				}
				return boss_location;
			}
			move_location.X = -(move_location.X / fabs(move_location.Y));
			move_location.Y = -(move_location.Y / fabs(move_location.Y));
		}
		else
		{
			if (FLOAT_IS_ZERO(move_location.X))
			{
				if (move_location.y > 0)
				{
					boss_location.y -= move_length;
				}
				else
				{
					boss_location.y += move_length;
				}
				return boss_location;
			}
			move_location.Y = -(move_location.Y / fabs(move_location.X));
			move_location.X = -(move_location.X / fabs(move_location.X));
		}
		move_location *= move_length;
		boss_location += move_location;
		return boss_location;
	}
	bool init_unit::in_skill_target(fvector boss_location, fvector target_location, int32 skill_wight, int32 skill_length, fvector unit_location)
	{
		if (boss_location.x == target_location.x)
		{
			return in_skill_target_x(boss_location, target_location, skill_wight, skill_length, unit_location);
		}
		else if (boss_location.y == target_location.y)
		{
			return in_skill_target_y(boss_location, target_location, skill_wight, skill_length, unit_location);
		}

		/*两点间直线公式(x1,y1)(x2,y2)
		(y-y1)/(y2 - y1) = (x-x1)/(x2-x1)
		(y-y1) * (x2-x1) = (x-x1) * (y2 - y1)
		y*(x2-x1) - y1 * (x2-x1) = x*(y2 - y1) - x1 * (y2 - y1)
		x*(y2 - y1) - x1 * (y2 - y1) - y*(x2-x1) + y1 * (x2-x1) =0
		x*(y2 - y1)- y*(x2-x1) + y1 * (x2-x1) - x1 * (y2 - y1) =0
		ax + by + c =0;
		a = y2-y1
		b = x2 -x1
		c = y1 * (x2-x1) - x1 * (y2 - y1) 
		k = (y2-y1)/(x2-x1)
		*/
		/*点到直线的距离
			d = abs( (a * x0 + b*y0 + c)/ sqrt(a * a + b * b) )
		*/
		float a = target_location.y - boss_location.y;
		float b = -(target_location.x - boss_location.x);
		float c = boss_location.y *  (target_location.x - boss_location.x) - boss_location.x *  (target_location.y - boss_location.y);
		if (FLOAT_IS_ZERO(-b))
		{
			return false;
		}
		float k = a / (-b);
		float  square_ab = sqrt(a * a + b * b);
		if (FLOAT_IS_ZERO(square_ab))
		{
			return false;
		}
		float d = fabs((a * unit_location.x + b * unit_location.y + c) / square_ab);
		if (d > skill_wight)
		{
			return false;
		}

		/*过(x0,y0), 斜率为k 的直线为 y-y0=k(x-x0)
		y-y0 = k * x - k * x0
		kx - y - k * x0 + y0 = 0
		*/
		/*垂直于一条直线的斜率为 -1/k
		*/
		if (FLOAT_IS_ZERO(k))
		{
			return false;
		}
		float ac = -1 / k;
		float bc = -1;
		float cc = -(-1 / k) * boss_location.x + boss_location.y;
		float  square_abc = sqrt(ac*ac + bc*bc);
		if (FLOAT_IS_ZERO(square_abc))
		{
			return false;
		}
		float dc = (ac * unit_location.x + bc * unit_location.y + cc) / square_abc;
		if (boss_location.y > target_location.y)
		{
			if (dc <= skill_length && dc >= 0)
			{
				return true;
			}
		}
		else
		{
			if (-dc <= skill_length && -dc >= 0)
			{
				return true;
			}
		}
		return false;
	}
	bool init_unit::in_skill_target_x(fvector boss_location, fvector target_location, int32 skill_wight, int32 skill_length, fvector unit_location)
	{
		if (boss_location.x != target_location.x)
		{
			return false;
		}
		if (fabs(unit_location.x - boss_location.x) > skill_wight)
		{
			return false;
		}
		float length = unit_location.y - boss_location.y;
		if (target_location.y > boss_location.y)
		{
			if (length <= skill_length && length >= 0)
			{
				return true;
			}
		}
		else
		{
			if (-length <= skill_length && -length >= 0)
			{
				return true;
			}
		}
		return false;
	}
	bool init_unit::in_skill_target_y(fvector boss_location, fvector target_location, int32 skill_wight, int32 skill_length, fvector unit_location)
	{
		if (boss_location.y != target_location.y)
		{
			return false;
		}
		if (fabs(unit_location.y - boss_location.y) > skill_wight)
		{
			return false;
		}
		float length = unit_location.x - boss_location.x;
		if (target_location.x > boss_location.x)
		{
			if (length <= skill_length && length >= 0)
			{
				return true;
			}
		}
		else
		{
			if (-length <= skill_length && -length >= 0)
			{
				return true;
			}
		}
		return false;
	}
	/*此接口在拍卖中重写,修改此处 在LUA中搜索init_mail_data_array*/
	bool init_unit::init_mail_data_array(faith::s_mail_info& msg_data, const std::vector<guid_64>& item_guid, int32 money_typ1, int32 money_num1, int32 money_typ2, int32 money_num2, guid_64 sender_guid, int64 cur_time_sec)
	{
		msg_data.mail_guid = guid_gen::make_guid(sender_guid);

		for (int32 i = 0; i < item_guid.size(); ++i)
		{
			if (i >= max_item_per_mail)
			{
				break;
			}
			msg_data.data_ary[i*2+ EMailInfo_ItemGuid11] = item_guid[i].A;
			msg_data.data_ary[i*2 + EMailInfo_ItemGuid12] = item_guid[i].B;
		}
		msg_data.data_ary[EMailInfo_MoneyTyp1] = money_typ1;
		msg_data.data_ary[EMailInfo_MoneyNum1] = money_num1;
		msg_data.data_ary[EMailInfo_MoneyTyp2] = money_typ2;
		msg_data.data_ary[EMailInfo_MoneyNum2] = money_num2;

		msg_data.data_ary[EMailInfo_ReadState] = 0;

		msg_data.data_ary[EMailInfo_SenderGuid1] = sender_guid.A;
		msg_data.data_ary[EMailInfo_SenderGuid2] = sender_guid.B;
		msg_data.data_ary[EMailInfo_SendTime] = cur_time_sec;

		return false;
	}
	void init_unit::range_pos(s_map_pos& pos)
	{
		float drop_range = 200;//默认保护值

		if (GAMECONFIG->DropRange > 0.0f)
		{
			drop_range = GAMECONFIG->DropRange;
		}

		int32 rand_range_x = random_gen::get_random(0, static_cast<int32> (drop_range));
		int32 rand_factor_x = random_gen::get_random(-100, 100);
		if (rand_factor_x >= 0)
			rand_factor_x = 1;
		else
			rand_factor_x = -1;

		rand_range_x = rand_range_x*rand_factor_x;

		int32 rand_range_y = random_gen::get_random(0, static_cast<int32> (drop_range));
		int32 rand_factor_y = random_gen::get_random(-100, 100);
		if (rand_factor_y >= 0)
			rand_factor_y = 1;
		else
			rand_factor_y = -1;

		rand_range_y = rand_range_y*rand_factor_y;

		pos.unit_location.x += rand_range_x;
		pos.unit_location.y += rand_range_y;
	}
	
	e_map_category init_unit::get_map_category(int32 map_type)
	{
		switch ((e_map_type)map_type)
		{
		case e_map_type_battle:
		case e_map_type_fly_battle:
		case e_map_type_exp_fuben:
		case e_map_type_money_fuben:
		case e_map_type_demon_tower_fuben:
		case e_map_type_princess_guard_war:
		case e_map_type_lava_hellfire:
		case e_map_type_sea_of_hate:
		case e_map_type_abyss_of_fear:
		case e_map_type_fallen_fantasy:
		case e_map_type_star_test:
		case e_map_type_oracle_trial_map:
		case e_map_type_single_land_boss:
		case e_map_type_single_assist_boss:
			return e_map_category_raid;

		case e_map_type_legion_boss:
		case e_map_type_city_war:
		case e_map_type_broken_sky:
		case e_map_type_king_of_pk:
		case e_map_type_temple:
		case e_map_type_empire_treasure:
		case e_map_type_crystak_dreamland:
		case e_map_type_legion_station:
			return e_map_category_activity;

		default:
			return e_map_category_common;
		}
	}

	bool init_unit::is_team_type_raid_map(int32 team_type)
	{
		switch (team_type)
		{
		//case e_team_type_act_broken_sky:
		case e_team_type_raid_daily_exp:
		case e_team_type_raid_daily_gold:
		case e_team_type_raid_multi_0:
		case e_team_type_raid_multi_1:
		case e_team_type_raid_multi_2:
		case e_team_type_raid_sky:
		case e_team_type_raid_temple:
		case e_team_type_raid_treasure:
		case e_team_type_ancient_hang_up:
			return true;
		}
		return false;
	}

	bool init_unit::is_team_type_raid_for_team(int32 team_type)
	{
		switch (team_type)
		{
		case e_team_type_raid_multi_0:
		case e_team_type_raid_multi_1:
		case e_team_type_raid_multi_2:
		case e_team_type_raid_sky:
		case e_team_type_raid_temple:
		case e_team_type_raid_treasure:
			return true;
		}
		return false;
	}

	bool init_unit::is_team_type_one_stop(int32 team_type)
	{
		switch (team_type)
		{
		case e_team_type_one_dragon:
		case e_team_type_raid_daily_exp:
		case e_team_type_raid_daily_gold:
		case e_team_type_raid_multi_0:
		case e_team_type_raid_multi_1:
		case e_team_type_raid_multi_2:
		case e_team_type_raid_sky:
		case e_team_type_raid_temple:
		case e_team_type_raid_treasure:
		case e_team_type_mission_daily:
		case e_team_type_mission_against:
			return true;
		}
		return false;
	}

	int32 init_unit::get_player_grade_num(int32 level)
	{
		if (level <= 0)
		{
			return -1;
		}

		// 注意: 边界情况类似100级的时候,要显示0转而不是1转
		return (level - 1) / player_grade_up_level_interval;
	}

	int32 init_unit::get_player_level_of_cur_grade(int32 level)
	{
		if (level <= 0)
		{
			return -1;
		}
		
		// 注意: 边界情况类似100级的时候,要显示100级而不是0级(求摩后)
		int32 ret = level % player_grade_up_level_interval;
		return ret != 0 ? ret : player_grade_up_level_interval;
	}
	e_bag_type init_unit::get_item_bag_type(int32 item_template_id)
	{
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_template_id);

		if (nullptr == item_template_ptr)
		{
			return e_bag_type_none;
		}
		switch (item_template_ptr->item_type)
		{
			case e_item_type_weapon:
			case e_item_type_armor:
			{
				return e_bag_type_storage;
			}
			break;
			case e_item_type_expendable:
			case e_item_type_stone:
			case e_item_type_mission:
			case e_item_type_spirit:
			case e_item_type_beast_spirit:
			case e_item_type_sky_equip:
			case e_item_type_skygod_equip:
			case e_item_type_goddess_equip:
			case e_item_type_mount_equip:
			case e_item_type_supreme_equip:
			case e_item_type_skill_book:
			{
				return e_bag_type_bag;
			}
			break;
			case e_item_type_element_heart:
			{
				return e_bag_type_elemenet;
			}
			break;
			case e_item_type_mount:
			{
				return e_bag_type_mount;
			}
			break;
			case e_item_type_fashion:
			{
				return e_bag_type_fasion;
			}
			break;
			case e_item_type_wing:
			{
				return e_bag_type_wing;
			}
			break;
			case e_item_type_protect:
			{
				return e_bag_type_protect;
			}
			break;
			case e_item_type_belief_rune:
			{
				return e_bag_type_belief_rune;
			}
			break;
			case e_item_type_time_limit:
			{
				return get_item_bag_type(item_template_ptr->logic_id);
			}
			break;
			case e_item_type_star_map_spirit:
			{
				return e_bag_type_spirit_upgrade_promote;
			}
			break;
			case e_item_type_break_will:
			{
				return e_bag_type_break_will;
			}
			break;
			case e_item_type_awaken_item:
			{
				return e_bag_type_awaken_item;
			}
			break;
		default:
			CONSOLE_ERROR("get_item_bag_type error item_id:{} item_type:{}", item_template_id, item_template_ptr->item_type);
			return e_bag_type_none;
			break;
		}
	}
	void init_unit::change_account_to_low(xchar* account)
	{
		return;
	/*	if (nullptr == account)
		{
			return;
		}
		for (int32 i = 0; i < max_account_length; ++i)
		{
			if (account[i] >= 65 && account[i] <= 90)
			{
				account[i] += 32;
			}
		}*/
	}
	bool init_unit::account_is_right(const std::string& account)
	{
		if (account.empty())
		{
			return false;
		}
		for (int32 i = 0; i < account.size(); ++i)
		{
			if (account[i] > 0)
			{
				if ((account[i] >= 48 && account[i] <= 57) || (account[i] >= 65 && account[i] <= 90) || (account[i] >= 97 && account[i] <= 122) || account[i] == '-')
				{
					;
				}
				else
				{
					return false;
				}
			}
			else if (account[i] < 0)
			{
				return false;
			}
			else
			{
				break;
			}
		}
		return true;
	}
	int32 init_unit::change_string_to_i32(std::string str_i32)
	{
		int32 res_value = 0;
		std::stringstream ss;
		ss << str_i32;
		ss >> res_value;
		return res_value;
	}
	std::string init_unit::change_i32_to_string(int32 value_i32)
	{
		std::string res_value = "";
		std::stringstream ss;
		ss << value_i32;
		ss >> res_value;
		return res_value;
	}
	int64 init_unit::change_string_to_i64(std::string str_i64)
	{
		int64 res_value = 0;
		std::stringstream ss;
		ss << str_i64;
		ss >> res_value;
		return res_value;
	}
	std::string init_unit::change_i64_to_string(int64 value_i64)
	{
		std::string res_value = "";
		std::stringstream ss;
		ss << value_i64;
		ss >> res_value;
		return res_value;
	}

	float init_unit::change_string_to_float(std::string str_i32)
	{
		float res_value = 0.0;
		std::stringstream ss;
		ss << str_i32;
		ss >> res_value;
		return res_value;
	}

	std::string init_unit::change_float_to_string(f32 value_f32)
	{
		std::string res_value = "";
		std::stringstream ss;
		ss << value_f32;
		ss >> res_value;
		return res_value;
	}

	bool init_unit::judge_func_unlock(int32 player_cond_id, int32 func_type)
	{
		e_unlock_func_type temp_func_type = (e_unlock_func_type)func_type;
		if (temp_func_type >= e_unlock_func_type_max)
		{
			return false;
		}

		switch (temp_func_type)
		{
			case e_unlock_func_type_skill_0:
				if (player_cond_id < 100000100) return false;
				break;
			case e_unlock_func_type_skill_1:
				if (player_cond_id < 100000110) return false;
				break;
			case e_unlock_func_type_skill_2:
				if (player_cond_id < 100000120) return false;
				break;
			case e_unlock_func_type_skill_3:
				if (player_cond_id < 100000140) return false;
				break;
			case e_unlock_func_type_skill_4:
				if (player_cond_id < 100000170) return false;
				break;
			case e_unlock_func_type_skill_5:
				if (player_cond_id < 100000190) return false;
				break;
			case e_unlock_func_type_auto_battle:
				if (player_cond_id < 100000144) return false;
				break;
			case e_unlock_func_type_mount:
				if (player_cond_id < 100000130) return false;
				break;
			case e_unlock_func_type_equip_upgrade:
				if (player_cond_id < 100000180) return false;
				break;
			case e_unlock_func_type_equip_addon:
				if (player_cond_id < 100000201) return false;
				break;
			case e_unlock_func_type_wing:
				if (player_cond_id < 100000240) return false;
				break;
			case e_unlock_func_type_activity:
				if (player_cond_id < 100000195) return false;
				break;
			case e_unlock_func_type_convert:
				if (player_cond_id < 100000280) return false;
				break;
			case e_unlock_func_type_ranking:
				if (player_cond_id < 100000210) return false;
				break;
			case e_unlock_func_type_achievement:
				return true;
			case e_unlock_func_type_raid_daily_exp:
				if (player_cond_id < 100000270) return false;
				break;
			case e_unlock_func_type_raid_daily_money:
				if (player_cond_id < 100000260) return false;
				break;
			case e_unlock_func_type_raid_demons_tower:
				if (player_cond_id < 100000250) return false;
				break;

			case e_unlock_func_type_raid_multiplayer:
			case e_unlock_func_type_raid_sky_battle:
			case e_unlock_func_type_raid_drama:
				if (player_cond_id < 100000150) return false;
				break;
		}

		return true;
	}

	bool init_unit::is_map_can_change_pk_mode(int32 map_type)
	{
		switch (map_type)
		{//除了大世界都不能变
		case e_map_type_battle:
		case e_map_type_field:
		case e_map_type_fly_battle:
		case e_map_type_exp_fuben:
		case e_map_type_money_fuben:
		case e_map_type_demon_tower_fuben:
		case e_map_type_legion_boss:
		case e_map_type_city_war:
		case e_map_type_broken_sky:
		case e_map_type_king_of_pk:
		case e_map_type_arena:
		case e_map_type_temple:
		case e_map_type_empire_treasure:
		case e_map_type_princess_guard_war:
		case e_map_type_lava_hellfire:
		case e_map_type_sea_of_hate:
		case e_map_type_abyss_of_fear:
		case e_map_type_fallen_fantasy:
		case e_map_type_star_test:
		case e_map_type_cross_server_pk:
		case e_map_type_oracle_trial_map:
		case e_map_type_legion_station:
		case e_map_type_single_land_boss:
		case e_map_type_single_assist_boss:
		case e_map_type_attack_city:
			return false;
		default:
			return true;
		}
	}
	int64 init_unit::get_end_time(const float& add_time, const int64& now_time)
	{
		int64 temp_now_time = now_time;
		if (temp_now_time < 0)
		{
			temp_now_time = utility::get_tick_count();
		}
		int32 mill_sec = add_time * second_tick_time;
		int64 end_time = mill_sec + temp_now_time;
		return end_time;
	}

	int64 init_unit::get_end_time(const int32& add_time)
	{
		int32 mill_sec = add_time * second_tick_time;
		int64 end_time= mill_sec + utility::get_tick_count();
		return end_time;
	}

	int32 init_unit::get_map_id_by_diff_level(int32 map_id, int32 map_type, int32 map_group_type)
	{
		if (map_group_type > 0)
		{
			return map_group_type;
		}
		return map_id;
	}

	bool init_unit::can_career_use_item(int32 role_class, const std::vector<int32>& class_arr)
	{
		if (role_class <= 0)
		{
			return false;
		}
		
		int32 len = class_arr.size();
		if (len <= 0)
		{
			return true;
		}
		for (int32 i = 0; i < len; ++i)
		{
			if (class_arr[i] == e_class_type_none
				|| role_class == class_arr[i])
			{
				return true;
			}
		}
		return false;
	}

	void init_unit::get_money_array_by_mission_id(int32 mission_id, int32 player_upgrade_id, int32 star_num, int32 get_times, std::vector<s_item_template_info>& money_array,int32 world_base_exp)
	{
		MissionTemplate* mission_template_ptr = GET_TEMPLATE(MissionTemplate, mission_id);
		PlayerUpgradeTemplate* player_upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, player_upgrade_id);
		if (mission_template_ptr == nullptr || player_upgrade_template_ptr == nullptr)
		{
			return;
		}

		switch (mission_template_ptr->FinishAwardType)
		{
		case e_mission_finish_award_type_none:
			break;
		case faith::e_mission_finish_award_type_normal:
		{
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_exp, mission_template_ptr->FinishEXP * get_times);
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_silver_bind, mission_template_ptr->FinishGold * get_times);
		}
		break;
		case faith::e_mission_finish_award_type_rate:
		{
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_exp, mission_template_ptr->FinishEXP * player_upgrade_template_ptr->BaseExp * get_times);
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_silver_bind, mission_template_ptr->FinishGold * player_upgrade_template_ptr->BaseCoin * get_times);
		}
		break;
		case faith::e_mission_finish_award_type_world:
		{
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_exp, mission_template_ptr->FinishEXP * world_base_exp * get_times);
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_silver_bind, mission_template_ptr->FinishGold * player_upgrade_template_ptr->BaseCoin * get_times);
		}
		break;
		default:
			break;
		}

		if (mission_template_ptr->FinishMoney.size() > 0
			&& mission_template_ptr->FinishMoney.size() % 2 == 0)
		{
			int32 other_money_num = mission_template_ptr->FinishMoney.size() / 2;
			for (int32 i = 0; i < other_money_num; ++i)
			{
				int32 temp_money_type = mission_template_ptr->FinishMoney[i * 2];
				int64 temp_money_value = mission_template_ptr->FinishMoney[i * 2 + 1];
				temp_money_value *= get_times;
				init_unit::merge_money_to_two_tuples(money_array, temp_money_type, temp_money_value);
			}
		}

		if (mission_template_ptr->MissionShowType == e_mission_type_daily)
		{
			int32 reward_coefficient = 1;
			if (star_num < GAMECONFIG->DailyMissionStarReward.size())
			{
				reward_coefficient = GAMECONFIG->DailyMissionStarReward[star_num];
			}
			for (auto& it : money_array)
			{
				it.m_item_num *= reward_coefficient;
			}
		}
	}

	void init_unit::get_money_array_by_mission_library_id(int32 mission_library_id, int32 player_upgrade_id, std::vector<s_item_template_info>& money_array, int32 world_base_exp)
	{
		MissionLibraryTemplate* mission_library_template_ptr = GET_TEMPLATE(MissionLibraryTemplate, mission_library_id);
		PlayerUpgradeTemplate* player_upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, player_upgrade_id);
		if (mission_library_template_ptr == nullptr
			|| player_upgrade_template_ptr == nullptr
			|| mission_library_template_ptr->AllDoneMoney.size() % 2)
		{
			return;
		}

		switch (mission_library_template_ptr->FinishAwardType)
		{
		case faith::e_mission_finish_award_type_none:
			break;
		case faith::e_mission_finish_award_type_normal:
		{
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_exp, mission_library_template_ptr->FinishEXP);
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_silver_bind, mission_library_template_ptr->FinishGold);
		}
		break;
		case faith::e_mission_finish_award_type_rate:
		{
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_exp, mission_library_template_ptr->FinishEXP * player_upgrade_template_ptr->BaseExp);
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_silver_bind, mission_library_template_ptr->FinishGold * player_upgrade_template_ptr->BaseCoin);
		}
		case faith::e_mission_finish_award_type_world:
		{
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_exp, mission_library_template_ptr->FinishEXP * world_base_exp);
			init_unit::merge_money_to_two_tuples(money_array, e_money_type_silver_bind, mission_library_template_ptr->FinishGold * player_upgrade_template_ptr->BaseCoin);

		}
		break;
		default:
			break;
		}

		if (mission_library_template_ptr->AllDoneMoney.size() > 0
			&& mission_library_template_ptr->AllDoneMoney.size() % 2 == 0)
		{
			for (int32 i = 0; i < mission_library_template_ptr->AllDoneMoney.size(); i += 2)
			{
				int32 money_id = mission_library_template_ptr->AllDoneMoney[i];
				int32 money_value = mission_library_template_ptr->AllDoneMoney[i + 1];
				init_unit::merge_money_to_two_tuples(money_array, money_id, money_value);
			}
		}
	}

	void init_unit::merge_item(std::vector<s_item_template_info>& source_array, int32 new_id, int32 new_num, bool new_lock_stated)
	{
		if (source_array.size() == 0)
		{
			source_array.push_back({ new_id, new_num, new_lock_stated });
			return;
		}
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, new_id);
		if (nullptr == item_template_ptr)
		{
			return;
		}

		{
			//这里为空的时候也是有意义的，说明是钱，这个括号是为了保证这个指针不会在之后的逻辑中被用到

			for (int32 i = 0; i < source_array.size(); i ++)
			{
				int32 source_id = source_array[i].m_item_id;
				int32 source_num = source_array[i].m_item_num;
				int32 source_lock_stated = source_array[i].m_lock;

				if (source_id != new_id || source_lock_stated != new_lock_stated)
				{
					continue;
				}
				int32 max_pile_num = item_template_ptr->max_pile_num;
				if (max_pile_num <= 0)
				{
					max_pile_num = 1;
				}

				if (source_num + new_num <= max_pile_num)
				{
					source_array[i].m_item_num = source_num + new_num;
					return;
				}
				else
				{
					int32 can_pile_num = max_pile_num - source_array[i].m_item_num;
					source_array[i].m_item_num = max_pile_num;
					new_num -= can_pile_num;
				}
			}
		}
		if (new_num <= 0)
		{
			return;
		}
		source_array.push_back({ new_id, new_num, new_lock_stated });
	}

	void init_unit::merge_money_to_two_tuples(std::vector<s_item_template_info>& source_array, int32 new_id, int32 new_num)
	{
		if (source_array.size() == 0)
		{
			source_array.push_back({ new_id , new_num });
			return;
		}
		for (auto& it : source_array)
		{
			int32 source_id = it.m_item_id;
			int32 source_num = it.m_item_num;

			if (source_id != new_id)
			{
				continue;
			}
			it.m_item_num += new_num;
			new_num = 0;
		}
		if (new_num <= 0)
		{
			return;
		}
		source_array.push_back({ new_id , new_num });
	}
	int32 init_unit::get_ladder_season()
	{
		int32 cur_season = 0;
		int64 temp_now_time = utility::get_tick_count() / second_tick_time;
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (temp_now_time <= 0 || nullptr == config_ptr || nullptr == region_template_ptr)
		{
			return cur_season;
		}
		int32 region_code = region_template_ptr->RegionCode;
		int64 last_time = 0;
		int32 last_season = 0;
		for (auto iter = config_ptr->ParamStringArr2.begin(); iter != config_ptr->ParamStringArr2.end(); ++iter)
		{
			int32 cur_code = -1;
			int32 season = 0;
			int64 beginTime = 0;
			int64 endTime = 0;
			sscanf_s((*iter).c_str(), "%d-%d-%I64d-%I64d", &cur_code, &season, &beginTime, &endTime);
			if (cur_code != region_code)
			{
				continue;
			}
			tm base_time;
			base_time.tm_year = beginTime / 10000000000 - 1900;
			base_time.tm_mon = beginTime / 100000000 % 100 - 1;
			base_time.tm_mday = beginTime / 1000000 % 100;
			base_time.tm_hour = beginTime / 10000 % 100;
			base_time.tm_min = beginTime / 100 % 100;
			base_time.tm_sec = beginTime % 100;
			beginTime = mktime(&base_time);
			base_time.tm_year = endTime / 10000000000 - 1900;
			base_time.tm_mon = endTime / 100000000 % 100 - 1;
			base_time.tm_mday = endTime / 1000000 % 100;
			base_time.tm_hour = endTime / 10000 % 100;
			base_time.tm_min = endTime / 100 % 100;
			base_time.tm_sec = endTime % 100;
			endTime = mktime(&base_time);
			if (temp_now_time >= beginTime && temp_now_time <= endTime)
			{
				cur_season = season;
				break;
			}
			if (temp_now_time > endTime)
			{
				//获取距离当前时间最近结束的赛季
				int32 time_gap = temp_now_time - endTime;
				if (last_time == 0)
				{
					last_season = season;
					last_time = time_gap;
				}
				else 
				{
					last_time = last_time <= time_gap ? last_time : time_gap;
					last_season = season;
				}
			}
		}

		//如果不在赛季时间内获取上个赛季
		if (cur_season == 0 && last_season > 0)
		{
			cur_season = last_season;
		}
		return cur_season;
	}
	int32 init_unit::get_element_season()
	{
		int32 cur_season = 0;
		int64 temp_now_time = utility::get_tick_count() / second_tick_time;
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_element_war);
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (temp_now_time <= 0 || nullptr == config_ptr || nullptr == region_template_ptr)
		{
			return cur_season;
		}
		int32 region_code = region_template_ptr->RegionCode;
		int64 last_time = 0;
		int32 last_season = 0;
		for (auto iter = config_ptr->ParamStringArr1.begin(); iter != config_ptr->ParamStringArr1.end(); ++iter)
		{
			int32 cur_code = -1;
			int32 season = 0;
			int64 beginTime = 0;
			int64 endTime = 0;
			sscanf_s((*iter).c_str(), "%d-%d-%I64d-%I64d", &cur_code, &season, &beginTime, &endTime);
			if (cur_code != region_code)
			{
				continue;
			}
			tm base_time;
			base_time.tm_year = beginTime / 10000000000 - 1900;
			base_time.tm_mon = beginTime / 100000000 % 100 - 1;
			base_time.tm_mday = beginTime / 1000000 % 100;
			base_time.tm_hour = beginTime / 10000 % 100;
			base_time.tm_min = beginTime / 100 % 100;
			base_time.tm_sec = beginTime % 100;
			beginTime = mktime(&base_time);
			base_time.tm_year = endTime / 10000000000 - 1900;
			base_time.tm_mon = endTime / 100000000 % 100 - 1;
			base_time.tm_mday = endTime / 1000000 % 100;
			base_time.tm_hour = endTime / 10000 % 100;
			base_time.tm_min = endTime / 100 % 100;
			base_time.tm_sec = endTime % 100;
			endTime = mktime(&base_time);
			if (temp_now_time >= beginTime && temp_now_time <= endTime)
			{
				cur_season = season;
				break;
			}
			if (temp_now_time > endTime)
			{
				//获取距离当前时间最近结束的赛季
				int32 time_gap = temp_now_time - endTime;
				if (last_time == 0)
				{
					last_season = season;
					last_time = time_gap;
				}
				else
				{
					last_time = last_time <= time_gap ? last_time : time_gap;
					last_season = season;
				}
			}
		}

		//如果不在赛季时间内获取上个赛季
		if (cur_season == 0 && last_season > 0)
		{
			cur_season = last_season;
		}
		return cur_season;
	}
	bool init_unit::is_all_time_map(int32 map_temp_id)
	{

		MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, map_temp_id);

		if (nullptr == map_temp_ptr)
		{
			return false;
		}
		int32 map_type = map_temp_ptr->Type;
		switch (map_type)
		{
		case e_map_type_appearance:
		case e_map_type_single_land_boss:
		case e_map_type_single_assist_boss:
			return true;
			break;
		}
		return false;
	}
	void init_unit::get_item_recovery_money_info(const faith::s_item_info& item_inst_data, std::vector<s_item_template_info>& money_reward_array, std::vector<s_item_template_info>& money_cost_array, bool is_clear_array, bool is_sprite_use_diamond)
	{
		if (is_clear_array)
		{
			money_reward_array.clear();
			money_cost_array.clear();
		}
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_inst_data.data_ary[e_item_info_info_id]);

		if (item_template_ptr == nullptr)
		{
			return;
		}
		int32 item_num = item_inst_data.data_ary[faith::e_item_info_stack_count];

		if (item_num <= 0)
		{
			item_num = 1;
		}

		if (is_equip_type(item_template_ptr->item_type))
		{
			EquipTemplate* equip_template_ptr = GET_TEMPLATE(EquipTemplate, item_template_ptr->logic_id);

			if (equip_template_ptr == nullptr)
			{
				return;
			}
			int32 recovery_array_size = 0;
			//recovery_array_size = equip_template_ptr->RecoveryMoney.size();

			//if (recovery_array_size > 0 && recovery_array_size % faith::e_money_tuple_max == 0)
			//{
			//	for (int32 j = 0; j < recovery_array_size; j += faith::e_money_tuple_max)
			//	{
			//		int32 money_id = equip_template_ptr->RecoveryMoney[j + faith::e_money_tuple_id];
			//		int32 money_num = equip_template_ptr->RecoveryMoney[j + faith::e_money_tuple_num] * item_num;
			//		merge_money_to_two_tuples(money_reward_array, money_id, money_num);
			//	}
			//}
		}
		else if (item_template_ptr->item_type == faith::e_item_type_spirit)
		{
			SpiritTemplate* spirit_tmpl_ptr = GET_TEMPLATE(SpiritTemplate, item_template_ptr->logic_id + item_inst_data.data_ary[e_item_info_upgrade_count]);

			if (spirit_tmpl_ptr == nullptr)
			{
				return;
			}
			if (is_sprite_use_diamond)
			{
				int32 recovery_reward_array_size = 0;
				int32 recovery_cost_array_size = 0;
				recovery_reward_array_size = spirit_tmpl_ptr->DiamondRecoveryMoney.size();
				recovery_cost_array_size = spirit_tmpl_ptr->DiamondRecoveryCost.size();

				if (recovery_reward_array_size > 0 && recovery_reward_array_size % faith::e_money_tuple_max == 0)
				{
					for (int32 j = 0; j < recovery_reward_array_size; j += faith::e_money_tuple_max)
					{
						int32 money_id = spirit_tmpl_ptr->DiamondRecoveryMoney[j + faith::e_money_tuple_id];
						int32 money_num = spirit_tmpl_ptr->DiamondRecoveryMoney[j + faith::e_money_tuple_num] * item_num;
						merge_money_to_two_tuples(money_reward_array, money_id, money_num);
					}
				}
				if (recovery_cost_array_size > 0 && recovery_cost_array_size % faith::e_money_tuple_max == 0)
				{
					for (int32 j = 0; j < recovery_cost_array_size; j += faith::e_money_tuple_max)
					{
						int32 money_id = spirit_tmpl_ptr->DiamondRecoveryCost[j + faith::e_money_tuple_id];
						int32 money_num = spirit_tmpl_ptr->DiamondRecoveryCost[j + faith::e_money_tuple_num] * item_num;
						merge_money_to_two_tuples(money_cost_array, money_id, money_num);
					}
				}
			}
			else
			{
				int32 recovery_reward_array_size = 0;
				recovery_reward_array_size = spirit_tmpl_ptr->RecoveryMoney.size();

				if (recovery_reward_array_size > 0 && recovery_reward_array_size % faith::e_money_tuple_max == 0)
				{
					for (int32 j = 0; j < recovery_reward_array_size; j += faith::e_money_tuple_max)
					{
						int32 money_id = spirit_tmpl_ptr->RecoveryMoney[j + faith::e_money_tuple_id];
						int32 money_num = spirit_tmpl_ptr->RecoveryMoney[j + faith::e_money_tuple_num] * item_num;
						merge_money_to_two_tuples(money_reward_array, money_id, money_num);
					}
				}
			}
		}
		else
		{
			int32 recovery_reward_array_size = 0;
			recovery_reward_array_size = item_template_ptr->SellMoney.size();

			if (recovery_reward_array_size > 0 && recovery_reward_array_size % faith::e_money_tuple_max == 0)
			{
				for (int32 j = 0; j < recovery_reward_array_size; j += faith::e_money_tuple_max)
				{
					int32 money_id = item_template_ptr->SellMoney[j + faith::e_money_tuple_id];
					int32 money_num = item_template_ptr->SellMoney[j + faith::e_money_tuple_num] * item_num;
					init_unit::merge_money_to_two_tuples(money_reward_array, money_id, money_num);
				}
			}
		}
	}

	int32 init_unit::get_mission_money_empty_slot(int64* money_value_array)
	{
		for (int32 i = 0; i < max_mission_money_reward_type; ++i)
		{
			if (money_value_array[i] == 0)
			{
				return i;
			}
		}
		return -1;
	}

	int32 init_unit::get_mission_money_same_slot(int32* money_type_array, int32 target_type)
	{
		//即使是经验（类型本身就是0）本函数也照常生效
		for (int32 i = 0; i < max_mission_money_reward_type; ++i)
		{
			if (money_type_array[i] == target_type)
			{
				return i;
			}
		}
		return -1;
	}

	int32 init_unit::get_upgrade_phase_level(int32 upgrade_num)
	{
		int32 return_value = 0;
		int32 upgrade_array_num = GAMECONFIG->UpgradeBuffLevelArray.size();
		if (upgrade_array_num <= 0)
		{
			return 0;
		}
		if (upgrade_num < GAMECONFIG->UpgradeBuffLevelArray[0])
		{
			return_value = 0;
		}
		else if (upgrade_num >= GAMECONFIG->UpgradeBuffLevelArray[upgrade_array_num - 1])
		{
			return_value = GAMECONFIG->UpgradeBuffLevelArray[upgrade_array_num - 1];
		}
		else
		{
			for (int32 i = 0; i < upgrade_array_num - 1; ++i)
			{
				if (GAMECONFIG->UpgradeBuffLevelArray[i] <= upgrade_num && upgrade_num < GAMECONFIG->UpgradeBuffLevelArray[i + 1])
				{
					return_value = GAMECONFIG->UpgradeBuffLevelArray[i];
					break;
				}
			}
		}
		return return_value;
	}

	int32 init_unit::get_addon_phase_level(int32 addon_num)
	{
		int32 array_max = GAMECONFIG->AddonBuffLevelArray.size();
		if (array_max <= 0)
		{
			return 0;
		}
		int32 return_value = 0;
		if (addon_num < GAMECONFIG->AddonBuffLevelArray[0])
		{
			return_value = 0;
		}
		else if (addon_num >= GAMECONFIG->AddonBuffLevelArray[array_max - 1])
		{
			return_value = GAMECONFIG->AddonBuffLevelArray[array_max - 1];
		}
		else
		{
			for (int32 i = 0; i < array_max - 1; ++i)
			{
				if (GAMECONFIG->AddonBuffLevelArray[i] <= addon_num && addon_num < GAMECONFIG->AddonBuffLevelArray[i + 1])
				{
					return_value = GAMECONFIG->AddonBuffLevelArray[i];
					break;
				}
			}
		}
		return return_value;
	}

	int32 init_unit::get_upgrade_buff_id(ItemUpgradeTemplate* upgrade_templalte_ptr, int32 unit_class, int32 branch_type, int32 grade_num, bool is_weapon)
	{
		const std::vector<int32>* temp_array = nullptr;
		if (upgrade_templalte_ptr == nullptr
			|| unit_class <= e_class_type_none
			|| unit_class >= e_class_type_max
			|| branch_type < e_branch_type_common
			|| branch_type >= e_branch_type_max)
		{
			return 0;
		}

		if (is_weapon)
		{
			//switch (unit_class)
			//{
			//case e_class_type_barserker:
			//	temp_array = &upgrade_templalte_ptr->BarserkerWeaponBuffArray;
			//	break;
			//case e_class_type_wizard:
			//	temp_array = &upgrade_templalte_ptr->WizardWeaponBuffArray;
			//	break;
			//case e_class_type_guardian:
			//	temp_array = &upgrade_templalte_ptr->GuardianWeaponBuffArray;
			//	break;
			//case e_class_type_assassinator:
			//	temp_array = &upgrade_templalte_ptr->AssassinWeaponBuffArray;
			//	break;
			//default:
			//	break;
			//}
		}
		else
		{
			switch (unit_class)
			{
			case e_class_type_barserker:
				switch (branch_type)
				{
					//case e_branch_type_common:
					//case e_branch_type_ice:
					//case e_branch_type_fire:
					//case e_branch_type_light:
					//case e_branch_type_dark:
					//	temp_array = &upgrade_templalte_ptr->BarserkerEquipBuffArrayCommon;
					//	break;
					//default:
					//	break;
					/*	case e_branch_type_common:
							temp_array = &upgrade_templalte_ptr->BarserkerEquipBuffArrayCommon;
							break;
						case e_branch_type_ice:
							temp_array = &upgrade_templalte_ptr->BarserkerEquipBuffArrayIce;
							break;
						case e_branch_type_fire:
							temp_array = &upgrade_templalte_ptr->BarserkerEquipBuffArrayFire;
							break;
						case e_branch_type_light:
							temp_array = &upgrade_templalte_ptr->BarserkerEquipBuffArrayLight;
							break;
						case e_branch_type_dark:
							temp_array = &upgrade_templalte_ptr->BarserkerEquipBuffArrayDark;
							break;
						default:
							break;*/
				}
				break;
			case e_class_type_wizard:
				switch (branch_type)
				{
					//case e_branch_type_common:
					//case e_branch_type_ice:
					//case e_branch_type_fire:
					//case e_branch_type_light:
					//case e_branch_type_dark:
					//	temp_array = &upgrade_templalte_ptr->WizardEquipBuffArrayCommon;
					//	break;
					//default:
					//	break;
			/*	case e_branch_type_common:
					temp_array = &upgrade_templalte_ptr->WizardEquipBuffArrayCommon;
					break;
				case e_branch_type_ice:
					temp_array = &upgrade_templalte_ptr->WizardEquipBuffArrayIce;
					break;
				case e_branch_type_fire:
					temp_array = &upgrade_templalte_ptr->WizardEquipBuffArrayFire;
					break;
				case e_branch_type_light:
					temp_array = &upgrade_templalte_ptr->WizardEquipBuffArrayLight;
					break;
				case e_branch_type_dark:
					temp_array = &upgrade_templalte_ptr->WizardEquipBuffArrayDark;
					break;
				default:
					break;*/
				}
				break;
			case e_class_type_guardian:
				switch (branch_type)
				{
					//case e_branch_type_common:
					//case e_branch_type_ice:
					//case e_branch_type_fire:
					//case e_branch_type_light:
					//case e_branch_type_dark:
					//	temp_array = &upgrade_templalte_ptr->GuardianEquipBuffArrayCommon;
					//	break;
					//default:
					//	break;
			/*	case e_branch_type_common:
					temp_array = &upgrade_templalte_ptr->GuardianEquipBuffArrayCommon;
					break;
				case e_branch_type_ice:
					temp_array = &upgrade_templalte_ptr->GuardianEquipBuffArrayIce;
					break;
				case e_branch_type_fire:
					temp_array = &upgrade_templalte_ptr->GuardianEquipBuffArrayFire;
					break;
				case e_branch_type_light:
					temp_array = &upgrade_templalte_ptr->GuardianEquipBuffArrayLight;
					break;
				case e_branch_type_dark:
					temp_array = &upgrade_templalte_ptr->GuardianEquipBuffArrayDark;
					break;
				default:
					break;*/
				}
				break;
			case e_class_type_assassinator:
				switch (branch_type)
				{
					//case e_branch_type_common:
					//case e_branch_type_ice:
					//case e_branch_type_fire:
					//case e_branch_type_light:
					//case e_branch_type_dark:
					//	temp_array = &upgrade_templalte_ptr->AssassinEquipBuffArrayCommon;
					//	break;
					//default:
					//	break;
					/*case e_branch_type_common:
						temp_array = &upgrade_templalte_ptr->AssassinEquipBuffArrayCommon;
						break;
					case e_branch_type_ice:
						temp_array = &upgrade_templalte_ptr->AssassinEquipBuffArrayIce;
						break;
					case e_branch_type_fire:
						temp_array = &upgrade_templalte_ptr->AssassinEquipBuffArrayFire;
						break;
					case e_branch_type_light:
						temp_array = &upgrade_templalte_ptr->AssassinEquipBuffArrayLight;
						break;
					case e_branch_type_dark:
						temp_array = &upgrade_templalte_ptr->AssassinEquipBuffArrayDark;
						break;
					default:
						break;*/
				}
				break;
			default:
				break;
			}
		}
		if (temp_array == nullptr
			|| grade_num >= temp_array->size())
		{
			return 0;
		}
		return (*temp_array)[grade_num];
	}

	void init_unit::random_sprite_flair_property(s_item_info& temp_info)
	{
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, temp_info.data_ary[e_item_info_info_id]);
		if (item_template_ptr == nullptr)
		{
			return;
		}
		SpiritTemplate* temp_spirit_template_ptr = GET_TEMPLATE(SpiritTemplate, item_template_ptr->logic_id);
		if (temp_spirit_template_ptr == nullptr)
		{
			return;
		}

		int32 flait_att_num = temp_spirit_template_ptr->FlairProperty.size() / EItemProperty_Max;
		if (temp_spirit_template_ptr->FlairPropertyWeight.size() != flait_att_num
			|| flait_att_num > e_item_info_random_property4 - e_item_info_random_property1 + 1)
		{
			return;
		}
		for (int32 i = 0; i < flait_att_num; ++i)
		{
			int32 temp_rand = random_gen::get_random(0, 99);
			if (temp_rand < temp_spirit_template_ptr->FlairPropertyWeight[i])
			{
				temp_info.data_ary[e_item_info_random_property1 + i] = 1;
			}
		}
	}

	bool init_unit::is_has_this_item_illusion(int32 has_illusion_byte, int32 target_index)
	{
		if (target_index < 0 || target_index >= item_illusion_max_num)
		{
			return false;
		}
		return (item_illusion_byte_array[target_index] & has_illusion_byte) == item_illusion_byte_array[target_index];
	}

	void init_unit::get_team_share_exp_ratio(int32 team_mem_num, int32 team_same_legion_mem_num, float& ratio_team, float& ratio_legion, float& ratio_team_raid)
	{
		float team_exp_ratio = 0;
		float legion_exp_ratio = 0;
		float team_raid_ratio = 0;

		team_exp_ratio = GAMECONFIG->TeamExpRatio;
		legion_exp_ratio = GAMECONFIG->LegionExpRatio;
		team_raid_ratio = GAMECONFIG->TeamRaidFinishRatio;

		ratio_team = team_exp_ratio * (team_mem_num - 1);
		ratio_legion = legion_exp_ratio * (team_same_legion_mem_num - 1);
		ratio_team_raid = team_raid_ratio * (team_mem_num - 1);

		ratio_team = ratio_team > 1e-4 ? ratio_team : 0;
		ratio_legion = ratio_legion > 1e-4 ? ratio_legion : 0;
		ratio_team_raid = ratio_team_raid > 1e-4 ? ratio_team_raid : 0;
	}

	int32 init_unit::get_npc_level_by_upgrade(int32 up_id, int32 cur_npc_temp_id)
	{
		int32 old_level_tmp = up_id - monster_upgrade_cfg_first_id + 1;
		int32 UpgradeId = 0;
		NpcTemplate* npc_temp_ptr = GET_TEMPLATE(NpcTemplate, cur_npc_temp_id);
		if (nullptr == npc_temp_ptr)
		{		
			if (old_level_tmp < 0)
			{
				old_level_tmp = 0;
			}
			return old_level_tmp;
		}
		UpgradeId = npc_temp_ptr->UpgradeId;

		if (UpgradeId == up_id)//非动态等级
		{
			if (up_id >= new_monster_upgrade_cfg_first_id)
			{
				return up_id - new_monster_upgrade_cfg_first_id + 1;
			}
			else
			{
				return old_level_tmp;
			}
		}
		int32 level_tmp = up_id - UpgradeId + 1;
		if (level_tmp < 0)
		{
			level_tmp = 0;
		}
		return level_tmp;
	}

	void init_unit::hit_and_critical(
		const s_hit_critical_calcu_param& attack_att_param,
		const s_hit_critical_calcu_param& target_att_param,
		const std::vector<float>& HitRateParams,
		const std::vector<float>& CriticalRateParams,
		bool& be_hit,
		bool& be_critical
		)
	{
		f32 attacker_hit_force = attack_att_param.att_hit;
		f32 target_dodge_force = target_att_param.att_dodge;
		f32 att_hit_prob = attack_att_param.att_hit_prob;
		f32 att_un_hit_prob = target_att_param.att_un_hit_prob;
		
		if (attacker_hit_force <= 0.0001)
		{
			be_hit = false;
			be_critical = false;
			return;
		}
		if (FLOAT_IS_ZERO((attacker_hit_force + target_dodge_force*HitRateParams[1] + HitRateParams[2])))
		{
			return;
		}
		f32 hit_rate = (HitRateParams[0] * attacker_hit_force + HitRateParams[2]) / (attacker_hit_force + target_dodge_force * HitRateParams[1] + HitRateParams[2]);
		if (get_map_level_suppress(attack_att_param.map_type, attack_att_param.map_order))
		{
			if (((attack_att_param.unit_type == e_unit_type_player && target_att_param.unit_type != e_unit_type_player)
				|| (attack_att_param.unit_type != e_unit_type_player && target_att_param.unit_type == e_unit_type_player))
				&& HitRateParams.size() >= 4)
			{
				f32 attacker_lv = attack_att_param.unit_type == e_unit_type_player ? attack_att_param.unit_level : get_npc_level_by_upgrade(attack_att_param.unit_upgrade_id, attack_att_param.att_npc_temp_id);
				int32 tar_lv = target_att_param.unit_type == e_unit_type_player ? target_att_param.unit_level : get_npc_level_by_upgrade(target_att_param.unit_upgrade_id, target_att_param.att_npc_temp_id);
				//f32 tar_lv = target_att_param.unit_level;
				f32 level_hit_rate = 2 * (attacker_lv * HitRateParams[3] + tar_lv) / (tar_lv * HitRateParams[3] + attacker_lv) - 1;
				if (level_hit_rate < 0.1)
				{
					level_hit_rate = 0.1;
				}
				//if (level_hit_rate < 1)
				{
					hit_rate *= level_hit_rate;
				}
			}
		}
		hit_rate += att_hit_prob - att_un_hit_prob;
		hit_rate *= 10000;
		f32 hit_rand = random_gen::skill_random() % 10000;

		if (hit_rate > hit_rand)
		{
			be_hit = true;
		}
		else
		{
			be_hit = false;
		}

		if (be_hit == false)
		{
			be_critical = false;
			return;
		}
		f32 attacker_critical_rate = attack_att_param.att_critical;//暴击精通
		f32 target_un_critical_rate = target_att_param.att_critical_armor;//暴抗精通
		f32 att_crit_prob = attack_att_param.att_crit_prob;//暴击率
		f32 att_un_crit_prob = target_att_param.att_un_crit_prob;//暴抗率
		if (FLOAT_IS_ZERO((attacker_critical_rate + CriticalRateParams[1] * target_un_critical_rate + CriticalRateParams[2])))
		{
			return;
		}
		f32 critical_prob_real = (attacker_critical_rate*CriticalRateParams[0]) / (attacker_critical_rate + CriticalRateParams[1] * target_un_critical_rate + CriticalRateParams[2]);
		if (critical_prob_real > 0.8)
		{
			critical_prob_real = 0.8f;
		}
		critical_prob_real += att_crit_prob - att_un_crit_prob;
		critical_prob_real *= 10000;
		f32 critical_rand = random_gen::skill_random() % 10000;

		if (critical_prob_real > critical_rand)
		{
			be_critical = true;
		}
		else
		{
			be_critical = false;
		}
	}

	s_damage_count_all init_unit::skill_damage_end(
		const s_hit_critical_calcu_param& attack_att_param,
		const s_hit_critical_calcu_param& target_att_param,
		const double* attack_att_array,
		const double* target_att_array,
		use_buff_param& param,
		const std::vector<f32>& coefficient_a,
		const std::vector<f32>& coefficient_b,
		f32 skill_ratio,
		int32 skill_plus,
		f32 damage_sub,
		f32 person_attack_change,
		f32 sky_attack_ground
		)
	{
		s_damage_count_all damage_info;

		if (coefficient_a.size() < 2)
		{
			return damage_info;
		}

		if (coefficient_b.size() < 3)
		{
			return damage_info;
		}

		if (false == param.be_hit)
		{
			return damage_info;
		}
		
		//技能命中
		double attacker_value_max = attack_att_array[e_unit_attack_att_attack_max];//最大攻击
		double percent_atacker_value = attacker_value_max * 0.05f;
		int32 attacker_value_real = attacker_value_max - percent_atacker_value + (percent_atacker_value*2)*(random_gen::skill_random() % 101) / 100;

		double attacker_value_average =  attacker_value_max * 0.02f;
		double target_armor_value = target_att_array[e_unit_attack_att_armor] / 5.0f;//防御力
		double damage = 0;
		if (attacker_value_real*0.98 - target_armor_value > attacker_value_average)
		{//破防
			damage = attacker_value_real - target_armor_value;
		}
		else
		{//不破防
			attacker_value_average = attacker_value_average < 1 ? 1 : attacker_value_average;
			damage = random_gen::get_random(1, attacker_value_average);
		}
		//基础伤害确定
		damage = damage*skill_ratio + skill_plus + param.damage_base_value;
		double attacker_att_attack_1 = attack_att_array[e_unit_attack_att_att_attack_1];
		double attacker_att_attack_2 = attack_att_array[e_unit_attack_att_att_attack_2];
		double attacker_att_attack_3 = attack_att_array[e_unit_attack_att_att_attack_3];
		double attacker_att_attack_4 = attack_att_array[e_unit_attack_att_att_attack_4];
		double attacker_att_attack_5 = 0;//attack_att_array[e_unit_attack_att_att_attack_5];

		double target_att_armor_1 = target_att_array[e_unit_attack_att_att_armor_1];
		double target_att_armor_2 = target_att_array[e_unit_attack_att_att_armor_2];
		double target_att_armor_3 = target_att_array[e_unit_attack_att_att_armor_3];
		double target_att_armor_4 = target_att_array[e_unit_attack_att_att_armor_4];
		double target_att_armor_5 = 0;// target_att_array[e_unit_attack_att_att_armor_5];

		double attacker_element_ability = attack_att_array[e_unit_attack_att_element_ability];//元素精通
		double target_element_resist = target_att_array[e_unit_attack_att_element_resist];//元素抗性

		double att_damage_ratio = ((attacker_element_ability - target_element_resist) > 0.3) ? attacker_element_ability - target_element_resist : 0.3;
		//double att_damage_1 = ((attacker_att_attack_1 - target_att_armor_1) > 0) ? (attacker_att_attack_1 - target_att_armor_1) * att_damage_ratio : 0;
		//double att_damage_2 = ((attacker_att_attack_2 - target_att_armor_2) > 0) ? (attacker_att_attack_2 - target_att_armor_2) * att_damage_ratio : 0;
		//double att_damage_3 = ((attacker_att_attack_3 - target_att_armor_3) > 0) ? (attacker_att_attack_3 - target_att_armor_3) * att_damage_ratio : 0;
		//double att_damage_4 = ((attacker_att_attack_4 - target_att_armor_4) > 0) ? (attacker_att_attack_4 - target_att_armor_4) * att_damage_ratio : 0;
		//double att_damage_5 = ((attacker_att_attack_5 - target_att_armor_5) > 0) ? (attacker_att_attack_5 - target_att_armor_5) * att_damage_ratio : 0;

		double temp_att_damage_1_sub_value = coefficient_b[1] * attacker_att_attack_1 + coefficient_b[2] * target_att_armor_1;
		double att_damage_1 = 0;
		if (!FLOAT_IS_ZERO(temp_att_damage_1_sub_value))
		{
			double temp_att_damage_1_value = ((coefficient_b[0] * attacker_att_attack_1) * (coefficient_b[0] * attacker_att_attack_1)) / temp_att_damage_1_sub_value;
			att_damage_1 = (temp_att_damage_1_value > 0) ? temp_att_damage_1_value * att_damage_ratio : 0;
		}
			
		double temp_att_damage_2_sub_value = coefficient_b[1] * attacker_att_attack_2 + coefficient_b[2] * target_att_armor_2;
		double att_damage_2 = 0;
		if (!FLOAT_IS_ZERO(temp_att_damage_2_sub_value))
		{
			double temp_att_damage_2_value = ((coefficient_b[0] * attacker_att_attack_2) * (coefficient_b[0] * attacker_att_attack_2)) / temp_att_damage_2_sub_value;
			att_damage_2 = (temp_att_damage_2_value > 0) ? temp_att_damage_2_value * att_damage_ratio : 0;
		}

		double temp_att_damage_3_sub_value = coefficient_b[1] * attacker_att_attack_3 + coefficient_b[2] * target_att_armor_3;
		double att_damage_3 = 0;
		if (!FLOAT_IS_ZERO(temp_att_damage_3_sub_value))
		{
			double temp_att_damage_3_value = ((coefficient_b[0] * attacker_att_attack_3) * (coefficient_b[0] * attacker_att_attack_3)) / temp_att_damage_3_sub_value;
			att_damage_3 = (temp_att_damage_3_value > 0) ? temp_att_damage_3_value * att_damage_ratio : 0;
		}

		double temp_att_damage_4_sub_value = coefficient_b[1] * attacker_att_attack_4 + coefficient_b[2] * target_att_armor_4;
		double att_damage_4 = 0;
		if (!FLOAT_IS_ZERO(temp_att_damage_4_sub_value))
		{
			double temp_att_damage_4_value = ((coefficient_b[0] * attacker_att_attack_4) * (coefficient_b[0] * attacker_att_attack_4)) / temp_att_damage_4_sub_value;
			att_damage_4 = (temp_att_damage_4_value > 0) ? temp_att_damage_4_value * att_damage_ratio : 0;
		}

		double temp_att_damage_5_sub_value = coefficient_b[1] * attacker_att_attack_5 + coefficient_b[2] * target_att_armor_5;
		double att_damage_5 = 0;
		if (!FLOAT_IS_ZERO(temp_att_damage_5_sub_value))
		{
			double temp_att_damage_5_value = ((coefficient_b[0] * attacker_att_attack_5) * (coefficient_b[0] * attacker_att_attack_5)) / temp_att_damage_5_sub_value;
			att_damage_5 = (temp_att_damage_5_value > 0) ? temp_att_damage_5_value * att_damage_ratio : 0;
		}
			
		switch (attack_att_param.branch_type)
		{
		case e_branch_type_ice:// 冰
			att_damage_1 *= coefficient_a[0];
			att_damage_2 *= coefficient_a[1];
			att_damage_3 *= coefficient_a[0];
			att_damage_4 *= coefficient_a[0];
			break;
		case e_branch_type_fire:// 火
			att_damage_1 *= coefficient_a[1];
			att_damage_2 *= coefficient_a[0];
			att_damage_3 *= coefficient_a[0];
			att_damage_4 *= coefficient_a[0];
			break;
		case e_branch_type_light:// 光
			att_damage_1 *= coefficient_a[0];
			att_damage_2 *= coefficient_a[0];
			att_damage_3 *= coefficient_a[1];
			att_damage_4 *= coefficient_a[0];
			break;
		case e_branch_type_dark:// 暗
			att_damage_1 *= coefficient_a[0];
			att_damage_2 *= coefficient_a[0];
			att_damage_3 *= coefficient_a[0];
			att_damage_4 *= coefficient_a[1];
			break;
		default://其他 比如没有分支
			att_damage_1 *= coefficient_a[0];
			att_damage_2 *= coefficient_a[0];
			att_damage_3 *= coefficient_a[0];
			att_damage_4 *= coefficient_a[0];
			break;
		}

		//	S（元素）=∑〖（（元素值-元素抗性）*对应系数〗）*（元素精通-元素亲和）   元素伤害确定

		//			double correct_value_a = 1;//修正值A
		//			double correct_value_b = 1;//修正值B
		//			double skill_value = 1;//技能系数

		if (param.be_critical)
		{//暴击
			double attacker_critical_attack_rate = attack_att_array[e_unit_attack_att_critical_attack_rate];//暴击伤害
			double target_critical_attack_relief = target_att_array[e_unit_attack_att_critical_attack_relief];//暴击减伤
			if (attacker_critical_attack_rate > target_critical_attack_relief + 1.0f)
			{//爆伤减爆抗低于1强制等于1
				damage = damage * (attacker_critical_attack_rate - target_critical_attack_relief);
			}
		}

		double attacker_effect_damage = attack_att_array[e_unit_attack_att_effect_damage];//无双
		double target_effect_damage_relief = target_att_array[e_unit_attack_att_effect_damage_relief];//识破
		
		double element_damage = att_damage_1 + att_damage_2 + att_damage_3 + att_damage_4 /*+ att_damage_5*/;
		element_damage = element_damage* skill_ratio;
		if (get_map_level_suppress(attack_att_param.map_type, attack_att_param.map_order))
		{
			if (attack_att_param.unit_type == e_unit_type_player
				&& target_att_param.unit_type != e_unit_type_player)
			{
				int32 tar_lv = get_npc_level_by_upgrade(target_att_param.unit_upgrade_id, target_att_param.att_npc_temp_id);
				if (attack_att_param.unit_level + 20 < tar_lv)
				{
					double exp_level_add = 1 - (tar_lv - attack_att_param.unit_level) * 0.01;
					exp_level_add = exp_level_add > 1 ? 1 : exp_level_add;
					exp_level_add = exp_level_add < 0.01 ? 0.01 : exp_level_add;
					damage *= exp_level_add;
					element_damage *= exp_level_add;
				}
			}
			else if (attack_att_param.unit_type != e_unit_type_player
				&& target_att_param.unit_type == e_unit_type_player)
			{
				int32 tar_lv = get_npc_level_by_upgrade(attack_att_param.unit_upgrade_id, attack_att_param.att_npc_temp_id);
				if (tar_lv > target_att_param.unit_level + 20)
				{
					double exp_level_add = 1 + (tar_lv - target_att_param.unit_level) * (tar_lv - target_att_param.unit_level) * 0.003;
					damage *= exp_level_add;
					element_damage *= exp_level_add;
				}
			}
		}
		if (attack_att_param.unit_type == e_unit_type_player)
		{
			if (target_att_param.move_type < e_move_ment_mount_fly && attack_att_param.move_type >= e_move_ment_mount_fly)
			{
				damage *= sky_attack_ground;
				element_damage *= sky_attack_ground;
			}
		}
		//double all_damage = (damage + element_damage);
		double all_damage_ratio = 0;
		double end_damage = 0;
		if (target_att_param.unit_type == e_unit_type_monster)
		{
			 all_damage_ratio = ((attacker_effect_damage - target_effect_damage_relief) > 0) ? (attacker_effect_damage - target_effect_damage_relief) : 0;
			 end_damage = damage * all_damage_ratio + element_damage;//S（最终）=（S（基础）或S（暴击）+S（元素））*（无双-识破）或闪避。
		}
		else
		{
			 all_damage_ratio = ((attacker_effect_damage - target_effect_damage_relief) > 0.05) ? (attacker_effect_damage - target_effect_damage_relief) : 0.05;
			 end_damage = damage * all_damage_ratio + element_damage;//S（最终）=（S（基础）或S（暴击）+S（元素））*（无双-识破）或闪避。
		}

		if (end_damage > target_att_array[e_unit_attack_att_hp_max] * 0.8)
		{
			//Y.	S（最终）>目标生命上限*80%时，S（最终）=S（最终）*95%
			end_damage = end_damage * 0.95;
		}
		end_damage = end_damage + end_damage * param.add_percent;//百分比buff带来的百分比伤害提升
		end_damage = end_damage - end_damage * damage_sub;//伤害减免

		float damage_unit = person_attack_change;//人打人伤害减少
		if (attack_att_param.unit_type == e_unit_type_player
			&& target_att_param.unit_type != e_unit_type_player)
		{
			damage_unit += attack_att_array[e_unit_attack_att_attack_npc];
		}
		else if(attack_att_param.unit_type == e_unit_type_player
			&& target_att_param.unit_type == e_unit_type_player)
		{
			damage_unit += attack_att_array[e_unit_attack_att_attack_player] + target_att_array[e_unit_attack_att_be_attack_player];
		}
		end_damage *= damage_unit;
		end_damage += attack_att_array[e_unit_attack_att_attack_multiple] * end_damage;
		element_damage += attack_att_array[e_unit_attack_att_attack_multiple] * element_damage;
		end_damage = end_damage > 1 ? end_damage : 1;
		damage_info.damage_total = end_damage;
		damage_info.damage_elem = element_damage;
		return damage_info;
	}


	int32 init_unit::get_community_group_relation_by_data(int32 type1, int32 type2)
	{
		if (type1 < 0 || type1 >= e_community_type_max)
		{
			return 0;
		}
		if (type2 < 0 || type2 >= e_community_type_max)
		{
			return 0;
		}
		const s_community_group_relation& community_relation = globle_data::get_instance().get_community_group_relation();
		return community_relation.data_ary[type1][type2];
	}

	TArray<int32> init_unit::get_item_by_class_type(const e_class_type class_type, const TArray<int32> item_array)
	{
		TArray<int32> result_item_array;
		return result_item_array;
	}

	bool init_unit::is_skill_friendly(e_skill_target_type skill_target_type)
	{
		switch (skill_target_type)
		{
		case eskilltarget_friend:
		case eskilltarget_friendplayer:
		case eskilltarget_friendnpc:
		case eskilltarget_self:
		case eskilltarget_selfpet:
		case eskilltarget_selfowner:
		case eskilltarget_friendplayer_noself:
			return true;
		default:
			return false;
		}
	}

	bool init_unit::is_enemy_with_param(const s_is_skill_target_param& param)
	{
		if (param.is_self == true || param.is_self_owner == true || param.is_self_pet == true)
		{
			return false;
		}
		if (param.region_type == e_region_type_protect)
		{
			return false; //所有unit在保护区内不可pk
		}
		if (param.is_team_protect && param.is_in_same_team)
		{
			return false;
		}
		if (param.is_legion_protect && param.is_in_same_legion)
		{
			return false;
		}

		//阵营不同时 判断阵营关系 若是对立阵营可打 若是友方阵营不可打
		if (param.sender_community_type != param.target_community_type)
		{
			return get_community_group_relation_by_data(param.sender_community_type, param.target_community_type) == 0;
		}
		else if ((param.sender_community_type >= e_community_type_player_group3) && (param.sender_community_type <= e_community_type_player_group6))
		{
			//3-6阵营  阵营内不可战斗
			return false;
		}
		else if ((param.sender_community_type >= e_community_type_attack_player) && (param.sender_community_type <= e_community_type_guard_boss) && param.sender_community_type == param.sender_community_type)
		{
			//13-17阵营  阵营内不可战斗
			return false;
		}

		// 后面的逻辑都是在同一阵营的基础上做判断
		// 统一用region_type判断了
		if (param.region_type == e_region_type_safe)
		{
			return false; //同阵营unit在安全区内不可pk
		}
		// 2line 不可pk
		if (param.sender_line_id == 2)
		{
			return false;
		}

		if (param.is_fight_back_state)
		{
			return true;
		}

		//同一阵营 走pk模式
		switch (param.sender_pk_mode)
		{
		case e_pk_mode_peace:
		{
			return false;
		}
		break;
		case e_pk_mode_good_and_evil:
		{
			if (param.target_pk_value <= 0)
			{
				return false;
			}
		}
		break;
		case e_pk_mode_team:
		{
			if (param.is_in_same_team)
			{
				return false;
			}
		}
		break;
		case e_pk_mode_family:
		{
			if (param.is_in_same_legion)
			{
				return false;
			}
		}
		break;
		case e_pk_mode_slaughter:
		{
		}
		break;
		case e_pk_mode_cross_sever:
		{
			if (param.sender_sever_id == param.target_sever_id)
			{
				return false;
			}
		}
		break;
		case e_pk_mode_plunder:
		{
			if ((param.sender_sever_id == param.cur_server_id && param.target_sever_id == param.cur_server_id) ||
				(param.sender_sever_id != param.cur_server_id && param.target_sever_id != param.cur_server_id))
			{
				return false;
			}	
		}
		break;
		default:
			break;
		}
		return true;
	}

	bool init_unit::is_skill_target_with_param(const s_is_skill_target_param& param)
	{
		if (param.skill_target_type < 0 || param.skill_target_type >= eskilltarget_max)
		{
			return false;
		}

		if (param.target_unit_type == faith::e_unit_type_trap 
			|| param.target_unit_type == faith::e_unit_type_drop_bag
			|| param.target_unit_type == faith::e_unit_type_hide)
		{
			return false;
		}

		bool is_skill_target = false;
		switch (param.skill_target_type)
		{
		case eskilltarget_enemy:
		{
			if (is_enemy_with_param(param) == true)
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_friend:
		{
			if (is_enemy_with_param(param) == false)
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_enemyplayer:
		{
			if ((is_enemy_with_param(param) == true)
				&& (param.target_unit_type == e_unit_type_player))
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_enemynpc:
		{
			if ((is_enemy_with_param(param) == true)
				&& (param.target_unit_type == e_unit_type_monster))
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_friendplayer:
		{
			if ((is_enemy_with_param(param) == false)
				&& (param.target_unit_type == e_unit_type_player))
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_friendnpc:
		{
			if ((is_enemy_with_param(param) == false)
				&& (param.target_unit_type == e_unit_type_monster))
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_team:
		{
			if (param.is_in_same_team == true)
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_self:
		{
			if (param.is_self)
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_selfpet:
		{
			if (param.is_self_pet)
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_selfowner:
		{
			if (param.is_self_owner)
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_boss:
		{
			if ((is_enemy_with_param(param) == true)
				&& (param.is_boss == true))
			{
				is_skill_target = true;
			}
		}
		break;
		case eskilltarget_groud:
		{

		}
		break;
		case eskilltarget_friendplayer_noself:
		{
			if ((is_enemy_with_param(param) == false)
				&& (param.target_unit_type == e_unit_type_player)
				&& (param.is_self == false))
			{
				is_skill_target = true;
			}
		}
		break;
		}
		return is_skill_target;
	}

	bool init_unit::is_robot_account(xstring robot_standard, xstring robot_account)
	{
		int32 account_len = robot_standard.size() > max_account_length ? max_account_length : robot_standard.size();
		account_len = account_len > robot_account.size() ? robot_account.size() : account_len;
		xchar temp_account[max_account_length + 1];
		memset(temp_account, 0, sizeof(temp_account));
		for (int32 i = 0; i < account_len; ++i)
		{
			temp_account[i] = robot_account[i];
		}
		if (strcmp(robot_standard.c_str(), temp_account) == 0)
		{
			return true;
		}
		return false;
	}

	void init_unit::set_excellent_had_flag(const int32& total_num, const int32& final_num, s_item_info& data_info, int32 item_const_att)
	{
		data_info.data_ary[e_item_info_random_had_flag] = 0;
		if (final_num <= 0
			|| total_num <= 0)
		{
			return;
		}

		std::vector<int32> get_att_index_array, temp_att_index_lib_array;
		get_att_index_array.reserve(10);
		temp_att_index_lib_array.reserve(total_num);
		if (item_const_att <= 0)//小于等于0代表不是固定属性装备，走正常逻辑
		{

			for (int32 i = 0; i < total_num; ++i)
			{
				temp_att_index_lib_array.push_back(i);
			}
			for (int32 i = 0; i < final_num; ++i)
			{
				if (temp_att_index_lib_array.size() <= 0)
				{
					break;
				}
				int32 size_temp_att_index_lib = temp_att_index_lib_array.size();
				int32 temp_index = random_gen::get_random(0, size_temp_att_index_lib - 1);
				get_att_index_array.push_back(temp_att_index_lib_array[temp_index]);
				temp_att_index_lib_array.erase(temp_att_index_lib_array.begin() + temp_index);
			}
		}
		else//是固定卓越属性装备，则取属性前N条
		{
			for (int32 i = 0; i < final_num; ++i)
			{
				get_att_index_array.push_back(i);
			}
		}

		for (int32 i = 0; i < get_att_index_array.size(); ++i)
		{
			if (get_att_index_array[i] < 0
				|| get_att_index_array[i] >= 32)
			{
				continue;
			}
			mark_flag_data(data_info.data_ary[e_item_info_random_had_flag], get_att_index_array[i] + 1);
		}
	}

	void init_unit::set_excellent_ratio_value(EquipTemplate* equip_template_ptr, s_item_info& data_info,int32 item_const_att)
	{
		for (int32 i = e_item_info_random_property1; i <= e_item_info_random_property6; ++i)
		{
			data_info.data_ary[i] = 0;
		}
		if (data_info.data_ary[e_item_info_random_had_flag] == 0)
		{
			return;
		}
		//if (equip_template_ptr->BaseExcellentAtt.size() % e_att_one_max != 0)
		//{
		//	return;
		//}
		//int32 total_att_num = equip_template_ptr->BaseExcellentAtt.size() / e_att_one_max;

		//int32 value_info_index = e_item_info_random_property1;
		//for (int32 i = 0; i < total_att_num; ++i)
		//{
		//	if (value_info_index > e_item_info_random_property6)
		//	{
		//		break;
		//	}
		//	if (get_flag_data(data_info.data_ary[e_item_info_random_had_flag], i + 1) <= 0)
		//	{
		//		continue;
		//	}
		//	int32 final_excellent_color = rand_excellent_att_color(equip_template_ptr, item_const_att);
		//	std::vector<int32>& temp_color_rate_array = template_manager::get_instance().get_excellent_target_color_array(equip_template_ptr, final_excellent_color);
		//	if (temp_color_rate_array.size() <= 0
		//		|| temp_color_rate_array.size() % 2 != 0
		//		|| temp_color_rate_array.size() / 2 != total_att_num)
		//	{
		//		continue;
		//	}
		//	if (item_const_att <= 0)
		//	{
		//		int32 min_limit = temp_color_rate_array[i * 2];
		//		int32 max_limit = temp_color_rate_array[i * 2 + 1];
		//		data_info.data_ary[value_info_index] = random_gen::get_random(min_limit, max_limit);
		//	}
		//	else//固定卓越属性装备一定取最小值
		//	{
		//		int32 min_limit = temp_color_rate_array[i * 2];
		//		data_info.data_ary[value_info_index] = min_limit;
		//	}
		//	value_info_index++;
		//}
	}

	void init_unit::set_spirit_excellent_ratio_value(SpiritTemplate* spirit_template_ptr, s_item_info & data_info, int32 item_const_att)
	{
		for (int32 i = e_item_info_random_property1; i <= e_item_info_random_property6; ++i)
		{
			data_info.data_ary[i] = 0;
		}
		if (data_info.data_ary[e_item_info_random_had_flag] == 0)
		{
			return;
		}
		if (spirit_template_ptr->BaseExcellentAtt.size() % e_att_one_max != 0)
		{
			return;
		}
		int32 total_att_num = spirit_template_ptr->BaseExcellentAtt.size() / e_att_one_max;

		int32 value_info_index = e_item_info_random_property1;
		for (int32 i = 0; i < total_att_num; ++i)
		{
			if (value_info_index > e_item_info_random_property6)
			{
				break;
			}
			if (get_flag_data(data_info.data_ary[e_item_info_random_had_flag], i + 1) <= 0)
			{
				continue;
			}
			int32 final_excellent_color = rand_spirit_excellent_att_color(spirit_template_ptr, item_const_att);
			std::vector<int32>& temp_color_rate_array = template_manager::get_instance().get_spirit_excellent_target_color_array(spirit_template_ptr, final_excellent_color);
			if (temp_color_rate_array.size() <= 0
				|| temp_color_rate_array.size() % 2 != 0
				|| temp_color_rate_array.size() / 2 != total_att_num)
			{
				continue;
			}
			if (item_const_att <= 0)
			{
				int32 min_limit = temp_color_rate_array[i * 2];
				int32 max_limit = temp_color_rate_array[i * 2 + 1];
				data_info.data_ary[value_info_index] = random_gen::get_random(min_limit, max_limit);
			}
			else//固定卓越属性装备一定取最小值
			{
				int32 min_limit = temp_color_rate_array[i * 2];
				data_info.data_ary[value_info_index] = min_limit;
			}
			value_info_index++;
		}
	}

	int32 init_unit::rand_excellent_att_color(EquipTemplate* equip_template_ptr, int32 item_const_att)
	{
		if (equip_template_ptr == nullptr)
		{
			return e_item_color_max;
		}
		if (item_const_att > 0)//如果是固定卓越属性，则属性颜色已经是固定的，直接返回
		{
			return item_const_att;
		}
		int32 color_rand_value = random_gen::get_random(1, 100);
		//不能从0开始

		//int32 color_rand_target_value = 0;
		//int32 final_excellent_color = 0;
		//for (int32 i = 0; i < equip_template_ptr->ExcellentAttQualityWeight.size(); ++i)
		//{
		//	color_rand_target_value += equip_template_ptr->ExcellentAttQualityWeight[i];
		//	if (color_rand_value <= color_rand_target_value)
		//	{
		//		return i;
		//	}
		//}
		return e_item_color_max;
	}

	int32 init_unit::rand_spirit_excellent_att_color(SpiritTemplate* spirit_template_ptr, int32 item_const_att)
	{
		if (spirit_template_ptr == nullptr)
		{
			return e_item_color_max;
		}
		if (item_const_att > 0)//如果是固定卓越属性，则属性颜色已经是固定的，直接返回
		{
			return item_const_att;
		}
		int32 color_rand_value = random_gen::get_random(1, 100);
		//不能从0开始

		int32 color_rand_target_value = 0;
		int32 final_excellent_color = 0;
		for (int32 i = 0; i < spirit_template_ptr->ExcellentAttQualityWeight.size(); ++i)
		{
			color_rand_target_value += spirit_template_ptr->ExcellentAttQualityWeight[i];
			if (color_rand_value <= color_rand_target_value)
			{
				return i;
			}
		}
		return e_item_color_max;
	}

	void init_unit::make_new_excellent_att(EquipTemplate* equip_template_ptr, faith::s_item_info& item_data, int32 item_const_att)
	{
		if (nullptr == equip_template_ptr)
		{
			return;
		}
		for (int32 i = e_item_info_random_had_flag; i <= e_item_info_random_property6; ++i)
		{
			item_data.data_ary[i] = 0;
		}
		//if (equip_template_ptr->GreenExcellentRatio.size() <= 0)
		//{
		//	return;
		//}

		//if (equip_template_ptr->BaseExcellentAtt.size() % faith::e_att_one_max != 0
		//	|| equip_template_ptr->GreenExcellentRatio.size() % 2 != 0
		//	|| equip_template_ptr->BlueExcellentRatio.size() % 2 != 0
		//	|| equip_template_ptr->PurpleExcellentRatio.size() % 2 != 0
		//	|| equip_template_ptr->OrangeExcellentRatio.size() % 2 != 0
		//	|| equip_template_ptr->RedExcellentRatio.size() % 2 != 0
		//	|| equip_template_ptr->PinkExcellentRatio.size() % 2 != 0)
		//{
		//	return;
		//}

		//int32 total_att_num = equip_template_ptr->BaseExcellentAtt.size() / faith::e_att_one_max;
		//if (total_att_num != equip_template_ptr->GreenExcellentRatio.size() / 2
		//	|| total_att_num != equip_template_ptr->BlueExcellentRatio.size() / 2
		//	|| total_att_num != equip_template_ptr->PurpleExcellentRatio.size() / 2
		//	|| total_att_num != equip_template_ptr->OrangeExcellentRatio.size() / 2
		//	|| total_att_num != equip_template_ptr->RedExcellentRatio.size() / 2
		//	|| total_att_num != equip_template_ptr->PinkExcellentRatio.size() / 2)
		//{
		//	return;
		//}
		//int32 att_num_rand_value = random_gen::get_random(1, 100);
		//int32 att_num_rand_target_value = 0;
		//int32 final_att_num = 0;//需要几条卓越属性
		//for (int32 i = 0; i < equip_template_ptr->ExcellentAttNumGenRate.size(); ++i)
		//{
		//	att_num_rand_target_value += equip_template_ptr->ExcellentAttNumGenRate[i];
		//	if (att_num_rand_value <= att_num_rand_target_value)
		//	{
		//		final_att_num = i;
		//		break;
		//	}
		//}
		//if (final_att_num <= 0)
		//{
		//	return;
		//}
		//set_excellent_had_flag(total_att_num, final_att_num, item_data, item_const_att);
		//set_excellent_ratio_value(equip_template_ptr, item_data, item_const_att);
	}

	void init_unit::make_new_spirit_eccellent_att(SpiritTemplate* spirit_template_ptr, faith::s_item_info & item_data, int32 item_const_att)
	{
		if (nullptr == spirit_template_ptr)
		{
			return;
		}
		for (int32 i = e_item_info_random_had_flag; i <= e_item_info_random_property6; ++i)
		{
			item_data.data_ary[i] = 0;
		}
		if (spirit_template_ptr->GreenExcellentRatio.size() <= 0)
		{
			return;
		}

		if (spirit_template_ptr->BaseExcellentAtt.size() % faith::e_att_one_max != 0
			|| spirit_template_ptr->GreenExcellentRatio.size() % 2 != 0
			|| spirit_template_ptr->BlueExcellentRatio.size() % 2 != 0
			|| spirit_template_ptr->PurpleExcellentRatio.size() % 2 != 0
			|| spirit_template_ptr->OrangeExcellentRatio.size() % 2 != 0
			|| spirit_template_ptr->RedExcellentRatio.size() % 2 != 0
			|| spirit_template_ptr->PinkExcellentRatio.size() % 2 != 0)
		{
			return;
		}

		int32 total_att_num = spirit_template_ptr->BaseExcellentAtt.size() / faith::e_att_one_max;
		if (total_att_num != spirit_template_ptr->GreenExcellentRatio.size() / 2
			|| total_att_num != spirit_template_ptr->BlueExcellentRatio.size() / 2
			|| total_att_num != spirit_template_ptr->PurpleExcellentRatio.size() / 2
			|| total_att_num != spirit_template_ptr->OrangeExcellentRatio.size() / 2
			|| total_att_num != spirit_template_ptr->RedExcellentRatio.size() / 2
			|| total_att_num != spirit_template_ptr->PinkExcellentRatio.size() / 2)
		{
			return;
		}
		int32 att_num_rand_value = random_gen::get_random(1, 100);
		int32 att_num_rand_target_value = 0;
		int32 final_att_num = 0;//需要几条卓越属性
		for (int32 i = 0; i < spirit_template_ptr->ExcellentAttNumGenRate.size(); ++i)
		{
			att_num_rand_target_value += spirit_template_ptr->ExcellentAttNumGenRate[i];
			if (att_num_rand_value <= att_num_rand_target_value)
			{
				final_att_num = i;
				break;
			}
		}
		if (final_att_num <= 0)
		{
			return;
		}
		set_excellent_had_flag(total_att_num, final_att_num, item_data, item_const_att);
		set_spirit_excellent_ratio_value(spirit_template_ptr, item_data, item_const_att);
	}

	int32 init_unit::get_activity_sec_left(int32 activity_type, e_activity_time_get time_get, bool is_other_time, bool is_cross_city_war, bool is_cross_active)
	{
		ActivityCommonConfigTemplate* _activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + activity_type);
		if (_activity_cfg_ptr == nullptr || 1 != _activity_cfg_ptr->IsOpen)
		{
			return -1;
		}
		std::vector<int32> ActivityOpenTime = _activity_cfg_ptr->ActivityOpenTime;
		std::vector<int32> OpenDate = _activity_cfg_ptr->OpenDate;
		if (is_other_time)
		{
			ActivityOpenTime = _activity_cfg_ptr->ActivityOpenTime1;
		}
		if (is_cross_active)
		{
			ActivityOpenTime = _activity_cfg_ptr->CrossActivityOpenTime;
			OpenDate = _activity_cfg_ptr->CrossOpenDate;
		}

		return get_activity_sec_left(activity_type, ActivityOpenTime, OpenDate, time_get, is_cross_city_war, is_cross_active);
	}
	
	int32 init_unit::get_activity_sec_left(int32 activity_type, const std::vector<int32>& ActivityOpenTime, const std::vector<int32>& OpenDate, e_activity_time_get time_get, bool is_cross_city_war, bool is_cross_active)
	{
		int32 activity_end_time = -1;

		int32 open_time_size = ActivityOpenTime.size();
		if (open_time_size < 8)
		{
			return activity_end_time;
		}

		bool is_in_weekday = false;
		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 now_stamp = cur_time_info.second;

		int32 open_day_size = OpenDate.size();
		if (open_day_size <= 0)
		{
			is_in_weekday = true;
		}
		else
		{
			int32 ServerOpenDays = globle_data::get_instance().get_server_days();
			int32 ActTemplateID = faith::first_activity_common_template_id + faith::e_activity_type_overlord_city_war;
			ActivityCommonConfigTemplate* _activity_type_overlord_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, ActTemplateID);
			if (nullptr == _activity_type_overlord_ptr)
			{
				return activity_end_time;
			}
			int32 OverLoadCityWarDay = _activity_type_overlord_ptr->ParamInt1;

			if (activity_type == faith::e_activity_type_overlord_city_war && ServerOpenDays < OverLoadCityWarDay && !is_cross_city_war)
			{
				is_in_weekday = false;
			}
			//else if (activity_type == faith::e_activity_type_overlord_city_war && ServerOpenDays == OverLoadCityWarDay && !is_cross_city_war)
			//{
			//	is_in_weekday = true;
			//}
			else
			{
				for (int32 idx = 0; idx < open_day_size; idx++)
				{
					if (cur_time_info.day_in_week == OpenDate[idx])
					{
						is_in_weekday = true;
						break;
					}
				}
			}
		}

		if (false == is_in_weekday)
		{
			return activity_end_time;
		}

		int32 begin_stamp = -1;
		int32 end_stamp = -1;
		for (int32 time_idx = 0; time_idx < open_time_size - 7; time_idx += 8)
		{
			switch (time_get)
			{
			case faith::e_activity_time_get_ready:
				begin_stamp = time_helper::get_stamp_by_hour_min(ActivityOpenTime[2 + time_idx], ActivityOpenTime[3 + time_idx], now_stamp);
				end_stamp = time_helper::get_stamp_by_hour_min(ActivityOpenTime[4 + time_idx], ActivityOpenTime[5 + time_idx], now_stamp);
				break;
			case faith::e_activity_time_get_gaming:
				begin_stamp = time_helper::get_stamp_by_hour_min(ActivityOpenTime[4 + time_idx], ActivityOpenTime[5 + time_idx], now_stamp);
				end_stamp = time_helper::get_stamp_by_hour_min(ActivityOpenTime[6 + time_idx], ActivityOpenTime[7 + time_idx], now_stamp);
				break;
			case faith::e_activity_time_get_all:
				begin_stamp = time_helper::get_stamp_by_hour_min(ActivityOpenTime[2 + time_idx], ActivityOpenTime[3 + time_idx], now_stamp);
				end_stamp = time_helper::get_stamp_by_hour_min(ActivityOpenTime[6 + time_idx], ActivityOpenTime[7 + time_idx], now_stamp);
				break;
			case faith::e_activity_time_get_notice:
				begin_stamp = time_helper::get_stamp_by_hour_min(ActivityOpenTime[0 + time_idx], ActivityOpenTime[1 + time_idx], now_stamp);
				end_stamp = time_helper::get_stamp_by_hour_min(ActivityOpenTime[2 + time_idx], ActivityOpenTime[3 + time_idx], now_stamp);
				break;
			default:
				break;
			}

			if (now_stamp > begin_stamp && now_stamp <= end_stamp)
			{
				activity_end_time = end_stamp;
				break;
			}
		}

		if (activity_end_time < 0)
		{
			return activity_end_time;
		}

		int32 left_sec = activity_end_time - now_stamp;
		return left_sec;
	}

	TArray<float> init_unit::get_excellent_att_array(const TArray<float>&  base_att_array, const faith::s_item_info& item_data)

	{
		TArray<float> return_array;
		int32 base_att_array_size = 0;
		base_att_array_size = base_att_array.size();
		return_array.reserve(faith::e_att_one_max * 6);

		if (base_att_array_size == 0
			|| base_att_array_size % faith::e_att_one_max != 0)
		{
			return return_array;
		}
		int32 total_att_num = base_att_array_size / faith::e_att_one_max;
		int32 value_info_index = e_item_info_random_property1;
		for (int32 i = 0; i < total_att_num; ++i)
		{
			if (value_info_index > e_item_info_random_property6)
			{
				break;
			}
			if (get_flag_data(item_data.data_ary[e_item_info_random_had_flag], i + 1) <= 0)
			{
				continue;
			}
			
			int32 info_start_index = i * faith::e_att_one_max;

			return_array.push_back(base_att_array[info_start_index + faith::e_att_one_level]);
			return_array.push_back(base_att_array[info_start_index + faith::e_att_one_att_id]);
			return_array.push_back(base_att_array[info_start_index + faith::e_att_one_value] * item_data.data_ary[value_info_index]);
			return_array.push_back(base_att_array[info_start_index + faith::e_att_one_percent] * item_data.data_ary[value_info_index]);
			return_array.push_back(base_att_array[info_start_index + faith::e_att_one_show]);

			value_info_index++;
		}

		return return_array;
	}
	TArray<int32> init_unit::get_excellent_color_array(
		const TArray<float>&  base_att_info_array,
		const TArray<int32>&  green_color_standard_array,
		const TArray<int32>&  blue_color_standard_array,
		const TArray<int32>&  purple_color_standard_array,
		const TArray<int32>&  orange_color_standard_array,
		const TArray<int32>&  red_color_standard_array,
		const TArray<int32>&  pink_color_standard_array,
		const faith::s_item_info& item_data)
	{
		TArray<int32> return_array;
		if (item_data.data_ary[faith::e_item_info_random_had_flag] == 0)
		{
			return return_array;
		}

		int32 base_att_array_size = 0;
		int32 green_color_array_size = 0;
		int32 blue_color_array_size = 0;
		int32 purple_color_array_size = 0;
		int32 orange_color_array_size = 0;
		int32 red_color_array_size = 0;
		int32 pink_color_array_size = 0;

		base_att_array_size = base_att_info_array.size();
		green_color_array_size = green_color_standard_array.size();
		blue_color_array_size = blue_color_standard_array.size();
		purple_color_array_size = purple_color_standard_array.size();
		orange_color_array_size = orange_color_standard_array.size();
		red_color_array_size = red_color_standard_array.size();
		pink_color_array_size = pink_color_standard_array.size();

		if (base_att_array_size == 0
			|| base_att_array_size % faith::e_att_one_max != 0
			|| green_color_array_size % 2 != 0
			|| blue_color_array_size % 2 != 0
			|| purple_color_array_size % 2 != 0
			|| orange_color_array_size % 2 != 0
			|| red_color_array_size % 2 != 0
			|| pink_color_array_size % 2 != 0)
		{
			return return_array;
		}

		int32 total_att_num = base_att_array_size / faith::e_att_one_max;
		if (total_att_num != green_color_array_size / 2
			|| total_att_num != blue_color_array_size / 2
			|| total_att_num != purple_color_array_size / 2
			|| total_att_num != orange_color_array_size / 2
			|| total_att_num != red_color_array_size / 2
			|| total_att_num != pink_color_array_size / 2)
		{
			return return_array;
		}

		int32 att_ratio_index = e_item_info_random_property1;
		for (int32 i = 0; i < total_att_num; ++i)
		{
			if (att_ratio_index > e_item_info_random_property6)
			{
				break;
			}
			if (get_flag_data(item_data.data_ary[faith::e_item_info_random_had_flag], i + 1) <= 0)
			{
				continue;
			}

			int32 max_green = green_color_standard_array[i * 2 + 1];

			int32 min_blue = blue_color_standard_array[i * 2];
			int32 max_blue = blue_color_standard_array[i * 2 + 1];

			int32 min_purple = purple_color_standard_array[i * 2];
			int32 max_purple = purple_color_standard_array[i * 2 + 1];

			int32 min_orange = orange_color_standard_array[i * 2];
			int32 max_orange = orange_color_standard_array[i * 2 + 1];

			int32 min_red = red_color_standard_array[i * 2];
			int32 max_red = red_color_standard_array[i * 2 + 1];

			int32 min_pink = pink_color_standard_array[i * 2];
			int32 max_pink = pink_color_standard_array[i * 2 + 1];

			int32 temp_real_value = item_data.data_ary[att_ratio_index];

			if (temp_real_value <= max_green)
			{
				return_array.push_back(faith::e_item_color_green);
			}
			else if(temp_real_value >= min_blue
				&& temp_real_value <= max_blue)
			{
				return_array.push_back(faith::e_item_color_blue);
			}
			else if (temp_real_value >= min_purple
				&& temp_real_value <= max_purple)
			{
				return_array.push_back(faith::e_item_color_purple);
			}
			else if (temp_real_value >= min_orange
				&& temp_real_value <= max_orange)
			{
				return_array.push_back(faith::e_item_color_orange);
			}
			else if (temp_real_value >= min_red
				&& temp_real_value <= max_red)
			{
				return_array.push_back(faith::e_item_color_red);
			}
			else if (temp_real_value >= min_pink
				&& temp_real_value <= max_pink)
			{
				return_array.push_back(faith::e_item_color_pink);
			}

			att_ratio_index++;
		}

		return return_array;
	}

	bool init_unit::is_can_use_this_jewel_slot_on_this_equip_level(int32 equip_level, int32 slot_index)
	{
		int32 array_size = 0;
		if (slot_index < faith::e_item_info_jewel_slot_0
			|| slot_index >= faith::e_item_info_jewel_vip_slot_0)
		{
			return false;
		}
		int32 temp_index = slot_index - faith::e_item_info_jewel_slot_0;
		array_size = GAMECONFIG->JewelSlotReqEquipLevel.size();

		if (temp_index >= array_size)
		{
			return false;
		}

		return equip_level >= GAMECONFIG->JewelSlotReqEquipLevel[temp_index];
	}

	bool init_unit::is_can_use_this_jewel_slot_on_this_vip_level(int32 role_vip_level, int32 slot_index)
	{
		if (slot_index < faith::e_item_info_jewel_vip_slot_0
			|| slot_index > faith::e_item_info_jewel_vip_slot_1)
		{
			return false;
		}
		int32 temp_index = slot_index - faith::e_item_info_jewel_vip_slot_0;
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(role_vip_level);
		if (vip_template_ptr == nullptr)
		{
			return false;
		}
		const VIPTemplate& vip_template = *vip_template_ptr;

		return vip_template.ExtraJewelSlotNum > temp_index;
	}

	int32 init_unit::get_jewel_slot_open_num_by_equip(int32 equip_level)
	{
		int32 return_value = 0;
		int32 array_size = 0;
		if (equip_level < 1)
		{
			return return_value;
		}

		array_size = GAMECONFIG->JewelSlotReqEquipLevel.size();

		for (int32 i = 0; i < array_size; ++i)
		{
			if (equip_level < GAMECONFIG->JewelSlotReqEquipLevel[i])
			{
				break;
			}
			return_value = i + 1;
		}
		return return_value;
	}
	int32 init_unit::get_jewel_slot_open_num_by_vip(int32 role_vip_level)
	{
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(role_vip_level);
		if (vip_template_ptr == nullptr)
		{
			return 0;
		}
		const VIPTemplate& vip_template = *vip_template_ptr;

		return vip_template.ExtraJewelSlotNum;
	}

	float init_unit::get_arena_first_high_reward_factor(int32 role_rank)
	{
		int32 array_size = 0;
		array_size = GAMECONFIG->ArenaNewHighRewardFactor.size();

		if (array_size % 3 != 0)
		{
			return 0;
		}
		int32 check_rank = role_rank + 1;
		int32 data_num = array_size / 3;
		float return_percentage = 0;
		for (int32 i = 0; i < array_size; i += 3)
		{
			int32 first_num = GAMECONFIG->ArenaNewHighRewardFactor[i];
			int32 second_num = GAMECONFIG->ArenaNewHighRewardFactor[i + 1];
			if (check_rank <= 0
				|| check_rank > rank_max)
			{
				if (second_num < 0
					|| second_num >= rank_max)
				{
					return_percentage = GAMECONFIG->ArenaNewHighRewardFactor[i + 2];
					break;
				}
			}
			else
			{
				if ((check_rank >= first_num || first_num < 0)
					&& (check_rank <= second_num || second_num < 0))
				{
					return_percentage = GAMECONFIG->ArenaNewHighRewardFactor[i + 2];
					break;
				}
			}
		}
		return return_percentage / 100;
	}

	int32 init_unit::get_cur_time_limit_branch_id_with_first_id(const s_time_limit_activity_temp& time_limit_activity_template_ptr, const std::map<int32, s_time_limit_activity_branch_temp>& branch_temp_map, int32 open_time, int32 end_time, int32 first_branch_temp_id)
	{
		if (!time_limit_activity_template_ptr.is_valid() || time_limit_activity_template_ptr.ActivityType < e_time_limit_activity_type_total_charge || time_limit_activity_template_ptr.ActivityType >= e_time_limit_activity_type_max)
		{
			return 0;
		}
		int32 cur_time = time_helper::get_cur_time_new().second;
		if (cur_time >= end_time || cur_time < open_time)
		{
			return 0;
		}
		int32 now_duation_days = (cur_time - open_time) / day_time_second;
		if (now_duation_days + 1 > time_limit_activity_template_ptr.DurationDays)
		{
			return 0;
		}
		int32 cycle_index = (now_duation_days) % (time_limit_activity_template_ptr.CycleDays) + 1;
		if (time_limit_activity_template_ptr.DurationDays < cycle_index)
		{
			return 0;
		}
		int32 cur_time_limit_activity_id = 0;
		switch (time_limit_activity_template_ptr.ActivityType)
		{
		case e_time_limit_activity_type_total_charge:
		case e_time_limit_activity_type_total_cost:
		case e_time_limit_activity_type_treasure:
		case e_time_limit_activity_type_treasure_royal1:
		case e_time_limit_activity_type_treasure_royal2:
		case e_time_limit_activity_type_treasure_royal3:
		case e_time_limit_activity_type_shop:
		case e_time_limit_activity_type_treasure_sprite:
		case e_time_limit_activity_type_item_exchange:
		case e_time_limit_activity_type_time_welfare5:
		case e_time_limit_activity_type_total_charge2:
		case e_time_limit_activity_type_behavior_welfare_reward:
		case e_time_limit_activity_type_behavior_welfare_reward1:
		case e_time_limit_activity_type_behavior_welfare_reward2:
		case e_time_limit_activity_type_behavior_welfare_reward3:
		case e_time_limit_activity_type_discount_shop:
		case e_time_limit_activity_type_discount_shop1:
		case e_time_limit_activity_type_discount_shop2:
		case e_time_limit_activity_type_discount_shop3:
		case e_time_limit_activity_type_total_landing:
		case e_time_limit_activity_type_total_cost_jewel:
		case e_time_limit_activity_type_landing_reward:
		case e_time_limit_activity_type_added_drop:
		case e_time_limit_activity_type_add_upgrade_level:
		case e_time_limit_activity_type_activity_notice_1:
		case e_time_limit_activity_type_activity_notice_2:
		case e_time_limit_activity_type_national_treasure_1:
		case e_time_limit_activity_type_national_treasure_2:
		case e_time_limit_activity_type_gs_up:
		case e_time_limit_activity_type_mount_up:
		case e_time_limit_activity_type_wing_up:
		case e_time_limit_activity_type_equip_up:
		case e_time_limit_activity_type_spirit_up:
		case e_time_limit_activity_type_cross_gs_up:
		case e_time_limit_activity_type_cross_mount_up:
		case e_time_limit_activity_type_cross_wing_up:
		case e_time_limit_activity_type_cross_equip_up:
		case e_time_limit_activity_type_cross_spirit_up:
		case e_time_limit_activity_type_treasure_element:
		case e_time_limit_activity_type_item_shop:
		case e_time_limit_activity_type_item_shop1:
		case e_time_limit_activity_type_item_shop2:
		case e_time_limit_activity_type_item_shop3:
		case e_time_limit_activity_type_dragontrip:
		case e_time_limit_activity_type_special_exchange:
		case e_time_limit_activity_type_new_total_landing:
		case e_time_limit_activity_type_new_behavior_welfare_reward:
		case e_time_limit_activity_type_sky_treasure:
		case e_time_limit_activity_type_starark:
		case e_time_limit_activity_type_weekend_top_up:
		case e_time_limit_activity_type_support_for_promotion:
		case e_time_limit_activity_type_lucky_card:
		case e_time_limit_activity_type_cross_total_charge:
		case e_time_limit_activity_type_new_total_charge_1:
		case e_time_limit_activity_type_new_total_charge_2:
		case e_time_limit_activity_type_new_total_charge_3:
		case e_time_limit_activity_type_new_total_cost_jewel_1:
		case e_time_limit_activity_type_new_total_cost_jewel_2:
		case e_time_limit_activity_type_new_total_cost_jewel_3:
		case e_time_limit_activity_type_new_server_treasure:
		case e_time_limit_activity_type_new_server_mail:
		case e_time_limit_activity_type_national_treasure_3:
		case e_time_limit_activity_type_national_treasure_4:
		case e_time_limit_activity_type_time_feed_back1:
		case e_time_limit_activity_type_time_feed_back2:
		case e_time_limit_activity_type_time_feed_back3:
		case e_time_limit_activity_type_limit_time_treasure2:
		case e_time_limit_activity_type_limit_time_notice:
		case e_time_limit_activity_type_diamond_shop:
		case e_time_limit_activity_type_star_trip:
		{
			return first_branch_temp_id;
		}
		break;
		case e_time_limit_activity_type_daily_charge:
		case e_time_limit_activity_type_time_welfare:
		case e_time_limit_activity_type_behavior_welfare:
		case e_time_limit_activity_type_behavior_welfare1:
		case e_time_limit_activity_type_behavior_welfare2:
		case e_time_limit_activity_type_behavior_welfare3:
		case e_time_limit_activity_type_time_welfare2:
		case e_time_limit_activity_type_time_welfare3:
		case e_time_limit_activity_type_time_welfare4:
		case e_time_limit_activity_type_daily_charge2:
		case e_time_limit_activity_type_daily_charge3:
		case e_time_limit_activity_type_daily_charge4:
		case e_time_limit_activity_type_daily_charge5:
		case e_time_limit_activity_type_daily_charge6:
		case e_time_limit_activity_type_small_charge:
		case e_time_limit_activity_type_cloud_shop:
		case e_time_limit_activity_type_cross_cloud_shop:
		case e_time_limit_activity_type_daily_gift_charge:
		case e_time_limit_activity_type_new_behavior_welfare:
		case e_time_limit_activity_type_daily_free_reward:
		{
			//找id最多循环100次 防止无限循环 
			for (int32 i = 0; i < 100; ++i)
			{
				auto ite = branch_temp_map.find(first_branch_temp_id + i);
				if (ite == branch_temp_map.end() || ite->second.ActivityType != time_limit_activity_template_ptr.ActivityType)
				{
					return 0;
				}
				if (ite->second.CycleIndex == cycle_index)
				{
					cur_time_limit_activity_id = first_branch_temp_id + i;
					break;
				}
			}
		}
		break;
		case e_time_limit_activity_type_word_desc:
		default:
			break;
		}
		return cur_time_limit_activity_id;
	}

	int32 init_unit::get_cur_time_limit_activity_id_by_temp(const s_time_limit_activity_temp & time_limit_act_temp, const std::map<int32,
		s_time_limit_activity_branch_temp>& branch_temp_map, int32 & open_time, int32 & end_time, int32 & cycle_num, int32 & cycle_index, int32& basics_open_time,
		int32& first_branch_temp_id, int32 cross_server_time, int32 merge_server_time, bool is_cross_server)
	{
		open_time = 0;
		end_time = 0;
		cycle_num = 0;
		cycle_index = 0;
		basics_open_time = 0;
		if (time_limit_act_temp.ActivityType < e_time_limit_activity_type_total_charge || time_limit_act_temp.ActivityType >= e_time_limit_activity_type_max)
		{
			return 0;
		}
		if (time_limit_act_temp.OpenType <= e_time_limit_activity_open_type_none
			|| time_limit_act_temp.OpenType >= e_time_limit_activity_open_type_max
			|| time_limit_act_temp.CycleParamType <= e_time_limit_activity_cycle_param_type_none
			|| time_limit_act_temp.CycleParamType >= e_time_limit_activity_cycle_param_type_max
			|| time_limit_act_temp.CycleParamNum.size() <= 0
			|| time_limit_act_temp.CycleParamNum.size() != time_limit_act_temp.CycleFirstId.size() + 1
			|| time_limit_act_temp.CycleDays <= 0
			|| time_limit_act_temp.DurationDays <= 0
			|| time_limit_act_temp.CycleDays < time_limit_act_temp.DurationDays)
		{
			return 0;
		}
		basics_open_time = get_cur_time_limit_activity_base_time_by_temp(time_limit_act_temp, cross_server_time, merge_server_time);
		
		if (basics_open_time <= 0)
		{
			return 0;
		}
		//计算周期内序号
		time_info cur_day_time = time_helper::get_today_zero_time_info();
		if (cur_day_time.second < basics_open_time)
		{
			return 0;
		}
		cycle_index = ((cur_day_time.second - basics_open_time) / day_time_second) % (time_limit_act_temp.CycleDays) + 1;
		if (time_limit_act_temp.DurationDays < cycle_index)
		{
			return 0;
		}


		//计算周期
		if (time_limit_act_temp.CycleParamType == e_time_limit_activity_cycle_param_type_num)
		{
			cycle_num = (cur_day_time.second - basics_open_time) / (time_limit_act_temp.CycleDays * day_time_second) + 1;
		}
		else
		{
			return 0;
		}

		//没找到比当前大的说明没有周期了
		int32 cycle_first_id_index = -1;
		for (int32 i = 1; i < time_limit_act_temp.CycleParamNum.size(); i++)
		{
			//遇到比我大的取前一位
			if (time_limit_act_temp.CycleParamNum[i] > cycle_num)
			{
				cycle_first_id_index = i - 1;
				break;
			}
		}

		if (cycle_first_id_index >= time_limit_act_temp.CycleFirstId.size() || cycle_first_id_index < 0)
		{
			return 0;
		}
		first_branch_temp_id = time_limit_act_temp.CycleFirstId[cycle_first_id_index];
		int32 cur_time_limit_activity_id = 0;
		switch (time_limit_act_temp.ActivityType)
		{
		case e_time_limit_activity_type_total_charge:
		case e_time_limit_activity_type_total_cost:
		case e_time_limit_activity_type_treasure:
		case e_time_limit_activity_type_treasure_royal1:
		case e_time_limit_activity_type_treasure_royal2:
		case e_time_limit_activity_type_treasure_royal3:
		case e_time_limit_activity_type_shop:
		case e_time_limit_activity_type_treasure_sprite:
		case e_time_limit_activity_type_item_exchange:
		case e_time_limit_activity_type_time_welfare5:
		case e_time_limit_activity_type_total_charge2:
		case e_time_limit_activity_type_behavior_welfare_reward:
		case e_time_limit_activity_type_behavior_welfare_reward1:
		case e_time_limit_activity_type_behavior_welfare_reward2:
		case e_time_limit_activity_type_behavior_welfare_reward3:
		case e_time_limit_activity_type_discount_shop:
		case e_time_limit_activity_type_discount_shop1:
		case e_time_limit_activity_type_discount_shop2:
		case e_time_limit_activity_type_discount_shop3:
		case e_time_limit_activity_type_total_landing:
		case e_time_limit_activity_type_total_cost_jewel:
		case e_time_limit_activity_type_landing_reward:
		case e_time_limit_activity_type_added_drop:
		case e_time_limit_activity_type_add_upgrade_level:
		case e_time_limit_activity_type_activity_notice_1:
		case e_time_limit_activity_type_activity_notice_2:
		case e_time_limit_activity_type_national_treasure_1:
		case e_time_limit_activity_type_national_treasure_2:
		case e_time_limit_activity_type_gs_up:
		case e_time_limit_activity_type_mount_up:
		case e_time_limit_activity_type_wing_up:
		case e_time_limit_activity_type_equip_up:
		case e_time_limit_activity_type_spirit_up:
		case e_time_limit_activity_type_cross_gs_up:
		case e_time_limit_activity_type_cross_mount_up:
		case e_time_limit_activity_type_cross_wing_up:
		case e_time_limit_activity_type_cross_equip_up:
		case e_time_limit_activity_type_cross_spirit_up:
		case e_time_limit_activity_type_treasure_element:
		case e_time_limit_activity_type_item_shop:
		case e_time_limit_activity_type_item_shop1:
		case e_time_limit_activity_type_item_shop2:
		case e_time_limit_activity_type_item_shop3:
		case e_time_limit_activity_type_dragontrip:
		case e_time_limit_activity_type_special_exchange:
		case e_time_limit_activity_type_new_total_landing:
		case e_time_limit_activity_type_new_behavior_welfare_reward:
		case e_time_limit_activity_type_sky_treasure:
		case e_time_limit_activity_type_starark:
		case e_time_limit_activity_type_weekend_top_up:
		case e_time_limit_activity_type_support_for_promotion:
		case e_time_limit_activity_type_lucky_card:
		case e_time_limit_activity_type_cross_total_charge:
		case e_time_limit_activity_type_new_total_charge_1:
		case e_time_limit_activity_type_new_total_charge_2:
		case e_time_limit_activity_type_new_total_charge_3:
		case e_time_limit_activity_type_new_total_cost_jewel_1:
		case e_time_limit_activity_type_new_total_cost_jewel_2:
		case e_time_limit_activity_type_new_total_cost_jewel_3:
		case e_time_limit_activity_type_new_server_treasure:
		case e_time_limit_activity_type_new_server_mail:
		case e_time_limit_activity_type_national_treasure_3:
		case e_time_limit_activity_type_national_treasure_4:
		case e_time_limit_activity_type_time_feed_back1:
		case e_time_limit_activity_type_time_feed_back2:
		case e_time_limit_activity_type_time_feed_back3:
		case e_time_limit_activity_type_limit_time_treasure2:
		case e_time_limit_activity_type_limit_time_notice:
		case e_time_limit_activity_type_diamond_shop:
		case e_time_limit_activity_type_star_trip:
		{
			cur_time_limit_activity_id = time_limit_act_temp.CycleFirstId[cycle_first_id_index];
		}
		break;
		case e_time_limit_activity_type_daily_charge:
		case e_time_limit_activity_type_time_welfare:
		case e_time_limit_activity_type_behavior_welfare:
		case e_time_limit_activity_type_behavior_welfare1:
		case e_time_limit_activity_type_behavior_welfare2:
		case e_time_limit_activity_type_behavior_welfare3:
		case e_time_limit_activity_type_time_welfare2:
		case e_time_limit_activity_type_time_welfare3:
		case e_time_limit_activity_type_time_welfare4:
		case e_time_limit_activity_type_daily_charge2:
		case e_time_limit_activity_type_daily_charge3:
		case e_time_limit_activity_type_daily_charge4:
		case e_time_limit_activity_type_daily_charge5:
		case e_time_limit_activity_type_daily_charge6:
		case e_time_limit_activity_type_small_charge:
		case e_time_limit_activity_type_cloud_shop:
		case e_time_limit_activity_type_cross_cloud_shop:
		case e_time_limit_activity_type_daily_gift_charge:
		case e_time_limit_activity_type_new_behavior_welfare:
		case e_time_limit_activity_type_daily_free_reward:
		{
			//找id最多循环100次 防止无限循环 
			for (int32 i = 0; i < 100; ++i)
			{		
				auto ite = branch_temp_map.find(time_limit_act_temp.CycleFirstId[cycle_first_id_index] + i);
				if (ite == branch_temp_map.end() || ite->second.ActivityType != time_limit_act_temp.ActivityType)
				{
					return 0;
				}
				if (ite->second.CycleIndex == cycle_index)
				{
					cur_time_limit_activity_id = time_limit_act_temp.CycleFirstId[cycle_first_id_index] + i;
					break;
				}
			}
		}
		break;
		case e_time_limit_activity_type_word_desc:
		default:
			break;
		}

		if (cur_time_limit_activity_id <= 0)
		{
			return 0;
		}
		open_time = (cycle_num - 1) * time_limit_act_temp.CycleDays * day_time_second + basics_open_time;
		end_time = time_limit_act_temp.DurationDays * day_time_second + open_time;
		if (time_limit_act_temp.ActivityType == e_time_limit_activity_type_cross_total_charge && is_cross_server == false)
		{
			open_time += 30;
			end_time -= 30;
		}
		return cur_time_limit_activity_id;
	}

	int32 init_unit::get_cur_time_limit_activity_base_time_by_temp(const s_time_limit_activity_temp& time_limit_act_temp, int32 cross_server_time, int32 merge_server_time)
	{
		if (time_limit_act_temp.ActivityType < e_time_limit_activity_type_total_charge || time_limit_act_temp.ActivityType >= e_time_limit_activity_type_max
			||time_limit_act_temp.OpenType <= e_time_limit_activity_open_type_none || time_limit_act_temp.OpenType >= e_time_limit_activity_open_type_max)
		{
			return 0;
		}

		//先计算基础开启时间
		int32 basics_open_time = 0;
		//按开服后几天
		if (time_limit_act_temp.OpenType == e_time_limit_activity_open_type_server_date)
		{
			//globle_data::get_instance().get_server_on_days()
			const tm server_open_time = globle_data::get_instance().get_server_open_tm();
			int32 server_open_time_stamp = time_helper::get_time_by_info(server_open_time.tm_year + 1900, server_open_time.tm_mon + 1, server_open_time.tm_mday).second;
			basics_open_time = server_open_time_stamp + time_limit_act_temp.OpenServerDate * day_time_second;
		}
		//按具体日期
		else if (time_limit_act_temp.OpenType == e_time_limit_activity_open_type_spencific_date)
		{
			if (time_limit_act_temp.OpenSpencificDate.size() < 3)
			{
				return 0;
			}
			basics_open_time = time_helper::get_time_by_info(time_limit_act_temp.OpenSpencificDate[0], time_limit_act_temp.OpenSpencificDate[1], time_limit_act_temp.OpenSpencificDate[2]).second;
		}
		//跨服日期
		else if (time_limit_act_temp.OpenType == e_time_limit_activity_open_type_cross_server)
		{
			int32 cross_begin_time = cross_server_time + time_limit_act_temp.OpenServerDate * day_time_second;
			basics_open_time = cross_begin_time - time_helper::get_time_by_stamp_new(cross_begin_time).second_in_day;
		}
		//合服日期
		else if (time_limit_act_temp.OpenType == e_time_limit_activity_open_type_merge_server)
		{
			int32 merge_begin_time = merge_server_time + time_limit_act_temp.OpenServerDate * day_time_second;
			basics_open_time = merge_begin_time - time_helper::get_time_by_stamp_new(merge_begin_time).second_in_day;
		}

		if (basics_open_time <= 0)
		{
			return 0;
		}

		//再计算带星期的
		if (time_limit_act_temp.FirstOpenDayOfWeek >= 0)
		{
			time_info basics_open_time_info = time_helper::get_time_by_stamp_new(basics_open_time);
			if (time_limit_act_temp.FirstOpenDayOfWeek >= basics_open_time_info.day_in_week)
			{
				basics_open_time += (time_limit_act_temp.FirstOpenDayOfWeek - basics_open_time_info.day_in_week) * day_time_second;
			}
			else
			{
				basics_open_time += (e_week_day_max + time_limit_act_temp.FirstOpenDayOfWeek - basics_open_time_info.day_in_week) * day_time_second;
			}
		}
		return basics_open_time;
	}

	void init_unit::parse_char_to_vector(std::vector<int32>& int_arr_value, const xstring& temp_str, bool is_need_add_mid_str, xstring symbol_str)
	{
		int_arr_value.clear();
		xstring mid_str = symbol_str;
		xstring _str = temp_str;
		if (is_need_add_mid_str)
		{
			_str += mid_str;
		}
		int32 len = temp_str.size();
		int32 pos = 0;
		for (int32 i = 0; i < len; i++)
		{
			pos = _str.find(mid_str, i);
			if (pos < 0)
			{
				break;
			}
			if (pos < len)
			{
				int32 k = atoi(_str.substr(i, pos - i).c_str());
				int_arr_value.push_back(k);
				i = pos;
			}
		}
	}

	void init_unit::parse_vector_to_char(xstring& temp_str, const std::vector<int32>& int_arr_value)
	{
		temp_str.clear();
		for (int32 i = 0; i < int_arr_value.size(); i++)
		{
			temp_str.append(boost::lexical_cast<xstring>(int_arr_value[i]));
			temp_str.append(boost::lexical_cast<xstring>(","));
		}
	}

	bool init_unit::is_cross_time_limit_cross_activity(int32 act_type)
	{
		switch (act_type)
		{
		case e_time_limit_activity_type_national_treasure_1:
		case e_time_limit_activity_type_national_treasure_2:
		//case e_time_limit_activity_type_national_treasure_3:
		//case e_time_limit_activity_type_national_treasure_4:
		case e_time_limit_activity_type_cross_gs_up:
		case e_time_limit_activity_type_cross_mount_up:
		case e_time_limit_activity_type_cross_wing_up:
		case e_time_limit_activity_type_cross_equip_up:
		case e_time_limit_activity_type_cross_spirit_up:
		case e_time_limit_activity_type_cross_cloud_shop:
		case e_time_limit_activity_type_cross_total_charge:
			return true;
			break;
		default:
			break;
		}
		return false;
	}

	bool init_unit::is_rank_time_limit_cross_activity(int32 act_type)
	{
		switch (act_type)
		{
		case e_time_limit_activity_type_total_charge:
		case e_time_limit_activity_type_total_cost:
		case e_time_limit_activity_type_gs_up:
		case e_time_limit_activity_type_mount_up:
		case e_time_limit_activity_type_wing_up:
		case e_time_limit_activity_type_equip_up:
		case e_time_limit_activity_type_spirit_up:
		case e_time_limit_activity_type_cross_gs_up:
		case e_time_limit_activity_type_cross_mount_up:
		case e_time_limit_activity_type_cross_wing_up:
		case e_time_limit_activity_type_cross_equip_up:
		case e_time_limit_activity_type_cross_spirit_up:
		case e_time_limit_activity_type_cross_total_charge:
			return true;
			break;
		default:
			break;
		}
		return false;
	}

	bool init_unit::is_time_limit_behavior_welfare(int32 act_type)
	{
		switch (act_type)
		{
		case e_time_limit_activity_type_behavior_welfare:
		case e_time_limit_activity_type_behavior_welfare1:
		case e_time_limit_activity_type_behavior_welfare2:
		case e_time_limit_activity_type_behavior_welfare3:
		case e_time_limit_activity_type_new_behavior_welfare:
			return true;
			break;
		default:
			break;
		}
		return false;
	}

	int32 init_unit::get_behavior_welfare_reward_type(int32 act_type)
	{
		switch (act_type)
		{
		case e_time_limit_activity_type_behavior_welfare:
			return e_time_limit_activity_type_behavior_welfare_reward;
			break;
		case e_time_limit_activity_type_behavior_welfare1:
			return e_time_limit_activity_type_behavior_welfare_reward1;
			break;
		case e_time_limit_activity_type_behavior_welfare2:
			return e_time_limit_activity_type_behavior_welfare_reward2;
			break;
		case e_time_limit_activity_type_behavior_welfare3:
			return e_time_limit_activity_type_behavior_welfare_reward3;
			break;
		case e_time_limit_activity_type_new_behavior_welfare:
			return e_time_limit_activity_type_new_behavior_welfare_reward;
			break;
		default:
			break;
		}
		return -1;
	}

	int32 init_unit::get_pk_king_max_num_by_time(e_pk_king_time_type time_type)
	{
		if (GAMECONFIG->PkKingPlayerNum.size() < e_pk_king_time_type_max)
		{
			return 0;
		}
		return GAMECONFIG->PkKingPlayerNum[time_type];
	}

	std::string init_unit::load_random_role_name()
	{
		std::string role_name = "Lucy";

		int32 first_name_index = random_gen::get_random(GAMECONFIG->CreateNameArray[0], GAMECONFIG->CreateNameArray[1]);
		int32 family_name_index = random_gen::get_random(GAMECONFIG->CreateNameArray[2], GAMECONFIG->CreateNameArray[3]);
		vector<std::string> first_name_array = template_manager::get_instance().get_string_list(first_name_index);
		vector<std::string> family_name_array = template_manager::get_instance().get_string_list(family_name_index);
		if (first_name_array.size() > 0)
		{
			role_name = first_name_array[0];
		}
		if (family_name_array.size() > 0)
		{
			role_name += family_name_array[0];
		}
		return role_name;
	}

	bool init_unit::is_no_superposition_time_limit_activity(int32 act_type)
	{
		switch (act_type)
		{
		case e_time_limit_activity_type_total_charge:
		case e_time_limit_activity_type_total_cost:
		case e_time_limit_activity_type_daily_charge:
		case e_time_limit_activity_type_treasure:
		case e_time_limit_activity_type_treasure_royal1:
		case e_time_limit_activity_type_treasure_royal2:
		case e_time_limit_activity_type_treasure_royal3:
		case e_time_limit_activity_type_shop://4
		case e_time_limit_activity_type_treasure_sprite://11
		case e_time_limit_activity_type_discount_shop://17
		case e_time_limit_activity_type_discount_shop1:
		case e_time_limit_activity_type_discount_shop2:
		case e_time_limit_activity_type_discount_shop3:
		case e_time_limit_activity_type_small_charge://18
		case e_time_limit_activity_type_added_drop://22
		case e_time_limit_activity_type_add_upgrade_level:
		case e_time_limit_activity_type_activity_notice_1:
		case e_time_limit_activity_type_activity_notice_2:
		case e_time_limit_activity_type_national_treasure_1:
		case e_time_limit_activity_type_national_treasure_2:
		case e_time_limit_activity_type_gs_up:
		case e_time_limit_activity_type_mount_up:
		case e_time_limit_activity_type_wing_up:
		case e_time_limit_activity_type_equip_up:
		case e_time_limit_activity_type_spirit_up:
		case e_time_limit_activity_type_cross_gs_up:
		case e_time_limit_activity_type_cross_mount_up:
		case e_time_limit_activity_type_cross_wing_up:
		case e_time_limit_activity_type_cross_equip_up:
		case e_time_limit_activity_type_cross_spirit_up:
		case e_time_limit_activity_type_treasure_element:
		case e_time_limit_activity_type_cloud_shop:
		case e_time_limit_activity_type_cross_cloud_shop:
		case e_time_limit_activity_type_daily_gift_charge:
		case e_time_limit_activity_type_item_shop:
		case e_time_limit_activity_type_item_shop1:
		case e_time_limit_activity_type_item_shop2:
		case e_time_limit_activity_type_item_shop3:
		case e_time_limit_activity_type_dragontrip:
		case e_time_limit_activity_type_sky_treasure:
		case e_time_limit_activity_type_starark:
		case e_time_limit_activity_type_support_for_promotion:
		case e_time_limit_activity_type_weekend_top_up:
		case e_time_limit_activity_type_lucky_card:
		case e_time_limit_activity_type_cross_total_charge:
		case e_time_limit_activity_type_new_server_treasure:
		case e_time_limit_activity_type_new_server_mail:
		case e_time_limit_activity_type_national_treasure_3:
		case e_time_limit_activity_type_national_treasure_4:
		case e_time_limit_activity_type_daily_free_reward:
		case e_time_limit_activity_type_limit_time_treasure2:
		case e_time_limit_activity_type_limit_time_notice:
		case e_time_limit_activity_type_diamond_shop:
		case e_time_limit_activity_type_star_trip:
			return true;
			break;
		default:
			break;
		}
		return false;
	}
	bool init_unit::is_big_endian()
	{
		union
		{
			char ch;
			int i;
		}un;
		un.i = 0x12345678;
		if (un.ch == 0x12)
		{
			return true;
		}
		return false;
	}
	std::vector<float> init_unit::get_equip_upgrade_att_array(int32 equip_id, int32 upgrade_num)
	{
		std::vector<float> equip_att;
		//equip_att.clear();
		//EquipTemplate* equip_template_ptr = GET_TEMPLATE(EquipTemplate, equip_id);
		//if (nullptr == equip_template_ptr || equip_template_ptr->EquipUpgradeAtt1.size() <= 0 || (equip_template_ptr->EquipUpgradeAtt1.size() % 2) != 0)
		//{
		//	return equip_att;
		//}
		//float att_base = 0;
		//for (int32 i = 0; i < equip_template_ptr->EquipUpgradeAtt1.size() / 2; ++i)
		//{
		//	if (equip_template_ptr->EquipUpgradeAtt1[i * 2] == upgrade_num)
		//	{
		//		att_base = equip_template_ptr->EquipUpgradeAtt1[i * 2 + 1];
		//		break;
		//	}
		//}
		//if (att_base > 0)
		//{
		//	for (int32 i = 0; i < equip_template_ptr->AttArray.size() / faith::e_att_one_max; ++i)
		//	{
		//		equip_att.push_back(equip_template_ptr->AttArray[i * faith::e_att_one_max + faith::e_att_one_level]);
		//		equip_att.push_back(equip_template_ptr->AttArray[i * faith::e_att_one_max + faith::e_att_one_att_id]);
		//		equip_att.push_back(upgrade_number_change(equip_template_ptr->AttArray[i * faith::e_att_one_max + faith::e_att_one_value] * att_base));
		//		equip_att.push_back(equip_template_ptr->AttArray[i * faith::e_att_one_max + faith::e_att_one_percent]);
		//		equip_att.push_back(equip_template_ptr->AttArray[i * faith::e_att_one_max + faith::e_att_one_show]);
		//	}
		//}
		return equip_att;
	}

	float init_unit::upgrade_number_change(float numerical)
	{
		float upgrade_value = 0;
		if (numerical <= 0)
		{
			upgrade_value = 0;
		}
		else if (numerical <= 1.0)
		{
			upgrade_value = 1.0;
		}
		else
		{
			upgrade_value = static_cast<float>(round(numerical));
		}
		return upgrade_value;
	}

	std::vector<int32> init_unit::reward_regional_diff(int32 template_id)
	{
		std::vector<int32> template_array;
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		RewardByRegionTemplate* config_ptr = GET_TEMPLATE(RewardByRegionTemplate, template_id);
		if (region_template_ptr == nullptr || config_ptr == nullptr)
		{
			return template_array;
		}
		int32 region_code = region_template_ptr->RegionCode;
		switch (region_code)
		{
		case e_version_region_type::e_version_region_type_china_main:
		{
			return config_ptr->ChinaMainRegionReward;
			break;
		}
		case e_version_region_type::e_version_region_type_kr:
			return config_ptr->KRRegionReward;
			break;
		case e_version_region_type::e_version_region_type_china_tw:
			return config_ptr->ChinaTWRegionReward;
			break;
		case e_version_region_type::e_version_region_type_en:
			return config_ptr->ENRegionReward;
			break;
		case e_version_region_type::e_version_region_type_tha:
			return config_ptr->THARegionReward;
			break;
		case e_version_region_type::e_version_region_type_vn:
			return config_ptr->VNRegionReward;
			break;
		case e_version_region_type::e_version_region_type_jp:
			return config_ptr->JPRegionReward;
			break;
		case e_version_region_type::e_version_region_type_rus:
			return config_ptr->RUSRegionReward;
			break;
		case e_version_region_type::e_version_region_type_sg:
			return config_ptr->SGRegionReward;
			break;
		default:
			return template_array;
			break;
		}
		return template_array;
	}
}
