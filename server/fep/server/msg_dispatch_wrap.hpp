/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   14:36
	file base:	msg_dispatch_wrap
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/

#ifndef _FEP_MSG_DISPATCH_WRAP_H_
#define _FEP_MSG_DISPATCH_WRAP_H_

#include <singleton.hpp>
#include "logic/type_def.hpp"
#include "base.hpp"

namespace faith
{
	

	class message_from_server_dispatcher : public singleton<message_from_server_dispatcher>
	{
	private:
		friend class singleton<message_from_server_dispatcher>;
	protected:
		message_from_server_dispatcher();
	public:
		~message_from_server_dispatcher(); 

	public:
		bool init();
	};

}

#endif
