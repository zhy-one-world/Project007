#pragma once
/********************************************************************
created: 2016年7月15日
file base: achievement
author: 张宇翔
purpose: 成就管理系统
*********************************************************************/
#ifndef _ACHIEVEMENT_MGR_HPP_
#define _ACHIEVEMENT_MGR_HPP_


#include "achievement.hpp"

namespace faith
{
	class player;
	class npc;
	class skill;
	class map_record;
	class achievement_proto_achievement_one;
	class s_achievement_info;

	class cachievement_mgr
	{
	public:
		cachievement_mgr();
		~cachievement_mgr();
	public:
		//初始化
		void clear_data();
		void set_player_ptr(const int32 array_index);
		void tick(float elapse_time);
		//消息
		void send_one_achievement(cachievement& my_achievement);
		void send_all_achievement();
		void make_one_achievement_msg(achievement_proto_achievement_one& achievement_msg, const cachievement& my_achievement);
		//存档
		void save_achievement_to_db(int32 save_type);
		void load_achievement_by_db(const s_achievement_info* achievement_data, int32 achievement_num);
		bool load_achievement_by_db_lua(const char *data_ptr, int32 data_len);

		cachievement* create_achievement_by_template(int32 achievement_id);
		cachievement* create_achievement_by_info(const s_achievement_info& temp_info);

		//操作
		cachievement*	find_achievement(int32 achievement_id);
		std::vector <cachievement*>   get_all_can_receive_achievement();
		void npc_killed(npc* const dead_npc_ptr);//杀怪相关判断
		void item_changed(citem* changed_item_ptr,e_item_operation operation_type);//物品相关判断
		void player_changed(e_achievement_type operate_type);
		void skill_level_up(skill* upgrade_skill);
		void skill_level_count(int32 new_value);
		void mission_finish(cmission* finished_mission);
		void fubenfinish(map_record* new_record);
		void create_team();
		void create_legion();
		void save_achievement_log(int32 achievement_id);

		void set_achievement_success(int32 achievement_id, int32 achievement_state);
		void set_can_receive_achievement_success();
		////属性
	private:
		std::map<int32, cachievement>	m_achievement_array;
		std::vector<cachievement*>		m_achievement_about_npc;
		std::vector<cachievement*>		m_achievement_about_item;
		std::vector<cachievement*>		m_achievement_about_self;
		std::vector<cachievement*>		m_achievement_about_skill;
		std::vector<cachievement*>		m_achievement_about_mission;
		std::vector<cachievement*>		m_achievement_about_map;
		std::vector<cachievement*>		m_achievement_about_team;
		std::vector<cachievement*>		m_achievement_about_legion;
		int32				m_array_index;
	};
}
#endif