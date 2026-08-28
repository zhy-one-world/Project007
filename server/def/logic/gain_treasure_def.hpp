/********************************************************************
created: 2018年5月5日
file base: gain_treasure_def
file ext: hpp
author: wangdi

purpose: 打宝相关
*********************************************************************/
#ifndef _GAIN_TREASURE_DEF_HPP_
#define _GAIN_TREASURE_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "team_def.hpp"

namespace hld
{
#pragma pack(push,1)
	const int32 gain_treasure_gain_item_max_num = 5;//单个boss最高掉落需要记录的物品数量
	const int32 gain_treasure_killed_record_one_boss_max_size = 5;
	const int32 gain_treasure_boss_max_size = 150;//最大boss数量
	const int32 gain_treasure_gain_item_id_max_size = gain_treasure_gain_item_max_num * (item_template_id_length + 2);
	const int32 gain_treasure_record_info_max_size = gain_treasure_boss_max_size * gain_treasure_killed_record_one_boss_max_size;
	const int32	gain_treasure_gain_item_record_max_size = 30;
	const int32 one_gain_item_record_list_get_num = 30;
	const int32 first_boss_field_map_id = 71190001;
	const int32 begin_boss_sing_map_id = 71170001;
	const int32 end_boss_field_map_id = 71170020;


	enum e_buy_simple_boss_num_error
	{
		e_buy_simple_boss_num_error_unkonwn,		// 未知错误
		e_buy_simple_boss_num_error_lack_money,		// 货币不足
		e_buy_simple_boss_num_error_lack_num,		// 无购买次数
		e_buy_simple_boss_num_error_succeed,		// 购买成功
		e_buy_simple_boss_num_error_max,
	};

	struct s_gain_treasure_info
	{
		int32								data_ary[e_gain_treasure_info_type_max];
		s_gain_treasure_info()
		{
			clear_data();
		}
		void clear_data()
		{
			for (int32 i = 0; i < e_gain_treasure_info_type_max; i++)
			{
				data_ary[i] = 0;
			}
		}
	};

	struct s_gain_treasure_info_db
	{
		guid_64								role_guid;
		s_gain_treasure_info				data_info;
		s_gain_treasure_info_db()
		{
			clear_data();
		}
		void clear_data()
		{
			role_guid.clear_data();
			data_info.clear_data();
		}
	};

	struct s_gain_treasure_record_player_info_db
	{
		xchar								role_name[max_name_size + 1];
		xchar								gain_item_template_id[gain_treasure_gain_item_id_max_size + 1];
		guid_64								role_guid;
		s_gain_treasure_record_player_info_db()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_gain_treasure_record_player_info_db));
		}
		void to_proto(hld::st_proto::st_gain_treasure_record_player_info_db* st_player_ptr)
		{
			if (st_player_ptr == nullptr)
			{
				return;
			}
			st_player_ptr->set_role_name(role_name);
			st_player_ptr->set_gain_item_template_id(gain_item_template_id);
			st_player_ptr->set_role_guid(role_guid.server_64);
		}
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const hld::st_proto::st_gain_treasure_record_player_info_db& st_player_ref)
		{
			my_memcopy_string(role_name, max_name_size, st_player_ref.role_name());
			my_memcopy_string(gain_item_template_id, gain_treasure_gain_item_id_max_size, st_player_ref.gain_item_template_id());
			role_guid.server_64 = st_player_ref.role_guid();


		}
	};

	struct s_gain_treasure_record_info_one_db
	{
		int32									boss_spawn_point_template_id;
		int32									killed_stamp;
		s_gain_treasure_record_player_info_db	player_info_arr[gain_treasure_gain_team_num];
		s_gain_treasure_record_info_one_db()
		{
			clear_data();
		}
		void clear_data()
		{
			boss_spawn_point_template_id = 0;
			killed_stamp = 0;
			for (int32 i = 0; i < gain_treasure_gain_team_num; i++)
			{
				player_info_arr[i].clear_data();
			}
		}
		bool to_proto(hld::st_proto::st_gain_treasure_record_info_one_db* st_treasure_ptr)
		{
			if (st_treasure_ptr == nullptr)
			{
				return false;
			}
			st_treasure_ptr->set_boss_spawn_point_template_id(boss_spawn_point_template_id);
			st_treasure_ptr->set_killed_stamp(killed_stamp);
			for (int32 i = 0; i < gain_treasure_gain_team_num; i++)
			{
				hld::st_proto::st_gain_treasure_record_player_info_db * st_player_ptr = st_treasure_ptr->add_player_info_arr();
				if (st_player_ptr == nullptr)
				{
					return false;
				}
				player_info_arr[i].to_proto(st_player_ptr);
			}
			return true;
		}
		void from_proto(const hld::st_proto::st_gain_treasure_record_info_one_db& st_treasure_ref)
		{
			boss_spawn_point_template_id = st_treasure_ref.boss_spawn_point_template_id();
			killed_stamp = st_treasure_ref.killed_stamp();
			for (int32 i = 0; i < gain_treasure_gain_team_num && i < st_treasure_ref.player_info_arr_size(); i++)
			{
				player_info_arr[i].from_proto(st_treasure_ref.player_info_arr(i));
			}

		}
	};

	struct s_gain_treasure_record_player_info
	{
		xchar								role_name[max_name_size + 1];
		int32								gain_item_template_id[gain_treasure_gain_item_max_num];
		guid_64								role_guid;
		s_gain_treasure_record_player_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_gain_treasure_record_player_info));
		}
		void to_proto(hld::st_proto::st_gain_treasure_record_player_info* st_player_ptr)
		{
			if (st_player_ptr == nullptr)
			{
				return;
			}
			st_player_ptr->set_role_name(role_name);
			for (int32 i = 0; i < gain_treasure_gain_item_max_num; i++)
			{
				st_player_ptr->add_gain_item_template_id(gain_item_template_id[i]);
			}
			st_player_ptr->set_role_guid(role_guid.server_64);
		}
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const hld::st_proto::st_gain_treasure_record_player_info& st_player_ref)
		{
			my_memcopy_string(role_name, max_name_size, st_player_ref.role_name());
			for (int32 i = 0; i < gain_treasure_gain_item_max_num && i < st_player_ref.gain_item_template_id_size(); i++)
			{
				gain_item_template_id[i] = st_player_ref.gain_item_template_id(i);

			}
			role_guid.server_64 = st_player_ref.role_guid();


		}
	};

	struct s_gain_treasure_record_player_info_one_things
	{
		xchar								role_name[max_name_size + 1];
		int32								gain_item_template_id;
		guid_64								role_guid;
		s_gain_treasure_record_player_info_one_things()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_gain_treasure_record_player_info_one_things));
		}
	};

	struct s_gain_treasure_record_info_one
	{
		int32								boss_spawn_point_template_id;
		int32								killed_stamp;
		s_gain_treasure_record_player_info	player_info_arr[max_team_member_num];
		s_gain_treasure_record_info_one()
		{
			clear_data();
		}
		void clear_data()
		{
			boss_spawn_point_template_id = 0;
			killed_stamp = 0;
			for (int32 i = 0; i < max_team_member_num; i++)
			{
			player_info_arr[i].clear_data();
			}
		}
		void to_proto(hld::st_proto::st_gain_treasure_record_info_one* st_treasure_ptr)
		{
			if (st_treasure_ptr == nullptr)
			{
				return;
			}
			st_treasure_ptr->set_boss_spawn_point_template_id(boss_spawn_point_template_id);
			st_treasure_ptr->set_killed_stamp(killed_stamp);
			for (int32 i = 0; i < max_team_member_num; i++)
			{
				hld::st_proto::st_gain_treasure_record_player_info *st_player_ptr = st_treasure_ptr->add_player_info_arr();
				if (st_player_ptr == nullptr)
				{
					return;
				}
				player_info_arr[i].to_proto(st_player_ptr);
			}
		}
	};

	struct s_gain_treasure_record_info_all
	{
		std::list<s_gain_treasure_record_info_one>		record_info_list;
		s_gain_treasure_record_info_all()
		{
			clear_data();
		}
		void clear_data()
		{
			record_info_list.clear();
		}
		//增加记录 如果记录已满 返回需要删除记录的时间戳
		int32 add_record(const s_gain_treasure_record_info_one& record_info)
		{
			s_gain_treasure_record_info_one add_record_info = record_info;
			record_info_list.push_front(add_record_info);
			int32 del_stamp = 0;
			if (record_info_list.size() > gain_treasure_killed_record_one_boss_max_size)
			{
				del_stamp = record_info_list.rbegin()->killed_stamp;
				record_info_list.pop_back();
			}
			return del_stamp;
		}

		bool is_valid()
		{
			return !record_info_list.empty();
		}
	};

	struct s_gain_treasure_gain_item_record_info
	{
		xchar											role_name[max_name_size + 1];
		int32											boss_spawn_point_template_id;
		int32											item_template_id;
		int32											killed_stamp;
		int32											record_index;
		guid_64											role_guid;
		s_gain_treasure_gain_item_record_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_gain_treasure_gain_item_record_info));
		}
		void set_role_name(const xstring& p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
	};

	struct s_gain_treasure_player_info
	{
		guid_64											player_guid;
		xchar											player_name[max_name_size + 1];
		s_gain_treasure_player_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_gain_treasure_player_info));
		}
		void set_role_name(const xstring& p_role_name)
		{
			memset(player_name, 0, sizeof(player_name));
			memcpy(player_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
		void to_proto(hld::st_proto::st_gain_treasure_player_info* st_player_ptr)
		{
			if (st_player_ptr == nullptr)
			{
				return;
			}
			st_player_ptr->set_player_guid(player_guid.server_64);
			st_player_ptr->set_player_name(player_name);
		}
		void my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const hld::st_proto::st_gain_treasure_player_info& st_player_ref)
		{
			player_guid.server_64 = st_player_ref.player_guid();
			my_memcopy_string(player_name, max_name_size, st_player_ref.player_name());
		}
	};

	struct s_gain_treasure_boss_info
	{
		int32											boss_spawn_point_template_id;
		guid_64											boss_guid;							//boss死亡时guid为0
		int32											next_refresh_stamp;
		s_gain_treasure_player_info						player_info[max_team_member_num];
		s_gain_treasure_boss_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_gain_treasure_boss_info));
		}
		void to_proto(hld::st_proto::st_gain_treasure_boss_info* st_boss_ptr)
		{
			if (st_boss_ptr == nullptr)
			{
				return;
			}
			st_boss_ptr->set_boss_spawn_point_template_id(boss_spawn_point_template_id);
			st_boss_ptr->set_boss_guid(boss_guid.server_64);
			st_boss_ptr->set_next_refresh_stamp(next_refresh_stamp);
			for (int32 i = 0; i < max_team_member_num; i++)
			{
				hld::st_proto::st_gain_treasure_player_info * st_player_ptr = st_boss_ptr->add_player_info();
				if (st_player_ptr == nullptr)
				{
					return;
				}
				player_info[i].to_proto(st_player_ptr);
			}
		}
		void from_proto(const hld::st_proto::st_gain_treasure_boss_info& st_boss_ref)
		{
			boss_spawn_point_template_id = st_boss_ref.boss_spawn_point_template_id();
			boss_guid.server_64 = st_boss_ref.boss_guid();
			next_refresh_stamp = st_boss_ref.next_refresh_stamp();
			for (int32 i = 0; i < max_team_member_num && i < st_boss_ref.player_info_size(); i++)
			{
				player_info[i].from_proto(st_boss_ref.player_info(i));
			}
		}
	};

	typedef std::map<int32, s_gain_treasure_record_info_all> gain_treasure_record_lists_type;
	typedef std::map<int32, s_gain_treasure_boss_info>	gain_treasure_boss_infos_type;
#pragma pack(pop)
}

#endif
