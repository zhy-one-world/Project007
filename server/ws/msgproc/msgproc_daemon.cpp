#include "msgproc_daemon.hpp"
#include "connection/daemon_client.hpp"
#include "daemon_eye_msg.hpp"
#include "utility/init_unit.h"
#include "utility/guid_gen.h"
#include "server_log.hpp"
#include "mail_msg.hpp"
#include "def/app/app_server.hpp"
#include "net/tcp_client.hpp"
#include "../server/mail/event_ws_mgr.h"
#include "../server/web_client.hpp"
#include <rlog.hpp>

namespace faith
{
	void daemon2ws_exit_func(protocolUnpacker& unpacker)
	{
		app_server::getInstance().stop();
		_RLOG_(MINFO, "FaithEye Stop Game!");
	}

	void daemon2ws_gmcmd_func(protocolUnpacker& unpacker)
	{
		E2S_GM_CMD e2s_proto;
		unpacker.popProtoBuf(e2s_proto);
		std::string data_str = e2s_proto.data();
		Json::Reader reader;
		Json::Value  json_value;
		web_client::get_instance().hanlde_json(data_str.c_str(), data_str.size());
	}
}
