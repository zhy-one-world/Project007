/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   22:00
	file base:	sql_builder
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _SQL_BUILDER_
#define _SQL_BUILDER_

#include "db_config.hpp"
#include <mem_pool.hpp>
#include "log/server_log.hpp"
#include <rlog.hpp>

const int32_t SQL_BUILDER_BUFF_SIZE = MAX_ROW_SIZE_LIMIT * 1024 * 4;	//dp允许最大行限制为MAX_ROW_SIZE_LIMIT， 那么一条sql文一般不可能超过BUFF_SIZE。

namespace faith
{
	
	/*	通过重载<<，简化sql文的构造
			1)对于数值类型（包括xchar）,使用c转化函数将其转化为字符串
			2)对于xchar[],xchar*,xstring类型，使用memcpy进行值拷贝
			3)对于其他类型（针对的是简单结构体，类。能使用memcpy给另一个类对象赋值的，并是按1个字节对齐的类），
			  将其视为2进制数据，调用escape_string() 函数进行转义构造。
	*/
	class sql_builder
	{
	public:
		sql_builder(db_accessor& db)/* :m_db(db)*/ { reset(); };
		~sql_builder(	)	{ reset(); };
	public:	
		void	reset() { m_wpos = 0; /*memset(m_buffer, 0, sizeof(m_buffer));*/ };
		xchar*	c_str(	)	{ return m_buffer; };
		i64		size(	)	{ return m_wpos; };

	//#if defined(_MSC_VER)
	//	template<typename t>
	//	inline typename boost::enable_if
	//	<
	//		boost::type_traits::ice_and<boost::is_class<t>::value,boost::has_trivial_assign<t>::value,boost::has_trivial_copy<t>::value>,
	//		sql_builder
	//	>::type& operator << (const t& value); 	
	//#else
	//	template<typename t>
	//	inline typename boost::enable_if<boost::is_class<t>,sql_builder>::type& operator << (const t& value);
	//#endif

		template<typename t>
		inline typename boost::enable_if<boost::is_signed<t>,sql_builder>::type& operator << (const t value); 

		template<typename t>
		inline typename boost::enable_if<boost::is_unsigned<t>,sql_builder>::type& operator << (const t value); 

		template<typename t>
		inline typename boost::enable_if<boost::is_float<t>,sql_builder>::type& operator << (const t value); 

		inline sql_builder& operator << (const xchar * value);
		inline sql_builder& operator << (const xstring& value);

	private:
		static xchar		m_buffer[SQL_BUILDER_BUFF_SIZE];
		static i64		m_wpos;
		//db_accessor&	m_db;
	};

//#if defined(_MSC_VER)
//	template<typename t>
//	inline typename boost::enable_if
//		<
//		boost::type_traits::ice_and<boost::is_class<t>::value, boost::has_trivial_assign<t>::value, boost::has_trivial_copy<t>::value>,
//		sql_builder
//		>::type& sql_builder::operator << (const t& value)
//#else
//	template<typename t>
//	inline typename boost::enable_if<boost::is_class<t>, sql_builder>::type& sql_builder::operator << (const t& value)
//#endif
//	{
//		BOOST_STATIC_ASSERT(boost::alignment_of<t>::value == 1);	//要求按一个字节对齐
//		assert(sizeof(t)*2 + 1 + m_wpos < (uint32)SQL_BUILDER_BUFF_SIZE);
//		int32 len = (int32)m_db.escape_string(m_buffer + m_wpos, (xchar *)&value, sizeof(t));
//		m_wpos += len;
//		return *this;
//	}

	template<typename t>
	inline typename boost::enable_if<boost::is_signed<t>,sql_builder>::type& sql_builder::operator << (const t value)
	{
		assert(22 + m_wpos < SQL_BUILDER_BUFF_SIZE);//64位十进制数最多20个字符，加上减号和\0，共需要22位
	#if defined(_MSC_VER) && _MSC_VER >= 1400
		_i64toa_s(value,m_buffer + m_wpos, 22, 10);
		int64 len = strlen(m_buffer + m_wpos);
	#elif defined(_MSC_VER)
		_i64toa(value,m_buffer + m_wpos, 10);
		int64 len = strlen(m_buffer + m_wpos);
	#else
		int64 len;
		if(sizeof(t)<=4)
		{
			len = sprintf(m_buffer + m_wpos, "%d", value);
		}
		else
		{
			len = sprintf(m_buffer + m_wpos, "%Ld", value);
		}
	#endif
		m_wpos += len;
		return *this;
	}

	template<typename t>
	inline typename boost::enable_if<boost::is_unsigned<t>,sql_builder>::type& sql_builder::operator << (const t value)
	{
		assert(22 + m_wpos < SQL_BUILDER_BUFF_SIZE);
	#if defined(_MSC_VER) && _MSC_VER >= 1400
		_ui64toa_s(value,m_buffer + m_wpos, 22, 10);
		i64 len = strlen(m_buffer + m_wpos);
	#elif defined(_MSC_VER)
		_ui64toa(value,m_buffer + m_wpos, 10);
		i64 len = strlen(m_buffer + m_wpos);
	#else
		i64 len;
		if(sizeof(t)<=4)
		{
			len = sprintf(m_buffer + m_wpos, "%u", value);
		}
		else
		{
			len = sprintf(m_buffer + m_wpos, "%Lu", value);
		}
	#endif
		m_wpos += len;
		return *this;
	}

	template<typename t>
	inline typename boost::enable_if<boost::is_float<t>,sql_builder>::type& sql_builder::operator << (const t value)
	{
		xstring f = boost::lexical_cast<xstring>(value);
		int32 len = f.size();
		if (len + 1 + m_wpos >= SQL_BUILDER_BUFF_SIZE)
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message(" len = {} m_wpos = {} SQL_BUILDER_BUFF_SIZE = {}",  len,  m_wpos,  SQL_BUILDER_BUFF_SIZE));
		}
		memcpy(m_buffer + m_wpos, f.c_str(), len);
		m_wpos += len;
		return *this;
	}

	inline sql_builder& sql_builder::operator << (const xchar * value)
	{
	#if defined(FAITH_UNICODE)
		ui64 in_len = wcslen(value);
		assert(in_len*3 + 1 + m_wpos < SQL_BUILDER_BUFF_SIZE);
		int32 real_len = utility::_iconv_one("UCS-2LE","UTF-8",(void*)value,in_len*sizeof(wchar_t),m_buffer+m_wpos,in_len*3);
		assert(real_len > 0);
		m_wpos += real_len;
		return *this;
	#else
		ui64 len = strlen(value);
		if (len + 1 + m_wpos >= SQL_BUILDER_BUFF_SIZE)
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message(" len = {} m_wpos = {} SQL_BUILDER_BUFF_SIZE = {}",  len,  m_wpos,  SQL_BUILDER_BUFF_SIZE));
		}
		memcpy(m_buffer + m_wpos, value, len);
		m_wpos += len;
		return *this;
	#endif
	}

	inline sql_builder& sql_builder::operator << (const xstring& value)
	{
	#if defined(FAITH_UNICODE)
		ui64 in_len = value.size();
		assert(in_len*3 + 1 + m_wpos < SQL_BUILDER_BUFF_SIZE);
		int32 real_len = utility::_iconv_one("UCS-2LE","UTF-8",(void*)value.c_str(),in_len*sizeof(wchar_t),m_buffer+m_wpos,in_len*3);
		assert(real_len > 0);
		m_wpos += real_len;
		return *this;
	#else
		ui64 len = value.size();
		if (len + 1 + m_wpos >= SQL_BUILDER_BUFF_SIZE)
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message(" len = {} m_wpos = {} SQL_BUILDER_BUFF_SIZE = {}",  len,  m_wpos,  SQL_BUILDER_BUFF_SIZE));
		}
		memcpy(m_buffer + m_wpos, value.c_str(), len);
		m_wpos += len;
		return *this;
	#endif
	}

}

#endif
