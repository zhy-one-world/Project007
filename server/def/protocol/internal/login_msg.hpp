/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 21:46
  file base: login
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_LOGIN_HPP_
#define _FAITH_LOGIN_HPP_

#include "logic/type_def.hpp"
#include "Logic/login_def.hpp"
#include "Logic/char_def.hpp"
#include "base.hpp"
#include "Logic/gate_def.hpp"
#include "internet/error.pb.h"

namespace faith 
{
#pragma pack(push,1)

	enum
	{
		// login
		e_msg_index_fep2ls_client_login = e_msg_base_login,
		e_msg_index_ls2fep_client_login,						
		e_msg_index_ls2dp_client_login,				
		e_msg_index_dp2ls_client_login,
		e_msg_index_fep2ws_client_reconnect,
		e_msg_index_fep2ws_client_logined,
		e_msg_index_fep2ws_leave_queued,
		e_msg_index_ws2fep_client_logined,	
		e_msg_index_c2fep_token_login,
		e_msg_index_fep2ls_client_login_token,
		e_msg_index_ls2dp_client_login_token,
		e_msg_index_ws2ls_reactive_client,
		e_msg_index_ws2c_login_queue_status,
		// logout
		e_msg_index_fep2ws_client_logout,
		e_msg_index_ws2cs_client_logout,
		e_msg_index_cs2ws_client_logout,
		// account
		e_msg_index_fep2ws_disable_account,
		e_msg_index_ws2fep_kickout_account,
		// check
		e_msg_index_ls2dp_check_cluster_login_key,
		e_msg_index_dp2ls_check_cluster_login_key,
		e_msg_index_fep2c_captcha_check,
		e_msg_index_c2fep_captcha_check,
		// encrypt
		e_msg_index_c2fep_encrypted_data,
		e_msg_index_c2fep_encrypted_data_,
		// captcha
		e_msg_index_c2fep_refresh_login_captcha,
		e_msg_index_fep2c_refresh_login_captcha,

	};

	/************************************************************************/
	/*           Login                                                      */
	/************************************************************************/

	// 登陆时ls向fep发送信息
	struct ls2fep_client_login : public packet_base		
	{
		s_client_uid								client_uid;
		int32										eResult;
		xchar										account[max_account_length + 1];
		int32										login_type;
		int32										login_type_plus;
		xchar										server_msg[max_server_msg_length + 1];
		int32										jewel_num;
		int32										radio_host;
		int64										ban_role_array[faith::max_character_num];
		int64										ban_chat_array[faith::max_character_num];
		bool										is_create_new;
		int32										online_duration;
		int32										online_time;
		int32										age;
		int32										month_recharge_num;
		int32										month_recharge_time;
		ls2fep_client_login()
		{
			memset(this,0,sizeof(*this));
			wheader  = e_msg_index_ls2fep_client_login;
		}
	};

	struct ls2dp_client_login : public packet_base
	{
		s_client_uid				client_uid;
		int32						server_id;
		xchar						account[max_account_length+1];
		xchar						password[max_password_length +1];
		int32						login_type;
		int32						login_type_plus;
		xchar						server_msg[max_server_msg_length + 1];
		xchar						create_ip[max_ip_address_length + 1];
		xchar						device_type[max_device_type_length + 1];
		int64						ban_role_array[faith::max_character_num];
		int64						ban_chat_array[faith::max_character_num];
		int32						age;

		ls2dp_client_login()
		{
			memset(this,0,sizeof(*this));
			wheader= e_msg_index_ls2dp_client_login;
			login_type = e_login_type_new_max;
			login_type_plus = 0;
			age = 0;
		}
	};

	struct dp2ls_client_login : public packet_base
	{
		s_client_uid						client_uid;
		int32								e_result;
		xchar								account[max_account_length + 1];
		xchar								server_msg[max_server_msg_length + 1];
		int32								login_type;
		int32								login_type_plus;
		int32								jewel_num;
		int32								radio_host;
		int64								ban_role_array[faith::max_character_num];
		int64								ban_chat_array[faith::max_character_num];
		bool								is_create_new;
		int32								online_duration;
		int32								online_time;
		int32								age;
		int32								month_recharge_num;
		int32								month_recharge_time;
		dp2ls_client_login()
		{
			memset(this,0,sizeof(*this));
			wheader= e_msg_index_dp2ls_client_login;
		}
	};
	// 登陆成功时fep向ws发送信息
	struct fep2ws_client_reconnect : public packet_base
	{
		s_client_uid								client_uid;
		xchar										account[max_account_length + 1];
		int32										cur_map_id;
		int32										cur_map_line_id;
		fep2ws_client_reconnect()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_fep2ws_client_reconnect;
		}
	};

	struct fep2ws_leave_queued : public packet_base
	{
		s_client_uid								client_uid;
		xchar										account[max_account_length + 1];
		fep2ws_leave_queued()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_fep2ws_leave_queued;
		}
	};

	// 登陆成功时fep向ws发送信息
	struct fep2ws_client_logined : public packet_base
	{
		s_client_uid								client_uid;
		xchar										account[max_account_length+1];
		int32										login_type;
		int32										login_type_plus;
		login_fixed_data							fixed_data;
		guid_64										role_guid;
		guid_64										map_guid;
		int32										war_index;
		int32										jewel_num;
		int32										server_id;
		int64										ban_role_array[faith::max_character_num];
		int64										ban_chat_array[faith::max_character_num];
		fep2ws_client_logined()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msg_index_fep2ws_client_logined;
		}
	};

	struct ws2fep_client_logined : public packet_base
	{
		s_client_uid									client_uid;
		e_error_code									eResult;
		int32											login_type;
		int32											login_type_plus;
		uint32											queue_pos;
		uint32											left_time_in_sec;
		ui16											data_len;
		xchar											account[max_account_length + 1];

		ws2fep_client_logined()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msg_index_ws2fep_client_logined;
		}
	};

	struct ws2fep_login_queue_status : public packet_base
	{
		uint32					left_time_in_sec;		//	估计剩余时间
		uint32					queue_pos;				//	在队列中的位置
		s_client_uid				client_uid;

		ws2fep_login_queue_status()
		{
			memset(this,0,sizeof(*this));
			wheader = e_msg_index_ws2c_login_queue_status;
		}
	};
	/************************************************************************/
	/*           Logout                                                      */
	/************************************************************************/
	struct fep2ws_client_logout : public packet_base
	{
		s_client_uid	client_uid;
		e_logout_result eReason;
		bool			need_save_online_time;
		xchar			account[max_account_length + 1];
		int32			online_duration;
		int32			online_time;
		int32			month_recharge_num;
		int32			month_recharge_time;
		fep2ws_client_logout()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msg_index_fep2ws_client_logout;
		}
	};

	struct ws2cs_client_logout : public packet_base
	{
		s_client_uid		client_uid;
		guid_64				role_guid;
		bool				need_send_save_end;
		ws2cs_client_logout()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msg_index_ws2cs_client_logout;
		}
	};

	struct cs2ws_client_logout : public packet_base
	{
		guid_64		player_guid;
		bool		bCS;
		e_logout_result logout_result;

		cs2ws_client_logout()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msg_index_cs2ws_client_logout;
		}
	};

	/************************************************************************/
	/*           account                                                    */
	/************************************************************************/
	struct fep2ws_disable_account : public packet_base
	{
		s_client_uid	client_uid;

		xchar													 desc[limit_disable_account_desc_length+1];
		int32													 reason;
		uint32													  minutes;

		fep2ws_disable_account()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_fep2ws_disable_account;
		}
	};

	struct ws2fep_kickout_account : public packet_base
	{
		s_client_uid		client_uid;
		e_logout_result		reason;

		ws2fep_kickout_account()
		{ 
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2fep_kickout_account;
		}
	};


	/************************************************************************/
	/*           check                                                      */
	/************************************************************************/
	struct ls2dp_check_cluster_login_key : public packet_base
	{ 
		s_client_uid	client_uid;
		uint32					account_id;
		xchar					ip_address[max_ip_address_length+1];
		xchar					cluster_key[limit_cluster_login_key_length];

		ls2dp_check_cluster_login_key()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msg_index_ls2dp_check_cluster_login_key;
		}
	};

	struct dp2ls_check_cluster_login_key : public packet_base
	{
		enum
		{
			e_failed_unknow_error = 0,		//	未知/内部错误
			e_success = 1,					//	成功并返回数据
			e_failed_invalid_password,		//	错误的KEY
			e_failed_server_internal_error,	//	服务器内部错误

		}eResult;

		s_client_uid			client_uid;

		dp2ls_check_cluster_login_key()
		{
			wheader= e_msg_index_dp2ls_check_cluster_login_key;
			eResult=e_failed_unknow_error;
		}
	};

	struct fep2c_check_captcha : public packet_base
	{
		enum{	datalen_limit = 8000,datafile_extname_len = 4, answer_len = 10, answer_num = 4	};
		enum
		{
			e_captcha_create_char,
			e_captcha_create_char_retry,	
			e_cpatcha_enter_game,
			e_cpatcha_enter_game_retry,	
			e_captcha_ingame_normal,
			e_captcha_ingame_retry,			
		};

		ui8		type;									
		xchar	file_type[datafile_extname_len + 1];	//验证码图片文件的格式（bmp/jpg/gif ...），如果fep没有使用验证码，则file_type="",data_len=0
		xchar	answer[answer_num][answer_len + 1];
		uint32	data_len;
		xchar	data[datalen_limit];

		i64 get_pak_length() const
		{
			const i64 basic_len = (ULONG_PTR)&data - (ULONG_PTR)&wheader;
			return basic_len + data_len;
		}

		fep2c_check_captcha()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_fep2c_captcha_check;
		}
	};

	struct c2fep_check_captcha : public packet_base
	{
		xchar	captcha_str[max_captcha_length + 1];		// captcha_str for login
		c2fep_check_captcha()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_c2fep_captcha_check;
		}
	};	

	/************************************************************************/
	/*         Encryp                                                       */
	/************************************************************************/
	enum e_encrypt_level
	{
		e_lv_high,		// 加密级别高
		e_lv_mid,		// 加密级别中
		e_lv_low,		// 加密级别低
		// 不需加密的不用注册
	};

	struct aes_encrypt_data : public packet_base
	{
		ui8	data[1];
	};
	

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	struct c2fep_refresh_login_captcha : public packet_base
	{
		c2fep_refresh_login_captcha()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_c2fep_refresh_login_captcha;
		}
	};

	struct fep2c_refresh_login_captcha : public packet_base
	{
		enum
		{
			e_failed_unknown_error			= 0,
			e_success						= 1,
			e_success_no_captcha,					//未开启Captcha
			e_failed_captcha_times_limit,			//达到次数限制
		}
		e_result;

		enum{	datalen_limit = 8000,datafile_extname_len = 4	};

		xchar	file_type[datafile_extname_len + 1];	//验证码图片文件的格式（bmp/jpg/gif ...），如果fep没有使用验证码，则file_type="",data_len=0
		uint32	data_len;
		xchar	data[datalen_limit];

		i64 get_pak_length() const
		{
			const i64 basic_len = (ULONG_PTR)&data - (ULONG_PTR)&wheader;
			return basic_len + data_len;
		}

		fep2c_refresh_login_captcha()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_fep2c_refresh_login_captcha;
		}
	};

#pragma pack(pop)

}

#endif 
