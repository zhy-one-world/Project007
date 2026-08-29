/********************************************************************
	created:	2014/07/26
	created:	26:7:2014   16:22
	file base:	guid_gen
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _GUID_GEN_H_
#define _GUID_GEN_H_

#include "logic/type_def.hpp"

namespace faith
{

	class guid_gen
	{
	public:
		static void				set_server_id(uint32 id);
		static guid_64			make_guid(guid_64 src_guid = 0);
		static guid_128			make_guid_128();
	private:
		static uint32		server_id;
		static uint32		head_second;
		static uint32		tail_second;
		static uint32		head_inc;
		static uint32		tail_inc;
		static bool			use_tail;
	};

}


#endif
