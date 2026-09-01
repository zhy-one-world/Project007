#ifndef _WS_WORLD_BOSS_WS_MGR_H_
#define _WS_WORLD_BOSS_WS_MGR_H_

#include "logic/type_def.hpp"
#include "Logic/world_boss_def.hpp"
#include "world_boss_ws.h"
#include "utility/score_indicator.h"
#include "legion_def.hpp"

namespace faith
{
	class client_session;
	struct s_item_info;

	class world_boss_ws_mgr
	{
		world_boss_ws_mgr();
		~world_boss_ws_mgr();
	public:
		static				world_boss_ws_mgr& get_instance()
		{
			static world_boss_ws_mgr instance;
			return instance;
		}

		void				tick(int32 dt);
		void				tick_3sec();

		void				clear_data();
		void				clear_world_boss_activity_info();
		void				init_manager();
		void				load_world_boss_event_end(const s_world_boss_info* world_boss_info_ptr , int32 info_num);
		void				init_world_boss_data(int32 respawn_point_template_id);
		world_boss_ws*		get_world_boss_info(int32 respawn_point_template_id);

		world_boss_ws*		get_empty_boss_info();

		world_boss_ws*		get_total_world_boss_info() {return m_world_boss_data;};

		//这俩函数逻辑根本就是一样的，但是add函数有针对Load的说明
		world_boss_ws*		add_world_boss_info(const s_world_boss_info world_boss_info);
		void				send_world_boss_all(client_session* session, bool is_need_show = false);
		void				send_world_boss_one(world_boss_ws send_info, bool is_need_show = false, bool is_only_to_first_killer = false);
		void				set_world_boss_dead(int32 npc_spawn_template_id, const xchar* killer_name);
		void                send_world_boss_list(); //向客户端发送世界boss玩家列表
		void				create_world_boss_success(guid_64 cur_boss_guid, int32 world_boss_array_index, int32 npc_spawn_template_id, int32 npc_spawn_line_id);
		//void				create_world_boss_ready_npc();
		//void				remove_world_boss_ready_npc(int32 ready_npc_id);

		void				operate_respawn_list(int32 npc_spawn_template_id, bool is_add);
		void				req_frist_kill_world_boss_prize(const guid_64& req_guid, int32 spawn_npc_template_id);
		void				first_killer_get_prize_func(int32 welfare_template_id, int32 is_get_prize ,const s_item_info (&reward_array)[max_world_boss_reward_num]);
		bool				create_world_boss(int32 NpcId);
		void				have_world_boss_time();
		void				send_all_boss_hp_per(client_session* session);

		void				set_damage_list(const score_record* top_record_arr, int32 record_num, int32 boss_type, bool is_legion_score = false, int32 cur_npc_id = 0);
		void				set_is_all_boss_dead(bool is_dead) { m_is_all_boss_dead = is_dead; };
		void				send_damage_list(client_session* session, int32 boss_id = 0);	//发送世界boss伤害列表需要用到boss_id
		void				clear_score() { m_damage_rank_indicator.clear_data(); };
		int32				get_legion_rank_with_legion_boss(guid_64 legion_guid);
		int64				get_legion_damage_with_legion_boss(guid_64 legion_guid);
		score_indicator*	get_legion_score_all();
		//rename
		void				change_player_name_func(const guid_64& role_guid, const xstring& role_name, const xstring& original_name);	//角色改名同步

		int32				get_cur_first_boss_spawn_id();//走服务器等级
		void				world_boss_change_legion_name_func(guid_64 legion_guid, const xchar* legion_name);

		void				send_leigon_welfare_activity_end(int32 activity_type);
		void				set_is_send_legion_welfare(bool is_send) { m_is_send_legion_welfare = is_send; }
		bool				get_is_send_legion_welfare() { return m_is_send_legion_welfare; }
		void                sync_single_boss_rank_result(client_session* session);
		void                sync_boss_rank_result();

		void                find_need_create_world_boss();
		void				assign_postion_to_world_boss();
		bool                can_create_world_boss(int32 boss_spawn_id);
		bool                can_remove_world_boss(int32 boss_spawn_id);
		void                assign_world_boss_to_legion();
		void                assign_world_boss_to_one_legion(const guid_64& legion_guid);
		void                send_strategy_world_boss_info(const guid_64& legion_guid, const guid_64& role_guid);
		guid_64*			find_legion_jion_member(const guid_64& legion_guid, int32& player_num);
		void                send_strategy_reward();
		void                send_world_boss_info(const guid_64& legion_guid, const guid_64& role_guid, int32 cur_boss_level);
		void                send_strategy_result();
		void                send_world_boss_notice();
		void				add_boss_already_dead_score();
		void				calculate_other_score(int32 boss_spawn_id);
		int32				get_world_boss_create_time(int32 boss_spawn_id);
		int32				get_world_boss_level(int32 boss_spawn_id);
	private:

		world_boss_ws*		m_world_boss_respawn_ptr_list[max_world_boss_and_gold_army_num];
		world_boss_ws		m_world_boss_data[max_world_boss_and_gold_army_num];
		float				m_last_save_time;
		int32				m_timer;
		int32				last_tick_time;
		int32				one_sec_time;
		int32				m_create_boss_tamp;
		int32				m_remove_boss_tamp;
		int32				m_ready_boss_tamp;
		bool				m_is_create_boss_ready_npc;
		std::vector<s_world_boss_ready_info>	m_create_ready_boss_npc_arr;
		score_indicator		m_damage_rank_indicator;
		score_indicator		m_legion_rank_indicator;
		score_indicator		m_activity_rank_indicator;									//军团伤害总排行 按军团上架机制的boss来算
		bool				m_is_send_legion_welfare;
		int32				m_3sec_tick;
		bool				m_is_all_boss_dead;
		int32				m_cur_score_npc_id;
		int32				m_cur_boss_type;

		bool				m_is_need_sort;

		bool				is_send_first_stage_notice;									//是否发送活动第一阶段公告
		bool				is_send_second_stage_notice;								//是否发送活动第二阶段公告
		bool				is_send_third_stage_notice;									//是否发送活动第三阶段公告
		int32               lower_boss_arr[max_world_boss_arr_num];                     //低级世界boss数组
		int32               intermediate_boss_arr[max_world_boss_arr_num];              //中级世界boss数组
		int32               senior_boss_arr[max_world_boss_arr_num];                    //高级世界boss数组
		bool                is_in_active_time;                                          //是否在活动时间内
		std::map<int32, score_indicator>		m_personal_world_boss_score_info;       //世界boss得分信息<boss_id,个人得分信息>
		std::map<int32, score_indicator>		m_legion_world_boss_score_info;         //世界boss得分信息<boss_id,军团得分信息>
		std::map<guid_64, guid_64>              m_all_join_world_boss_member;           //所有参加世界boss的成员<角色guid,角色对应军团guid>
		score_indicator							m_personal_total_rank_indicator;        //个人伤害总排行 按军团上架机制的boss来算
		std::map<int32, int32>					m_boss_create_pos;						//世界boss创建位置<boss_spawn_id,随机位置数组中的id>
		guid_64				legion_member_arr[max_legion_member_num];					//军团参加世界boss人员的数组
	};
}



#endif
