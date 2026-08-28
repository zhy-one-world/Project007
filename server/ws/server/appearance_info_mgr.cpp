#include "appearance_info_mgr.h"
#include "utility/serialize_msg.h"
#include "world_server.hpp"
#include "ws_client.hpp"
#include <appearance_info_msg.hpp> 
#include "game_cfg/servers_config.h"
#include "base.hpp"
#include "character.pb.h"
#include "net.pb.h"

namespace hld
{
	appearance_info_mgr::appearance_info_mgr()
	{
		m_appearance_info_by_id.clear();
		m_appearance_info_by_string.clear();
	}

	appearance_info_mgr::~appearance_info_mgr()
	{
		m_appearance_info_by_id.clear();
		m_appearance_info_by_string.clear();
	}

	void appearance_info_mgr::req_save_appearance_info_to_id(s_client_uid client_uid, const std::string& appearance_info)
	{
		auto iter = m_appearance_info_by_string.find(appearance_info);
		if (iter != m_appearance_info_by_string.end())
		{
			character_proto_save_appearance_info_to_id_end net_proto;
			net_proto.set_appearance_info_id(iter->second);
			net_proto.set_appearance_info_string(appearance_info);
			packet_c2s_s2c msg;
			serialize_msg::get_instance().set_serialize_msg_new(msg, &net_proto, client_uid, e_msgindex_s2c_save_appearance_info_to_id_end);
			world_server::getInstance().send_to_fep(client_uid.fepserver_uid, (void*)&msg, msg.get_packet_len());
		}
		else
		{
			ws2dp_save_appearance_info_to_id msg;
			msg.client_uid = client_uid;
			memcpy(msg.appearance_info, appearance_info.c_str(), sizeof(msg.appearance_info) > appearance_info.size() ? appearance_info.size() : sizeof(msg.appearance_info));
			ws_client::getInstance().send_to_dp(&msg, sizeof(msg), GATECONFIG->gate_id);
		}
	}

	void appearance_info_mgr::req_save_appearance_info_to_id_end(s_client_uid client_uid, const std::string& appearance_info, int32 id)
	{
		character_proto_save_appearance_info_to_id_end net_proto;
		net_proto.set_appearance_info_id(id);
		net_proto.set_appearance_info_string(appearance_info);
		packet_c2s_s2c msg;
		serialize_msg::get_instance().set_serialize_msg_new(msg, &net_proto, client_uid, e_msgindex_s2c_save_appearance_info_to_id_end);
		world_server::getInstance().send_to_fep(client_uid.fepserver_uid, (void*)&msg, msg.get_packet_len());

		m_appearance_info_by_string[appearance_info] = id;
		m_appearance_info_by_id[id] = appearance_info;
	}

	void appearance_info_mgr::req_load_appearance_info_by_id(s_client_uid client_uid, int32 id)
	{
		auto iter = m_appearance_info_by_id.find(id);
		if (iter != m_appearance_info_by_id.end())
		{
			character_proto_save_appearance_info_to_id_end net_proto;
			net_proto.set_appearance_info_id(id);
			net_proto.set_appearance_info_string(iter->second);
			packet_c2s_s2c msg;
			serialize_msg::get_instance().set_serialize_msg_new(msg, &net_proto, client_uid, e_msgindex_s2c_load_appearance_info_by_id_end);
			world_server::getInstance().send_to_fep(client_uid.fepserver_uid, (void*)&msg, msg.get_packet_len());
		}
		else
		{
			ws2dp_load_appearance_info_by_id msg;
			msg.client_uid = client_uid;
			msg.appearance_id = id;
			ws_client::getInstance().send_to_dp(&msg, sizeof(msg), GATECONFIG->gate_id);
		}
	}

	void appearance_info_mgr::req_load_appearance_info_by_id_end(s_client_uid client_uid, int32 id, const std::string& appearance_info)
	{
		character_proto_load_appearance_info_by_id_end net_proto;
		net_proto.set_appearance_info_id(id);
		net_proto.set_appearance_info_string(appearance_info);
		packet_c2s_s2c msg;
		serialize_msg::get_instance().set_serialize_msg_new(msg, &net_proto, client_uid, e_msgindex_s2c_load_appearance_info_by_id_end);
		world_server::getInstance().send_to_fep(client_uid.fepserver_uid, (void*)&msg, msg.get_packet_len());

		if (appearance_info != "")
		{
			m_appearance_info_by_string[appearance_info] = id;
			m_appearance_info_by_id[id] = appearance_info;
		}
	}
}