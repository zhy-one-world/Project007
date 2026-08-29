#pragma once
/********************************************************************
created: 2016/8/22
created: 22:8:2016 19:33
file base: ranking_def
file ext: hpp
author: xinglong

purpose:define ranking
*********************************************************************/
#ifndef _CLIENT_LOG_DEF_HPP_
#define _CLIENT_LOG_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"

namespace faith
{
#pragma pack(push,1)
	//const int32 ClientStart				= 1;	//ok
	//const int32 ShowLogin				= 3;	//ok
	const int32	SDKLoginSuccess			= 22;	//ok
	//const int32 ShowNotice				= 15;	//ok
	//const int32 ShowServerChoiceUI		= 16;	//ok
	//const int32 ClickServerBtn			= 17;	//ok
	const int32 ClickSdkLogin			= 21;
	const int32 ClickEnterGameBtn		= 25;	//ok
	//const int32 ShowRoleList			= 19;	//ok
	//const int32 ClickCreateRoleBtn		= 26;	//ok
	//const int32 CreateRoleSuccess		= 21;	//ok
	const int32 EnterScene				= 28;	//ok

#pragma pack(pop)
}

#endif