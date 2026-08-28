/********************************************************************
	created:	2017Äê11ÔÂ23ÈÕ19:36:16
	file base:	server_data
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _SERVER_DATA_H_
#define _SERVER_DATA_H_
#include "logic/type_def.hpp"
/*#include <winsock2.h>
#include <windows.h>*/    

namespace hld
{
	class server_data
	{
	public:
		server_data() { m_server_data_time = 0; };
		~server_data() {};
		static server_data& get_instance()
		{
			static server_data s_server_data;
			return s_server_data;
		}
	public:
		void heart_tick(int64& tick_time, xstring server_name, int64 other_data);
	public:
		int get_memory_usage(int64* mem, int64* vmem);
		int64 file_time_2_utc(const FILETIME* ftime);
		int get_processor_number();
		double get_cpu_usage();
		int get_io_bytes(int64* read_bytes, int64* write_bytes);
	private:
		int64 m_server_data_time;
	};
}
#endif
