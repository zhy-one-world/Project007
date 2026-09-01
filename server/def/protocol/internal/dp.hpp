/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 11:14
  file base: base_dp
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _BASE_DP_HPP_
#define _BASE_DP_HPP_

#include "logic/type_def.hpp"
#include <Logic/char_def.hpp>
#include <base.hpp>

using namespace faith;

namespace faith
{
	const int32 msgindex_req_query_log_db	=			e_msg_base_all2dp;
	const int32 msgindex_rep_query_log_db	=			e_msg_base_dp2all;

#pragma pack(push,1)
	//向LogDB中提交一个SQL查询请求
	struct req_query_log_db : public packet_base
	{
		enum{max_limit = 1024,};
		ui16				data_size;
		xchar				data[max_limit];

		req_query_log_db()
		{
			memset(this,0,sizeof(*this));
			wheader=msgindex_req_query_log_db;
		}

		uint32 get_pak_length() const
		{
			const uint32 basic_len = (ULONG_PTR)&data - (ULONG_PTR)&wheader;
			return ( basic_len + data_size );
		}
	};

	//SQL执行结果
	struct rep_query_log_db : public packet_base
	{
		enum{max_limit = 4096,};
		enum
		{
			e_success = 0,
			e_failed,
		}
		e_rusult;
		ui16				data_size;
		xchar				data[max_limit];

		rep_query_log_db()
		{
			memset(this,0,sizeof(*this));
			wheader=msgindex_rep_query_log_db;
			e_rusult=e_failed;
		}

		uint32 get_pak_length() const
		{
			const uint32 basic_len = (ULONG_PTR)&data - (ULONG_PTR)&wheader;
			return ( basic_len + data_size );
		}
	};
#pragma pack(pop)
}
#endif
