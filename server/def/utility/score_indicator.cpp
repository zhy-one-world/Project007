
#include "score_indicator.h"
#include "internet/game.pb.h"
#include "world_boss_def.hpp"

namespace hld
{
	void score_indicator::clear_data()
	{
		m_score_record_list.clear();
	}

	bool score_indicator::exist_score_record(const guid_64& role_guid)
	{
		int32 temp_len = m_score_record_list.size();
		for (int32 i = 0; i < temp_len; ++i)
		{
			if (role_guid == m_score_record_list[i].role_guid)
			{
				return true;
			}
		}

		return false;
	}
	void score_indicator::add_score_record(const guid_64& role_guid, const xchar* role_name, int32 role_level, int64 role_score, int32 server_id, guid_64 legion_guid)
	{
		if (!role_guid.is_valid() || exist_score_record(role_guid))
		{
			return;
		}

		score_record new_record;
		new_record.role_guid = role_guid;
		new_record.set_role_name(role_name);
		new_record.role_level = role_level;
		new_record.score = role_score;
		new_record.server_id = server_id;
		new_record.legion_guid = legion_guid;
		m_score_record_list.push_back(new_record);
	}

	void score_indicator::del_score_record(const guid_64& role_guid)
	{
		std::vector<score_record>::iterator ite;
		for (ite = m_score_record_list.begin(); ite != m_score_record_list.end(); ++ite)
		{
			if (role_guid == ite->role_guid)
			{
				m_score_record_list.erase(ite);
				return;
			}
		}
	}

	int64 score_indicator::set_score_by_info(const guid_64& role_guid, const xchar* role_name, int32 role_level, int64 role_score /*= 0*/, int32 server_id /*= 0*/, guid_64 legion_guid /*= guid_64()*/)
	{
		score_record* personal_record = get_score_record(role_guid);
		if (nullptr == personal_record)
		{
			add_score_record(role_guid, role_name, role_level, role_score, server_id, legion_guid);
			return 0;
		}
		else
		{
			int64 temp_score = personal_record->score;
			personal_record->score = role_score;
			return temp_score;
		}
	}

	int64 score_indicator::add_score_by_info(const guid_64& role_guid, const xchar* role_name, int32 role_level, int64 add_score, int32 server_id /*= 0*/, guid_64 legion_guid /*= guid_64()*/)
	{
		score_record* personal_record = get_score_record(role_guid);
		if (nullptr == personal_record)
		{
			add_score_record(role_guid, role_name, role_level, add_score, server_id, legion_guid);
			return 0;
		}
		else
		{
			int64 temp_score = personal_record->score;
			personal_record->score += add_score;
			return temp_score;
		}
	}

	score_record* score_indicator::get_score_record_at(int32 score_list_index)
	{
		if (0 <= score_list_index && score_list_index < m_score_record_list.size())
		{
			return &(m_score_record_list[score_list_index]);
		}

		return nullptr;
	}

	score_record* score_indicator::get_score_record(const guid_64& role_guid)
	{
		for (int32 i = 0; i < m_score_record_list.size(); ++i)
		{
			if (role_guid == m_score_record_list[i].role_guid)
			{
				return &(m_score_record_list[i]);
			}
		}
		return nullptr;
	}

	score_record* score_indicator::get_score_record_and_idx(const guid_64& role_guid, int32& score_idx)
	{
		int32 temp_len = m_score_record_list.size();
		for (int32 i = 0; i < temp_len; ++i)
		{
			if (role_guid == m_score_record_list[i].role_guid)
			{
				score_idx = i + 1;
				return &(m_score_record_list[i]);
			}
		}
		return nullptr;
	}

	int32 score_indicator::get_score_record_idx(const guid_64& role_guid)
	{
		int32 temp_len = m_score_record_list.size();
		for (int32 i = 0; i < temp_len; ++i)
		{
			if (role_guid == m_score_record_list[i].role_guid)
			{
				return i;
			}
		}
		return -1;
	}

	int32 score_indicator::get_score_recore_num()
	{
		return m_score_record_list.size();
	}

	void score_indicator::set_legion_guid(const guid_64& role_guid, const guid_64& legion_guid)
	{
		if (role_guid.is_valid() && legion_guid.is_valid())
		{
			int32 temp_len = m_score_record_list.size();
			for (int32 i = 0; i < temp_len; ++i)
			{
				if (m_score_record_list[i].role_guid == role_guid)
				{
					m_score_record_list[i].legion_guid = legion_guid;
					return;
				}
			}
		}
	}

	int64 score_indicator::add_score(const guid_64& role_guid, double add_score)
	{
		int32 temp_len = m_score_record_list.size();
		for (int32 i = 0; i < temp_len; ++i)
		{
			if (m_score_record_list[i].role_guid == role_guid)
			{
				m_score_record_list[i].score += add_score;
				return m_score_record_list[i].score;
			}
		}
		return -1;
	}

	bool score_indicator::set_score(const guid_64& role_guid, int64 score)
	{
		int32 temp_len = m_score_record_list.size();
		for (int32 i = 0; i < temp_len; ++i)
		{
			if (m_score_record_list[i].role_guid == role_guid)
			{
				m_score_record_list[i].score = score;
				return true;
			}
		}
		return false;
	}

	void score_indicator::sort()
	{
		int32 temp_len = m_score_record_list.size();
		score_record temp_record;
		for (int32 i = 0; i < temp_len - 1; ++i)
		{
			bool changed = false;
			for (int32 j = 0; j < temp_len - i - 1; ++j)
			{
				if (m_score_record_list[j].score < m_score_record_list[j + 1].score)
				{
					temp_record = m_score_record_list[j];
					m_score_record_list[j] = m_score_record_list[j + 1];
					m_score_record_list[j + 1] = temp_record;
					changed = true;
				}
			}
			if (false == changed)
			{
				return;
			}
		}
	}

	// 获取伤害记录中伤害值最大的一条记录,如果没有记录则返回nullptr
	score_record* score_indicator::get_highest_score_record()
	{
		int32 temp_len = m_score_record_list.size();
		if (temp_len <= 0)
		{
			return nullptr;
		}

		score_record& record = m_score_record_list[0];
		for (int32 i = 1; i < temp_len; ++i)
		{
			if (m_score_record_list[i].score > record.score)
			{
				record = m_score_record_list[i];
			}
		}

		return &record;
	}
	
	bool score_indicator::fill_score_record_list_msg(game_proto_score_record_list& score_record_list_msg, int32 max_record_count)
	{
		if (max_record_count > sync_record_count_max)
		{
			max_record_count = sync_record_count_max;
		}

		score_record_list_msg.set_score_list_len(0);
		int32 record_count = 0;

		int32 temp_len = m_score_record_list.size();
		for (int32 i = 0; i < temp_len; ++i)
		{
			const score_record& record = m_score_record_list[i];
			if (record.role_guid.is_valid() == false)
			{
				continue;
			}

			game_proto_score_record* score_record_msg = score_record_list_msg.add_score_list();
			if (nullptr == score_record_msg)
			{
				return false;
			}

			score_record_msg->add_role_guid(record.role_guid.A);
			score_record_msg->add_role_guid(record.role_guid.B);
			score_record_msg->set_role_name(record.role_name);
			score_record_msg->set_score(record.score);
			++record_count;

			if (max_record_count >= 0 && record_count >= max_record_count)
			{
				break;
			}
		}
		score_record_list_msg.set_score_list_len(record_count);

		return true;
	}

	void score_indicator::fill_extra_score_record_msg(game_proto_score_record_list& exrea_score_record_list_msg, guid_64 need_extra_guid)
	{
		if (!need_extra_guid.is_valid())
		{
			return;
		}
		game_proto_score_record* extra_score_record_msg = exrea_score_record_list_msg.mutable_extra_score();
		if (nullptr == extra_score_record_msg)
		{
			return;
		}

		int32 rank_idx;
		score_record* record = get_score_record_and_idx(need_extra_guid, rank_idx);
		if (nullptr == record)
		{
			return;
		}
		if (record->role_guid.is_valid() == false)
		{
			return;
		}

		extra_score_record_msg->add_role_guid(record->role_guid.A);
		extra_score_record_msg->add_role_guid(record->role_guid.B);
		extra_score_record_msg->set_role_name(record->role_name);
		extra_score_record_msg->set_score(record->score);

		exrea_score_record_list_msg.set_extra_rank(rank_idx);
	}

	void score_indicator::change_legion_name_func(const guid_64& legion_guid, const xchar* legion_name)
	{
		if (false == exist_score_record(legion_guid))
		{
			return;
		}
		if (sizeof(legion_name) < 0 || legion_name == nullptr)
		{
			return;
		}
		for (int i = 0; i < m_score_record_list.size(); ++i)
		{
			if (m_score_record_list[i].role_guid == legion_guid)
			{
				memset(m_score_record_list[i].role_name, 0, sizeof(m_score_record_list[i].role_name));
				m_score_record_list[i].set_role_name(legion_name);
			}
			
		}
	}

	int64 score_indicator::add_score_record_by_list(const score_record& score_list)
	{
		return add_score_by_info(score_list.role_guid, score_list.role_name, score_list.role_level, score_list.score, score_list.server_id, score_list.legion_guid);
	}

	int64 score_indicator::set_score_record_by_list(const score_record& score_list)
	{
		return set_score_by_info(score_list.role_guid, score_list.role_name, score_list.role_level, score_list.score, score_list.server_id, score_list.legion_guid);
	}

	void score_indicator::calculate_other_score(const guid_64& role_guid, int32 kill_boss_sum_time)
	{
		if (kill_boss_sum_time <= 0)
		{
			return;
		}
		score_record* player_record = get_score_record(role_guid);
		if (nullptr != player_record)
		{
			player_record->other_score = player_record->score / kill_boss_sum_time * world_boss_add_other_score_power;
		}
	}

}