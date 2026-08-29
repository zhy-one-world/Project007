/********************************************************************
created: 2016Äê8ÔÂ16ÈÕ
file base: mail_mgr
file ext: h
author: luoxingyu

purpose: about role's mail_mgr
*********************************************************************/
#ifndef _MAIL_MGR_H_
#define _MAIL_MGR_H_

#include "mail.h"
#include "item.h"
#include "Logic/time_def.hpp"
#include "Logic/event_def.hpp"
#include "utility/cs_date.hpp"

namespace faith
{
	class player;
	class mail_proto_update_mail;
	typedef std::map<ui64, cmail> unit_mail_map;
	typedef std::map<ui64, s_item_info> unit_mail_item_map;
	typedef unit_mail_item_map::iterator unit_mail_item_map_it;


	class cmail_mgr
	{
	public:
		cmail_mgr();
		~cmail_mgr(void);
		void						get_mail_msg(mail_proto_update_mail& mail_msg, cmail& mail_ptr);
		void						get_mail_item_msg(item_proto_update_character_item& item_msg, const s_item_info& item_info);
		void						set_player_ptr(player* player_ptr) { m_player_ptr = player_ptr; };
		void						clear_data();
		void						check_mail_event_state();
		void						load_had_globel_mail_guid_end(const guid_64* guid_array);
		bool                        load_had_globel_mail_guid_by_db_lua(const char *data_ptr, int32 data_len);
		void						add_event_guid(guid_64 temp_guid);
		void						send_mail_one(cmail& mail_ptr);
		void                        send_mail_one_item(cmail&  new_mail);
		void						send_mail_all();
		void						send_mail_item_all();
		void						send_mail_batch(std::vector<cmail> mail_list);
		void						heart_tick(const int64& new_time);
		void						clear_date_by_index(int32 target_index);
		//void save_mail_to_db(const s_mail_info* mail_data, int32 mail_num);

		void						add_mail_to_db(s_mail_info mail_data);
		void						delete_mail_in_db(guid_64* mail_guid, int32 guid_num);
		void						delete_mail_in_db(const guid_64& mail_guid);
		void						load_mail_by_db(const s_mail_info* mail_data, int32 mail_num);
		bool                        load_mail_by_db_lua(const char *data_ptr, int32 data_len);
									
		void						delete_mail_item_in_db(guid_64 mail_item_guid);
		void						load_mail_item_by_db(const s_item_info* item_data, int32 item_num);
		bool                        load_mail_item_by_db_lua(const char *data_ptr, int32 data_len);
		void						del_mail_end();

		int32						mail_operate(const std::vector<guid_64>& mail_guid, int32 operation_type);
		cmail*						receive_mail(s_mail_info mail_info, std::vector<s_item_info>& item_list);
		cmail*						create_mail_by_info(s_mail_info& temp_info);
		cmail*						get_mail(const guid_64& mail_guid);
		int32						get_mail_index(const guid_64& mail_guid);
		void						send_mail_to_another_player(guid_64 TargetPlayerGuid,
										int32 target_server_id,
										std::string title,
										std::string contentText,
										int32 money_typ1,
										int32 money_num1,
										int32 money_typ2,
										int32 money_num2,
										std::vector<guid_64>& item_guid_list);

		void						send_mail_to_another_player_by_system(guid_64 TargetPlayerGuid,
										int32 target_server_id,
										std::string title,
										std::string contentText,
										int32 money_typ1,
										int32 money_num1,
										int32 money_typ2,
										int32 money_num2,
										const std::vector<citem*>& item_list);

		void						send_mail_to_another_player_by_system(guid_64 TargetPlayerGuid,
										int32 target_server_id,
										std::string title,
										std::string contentText,
										int32 money_typ1,
										int32 money_num1,
										int32 money_typ2,
										int32 money_num2,
										s_item_info* item_list,
										int32 item_num,
										int64 cur_time_sec = time_helper::get_cur_time_new().second);

		void						send_mail_to_all_player(
										std::string title,
										std::string contentText,
										int32 money_typ1,
										int32 money_num1,
										int32 money_typ2,
										int32 money_num2,
										std::vector<guid_64>& item_guid_list);

		int32						auto_get();
		int32						del_readed();
		int32						get_and_del(guid_64 mail_guid);
		int32						del_mail(guid_64 mail_guid);
			 						
		int32						get_one_content(guid_64 mail_guid, 
										std::vector<int32>&			all_money_get_array,
										std::vector<int32>&			all_item_get_array, bool& has_item_add_bag_failed);
		int32						force_delete_one(guid_64 mail_guid);
		int32						force_delete_all_readed();
			 						
		int32						read_mail(std::vector<guid_64>  mail_guid);
		int32						get_contents_and_del(std::vector<guid_64>  mail_guid);
		int32						force_delete_mail(std::vector<guid_64>  mail_guid);

		int32						get_mail_num();
		bool						can_receive_new_mail();
		void						del_oldest_mail();
		int32						get_empty_mail_slot();

		void						auto_load_mail();

		void						operate_load_lock(int32 change_num);
	private:
		player*						m_player_ptr;
		cmail						m_mail_list[MAX_MAIL_NUM];
		unit_mail_item_map			m_mail_item_map;
		//s_item_info					m_mail_item_list[max_mail_item_num];
		int64						m_auto_load_timer;
		int32						m_mail_list_num;
		//int64						m_globel_mail_next_check_time;
		guid_64						m_globel_mail_sign[max_globel_mail_in_same_time];
		int32						m_mail_load_lock;
		guid_64						m_del_mail_array[MAX_MAIL_NUM];
		int32						del_array_effect_num;
		void						clear_del_array();
		void						add_to_del_array(const guid_64& target_guid);
		void						del_by_del_array();
	};
}
#endif