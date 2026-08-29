
#ifndef __WEB_CLIENT_H__
#define __WEB_CLIENT_H__
#include "Logic/web_def.hpp"
#include <json/json.h>
#include "client_session_mgr.hpp"
#include "net/http_def.hpp"
#include <net/net_client_mgr.hpp>
#include "recharge_mgr_ws.hpp"

namespace faith
{
	class web_client
	{
	public:
		web_client();

		static web_client& get_instance()
		{
			static web_client s_web_client;
			return s_web_client;
		}

		void clear_data();
		void tick(int64& new_time);

		void handler_http_onrecv(const s_http_receive_info& receive_info);
		void hanlde_login(const char* data_ptr, uint32 length);
		void hanlde_json(const char* data_ptr, uint32 length);

		//gmt以及billing被干掉后，可删除全部关于is_send_to_old_gmt的逻辑
		void do_gm_order(int32 gm_type, Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt);
		void gm_order_end(int32 result_code, xstring result_msg, s_daemon_callback_info callback_info);
		void gm_order_end_new(int32 result_code, xstring result_msg, s_daemon_callback_info callback_info);
		void new_do_gm_order(xstring gm_type, Json::Value& json_value, s_daemon_callback_info callback_info);
	public:
		void set_role_info(const s_unit_info* role_info, int32 data_num, s_daemon_callback_info callback_info,int32 check_type);
		void set_role_time(const guid_64& role_guid, const s_time_info& role_time, int64 order_num);
		void set_role_money(const guid_64& role_guid, const s_money_info& role_money, int64 order_num);
		void set_role_lock(const guid_64& role_guid, int32 is_login, int32 is_send_chat, int64 order_num);
		void data_load_over(int64 order_num);
		void db_role_info_modified(const guid_64& role_guid, s_daemon_callback_info callback_info);
		void check_gm_modify_role_info(const guid_64& role_guid);
		void set_role_items_info(const guid_64& role_guid, const s_item_info* items_info, int32 item_num, int64 order_num);
		void send_server_state_to_gm(const server_info_gm& srv_info);
		void send_server_state_to_gm_end(ui64 http_id, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result);
		void send_role_info_to_gm(const s_client_uid client_uid, const s_unit_info& role_info);
		void send_role_info_to_to_gm_end(ui64 http_id, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result);
		void recv_recharge(xstring& order_id_str, guid_64& role_guid, int32& game_goods_id, float& pay_price, int32& direct_diamond, s_daemon_callback_info& callback_info);

	private:
		void send_role_right(const xchar* role_mark, int32 right_type, int32 right_value, s_daemon_callback_info callback_info, int32 right_time = 0);
		void send_get_role_info(int32 check_type, const xchar* role_mark, e_unit_guid get_type, s_daemon_callback_info callback_info);
		void send_modify_role_info(const guid_64& role_guid, xstring sql_str, s_daemon_callback_info callback_info);
		void send_get_role_items_info(const guid_64& role_guid, s_daemon_callback_info callback_info);
		void get_mail_data(s_mail_info& mail_info, s_item_info* item_array, int32& item_num, const std::vector<int32>& item_list, const xstring& title, const xstring& content, int32 money_type_1, int32 money_num_1, int32 money_type_2, int32 money_num_2);
		void send_mail_proc(client_session* target_session, s_mail_info& mail_info, s_item_info* item_array, int32& item_num,
			xstring role_guid_str, xstring role_name_str, xstring role_acount_str);
		void send_order_win(int32 json_type, int64 order_num);
		void send_order_fail(int32 json_type, std::string error_type, int64 order_num);
		void send_taiwan_order_fail(int32 json_type, std::string error_types, int64 order_num);
		void send_get_server_info_gm();
		void send_del_unit_item(int32 cs_unit_index, int32 bag_type, int32 bag_slot_begin, int32 bag_slot_end);
		void send_change_unit_vip(int32 cs_unit_index, int32 vip_value);
		void send_del_unit_specified_item(int32 cs_unit_index, int32 bag_type, int32 item_id);
		void send_stop_login(int32 stop_login, s_daemon_callback_info callback_info);
		void send_server_reload_csv();
		void send_mail_by_role_reward(const xstring& title, const xstring& content);
		void send_server_kick_out_legion_chief(guid_64 legion_guid);
		void send_del_role_cache(guid_64 role_guid);
		void send_del_role(guid_64 role_guid, int32 is_del, s_daemon_callback_info callback_info);
		void send_server_end_refresh_server_list();

		void send_server_need_begin_cross_with_type(int32 gm_type, int32 need_begin, int64 req_http_id);
		void send_server_info_arr_with_type(int32 server_info_type, int32 server_info_value, s_daemon_callback_info callback_info);
	public:
		void send_change_account(const xstring& change_name, const xstring& target_name, s_daemon_callback_info callback_info);
		void send_change_account_end(int32 change_res, s_daemon_callback_info callback_info);
	public:
		void send_activate_code(client_session* session, std::string activate_code, int32 login_type);
		void send_activate_code_end(ui64 http_id, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result);
		void send_recharge_end_result(int32 result_type, int64 order_num, int32 payment_type = 0);

	public:
		void parse_json_get_role_info(Json::Value& json_value, s_daemon_callback_info callback_info);
		void parse_json_modify_role_info(Json::Value& json_value, xstring sql_table_name, s_daemon_callback_info callback_info);
		void parse_json_get_role_items_info(Json::Value& json_value, s_daemon_callback_info callback_info);

		void parse_json_set_send_chat(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt);
		void parse_json_set_ban_login(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt);
		void parse_json_kickout_player(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt);
		void parse_json_do_text_command(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt);
		void parse_json_set_send_mail(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt);
		void parse_json_set_send_delay_marquee(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt);
		void parse_json_activate_code(Json::Value& json_value);
		void parse_json_recharge(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt);
		void parse_json_robot_op(Json::Value& json_value, int64 order_num);
		void parse_json_open_time(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt);
		void parse_json_change_password(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt);
		

		//新后台GM指令
		void parse_json_set_send_mail_new(Json::Value& json_value, s_daemon_callback_info callback_info);
		void parse_json_set_send_chat_new(Json::Value& json_value, s_daemon_callback_info callback_info);
		void parse_json_kickout_player_new(Json::Value& json_value, s_daemon_callback_info callback_info);
	public:
		void recv_recharge_en_end(int64 handle_index, int32 code_index);
		void handler_http_on_en_recharge_end(const s_http_receive_info& receive_info);
		void handler_http_on_recv_recharge_end(const s_http_receive_info& receive_info);
		void handler_http_on_recv_umisdkrecharge_end(const s_http_receive_info& receive_info);
	private:
		gm_all_info_map m_all_role_info_map;
		gm_mod_role_info_map m_modify_role_info_sql_map;
		int64							m_role_info_map_tick = 0;
		std::map<xstring, int32>	m_gmcode_map;
	};
}

#endif //
