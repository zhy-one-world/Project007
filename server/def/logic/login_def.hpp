/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 15:49
  file base: login_def
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _LOGIN_DEF_HPP_
#define _LOGIN_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"

namespace faith 
{
#pragma pack(push,1)

#define SIGN_PARAM_NUM			6
#define HTTP_HEAD_PARAM_NUM		6
#define	MD5_CHAR_NUM			129

	enum e_enum_buff_info
	{
		e_enum_buff_info_id,
		e_enum_buff_info_effect_index,
		e_enum_buff_info_max
	};

	const int32 MAX_LS_ENUM_BUFF = 40;
	const int32 MAX_LS_ENUM_SPIRIT = 6;
	const int32 MAX_LS_ENUM_TIME = 2;

	const int32 max_enum_buff_data_num = MAX_LS_ENUM_BUFF * e_enum_buff_info_max;
	const int32 max_enum_character_num = 3;

	const int32 login_proto_keep_time = 30;
	const int32 e_login_fixed_data_lenght = 128;

	enum e_del_char_type
	{
		e_del_char_del_resume, //0暂时删除 可恢复
		e_del_char_resume, //1恢复
		e_del_char_delete, //2彻底删除
		e_del_char_none, //最后一位 表示错误
	};

	struct login_fixed_data				// 用于通过第三方sdk登陆保存的固定数据
	{
		login_fixed_data()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		xchar param1[e_login_fixed_data_lenght];		//app key
		xchar param2[e_login_fixed_data_lenght];		//客户端版本
		xchar param3[e_login_fixed_data_lenght];		//游戏服务器ID
		xchar param4[e_login_fixed_data_lenght];		//MediaId
		xchar param5[e_login_fixed_data_lenght];		//设备唯一标识
		xchar param6[e_login_fixed_data_lenght];		//登录IP
		xchar param7[e_login_fixed_data_lenght];		//ChannelId
		xchar param8[e_login_fixed_data_lenght];		//model
		xchar param9[e_login_fixed_data_lenght];		//idfa
		xchar param10[e_login_fixed_data_lenght];		//imei
		xchar param11[e_login_fixed_data_lenght];		//android_id
		xchar param12[e_login_fixed_data_lenght];		//mac
		xchar param13[e_login_fixed_data_lenght];		//sn
		xchar param14[e_login_fixed_data_lenght];		//recharge_type
		xchar param15[e_login_fixed_data_lenght];		//version_code
		void set_proto_info(server2dp_proto_login_fixed_data * _info) const
		{
			if (nullptr == _info)
			{
				return;
			}
			_info->set_param1(param1);
			_info->set_param2(param2);
			_info->set_param3(param3);
			_info->set_param4(param4);
			_info->set_param5(param5);
			_info->set_param6(param6);
			_info->set_param7(param7);
			_info->set_param8(param8);
			_info->set_param9(param9);
			_info->set_param10(param10);
			_info->set_param11(param11);
			_info->set_param12(param12);
			_info->set_param13(param13);
			_info->set_param14(param14);
			_info->set_param15(param15);
		}
		void proto_set_info(server2dp_proto_login_fixed_data _info)
		{
			clear_data();
			memcpy(param1, _info.param1().c_str(), _info.param1().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param1().size());
			memcpy(param2, _info.param2().c_str(), _info.param2().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param2().size());
			memcpy(param3, _info.param3().c_str(), _info.param3().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param3().size());
			memcpy(param4, _info.param4().c_str(), _info.param4().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param4().size());
			memcpy(param5, _info.param5().c_str(), _info.param5().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param5().size());
			memcpy(param6, _info.param6().c_str(), _info.param6().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param6().size());
			memcpy(param7, _info.param7().c_str(), _info.param7().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param7().size());
			memcpy(param8, _info.param8().c_str(), _info.param8().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param8().size());
			memcpy(param9, _info.param9().c_str(), _info.param9().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param9().size());
			memcpy(param10, _info.param10().c_str(), _info.param10().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param10().size());
			memcpy(param11, _info.param11().c_str(), _info.param11().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param11().size());
			memcpy(param12, _info.param12().c_str(), _info.param12().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param12().size());
			memcpy(param13, _info.param13().c_str(), _info.param13().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param13().size());
			memcpy(param14, _info.param14().c_str(), _info.param14().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param14().size());
			memcpy(param15, _info.param15().c_str(), _info.param15().size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : _info.param15().size());
		}
		bool to_proto(faith::st_proto::st_login_fixed_data* st_login_ptr)
		{
			if (st_login_ptr == nullptr)
			{
				return false;
			}
			st_login_ptr->set_param1(param1);
			st_login_ptr->set_param2(param2);
			st_login_ptr->set_param3(param3);
			st_login_ptr->set_param4(param4);
			st_login_ptr->set_param5(param5);
			st_login_ptr->set_param6(param6);
			st_login_ptr->set_param7(param7);
			st_login_ptr->set_param8(param8);
			st_login_ptr->set_param9(param9);
			st_login_ptr->set_param10(param10);
			st_login_ptr->set_param11(param11);
			st_login_ptr->set_param12(param12);
			st_login_ptr->set_param13(param13);
			st_login_ptr->set_param14(param14);
			st_login_ptr->set_param15(param15);
			return true;
		}
		void my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::st_proto::st_login_fixed_data&  st_login_ref)
		{
			clear_data();
			my_memcopy_string(param1, e_login_fixed_data_lenght, st_login_ref.param1());
			my_memcopy_string(param2, e_login_fixed_data_lenght, st_login_ref.param2());
			my_memcopy_string(param3, e_login_fixed_data_lenght, st_login_ref.param3());
			my_memcopy_string(param4, e_login_fixed_data_lenght, st_login_ref.param4());
			my_memcopy_string(param5, e_login_fixed_data_lenght, st_login_ref.param5());
			my_memcopy_string(param6, e_login_fixed_data_lenght, st_login_ref.param6());
			my_memcopy_string(param7, e_login_fixed_data_lenght, st_login_ref.param7());
			my_memcopy_string(param8, e_login_fixed_data_lenght, st_login_ref.param8());
			my_memcopy_string(param9, e_login_fixed_data_lenght, st_login_ref.param9());
			my_memcopy_string(param10, e_login_fixed_data_lenght, st_login_ref.param10());
			my_memcopy_string(param11, e_login_fixed_data_lenght, st_login_ref.param11());
			my_memcopy_string(param12, e_login_fixed_data_lenght, st_login_ref.param12());
			my_memcopy_string(param13, e_login_fixed_data_lenght, st_login_ref.param13());
			my_memcopy_string(param14, e_login_fixed_data_lenght, st_login_ref.param14());
			my_memcopy_string(param15, e_login_fixed_data_lenght, st_login_ref.param15());
		}
	};

	struct player_info_enter_game
	{
		player_info_enter_game()
		{
			memset(this,0, sizeof(*this));
		}
		guid_64			role_guid;										// 所选进入游戏的角色id
		uint32			scene_uid;										// 角色所在场景id
		uint32			line_uid;										// 角色所在新手村分线id
		int32			array_index;									// 管理器下標
		s_map_pos		map_pos;										// 角色的地图位置
	};
#pragma pack(pop)
}

#endif
