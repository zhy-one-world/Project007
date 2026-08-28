#pragma once
/********************************************************************
created: 2016/10/17
file base: title_def
file ext: hpp
author: wucun

purpose:
*********************************************************************/

#ifndef _MISC_DEF_HPP_
#define _MISC_DEF_HPP_

namespace hld
{
#pragma pack(push,1)
	const int32 regular_notification_max_count = 1;		//定时推送最大次数
	const int32	regular_notification_day_num = 3;		//往后推几天
	const float float_type_compare_precision = 0.0001;	//float类型比较精度
	const int32 sdk_callback_array_max_num = 100;		//sdk回调数组大小

	enum e_notification_type
	{
		e_notification_type_regular,		//定时推送
		e_notification_type_activity,		//活动推送
		e_notification_type_offline_time,	//下线时间推送
		e_notification_type_vit_full,		//体力回满推送
	};

#pragma pack(pop)
}
#endif
