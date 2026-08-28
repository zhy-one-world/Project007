/********************************************************************
	created:	2014/07/26
	created:	26:7:2014   16:18
	file base:	cs_date
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _CS_DATE_HPP_
#define _CS_DATE_HPP_

#include "logic/type_def.hpp"

namespace hld
{
	//const int32 update_data_time = 5;			//代表24h中的5点

	struct time_info
	{
		int64 second;
		int64 millisecond;

		int32 second_in_minute;	//从0开始	0-59
		int32 second_in_hour;	//从0开始	0-3599
		int32 second_in_day;	//从0开始	0-86399

		int32 minute_in_hour;	//从0开始	0-59
		int32 minute_in_day;	//从0开始	0-1439

		int32 hour_in_day;		//从0开始	0-23

		int32 day_in_week;		//从0开始	0-6,0是周一，有对应枚举
		int32 day_in_month;		//从0开始	0-30
		int32 day_in_year;		//从0开始	0-365

		int32 week_in_year;		//从0开始	0-51

		int32 month_in_year;	//从0开始	0-11,0是1月，有对应枚举

		int32 year;				//正常记录

		time_info()
		{
			memset(this, 0, sizeof(time_info));
		}

		~time_info()
		{

		}
	};

	class cs_date : public boost::noncopyable
	{
		// |year+2008 6 | month 4|   day 5  |			time 17			     |
		// |□□□□□□|□□□□|□□□□□|□□□□□□□□□□□□□□□□|
	public:
		cs_date(int32 date) { date_ = date; }
		cs_date() { date_ = 0; }
		~cs_date() { }

	public:
		void set_date(int32 year, int32 month, int32 day, int32 hours, int32 minutes, int32 seconds)
		{
			set_year(year);
			set_month(month);
			set_day(day);
			set_time(hours, minutes, seconds);
		}

		void get_date(int32& year, int32& month, int32& day, int32& hours, int32& minutes, int32& seconds)
		{
			year	= get_year();
			month	= get_month();
			day		= get_day();
			get_time(hours, minutes, seconds);
		}

		int32 get_date_int32()
		{
			return date_;
		}

		// 得到当前时间
		static int32 get_local_time();

	private:
		void set_year(int32 year)
		{
			int32 year_ = year - 2008;
			year_ <<= 26;
			date_ &= 0X03FFFFFF;
			date_ |= year_;
		}

		void set_month(int32 month)
		{
			month <<= 22;
			date_ &= 0XFC3FFFFF;
			date_ |= month;
		}

		void set_day(int32 day)
		{
			day <<= 17;
			date_ &= 0XFFC1FFFF;
			date_ |= day;
		}

		void set_time(int32 hours, int32 minutes, int32 seconds)
		{
			int32 time = hours * 3600 + minutes * 60 + seconds;
			date_ &= 0XFFFE0000;
			date_ |= time;
		}


		int32 get_year()
		{
			int32 date = date_;
			return (date >> 26) + 2008;
		}

		int32 get_month()
		{
			int32 date = date_;
			date >>= 22;
			return date &= 0X0000000F;
		}

		int32 get_day()
		{
			int32 date = date_;
			date >>= 17;
			return date &= 0X0000001F;
		}

		void get_time(int32& hours, int32& minutes, int32& seconds)
		{
			int32 date = date_;
			date &= 0X0001FFFF;
			hours = date / 3600;
			date %= 3600;
			minutes = date / 60;
			seconds = date % 60;
		}

	private:
		int32 date_;
	};

	//cc below
	class time_helper
	{
	public:
		static uint32		get_time();//unix time scene 1970		
		static uint32		get_second();
		static int32		get_days_by_month(int y,int m);	
		static int32		get_day_in_week(int y, int m, int d);
		static int32		get_days_in_year(int y, int m, int d);//获取当前日期为当年的第几天
		static uint32		get_part_in_year(uint32 part_duration);//一年中的第几个周期；part_duration == 周期		
		static int32		get_days_of_year(int y);//获取一年有几天
		static xstring		get_current_time();// 返回当前时间，格式为yyyy-mm-dd hh:mm:ss
		static xstring		get_cur_time();// 返回当前时间，格式为yyyy/mm/dd hh:mm:ss
		static xstring		get_cur_day_hour();// 返回当前时间，格式为yyyymmddhh
		static xstring		get_str_time_by_stamp(uint32 time_stamp);// 返回时间格式为yyyy-mm-dd hh:mm:ss
		static xstring		get_str_time_by_tm(tm* tm_);// 返回时间格式为yyyy-mm-dd hh:mm:ss

		static tm*			get_tm_by_time(uint32 time);
		static tm*			get_cur_tm_by_time(uint32 time);
		static void			get_time_by_string(const char* str, tm* tm_);
		static tm*			get_localtime();
		static int32		compare_tm(tm* tm_1, tm* tm_2);//<0代表小于,0代表相等,>0代表大于
		static int32		get_int_day();//获得一个年月日组合的数字,年*10000 + 月*100+日
		static int32		get_today_time_in_sec(); // 获取当天的以秒为单位的时间(时*3600 + 分*60 + 秒)
		static bool			is_diff_day(uint32 time_a, uint32 time_b = 0);//通过unix时间戳判断是否同一天
		static bool			is_diff_month(uint32 time_a, uint32 time_b = 0);//通过unix时间戳判断是否同一月
		static int64		get_stamp_by_hour_min(int32 time_hour, int32 time_min, int32 cur_time = 0);//通过固定的 小时 和 分钟 获得当天unix时间戳
		static int64		get_stamp_by_min(int32 time_min);//通过固定的分钟 获得当前小时unix时间戳
		static int64		get_next_refresh_time_stamp(int64 cur_time = -1, int32 target_hour = 0);
		static bool			is_over_refresh_time(int64 save_stamp_time);

		static time_info	get_cur_time_new();
		static time_info	get_time_by_today_stamp_new(int64 target_second);
		static time_info	get_time_by_stamp_new(int64 target_second, bool is_millisecond = false);

		static time_info	get_today_zero_time_info();
		static time_info	get_time_by_info(int32 year, int32 month, int32 day, int32 hour = 0, int32 min = 0, int32 sec = 0);
		static int64		get_local_cur_time_stamp();
	private:
	};
}

#endif // CS_DATE_HPP
