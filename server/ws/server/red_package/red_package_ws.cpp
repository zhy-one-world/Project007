#include "red_package_ws.h"
#include "ws_client.hpp"
#include "utility/cs_date.hpp"
#include "utility/guid_gen.h"
#include "utility/random.h"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "server/client_session.hpp"
#include "server/client_session_mgr.hpp"
#include "server/red_package/red_package_ws_mgr.h"
#include "net.pb.h"

namespace faith
{
	red_package_ws::red_package_ws()
	{
		clear_data();
	}

	red_package_ws::~red_package_ws()
	{


	}
	void red_package_ws::clear_data()
	{
		m_red_package_info.clear_data();
		m_red_package_template_ptr = nullptr;
		m_money_type = -1;
	}

	void red_package_ws::tick(float dt)
	{
	}

	int32 red_package_ws::init_by_info(int32 red_package_template_id, guid_64 sender_guid, int32 sender_vip_level, xstring sender_name, xstring text)
	{
		RedPackageTemplate* red_pack_template_ptr = GET_TEMPLATE(RedPackageTemplate, red_package_template_id);
		if (red_pack_template_ptr == nullptr
			|| red_pack_template_ptr->CanGetMoneyArray.size() < 2)
		{
			return e_item_string_unkown;
		}
		if (sender_name.length() > max_name_size)
		{
			return e_item_string_unkown;
		}
		if (text.length() > red_package_max_text_length)
		{
			return e_item_string_unkown;
		}

		clear_data();

		m_red_package_info.base_info.red_package_guid = guid_gen::make_guid(sender_guid);
		m_red_package_info.base_info.red_package_template_id = red_package_template_id;
		m_red_package_info.base_info.sender_guid = sender_guid;
		m_red_package_info.base_info.create_time = time_helper::get_cur_time_new().second;
		m_red_package_info.base_info.remain_times = red_pack_template_ptr->CanGetTimes;
		m_red_package_info.base_info.sender_vip_level = sender_vip_level;

		m_red_package_info.base_info.remain_money_num = red_pack_template_ptr->CanGetMoneyArray[1] - red_pack_template_ptr->CanGetTimes;
		//因为在实际 领取 红包时，会强行每人至少1块钱，所以在存储余额时，提前减掉 总人数 X 1块钱

		memcpy(m_red_package_info.base_info.sender_name, sender_name.c_str(), sender_name.length());
		memcpy(m_red_package_info.base_info.text, text.c_str(), text.length());

		//不可单独设置，会在 get_XXX 函数中自动设置
		//m_red_package_template_ptr = red_pack_template_ptr;
		//m_money_type = red_pack_template_ptr->CanGetMoneyArray[0];

		for (int32 i = 0; i < red_package_max_receiver_num; ++i)
		{
			m_red_package_info.receiver_info_list[i].red_package_guid = m_red_package_info.base_info.red_package_guid;
		}
		switch (red_pack_template_ptr->Type)
		{
		case e_red_package_type_globel :
		{
			return e_red_bag_get_globel_success;
		}
		break;
		case e_red_package_type_legion :
		{
			return e_red_bag_get_legion_success;
		}
		break;
		case e_red_package_type_vip :
		{
			return e_red_bag_get_vip_success;
		}
		break;
		default:
			break;
		}
		return e_red_bag_send_success;
	}

	int32 red_package_ws::find_empty_receiver_list_slot()
	{
		for (int32 i = 0; i < red_package_max_receiver_num; ++i)
		{
			if (!m_red_package_info.receiver_info_list[i].receiver_guid.is_valid())
			{
				return i;
			}
		}
		return -1;
	}

	bool red_package_ws::is_in_receiver_list(guid_64 receiver_guid)
	{
		for (int32 i = 0; i < red_package_max_receiver_num; ++i)
		{
			if (m_red_package_info.receiver_info_list[i].receiver_guid == receiver_guid)
			{
				return true;
			}
		}
		return false;
	}

	RedPackageTemplate* red_package_ws::get_template_ptr()
	{ 
		if (m_red_package_template_ptr == nullptr)
		{
			m_red_package_template_ptr = GET_TEMPLATE(RedPackageTemplate, m_red_package_info.base_info.red_package_template_id);
		}
		return m_red_package_template_ptr; 
	}

	int32 red_package_ws::get_money_type() 
	{ 
		if (m_money_type < 0
			|| m_money_type >= e_money_type_max)
		{
			if (get_template_ptr() != nullptr
				&& get_template_ptr()->CanGetMoneyArray.size() >= 2)
			{
				return m_money_type = get_template_ptr()->CanGetMoneyArray[0];
			}
		}
		return m_money_type; 
	}

	void red_package_ws::set_base_info(s_red_package_base temp_info)
	{
		//服务器刚开机，这里会读出空表格,所以不可设置
		//RedPackageTemplate* red_pack_template_ptr = GET_TEMPLATE(RedPackageTemplate, temp_info.red_package_template_id);
		//if (red_pack_template_ptr == nullptr
		//	|| red_pack_template_ptr->CanGetMoneyArray.size() < 2)
		//{
		//	return;
		//}
		clear_data();
		m_red_package_info.base_info = temp_info;
		for (int32 i = 0; i < red_package_max_receiver_num; ++i)
		{
			m_red_package_info.receiver_info_list[i].red_package_guid = temp_info.red_package_guid;
		}
	}

	void red_package_ws::add_receiver(s_red_package_receiver receiver_info)
	{
		if (!receiver_info.receiver_guid.is_valid())
		{
			return;
		}
		int32 temp_slot = m_red_package_info.get_empty_receiver_slot();
		if (temp_slot >= 0
			&& temp_slot < red_package_max_receiver_num)
		{
			m_red_package_info.receiver_info_list[temp_slot] = receiver_info;
		}
	}

	int32 red_package_ws::draw_red_package(guid_64 receiver_guid, xstring receiver_name)
	{
		if (is_in_receiver_list(receiver_guid))
		{
			return -1;
		}
		client_session* session = client_session_mgr::getInstance().get_session(receiver_guid);
		if (session == nullptr)
		{
			return -1;
		}

		int32 temp_num = get_random_money_num();
		if (temp_num < 0)
		{
			return -1;
		}
		int32 final_num = temp_num + 1;  //至少给1块钱，这个随机出来的数不含1块钱，直接加就行，总钱数在创建红包时就已经做过处理了
		m_red_package_info.base_info.remain_times--;
		m_red_package_info.base_info.remain_money_num -= temp_num;

		int32 empty_receiver_slot = find_empty_receiver_list_slot();
		if (empty_receiver_slot >= 0
			&& empty_receiver_slot < red_package_max_receiver_num)
		{
			m_red_package_info.receiver_info_list[empty_receiver_slot].receiver_guid = receiver_guid;
			m_red_package_info.receiver_info_list[empty_receiver_slot].money_num = final_num;
			int32 name_len = receiver_name.length();
			if (name_len > max_name_size)
			{
				name_len = max_name_size;
			}
			memcpy(m_red_package_info.receiver_info_list[empty_receiver_slot].role_name, receiver_name.c_str(), name_len);
			//服务端的抽奖人不排序，发到客户端自己排

			server2dp_proto_ws2dp_add_red_package_receiver_to_sql add_msg;
			red_package_ws_mgr::get_instance().set_red_package_receiver_proto(add_msg.mutable_red_info(), m_red_package_info.receiver_info_list[empty_receiver_slot]);
			ws_client::getInstance().send_to_dp_new(&add_msg, e_msg_ws2dp_add_red_pack_receiver_to_sql);
			//ws2dp_add_red_package_receiver_to_sql add_msg;
			//add_msg.receiver_info = m_red_package_info.receiver_info_list[empty_receiver_slot];
			//ws_client::getInstance().send_to_dp(&add_msg, sizeof(ws2dp_add_red_package_receiver_to_sql));
		}
		return final_num;
	}

	int32 red_package_ws::get_random_money_num()
	{
		if (m_red_package_info.base_info.remain_times <= 0)
		{//不能拿剩余钱数做判断，剩余钱数是可能等于0的，因为默认的1块钱并不算入 剩余钱数 中
			return -1;
		}
		//虽然默认给分了一块钱，但不能在这随机时候处理，因为 新剩余钱数 = 原剩余钱 - 随机数（此函数返回值）
		//如果此时就 +1 ，那之后的逻辑就等于用 原完全随机的数值 减掉了 完全随机出出的值+不随机的基础值（最低的那1块钱）
		//这样会造成在最后一个人时候，永远会多给出1块钱
		if (m_red_package_info.base_info.remain_money_num <= 0)
		{
			return 0;
		}
		else if(m_red_package_info.base_info.remain_times == 1)
		{
			return m_red_package_info.base_info.remain_money_num;
		}
		else
		{
			int32 max_num = m_red_package_info.base_info.remain_money_num / m_red_package_info.base_info.remain_times * 2;
			//最大值是2倍平均值

			int32 random_value = random_gen::get_random(0, max_num);
			return random_value;
		}
		return -1;
	}

	void red_package_ws::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		if (m_red_package_info.base_info.sender_guid == role_guid)
		{
			m_red_package_info.base_info.set_sender_name(role_name);
		}

		for (int32 i = 0; i < red_package_max_receiver_num; ++i)
		{
			if (m_red_package_info.receiver_info_list[i].receiver_guid)
			{
				m_red_package_info.receiver_info_list[i].set_role_name(role_name);
			}	
		}
	}
}
