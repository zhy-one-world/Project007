#include "msgproc/msgproc_daemon.hpp"
#include "business_stubs.hpp"

namespace faith
{
	void daemon2ws_exit_func(protocolUnpacker& unpacker)
	{
		(void)unpacker;
	}

	void daemon2ws_gmcmd_func(protocolUnpacker& unpacker)
	{
		(void)unpacker;
	}
}
