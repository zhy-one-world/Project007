/********************************************************************
created: 2017Äê3ÔÂ21ÈÕ13:39:29
file base: client_net
file ext: hpp
author: zhy

purpose:
*********************************************************************/
#ifndef _CLIENT_NET_HPP_
#define _CLIENT_NET_HPP_

#define USE_UNREAL_NET
//#define USE_BOOST_NET
//#define USE_UNREAL_TIMER
enum ENetType
{
	ENetType_UE,
	ENetType_Boost,
	ENetType_Timer,
	ENetType_Max,
};

const float		RECEIVER_WAIT_TIME = 0.001;
const uint32	SEND_WAIT_TIME = 1;
const uint32	MAX_INTERNET_COMMAND_SEND_PER_TIME = 5;
const uint32	INIT_RECE_VECTOR_LEN = 1000;
const uint32	INIT_BUFFER_ARRAY_LEN = 4096 * 1024;
const uint32	INIT_SEND_VECTOR_LEN = 1000;
const int64		HEART_TICK_TIME = 300000000;
const int32		CLIENT_RECV_BUFFER_SIZE = 4096 * 1024;
const int32		CLIENT_SEND_BUFFER_SIZE = 4096 * 1024;
const int32		SERVER_MAX_CONNECT = 20;
const int32		SERVER_RECV_BUFFER_SIZE = 256 * 1024;
const int32		SERVER_SEND_BUFFER_SIZE = 512 * 1024;
const int32		SERVER_THREAD_COUNT = 4;
const uint32	HEAD_LENGTH = 8;
const int32		LostNetTime = 30 * 1000 * 10000;
const int32		Lost_Msg_Num = 30;

#endif