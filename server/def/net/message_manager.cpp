/********************************************************************
	created:	2014/07/31
	created:	31:7:2014   15:20
	file base:	message_manager
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "message_manager.hpp"
#include <base.hpp>
#include "template/template_manager.h"
#include "lua/script_mgr.h"
#include "utility/parse_msg.h"
#include "tolua++.h"
#include "server_log.hpp"


namespace hld
{
	TOLUA_API int  tolua_logic_open(lua_State* tolua_S);

	void none_msg_fuction(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		return;
	}
	message_manager::message_manager() 
	{
		for (int32 i = 0; i < e_msg_base_max;++i)
		{
			register_handler(i, none_msg_fuction);
		}
		m_server_type = e_server_type_invalid;
	}

	message_manager::~message_manager() 
	{
	}
	bool message_manager::register_handler(ui16 packet_id, const on_recv_handler_type& recv_handler)
	{
		m_handler_map[packet_id] = recv_handler;
		return true;
	}
	void message_manager::on_data_received(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		ZoneScoped;
		const packet_base* pPacket = static_cast<const packet_base*>(data_ptr);
		if (NULL == pPacket)
		{
			return;
		}
		if (pPacket->wheader < 0 || pPacket->wheader >= e_msg_base_max)
		{
			return;
		}
		int32 wheader = pPacket->wheader;

		LuaMessageTemplate* LuaMessageTemplatePtr = GET_TEMPLATE(LuaMessageTemplate, pPacket->wheader);

		if (LuaMessageTemplatePtr && LuaMessageTemplatePtr->UseLua == m_server_type)
		{
			if (LuaMessageTemplatePtr->UseLua == (int32)e_server_type_dp)   
			{

				//CONSOLE_INFO("on_data_received_dp") << pPacket->wheader);

				packet_s2s* pPacket = (packet_s2s*)data_ptr;
				//script_mgr::get_instance().call_func(nullptr, "hotupdate", 0, false, "");
				script_mgr::get_instance().call_func(nullptr, "on_data_received_dp", 0, true, "%s%d%d", pPacket->google_data, pPacket->google_data_len, pPacket->wheader, conn_index);
				return;
			}

			if (LuaMessageTemplatePtr->UseLua == (int32)e_server_type_cs)
			{
				if (wheader > e_msg_c2s_s2c_begin && wheader < e_msg_c2s_s2c_end) // client--->server
				{
					s_client_uid client_uid;
					parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
					packet_c2s_s2c* pPacket = (packet_c2s_s2c*)data_ptr;
				    //script_mgr::get_instance().call_func(nullptr,"hotupdate", 0, false, "");
					script_mgr::get_instance().call_func(nullptr, "on_data_received_cs", 0, true, "%s%d%d", pPacket->google_data, pPacket->google_data_len, pPacket->wheader, client_uid.fepsession_uid);
					return;
				}

				//CONSOLE_INFO("on_data_received") << pPacket->wheader);
				packet_s2s* pPacket = (packet_s2s*)data_ptr;
				//script_mgr::get_instance().call_func(nullptr, "hotupdate", 0, false, "");
				script_mgr::get_instance().call_func(nullptr, "on_data_received_ss", 0, true, "%s%d%d", pPacket->google_data, pPacket->google_data_len, pPacket->wheader, conn_index);
				return;
			}

			else if (LuaMessageTemplatePtr->UseLua == (int32)e_server_type_ws)
			{
				if (wheader > e_msg_c2s_s2c_begin && wheader < e_msg_c2s_s2c_end) // client--->server
				{
					s_client_uid client_uid;
					parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
					packet_c2s_s2c* pPacket = (packet_c2s_s2c*)data_ptr;
					//script_mgr::get_instance().call_func(nullptr, "hotupdate", 0, false, "");
					script_mgr::get_instance().call_func(nullptr, "on_data_received_cs", 0, true, "%s%d%l", pPacket->google_data, pPacket->google_data_len, pPacket->wheader, client_uid.fep_uid_64);
					return;
				}
				//CONSOLE_INFO("on_data_received_ws") << pPacket->wheader);
				packet_s2s* pPacket = (packet_s2s*)data_ptr;
				script_mgr::get_instance().call_func(nullptr, "on_data_received_ws", 0, true, "%s%d%d", pPacket->google_data, pPacket->google_data_len, pPacket->wheader, conn_index);
				return;
			}

		}
		else
		{
			m_handler_map[pPacket->wheader](conn_index, data_ptr, data_len);
		}
	}
}
