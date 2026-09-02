/********************************************************************
	created:	2014/07/25
	created:	25:7:2014   19:23
	file base:	game_log
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _CGAME_LOG_ENACTMENT_H_
#define _CGAME_LOG_ENACTMENT_H_

#include "logic/type_def.hpp"
#include "common/singleton.hpp"
#include <rlog.hpp>

/**
* Code : ZeGuo.G
* 游戏Log模块: Log设定XML读取
*/
namespace faith
{

	//开启后记录各个模块UPDATE的调用时间
	#define FAITH_LOG_UPDATE

	#define LOG_TIME_LEN	20000

	#ifdef FAITH_LOG_UPDATE
	#define LOG_UPDATE_BEGIN\
		static uint32 log_time = 0; \
		uint32 time_before = utility::get_tick_count(); \
		bool do_log = false; \
		if (time_before - log_time > LOG_TIME_LEN)\
		{\
		do_log = true; \
		log_time = time_before; \
	}\
		uint32 time_after = 0; \
		time_before = utility::get_tick_count(); \
		uint32 time_first = time_before;
	#define LOG_UPDATETIME(s)\
		if (do_log)\
		{\
		time_after = utility::get_tick_count(); \
		uint32 elapse = time_after - time_before; \
		if (elapse != 0)\
		_RLOG_(MINFO, "\t" << #s << ":" << elapse); \
		time_before = utility::get_tick_count(); \
	}
	#else
#define LOG_UPDATETIME(s)
#define LOG_UPDATE_BEGIN
	#endif

	class cgame_log_enactment : public singleton< cgame_log_enactment >
	{
		friend class singleton< cgame_log_enactment >;
		// Log各类内容设定表( key: 日志类型的枚举值 value: 开关 )
		typedef std::map< uint32, bool >	cgame_log_info;
		typedef cgame_log_info::iterator							cgame_log_info_iter;

	public:
		bool load(xstring strResPath);
		void unload();
		bool get_log_enactment( uint32 eType );
	private:
		bool load_resource	   ( xstring strFilename );
		void read_log_enactment ( /*att_xml_doc& Doc */);

	private:
		cgame_log_info	m_log_enactment;
	};
}


#endif
