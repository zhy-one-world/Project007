 /*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-2-25 11:06
	
	File Name :	gate_client.hpp
	
	Author : zhangjianyu (zhangjianyu@163.com)
	
	Description : 
	
	Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#ifndef _GATE_CLIENT_H_
#define _GATE_CLIENT_H_

#include <singleton.hpp>
#include "server_def.hpp"
#include <net/net_client_mgr.hpp>

namespace hld
{
	using namespace net;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Class gate_client Declare
	//
	//////////////////////////////////////////////////////////////////////////
	class gate_client :	public singleton<gate_client>
	{
		friend class singleton<gate_client>;
	private:
		gate_client(void);
	public:	
		~gate_client(void);
	public:
		bool start();
		void stop();
	private:
		void on_conn_status(const net_client* faith_client_ptr);
		void on_conn_closed(const net_client* faith_client_ptr);
	private:
		s_server_info m_gate_info;
		uint32 m_gate_conn_index;
	};

}

#endif//#define __GATE_CLIENT_H_
