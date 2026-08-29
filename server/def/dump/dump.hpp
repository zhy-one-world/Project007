/********************************************************************
created:	2019/01/14
created:	11:45:00
file base:	dump
file ext:	hpp
author:		zhy

purpose:
*********************************************************************/
#include "logic/type_def.hpp"
#include <postmortem.hpp>
namespace faith
{
	class dump
	{
	public:
		dump(xstring dump_name, xstring dump_file_name)
		{
			postmortem::getInstance().init(dump_file_name, dump_name);
		}
		~dump()
		{
			postmortem::getInstance().release();
		}
	};
}