/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-4-25 20:56
	
	File Name :	gate_proxy.hpp
	
	Author : zhangming 
	
	Description : 
	
	Change List :


@@*/

#ifndef _GATE_PROXY_HPP_
#define _GATE_PROXY_HPP_

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include "logic/type_def.hpp"
#include <singleton.hpp>
#include <net/net_client_mgr.hpp>
#include "server_def.hpp"
#include "Logic/gate_def.hpp"

namespace hld
{
	using namespace net;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Class Declare
	//
	//////////////////////////////////////////////////////////////////////////
	class gate_proxy : public singleton<gate_proxy>
	{
		friend class singleton<gate_proxy>;
	private:
		gate_proxy();
		void	on_conn_status(const net_client* faith_client_ptr);
		void	on_conn_closed(const net_client* faith_client_ptr);
	public:
		bool	start();
		void	send_message(const void *data_ptr, size_t data_len);
		bool	is_gate_run()	{ return m_status==E_GATE_REGISTERED; }
		void	set_status(int32 status);
	private:
		// gate conn info.
		s_server_info   m_server_info;
		uint32			m_connIndex;
		int32			m_status;
	};
}


#endif