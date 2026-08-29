/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   22:03
	file base:	sql_builder
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "db_config.hpp"
#include "sql_builder.hpp"

namespace faith
{
	xchar sql_builder::m_buffer[SQL_BUILDER_BUFF_SIZE];
	i64 sql_builder::m_wpos = 0;
}
