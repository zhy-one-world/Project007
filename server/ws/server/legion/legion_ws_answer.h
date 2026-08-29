#ifndef _WS_LEGION_WS_ANSWER_H_
#define _WS_LEGION_WS_ANSWER_H_

#include "legion.pb.h"
#include "Logic/legion_def.hpp"
#include "utility/score_indicator.h"
namespace faith
{
	class legion_ws_answer
	{
	public:
		explicit legion_ws_answer();
		~legion_ws_answer();

		static legion_ws_answer& get_instance()
		{
			static legion_ws_answer instance;
			return instance;
		}
		void tick(const int64& new_time);
		void answer_legion_question(uint64 role_guid, std::vector<xstring> answer_vec);
		void check_prepare_notice();
		void clear_data();
		//void add_legion_info(guid_64 legion_guid);
		//答题公告
		void notice_on_legion_prepare_answer(uint32 prepare_time);
		void get_legion_question_info(guid_64 role_guid);//返回当前答题进度单信息
		void send_legion_question_info_by_legion(guid_64 legion_guid);

		void answer_correctly(guid_64 role_guid);		//回答正确的处理
		void refresh_question_answer();
		bool is_in_legion_answer_time();		    //是否在答题时间段内
		bool is_in_legion_answer_read_time();
		bool is_in_legion_answer_all_time();
		void notice_on_first_answer_true_notice(guid_64 legion_guid, const xstring& role_name, int32  answer_id);
		void notice_on_answer_failure_notice(guid_64 legion_guid, int32  answer_id);
		void notice_on_play_answer_num_notice(guid_64 legion_guid, const xstring& role_name, int32 question_num);
		void notice_on_legion_answer_end_notice(guid_64 legion_guid, const xstring& role_name, int32 question_num);
		void send_activity_end_notice();
		void legion_answer_end();
		void send_legion_answer_rank_list(guid_64 role_guid);
		void send_legion_answer_rank_list_all();
		
	private:
		int64					m_legion_answer_activity_begin_time;	//记录当天军团答题的开始
		int64					m_legion_answer_activity_end_time;		//记录当天军团答题的结束	
		bool					m_legion_answer_activity_begin;			//判断军团答题活动
		score_indicator			m_legion_answer_rank_list;				//军团打题结束时间排名
		int32					m_notice_index;							//答题准备公告索引

		int32					m_answer_index;							//题目索引
		int64					m_next_answer_time;						//下一题的出题时间
		int32					m_topic_id;								//当前题目
		int32					m_answer_id;							//当前题目答案id
		std::vector<int32>		m_old_answer;							//本次答题已经出过的题
		int64					m_next_rank_list_refresh_time;			//排行榜列表刷新时间
	};
}

#endif