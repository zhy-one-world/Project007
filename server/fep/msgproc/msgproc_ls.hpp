/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   11:45
	file base:	msgproc_ls
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _FEP_MSGPROC_LS_H_
#define _FEP_MSGPROC_LS_H_

#include "logic/type_def.hpp"
namespace hld
{
	void ls2fep_rep_client_login(uint32 connindex, const void* data_ptr, size_t data_len);

	void ls2fep_rep_client_enum_char(uint32 connindex, const void* data_ptr, size_t data_len);
	void ls2fep_rep_client_create_char(uint32 connindex, const void* data_ptr, size_t data_len);
	void ls2fep_rep_client_del_char(uint32 connindex, const void* data_ptr, size_t data_len);
}

#endif
