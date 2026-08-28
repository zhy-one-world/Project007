
#ifndef __RECHARGE_MGR_WS_H__
#define __RECHARGE_MGR_WS_H__

#include "logic/type_def.hpp"
#include "Logic/char_def.hpp"
#include "Logic/mail_def.hpp"
#include "Logic/item_def.hpp"
#include "def/http/http_access_mgr.hpp"
#include "net/tcp_client.hpp"
#include "client_session_mgr.hpp"
#include <json/json.h>
#include <daemon.hpp>

namespace hld
{
	struct s_recharge_temp_info
	{
		guid_64	role_guid;
		float	pay_price;
		int32	payment_type;
		xchar	md5[md5_length + 1];

		void clear_data()
		{
			role_guid.clear_data();
			pay_price = 0;
			payment_type = 0;
			memset(md5, 0, sizeof(md5));
		}

		s_recharge_temp_info()
		{
			clear_data();
		};
	};

	struct s_unfinish_message_info
	{
		int64 invalid_time;
		int32 old_package_part;
		std::string unfinish_token;
		s_recharge_temp_info recharge_info;
		int64		order_num;
		int64		client_uid;

		void clear_data()
		{
			invalid_time = 0;
			old_package_part = -1;
			unfinish_token = "";
			order_num = 0;
			client_uid = 0;
			recharge_info.clear_data();

			//string ²»ÒªÓÃmemset
		}

		s_unfinish_message_info()
		{
			clear_data();
		}
	};

	enum e_recharge_end_msg_error_code
	{
		e_recharge_end_msg_error_code_success = 1001,
		e_recharge_end_msg_error_code_perpeat_order,
		e_recharge_end_msg_error_code_sign_error,
		e_recharge_end_msg_error_code_price_error,
		e_recharge_end_msg_error_code_user_error,
		e_recharge_end_msg_error_code_platform_error,
		e_recharge_end_msg_error_code_order_timeout,
		e_recharge_end_msg_error_code_failed,
		e_recharge_end_msg_error_code_max,
	};

	enum e_en_recharge_buy_type
	{
		e_en_recharge_buy_type_none,
		e_en_recharge_buy_type_normal,
		e_en_recharge_buy_type_manual_supplement,
		e_en_recharge_buy_type_max,
	};

	const std::string en_secr_key = "af8143bf48ad60ccad6484ee7e3213c4";

	class recharge_mgr_ws
	{
		recharge_mgr_ws();
		~recharge_mgr_ws();
	public:
		static recharge_mgr_ws& get_instance()
		{
			static recharge_mgr_ws s_recharge_mgr_ws;
			return s_recharge_mgr_ws;
		}

		void clear_data();
		void init_manager();
		void tick(int64 time_new);
		void check_and_save_recharge(const xchar* order_id, guid_64 role_id, int32 game_goods_id, float pay_price, int64 order_num, int32 payment_type, const s_client_uid client_uid = s_client_uid(), const xstring token = "", const int32 force_error_code = e_recharge_error_none, const int32 direct_diamond = 0);
		void db_save_recharge_end(const xchar* order_id, const guid_64& role_id, const int32& game_goods_id, float pay_price, const int32& success, int64 order_num, int32 payment_type, const int64 client_uid = 0, const xstring token = "", const int32 direct_diamond = 0);
		void recharge_send_mail(const xchar* order_id, const guid_64& role_id, const int32& game_goods_id, float pay_price, int64 order_num, int32 payment_type, int32 direct_diamond = 0);

		void recharge_process_success(const xchar* order_id, guid_64 role_id, int32 game_goods_id, int64 order_num, int32 payment_type);
		void send_recharge_end_result(int32 result_type, int64 order_num, int32 payment_type, guid_64 role_guid, int32 goods_id, const xchar* order_id, bool is_first_recharge = false);

		void do_send_recharge_reward(const xchar* order_id, const guid_64& role_id, const int32& game_goods_id, float pay_price, int64 order_num, int32 payment_type, int32 direct_diamond = 0);
		RechargeTemplate* get_recharge_template_ptr(std::string product_id);

	private:
		std::map<std::string, int32>		m_product_id_map;
	};
}

#endif //
