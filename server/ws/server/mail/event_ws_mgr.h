#ifndef _WS_EVENT_WS_MGR_H_
#define _WS_EVENT_WS_MGR_H_

#include "logic/type_def.hpp"
#include "Logic/chat_def.hpp"
#include "Logic/event_def.hpp"
#include "Logic/item_def.hpp"

namespace faith
{
	typedef std::vector<uint32> notice_time_array;
	typedef notice_time_array::iterator notice_time_array_ite;
	//用于存储那些需要活一段时间，而不是只播一次就行的公告
	struct continued_notice
	{
		xchar		text[globel_message_max_size + 1];	//显示内容
		int64		m_begin_time;						//何时显示，单位：毫秒
		int64		m_end_time;							//何时结束，单位：毫秒
		int64		m_interval_time;					//间隔时间，单位：毫秒
		int32		notice_id;							//公告的ID，有这个，就不需要公告类型和内容了
		guid_64		sender_guid;
		bool		is_effect;

		continued_notice()
		{
			clear_data();
		}
		
		void clear_data()
		{
			memset(text, 0, sizeof(text));
			m_begin_time = 0;
			m_end_time = 0;
			m_interval_time = 0;
			sender_guid.clear_data();
			notice_id = -1;
			is_effect = false;
		}
	};
	typedef std::unordered_map<ui64, continued_notice> continued_notice_map;
	typedef continued_notice_map::iterator continued_notice_map_ite;

	class event_ws_mgr
	{
		event_ws_mgr();
		~event_ws_mgr();
	public:
		static event_ws_mgr& get_instance()
		{
			static event_ws_mgr instance;
			return instance;
		}

		void clear_data();

		void tick(const int64&  now_stamp);
		void tick_1sec(const int64&  now_stamp);

		void save_globel_mail_to_sql();
		void save_mail_event_end();

		void load_globel_mail_by_sql();
		void load_globel_mail_item_by_sql();

		void load_mail_event_end(s_mail_info* mail_info_ptr , int32 mail_num);
		void load_mail_event_item_end(s_item_info* item_info_ptr, int32 item_num);

		void load_all_event_info_to_ws();

		void add_globel_mail(s_mail_info temp_mail_info,const s_item_info* temp_item_list,int32 item_num);

		void check_player_globel_mail(const guid_64* had_event_array, int32 data_num, guid_64 player_guid, int64 role_create_time);

		//void set_under_notice(int32 notice_id, int32 notice_create_time, int32 notice_live_time);

		//void set_contiunted_notice(guid_64 sender_guid, int32 notice_id, int32 notice_create_time, std::string text = "", int32 notice_live_time = 0 , int32 trigger_time = 0);
		bool add_delay_globle_notice(guid_64 notice_guid, xstring& notice_content, int64 startStamp, int64 endStamp, int64 marqueeInterval);
		void del_delay_globle_notice(guid_64 notice_guid);

		void send_notice_to_all_only_string(xstring& string_content);
		void send_notice_with_text(std::string notice_text, int32 notice_create_time, int32 notice_show_typ);//用于不在表里的，随便写的TEXT的公告
		void send_mail_to_player_by_system(guid_64 terget_player_guid,
			int32 target_server_id,
			std::string sender_name,
			std::string title,
			std::string content_text,
			int32 money_typ1,
			int32 money_num1,
			int32 money_typ2,
			int32 money_num2,
			const std::vector<s_item_info>& item_list);

		void send_notice_to_all(int32 notice_id, int32 notice_create_time, guid_64 sender_guid, std::string final_string,int32 sender_template_id = 0);		// 模板公告唯一接口
		void send_notice_to_all(int32 notice_id, int32 notice_create_time, guid_64 sender_guid, std::string final_string, s_item_info* item_data, int32 item_num, int32 sender_template_id = 0);

		void send_notice_with_param(guid_64 sender_guid, const xchar* sender_name, const xchar* content_text, 
			const std::vector<int32>& param_array, int32 param_type, int32 chat_type, int32 sender_template_id = 0, int32 notice_id = 0);


		int32	get_empty_globel_mail_slot();

	private:
		void send_notice_to_everyone(google::protobuf::Message* net_pro, uint32 header);										// 将消息发送给全服
		void send_notice_to_legion(guid_64 sender_guid, google::protobuf::Message* net_pro, uint32 header);						// 只将消息发送给军团
		void send_notice_to_team(guid_64 sender_guid, google::protobuf::Message* net_pro, uint32 header);						// 只将消息发送给队伍
		void send_notice_to_self(guid_64 sender_guid, google::protobuf::Message* net_pro, uint32 header);						// 只将消息发送给自己

		void set_mail_proto_info(server2dp_proto_s_mail_info * mail_info, s_mail_info _info);

	private:

		//std::vector<s_mail_info> m_globel_mail_array;
		s_mail_info			m_globel_mail_array[max_globel_mail_in_same_time];
		s_mail_info			m_temp_mail_array[max_globel_mail_in_same_time];
		guid_64				m_temp_event_guid_array[max_globel_mail_in_same_time];
		std::unordered_map<ui64, s_item_info> m_globel_mail_item_array;

		continued_notice_map m_continued_notice_array;

		int32 m_timer_process_time;
		int32 m_mail_event_living_time;
		int32 m_refresh_time;
		int32 m_cur_refresh_time;
	public:
		s_item_info* get_free_item() { free_item.clear_data(); return &free_item; }
		std::vector<s_item_info>*  get_free_list()  { free_item_list.clear(); return  &free_item_list; }
		void  add_free_item(s_item_info* free_item) { free_item_list.push_back(*free_item); }
		std::vector<int32>*  get_free_param() { free_param_arr.clear(); return &free_param_arr; }
		void  add_free_param(int32 value) { free_param_arr.push_back(value); }
		
	private:
		s_item_info  free_item;
		std::vector<s_item_info> free_item_list;
		std::vector<int32> free_param_arr;


	};
}



#endif
