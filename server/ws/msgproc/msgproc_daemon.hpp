#ifndef _WS_MSGPROC_DAEMON_H_
#define _WS_MSGPROC_DAEMON_H_

#include "logic/type_def.hpp"
#include "serializer.hpp"
#include <json/json.h>
#include "Logic/mail_def.hpp"
#include "Logic/item_def.hpp"
#include "../server/client_session.hpp"

namespace hld
{
	void	daemon2ws_exit_func(protocolUnpacker& unpacker);
	void	daemon2ws_gmcmd_func(protocolUnpacker& unpacker);
}


#endif
