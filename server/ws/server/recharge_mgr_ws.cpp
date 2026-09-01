
#include "recharge_mgr_ws.hpp"
#include "web_client.hpp"
#include "world_server.hpp"
#include "server_log.hpp"
#include "recharge/recharge_service.hpp"
#include "utility/init_unit.h"
#include <char_msg.hpp>
#include "ws_client.hpp"
#include "template/template_manager.h"
#include "server_log.hpp"
#include "globle_data.h"
#include "server/mail/mail_event_ws.h"
#include "net.pb.h"
#include "character.pb.h"

namespace faith
{

	const int32 custom_recharge_id = 104099999;


	recharge_mgr_ws::recharge_mgr_ws()
	{
		clear_data();
	}
	recharge_mgr_ws::~recharge_mgr_ws()
	{

	}

	void recharge_mgr_ws::clear_data()
	{
	}

	void recharge_mgr_ws::init_manager()
	{
		m_product_id_map.clear();

		template_manager::template_type* template_table = template_manager::get_instance().get_templates(e_RechargeTemplate);
		if (nullptr == template_table)
		{
			return;
		}
		
		for (auto ite = template_table->begin(); ite != template_table->end(); ++ite)
		{
			RechargeTemplate* template_ptr = (RechargeTemplate*)(ite->second);
			m_product_id_map.insert({ template_ptr->ProductId, ite->first });
		}
	}

	void recharge_mgr_ws::tick(int64 time_new)
	{

		
	}
	void recharge_mgr_ws::check_and_save_recharge(const xchar* order_id, guid_64 role_id, int32 game_goods_id, float pay_price, int64 order_num, int32 payment_type, const s_client_uid client_uid, const xstring token, const int32 force_error_code, const int32 direct_diamond)
	{
		if (order_id == nullptr)
		{
			send_recharge_end_result(e_recharge_error_send_save_recharge_order_id_is_null, order_num, payment_type, role_id, game_goods_id, order_id);
			return;
		}

		{
			RechargeTemplate* recharge_template_ptr = GET_TEMPLATE(RechargeTemplate, game_goods_id);
			if (recharge_template_ptr == nullptr)
			{
				if (direct_diamond <= 0)
				{
					send_recharge_end_result(e_recharge_error_recharge_by_template_id_recharge_template_ptr_is_null, order_num, payment_type, role_id, game_goods_id, order_id);
					return;
				}
			}
			else
			{
				if (FLOAT_IS_ZERO(pay_price))
				{
					pay_price = recharge_template_ptr->Price;
				}
			}
		}

		xstring order_id_str = xstring(order_id);
		client_session* session = client_session_mgr::getInstance().get_session(role_id);
		login_fixed_data temp_third_data;
		int32 login_type = e_login_type_new_account_and_password;
		s_unit_info *role_info_ptr = nullptr;
		if (session)
		{
			temp_third_data = session->m_login_third_data;
			login_type = session->m_login_type;
			role_info_ptr = &session->m_role_info;
			if (e_payment_method_tl_umisdk_google == payment_type)
			{
				payment_type = atoi(temp_third_data.param14);
			}
		}
		server_log::recharge_step_log(order_id_str, role_id, game_goods_id, e_recharge_step_ws_save_recharge, temp_third_data, login_type, role_info_ptr);

		ws2dp_save_recharge req;
		int32 order_id_len = strlen(order_id);
		memcpy(req.order_id, order_id, order_id_len < max_recharge_order_length ? order_id_len : max_recharge_order_length);
		req.role_guid = role_id;
		req.game_goods_id = game_goods_id;
		req.pay_price = pay_price;
		req.order_num = order_num;
		req.payment_type = payment_type;
		req.client_uid = client_uid.fep_uid_64;
		req.direct_diamond = direct_diamond;
		req.force_error_code = force_error_code;
		req.third_info = temp_third_data;
		req.login_type = login_type;

		int32 order_token_len = token.length();
		memcpy(req.order_token, token.c_str(), order_token_len < max_recharge_order_token_length ? order_token_len : max_recharge_order_token_length);

		ws_client::getInstance().send_to_dp(&req, sizeof(req));
	}

	void recharge_mgr_ws::db_save_recharge_end(const xchar* order_id, const guid_64& role_id, const int32& game_goods_id, float pay_price, const int32& success, int64 order_num, int32 payment_type, const int64 client_uid, const xstring token, const int32 direct_diamond)
	{
		if (order_id == nullptr)
		{
			send_recharge_end_result(e_recharge_error_db_save_recharge_order_id_is_null, order_num, payment_type, role_id, game_goods_id, order_id);
			return;
		}
		xstring order_id_str = xstring(order_id);
		client_session* cur_session = client_session_mgr::getInstance().get_session(role_id);
		login_fixed_data temp_third_data;
		int32 login_type = e_login_type_new_account_and_password;
		s_unit_info *role_info_ptr = nullptr;
		if (cur_session)
		{
			temp_third_data = cur_session->m_login_third_data;
			login_type = cur_session->m_login_type;
			role_info_ptr = &cur_session->m_role_info;
		}
		server_log::recharge_step_log(order_id_str, role_id, game_goods_id, e_recharge_step_ws_save_recharge_end, temp_third_data, login_type, role_info_ptr);
		int32 order_id_len = strlen(order_id);
		if (order_id_len <= 0 || order_id_len > max_recharge_order_length)
		{
			if (order_id_len <= 0)
			{
				send_recharge_end_result(e_recharge_error_db_save_recharge_order_id_len_less, order_num, payment_type, role_id, game_goods_id, order_id);
			}
			if (order_id_len > max_recharge_order_length)
			{
				send_recharge_end_result(e_recharge_error_db_save_recharge_order_id_len_bigger, order_num, payment_type, role_id, game_goods_id, order_id);
			}
			return;
		}
		if (role_id.is_valid() == false)
		{
			send_recharge_end_result(e_recharge_error_db_save_recharge_role_id_is_not_valid, order_num, payment_type, role_id, game_goods_id, order_id);
			return;
		}
		if (game_goods_id <= 0
			&& direct_diamond <= 0)
		{
			send_recharge_end_result(e_recharge_error_db_save_recharge_game_goods_id_less, order_num, payment_type, role_id, game_goods_id, order_id);
			return;
		}
		if (success <= 0)
		{
			send_recharge_end_result(e_recharge_error_db_save_recharge_success_less, order_num, payment_type, role_id, game_goods_id, order_id);
			return;
		}
		do_send_recharge_reward(order_id, role_id, game_goods_id, pay_price, order_num, payment_type, direct_diamond);
	}

	void recharge_mgr_ws::recharge_send_mail(const xchar* order_id, const guid_64& role_id, const int32& game_goods_id, float pay_price, int64 order_num, int32 payment_type, int32 direct_diamond)
	{
		if (order_id == nullptr)
		{
			send_recharge_end_result(e_recharge_error_recharge_send_mail_order_id_is_null, order_num, payment_type, role_id, game_goods_id, order_id);
			return;
		}
		xstring order_id_str = xstring(order_id);
		client_session* cur_session = client_session_mgr::getInstance().get_session(role_id);
		login_fixed_data temp_third_data;
		int32 login_type = e_login_type_new_account_and_password;
		s_unit_info *role_info_ptr = nullptr;
		if (cur_session)
		{
			temp_third_data = cur_session->m_login_third_data;
			login_type = cur_session->m_login_type;
			role_info_ptr = &cur_session->m_role_info;
		}
		server_log::recharge_step_log(order_id_str, role_id, game_goods_id, e_recharge_step_ws_recharge_send_mail, temp_third_data, login_type, role_info_ptr);
		int32 order_id_len = strlen(order_id);
		if (order_id_len <= 0 || order_id_len > max_recharge_order_length)
		{
			if (order_id_len <= 0)
			{
				send_recharge_end_result(e_recharge_error_recharge_send_mail_order_id_len_less, order_num, payment_type, role_id, game_goods_id, order_id);
			}
			if (order_id_len > max_recharge_order_length)
			{
				send_recharge_end_result(e_recharge_error_recharge_send_mail_order_id_len_bigger, order_num, payment_type, role_id, game_goods_id, order_id);
			}
			return;
		}
		if (role_id.is_valid() == false)
		{
			send_recharge_end_result(e_recharge_error_recharge_send_mail_role_id_is_not_valid, order_num, payment_type, role_id, game_goods_id, order_id);
			return;
		}
		if (game_goods_id <= 0
			&& direct_diamond <= 0)
		{
			send_recharge_end_result(e_recharge_error_recharge_send_mail_game_goods_id_less, order_num, payment_type, role_id, game_goods_id, order_id);
			return;
		}

		std::vector<std::string> title_params;
		title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_active_recharge_package_title));
		std::string title_text = init_unit::implode(title_params);
		std::string content_text = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_active_recharge_package_context);
		std::vector<s_item_template_info> drop_item_list;

		{
			RechargeTemplate* recharge_template_ptr = GET_TEMPLATE(RechargeTemplate, game_goods_id);
			if (recharge_template_ptr == nullptr)
			{
				if (direct_diamond <= 0)
				{
					send_recharge_end_result(e_recharge_error_recharge_by_template_id_send_mail_recharge_template_ptr_is_null, order_num, payment_type, role_id, game_goods_id, order_id);
					return;
				}
				if (cur_session)
				{
					server_log::recharge_log(cur_session->m_login_third_data, cur_session->m_role_info, 0, order_id, pay_price, direct_diamond, game_goods_id, 0, payment_type, login_type);
					set_log_var(log_head)
					cur_session->get_log_common_head_info(log_head);

					server_log::serverRechargeSuccess(log_head, cur_session->m_role_info.data_ary[e_role_info_vip_level], "", order_id, pay_price, 0, 0, 0, login_type,
					(payment_type > 0 ?  init_unit::change_i32_to_string(payment_type): cur_session->m_login_third_data.param14), direct_diamond, 0, 1, game_goods_id, 0, 
					cur_session->m_login_third_data.param8);


				}
				else
				{
					login_fixed_data third_info;
					s_unit_info role_info;
					role_info.role_guid = role_id;
					server_log::recharge_log(third_info, role_info, 0, order_id, pay_price, direct_diamond, game_goods_id, 0, payment_type, login_type);

					set_log_var(log_head)
					
					set_log_common_head_part3(log_head, "null", world_server::getInstance().get_server_id(), utility::get_tick_count())
					
					server_log::serverRechargeSuccess(log_head, 0, "", order_id, pay_price, 0, 0, 0, login_type,
					"", direct_diamond, 0, 1, game_goods_id, 0, "");
				}

				RechargeTemplate* custom_recharge_template_ptr = GET_TEMPLATE(RechargeTemplate, custom_recharge_id);
				if (custom_recharge_template_ptr == nullptr)
				{
					send_recharge_end_result(e_recharge_error_recharge_by_template_id_send_mail_recharge_template_ptr_is_null, order_num, payment_type, role_id, game_goods_id, order_id);
					return;
				}

				drop_item_list.push_back({ custom_recharge_template_ptr->OfflineItem , direct_diamond });
			}
			else
			{
				if (cur_session)
				{
					server_log::recharge_log(cur_session->m_login_third_data, cur_session->m_role_info, 0, order_id, pay_price, recharge_template_ptr->MoneyGetArray[1], game_goods_id, 0, payment_type, login_type);

					set_log_var(log_head)
					cur_session->get_log_common_head_info(log_head);
	
					server_log::serverRechargeSuccess(log_head, cur_session->m_role_info.data_ary[e_role_info_vip_level], "", order_id, pay_price, 0, 0, 0, login_type,
					(payment_type > 0 ? init_unit::change_i32_to_string(payment_type) : cur_session->m_login_third_data.param14), recharge_template_ptr->MoneyGetArray[1], 0, 1, game_goods_id, 0,
					cur_session->m_login_third_data.param8);

				}
				else
				{
					login_fixed_data third_info;
					s_unit_info role_info;
					role_info.role_guid = role_id;
					server_log::recharge_log(third_info, role_info, 0, order_id, pay_price, recharge_template_ptr->MoneyGetArray[1], game_goods_id, 0, payment_type, login_type);

					set_log_var(log_head)

					set_log_common_head_part3(log_head, "null", world_server::getInstance().get_server_id(), utility::get_tick_count())

					server_log::serverRechargeSuccess(log_head, 0, "", order_id, pay_price, 0, 0, 0, login_type,
							"", recharge_template_ptr->MoneyGetArray[1], 0, 1, game_goods_id, 0, "");
				}
				drop_item_list.push_back({ recharge_template_ptr->OfflineItem , 1});
			}
		}

		mail_ws_mgr::get_instance().send_mail_system(role_id, 0, drop_item_list, title_text, content_text);
		recharge_process_success(order_id, role_id, game_goods_id, order_num, payment_type);
		send_recharge_end_result(e_recharge_error_none, order_num, payment_type, role_id, game_goods_id, order_id);
		server_log::recharge_step_log(order_id_str, role_id, game_goods_id, e_recharge_step_ws_send_mail_end, temp_third_data, login_type, role_info_ptr);
	}

	void recharge_mgr_ws::recharge_process_success(const xchar* order_id, guid_64 role_id, int32 game_goods_id, int64 order_num, int32 payment_type)
	{
		if (order_id == nullptr)
		{
			send_recharge_end_result(e_recharge_error_send_save_recharge_end_order_id_is_null, order_num, payment_type, role_id, game_goods_id, order_id);
			return;
		}
		ws2dp_save_recharge_end req;
		int32 order_id_len = strlen(order_id);
		memcpy(req.order_id, order_id, order_id_len < max_recharge_order_length ? order_id_len : max_recharge_order_length);
		req.role_guid = role_id;
		req.game_goods_id = game_goods_id;
		req.order_num = order_num;
		req.payment_type = payment_type;
		ws_client::getInstance().send_to_dp(&req, sizeof(req));

		//client_session* session = client_session_mgr::getInstance().get_session(role_id);
		//if (session)
		//{
		//	session->send_to_client();
		//}
	}

	void recharge_mgr_ws::send_recharge_end_result(int32 result_type, int64 order_num, int32 payment_type, guid_64 role_guid, int32 goods_id, const xchar* order_id, bool is_first_recharge)
	{
		if (result_type != e_recharge_error_none)
		{
			server_log::recharge_error_log(order_id, role_guid, goods_id, result_type);
		}

		web_client::get_instance().send_recharge_end_result(result_type, order_num, payment_type);

		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (session)
		{
			faith::character_proto_recharge_end msg;
			msg.set_order_id(order_id);
			msg.set_goods_id(goods_id);
			msg.set_role_guid(role_guid.server_64);
			msg.set_payment_type(payment_type);
			msg.set_end_result(result_type);
			msg.set_is_first_recharge(is_first_recharge);
			session->send_to_client(&msg, e_msgindex_s2c_recharge_end);
		}
	}

	void recharge_mgr_ws::do_send_recharge_reward(const xchar* order_id, const guid_64& role_id, const int32& game_goods_id, float pay_price, int64 order_num, int32 payment_type, int32 direct_diamond)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_id);
		if (session && session->m_status == client_session::e_ss_ingame)
		{
			session->send_recharge(order_id, game_goods_id, pay_price, order_num, payment_type, direct_diamond);
		}
		else
		{
			recharge_send_mail(order_id, role_id, game_goods_id, pay_price, order_num, payment_type, direct_diamond);
		}
	}

	
	RechargeTemplate* recharge_mgr_ws::get_recharge_template_ptr(std::string product_id)
	{
		auto product_id_ite = m_product_id_map.find(product_id);
		if (product_id_ite == m_product_id_map.end())
		{
			return nullptr;
		}
		return GET_TEMPLATE(RechargeTemplate, product_id_ite->second);
	}

}