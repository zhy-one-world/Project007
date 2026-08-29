/********************************************************************
	created:	2017年11月23日19:36:39
	file base:	server_data
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "server_data.hpp"
#include "log/server_log.hpp"
#include <psapi.h>    
#pragma comment(lib,"psapi.lib")

namespace faith 
{

	void server_data::heart_tick(int64& tick_time, xstring server_name, int64 other_data)
	{
		ZoneScoped;
		//if (tick_time > m_server_data_time)
		//{
		//	int64 mem, vmem;
		//	int64 read_bytes, write_bytes;
		//	get_memory_usage(&mem, &vmem);
		//	double cpu_use = get_cpu_usage();
		//	get_io_bytes(&read_bytes, &write_bytes);
		//	PROJECT_RECORD_LOG(server_log::get_game_log(),
		//		server_name
		//		<< _XTEXT(" memory =") << mem
		//		<< _XTEXT(" vmemory =") << vmem
		//		<< _XTEXT(" cpu_use =") << cpu_use
		//		<< _XTEXT(" read_bytes =") << read_bytes
		//		<< _XTEXT(" write_bytes =") << write_bytes
		//		<< _XTEXT(" other_data =") << other_data);
		//	m_server_data_time = tick_time + 1000;
		//}
	}
	int server_data::get_memory_usage(int64* mem, int64* vmem)
	{
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
		{
			if (mem) *mem = pmc.WorkingSetSize;
			if (vmem) *vmem = pmc.PagefileUsage;
			return 0;
		}
		return -1;
	}
	/// 时间转换
	int64 server_data::file_time_2_utc(const FILETIME* ftime)
	{
		LARGE_INTEGER li;

		li.LowPart = ftime->dwLowDateTime;
		li.HighPart = ftime->dwHighDateTime;
		return li.QuadPart;
	}
	/// 获得CPU的核数
	int server_data::get_processor_number()
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return (int)info.dwNumberOfProcessors;
	}
	double server_data::get_cpu_usage()
	{
		//cpu数量
		static int processor_count_ = -1;
		//上一次的时间
		static int64_t last_time_ = 0;
		static int64_t last_system_time_ = 0;

		FILETIME now;
		FILETIME creation_time;
		FILETIME exit_time;
		FILETIME kernel_time;
		FILETIME user_time;
		int64_t system_time;
		int64_t time;
		int64_t system_time_delta;
		int64_t time_delta;

		double cpu = -1;

		if (processor_count_ == -1)
		{
			processor_count_ = get_processor_number();
		}

		GetSystemTimeAsFileTime(&now);

		if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time, &kernel_time, &user_time))
		{
			// We don’t assert here because in some cases (such as in the TaskManager)
			// we may call this function on a process that has just exited butwe have
			// not yet received the notification.
			return -1;
		}
		
		if (processor_count_ == 0)
		{
			return -1;
		}
		system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;
		time = file_time_2_utc(&now);

		if ((last_system_time_ == 0) || (last_time_ == 0))
		{
			// First call, just set the last values.
			last_system_time_ = system_time;
			last_time_ = time;
			return -1;
		}

		system_time_delta = system_time - last_system_time_;
		time_delta = time - last_time_;

		//assert(time_delta != 0);

		if (time_delta == 0)
			return -1;

		// We add time_delta / 2 so the result is rounded.
		cpu = (system_time_delta * 100 + time_delta / 2) / time_delta;
		last_system_time_ = system_time;
		last_time_ = time;
		return cpu;
	}

	int server_data::get_io_bytes(int64* read_bytes, int64* write_bytes)
	{
		IO_COUNTERS io_counter;
		if (GetProcessIoCounters(GetCurrentProcess(), &io_counter))
		{
			if (read_bytes) *read_bytes = io_counter.ReadTransferCount;
			if (write_bytes) *write_bytes = io_counter.WriteTransferCount;
			return 0;
		}
		return -1;
	}
}
