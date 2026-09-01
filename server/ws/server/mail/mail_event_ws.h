#ifndef _WS_MAIL_EVENT_WS_H_
#define _WS_MAIL_EVENT_WS_H_

#include "server/client_session_mgr.hpp"
#include "logic/type_def.hpp"
#include "mail.pb.h"
#include "Logic/mail_def.hpp"
#include "Logic/item_def.hpp"
#include "template/template_manager.h"

namespace faith
{
	struct s_single_mail_info
	{
		s_mail_info mail_info;
		int32		item_num;
		s_item_info	item_array[max_item_per_mail];

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		s_single_mail_info()
		{
			clear_data();
		}
	};

	class mail_ws_mgr
	{
		mail_ws_mgr();
		~mail_ws_mgr();
	public:
		static mail_ws_mgr& get_instance()
		{
			static mail_ws_mgr instance;
			return instance;
		}

		void tick(const int64& new_time);

		void send_mail(guid_64 address_guid, int32 address_server_id, const s_mail_info& mail_info, const s_item_info* item_list, int32 item_num, bool is_only_to_sql = false);
		void send_mail(guid_64 address_guid, int32 address_server_id, s_mail_info mail_info, const std::vector<s_item_template_info>& item_list, bool is_only_to_sql = false);

		void send_mail_system(guid_64 player_guid, int32 player_server_id, const std::vector<s_item_template_info>& item_list, std::string title, std::string contentText, int32 money_typ1 = 0, int32 money_num1 = 0, int32 money_typ2 = 0, int32 money_num2 = 0);

		void send_mail_by_mail_id(guid_64 address_guid, int32 address_server_id, int32 mail_tamplate_id, int32 show_send_time = 0);
		void send_mail_by_mail_template_ptr(guid_64 address_guid, int32 address_server_id, MailTemplate* mail_template_ptr, int32 show_send_time = 0);
		
		std::vector<s_single_mail_info> get_mail_info_array_by_mail_template_ptr(MailTemplate* mail_template_ptr, int32 show_send_time = 0);

		void check_id_mail(guid_64 role_guid, int32 server_id, int32 cur_time);
		void check_id_mail_end(guid_64 role_guid, int32 server_id, const s_id_mail_info* mail_array, int32 mail_num);

		int32 get_effect_index_from_money_array(std::vector<int32>& money_array);
		int32 get_effect_index_from_item_array(std::vector<s_item_info>& item_array);
		
		void load_other_server_mail_record_end(const s_other_server_mail_record* mail_record, int32 data_num);
		void save_other_server_mail_record(s_other_server_mail_record mail_record);
		void add_other_server_mail_record(s_other_server_mail_record mail_record);
		void del_other_server_mail_record(s_other_server_mail_record mail_record);
		s_other_server_mail_record find_other_server_mail_record(int32 source_server_id, int32 target_server_id);

		void send_other_server_mail(s_other_server_mail_info mail_info);			//发送方
		void recv_other_server_mail(const s_other_server_mail_record& mail_record, const s_other_server_mail_info& mail_info);	//接收方
		void recv_other_server_mail_end(const s_other_server_mail_record& mail_record);		//发送方

		void load_other_server_mail_tick(const int64& new_time);
		void load_other_server_mail_end(int32 server_id, const s_other_server_mail_info* mail_record, int32 data_num);
		void send_other_server_mail_tick(const int64& new_time);
		void del_other_server_mail(s_other_server_mail_record mail_record);

	public:
		s_other_server_mail_record	m_other_server_mail_record[max_other_server_mail_record_num];
		s_other_server_mail			m_other_server_mail[max_server_num];

		bool						other_server_mail_record_load_finish = false;
	public:
		std::vector<s_item_template_info>* get_free_item_list();
		void  add_free_item(int32 item_id, int32 item_num);
	public:
		std::vector<s_item_template_info>  free_item_list;

	};

}

#endif