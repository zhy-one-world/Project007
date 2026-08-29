/********************************************************************
	created:	2014/07/25
	created:	25:7:2014   19:38
	file base:	logic_logger
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "logic_logger.h"
#include "log_db_proxy.hpp"
#include "game_log.hpp"
#include <time.hpp>
#include <strstream>
#include <Utility/guid_gen.h>

namespace faith
{

	log_field log_null;

	bool low_logger::log(log_content t, log_field f1, log_field f2, log_field f3, log_field f4, log_field f5, log_field f6, 
		log_field f7, log_field f8, log_field f9, log_field f10, log_field f11, log_field f12, 
		log_field f13, log_field f14, log_field f15, log_field f16, log_field f17, log_field f18, 
		log_field f19, log_field f20, log_field f21, log_field f22, log_field f23, log_field f24)
	{
		bool switch_on = cgame_log_enactment::getInstance().get_log_enactment( t );
		if (!switch_on)
			return false;

		xostringstream sql_builder;
		sql_builder << log_tables[t];
		sql_builder << f1.get_string();
		if (f2.is_null())	goto sql_end;	else	sql_builder << "," << f2.get_string();
		if (f3.is_null())	goto sql_end;	else	sql_builder << "," << f3.get_string();
		if (f4.is_null())	goto sql_end;	else	sql_builder << "," << f4.get_string();
		if (f5.is_null())	goto sql_end;	else	sql_builder << "," << f5.get_string();
		if (f6.is_null())	goto sql_end;	else	sql_builder << "," << f6.get_string();
		if (f7.is_null())	goto sql_end;	else	sql_builder << "," << f7.get_string();
		if (f8.is_null())	goto sql_end;	else	sql_builder << "," << f8.get_string();
		if (f9.is_null())	goto sql_end;	else	sql_builder << "," << f9.get_string();
		if (f10.is_null())	goto sql_end;	else	sql_builder << "," << f10.get_string();
		if (f11.is_null())	goto sql_end;	else	sql_builder << "," << f11.get_string();
		if (f12.is_null())	goto sql_end;	else	sql_builder << "," << f12.get_string();
		if (f13.is_null())	goto sql_end;	else	sql_builder << "," << f13.get_string();
		if (f14.is_null())	goto sql_end;	else	sql_builder << "," << f14.get_string();
		if (f15.is_null())	goto sql_end;	else	sql_builder << "," << f15.get_string();
		if (f16.is_null())	goto sql_end;	else	sql_builder << "," << f16.get_string();
		if (f17.is_null())	goto sql_end;	else	sql_builder << "," << f17.get_string();
		if (f18.is_null())	goto sql_end;	else	sql_builder << "," << f18.get_string();
		if (f19.is_null())	goto sql_end;	else	sql_builder << "," << f19.get_string();
		if (f20.is_null())	goto sql_end;	else	sql_builder << "," << f20.get_string();
		if (f21.is_null())	goto sql_end;	else	sql_builder << "," << f21.get_string();
		if (f22.is_null())	goto sql_end;	else	sql_builder << "," << f22.get_string();
		if (f23.is_null())	goto sql_end;	else	sql_builder << "," << f23.get_string();
		if (f24.is_null())	goto sql_end;	else	sql_builder << "," << f24.get_string();
sql_end:	sql_builder << ")";
		//clog_db_proxy::getInstance().execute_sql(sql_builder.str());

		return true;
	}


	log_field_time::log_field_time(unix_time t)
	{
		_null = false;
		std::strstream ss;
		ss << "FROM_UNIXTIME(" << t << ")";
		ss >> data_s;
	}

	log_field_time::log_field_time()
	{
		_null = false;
		std::strstream ss;
		ss << "FROM_UNIXTIME(" << utility::time() << ")";
		ss >> data_s;
	}

	log_field_seconds::log_field_seconds(uint32 seconds)
	{
		_null = false;
		std::strstream ss;
		ss << "SEC_TO_TIME(" << seconds << ")";
		ss >> data_s;
	}

	log_field::log_field(i16 _number):_null(false)
	{
		std::strstream ss;
		ss << _number;
		ss >> data_s;
	}

	log_field::log_field(ui16 _number):_null(false)
	{
		std::strstream ss;
		ss << _number;
		ss >> data_s;
	}

	log_field::log_field(int32 _number):_null(false)
	{
		std::strstream ss;
		ss << _number;
		ss >> data_s;
	}

	log_field::log_field(uint32 _number):_null(false)
	{
		std::strstream ss;
		ss << _number;
		ss >> data_s;
	}

	log_field::log_field(i64 _number):_null(false)
	{
		std::strstream ss;
		ss << _number;
		ss >> data_s;
	}

	log_field::log_field(ui64 _number):_null(false)
	{
		std::strstream ss;
		ss << _number;
		ss >> data_s;
	}

	log_field::log_field(float _number):_null(false)
	{
		std::strstream ss;
		ss << _number;
		ss >> data_s;
	}

	void xreplace(xstring& str, const xstring& sequence, const xstring& replace)
	{
		std::string::size_type offset = 0;
		while (offset < str.length())
		{
			std::string::size_type pos = str.find(sequence, offset);
			if (pos == std::string::npos)
				break;

			str = str.replace(pos, sequence.length(), replace);
			offset = (pos+replace.length());
		}
	}

	log_field::log_field(xstring _str):_null(false)
	{
		xstring escaped = _str;
		xreplace(escaped, "\\", "\\\\");
		xreplace(escaped, "'", "\\'");
		xreplace(escaped, "\"", "\\\"");

		data_s = "'";
		data_s += escaped;
		data_s += "'";
	}

	log_field::log_field(guid_64 _number):_null(false)
	{
		ui64 _value = *(ui64*)&_number;
		std::strstream ss;
		ss << _value;
		ss >> data_s;
	}

	low_logger::low_logger(void)
	{
		for (int32 i = 0; i < max_content_id; ++i)
			log_tables[i] = "insert ";
		init_table_name();
		for (int32 i = 0; i < max_content_id; ++i)
			log_tables[i] += " values(";
	}

	low_logger::~low_logger(void)
	{
	}

	ui64 low_logger::get_log_id()
	{
		return guid_gen::make_guid().server_64;
	}

	bool low_logger::get_log_switch(log_content t)
	{
		return cgame_log_enactment::getInstance().get_log_enactment( t );
	}

	void low_logger::send(const void* data, size_t length)
	{
		clog_db_proxy::getInstance().send(data, length);
	}
}
