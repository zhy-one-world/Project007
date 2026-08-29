//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include "connection/daemon_client.hpp"
#include "game_cfg/servers_config.h"
#include "http/http_access_mgr.hpp"
#include "log/server_log.hpp"
#include "Logic/time_def.hpp"
#include <json/json.h>
#include <TlHelp32.h> 
#include "net/net_client.hpp"
#include "serializer.hpp"


//////////////////////////////////////////////////////////////////////////
//
//	Macro And Struct Define
//
//////////////////////////////////////////////////////////////////////////
using namespace faith;
faith::allocator g_DataSerializerAlocator;

//////////////////////////////////////////////////////////////////////////
//
//	Class Implement
//
//////////////////////////////////////////////////////////////////////////
daemon_client::daemon_client()
{
	m_last_pingeye_time = 0;
	m_server_close = false;
	m_server_type = e_server_type_invalid;
	m_gameserver_id = 0;
	m_process_error = "";
	m_server_error = "";
}

void daemon_client::init(e_server_type type, uint32 gameserver_id, onrecved_handler_type onrecved_handler)
{
	m_server_type = type;
	m_gameserver_id = gameserver_id;
	m_onrecved_handler = onrecved_handler;
}

void daemon_client::on_data_received(const net_client* faith_client_ptr, const void *data_ptr, size_t data_len)
{
	m_onrecved_handler(data_ptr, data_len);
}

void daemon_client::send_server_info()
{
	Json::Value param_json;
	param_json["param"] = m_server_error;
	param_json["type"] = m_server_type;
	param_json["serverid"] = m_gameserver_id;
	Json::FastWriter fast_writer;
	xstring temp_param = fast_writer.write(param_json);
	xstring daemon_address = SERVERCONFIG->daemon_address;
	std::vector<xstring> head_list;
	head_list.push_back("Content-Type:application/json");
	//head_list.push_back("application/json");
	http_access_mgr::get_instance().async_request
	(
		0,
		daemon_address,
		head_list,
		temp_param,
		boost::bind(&daemon_client::send_server_info_handle, this, _1, _2, _3, _4),
		e_http_request_type_post
	);
}

void daemon_client::send_server_info_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result)
{
	if (http_error_code != 0)
	{
		std::cout << http_error_code <<" "<< http_error_info << " " << http_result << std::endl;
	}
}
void daemon_client::heart_tick(const int64& new_time)
{
	ZoneScoped;
	static int64 tick_time = 0;
	if (new_time >= tick_time)
	{
		set_process_num_error_string();
		tick_time = new_time + minute_tick_time;
	}
	if (new_time >= m_last_pingeye_time)
	{
		m_last_pingeye_time = new_time + 4000;
		send_server_info();
		m_server_error = "";
	}

}

void daemon_client::set_process_num_error_string()
{
	m_process_error = "";
	xstring server_process_name = e_server_type_str[m_server_type];
	server_process_name += ".exe";


	int32 process_num = 0;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	bool bResult = Process32First(hProcessSnap, &pe32);

	for (int32 i = 0; i < process_check_max_num; ++i)
	{
		if (!bResult)
		{
			break;
		}

		if (pe32.szExeFile == server_process_name)
		{
			process_num++;
		}

		bResult = Process32Next(hProcessSnap, &pe32);
	}

	CloseHandle(hProcessSnap);

	if (process_num != 1)
	{
		std::stringstream temp_stream;
		temp_stream << "ProcessCountWrong,CurrentProcessNumIs-----";
		temp_stream << process_num;
		temp_stream >> m_process_error;
	}
}
