/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved.

	Created_datetime : 	2019-07-20

	File Name :	game_mgr.cpp

	Author : zhy

	Description :

	Change List :


@@*/

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include "game_mgr.h"
#include "game_cfg/servers_config.h"
#include "server_log.hpp"
#include <net/scheduler.hpp>
#include <time.hpp>
#include "connection/daemon_client.hpp"
#include "Logic/time_def.hpp"
#include "net/message_manager.hpp"
#include "utility/parse_msg.h"
#include <gate_msg.hpp>
#include <internal.hpp>
#include "daemon_eye_msg.hpp"
#include "http/http_access_mgr.hpp"
#include "app/app_server.hpp"
#include "tracy/Tracy.hpp"
#include "serializer.hpp"
#include "net/net_client_mgr.hpp"
#include "net.pb.h"

//////////////////////////////////////////////////////////////////////////
//
//	Macro And Struct Define
//
//////////////////////////////////////////////////////////////////////////
namespace faith
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	Class Implement
	//
	//////////////////////////////////////////////////////////////////////////
	using namespace faith::utility;
	game_mgr::game_mgr()
	{
		m_timer_index_game_loop = scheduler::scheduler_invalid_timer_index;
		m_cross_mgr_map.clear();
		m_game_server_map.clear();
		m_all_server_flag_end_map.clear();
		m_all_server_id_arr.clear();
		m_all_server_msg_with_ip.clear();
		m_cur_server_msg_with_ip.clear();
		m_begin_reload_csv_time = -1;
		message_manager::getInstance().register_handler(e_msg_index_req_login, boost::bind(&game_mgr::on_req_login, this, _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_index_req_stop, boost::bind(&game_mgr::on_req_stop, this, _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_server2gate_register, boost::bind(&game_mgr::on_server_register, this, _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_server2gate_send_to_server, boost::bind(&game_mgr::on_data_received, this, _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_ws2gate_change_cross_id, boost::bind(&game_mgr::on_server_change_cross_id, this, _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_ws2gate_flag_load_end, boost::bind(&game_mgr::on_server_flag_load_end, this, _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_ws2gate_server_hide_act, boost::bind(&game_mgr::on_server_act_hide_end, this, _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_server2gate_reload_csv, boost::bind(&game_mgr::on_server_need_reload_csv, this, _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_server2gate_reload_csv_end, boost::bind(&game_mgr::on_server_reload_csv_one_server_end, this, _1, _2, _3));
	}

	game_mgr::~game_mgr()
	{

	}

	bool game_mgr::init()
	{
		xstring gate_ip = WSCONFIG->internal_host;
		int32 gate_port = GATECONFIG->internal_port;
		m_gate_server_id = GATECONFIG->gate_id;
		if (false == net_server_mgr::getInstance().init(e_server_type_gate, gate_ip, gate_port,
			GATE_SEVER_SEND_BUFFER_SIZE, GATE_SEVER_RECV_BUFFER_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, GATE_NEED_SERVER_COUNT, SERVER_GATE_COUNT,
			boost::bind(&game_mgr::on_conn_closed, this, _1)))
		{
			return false;
		}
		return true;
	}

	bool game_mgr::start()
	{
		net_server_mgr::getInstance().start();
		m_timer_index_game_loop = scheduler::getInstance().add_timer(50, boost::bind(&game_mgr::server_loop, this, _1));
		return true;
	}

	void game_mgr::stop()
	{
		net_server_mgr::getInstance().stop();
	}
	void game_mgr::server_loop(uint32 timer_index)
	{
		ZoneScoped;
		if (timer_index != m_timer_index_game_loop)
		{
			return;
		}
		static int32 loop_counter = 0;
		static int64 tick_time = get_tick_count();	// 每次场景tick时间(平均值)
		static int64 last_log_time = 0;
		++loop_counter;
		int64 time_now = get_tick_count();
		if (time_now > last_log_time)
		{
			faith::int32 ws_num = net_server_mgr::getInstance().get_server_count(e_server_type_ws);
			faith::int32 dp_num = net_server_mgr::getInstance().get_server_count(e_server_type_dp);
			faith::int32 cs_num = net_server_mgr::getInstance().get_server_count(e_server_type_cs);
			faith::int32 gate_num = net_server_mgr::getInstance().get_server_count(e_server_type_gate);
			faith::int32 gate_client_num = net_client_mgr::getInstance().get_server_count(e_server_type_gate);
			CONSOLE_INFO("ws:{} dp:{} cs:{} gate:{}", ws_num, dp_num, cs_num, gate_num + gate_client_num);
			CONSOLE_INFO("tick:{}", (time_now - tick_time) / loop_counter);
			last_log_time = time_now + server_console_time;
			tick_time = time_now;
			loop_counter = 0;
		}
		if (time_now > m_begin_reload_csv_time + reload_csv_constraint_time && m_begin_reload_csv_time > 0)
		{
			m_begin_reload_csv_time = -1;
			last_server_reload_csv_all();
			m_cur_server_msg_with_ip.clear();
		}
		daemon_client::getInstance().heart_tick(time_now);
		http_access_mgr::get_instance().tick(time_now);
		if (daemon_client::getInstance().get_server_close())
		{
			// 输出信息
			CONSOLE_INFO("daemon close, all player offline, please shutdown gate ! ! !");
			stop();
			app_server::getInstance().stop();
			return;
		}
	}
	void game_mgr::on_conn_closed(const net_server* faith_server_ptr)
	{
		CONSOLE_INFO(" session close, connindex: {}", faith_server_ptr->get_conn_index());
		del_game_server(faith_server_ptr->get_conn_index());
	}

	void game_mgr::on_req_login(unsigned int connindex, const void* data_ptr, size_t data_len)
	{
		const faith::req_login* msg = static_cast<const faith::req_login*>(data_ptr);
		if (NULL == msg)
		{
			return;
		}
		CONSOLE_INFO("connindex:{} ip_addr:{} port:{} server_type:{} server_index:{}", connindex,  msg->server_info.ip_addr, msg->server_info.port, (int32)msg->server_info.server_type, msg->server_info.server_index);
		if (msg->server_info.server_type >= e_server_type_max)
		{
			return;
		}
		if (msg->server_info.server_index < 0)
		{
			return;
		}
		net_server* faith_server_ptr = net_server_mgr::getInstance().get_peer_by_conn_index(connindex);
		if (nullptr == faith_server_ptr)
		{
			return;
		}
		faith_server_ptr->set_server_info(msg->server_info);
		if (msg->server_info.server_type == e_server_type_gate)
		{
			gate2gate_req_new new_gate_msg;
			new_gate_msg.server_info = msg->server_info;
			broad_cast(&new_gate_msg, sizeof(new_gate_msg), 0, e_server_type_invalid);
		}
		else
		{
			std::vector<uint32> gate_array = net_server_mgr::getInstance().get_server_conn_array(e_server_type_gate);
			for (int32 i = 0; i < gate_array.size(); ++i)
			{
				net_server* temp_server_ptr = net_server_mgr::getInstance().get_peer_by_conn_index(gate_array[i]);
				if (nullptr == temp_server_ptr)
				{
					continue;
				}
				gate2gate_req_new new_gate_msg;
				new_gate_msg.server_info = temp_server_ptr->get_server_info();
				faith_server_ptr->send_message(&new_gate_msg, sizeof(new_gate_msg));
			}
		}
	}
	void game_mgr::on_req_stop(unsigned int connindex, const void* data_ptr, size_t data_len)
	{
		const faith::req_stop* msg = static_cast<const faith::req_stop*>(data_ptr);
		if (NULL == msg)
		{
			return;
		}
		if (msg->server_type != e_server_type_gate)
		{
			return;
		}
		app_server::getInstance().stop();
		CONSOLE_INFO("FaithEye Stop Game!");
	}
	void game_mgr::last_server_reload_csv_all()
	{
		std::map<std::string, std::vector<int32>>::iterator server_ite;
		std::vector<int32>::iterator ite;
		int32 need_send_server_id = 0;
		for (server_ite = m_cur_server_msg_with_ip.begin(); server_ite != m_cur_server_msg_with_ip.end(); server_ite++)
		{
			for (ite = server_ite->second.begin(); ite != server_ite->second.end();)
			{
				if (*ite > 0)
				{
					need_send_server_id = *ite;
					gate2server_reload_csv msg;
					msg.need_reload = 1;
					send_to_server(&msg, sizeof(msg), need_send_server_id, e_server_type_ws);
					//查看剩哪些服务器没有reloadcsv
					CONSOLE_INFO("last_server_reload_csv_all! need_send_server_id:{}", need_send_server_id);
				}
				ite = server_ite->second.erase(ite);
			}
		}
	}
	void game_mgr::on_server_need_reload_csv(unsigned int connindex, const void* data_ptr, size_t data_len)
	{
		const server2gate_reload_csv* packet = static_cast<const server2gate_reload_csv*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		CONSOLE_INFO("on_server_need_reload_csv_begin!");
		m_cur_server_msg_with_ip.clear();
		m_begin_reload_csv_time = get_tick_count();
		m_cur_server_msg_with_ip = m_all_server_msg_with_ip;

		std::map<std::string, std::vector<int32>>::iterator server_ite;
		std::vector<int32>::iterator ite;
		int32 need_send_server_id = 0;
		for (server_ite = m_cur_server_msg_with_ip.begin(); server_ite != m_cur_server_msg_with_ip.end(); server_ite++)
		{
			ite = server_ite->second.begin();
			if (ite != server_ite->second.end())
			{
				need_send_server_id = *ite;
				gate2server_reload_csv msg;
				msg.need_reload = 1;
				send_to_server(&msg, sizeof(msg), need_send_server_id, e_server_type_ws);
				CONSOLE_INFO("send_to_server_reload_csv need_send_server_id:{}", need_send_server_id);
				server_ite->second.erase(ite);
			}
		}
	}
	void game_mgr::on_server_reload_csv_one_server_end(unsigned int connindex, const void* data_ptr, size_t data_len)
	{
		const server2gate_reload_csv_end* packet = static_cast<const server2gate_reload_csv_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		xstring ip_msg = packet->out_ip;
		int32 server_id = packet->server_id;
		CONSOLE_INFO("server_reload_csv_end server_id:{}", server_id);
		if (m_cur_server_msg_with_ip.find(ip_msg) == m_cur_server_msg_with_ip.end())
		{
			return;
		}
		int32 next_server_id = -1;
		std::vector<int32>::iterator ite;
		//for (ite = m_cur_server_msg_with_ip[ip_msg].begin(); ite != m_cur_server_msg_with_ip[ip_msg].end(); )
		//{
		//	if (*ite == server_id)
		//	{
		//		ite = m_cur_server_msg_with_ip[ip_msg].erase(ite);
		//	}
		//	else
		//	{
		//		ite++;
		//	}
		//}
		ite = m_cur_server_msg_with_ip[ip_msg].begin();
		if (ite != m_cur_server_msg_with_ip[ip_msg].end())
		{
			next_server_id = *ite;
			m_cur_server_msg_with_ip[ip_msg].erase(ite);
		}

		if (next_server_id > 0)
		{
			gate2server_reload_csv msg;
			msg.need_reload = 1;
			send_to_server(&msg, sizeof(msg), next_server_id, e_server_type_ws);
			CONSOLE_INFO("send_to_server_reload_csv next_server_id:{}", next_server_id);
		}

	}
	void game_mgr::on_server_register(unsigned int connindex, const void* data_ptr, size_t data_len)
	{
		const server2gate_req_register* packet = static_cast<const server2gate_req_register*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		game_server* game_server_ptr = add_game_server(packet->game_info, packet->server_type, packet->server_index, connindex);
		if (nullptr == game_server_ptr)
		{
			return;
		}
		CONSOLE_INFO("connindex:{} out_ip:{} out_port:{} server_id:{} cross_id:{} server_type:{} server_index:{}", connindex, packet->game_info.out_ip, packet->game_info.out_port, packet->game_info.server_id, packet->game_info.cross_id, (int32)packet->server_type, packet->server_index);
		if (packet->game_info.cross_id > 0)
		{
			cross_mgr* cross_mgr_ptr = get_cross_server(packet->game_info.cross_id);
			if (nullptr == cross_mgr_ptr)
			{
				cross_mgr_ptr = add_cross_server(packet->game_info.cross_id);
			}
			cross_mgr_ptr->add_game_server(game_server_ptr);
			if (packet->server_type == e_server_type_ws)
			{
				if (packet->recv_server_id > 0)
				{
					game_server* recv_game_server_ptr = game_mgr::get_game_server(packet->recv_server_id);
					if (recv_game_server_ptr)
					{
						recv_game_server_ptr->send_message(data_ptr, data_len, e_server_type_ws);
					}
				}
				else
				{
					broad_cast(data_ptr, data_len, packet->game_info.server_id);
				}
			}
		}
	}

	bool game_mgr::is_all_server_flag_load_end()
	{
		int32 max_num = m_all_server_id_arr.size();
		int32 cur_num = m_all_server_flag_end_map.size();
		if (cur_num >= max_num && max_num > 0)
		{
			return true;
		}
		return false;
	}

	void game_mgr::del_server_flag_with_server_close(int32 server_id)
	{
		std::map<int32, int32>::iterator ite = m_all_server_flag_end_map.find(server_id);
		if (ite == m_all_server_flag_end_map.end())
		{
			return;
		}
		m_all_server_flag_end_map.erase(ite);
	}

	void game_mgr::on_server_act_hide_end(unsigned int connindex, const void* data_ptr, size_t data_len)
	{
		const ws2gate_server_act_hide* packet = static_cast<const ws2gate_server_act_hide*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		bool is_need_hide = packet->is_need_hide;
		gate2ws_server_act_hide msg;
		msg.is_need_hide = is_need_hide;
		broad_cast(&msg, sizeof(msg), 0, e_server_type_ws);
	}

	void game_mgr::on_server_flag_load_end(unsigned int connindex, const void* data_ptr, size_t data_len)
	{
		const ws2gate_flag_load_end* packet = static_cast<const ws2gate_flag_load_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		int32 server_id = packet->cur_server_id;
		if (server_id <= 0)
		{
			return;
		}
		m_all_server_flag_end_map[server_id] = 0;
		if (is_all_server_flag_load_end())
		{
			send_all_server_end_refresh_list();
		}
	}

	void game_mgr::set_all_server_id(int32 server_id)
	{
		m_all_server_id_arr.insert(server_id);
	}

	void game_mgr::send_all_server_end_refresh_list()
	{
		gate2ws_end_refresh_list msg;
		broad_cast(&msg, sizeof(msg), 0, e_server_type_ws);
	}

	void game_mgr::on_server_change_cross_id(unsigned int connindex, const void* data_ptr, size_t data_len)
	{
		const ws2gate_change_cross_id* packet = static_cast<const ws2gate_change_cross_id*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		int32 server_id = packet->cur_server_id;
		int32 cross_id = packet->cross_id;
		int32 old_cross_id = packet->old_cross_id;
		game_server* game_server_ptr = get_game_server(server_id);
		if (nullptr == game_server_ptr)
		{
			return;
		}
		gate2ws_change_server_id msg;//回给该跨服组所有WS的数据
		cross_mgr* old_cross_mgr_ptr = get_cross_server(old_cross_id);
		if (old_cross_mgr_ptr != nullptr)
		{
			msg.is_add = false;
			msg.server_info = game_server_ptr->get_game_info();
			old_cross_mgr_ptr->broad_cast(&msg, sizeof(msg), server_id);

			old_cross_mgr_ptr->del_game_server(server_id);
			if (old_cross_id == server_id)
			{
				del_cross_server(server_id);
			}
		}
		cross_mgr* new_cross_mgr_ptr = get_cross_server(cross_id);
		if (new_cross_mgr_ptr != nullptr)
		{
			new_cross_mgr_ptr->add_game_server(game_server_ptr);

			msg.is_add = true;
			msg.server_info = game_server_ptr->get_game_info();
			new_cross_mgr_ptr->broad_cast(&msg, sizeof(msg), server_id);
		}
		else
		{
			if (cross_id > 0)
			{
				new_cross_mgr_ptr = add_cross_server(cross_id);
			}
			if (new_cross_mgr_ptr == nullptr)
			{
				return;
			}
			new_cross_mgr_ptr->add_game_server(game_server_ptr);

			msg.is_add = true;
			msg.server_info = game_server_ptr->get_game_info();
			new_cross_mgr_ptr->broad_cast(&msg, sizeof(msg));
			return;
		}

		gate2ws_all_server_arr all_msg;//发给本服该跨服组当前所有服务器信息
		new_cross_mgr_ptr->get_this_cross_all_server(all_msg.server_info, all_msg.server_num);
		game_server_ptr->send_message(&all_msg, sizeof(all_msg), e_server_type_ws);
	}
	void game_mgr::on_data_received(unsigned int connindex, const void* data_ptr, size_t data_len)
	{
		const server2gate_to_server* packet = static_cast<const server2gate_to_server*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		//std::cout << __FUNCTION__ << " wheader = " << packet->wheader << std::endl;
		//std::cout << __FUNCTION__ << " recv_server_id = " << packet->recv_server_id << std::endl;
		//std::cout << __FUNCTION__ << " recv_server_type = " << packet->recv_server_type << std::endl;
		//std::cout << __FUNCTION__ << " recv_server_index = " << packet->recv_server_index << std::endl;
		//std::cout << __FUNCTION__ << " header = " << packet->header << std::endl;
		if (packet->recv_server_type == e_server_type_gate && packet->recv_server_index == 0)
		{
			cross_mgr* cross_mgr_ptr = get_cross_server(packet->recv_server_id);
			if (cross_mgr_ptr)
			{
				cross_mgr_ptr->broad_cast(data_ptr, data_len, packet->send_server_id);
			}
		}
		else if (packet->recv_server_type == e_server_type_gate && packet->recv_server_index == -1)
		{
			broad_cast(data_ptr, data_len, packet->send_server_id);
		}
		else
		{
			game_server* game_server_ptr = get_game_server(packet->recv_server_id);
			if (game_server_ptr)
			{
				game_server_ptr->send_message(data_ptr, data_len, packet->recv_server_type, packet->recv_server_index);
			}
		}
	}

	cross_mgr* game_mgr::get_cross_server(int32 cross_id)
	{
		cross_mgr_map_it it = m_cross_mgr_map.find(cross_id);
		if (it == m_cross_mgr_map.end())
		{
			return nullptr;
		}
		return it->second;
	}
	cross_mgr* game_mgr::add_cross_server(int32 cross_id)
	{
		cross_mgr* cross_mgr_ptr = get_cross_server(cross_id);
		if (nullptr == cross_mgr_ptr)
		{
			cross_mgr_ptr = new cross_mgr();
			m_cross_mgr_map[cross_id] = cross_mgr_ptr;
		}
		cross_mgr_ptr->set_cross_id(cross_id);
		return cross_mgr_ptr;
	}
	void game_mgr::del_cross_server(int32 cross_id)
	{
		cross_mgr_map_it it = m_cross_mgr_map.find(cross_id);
		if (it != m_cross_mgr_map.end())
		{
			delete it->second;
			it->second = nullptr;
			m_cross_mgr_map.erase(it);
		}
	}
	game_server* game_mgr::get_game_server(int32 server_id)
	{
		game_server_map_it it = m_game_server_map.find(server_id);
		if (it == m_game_server_map.end())
		{
			return nullptr;
		}
		return it->second;
	}
	game_server* game_mgr::get_game_server_by_conn(uint32 conn_index)
	{
		game_server_map_it it = m_conn_server_map.find(conn_index);
		if (it == m_conn_server_map.end())
		{
			return nullptr;
		}
		return it->second;
	}
	game_server* game_mgr::add_game_server(const s_game_info& game_info, e_server_type server_type, int32 server_index, uint32 conn_index)
	{
		game_server* game_server_ptr = game_mgr::get_game_server(game_info.server_id);
		if (nullptr == game_server_ptr)
		{
			CONSOLE_INFO("add_game_server id:{} server_type:{}", game_info.server_id, (int32)server_type);
			game_server_ptr = new game_server();
			m_game_server_map[game_info.server_id] = game_server_ptr;
			m_conn_server_map[conn_index] = game_server_ptr;
		}
		if (server_type == e_server_type_ws)
		{
			game_server_ptr->set_game_info(game_info);
			if (game_info.server_id != m_gate_server_id)
			{
				bool is_have = false;
				for (int32 i = 0; i < m_all_server_msg_with_ip[game_info.out_ip].size(); ++i)
				{
					if (m_all_server_msg_with_ip[game_info.out_ip][i] == game_info.server_id)
					{
						is_have = true;
						CONSOLE_INFO("add_game_server fail server_id:{}", game_info.server_id);
						break;
					}
				}
				if (!is_have)
				{
					m_all_server_msg_with_ip[game_info.out_ip].push_back(game_info.server_id);
					CONSOLE_INFO("add_game_server succeed server_id:{}", game_info.server_id);
				}
			}
			/*if (game_info.server_id != m_gate_server_id)
			{
				m_all_server_msg_with_ip[game_info.out_ip].push_back(game_info.server_id);
			}*/
		}
		game_server_ptr->set_server_index(server_type, server_index, conn_index);
		return game_server_ptr;
	}
	void game_mgr::del_game_server(uint32 conn_index)
	{
		game_server* game_server_ptr = get_game_server_by_conn(conn_index);
		if (nullptr == game_server_ptr)
		{
			return;
		}
		int32 server_id = game_server_ptr->get_server_id();
		del_server_flag_with_server_close(server_id);
		game_server_ptr->set_server_close(conn_index);
		game_server_map_it conn_it = m_conn_server_map.find(conn_index);
		if (conn_it != m_conn_server_map.end())
		{
			m_conn_server_map.erase(conn_it);
		}
		if (game_server_ptr->is_game_run())
		{
			return;
		}
		xstring out_ip = game_server_ptr->get_game_info().out_ip;
		if (m_all_server_msg_with_ip.find(out_ip) != m_all_server_msg_with_ip.end())
		{
			std::vector<int32>::iterator ite;
			for (ite = m_all_server_msg_with_ip[out_ip].begin(); ite != m_all_server_msg_with_ip[out_ip].end();)
			{
				if (server_id == *ite)
				{
					ite = m_all_server_msg_with_ip[out_ip].erase(ite);
					CONSOLE_INFO("del_game_server id=:{}", server_id);
				}
				else
				{
					ite++;
				}
			}
		}

		gate2server_req_off_line msg;
		msg.server_id = game_server_ptr->get_server_id();
		cross_mgr* cross_mgr_ptr = get_cross_server(game_server_ptr->get_cross_id());
		if (nullptr != cross_mgr_ptr)
		{
			cross_mgr_ptr->del_game_server(game_server_ptr->get_server_id());
		}
		game_server_map_it server_it = m_game_server_map.find(game_server_ptr->get_server_id());
		if (server_it != m_game_server_map.end())
		{
			delete server_it->second;
			server_it->second = nullptr;
			m_game_server_map.erase(server_it);
		}
		broad_cast(&msg, sizeof(msg));
	}
	void game_mgr::send_to_server(const void* data_ptr, size_t data_len, int32 server_id, e_server_type server_type, int32 server_index)
	{
		game_server* game_server_ptr = game_mgr::get_game_server(server_id);
		if (nullptr == game_server_ptr)
		{
			return;
		}
		game_server_ptr->send_message(data_ptr, data_len, server_type, server_index);
	}
	void game_mgr::send_to_cross(const void* data_ptr, size_t data_len, int32 cross_id)
	{
		cross_mgr* cross_mgr_ptr = get_cross_server(cross_id);
		if (nullptr == cross_mgr_ptr)
		{
			return;
		}
		cross_mgr_ptr->broad_cast(data_ptr, data_len);
	}

	void game_mgr::broad_cast(const void* data_ptr, size_t data_len, int32 server_id, e_server_type server_type)
	{
		for (game_server_map_it it = m_game_server_map.begin(); it != m_game_server_map.end(); ++it)
		{
			if (it->first != server_id)
			{
				if (server_type == e_server_type_invalid)
				{
					it->second->send_message_all(data_ptr, data_len);
				}
				else
				{
					it->second->send_message(data_ptr, data_len, server_type);
				}
			}
		}
	}
	void game_mgr::HandlerDaemonOnrecv(const void* data_ptr, size_t data_len)
	{
		protocolUnpacker unpacker(data_ptr, data_len);
		int32 server_id = 0;
		unpacker.pop(server_id);

		switch (unpacker.getProtocol())
		{
		case e_msgindex_d2s_close_process:
		{
			close_server(data_ptr, data_len);
		}
		break;
		}
	}
	void game_mgr::close_server(const void* data_ptr, size_t data_len)
	{
		eye_proto_d2s_close_process request;
		if (!parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len))
		{
			return;
		}
		if (request.server_type() != e_server_type_gate)
		{
			return;
		}
		app_server::getInstance().stop();
	}
}