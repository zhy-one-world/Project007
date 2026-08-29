#include "log_cache.hpp"
#include "db_log_type_def.hpp"
#include "msgproc_server_log.hpp"
#include <net/scheduler.hpp>
#include "utility/random.h"
#include "time.hpp"
#include "cs_date.hpp"

#define WRITE_DB_INTERVAL		(200)
#define MAX_WRITE_LOG_COUNT		(50)

const xstring TABLE_NAMES[] = {
	TABLE_NAME_LOGOUT,
	TABLE_NAME_CREATE,
	TABLE_NAME_LEVELCHANGE,
	TABLE_NAME_TASK,
	TABLE_NAME_MONEYCHANGE,
	TABLE_NAME_ITEMCHANGE,
	TABLE_NAME_NEWGUIDE,
	TABLE_NAME_RECHARGE_STEP,
	TABLE_NAME_PLAYERCOUNT,
	TABLE_NAME_RECHARGE_SUCCESS,
	TABLE_NAME_ACCOUNT_CREATE,
	TABLE_NAME_LUCKY_DRAW_LOG,
	TABLE_NAME_AUCTION_LOG,
	TABLE_NAME_GOODS_LOG,
	TABLE_NAME_MAIL_GET_CONTENT_LOG,
	TABLE_NAME_JEWEL_COUNT_LOG,
	TABLE_NAME_TIME_LIMIT_ACTIVITY_LOG,
	TABLE_NAME_GROW_UP,
	TABLE_NAME_BOSS_KILL_LOG,
	TABLE_NAME_BOSS_KILL_DROP_LOG,
	TABLE_NAME_CROSS_SERVER_BOSS_KILL_LOG,
	TABLE_NAME_MAP_CHANGE,
	TABLE_NAME_CROSS_SERVER_HARRY,
	TABLE_NAME_OCCUPATION_PK,
	TABLE_NAME_RANK_LIST,
	TABLE_NAME_MALL,
	TABLE_NAME_LEGION_GRANT,
	TABLE_NAME_LEGION_ACTIVE,
};

const Table_Filed TABLE_FILEDS[] = {
	{ TABLE_FILED_LOGOUT, sizeof(TABLE_FILED_LOGOUT) / sizeof(xstring) },
	{ TABLE_FILED_CREATE, sizeof(TABLE_FILED_CREATE) / sizeof(xstring) },
	{ TABLE_FILED_LEVELCHANGE, sizeof(TABLE_FILED_LEVELCHANGE) / sizeof(xstring) },
	{ TABLE_FILED_TASK, sizeof(TABLE_FILED_TASK) / sizeof(xstring) },
	{ TABLE_FILED_MONEYCHANGE, sizeof(TABLE_FILED_MONEYCHANGE) / sizeof(xstring) },
	{ TABLE_FILED_ITEMCHANGE, sizeof(TABLE_FILED_ITEMCHANGE) / sizeof(xstring) },
	{ TABLE_FILED_NEWGUIDE, sizeof(TABLE_FILED_NEWGUIDE) / sizeof(xstring) },
	{ TABLE_FILED_RECHARGE_STEP, sizeof(TABLE_FILED_RECHARGE_STEP) / sizeof(xstring) },
	{ TABLE_FILED_PLAYERCOUNT, sizeof(TABLE_FILED_PLAYERCOUNT) / sizeof(xstring) },
	{ TABLE_FILED_RECHARGE_SUCCESS, sizeof(TABLE_FILED_RECHARGE_SUCCESS) / sizeof(xstring) },
	{ TABLE_FILED_ACCOUNT_CREATE, sizeof(TABLE_FILED_ACCOUNT_CREATE) / sizeof(xstring) },
	{ TABLE_FILED_LUCKY_DRAW_LOG, sizeof(TABLE_FILED_LUCKY_DRAW_LOG) / sizeof(xstring) },
	{ TABLE_FILED_AUCTION_LOG, sizeof(TABLE_FILED_AUCTION_LOG) / sizeof(xstring) },
	{ TABLE_FILED_GOODS_LOG, sizeof(TABLE_FILED_GOODS_LOG) / sizeof(xstring) },
	{ TABLE_FILED_MAIL_GET_CONTENT_LOG, sizeof(TABLE_FILED_MAIL_GET_CONTENT_LOG) / sizeof(xstring) },
	{ TABLE_FILED_JEWEL_COUNT_LOG, sizeof(TABLE_FILED_JEWEL_COUNT_LOG) / sizeof(xstring) },
	{ TABLE_FILED_TABLE_NAME_TIME_LIMIT_ACTIVITY_LOG, sizeof(TABLE_FILED_TABLE_NAME_TIME_LIMIT_ACTIVITY_LOG) / sizeof(xstring) },
	{ TABLE_FILED_GROW_UP, sizeof(TABLE_FILED_GROW_UP) / sizeof(xstring) },
	{ TABLE_FILED_TABLE_NAME_BOSS_KILL_LOG, sizeof(TABLE_FILED_TABLE_NAME_BOSS_KILL_LOG) / sizeof(xstring) },
	{ TABLE_FILED_TABLE_NAME_BOSS_KILL_DROP_LOG, sizeof(TABLE_FILED_TABLE_NAME_BOSS_KILL_DROP_LOG) / sizeof(xstring) },
	{ TABLE_FILED_TABLE_NAME_CROSS_SERVER_BOSS_KILL_LOG, sizeof(TABLE_FILED_TABLE_NAME_CROSS_SERVER_BOSS_KILL_LOG) / sizeof(xstring) },
	{ TABLE_FILED_MAP_CHANGE, sizeof(TABLE_FILED_MAP_CHANGE) / sizeof(xstring) },
	{ TABLE_FILED_CROSS_SERVER_HARRY, sizeof(TABLE_FILED_CROSS_SERVER_HARRY) / sizeof(xstring) },
	{ TABLE_FILED_OCCUPATION_PK, sizeof(TABLE_FILED_OCCUPATION_PK) / sizeof(xstring) },
	{ TABLE_FILED_RANK_LIST, sizeof(TABLE_FILED_RANK_LIST) / sizeof(xstring) },
	{ TABLE_FILED_MALL, sizeof(TABLE_FILED_MALL) / sizeof(xstring) },
	{ TABLE_FILED_LEGION_GRANT, sizeof(TABLE_FILED_LEGION_GRANT) / sizeof(xstring) },
	{ TABLE_FILED_LEGION_ACTIVE, sizeof(TABLE_FILED_LEGION_ACTIVE) / sizeof(xstring) },

};

namespace faith
{
	void log_cache::init()
	{
		for (int i = 0; i < sizeof(TABLE_NAMES) / sizeof(xstring); ++i)
		{
			m_table_fileds_map.insert(std::make_pair(TABLE_NAMES[i], &TABLE_FILEDS[i]));
			log_queue logs;
			m_table_logs_map.insert(std::make_pair(TABLE_NAMES[i], logs));
			if (TABLE_NAMES[i] == TABLE_NAME_RANK_LIST)
				m_ranking_table_index = i;
		}
		m_write_db_timerindex = net::scheduler::getInstance().add_timer(WRITE_DB_INTERVAL, boost::bind(&log_cache::write_log_cache_to_db, this));
		init_ranking_list_log_table();
	}

	log_cache::log_cache()
	{
		m_test_write_count = 0;
		m_write_db_count = 0;
		m_last_write_time = 0;
		m_write_db_timerindex = net::scheduler::scheduler_invalid_timer_index;
		m_ranking_table_index = -1;
	}

	log_cache::~log_cache()
	{
		if (m_write_db_timerindex != net::scheduler::scheduler_invalid_timer_index)
		{
			net::scheduler::getInstance().remove_timer(m_write_db_timerindex);
		}
	}

	bool log_cache::push_log_to_cache(const xstring& table_name, const xstring& log_string)
	{
		table_logs_map_it it = m_table_logs_map.find(table_name);
		if (it != m_table_logs_map.end())
		{
			it->second.push(log_string);
			return true;
		}
		return false;
	}

	void log_cache::write_log_cache_to_db()
	{
		int64 time_now = utility::get_tick_count();
		int64 time_pass = time_now - m_last_write_time;
		table_logs_map_it it = m_table_logs_map.begin();
		bool bWrite = false;
		for (; it != m_table_logs_map.end(); ++it)
		{
			if (it->second.size() >= MAX_WRITE_LOG_COUNT || (it->second.size() > 0 && time_pass > 5 * WRITE_DB_INTERVAL))
			{
				table_fileds_map_it filed_it = m_table_fileds_map.find(it->first);
				if (filed_it == m_table_fileds_map.end())
				{
					continue;
				}

				sql_builder sql(db_manager::getInstance().get_db_link().log_db);
				xstring table_name = it->first;
				sql << "INSERT INTO " << table_name.c_str() << " (";

				for (int i = 0; i < filed_it->second->filed_len; ++i)
				{
					if (i == 0)
					{
						sql << (filed_it->second->fileds)[i];
					}
					else
					{
						sql << "," << (filed_it->second->fileds)[i];
					}
				}
				sql << ") VALUES ";
				int count = 0;
				while (it->second.size() > 0 && count < MAX_WRITE_LOG_COUNT)
				{
					xstring log_string = it->second.front();
					it->second.pop();
					count++;
					Json::Reader reader;
					Json::Value  json_value;
					xstring insert_value = "";
					if (reader.parse(log_string, json_value))
					{
						sql << "(";
						for (int i = 0; i < filed_it->second->filed_len; ++i)
						{
							if (i != 0)
							{
								sql << ",";
							}
							if (json_value[(filed_it->second->fileds)[i]].isNull())
							{
								sql << "NULL";
							}
							else
							{
								sql << "'" << json_value[(filed_it->second->fileds)[i]].asString() << "'";
							}
						}
						if (it->second.size() > 0 && count < MAX_WRITE_LOG_COUNT)
						{
							sql << "),";
						}
						else
						{
							sql << ")";
						}
					}
					m_write_db_count++;
				}
				db_query_type query;
				query.sql_str.assign(sql.c_str(), sql.size());
				//std::cout << query.sql_str << std::endl;
				query.handler = boost::bind(&log_cache::write_log_cache_to_db_end, this, _1, query.sql_str);
				db_manager::getInstance().get_db_link().log_db.add_query(query);
				bWrite = true;
			}
		}
		if (bWrite)
		{
			m_last_write_time = time_now;
		}
	}

	void log_cache::write_log_cache_to_db_end(db_result_type result, xstring sql_str)
	{
		if (result.error)
		{
#ifdef _DEBUG
			FAITH_LOG_INFO(server_log::get_server_log(), _XTEXT("write_log_cache_to_db_end error:") << result.error);
			FAITH_LOG_INFO(server_log::get_server_log(), _XTEXT("sql_str:") << sql_str);
#endif
		}
	}

	void log_cache::test_write_log()
	{
		static int day = 0;
		int account_count = 10;
		int recharge_count = 1;
		int recharge_prob = 50;
		if (day >= 15)
		{
			return;
		}
		static std::map<int, int> first_recharge_map;
		for (int account_idx = 1; account_idx <= (day + 1) * account_count; account_idx++)
		{
			if (account_idx > day * account_count && account_idx <= (day + 1) * account_count)
			{
				test_write_create_role_log(day, account_idx);
			}
			test_write_loginout_log(day, account_idx);
			for (int i = 0; i < recharge_count; i++)
			{
				int32 random_value = random_gen::get_random(1, 100);
				if (random_value <= recharge_prob)
				{
					int is_first = 0;
					if (first_recharge_map.find(account_idx) == first_recharge_map.end())
					{
						is_first = 1;
					}
					test_write_recharge_success_log(day, account_idx, is_first);
					if (is_first == 1)
					{
						first_recharge_map.insert(std::make_pair(account_idx, 1));
					}
				}
			}
			
// 			test_write_level_change_log();
// 			test_write_task_log();
// 			test_write_money_change_log();
// 			test_write_new_guide_log();
// 			test_write_recharge_step_log();
// 			test_write_player_count_log();
			
			m_test_write_count++;
			// FAITH_LOG_INFO(server_log::get_server_log(), _XTEXT("test_write_log count:") << m_test_write_count);
		}
		day++;
		CONSOLE_INFO("day:{}", day);
	}

	void log_cache::test_write_loginout_log(int day, int account_idx)
	{
		xchar temp[32] = "";
		time_info cur_time_info = time_helper::get_cur_time_new();
		sprintf_s(temp, sizeof(temp), "%d-%02d-%02d %02d:%02d:%02d"
			, cur_time_info.year, cur_time_info.month_in_year + 1, cur_time_info.day_in_month + 1 + day
			, cur_time_info.hour_in_day, cur_time_info.minute_in_hour, cur_time_info.second_in_minute);
		xstring cur_time = temp;

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		xchar account_id[128] = "";
		sprintf_s(account_id, sizeof(account_id), "account_id%d", account_idx);
		json["account_id"] = account_id;
		xchar device_id[128] = "";
		sprintf_s(device_id, sizeof(device_id), "device_id%d", account_idx);
		json["device_id"] = device_id;
		xchar role_id[128] = "";
		sprintf_s(role_id, sizeof(role_id), "role_id%d", account_idx);
		json["role_id"] = role_id;
		xchar role_name[128] = "";
		sprintf_s(role_name, sizeof(role_name), "role_name%d", account_idx);
		json["role_name"] = role_name;
		json["role_level"] = 10;
		json["role_power"] = 10000;

		json["ip"] = "192.168.0.79";
		json["op_type"] = 0;
		json["create_time"] = cur_time;
		json["online_time"] = 1000000;

		xstring str_log = writer.write(json);

		faith::server2dp_save_log req;
		memcpy(req.sql_table_name, TABLE_NAME_LOGOUT.c_str(), TABLE_NAME_LOGOUT.size());
		int32 cpy_size = str_log.size() > max_log_char_num ? max_log_char_num : str_log.size();
		memcpy(req.save_string, str_log.c_str(), cpy_size);

		server2dp_req_save_log_info(0, &req, sizeof(server2dp_save_log));
	}

	void log_cache::test_write_create_role_log(int day, int account_idx)
	{
		xchar temp[32] = "";
		time_info cur_time_info = time_helper::get_cur_time_new();
		sprintf_s(temp, sizeof(temp), "%d-%02d-%02d %02d:%02d:%02d"
			, cur_time_info.year, cur_time_info.month_in_year + 1, cur_time_info.day_in_month + 1 + day
			, cur_time_info.hour_in_day, cur_time_info.minute_in_hour, cur_time_info.second_in_minute);
		xstring cur_time = temp;

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		xchar account_id[128] = "";
		sprintf_s(account_id, sizeof(account_id), "account_id%d", account_idx);
		json["account_id"] = account_id;
		xchar device_id[128] = "";
		sprintf_s(device_id, sizeof(device_id), "device_id%d", account_idx);
		json["device_id"] = device_id;
		xchar role_id[128] = "";
		sprintf_s(role_id, sizeof(role_id), "role_id%d", account_idx);
		json["role_id"] = role_id;
		xchar role_name[128] = "";
		sprintf_s(role_name, sizeof(role_name), "role_name%d", account_idx);
		json["role_name"] = role_name;
		json["role_level"] = 10;
		json["role_power"] = 10000;

		json["ip"] = "192.168.0.79";
		json["career"] = "career";
		json["create_time"] = cur_time;
		json["online_time"] = 1000000;

		xstring str_log = writer.write(json);

		faith::server2dp_save_log req;
		memcpy(req.sql_table_name, TABLE_NAME_CREATE.c_str(), TABLE_NAME_CREATE.size());
		int32 cpy_size = str_log.size() > max_log_char_num ? max_log_char_num : str_log.size();
		memcpy(req.save_string, str_log.c_str(), cpy_size);

		server2dp_req_save_log_info(0, &req, sizeof(server2dp_save_log));
	}

	void log_cache::test_write_level_change_log()
	{
		xstring cur_time = time_helper::get_current_time();

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["account_id"] = "account_id";
		json["device_id"] = "device_id";
		json["role_id"] = "role_id";
		json["role_name"] = "role_name";
		json["role_level"] = 10;
		json["role_power"] = 10000;

		json["old_level"] = 11;
		json["new_level"] = 12;
		json["online_time"] = 1000000;

		xstring str_log = writer.write(json);

		faith::server2dp_save_log req;
		memcpy(req.sql_table_name, TABLE_NAME_LEVELCHANGE.c_str(), TABLE_NAME_LEVELCHANGE.size());
		int32 cpy_size = str_log.size() > max_log_char_num ? max_log_char_num : str_log.size();
		memcpy(req.save_string, str_log.c_str(), cpy_size);

		server2dp_req_save_log_info(0, &req, sizeof(server2dp_save_log));
	}

	void log_cache::test_write_task_log()
	{
		xstring cur_time = time_helper::get_current_time();

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["account_id"] = "account_id";
		json["device_id"] = "device_id";
		json["role_id"] = "role_id";
		json["role_name"] = "role_name";
		json["role_level"] = 10;
		json["role_power"] = 10000;

		json["op_type"] = 0;
		json["task_type"] = 1;
		json["task_id"] = "100001";

		xstring str_log = writer.write(json);

		faith::server2dp_save_log req;
		memcpy(req.sql_table_name, TABLE_NAME_TASK.c_str(), TABLE_NAME_TASK.size());
		int32 cpy_size = str_log.size() > max_log_char_num ? max_log_char_num : str_log.size();
		memcpy(req.save_string, str_log.c_str(), cpy_size);

		server2dp_req_save_log_info(0, &req, sizeof(server2dp_save_log));
	}

	void log_cache::test_write_money_change_log()
	{
		xstring cur_time = time_helper::get_current_time();

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["account_id"] = "account_id";
		json["device_id"] = "device_id";
		json["role_id"] = "role_id";
		json["role_name"] = "role_name";
		json["role_level"] = 10;
		json["role_power"] = 10000;

		json["role_vip"] = 3;
		json["money_type"] = 1;
		json["old_money"] = 20000;
		json["new_money"] = 30000;
		json["op_type"] = 0;
		json["cause_id"] = "cause_id";
		json["location"] = "location";

		xstring str_log = writer.write(json);

		faith::server2dp_save_log req;
		memcpy(req.sql_table_name, TABLE_NAME_MONEYCHANGE.c_str(), TABLE_NAME_MONEYCHANGE.size());
		int32 cpy_size = str_log.size() > max_log_char_num ? max_log_char_num : str_log.size();
		memcpy(req.save_string, str_log.c_str(), cpy_size);

		server2dp_req_save_log_info(0, &req, sizeof(server2dp_save_log));
	}

	void log_cache::test_write_new_guide_log()
	{
		xstring cur_time = time_helper::get_current_time();

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["account_id"] = "account_id";
		json["device_id"] = "device_id";
		json["role_id"] = "role_id";
		json["role_name"] = "role_name";
		json["role_level"] = 10;
		json["role_power"] = 10000;
		json["step_id"] = "100001";

		xstring str_log = writer.write(json);

		faith::server2dp_save_log req;
		memcpy(req.sql_table_name, TABLE_NAME_NEWGUIDE.c_str(), TABLE_NAME_NEWGUIDE.size());
		int32 cpy_size = str_log.size() > max_log_char_num ? max_log_char_num : str_log.size();
		memcpy(req.save_string, str_log.c_str(), cpy_size);

		server2dp_req_save_log_info(0, &req, sizeof(server2dp_save_log));
	}

	void log_cache::test_write_recharge_step_log()
	{
		xstring cur_time = time_helper::get_current_time();

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["order_id"] = "order_id";
		json["role_id"] = "role_id";
		json["goods_id"] = "goods_id";
		json["step_num"] = 1;

		xstring str_log = writer.write(json);

		faith::server2dp_save_log req;
		memcpy(req.sql_table_name, TABLE_NAME_RECHARGE_STEP.c_str(), TABLE_NAME_RECHARGE_STEP.size());
		int32 cpy_size = str_log.size() > max_log_char_num ? max_log_char_num : str_log.size();
		memcpy(req.save_string, str_log.c_str(), cpy_size);

		server2dp_req_save_log_info(0, &req, sizeof(server2dp_save_log));
	}

	void log_cache::test_write_player_count_log()
	{
		xstring cur_time = time_helper::get_current_time();

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["player_count"] = 2000;

		xstring str_log = writer.write(json);

		faith::server2dp_save_log req;
		memcpy(req.sql_table_name, TABLE_NAME_PLAYERCOUNT.c_str(), TABLE_NAME_PLAYERCOUNT.size());
		int32 cpy_size = str_log.size() > max_log_char_num ? max_log_char_num : str_log.size();
		memcpy(req.save_string, str_log.c_str(), cpy_size);

		server2dp_req_save_log_info(0, &req, sizeof(server2dp_save_log));
	}

	void log_cache::test_write_recharge_success_log(int day, int account_idx, int is_first)
	{
		xchar temp[32] = "";
		time_info cur_time_info = time_helper::get_cur_time_new();
		sprintf_s(temp, sizeof(temp), "%d-%02d-%02d %02d:%02d:%02d"
			, cur_time_info.year, cur_time_info.month_in_year + 1, cur_time_info.day_in_month + 1 + day
			, cur_time_info.hour_in_day, cur_time_info.minute_in_hour, cur_time_info.second_in_minute);
		xstring cur_time = temp;

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		xchar account_id[128] = "";
		sprintf_s(account_id, sizeof(account_id), "account_id%d", account_idx);
		json["account_id"] = account_id;
		xchar device_id[128] = "";
		sprintf_s(device_id, sizeof(device_id), "device_id%d", account_idx);
		json["device_id"] = device_id;
		xchar role_id[128] = "";
		sprintf_s(role_id, sizeof(role_id), "role_id%d", account_idx);
		json["role_id"] = role_id;
		xchar role_name[128] = "";
		sprintf_s(role_name, sizeof(role_name), "role_name%d", account_idx);
		json["role_name"] = role_name;
		json["role_level"] = 10;
		json["role_power"] = 10000;
		json["ip"] = "127.0.0.1";
		json["role_vip"] = 3;
		json["game_order_id"] = "game_order_id";
		json["game_channel_order_id"] = "game_channel_order_id";
		json["order_amount"] = 648;
		json["share_amount"] = 0;
		json["no_share_amount"] = 0;
		json["pay_id"] = 0;
		json["recharge_channel"] = "recharge_channel";
		json["add_jewel"] = 100;
		json["total_jewel"] = 1000;
		json["currency_type"] = 1;
		json["item_id"] = 100010;
		json["is_first_recharge"] = is_first;
		json["device_model"] = "device_model";
		json["android_id"] = "android_id";

		xstring str_log = writer.write(json);

		faith::server2dp_save_log req;
		memcpy(req.sql_table_name, TABLE_NAME_RECHARGE_SUCCESS.c_str(), TABLE_NAME_RECHARGE_SUCCESS.size());
		int32 cpy_size = str_log.size() > max_log_char_num ? max_log_char_num : str_log.size();
		memcpy(req.save_string, str_log.c_str(), cpy_size);

		server2dp_req_save_log_info(0, &req, sizeof(server2dp_save_log));
	}

	void log_cache::init_ranking_list_log_table()
	{
		if (m_ranking_table_index == -1)
			return;

		//动态增加每个排行榜结构
		for (int32 i = 0; i < e_RankingIndex_max; ++i)
		{
			std::string tmp_table_name = TABLE_NAMES[m_ranking_table_index] + "_";
			std::string rank_type = "";
			std::stringstream ss;
			ss << i;
			ss >> rank_type;
			tmp_table_name = tmp_table_name + rank_type;
			m_table_fileds_map.insert(std::make_pair(tmp_table_name, &TABLE_FILEDS[m_ranking_table_index]));
			log_queue logs;
			m_table_logs_map.insert(std::make_pair(tmp_table_name, logs));
		}

		//刷新数据库中的表格
		sql_builder sql(db_manager::getInstance().get_db_link().log_db);
		sql << _XTEXT("CALL sp_init_ranking_list_log_table(")
			<< e_RankingIndex_max
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		boost::bind(&log_cache::init_ranking_list_log_table_end, this, _1);
		db_manager::getInstance().get_db_link().log_db.add_query(query);
	}

	void log_cache::init_ranking_list_log_table_end(db_result_type result)
	{
		if (result.error)
		{
			CONSOLE_INFO("init_ranking_list_log_table error:{}", result.error);
			return;
		}
	}
}