/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   18:04
	file base:	proxy_service_cli
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "proxy_service_cli.hpp"
#include "fep_client.hpp"
#include <net/scheduler.hpp>
#include "game_cfg/servers_config.h"
#include <base.hpp>
#include "Logic/time_def.hpp"
#include "server_log.hpp"
#include <rlog.hpp>
#include "error.pb.h"
#include "net.pb.h"
#include "login_msg.hpp"

namespace faith
{
	using namespace net;
	bool check_packet_size(const void* data_ptr, size_t data_len)
	{
		packet_c2s_s2c* package_ptr = (packet_c2s_s2c*)data_ptr;
		if (nullptr == package_ptr)
		{
			return false;
		}
		if (package_ptr->get_packet_len() == data_len &&
			data_len <= sizeof(packet_c2s_s2c))
		{
			return true;
		}
		else
		{
			_RLOG_(MWARN, "check_packet_size get_packet_len="
				<< package_ptr->get_packet_len() << " data_len=" << data_len
				<< " sizeof(packet_c2s_s2c)=" << sizeof(packet_c2s_s2c));
			return false;
		}
	}

	proxy_service_cli::proxy_service_cli()
	{
		m_tcpserver=NULL;
		m_port=2200;
		m_enable_connect = false;
		m_session_array_num = 0;
		for (int32 i = 0; i < e_msg_base_max; ++i)
		{
			register_message(i, none_fuction);
		}
	}

	proxy_service_cli::~proxy_service_cli()
	{
	}
	void proxy_service_cli::register_message(ui16 msg_index, const msg_handler_type &handler)
	{
		if (msg_index >= e_msg_base_max)
		{
			return;
		}

		m_handler_map[msg_index] = handler;
	}

	bool proxy_service_cli::init()
	{
		m_port = FEPCONFIG->external_port;
		_RLOG_(MINFO, "proxy service initialization started, endpoint=127.0.0.1:"
			<< m_port << " acceptor thread=0");
		m_tcpserver=new net::tcp_server(
			boost::bind(&proxy_service_cli::on_serverstatus_changed,this,_1),
			boost::bind(&proxy_service_cli::on_conn_created,this,_1),
			boost::bind(&proxy_service_cli::on_conn_closed,this,_1),
			boost::bind(&proxy_service_cli::on_data_received,this,_1,_2,_3),
			"127.0.0.1",m_port,
			FEP_ACCEPTOR_SCHEDULER_THREAD_ID );

		if (!m_tcpserver)
		{
			_RLOG_(MERROR, "m_tcpserver is null");
			return false;
		}

		// set  max default packet size 
		if(!set_netpara_option(FEP_INTERNET_SERVER_SEND_BUFF_SIZE, FEP_INTERNET_SERVER_RECV_BUFF_SIZE, FEP_INTERNET_MAX_PACKAGE_SIZE))
		{
			_RLOG_(MERROR, "set_netpara_option");
			return false;
		}
		_RLOG_(MINFO, "proxy network parameters initialized, initial connections="
			<< FEPCONFIG->init_socket_num);

		set_allow_connection(true);

		//���ļ�װ��RDE���ݵ�������
		//

		m_scl_cli_sender = boost::bind(&tcp_server::send, m_tcpserver, _1,_2,_3);
		m_scl_cli_recver = boost::bind(&proxy_service_cli::on_data_received, &proxy_service_cli::getInstance(),_1,_2,_3);

		_RLOG_(MINFO, "proxy service initialization completed");
		return true;
	}

	bool proxy_service_cli::start()
	{
		if (m_tcpserver == nullptr)
		{
			_RLOG_(MERROR, "proxy service start failed: tcp server is null");
			return false;
		}
		_RLOG_(MINFO, "proxy service starting acceptor");
		if(!m_tcpserver->start())
		{
			_RLOG_(MERROR, "proxy service acceptor start failed, port=" << m_port);
			return false;
		}
		_RLOG_(MINFO, "proxy service acceptor started, port=" << m_port);
		return true;
	}

	void proxy_service_cli::stop()
	{
		if(m_tcpserver)
		{
			m_tcpserver->stop();
		}

	}

	void proxy_service_cli::on_serverstatus_changed(uint32 status)
	{
		if(status==tcp_server::e_ss_all_connection_closed)
		{
			delete m_tcpserver;
			m_tcpserver=NULL;
			return;
		}
	}		

	bool proxy_service_cli::alloc_session(uint32 connindex)
	{
		if (connindex >= init_socket_more)
		{
			return false;
		}
		if (m_session_array[connindex])
		{
			return false;
		}
		auto new_session_ptr =
			std::make_shared<client_session>();
		new_session_ptr->set_array_index(connindex + 1);
		new_session_ptr->set_scheduler_thread_id(
			m_tcpserver->get_session_thread_id(connindex));
		xstring ip_str = m_tcpserver->get_ip_addr(connindex);
		int32 ip_len = ip_str.size() > max_ip_address_length ? max_ip_address_length : ip_str.size();
		memcpy(new_session_ptr->m_ipaddr, ip_str.c_str(), ip_len);
		new_session_ptr->set_conn_index(connindex);
		new_session_ptr->set_client_uid();
		new_session_ptr->refresh_heart_beat();
		new_session_ptr->m_heart_login_time = utility::get_tick_count() + client_session_login_time;
		new_session_ptr->set_data_use(true);
		m_session_array[connindex] = new_session_ptr;
		++m_session_array_num;
		_RLOG_(MINFO, "client session allocated, connindex:" << connindex
			<< " arrayindex:" << new_session_ptr->get_array_index()
			<< " ip:" << ip_str << " scheduler thread:"
			<< new_session_ptr->get_scheduler_thread_id()
			<< " session count:" << m_session_array_num.load());

		if (get_session_num() > init_socket_link)
		{
			login_proto_login_end login_end;
			login_end.set_result(e_error_code_login_login_full);
			security_communication_layer::getInstance().send_to_session(connindex, &login_end, e_msgindex_s2c_client_login);
			new_session_ptr->set_is_logout(true);
		}
		return true;
	}
	void proxy_service_cli::logout(uint32 connindex, e_logout_result logout_result)
	{
		if (logout_result == e_logout_result_kick_by_rpc)
		{
			return;
		}
		auto client_session_ptr =
			get_session_by_connect(connindex);
		if (nullptr == client_session_ptr || client_session_ptr->get_is_logout())
		{
			return;
		}
		client_session_ptr->set_is_logout(true);
		client_session_ptr->set_cs_array_index(0);
		fep2ws_client_logout request;
		request.client_uid = client_session_ptr->get_client_uid();
		request.eReason = logout_result;

		if ((client_session_ptr->get_login_proto_login().login_type() == faith::e_login_type_new_zw || client_session_ptr->get_login_proto_login().login_type() == faith::e_login_type_new_account_and_password)
			&& client_session_ptr->get_age() < 18)
		{
			request.need_save_online_time = true;
			memcpy(request.account, client_session_ptr->get_account(), sizeof(request.account));
			request.online_time = client_session_ptr->get_online_time();
			request.online_duration = client_session_ptr->get_online_duration();
			request.month_recharge_num = client_session_ptr->get_month_recharge_num();
			request.month_recharge_time = client_session_ptr->get_month_recharge_time();
		}

		fep_client::getInstance().send_message_to_ws( &request, sizeof(request));
	}
	bool proxy_service_cli::free_session(uint32 connindex)
	{
		auto client_session_ptr =
			get_session_by_connect(connindex);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return false;
		}
		_RLOG_(MINFO, "client session releasing, connindex:" << connindex
			<< " arrayindex:" << client_session_ptr->get_array_index()
			<< " scheduler thread:" << client_session_ptr->get_scheduler_thread_id()
			<< " session count:" << m_session_array_num.load());
		client_session_ptr->clear_data();
		{
			std::lock_guard<std::mutex> lock(m_session_mutex);
			if (m_session_array[connindex] != client_session_ptr)
			{
				return false;
			}
			m_session_array[connindex].reset();
			--m_session_array_num;
		}
		return true;
	}

	client_session_ptr proxy_service_cli::get_session_by_id(int32 array_index)
	{
		if (array_index < 1 || array_index > init_socket_more)
		{
			return client_session_ptr();
		}
		std::lock_guard<std::mutex> lock(m_session_mutex);
		return m_session_array[array_index - 1];
	}
	client_session_ptr proxy_service_cli::get_empty_session()
	{
		for (int32 i = 0; i < init_socket_more; ++i)
		{
			std::lock_guard<std::mutex> lock(m_session_mutex);
			if (!m_session_array[i])
			{
				auto new_session_ptr =
					std::make_shared<client_session>();
				new_session_ptr->set_array_index(i + 1);
				new_session_ptr->set_scheduler_thread_id(
					net::scheduler::getInstance().get_current_thread_id());
				new_session_ptr->set_data_use(true);
				m_session_array[i] = new_session_ptr;
				m_session_array_num++;
				return new_session_ptr;
			}
		}
		return client_session_ptr();
	}
	client_session_ptr proxy_service_cli::get_session_by_connect(uint32 connindex)
	{
		if (connindex >= init_socket_more)
		{
			return client_session_ptr();
		}
		std::lock_guard<std::mutex> lock(m_session_mutex);
		return m_session_array[connindex];
	}

	client_session_ptr proxy_service_cli::get_session_by_account(int32 array_index, const xchar* account)
	{
		auto client_session_ptr =
			get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return nullptr;
		}
		if (strcmp(account, client_session_ptr->get_account()) == 0)
		{
			return client_session_ptr;
		}
		return nullptr;
	}
	void proxy_service_cli::on_conn_created( uint32 connindex )
	{
		xstring ip_str = m_tcpserver ? m_tcpserver->get_ip_addr(connindex) : "unknown";
		_RLOG_(MINFO, "client connection received, connindex:" << connindex
			<< " ip:" << ip_str << " connection callback thread:"
			<< net::scheduler::getInstance().get_current_thread_id());
		if(!m_enable_connect)
		{
			_RLOG_(MWARN, "client connection rejected, connindex:" << connindex
				<< " reason: connections disabled");
			m_tcpserver->close(connindex);
			return;
		}
		security_communication_layer::getInstance().add_scl_session(connindex, m_scl_cli_sender, m_scl_cli_recver);

		if(!alloc_session(connindex)) 
		{
			_RLOG_(MERROR, "client session allocation failed, connindex:" << connindex);
			m_tcpserver->close(connindex);
			return ;
		}
		_RLOG_(MINFO, "client connection accepted, connindex:" << connindex);
	}

	void proxy_service_cli::on_conn_closed( uint32 connindex )
	{
		_RLOG_(MINFO, "client connection closed, connindex:" << connindex);
		logout(connindex, e_logout_result_connect_dis);
		if (!free_session(connindex))
		{
			_RLOG_(MWARN, "client session release skipped, connindex:" << connindex);
		}
	}

	void proxy_service_cli::disconn_session(uint32 connindex, e_logout_result logout_result)
	{
		logout(connindex, logout_result);
		m_tcpserver->close(connindex);
	}

	void proxy_service_cli::on_data_received( uint32 connindex, const void *data_ptr, size_t data_len )
	{
		auto client_session_ptr =
			get_session_by_connect(connindex);
		if (nullptr == client_session_ptr || client_session_ptr->get_is_logout())
		{
			return;
		}
		packet_c2s_s2c* package_ptr = (packet_c2s_s2c*)data_ptr;
		if (NULL == package_ptr)
		{
			return;
		}

		if (!check_packet_size(data_ptr, data_len))
		{
			return;
		}

		int32 header = package_ptr->wheader;
		if (header < proto_header_MIN || header > proto_header_MAX)
		{
			return;
		}

		if (package_ptr->header_verstion != server_header_verstion)
		{
			client_session_ptr->set_is_logout(true);
			return;
		}

		client_session_ptr->m_client_send_msg_count++;
		client_session_ptr->refresh_heart_beat();

		package_ptr->client_uid = client_session_ptr->get_client_uid();
		m_handler_map[header](client_session_ptr->get_array_index(), package_ptr, package_ptr->get_packet_len());
		switch (header)
		{
		case e_msgindex_c2fep_ping:
		case e_msgindex_c2s_aoi_location:
		case e_msgindex_c2s_aoi_rotation:
		case e_msgindex_c2s_skill_spell:
		case e_msgindex_c2s_skill_cast:
		case e_msgindex_c2s_skill_hurt:
		case e_msgindex_c2s_skill_operation:
			break;
		default:
			_RLOG_(MINFO, "account num:" << client_session_ptr->m_account
				<< " header:" << header);
			break;
		}
	}

	void proxy_service_cli::broadcast(const void* data_ptr, size_t data_len, int32 header)
	{
		for (int32 i = 0; i < init_socket_more; ++i)
		{
			auto client_session_ptr =
				get_session_by_connect(i);
			if (client_session_ptr && client_session_ptr->is_vaild())
			{
				security_communication_layer::getInstance().send_to_session(
					client_session_ptr->get_conn_index(), header, data_ptr, data_len);
			}
		}
	}
	void proxy_service_cli::broadcast(const void* data_ptr, size_t data_len)
	{
		for (int32 i = 0; i < init_socket_more; ++i)
		{
			auto client_session_ptr =
				get_session_by_connect(i);
			if (client_session_ptr && client_session_ptr->is_vaild())
			{
				security_communication_layer::getInstance().send_to_session(
					client_session_ptr->get_conn_index(), data_ptr, data_len);
			}
		}
	}

	bool proxy_service_cli::set_netpara_option(uint32 send_buf_size, uint32 recv_buf_size, uint32 _max_packet_size)
	{
		const int32 FAITH_INNER_HEADER_SIZE = 8; //FAITH�ڲ����ݰ�ͷ��С

		const int32 client_send_buffer_coef = 32;
		const int32 client_recv_buffer_coef = 8;
		const int32 client_delaysending_size_coef = 64;
		const int32 client_delaysending_time_threshold = 60;

// 		m_tcpserver->set_option(tcp_server::options::max_packet_size(1));
// 		m_tcpserver->set_option(tcp_server::options::delaysending_size_threshold(0));

		if(!m_tcpserver->set_option(tcp_server::options::send_buffer_size(send_buf_size))) return false;
		if(!m_tcpserver->set_option(tcp_server::options::recv_buffer_size(recv_buf_size))) return false;
		if(!m_tcpserver->set_option(tcp_server::options::max_packet_size(_max_packet_size))) return false;
//		if(!m_tcpserver->set_option(tcp_server::options::tcp_nodelay(false))) return false;
		if(!m_tcpserver->set_option(tcp_server::options::delaysending_size_threshold(_max_packet_size))) return false;
//		if(!m_tcpserver->set_option(tcp_server::options::delaysending_time_threshold(0))) return false;
		m_tcpserver->init_client_server(init_socket_more, FEPCONFIG->init_socket_num);
		return true;
	}

	bool proxy_service_cli::is_valid_session(uint32 conn_index)
	{
		auto client_session_ptr =
			get_session_by_connect(conn_index);
		return client_session_ptr != nullptr && client_session_ptr->is_vaild();
	}

	client_session_ptr proxy_service_cli::get_session_by_fep_uid_64(ui64 uid)
	{
		auto client_session_ptr =
			get_session_by_id(s_client_uid(uid).fepsession_uid);
		if (client_session_ptr == nullptr || !client_session_ptr->is_vaild() || client_session_ptr->get_client_uid().fep_uid_64 != uid)
		{
			return nullptr;
		}
		else
		{
			return client_session_ptr;
		}
	}
}
