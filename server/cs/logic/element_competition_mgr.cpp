#include "logic/unit_man.h"
#include "logic/player.hpp"
#include "logic/item_set.h"
#include "element_competition_mgr.h"
#include "cell_server.hpp"
#include "internet/game.pb.h"
#include "internet/net.pb.h"
#include "internet/cs2dp.pb.h"
#include "internal/char_msg.hpp"
#include "utility/parse_msg.h"

namespace faith
{
	element_competition_mgr::element_competition_mgr()
	{
		m_array_index = 0;
	}
	element_competition_mgr::~element_competition_mgr()
	{

	}
	void element_competition_mgr::init_data()
	{
		ActivityCommonConfigTemplate*config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_element_war);
		if (nullptr == config_ptr)
		{
			return;
		}
		m_all_competition.clear();
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return;
		}
		for (auto iter = config_ptr->ParamStringArr1.begin(); iter != config_ptr->ParamStringArr1.end(); ++iter)
		{
			s_competition_time item;
			int64 beginTime = 0;
			int64 endTime = 0;
			int32 region_type = -1;
			sscanf_s((*iter).c_str(), "%d-%d-%I64d-%I64d",&region_type, &item.season, &beginTime, &endTime);
			if (region_type != region_template_ptr->RegionCode)
			{
				continue;
			}
			item.begin_time = transfer_date_to_stamp(beginTime);
			item.end_time = transfer_date_to_stamp(endTime);
			m_all_competition.push_back(item);
		}
	}
	void element_competition_mgr::clear_data()
	{
		cur_state = e_competition_manual_state_before_begining;
		m_cur_competition_info.clear_data();
		m_all_competition.clear();
		can_sync = false;
	}
	int64 element_competition_mgr::transfer_date_to_stamp(int64 date)
	{
		tm base_time;
		base_time.tm_year = date / 10000000000 - 1900;
		base_time.tm_mon = date / 100000000 % 100 - 1;
		base_time.tm_mday = date / 1000000 % 100;
		base_time.tm_hour = date / 10000 % 100;
		base_time.tm_min = date / 100 % 100;
		base_time.tm_sec = date % 100;
		return mktime(&base_time);
	}
	void element_competition_mgr::heart_tick(const int64& new_time)
	{
		check_state(new_time);
	}
	void element_competition_mgr::check_state(const int64& new_time)
	{
		switch (cur_state)
		{
			case faith::e_competition_manual_state_before_begining:
			{
				if ((new_time / 1000) >= m_cur_competition_info.competition_begin_time)
				{
					cur_state = e_competition_manual_state_begining;
					if (can_sync)
					{
						sync_competition_info();
					}
				}
			}
				break;
			case faith::e_competition_manual_state_begining:
			{
				if ((new_time / 1000) >= m_cur_competition_info.competition_end_time)
				{
					cur_state = e_competition_manual_state_over;
					if (can_sync)
					{
						sync_competition_info();
					}
				}
			}
				break;
			case faith::e_competition_manual_state_over:
			{
				calcu_cur_competition();
			}
				break;
		}
	}
	void element_competition_mgr::change_point()
	{
		if (m_cur_competition_info.level >= role_competition_max)
		{
			ElementCompetitionTemplate *competition_temp = template_manager::get_instance().get_template_by_element_competition_level(role_competition_max, cell_server::getInstance().get_ladder_world_level());
			if (nullptr == competition_temp)
			{
				return;
			}
			//100级特殊处理
			int64 overflow_num = m_cur_competition_info.cur_exp - competition_temp->exp;
			if (overflow_num > 0)
			{
				player& player_ref = unit_man::get_player(m_array_index);
				//多余的转换成天梯币
				player_ref.add_money_or_exp(e_money_type_cross_ladder_honor, overflow_num, e_server_log_add_money_element_competition);
				m_cur_competition_info.cur_exp = competition_temp->exp;
			}
		}
	}

	void element_competition_mgr::reset_competition_data()  
	{
		m_cur_competition_info.clear_reward_data();
		int32 cur_first_id = template_manager::get_instance().get_element_competition_first_id(cell_server::getInstance().get_ladder_world_level());
		ElementCompetitionTemplate *competition_temp = GET_TEMPLATE(ElementCompetitionTemplate, cur_first_id);
		if (nullptr != competition_temp)
		{
			m_cur_competition_info.level = competition_temp->Level;
			m_cur_competition_info.cur_exp = competition_temp->exp;
		}
		m_cur_competition_info.is_buy = 0;
		m_cur_competition_info.buy_manual_type = 0;
		m_cur_competition_info.point_add_percent = 0;
	}

	void element_competition_mgr::load_competition_info_end(s_role_competition_info info)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		m_cur_competition_info = info;
		m_cur_competition_info.role_guid = player_ref.get_unit_guid();
		init_data();
		if (m_cur_competition_info.season == 0)
		{
			reset_competition_data();
		}
		//计算最新的赛季
		calcu_cur_competition();
	}
	bool element_competition_mgr::load_element_competition_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_competition_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_role_competition_info db_data;
		db_data.role_guid.server_64 = msg.role_guid();
		db_data.have_type = msg.have_type();
		db_data.is_buy = msg.is_buy();
		db_data.buy_manual_type = msg.buy_manual_type();
		db_data.point_add_percent = msg.point_add_percent();
		db_data.season = msg.season();
		db_data.competition_begin_time = msg.competition_begin_time();
		db_data.competition_end_time = msg.competition_end_time();
		db_data.level = msg.level();
		db_data.cur_exp = msg.cur_exp();
		db_data.need_reset = msg.need_reset();
		db_data.assist_fight_guid.server_64 = msg.assist_fight_guid();
		for (int32 i = 0; i < msg.reward_state_size(); i++)
		{
			db_data.reward_state[i] = msg.reward_state(i);
		}

		load_competition_info_end(db_data);

		return true;
	}
		
	void element_competition_mgr::calcu_cur_competition()
	{
		can_sync = false;
		if (!cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
		{
			return;
		}
		int32 cur_time = time_helper::get_time();
		s_role_competition_info cur_season_info;
		int count = m_all_competition.size();
		if (count <= 0)
		{
			return;
		}
		for (int32 i = 0; i < count;i++)
		{
			s_competition_time& item = m_all_competition[i];
			if (cur_time >= item.begin_time && cur_time < item.end_time)
			{
				cur_season_info.season = item.season;
				cur_season_info.competition_begin_time = item.begin_time;
				cur_season_info.competition_end_time = item.end_time;
				break;
			}
		}
		if (cur_season_info.season == 0)
		{
			return;
		}
		can_sync = true;
		if (cur_season_info.season > m_cur_competition_info.season)
		{
			//结算
			change_point();
			reset_competition_data();
		}
		m_cur_competition_info.season = cur_season_info.season;
		m_cur_competition_info.competition_begin_time = cur_season_info.competition_begin_time;
		m_cur_competition_info.competition_end_time = cur_season_info.competition_end_time;
		cur_state = e_competition_manual_state_before_begining;
		sync_competition_info();
	}
	void element_competition_mgr::save_to_db(e_save_role_data_type eType)
	{
		player& player_ref = unit_man::get_player(m_array_index);

		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_role_competition_to_db);
		if (is_use == false)
		{
			cs2dp_save_role_competition_to_db  request;
			request.role_guid = player_ref.get_unit_guid();
			request.unit_array_index = m_array_index;
			request.competition_info = m_cur_competition_info;
			request.save_type_ex = eType;
			player_ref.send_message_to_dp(&request, sizeof(request));
		}
		else
		{
			faith::cs2dp_proto::save_role_competition msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(eType);

			faith::cs2dp_proto::role_competition_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			db_data->set_role_guid(m_cur_competition_info.role_guid.server_64);
			db_data->set_have_type(e_competition_type_element);
			db_data->set_is_buy(m_cur_competition_info.is_buy);
			db_data->set_buy_manual_type(m_cur_competition_info.buy_manual_type);
			db_data->set_point_add_percent(m_cur_competition_info.point_add_percent);
			db_data->set_season(m_cur_competition_info.season);
			db_data->set_competition_begin_time(m_cur_competition_info.competition_begin_time);
			db_data->set_competition_end_time(m_cur_competition_info.competition_end_time);
			db_data->set_level(m_cur_competition_info.level);
			db_data->set_cur_exp(m_cur_competition_info.cur_exp);
			db_data->set_need_reset(m_cur_competition_info.need_reset);
			db_data->set_assist_fight_guid(m_cur_competition_info.assist_fight_guid.server_64);
			for (int32 i = 0; i < role_competition_tag_num; i++)
			{
				db_data->add_reward_state(m_cur_competition_info.reward_state[i]);
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_role_competition_to_db);
		}
	}

	void element_competition_mgr::sync_competition_info()
	{
		player& player_ref = unit_man::get_player(m_array_index);

		faith::game_proto_sync_role_competition msg;
		msg.set_is_buy(m_cur_competition_info.is_buy);
		msg.set_season(m_cur_competition_info.season);
		msg.set_buy_manual_type(m_cur_competition_info.buy_manual_type);
		msg.set_competition_begin_time(m_cur_competition_info.competition_begin_time);
		msg.set_competition_end_time(m_cur_competition_info.competition_end_time);
		msg.set_level(m_cur_competition_info.level);
		msg.set_cur_exp(m_cur_competition_info.cur_exp);
		msg.set_cur_state(cur_state);
		msg.set_world_level(cell_server::getInstance().get_ladder_world_level());
		for (int32 i = 0; i < role_competition_max;i++)
		{
			int32 m_tag_char_normal = m_cur_competition_info.get_reward_tag(0, i);
			msg.add_normal_reward(m_tag_char_normal);

			int32 m_tag_char_special = m_cur_competition_info.get_reward_tag(1, i);
			msg.add_special_reward(m_tag_char_special);
		}
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_sync_element_competition);
	}

	void element_competition_mgr::get_reward(int32 id, int32 type)
	{
		if (!cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
		{
			return;
		}
		ElementCompetitionTemplate *competition_temp = GET_TEMPLATE(ElementCompetitionTemplate, id);
		if (nullptr == competition_temp)
		{
			return;
		}
		int level = competition_temp->Level;
		if (level <= 0 || level > role_competition_max)
		{
			return;
		}
		if (m_cur_competition_info.level < level)
		{
			return;
		}

		bool m_success = false;
		if (type == e_competition_manual_type_normal)
		{
			int32 m_tag_char_normal = m_cur_competition_info.get_reward_tag(0, level - 1);
			if (m_tag_char_normal == 1)
			{
				//已领
				return;
			}
			if (competition_temp->BigRewardTag < 0)
			{
				//未开放等级
				return;
			}
			//勇者
			send_reward(id,type);
			m_cur_competition_info.set_reward_tag(0, level - 1);
			m_success = true;
		}
		else if (type == e_competition_manual_type_special)
		{
			//精英
			if (m_cur_competition_info.is_buy == 0)
			{
				//提示
				return;
			}
			int32 m_tag_char_normal = m_cur_competition_info.get_reward_tag(1, level - 1);
			if (m_tag_char_normal == 1)
			{
				//已领
				return;
			}
			//勇者
			send_reward(id,type);
			m_success = true;
			m_cur_competition_info.set_reward_tag(1, level - 1);
		}
		if (m_success == true)
		{
			faith::game_proto_get_competition_lv_reward_end msg;
			msg.add_manual_id_list(id);
			msg.add_manual_type_list(type);
			msg.set_get_type(type);
			player& player_ref = unit_man::get_player(m_array_index);
			if (true == player_ref.is_valid() && player_ref.get_session_state() == e_session_status_in_gaming)
			{
				player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_element_competition_lv_reward_end);
			}
		}
	}
	void element_competition_mgr::send_reward(int32 m_id,int32 m_type)
	{
		if (!cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
		{
			return;
		}
		ElementCompetitionTemplate *competition_temp = GET_TEMPLATE(ElementCompetitionTemplate, m_id);
		if (nullptr == competition_temp)
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		if (true == player_ref.is_valid() && player_ref.get_session_state() == e_session_status_in_gaming)
		{
			std::vector<int32> m_TemPlate_ItemRewardArray;
			std::vector<int32> m_ItemRewardArray;
			int32 m_class_type = player_ref.get_unit_info(e_role_info_class_type);
			if (m_type == e_competition_manual_type_normal)
			{	
				m_TemPlate_ItemRewardArray = competition_temp->NormalRewardArray;
			}
			else if (m_type == e_competition_manual_type_special)
			{
				m_TemPlate_ItemRewardArray = competition_temp->SpecialRewardArray;
			}
			if (competition_temp->BigRewardTag == 1)
			{
				if (m_class_type * 2 <= m_TemPlate_ItemRewardArray.size())
				{
					m_ItemRewardArray.push_back(m_TemPlate_ItemRewardArray[(m_class_type - 1) * 2]);
					m_ItemRewardArray.push_back(m_TemPlate_ItemRewardArray[(m_class_type - 1) * 2 + 1]);
				}
				else
				{
					m_ItemRewardArray = m_TemPlate_ItemRewardArray;
				}
			}
			else
			{
				m_ItemRewardArray = m_TemPlate_ItemRewardArray;
			}

			//物品奖励
			std::vector<citem*> item_array;
			std::vector<s_item_template_info> promp_item_data;
			for (int32 i = 0; i * 2 + 1 < m_ItemRewardArray.size(); i++)
			{
				int32 item_template_id = m_ItemRewardArray[i * 2];
				int32 item_num = m_ItemRewardArray[i * 2 + 1];
				ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_template_id);
				if (nullptr == ItemTemplatePtr)
				{
					continue;
				}
				citem* temp_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_item_element_competition, 0, item_template_id, item_num, 1);
				if (nullptr != temp_item)
				{
					item_array.push_back(temp_item);
					promp_item_data.push_back({ item_template_id ,item_num ,temp_item->get_data_info(e_item_info_locked) > 0 });
				}
			}
			player_ref.get_item_set().put_in_bag(item_array);
			if (m_cur_competition_info.is_buy == 1 && promp_item_data.size() > 0)
			{
				player_ref.get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
			}
		}
	}
	void element_competition_mgr::sync_lv_exp()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (true == player_ref.is_valid() && player_ref.get_session_state() == e_session_status_in_gaming)
		{	
			//同步手册等级和经验
			faith::game_proto_buy_competition_lv_end msg;
			msg.set_level(m_cur_competition_info.level);
			msg.set_cur_exp(m_cur_competition_info.cur_exp);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_buy_element_competition_lv_end);
		}
	}
	void element_competition_mgr::add_competition_exp(int64 point_num)
	{
		if (!cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
		{
			return;
		}
		if (cur_state != e_competition_manual_state_begining)
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		if (true == player_ref.is_valid() && player_ref.get_session_state() == e_session_status_in_gaming)
		{
			int64 cur_exp = m_cur_competition_info.cur_exp + point_num;
			int32 begin_index = m_cur_competition_info.level - 1;
			if (begin_index < 0)
			{
				begin_index = 0;
			}
			for (; begin_index < role_competition_max; begin_index++)
			{
				ElementCompetitionTemplate *competition_temp = template_manager::get_instance().get_template_by_element_competition_level(begin_index + 1, cell_server::getInstance().get_ladder_world_level());
				if (nullptr == competition_temp)
				{
					continue;
				}
				if (cur_exp >= competition_temp->exp)
				{
					if (competition_temp->Level == role_competition_max)
					{
						m_cur_competition_info.level = role_competition_max;
						m_cur_competition_info.cur_exp = cur_exp;
						sync_lv_exp();
						break;
					}
					continue;
				}
				m_cur_competition_info.level = competition_temp->Level - 1;
				m_cur_competition_info.cur_exp = cur_exp;
				sync_lv_exp();
				break;
			}
		}	
	}
	void element_competition_mgr::get_reward_all(int32 get_type)
	{
		if (!cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
		{
			return;
		}
		int32 cur_level = m_cur_competition_info.level;
		int32 cur_first_id = template_manager::get_instance().get_element_competition_first_id(cell_server::getInstance().get_ladder_world_level());
		faith::game_proto_get_competition_lv_reward_end msg;
		for (int32 i = 0 ; i  < role_competition_max;i++)
		{
			int32 m_id = i + cur_first_id;
			ElementCompetitionTemplate *competition_temp = template_manager::get_instance().get_template_by_element_competition_level(i + 1, cell_server::getInstance().get_ladder_world_level());
			if (nullptr == competition_temp)
			{
				continue;
			}
			if (cur_level < competition_temp->Level)
			{
				break;
			}

			bool is_have = false;
			int32 m_tag_char_normal = m_cur_competition_info.get_reward_tag(0, i);
			if (m_tag_char_normal == 0)
			{
				if (competition_temp->BigRewardTag >= 0)
				{
					send_reward(m_id, e_competition_manual_type_normal);
					m_cur_competition_info.set_reward_tag(0, i);
					msg.add_manual_id_list(i + cur_first_id);
					msg.add_manual_type_list(1);
					is_have = true;
				}
			}
			if (m_cur_competition_info.is_buy == 1)
			{
				int32 m_tag_char_special = m_cur_competition_info.get_reward_tag(1, i);
				if (m_tag_char_special == 0)
				{
					//发奖
					send_reward(m_id, e_competition_manual_type_special);
					m_cur_competition_info.set_reward_tag(1, i);
					if (is_have == false)
					{
						msg.add_manual_id_list(i + cur_first_id);
						msg.add_manual_type_list(2);
					}
					else
					{
						msg.set_manual_type_list(msg.manual_type_list_size() - 1, 3);
					}
				}
			}
		}
		msg.set_get_type(get_type);
		if (msg.manual_id_list_size() > 0)
		{	
			player& player_ref = unit_man::get_player(m_array_index);
			if (true == player_ref.is_valid() && player_ref.get_session_state() == e_session_status_in_gaming)
			{
				player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_element_competition_lv_reward_end);
				sync_competition_info();
			}
		}
	}
	void element_competition_mgr::buy_competition_level(int32 level)
	{
		if (!cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
		{
			return;
		}
		if (cur_state == e_competition_manual_state_begining)
		{
			player& player_ref = unit_man::get_player(m_array_index);
			if (true == player_ref.is_valid() && player_ref.get_session_state() == e_session_status_in_gaming)
			{
				int32 cur_level = m_cur_competition_info.level;
				if (cur_level == role_competition_max)
				{
					player_ref.send_notice("90204094");
					return;
				}
				int32 aim_level = level + cur_level;
				if (aim_level > role_competition_max)
				{
					aim_level = role_competition_max;
				}

				if (GAMECONFIG->ElementCompetitionExpToMoney.size() < 2)
				{
					return;
				}
				ElementCompetitionTemplate *aim_temp = template_manager::get_instance().get_template_by_element_competition_level(aim_level, cell_server::getInstance().get_ladder_world_level());
				if (nullptr == aim_temp)
				{
					return;
				}
				int64 need_exp = aim_temp->exp - m_cur_competition_info.cur_exp;
				int64 need_money = need_exp * GAMECONFIG->ElementCompetitionExpToMoney[1];
				if (!player_ref.can_cut_money(e_money_type(GAMECONFIG->ElementCompetitionExpToMoney[0]), need_money))
				{
					return;
				}
				player_ref.cut_money(e_money_type(GAMECONFIG->ElementCompetitionExpToMoney[0]), need_money, e_server_log_cut_money_buy_element_competition_lv);
				m_cur_competition_info.level = aim_level;
				m_cur_competition_info.cur_exp = aim_temp->exp;
				sync_lv_exp();
				get_reward_all(e_competition_manual_get_reward_type_all);
			}
		}
		else
		{
			player& player_ref = unit_man::get_player(m_array_index);
			player_ref.send_notice("90202371");
		}
	}

	void element_competition_mgr::buy_speical_manual(int32 manual_type)
	{
		if (!cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
		{
			return;
		}
		if (cur_state == e_competition_manual_state_begining)
		{
			player& player_ref = unit_man::get_player(m_array_index);
			if (true == player_ref.is_valid() && player_ref.get_session_state() == e_session_status_in_gaming)
			{
				if (m_cur_competition_info.is_buy == 1 && ( m_cur_competition_info.buy_manual_type == manual_type || m_cur_competition_info.buy_manual_type >= e_competition_manual_type_special))
				{
					//已经购买精英手册
					return;
				}

				//扣蓝钻
				int32 money_num = 0;
				switch (manual_type)
				{
				case faith::e_competition_manual_type_normal:
					money_num = GAMECONFIG->ElementNormalManualByDiamond;
					break;
				case faith::e_competition_manual_type_special:
					money_num = GAMECONFIG->ElementLuxurylManualByDiamond;
					break;
				case faith::e_competition_manual_type_add_special:
					money_num = GAMECONFIG->ElementAddLuxurylManualByDiamond;
					break;
				default:
					break;
				}
				if (!player_ref.can_cut_money(e_money_type_jewel,money_num))
				{					
					return;
				}
				player_ref.cut_money(e_money_type_jewel, money_num, e_server_cut_add_money_competition);
				m_cur_competition_info.is_buy = 1;
				m_cur_competition_info.buy_manual_type = manual_type;
				//触发特权
				activati_manual(manual_type);
				faith::game_proto_buy_speical_manual_end msg;
				msg.set_is_buy(1);
				msg.set_manual_type(manual_type);
				player_ref.send_message_to_self(&msg, e_msgindex_s2c_buy_element_speical_manual_end);
				get_reward_all(e_competition_manual_get_reward_type_all);
			}
		}
		else
		{
			player& player_ref = unit_man::get_player(m_array_index);
			player_ref.send_notice("90202371");
		}
	}

	void element_competition_mgr::check_is_manual(int32 goods_id)
	{
		const RechargeTemplate* recharge_template_ptr = GET_TEMPLATE(RechargeTemplate, goods_id);
		if (recharge_template_ptr == nullptr)
		{
			return;
		}
		int32 recharge_type = recharge_template_ptr->Type;
		if (recharge_type == element_normal_manual_goods_type)
		{
			buy_speical_manual(e_competition_manual_type_normal);
		}
		else if (recharge_type == element_special_manual_goods_type)
		{
			buy_speical_manual(e_competition_manual_type_special);
		}
		else if (recharge_type == element_add_special_manual_goods_type)
		{
			buy_speical_manual(e_competition_manual_type_add_special);
		}
	}
	void element_competition_mgr::activati_manual(int32 manual_type)
	{
		//普通精英手册
		std::vector<int32> reward_list;
		if (manual_type == e_competition_manual_type_normal)
		{
			//普通精英手册
			reward_list = GAMECONFIG->ElementNormalManual;
		}
		else if (manual_type == e_competition_manual_type_special)
		{
			//豪华精英手册
			reward_list = GAMECONFIG->ElementLuxurylManual;
		}
		else if (manual_type == e_competition_manual_type_add_special)
		{
			//豪华精英手册(差价)
			reward_list = GAMECONFIG->ElementAddLuxurylManual;
		}
		if (reward_list.size()> 0 && reward_list.size() % 3 == 0)
		{
			for (int32 i = 0; i * 3 + 1 < reward_list.size(); i++)
			{
				int32 active_type = reward_list[i * 3];
				int32 active_ext0 = reward_list[i * 3 + 1];
				int32 active_ext1 = reward_list[i * 3 + 2];
				activati_manual_reward_by_type(active_type, active_ext0, active_ext1);
			}
		}
		m_cur_competition_info.buy_manual_type = manual_type;
		m_cur_competition_info.is_buy = 1;
	}

	void element_competition_mgr::activati_manual_reward_by_type(int32 active_type, int32 active_ext0, int32 active_ext1)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (true == player_ref.is_valid() && player_ref.get_session_state() == e_session_status_in_gaming)
		{
			switch ((e_manual_reward_type)active_type)
			{
				case faith::e_manual_reward_type_add_item:
				{
					std::vector<s_item_template_info> promp_item_data;
					citem* temp_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_item_element_competition, 0, active_ext0, active_ext1);
					if (temp_item != nullptr)
					{
						player_ref.get_item_set().put_in_bag(temp_item);
						promp_item_data.push_back({ active_ext0 ,active_ext1 ,temp_item->get_data_info(e_item_info_locked) > 0 });
						player_ref.get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
					}
					break;
				}
				case faith::e_manual_reward_type_manual_level:
				{
					//手册升级
					ElementCompetitionTemplate *aim_temp = template_manager::get_instance().get_template_by_element_competition_level(active_ext0, cell_server::getInstance().get_ladder_world_level());
					if (nullptr == aim_temp)
					{
						return;
					}
					player_ref.add_money(e_money_type_element_point, aim_temp->exp, e_server_log_add_money_element_competition, -1, false);
					break;
				}
				case faith::e_manual_reward_type_add_point:
				{
					//增加积分获取比例
					m_cur_competition_info.point_add_percent += active_ext0;
					break;
				}
			}
		}
	}
}