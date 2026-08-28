/********************************************************************
created: 2016Äê9ÔÂ6ÈÕ13:56:28
file base: chat_mgr
file ext: h
author: lxy

purpose: about chat
*********************************************************************/
#ifndef _CHAT_MGR_H_
#define _CHAT_MGR_H_

#include "Logic/chat_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/aoi_def.h"

namespace hld
{
	class player;

	class cchat_mgr
	{
	public:
		cchat_mgr();
		~cchat_mgr(void);

		void clear_data();
		void set_player_ptr(player* player_ptr) { m_player_ptr = player_ptr; };

		void send_globel_message(std::string chat_text);

		bool parse_notice_by_symbol_type(const int32& notice_id, const xstring& unit_name, const std::vector<xstring>& other_name, const std::vector<xstring>& describe_str, const std::vector<xstring>& item_name, const  std::vector<int32>& num_type_data, xstring& _out_notice_str);
		bool make_and_send_notic_msg_data(const guid_64&  unit_guid, const int32&   notice_template_id, const std::string& show_string, const std::vector<s_item_info>& item_data);
		void send_notice_new(const int32&  notice_template_id, const std::vector<xstring>& other_name, const std::vector<xstring>& describe_str, const std::vector<xstring>& item_name, const  std::vector<int32>& num_type_data, const std::vector<s_item_info>& item_data);
		void send_notice_new(const int32&  notice_template_id, const s_chat_notice_info& notice_data);
		void send_change_first_ranking_notice(const int32& ranking_type, bool is_first);

		void send_notice(int32 notice_template_id, std::string show_string);
		static void send_notice(int32 notice_template_id, std::string show_string, guid_64 role_guid);
		void send_notice(int32 notice_template_id, std::string show_string, s_item_info* item_data, int32 item_num);
		void receive_notice(int32 notice_template_id, guid_64 sender_guid,int32 create_time);
		void receive_top_tip(int32 string_id, int32 top_tip_type = e_top_tip_type_normal);

		void send_chat_new(e_chat_content_info* chat_content_array, int32 chat_content_num, std::string common_string,int32 chat_type, guid_64 addressee_guid);
		void send_failed_process(int32 send_result, int32 chat_type);
		
		void masking_word_check(std::string& temp_text);		//ÆÁ±Î×ÖÌæ»»
		bool is_had_special_word(std::string& temp_text);		//ÌØÊâ×Ö·û¼ì²é
		void forbidden_word_check(e_chat_content_info* chat_info_ary, int32 chat_content_num); // forbidden word check
		
		bool is_ban_speech();									//ÊÇ·ñ±»½ûÑÔ
	private:
		void check_forbidden_and_reconstruct(std::string& str_to_be_checked, const std::vector<int32>& chat_composition, e_chat_content_info* chat_content_ary); // called inside forbidden_word_check

	private:
		player*						m_player_ptr;
		bool						m_is_disable_private_chat;
		int64						m_last_send_chat_time;
		std::string					m_last_chat_str;
		int64						m_last_chat_time;
		int64						m_last_send_server_chat_time;
	public:
		void load_chat_record_by_db(const s_chat_record * record_data, int32 load_num);
		bool load_chat_record_by_db_lua(const char *data_ptr, int32 data_len);
		void save_chat_record(int32 save_type);
		void sync_all_chat_record_to_client();
		void sync_one_chat_record_to_client(const s_chat_record * record_data);
		void chat_oper(int32 pos, xstring record);
		void send_to_result(int32 pos, int32 result);
		static int32 get_offset(xchar value);
		static int32 calc_string_len(xstring& record);

	private:
		s_chat_record               m_chat_record_array[chat_record_max_num];

	};
}

#endif
