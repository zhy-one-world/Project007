/********************************************************************
	created:	2015/03/23
	created:	23:3:2015   16:24
	file base:	world_def
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _WORLD_DEF_HPP_
#define _WORLD_DEF_HPP_

#include "logic/type_def.hpp"
#include <string>

namespace hld 
{
#pragma pack(push,1)

	const int32 color_arr_size = 4; // RGBA
	const int32 cross_state_mark = 500000;//跨服组标记位
	const int32 cross_state = 10000;
	const int32 gm_set_result_len = 30;

//////////////////////////////////WSloading链////////////////////////////////////////
	const int64 e_ws_flag_none =											0x0000000000000000;
	const int64 e_ws_flag_big_player =										0x0000000000000001;//名人堂
	const int64 e_ws_flag_cs_connect =										0x0000000000000002;//CS连接成功
	const int64 e_ws_flag_gm_common =										0x0000000000000004;//GM指令load完毕
	const int64 e_ws_flag_harry =											0x0000000000000008;//跨服掠夺
	const int64 e_ws_flag_cross_pk =										0x0000000000000010;//跨服职业竞技
	const int64 e_ws_flag_overload_war =									0x0000000000000020;//跨服霸主战
	const int64 e_ws_flag_city_war =										0x0000000000000040;//跨服城战
	const int64 e_ws_flag_cross_boss =										0x0000000000000080;//跨服BOSS
	const int64 e_ws_flag_server_cross_time =								0x0000000000000100;//服务器跨服时间
	const int64 e_ws_flag_server_refresh =									0x0000000000000200;//服务器server_refresh表中的数据，包含各种刷新时间以及服务器等级
	const int64 e_ws_flag_time_limit_info =									0x0000000000000400;//限时活动数据
	const int64 e_ws_flag_time_limit_template =								0x0000000000000800;//限时活动表格数据
	const int64	e_ws_flag_ranking_info =									0x0000000000001000;//排行榜数据
	const int64	e_ws_flag_load_legion_info =								0x0000000000002000;//军团数据
	const int64	e_ws_flag_cross_server_state =								0x0000000000004000;//跨服状态
	const int64 e_ws_flag_init_time_limit_template =						0x0000000000008000;//限时活动表格数据初始化完成
	const int64 e_ws_flag_load_cloud_shop_info =							0x0000000000010000;//云购数据读取
	const int64 e_ws_flag_fep_connect =										0x0000000000020000;//fep连接成功
	const int64 e_ws_flag_attack_city =										0x0000000000040000;//攻城战
///////////////////////////////////////////////////////////////////////////////////////

	//	场景相关常量定义
	enum
	{
		e_zone_uid_invalid				= 0,			//	非法的区域UID
		e_invalid_instance_obj_id		= 0,			// 无效副本实例ID
		e_max_limited_broadcast_count	= 0xFFFFFFFF,
	};

	//如果修改请注意数据库中相关逻辑
	enum e_scene_type : uint32
	{
		e_scene_type_low_world,				//0无缝主世界,本项目无用(0)
		e_scene_type_low_seam_world,		//1有缝世界(0)
		e_scene_type_low_instance,			//2普通副本进入(type_id)默认跟随队友
		e_scene_type_low_new_instance,		//3创建新实例并进入(type_id)强制
		e_scene_type_low_instance_object,	//4副本运行实例(id)
		e_scene_type_low_map,				//5 low地图(id)
		e_scene_type_low_load_area,			//6载入区域
		e_scene_type_low_nouse0,			//7阵营(id/0)
		e_scene_type_low_guild,				//8家族(id/0)
		e_scene_type_low_battle,			//9战场(id/0)
		e_scene_type_low_nouse1,			//10跟随玩家(id)
		e_scene_type_low_exit,				//11返回上一地点(退出副本等)
		e_scene_type_low_local = 0xffffffff,//本地
	};

	enum e_need_server_cross
	{
		e_need_server_cross_none,
		e_need_server_cross_begin_cross,//开始跨服
		e_need_server_cross_begin_cross_pk,//开始跨服职业竞技
		e_need_server_cross_begin_cross_activity,//开始跨服PK之王和破碎虚空
		e_need_server_cross_begin_cross_harry,//开始跨服掠夺
		e_need_server_cross_begin_cross_city_war,//开始霸主战
		e_need_server_cross_begin_cross_legion_territory_war,//开始跨服城战
		e_need_server_cross_begin_cross_boss,	//开启跨服世界boss
		e_need_server_cross_begin_cross_ladder,//开始跨服天梯
		e_need_server_cross_begin_cross_cloud,//开始跨服云购
		e_need_server_element_war,	//开始元素争霸
		e_need_server_attack_city,	//开始攻城战
		e_need_server_cross_max
	};

	enum e_server_info_type
	{
		e_server_info_type_begin_cross_server_time,				//开始跨服时间
		e_server_info_type_server_state,						//服务器类型（新，老，和，跨等)
		e_server_info_type_server_cross_state,					//服务器跨服类型（新，老等）
		e_server_info_type_begin_first_cross_server_time,		//第一次开启跨服的时间 不受同步影响
		e_server_info_type_begin_first_merge_server_time,		//第一次开启合服的时间
		e_server_info_type_time_limit_ranking_level,			//限时活动等级 用于读取不同等级区间数据 只在没开启任何跨服活动时刷新
		e_server_info_type_close_exchange,						//关闭礼品兑换
		e_server_info_type_only_create_role,                    // 创建角色服
		e_server_info_type_max
	};

	enum e_server_state
	{
		e_server_state_none,//空
		e_server_state_new,//新服组
		e_server_state_cross,//跨服组
		e_server_state_merge,//合服组
		e_server_state_max
	};

	enum e_server_cross_state
	{
		e_server_cross_state_none,//空
		e_server_cross_state_new,//新服跨服组
		e_server_cross_state_mid,//跨服组
		e_server_cross_state_old,//老跨服组
		e_server_cross_state_max
	};

	enum e_http_error_code
	{
		e_http_error_code_no_error = 0,
		e_http_error_code_wrong_server_id,
		e_http_error_code_operate_failed,
		e_http_error_code_json_parse_failed,
		e_http_error_code_server_is_running,
		e_http_error_code_wrong_server_type,
		e_http_error_code_proto_parse_failed,
		e_http_error_code_db_error,
		e_http_error_code_role_online,
		e_http_error_code_time_out,
	};

	struct world_location
	{
		world_location()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		uint32			transfer_type_id;
		uint32			instance_type_id;
		s_map_pos		map_pos;
	};

#pragma pack(pop)
}

#endif
