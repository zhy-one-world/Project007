/********************************************************************
	created:	2014/07/26
	created:	26:7:2014   16:19
	file base:	cs_date
	file ext:	cpp
	author:		zhy

	purpose:
*********************************************************************/

#include <log/game_log.hpp>
#include <time.hpp>
#include <ctime>
#include "utility/cs_date.hpp"
#include "logic/time_def.hpp"

namespace faith
{

	void none_fuction(uint32 array_index, const void* data_ptr, size_t data_len)
	{
		return;
	}

	using namespace boost::gregorian;
	int32 cs_date::get_local_time()
	{
		time_t now = utility::time();
		tm* time_now = localtime(&now);
		cs_date date_now;
		date_now.set_date(time_now->tm_year + 1900, time_now->tm_mon + 1, time_now->tm_mday,
			time_now->tm_hour, time_now->tm_min, time_now->tm_sec);
		return 	date_now.get_date_int32();
	}

	//cc below
	uint32 time_helper::get_time()
	{
		time_t now = utility::time();
		return now;
	}
	uint32 time_helper::get_part_in_year(uint32 part_duration)
	{
		if (part_duration == 0)
			return 0;
		tm* t = get_localtime();
		return t->tm_yday / part_duration + 1;
}

	tm* time_helper::get_localtime()
	{
		time_t now = utility::time();

		return localtime(&now);
	}
	int32 time_helper::get_int_day()
	{
		tm* now_time = time_helper::get_localtime();
		if (nullptr == now_time)
		{
			return 0;
		}
		return now_time->tm_year * 10000 + now_time->tm_mon * 100 + now_time->tm_mday;
	}

	// 以秒为单位获取当天的时分秒
	int32 time_helper::get_today_time_in_sec()
	{
		tm* now_time = get_localtime();
		if (nullptr == now_time)
		{
			return -1;
		}

		int32 cur_time = now_time->tm_hour * 3600 + now_time->tm_min * 60 + now_time->tm_sec;
		return cur_time;
	}

	int32 time_helper::compare_tm(tm* tm_1, tm* tm_2)
	{
		if (nullptr == tm_1)
		{
			return -1;
		}
		if (nullptr == tm_2)
		{
			return 1;
		}
		if (tm_1->tm_year - tm_2->tm_year != 0)
		{
			return tm_1->tm_year - tm_2->tm_year;
		}
		if (tm_1->tm_mon - tm_2->tm_mon != 0)
		{
			return tm_1->tm_mon - tm_2->tm_mon;
		}
		if (tm_1->tm_mday - tm_2->tm_mday != 0)
		{
			return tm_1->tm_mday - tm_2->tm_mday;
		}
		if (tm_1->tm_hour - tm_2->tm_hour != 0)
		{
			return tm_1->tm_hour - tm_2->tm_hour;
		}
		if (tm_1->tm_min - tm_2->tm_min != 0)
		{
			return tm_1->tm_min - tm_2->tm_min;
		}
		if (tm_1->tm_sec - tm_2->tm_sec != 0)
		{
			return tm_1->tm_sec - tm_2->tm_sec;
		}
		return 0;
	}
	tm* time_helper::get_tm_by_time(uint32 time)
	{
		tm* timeStruct;
		time_t temp_time;
		temp_time = (time_t)time;
		timeStruct = localtime(&temp_time);
		timeStruct->tm_year = timeStruct->tm_year + 1900;
		timeStruct->tm_mon = timeStruct->tm_mon + 1;
		return timeStruct;
	}

	tm* time_helper::get_cur_tm_by_time(uint32 time)
	{
		tm* timeStruct;
		time_t temp_time;
		temp_time = (time_t)time;
		timeStruct = localtime(&temp_time);
		return timeStruct;
	}

	void time_helper::get_time_by_string(const char* str, tm* tm_)
	{
		if (nullptr == tm_ || nullptr == str || *str == '\0')
		{
			return;
		}
		tm* now_local_time_ptr = get_localtime();
		if (nullptr == now_local_time_ptr)
		{
			return;
		}

		int32 year, month, day, hour, minute, second;

		sscanf_s(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);

		if (year > 0)
		{
			tm_->tm_year = year - 1900;
		}
		else
		{
			tm_->tm_year = now_local_time_ptr->tm_year;
		}
		if (month > 0)
		{
			tm_->tm_mon = month - 1;
		}
		else
		{
			tm_->tm_mon = now_local_time_ptr->tm_mon;
		}
		if (day > 0)
		{
			tm_->tm_mday = day;
		}
		else
		{
			tm_->tm_mday = now_local_time_ptr->tm_mday;
		}
		tm_->tm_hour = hour;
		tm_->tm_min = minute;
		tm_->tm_sec = second;

		tm_->tm_yday = time_helper::get_days_in_year(year, month - 1, day);
		}

	uint32 time_helper::get_second()
	{
		tm* t = get_localtime();
		return t->tm_sec;
	}

	int32 time_helper::get_days_by_month(int y, int m)
	{
		int32 return_value = 0;
		int32 day[e_month_max] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		if (e_month_february == m)
		{
			return_value = ((((0 == y % 4) && (0 != y % 100)) || (0 == y % 400)) ? 29 : 28);
		}
		else if (m >= e_month_january
			&& m < e_month_max)
		{
			return_value = day[m];
		}
		return return_value;
	}
	int32 time_helper::get_days_in_year(int y, int m, int d)
	{
		int32 sun_d = 0;
		for (int i = e_month_january; i < m; i++)
		{
			sun_d += get_days_by_month(y, i);
		}
		sun_d += d;

		return sun_d - 1;
	}
	int32 time_helper::get_days_of_year(int y)
	{
		int32 days;
		if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
		{
			days = 366;
		}
		else
		{
			days = 365;
		}

		return days;
	}

	int32 time_helper::get_day_in_week(int y, int m, int d)
	{
		int32 week = 0;

		int32 century = y / 100;

		y = y % 100;

		week = y + y / 4 + century / 4 - 2 * century + (26 * (m + 1)) / 10 + d - 1;

		week = week % 7;

		return week;

	}

	xstring time_helper::get_current_time()
	{
		xchar temp[32] = "";
		time_info cur_time_info = time_helper::get_cur_time_new();
#if PLATFORM_WINDOWS
		int32 buffer_length = sizeof(temp);
		sprintf_s(temp, buffer_length, "%d-%02d-%02d %02d:%02d:%02d"
			, cur_time_info.year, cur_time_info.month_in_year + 1, cur_time_info.day_in_month + 1
			, cur_time_info.hour_in_day, cur_time_info.minute_in_hour, cur_time_info.second_in_minute);
#else
		sprintf(temp, "%d-%02d-%02d %02d:%02d:%02d"
			, cur_time_info.year, cur_time_info.month_in_year + 1, cur_time_info.day_in_month + 1
			, cur_time_info.hour_in_day, cur_time_info.minute_in_hour, cur_time_info.second_in_minute);
#endif
		xstring time = temp;
		return time;
	}
	xstring time_helper::get_cur_time()
	{
		xchar temp[32] = "";
		time_info cur_time_info = time_helper::get_cur_time_new();
#if PLATFORM_WINDOWS
		int32 buffer_length = sizeof(temp);
		sprintf_s(temp, buffer_length, "%d/%02d/%02d %02d:%02d:%02d"
			, cur_time_info.year, cur_time_info.month_in_year + 1, cur_time_info.day_in_month + 1
			, cur_time_info.hour_in_day, cur_time_info.minute_in_hour, cur_time_info.second_in_minute);
#else
		sprintf(temp, "%d/%02d/%02d %02d:%02d:%02d"
			, cur_time_info.year, cur_time_info.month_in_year + 1, cur_time_info.day_in_month + 1
			, cur_time_info.hour_in_day, cur_time_info.minute_in_hour, cur_time_info.second_in_minute);
#endif
		xstring time = temp;
		return time;
	}

	xstring time_helper::get_cur_day_hour()
	{
		xchar temp[32] = "";
		time_info cur_time_info = time_helper::get_cur_time_new();
#if PLATFORM_WINDOWS
		int32 buffer_length = sizeof(temp);
		sprintf_s(temp, buffer_length, "%d%02d%02d%02d"
			, cur_time_info.year, cur_time_info.month_in_year + 1, cur_time_info.day_in_month + 1, cur_time_info.hour_in_day);
#else
		sprintf(temp, "%d%02d%02d%02d"
			, cur_time_info.year, cur_time_info.month_in_year + 1, cur_time_info.day_in_month + 1, cur_time_info.hour_in_day);
#endif
		xstring time = temp;
		return time;
	}

	xstring time_helper::get_str_time_by_stamp(uint32 time_stamp)
	{
		time_info temp_time = time_helper::get_time_by_stamp_new(time_stamp);

		xchar temp[32] = "";
#if PLATFORM_WINDOWS
		int32 buffer_length = sizeof(temp);
		sprintf_s(temp, buffer_length, "%d-%02d-%02d %02d:%02d:%02d"
			, temp_time.year, temp_time.month_in_year + 1, temp_time.day_in_month + 1
			, temp_time.hour_in_day, temp_time.minute_in_hour, temp_time.second_in_minute);
#else
		sprintf(temp, "%d-%02d-%02d %02d:%02d:%02d"
			, temp_time.year, temp_time.month_in_year + 1, temp_time.day_in_month + 1
			, temp_time.hour_in_day, temp_time.minute_in_hour, temp_time.second_in_minute);
#endif
		xstring time = temp;
		return time;
	}

	xstring time_helper::get_str_time_by_tm(tm* tm_)
	{
		if (nullptr == tm_)
		{
			return "";
		}

		xchar temp[32] = "";
#if PLATFORM_WINDOWS
		int32 buffer_length = sizeof(temp);
		sprintf_s(temp, buffer_length, "%d-%02d-%02d %02d:%02d:%02d", tm_->tm_year, tm_->tm_mon, tm_->tm_mday, tm_->tm_hour, tm_->tm_min, tm_->tm_sec);
#else
		sprintf(temp, "%d-%02d-%02d %02d:%02d:%02d", tm_->tm_year, tm_->tm_mon, tm_->tm_mday, tm_->tm_hour, tm_->tm_min, tm_->tm_sec);
#endif
		xstring time = temp;
		return time;
	}

	bool time_helper::is_diff_day(uint32 time_a, uint32 time_b)
	{
		if (0 == time_a)
		{
			return true;
		}
		if (0 == time_b)
		{
			time_b = get_time();
		}
		time_t temp_time;
		temp_time = (time_t)time_a;
		tm time_struct_a = *localtime(&temp_time);

		temp_time = (time_t)time_b;
		tm time_struct_b = *localtime(&temp_time);

		if (time_struct_a.tm_yday != time_struct_b.tm_yday)
		{
			return true;
		}
		if (time_struct_a.tm_year != time_struct_b.tm_year)
		{
			return true;
		}
		return false;
	}


	bool time_helper::is_diff_month(uint32 time_a, uint32 time_b /*= 0*/)
	{
		if (0 == time_a)
		{
			return true;
		}
		if (0 == time_b)
		{
			time_b = get_time();
		}
		time_t temp_time;
		temp_time = (time_t)time_a;
		tm time_struct_a = *localtime(&temp_time);

		temp_time = (time_t)time_b;
		tm time_struct_b = *localtime(&temp_time);

		if (time_struct_a.tm_mon != time_struct_b.tm_mon)
		{
			return true;
		}
		if (time_struct_a.tm_year != time_struct_b.tm_year)
		{
			return true;
		}
		return false;
	}

	int64 time_helper::get_stamp_by_hour_min(int32 time_hour, int32 time_min, int32 cur_time)
	{
		if (cur_time <= 0)
		{
			tm time_struct = *time_helper::get_localtime();
			time_struct.tm_hour = time_hour;
			time_struct.tm_min = time_min;
			time_struct.tm_sec = 0;
			int64 time_stamp = mktime(&time_struct); //time_t
			return time_stamp;
		}
		else
		{
			tm time_struct = *time_helper::get_cur_tm_by_time(cur_time);
			time_struct.tm_hour = time_hour;
			time_struct.tm_min = time_min;
			time_struct.tm_sec = 0;
			int64 time_stamp = mktime(&time_struct); //time_t
			return time_stamp;
		}
	}

	int64 time_helper::get_stamp_by_min(int32 time_min)
	{
		tm time_struct = *time_helper::get_localtime();
		time_struct.tm_min = time_min;
		time_struct.tm_sec = 0;
		int64 time_stamp = mktime(&time_struct); //time_t
		return time_stamp;
	}

	int64	time_helper::get_next_refresh_time_stamp(int64 cur_time, int32 target_hour)
	{
		if (cur_time <= 0)
		{
			cur_time = get_time();
		}
		tm time_struct = *time_helper::get_localtime();
		if (target_hour >= 0
			&& target_hour < 24)
		{
			time_struct.tm_hour = target_hour;
		}
		time_struct.tm_min = 0;
		time_struct.tm_sec = 0;
		int64 time_stamp = mktime(&time_struct); //time_t

		if (cur_time >= time_stamp)
		{
			time_stamp = time_stamp + faith::day_time_second;
		}

		return time_stamp;
	}


	bool time_helper::is_over_refresh_time(int64 save_stamp_time)
	{
		if (save_stamp_time <= 0)
		{
			return true;
		}

		int64 cur_time = get_time();
		if (cur_time >= save_stamp_time)
		{
			return true;
		}

		return false;
	}

	time_info time_helper::get_cur_time_new()
	{
		time_info return_info;

		int64 now = 0;
		now = utility::get_tick_count();

		return_info = get_time_by_stamp_new(now, true);
		return return_info;
	}
	time_info time_helper::get_time_by_today_stamp_new(int64 target_second)
	{
		return get_time_by_stamp_new(get_today_zero_time_info().second + target_second);
	}
	time_info time_helper::get_time_by_stamp_new(int64 target_second, bool is_millisecond)
	{
		time_info return_info;
		time_t temp_time;
		if (is_millisecond)
		{
			temp_time = (time_t)(target_second / second_tick_time);
		}
		else
		{
			temp_time = (time_t)target_second;
		}
		tm* time_struct = localtime(&temp_time);
		if (time_struct == nullptr)
		{
			return return_info;
		}

		if (is_millisecond
			&& second_tick_time > 0)
		{
			return_info.second = target_second / second_tick_time;
			return_info.millisecond = target_second;
		}
		else
		{
			return_info.second = target_second;
			return_info.millisecond = target_second * second_tick_time;
		}

		return_info.second_in_minute = time_struct->tm_sec;
		return_info.second_in_hour = time_struct->tm_min * 60 + time_struct->tm_sec;
		return_info.second_in_day = time_struct->tm_hour * 3600 + time_struct->tm_min * 60 + time_struct->tm_sec;

		return_info.minute_in_hour = time_struct->tm_min;
		return_info.minute_in_day = time_struct->tm_hour * 60 + time_struct->tm_min;

		return_info.hour_in_day = time_struct->tm_hour;

		if (time_struct->tm_wday == 0)
		{//原始数据是 0|周日，1|周一，2|周二，3|周三，4|周四，5|周五，6|周六
			return_info.day_in_week = 6;
		}
		else
		{
			return_info.day_in_week = time_struct->tm_wday - 1;
		}
		return_info.day_in_month = time_struct->tm_mday - 1;	//原始数据从1开始
		return_info.day_in_year = time_struct->tm_yday;

		date temp_date(time_struct->tm_year + 1900, time_struct->tm_mon + 1, time_struct->tm_mday);
		return_info.week_in_year = temp_date.week_number() - 1;

		return_info.month_in_year = time_struct->tm_mon;

		return_info.year = time_struct->tm_year + 1900;
		return return_info;
	}

	time_info time_helper::get_time_by_info(int32 year, int32 month, int32 day, int32 hour, int32 min, int32 sec)
	{
		tm base_time;
		base_time.tm_year = year - 1900;
		base_time.tm_mon = month - 1;
		base_time.tm_mday = day;
		base_time.tm_hour = hour;
		base_time.tm_min = min;
		base_time.tm_sec = sec;
		int32 base_time_stamp = mktime(&base_time);
		return get_time_by_stamp_new(base_time_stamp);
	}

	int64 time_helper::get_local_cur_time_stamp()
	{
		return utility::get_local_tick_count();	
	}

	time_info time_helper::get_today_zero_time_info()
	{
		time_info cur_info = get_cur_time_new();
		int64 base_time = cur_info.second - cur_info.second_in_day;
		return get_time_by_stamp_new(base_time);
	}
	}
