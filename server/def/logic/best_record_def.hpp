#ifndef _BEST_RECORD_DEF_HPP_
#define _BEST_RECORD_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"

namespace hld
{
#pragma pack(push,1)

	const int32 max_best_record_num_all = 1000;
	const int32 max_best_record_num = 50;
	const int32 broken_sky_extra_award_player_num = 5;		//破碎虚空活动中会获得额外奖励的玩家的个数
	const int32 single_map_record_num = 5;					//每个地图的记录数量
	const int32 exp_best_record_hundred_million_notice_id = 93000252; //经验副本新纪录公告 经验值过亿

	enum e_best_record_order
	{
		e_best_record_order_none,		// 无
		e_best_record_order_by_bigger,	// 值更大的记录是更好的记录
		e_best_record_order_by_smaller,	// 值更小的记录是更好的记录
		e_best_record_order_max
	};

	struct s_single_map_record
	{
		guid_64	role_guid;
		xchar	role_name[max_name_size_one_team + 1];
		int32	value;
		void set_role_name(std::string _name)
		{
			memset(role_name, 0, sizeof(role_name));
			int32 len = _name.size() > max_name_size_one_team ? max_name_size_one_team : _name.size();
			memcpy(role_name, _name.c_str(), len);
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		void to_proto(hld::st_proto::st_single_map_record* st_single_ptr)
		{
			if (st_single_ptr == nullptr)
			{
				return;
			}
			st_single_ptr->set_role_guid(role_guid.server_64);
			st_single_ptr->set_role_name(role_name);
			st_single_ptr->set_value(value);
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
		void from_proto(const hld::st_proto::st_single_map_record& st_single_ref)
		{
			role_guid.server_64 = st_single_ref.role_guid();
			my_memcopy_string(role_name, max_name_size, st_single_ref.role_name());
			value = st_single_ref.value();
		}
	};

	struct s_best_record
	{
		int32	map_template_id;
		s_single_map_record		single_map_best_record[hld::single_map_record_num];

		s_best_record()
		{
			memset(this, 0, sizeof(*this));
		}
	};

#pragma pack(pop)
}

#endif
