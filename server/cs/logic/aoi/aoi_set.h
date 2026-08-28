/********************************************************************
created:	2016年1月4日17:26:07
file base:	aoi_set_h
file ext:	h
author:		zhy

purpose:
*********************************************************************/
#ifndef _AOI_SET_H_
#define _AOI_SET_H_
#include "aoi_tower.h"
#include "logic/aoi_def.h"

namespace hld
{
	enum e_aoi_move_type;

	class aoi_set
	{
	public:
		aoi_set();
		~aoi_set();
	public:
		bool create_aoi(int32 begin_x, int32 end_x, int32 begin_y, int32 end_y, int32 tower_width, int32 tower_length);
	public:
		//获得某个坐标所有可见物体
		void get_unit_all(const fvector& unit_location, int32* unit_array, int32& watch_array_num);
		//获得移動過後的观察着
		const unit_index_map& get_watcher_all(const fvector& map_pos);
		void get_watcher_by_type(const fvector& map_pos_old, const fvector& map_pos_new, e_aoi_move_type move_type, int32* unit_ary, int32& watch_array_num);
	public:
		//把物体添加到aoi管理中
		bool add_object(const fvector& unit_location, const int32 unit_index);
		//把物体从aoi管理中删除
		bool remove_object(const fvector& unit_location, const int32 unit_index, bool is_dead);
		//更新物体aoi状态
		bool update_object(const fvector& map_pos_old, const fvector& map_pos_new, const int32& unit_index);
	private:
		//检测角色的坐标是否超出了地图范围
		const bool check_pos(const fvector& unit_location) const;
		//把角色的世界坐标转换成相对坐标
		const void trans_pos(const fvector& unit_location, s_tower_pos& tower_pos) const;
		void get_pos_limit(const s_tower_pos& map_pos, s_tower_pos* result);
		void get_pos_limit(const s_tower_pos& map_pos_old, const s_tower_pos& map_pos_new, e_aoi_move_type move_type, s_tower_pos* result);
		std::shared_ptr<aoi_tower> get_tower(const s_tower_pos& tower_pos);
	private:
		s_tower_pos						m_tower_pos_max;	//tower最大的长和宽
		std::unordered_map<int32, std::unordered_map<int32, std::shared_ptr<aoi_tower>>> m_aoi_tower_array;//所有的tower管理
		int32							m_map_begin_x;	//地图的开始的x
		int32							m_map_end_x;	//地图的结束的x
		int32							m_map_begin_y;	//地图的开始的y
		int32							m_map_end_y;	//地图的结束的y
		int32							m_tower_width;	//格子宽度
		int32							m_tower_length;	//格子长度
		s_tower_pos						m_tower_array[AOI_SEE_TOWER_NUM];//獲取玩家可見格子數
		s_tower_pos						m_tower_num[2];//格子的周長
		unit_index_map					m_empty_map;
	};
}
#endif

