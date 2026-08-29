/********************************************************************
created: 2021年9月23日
file base: attack_city_ws_mgr
file ext: hpp
author: zhaoyuming
purpose:
*********************************************************************/
#ifndef _ATTACK_CITY_WS_MGR_H_
#define _ATTACK_CITY_WS_MGR_H_


#include "Logic/legion_def.hpp"
#include "Logic/attack_city_def.hpp"
#include <attack_city_msg.hpp>

namespace faith
{
	class attack_city_ws_mgr
	{
	public:
		explicit attack_city_ws_mgr();
		static attack_city_ws_mgr& get_instance()
		{
			static attack_city_ws_mgr instance;
			return instance;
		}

	public:

		// 时间Tick
		void						tick(int64 time_new);
		// 数据清理
		void						clear_data();
		// 初始化配置数据
		void						init_manager(bool need_load_dp = true);
		// 加载排名数据
		void						load_rank_data();
		// 加载排名数据结果
		void						clear_rank_data(bool is_clear_dp = false);
		// 添加排名数据
		void						add_rank_data(const s_attack_city_rank_info& rank_info, bool is_save = false);
		// 保存全部排名数据
		void						save_rank_data_all(bool is_clear_dp = false);
		// 保存单人排名数据
		void						save_rank_data_one(const s_attack_city_rank_info& rank_info);
		// 加载分组数据
		void						load_group_data();
		// 清理分组数据
		void						clear_group_dp_data();
		// 清理排名数据
		void						clear_rank_dp_data();
		// 加载分组数据结果
		void						load_group_data_end(const s_attack_city_legion_info * dp_info, int32 data_num);
		// 保存分组数据
		void						save_group_data(const s_attack_city_legion_info& legion_info);
		// 保存分组数据
		void						save_group_data_all(bool is_clear_dp = false);
		// 是否在活动时间内
		bool						is_in_game_time();
		// 是否是检查时间
		bool						is_in_check_time();
		// 是否是分组时间
		bool						is_in_group_time();
		// 活动开启初始化
		void						game_init();
		// 活动开启前检查
		void						game_check();
		// 活动检查
		void						check_legion_is_valid(guid_64 legion_guid);
		// 活动检查结果
		void						set_legion_is_valid(guid_64 legion_guid);
		// 活动开启初始化
		void						game_group();
		// 临时分组
		void						tem_game_group();
		// 活动结束
		void						game_end();
		// 获取军团信息
		void						send_attack_city_legion_info(guid_64 legion_guid, guid_64 role_guid, int32 server_id);
		// 获取军团信息
		void						send_attack_city_legion_rank_info(guid_64 legion_guid, guid_64 role_guid, int32 server_id);
		// 活动结束
		void						send_load_succend(int32 server_id);
		// 刷新军团信息
		void						refresh_legion_list();
		// 清除军团信息
		void						clear_legion_list();
		// 军团改名
		void						change_legion_name(guid_64 legion_guid, xstring legion_name);
		////////////////////////////////////活动地图相关////////////////////////////////////
		// 传送到活动地图
		void						recv_other_ws_transger_gate_map(guid_64 role_guid, guid_64 legion_guid, int32 server_id);
		// 检查是否可以进入地图
		int32						check_can_transfer_to_map(guid_64 map_guid);
		//  传送返回结果处理
		void						transfer_to_map_with_gate_msg(guid_64 role_guid, int32 error_ret, int32 map_temp_id, guid_64 map_guid, int32 war_idex, int32 server_id);



		////////////////////////////////////工具函数////////////////////////////////////
		// 获取配置参数
		int32						get_config_data(int32 data_type);
		// 根据军团Guid获取地图Guid
		int32						get_map_guid_by_legion(guid_64 legion_guid, guid_64& map_guid);
		// 获取军团引用
		s_attack_city_legion_info&	get_legion_info(guid_64 legion_guid);
		// 同步数据到其他服务器
		void						send_info_all_to_other_ws(int32 server_id);

	private:
		// 是否进行过初始化
		bool										m_is_begin_time;
		// 是否进行军团检查
		bool										m_is_check_time;
		// 是否进行分组
		bool										m_is_group_time;
		// 空军团信息
		s_attack_city_legion_info					m_empty_legion_info;
		// 配置文件数组
		int32										m_config_data[e_attack_city_data_max];	
		// 攻城战地图Id
		guid_64										m_map_guid_list[e_attack_city_group_level_max];
		// 所有参战军团信息
		std::vector<s_attack_city_legion_info>		m_legion_info_list;	
		// 参战军团临时信息
		std::vector<s_attack_city_legion_info>		m_tem_legion_info_list;
		// 战场排名信息
		std::vector<s_attack_city_rank_info>		m_rank_list;
		// 分组tick时间
		int64										m_tem_game_group_time;
		
		bool										m_is_legion_data;
	};
}

#endif