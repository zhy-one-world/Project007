/* slim gate2ws handlers (msgproc_cross replacement) */
#include "cross_group/msgproc_cross.hpp"
#include <gate_msg.hpp>
#include <core.hpp>
#include "world_server.hpp"
#include "ws_client.hpp"
#include "connection/daemon_client.hpp"
#include "business_stubs.hpp"
#include <rlog.hpp>

namespace faith
{
	void gate2ws_rep_register(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		_RLOG_(MINFO, ::faith::log_detail::format_message("data_len : {}",  data_len));
		if (data_len != sizeof(server2gate_req_register))
		{
			return;
		}
		const server2gate_req_register* packet = (const server2gate_req_register*)data_ptr;
		if (nullptr == packet)
		{
			return;
		}

		world_server::getInstance().set_gate_server_config(packet->game_info);
		if (packet->recv_server_id <= 0)
		{
			ws_client::getInstance().send_cross_msg(packet->game_info.server_id);
		}

		if (packet->game_info.cross_id <= 0 || world_server::getInstance().get_cross_id() <= 0)
		{
			world_server::getInstance().remove_server_config(packet->game_info.server_id);
			world_server::getInstance().send_server_config_to_all_cs(packet->game_info, false);
			return;
		}
		if (packet->game_info.cross_id != world_server::getInstance().get_cross_id())
		{
			world_server::getInstance().remove_server_config(packet->game_info.server_id);
			world_server::getInstance().send_server_config_to_all_cs(packet->game_info, false);
			return;
		}
		world_server::getInstance().set_server_config(packet->game_info);
		world_server::getInstance().send_server_config_to_all_cs(packet->game_info, true);
		world_server::getInstance().func_when_add_other_server(packet->game_info.server_id);
		world_server::getInstance().send_server_config_all_to_client(nullptr);

		if (packet->game_info.server_id == world_server::getInstance().get_cross_id())
		{
			world_server::getInstance().refresh_server_cross_time();
			world_server::getInstance().send_msg_when_gate_register(e_ws_flag_server_cross_time);
		}
	}

	void gate2ws_rep_off_line(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		_RLOG_(MINFO, ::faith::log_detail::format_message("data_len : {}",  data_len));
		if (data_len != sizeof(gate2server_req_off_line))
		{
			return;
		}
		const gate2server_req_off_line* packet = (const gate2server_req_off_line*)data_ptr;
		if (nullptr == packet)
		{
			return;
		}
		if (daemon_client::getInstance().get_server_close())
		{
			return;
		}
		s_game_info*  game_info_ptr = world_server::getInstance().get_server_config_by_server_id(packet->server_id);
		if (game_info_ptr)
		{
			world_server::getInstance().send_server_config_to_all_cs(*game_info_ptr, false);
			world_server::getInstance().func_when_remove_other_server(packet->server_id);
			world_server::getInstance().remove_server_config(packet->server_id);
			world_server::getInstance().send_server_config_all_to_client(nullptr);
		}
	}

	void gate2ws_rep_gate_new(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		_RLOG_(MINFO, ::faith::log_detail::format_message("data_len : {}",  data_len));
		if (data_len != sizeof(gate2gate_req_new))
		{
			return;
		}
		const gate2gate_req_new* packet = (const gate2gate_req_new*)data_ptr;
		if (nullptr == packet)
		{
			return;
		}
		ws_client::getInstance().start_gate_new(packet->server_info);
	}

	void gate2ws_reload_csv(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2server_reload_csv* packet = static_cast<const gate2server_reload_csv*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		world_server::getInstance().reload_csv_func();
	}

	void gate2ws_set_server_act_hide_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2ws_server_act_hide* packet = static_cast<const gate2ws_server_act_hide*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		world_server::getInstance().set_server_act_hidden(packet->is_need_hide, true);
	}

	void gate2ws_end_refresh_server_list_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2ws_end_refresh_list* packet = static_cast<const gate2ws_end_refresh_list*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		world_server::getInstance().end_refresh_server_list();
	}

	void gate2ws_change_server_id_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2ws_change_server_id* packet = static_cast<const gate2ws_change_server_id*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		bool is_add = packet->is_add;
		if (is_add)
		{
			world_server::getInstance().set_server_config(packet->server_info);
			world_server::getInstance().send_server_config_to_all_cs(packet->server_info, true);
			world_server::getInstance().func_when_add_other_server(packet->server_info.server_id);
			world_server::getInstance().send_server_config_all_to_client(nullptr);
		}
		else
		{
			world_server::getInstance().remove_server_config(packet->server_info.server_id);
			world_server::getInstance().send_server_config_to_all_cs(packet->server_info, false);
			world_server::getInstance().func_when_remove_other_server(packet->server_info.server_id);
			world_server::getInstance().send_server_config_all_to_client(nullptr);
		}
	}

	void gate2ws_this_cross_all_server_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2ws_all_server_arr* packet = static_cast<const gate2ws_all_server_arr*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		int32 server_num = packet->server_num;
		for (int32 i = 0; i < server_num; i++)
		{
			world_server::getInstance().set_server_config(packet->server_info[i]);
			world_server::getInstance().send_server_config_to_all_cs(packet->server_info[i], true);
			world_server::getInstance().func_when_add_other_server(packet->server_info[i].server_id);
		}
		world_server::getInstance().send_msg_when_gate_register(e_ws_flag_server_cross_time);
		world_server::getInstance().send_to_gate_get_all_msg_with_change_gate_server();
		world_server::getInstance().send_server_config_all_to_client(nullptr);
	}

	void gate2ws_rep_from_ws(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		(void)conn_index;
		(void)data_ptr;
		(void)data_len;
		// slim: cross business relays excluded
	}

	void gate2ws_rep_sign_up(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		(void)conn_index; (void)data_ptr; (void)data_len;
	}

	void gate2ws_rep_join_war(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		(void)conn_index; (void)data_ptr; (void)data_len;
	}
}
