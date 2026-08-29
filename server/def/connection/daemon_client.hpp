#ifndef _DAEMON_CLIENT_HPP_
#define _DAEMON_CLIENT_HPP_

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include <singleton.hpp>
#include "logic/type_def.hpp"
#include "server_def.hpp"

//////////////////////////////////////////////////////////////////////////
//
//	Class Declare
//
//////////////////////////////////////////////////////////////////////////



namespace faith
{
	const int32 process_check_max_num = 400;

	using namespace net;

	class net_client;

	class daemon_client : public singleton<daemon_client>
	{
		friend class singleton<daemon_client>;
		typedef boost::function<void(const void*, size_t)>	onrecved_handler_type;
	public:
		void init(e_server_type type, uint32 gameserver_id, onrecved_handler_type onrecved_handler);
		void send_server_info();
		void send_server_info_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result);
		void heart_tick(const int64& new_time);
		bool get_server_close() { return m_server_close; }
		void set_server_close(bool server_close) { m_server_close = server_close; }
		xstring& get_process_num_error_string() { return m_process_error; }
		void set_process_num_error_string();
		void set_server_error_string(const xstring& server_error) { m_server_error = m_process_error + server_error; }
	public:
		void	on_data_received(const net_client* faith_client_ptr, const void *data_ptr, size_t data_len);
	private:
		daemon_client();
	private:
		e_server_type	m_server_type;
		uint32			m_gameserver_id;
		int64			m_last_pingeye_time;
		bool			m_server_close;
		xstring			m_process_error;
		xstring			m_server_error;
		onrecved_handler_type		m_onrecved_handler;
	};

}
#endif // _DAEMON_CLIENT_HPP_