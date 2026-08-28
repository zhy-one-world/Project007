/********************************************************************
created: 2014/07/11
created: 11:7:2014 11:43
file base: player
file ext: hpp
author: zhy

purpose: about role and role's operation
*********************************************************************/
#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

/************************************************************************/
/*                           File Include                               */
/************************************************************************/
#include "Logic/login_def.hpp"
#include "Logic/server_log_def.hpp"
#include "Logic/legion_def.hpp"
#include "legion_skill.h"
#include "logic/type_def.hpp"
#include "logic/unit.hpp"
#include "belief_mgr.h"
#include "talent_mgr.h"
#include "welfare_mgr.h"
#include "pokedex_mgr.h"
#include "auction_mgr.h"
#include "chat_mgr.h"
#include "mission_mgr.hpp"
#include "treasure_mgr.h"
#include "convert_mgr.h"
#include "achievement_mgr.hpp"
#include "service_goal_mgr.hpp"
#include "seven_day_goal_mgr.h"
#include "teleport_control.hpp"
#include "fuben_data_set.h"
#include "store_mgr.hpp"
#include "player_arena_mgr.hpp"
#include "meditation_manager.h"
#include "patron_saint_mgr.h"
#include "special_name_mgr.hpp"
#include "title_mgr.h"
#include "player_aoi_watch.h"
#include "interaction_mgr.hpp"
#include "mail_mgr.h"
#include "guide_mgr.h"
#include "first_time_do_mgr.h"
#include "ranking_mgr.h"
#include "passive_skill.h"
#include "worship_target.h"
#include "func_unlock_mgr.h"
#include "service_rank_mgr.h"
#include "gain_treasure_mgr.h"
#include "boss_island_mgr.h"
#include "time_limit_activity_mgr.h"
#include "belief_rune_mgr.h"
#include "Logic/server_harry_mgr.h"
#include "feather_heart_mgr.h"
#include "phantom_mgr.h"
#include "recycle_mgr.h"
#include "awaken_mgr.h"
#include "goddess_mgr.h"
#include "marry_mgr.h"
#include "oracle_trial_mgr.h"
#include "person_information_mgr.h"
#include "role_competition_mgr.h"
#include "legion_dance_mgr.h"
#include "assist_fight_cs_mgr.h"
#include "dragontrip_mgr.h"
#include "mount_power_mgr.h"
#include "jewel_carve_mgr.h"
#include "skytreasure_mgr.h"
#include "starark_mgr.h"
#include "lucky_card_mgr.h"
#include "element_competition_mgr.h"
#include "time_feed_back_mgr.h"
#include "time_limit_gift_mgr.h"
#include "star_trip_mgr.h"
#include "map_record_set.h"
/************************************************************************/
/*                            Maroc Define                              */
/************************************************************************/
namespace hld
{
	class npc;
	class citem;
	class character_s2s_sl_logic_data;
	class character_s2s_sl_time_data;
	struct s_fake_player_info;
	struct s_log_common_head;
	struct finger_guess_component;
	struct smashing_bottle_component;
	struct time_activity_component;
	struct item_upgrade_component;




	struct player_grade_and_level
	{
		int32 grade_num;
		int32 level_of_cur_grade;
	};

	struct interact_npc_info
	{
		bool		is_interact;
		int64		finish_interact_time;
		NpcTemplate	npc_template;			//多人采集一个东西时，后开始采集的玩家的这个数据中的npc指针会被清空或者变野指针，所以要单存这些东西
		guid_64		npc_guid;				//多人采集一个东西时，后开始采集的玩家的这个数据中的npc指针会被清空或者变野指针，所以要单存这些东西
		int32		max_distance;

		int32		npc_array_index;

		interact_npc_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			max_distance = default_gather_distance;
		}
	};

	const int32 npc_position_max = 128;
	const int32 recharge_exclusive_type = 2;
	class player : public unit
	{
	public:
		player();
		virtual ~player();
	private://内存已经创建好了 禁止拷贝
		player(const player& unit_ref);
		player& operator=(const player&);
	public:
		static player*								cast(unit* o);
	public:
		void										set_client_uid(const s_client_uid& client_uid) { m_client_uid = client_uid; }
		void										init_enter_data(int32 login_type, s_client_uid client_uid, const login_fixed_data& login_data);
		void										set_array_index(int32 array_index);
	public:
		/************************************************************************/
		/*							初始化以及数据载入、存盘					*/
		/************************************************************************/
		void 										init_by_inst_data_over();
		int32										put_in_scene();
		void										check_srv_world_level_buff();
		void										cost_enter_scene_consume();
		void										refresh_heart_beat();
		virtual void								heart_tick(const int64& new_time, const int32& tick_time);
		void										tick_2_sec(const int64& new_time);
		void										check_vip_experience(const int64& new_time);
		void										clear_data();
		void										clear_trans_cache();
		void										set_loading_flag(e_data_flag flag);
		bool										get_loading_flag_all_finish(void);
		void										load_role_data_online(const guid_64& up_role_guid); // 上线基础数据Loading加载	
		void										send_role_data_online(); // 发送玩家所有的在线数据
		void										clear_saving_flag(int32 type);
		void										set_saving_flag(int32 type, e_data_flag flag);
		bool										get_saveing_flag_all_finish(void);
		void										save_role_all_data(e_save_role_data_type eType = e_save_data_type_update);	// 角色下线、传送、实时存盘数据统一处理
		void										map_in_out_hp_process();
		void										set_update_db_flag(bool saving_flag) { m_update_db_flag = saving_flag; }
		void										push_all_info_to_client();
		void										on_player_disconnect();
		void										on_player_reconnect(const s_client_uid& client_uid, int32 client_map_id, int32 client_line_id);

		virtual	void								be_dead(bool is_auto_put_into_bag = false);
		virtual bool								can_become_target();
		void										save_yesterday_must_do_remain(int32 save_typ); //存储昨日未完成的每日任务数据
		void										load_yesterday_must_do_remain_end(const s_unit_yesterday_must_do_remain* data_info); //读取昨日未完成的每日任务数据
		bool                                        load_yesterday_must_do_remain_by_db_lua(const char *data_ptr, int32 data_len);
		login_fixed_data&							get_third_info() { return m_login_third_data; };
		int64										get_login_time() { return m_login_time; };
		void										set_login_time(int64 login_time) { m_login_time = login_time; };
		void										save_daily_must_do_count(int32 save_typ);
		void										load_daily_must_do_count(s_role_daily_must_do_count data_info); //读取每日必做任务次数
		bool                                        load_daily_must_do_count_by_db_lua(int64 role_id,const char *data_ptr, int32 data_len);
		void										set_daily_active_degree_info(e_daily_must_do_typ eType, int32 add_num = 1); //每日活跃度同步
		int32                                       is_need_daily_finish(e_daily_must_do_typ eType);
		void										send_dead_notice_with_cross_server(int32 server_id, std::string killer_name);
		int32										get_daily_exp_raid_time();				//获取经验副本根据活跃时间获得的时间(秒)
		int32										get_max_daily_exp_raid_time();			//获取经验副本根据最大活跃时间获得的时间(秒)
		void										reload_csv();
		int32										get_leave_num() { return m_leave_num; }
		void										set_leave_num(int32 leave_num) { m_leave_num = leave_num; }

		void										set_send_gate_save_end(bool need_send) { m_is_need_send_gate_save_end = need_send; };
		bool										get_send_gate_save_end() { return m_is_need_send_gate_save_end; };
		bool										m_is_need_send_gate_save_end;//是否需要发save结束信息到本服

		/************************************************************************/
		/*                            发送消息									*/
		/************************************************************************/
	public:
		virtual	void								get_aoi_msg(hld::aoi_proto_unit_aoi_all& msg);
		virtual void								send_message_to_dp(const void* data, uint32 date_len);
		virtual void								send_message_to_dp(google::protobuf::Message* net_pro, uint32 heade, int32 save_type_ex = 0);
		virtual void								send_message_to_dp(google::protobuf::Message* net_pro, uint32 heade, int64 save_flag, int32 save_type_ex = 0);
		virtual int32								send_message(const void* data, uint32 date_len)	override;
		virtual int32								send_message(google::protobuf::Message* net_pro, uint32 header) override;
		virtual void								send_message_to_aoi(google::protobuf::Message* net_pro, uint32 header, bool to_self = true, bool use_length = false);
		virtual void								send_message_to_self(void* data, uint32 date_len) override;
		virtual void								send_message_to_self(google::protobuf::Message* net_pro, uint32 header) override;
		void										send_message_to_self_lua(const char* msg, int32 msg_len, uint32 header);
		void										send_message_to_aoi_lua(const char* msg, int32 msg_len, uint32 header, bool to_self, bool use_length);
		void                                        send_message_to_ws_lua(const void* data_ptr, int32 data_len, int32 server_id = 0);
		void                                        send_message_to_dp_lua(const char* msg, int32 msg_len, uint32 header);
		void                                        send_message_to_dp_lua(google::protobuf::Message* net_pro, uint32 heade);
		/************************************************************************/
		/*                            物品相关                                  */
		/************************************************************************/
	public:
		void                                        set_bag_slot_open_num(int32 num); // 设置角色当前背包中开启的物品格的数目
		void										set_storage_slot_open_num(int32 num);//设置角色当前仓库中开启的物品格的数目
		void										open_bag(const int64& diff_time);//背包开启
		void										storage_open(const int64& diff_time);//仓库开启
		void										send_open_bag_info(); // 发送背包开放的
		bool                                        unlock_bag_slot(int32 unlock_slot_index, int32 unlock_storage_slot_index); // 根据物品格子在物品栏中下表解锁相应的格子
		bool                                        unlock_storage_slot(int32 unlock_slot_index, int32 unlock_storage_slot_index);
		cwelfare_mgr&								get_welfare_mgr() { return m_welfare_mgr; }
		cauction_mgr&								get_auction_mgr() { return m_auction_mgr; }
		void										recovery_for_item(const DropTemplate* drop_template_ptr, std::vector< int32 >& item_ids, std::vector<int32>& item_num, std::vector<int32>& item_array);
		int32										get_highest_mount_level();
		void										fix_bag_slot_open_time_by_vip(int32 old_vip_level, int32 new_vip_level);
		void										fix_storage_slot_open_time_by_vip(int32 old_vip_level, int32 new_vip_level);
		void										drop_item_send_notice(const std::vector<s_item_info>& item_data, int32 drop_npc_temp_id);
		goddess_mgr&								get_goddess_mgr() { return m_goddess_mgr; }

		bool										get_is_have_cant_equip_item() { return m_is_have_cant_equip_item; }
		void										set_is_have_cant_equip_item(bool cant_equip) { m_is_have_cant_equip_item = cant_equip; }
	private:
		bool										m_is_have_cant_equip_item;
	public:
		/************************************************************************/
		/*                            信仰相关                                  */
		/************************************************************************/
		cbelief_mgr&								get_belief_mgr() { return m_belief_mgr; }
		belief_rune_mgr&							get_belief_rune_mgr() { return m_belief_rune_mgr; }

		/************************************************************************/
		/*                            天赋相关                                  */
		/************************************************************************/
		ctalent_mgr&								get_talent_mgr() { return m_talent_mgr; }

		/************************************************************************/
		/*                            图鉴相关                                  */
		/************************************************************************/
		cpokedex_mgr&								get_pokedex_mgr() { return m_pokedex_mgr; }
		int32										get_pokedex_finished_num();
		/************************************************************************/
		/*                            守护神相关                                  */
		/************************************************************************/
		cpatron_saint_mgr&							get_patron_saint_mgr() { return m_patron_saint_mgr; }
		/************************************************************************/
		/*                            最高记录                                  */
		/************************************************************************/
		void										load_history_highest_record(const s_role_history_high_record_info& highest_record_info);
		bool                                        load_history_highest_record_by_db_lua(const char *data_ptr, int32 data_len);
		void										send_history_highest_record_all();
		void										send_history_highest_record_one(int32 info_index);
		void										save_history_highest_record(int32 save_type_ex);
		int64										get_highest_record(int32 index);
		void										set_highest_record(int32 index, int64 value);
		void										upgrade_highest_record(e_role_history_highest_record record_type, int64 new_value);
		bool										check_and_set_highest_record(e_role_history_highest_record record_type, int64 single_type_target_level = 0, int64 single_type_target_color = 0);
		int64										get_new_highest_value(e_role_history_highest_record record_type, int64 single_type_target_level = 0, int64 single_type_target_color = 0);

		/************************************************************************/
		/*							  Marry相关                                 */
		/************************************************************************/
	public:
		void										clear_couple_info();
		void										check_can_marry(const guid_64& sub_guid, int32 marry_item_id, bool is_cost);
		void										check_can_divorce(bool is_cost);
		void										set_couple_guid(const guid_64& couple_guid) { m_couple_guid = couple_guid; };
		guid_64&									get_couple_guid() { return m_couple_guid; };
		bool										had_couple() { return m_couple_guid.is_valid(); };
		void										set_couple_name(const xchar* couple_name);
		xchar*										get_couple_name() { return m_couple_name; };
		void										get_new_married_reward_end(int32 old_marry_state);
		bool										is_couple(const guid_64& role_guid) { return get_couple_guid() == role_guid; };
		void										sync_married_info_guid_to_aoi(const guid_64& role_guid, const  xchar* couple_name, bool is_marry);
		void										handle_marry_result_func(const guid_64& couple_guid, const  xchar* couple_name, bool is_marry, int32 marry_item_id);

		marry_mgr&									get_marry_mgr() { return m_marry_mgr; }
		void										sync_wedding_ring_level_to_ws();
		void										get_marry_dati_reward(int32 dati_id, bool is_true);
		void										get_marry_dati_last_reward();
		void										add_heart_value(int32 heart_value, int32 send_type = 0);
		void										player_changed(e_achievement_type operate_type);
		void										add_title_by_type_and_value(e_title_type title_type, int32 value1 = 0, int32  value2 = 0);
	private:
		marry_mgr									m_marry_mgr;
		guid_64										m_couple_guid;
		xchar										m_couple_name[max_name_size + 1];
	public:
		/************************************************************************/
		/************************************************************************/
		/*                            排行榜相关                                */
		/************************************************************************/

		cworship_target&							get_worship_target() { return m_worship_target; }
		cranking_mgr&								get_ranking_mgr() { return m_ranking_mgr; }

		/************************************************************************/
		/*                            任务相关                                  */
		/************************************************************************/

		cmission_mgr&								get_mission_mgr() { return m_mission_mgr; }
		void										check_captain_mission_by_npc_dead(e_mission_slot mission_index, int32 npc_id);
		int32										get_mission_id(e_mission_slot mission_index);
		void										change_mission(e_mission_slot mission_index, int32 new_mission_id);
		int32										get_new_mission_id(e_mission_slot mission_index);
		bool										is_cur_mission_accord_level(e_mission_slot mission_index);
		bool										is_cur_mission_high_level(e_mission_slot mission_index);
		bool										is_special_mission_finished(e_mission_slot mission_slot);
		bool										is_special_mission_unlocked(e_mission_slot mission_slot);

		void										fuben_map_enter_activity();
		//成就
		int32										get_element_heart_equip_num();


		cachievement_mgr&							get_achievement_mgr() { return m_achievement_mgr; }
		cservice_goal_mgr&							get_service_goal_mgr() { return m_service_goal_mgr; }
		cseven_day_goal_mgr&						get_seven_day_goal_mgr() { return m_seven_day_goal_mgr; }

		int32										get_pound_num() { return m_cur_pound_num; }
		void										set_pound_num(int32 pound_num) { m_cur_pound_num = pound_num; }


		feather_heart_mgr&                          get_feather_heart_mgr() { return m_feather_heart_mgr; }
		cphantom_mgr&                               get_phantom_mgr() { return m_phantom_mgr; }
		recycle_mgr&                                get_recycle_mgr() { return m_recycle_mgr; }
		awaken_mgr&                                 get_awaken_mgr()  { return m_awakan_mgr;  }

		/************************************************************************/
		/*                            社交相关                                  */
		/************************************************************************/
		cmail_mgr&									get_mail_mgr() { return m_mail_mgr; }
		cchat_mgr&									get_chat_mgr() { return m_chat_mgr; }
		cinteraction_mgr&							get_interaction_mgr() { return m_interaction_mgr; }

		/************************************************************************/
		/*                            交互相关                                  */
		/************************************************************************/
		//void										prepare_interaction(guid_64 be_invited_interaction_guid, guid_64 initiator_interaction_guid, int32 be_invited_interaction_type, int32 initiator_interaction_type);
		//void										update_interaction(guid_64 be_invited_interaction_guid, guid_64 initiator_interaction_guid, int32 be_invited_interaction_type, int32 initiator_interaction_type, int32 interaction_result);
		//void										start_interaction(guid_64 be_invited_interaction_guid, guid_64 initiator_interaction_guid, int32 be_invited_interaction_type, int32 initiator_interaction_type);



		//e_interaction_type							m_interaction_type;
		//guid_64										m_interaction_object_guid;

	public:
		/************************************************************************/
		/*                            时间相关                                  */
		/************************************************************************/
		void										load_role_time(const character_s2s_sl_time_data& msg);
		bool                                        load_role_time_by_db_lua(const char *data_ptr, int32 data_len);
		void										save_role_time(int32 save_type_ex);
		i64											get_time_data(e_time_type time_type);
		void										set_time_data(e_time_type time_type, i64 time_num);
		int32                                       get_time_data_sec(e_time_type tim_type);
		
		void										send_time_all();
		void										send_time_one(e_time_type time_type);
		i64											get_old_tick_time() { return m_old_tick_time; };
		void										send_open_server_time_to_self();

		std::shared_ptr<map_record_set>				get_map_record_mgr() { return m_map_record_mgr; }
	public:
		/************************************************************************/
		/*                            逻辑相关                                  */
		/************************************************************************/
		void										load_role_logic(const character_s2s_sl_logic_data& msg);
		bool                                        load_role_logic_by_db_lua(const char *data_ptr, int32 data_len);
		void										save_role_logic(int32 save_type_ex);
		int32										get_logic_data(e_role_logic_info logic_type);
		void										set_logic_data(e_role_logic_info logic_type, int32 logic_num);
		s_logic_info&								get_logic_info_inst() { return m_role_logic; }
		void										send_logic_all();
		void										send_logic_one(e_role_logic_info logic_type);
		int32										get_max_level_by_func(int32 func_type);
	

	public:
		/************************************************************************/
		/*                            财产相关                                  */
		/************************************************************************/
		inline bool									isnt_money_type(e_money_type money_type) { if (money_type >= e_money_type_max) { return true; }return false; }
		void										set_money_all(const s_money_info& money_info);
		void										set_money_data(e_money_type money_type, int64 money_value);
		int64										get_money_data(e_money_type money_type);//当前经济点数
		int32										get_money_data_32(e_money_type money_type);//当前经济点数
		s_money_info&								get_money_data_inst() { return m_money_info; };//当前经济点数
		int64										get_all_money_data(e_money_type money_type);//累计经济点数
		bool										can_cut_money(e_money_type money_type, int64 money_value);
		bool										can_cut_money_by_template_two_tuple_arr(const std::vector<int32>& money_arr, int32 extra_times = 1);
		void										cut_money(e_money_type money_type, int64 money_value, e_server_log_cut_money cut_type, int32 param = -1, int32 param2 = 0);
		bool										can_add_money(e_money_type money_type, int64 money_value);
		bool										add_money(e_money_type money_type, int64 money_value, e_server_log_add_money add_type, int32 param = -1, bool is_need_show_toptip = true);
		void										send_money_all();
		void										send_money_one(e_money_type money_type);
		void										load_money(const s_money_info& money_info);
		bool                                        load_money_by_db_lua(const char *data_ptr, int32 data_len);
		void										save_money(int32 save_type_ex);
		void										worship_by_type(int32 worship_type, bool is_cost_diamond);

		bool										add_money_by_template_tuple(const std::vector<int32>& money_two_tuple_arr, bool pop_msg = true, float fac = 1.0); // 通过传入钱物二元组来给玩家加上相应的钱数

		bool										add_item_by_template_tuple(e_server_log_add_item add_type, int32 param, const std::vector<int32>& item_two_tuple_arr, int32 is_locked = 0, bool pop_msg = true, float fac = 1.0, bool check_career = false, bool auto_to_money = false); // 通过传入物品二元组来给玩家加上相应的物品

		bool										add_item_by_template_tuple(e_server_log_add_item add_type, int32 param, const std::vector<int32>& item_id_arr, const std::vector<int32>& item_num_arr, int32 is_locked = 0, bool pop_msg = true, float fac = 1.0, bool check_career = false, bool auto_to_money = false);

		bool										add_item_by_drop_items_list(const std::vector<s_item_template_info>& drop_item_list, bool pop_msg = true, float fac = 1.0, bool check_career = false, bool auto_to_money = false);

		void										send_add_money_msg(int32 money_id, int32 money_num);
		void										send_add_item_msg(int32 item_id, int32 item_num, bool send_by_mail = false);
		void										send_promp_msg_to_client(const std::vector<s_item_template_info>& items_array, const std::vector<s_item_template_info>& money_array = {});		//发送获得的物品弹框

		int64										get_cur_level_max_talent_num();
		int64										get_cur_level_max_attribute_talent_num();

	public:
		/************************************************************************/
		/*                            经验相关                                  */
		/************************************************************************/
		bool										add_money_or_exp(e_money_type money_type, int64 money_value, e_server_log_add_money add_type, int32 param = 0);
		bool										lua_add_money_or_exp(e_money_type money_type, xstring money_value, e_server_log_add_money add_type, int32 param = 0);
		int64										add_exp(int64 exp_value, bool is_need_show = false, bool gm = false, bool grade_up = false, float extra_fix = 0.0f, bool is_add_by_kill_npc = false);
		xstring										lua_add_exp(xstring str_exp_value, bool is_need_show = false, bool gm = false, bool grade_up = false, float extra_fix = 0.0f, bool is_add_by_kill_npc = false);
		int64										get_level_up_exp();
		bool										set_level_up_exp();
		int32										calcu_level_up();
		void										add_exp_on_kill_npc(npc& dead_npc_ref);
		int64										add_exp_with_fix(int64 exp_val, float extra_fix = 0.0f);
		bool										can_add_exp_from_monster();
		bool										can_get_drop_from_monster();
		bool										is_can_levelup();
		player_grade_and_level						get_grade_num_and_level_num();
		int32										get_base_exp_of_dead_npc(npc& dead_npc_ref);
		int64										get_gain_exp_in_raid() { return m_get_gain_exp_in_raid; };
		void										clear_gain_exp_in_raid() { m_get_gain_exp_in_raid = 0; };
		int64										get_relic_exp() { return m_get_relic_exp; };
		void										clear_relic_exp() { m_get_relic_exp = 0; };
		void										set_relic_exp(int64 exp_value);
		void										req_relation_end(int32 relation_type, guid_64 target_guid);
		void										send_mail_with_level(int32 cur_level);

		void										set_cur_player_world_level();//获取对应该玩家的世界等级
		int32										get_cur_player_world_level_exp();
	private:
		int64										m_get_gain_exp_in_raid;
		int64										m_get_relic_exp;
		int32										m_cur_world_level;
	public:
		/************************************************************************/
		/*							  角色称号相关	   	          	        */
		/************************************************************************/
		special_name_mgr&							get_special_name_mgr() { return m_special_name_mgr; }
		ctitle_mgr&									get_title_mgr() { return m_title_mgr; }
		void										change_pk_king_title();

	public:
		/************************************************************************/
		/*							  冥想相关	   	          	        */
		/************************************************************************/
		meditation_manager&							get_meditation_mgr() { return m_meditation_mgr; }

	public:
		/************************************************************************/
		/*							  转换相关	   	          	        */
		/************************************************************************/
		convert_mgr&								get_convert_mgr() { return m_convert_mgr; }
	public:
		/************************************************************************/
		/*                            副本相关                                  */
		/************************************************************************/
		void										game_over();
		void										fuben_game_over(s_fuben_settlement& settlement, int32 fuben_template_id, bool finished, float award_fac);
		int32										give_assist_reward();
		void                                        lucky_draw(int32 fuben_template_id);
		int32										get_assist_chest();//获取今日已获得助战奖励次数
		int32										get_max_buy_count_with_map_id(int32 map_temp_id);// 获取当前地图ID可购买的最大次数
		int32										get_remain_buy_count_with_map_id(int32 map_temp_id);//获取当前地图ID剩余购买次数
		int32										get_buy_count_with_map_id(int32 map_temp_id);//获取当前地图ID已购买次数
		bool										buy_count_with_map_id(int32 map_temp_id, int32 buy_count = 1);//购买次数
		int32										get_use_things_add_map_count(int32 map_temp_id);//获取当前地图通过使用物品增加的次数
		bool										add_map_count_with_things(std::vector<int32> map_temp_id, int32 add_count);//增加对应(组)地图次数
		bool										add_map_time_with_things(std::vector<int32> map_time_arr, int32 item_num);//增加对应地图的剩余时间

		void										fuben_vip_mopping(int32 fuben_template_id, int32 settlement);

																												  /************************************************************************/
																												  /*                            友好度相关                                 */
																												  /************************************************************************/
		void										send_gift_add_friend_values(guid_64& addreessee_guid, int32 gift_id, int32 gift_num, std::string chat_message);
		void										append_gift_mail_system_string(std::string& title, std::string& context, std::string sender_name, int32 gift_id, int32 gift_count, int32 add_friendliness, const std::string& chat_info, int32 incidental_gift_id);
		/************************************************************************/
		/*                            世界boss首杀领奖                          */
		/************************************************************************/
		void										get_first_kill_world_boss_prize_func(int32 first_kill_welfare_template_id);
	public:
		/************************************************************************/
		/*                            军团相关                                  */
		/************************************************************************/
		const char*									get_player_legion_name() { return get_legion_info().legion_name; }
		int32										get_player_legion_level() { return get_legion_info().legion_level; }
		int32                                       get_player_legion_level_3() { return get_legion_info().legion_construction_level[ELegionInfo_construction_level_3]; }
		e_legion_job_title&							get_player_legion_job_title() { return get_legion_info().job_title; }
		void										send_chief_dead_notice();//发送军团长或副军团长被打死公告

																			 /************************************************************************/
																			 /*                            组队相关                                  */
																			 /************************************************************************/
		void										update_team_member_pos_info_tick();	// 刷新队员位置信息

		int32										get_player_team_member_num() { return get_team_info().member_num; }
		bool										is_player_team_captain() { return get_team_info().captain_guid == get_unit_guid(); }
		bool										is_player_in_team() { return get_team_info().team_guid.is_valid(); }
		bool										is_player_in_the_team(guid_64 team_guid); // 角色如果在指定队伍中就返回true
		bool										is_player_in_the_same_team(player *m_player);
		int32										get_team_map_id() { return get_team_info().team_map_id; }

		bool										is_get_kill_legion_boss_award_today();
		void										mark_kill_legion_boss_get_award_today();//设置今日已击杀过军团boss
		void										refresh_kill_legion_boss_get_award_today();

		/************************************************************************/
		/*                          功能解锁相关                                */
		/************************************************************************/
		void										get_func_unlock_award(int32 func_unlock_id);
		/************************************************************************/
		/*                          战斗相关									*/
		/************************************************************************/
	public:
		virtual int32					get_random_num(int32 random_index);
		virtual void					set_random_num(const int64& new_time);
	private:
		int32 m_random_array[random_seed_num];
		int64 m_random_array_time;
		//////////////////////////////////////////////////////////////////////////
		// 玩家关系相关 end
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// 拍卖行
		//////////////////////////////////////////////////////////////////////////
	public:
		void				set_last_auction_time(int64 time);
		int64				get_last_auction_time();
	private:
		int64				m_last_auction_time;
	public:
		/************************************************************************/
		/*                            响应客户端消息                            */
		/************************************************************************/
		void change_hp_and_del_fuhuo_buff(float hp_percent = 1);
		void set_player_relive(float hp_percent = 1, bool summon_pet = true);
		void back_to_life(e_revive_type revive_mode, bool is_free = false);
		void back_to_life_with_map_end();
		void on_player_revive_suc(e_revive_type revive_mode, int64 dead_duration);
	public:
		void get_fuhuopos(s_map_pos& map_pos, int32 map_template_id, s_map_pos& cur_pos);
		fuben_data_set& get_fuben_data_set() { return m_fuben_data_set; };
		bool can_entry_fuben(int32 fuben_template_id, bool notice_to_client = false, bool is_cross_to_gate_server = false);
		bool is_fuben_clearance(int32 fuben_id);
		void send_notice(const std::string& notice_str);
		void send_notice();
		void send_addition_notice_with_type(e_addition_buff addition_buff_type, int32 addtion_id);
		void send_fuben_info_to_client(s_fuben_info_to_client& info);
		void gm_order(const int32& command_name, const int32& sub_command, std::vector<int32> arg_int, std::vector<std::string> arg_string);
		void on_req_pick_drop_box(const s_unit_identifier& unit_identifier);
		int32 get_map_drop_box_notice_id(const int32& map_type, const int32& item_type, const int32& item_color, const int32& map_difficult);
		void send_get_pick_special_drop_box_notice(const std::vector<s_item_info>&  item_data);
		void send_grade_up_response(int32 result_code, int32 old_equip_buff_id, int32 new_equip_buff_id, int32 skill_template_id = 0);
		void send_character_error(int32 error);
		bool is_grow_up_fund_purchased(int32 fund_type);
		bool is_grow_up_fund_geted(int32 fund_type, int32 index);
		bool is_grow_up_fund_enable(int32 fund_template_id);
		void aoi_change_show_player(int32 show_num);
		void grade_up_level_ani_end(int32 old_buff_id, int32 new_buff_id);
		int32 get_login_type() { return m_login_type; };
	private:
		void										update_character_attribute();											// 同步角色属性
	public:
		s_client_uid&								get_client_uid() { return m_client_uid; }
		uint32										get_off_line_time(void);
	public:
		/************************************************************************/
		/*							 场景管理工具  		                        */
		/************************************************************************/
		void										transfer_by_template(int32 map_template_id, int32 line_id, int32 group_id, const guid_64& map_guid, int32 war_index);
		void										transfer_by_template_pos(int32 map_template_id, int32 line_id, s_map_pos map_pos, guid_64 map_guid);
		void										transfer_local_pos(const s_map_pos& map_pos, bool send_client = true);
		void										transfer_born();
		teleport_control&							get_transfer_control() { return m_teleport_control; }
		bool										is_during_transfer() { return m_teleport_control.is_teleporting(); }
		bool										is_in_cross_pk_map();
		bool										is_in_copy_map();
		bool										is_can_transfer_new_map();
		bool										is_lock_map_mission(int32 map_id);
		void										get_pk_king_bron_pos(guid_64 map_guid, s_map_pos& temp_pos);

		void										reset_client_camera();
	private:
		void 										save_role_base_info(int32 save_type_ex);
	public:
		/************************************************************************/
		/*							 npc占位相关			   					*/
		/************************************************************************/
		int32										get_npc_position();
		void										set_npc_position(int32 npc_pos);

	public:
		/************************************************************************/
		/*							 角色转生相关			   					*/
		/************************************************************************/
		void										grade_god_hood(int32 hood_index, int32 skill_template_id);
		int32										get_grade_num();
		void										check_can_done_grade_mission(const int32& mission_template_id);
		void										send_grade_up_notice(int32 new_player_template_id);
		void										add_att_by_load();
	public:
		/************************************************************************/
		/*							 角色转职相关			   					*/
		/************************************************************************/
		bool										change_role_class(int32 class_change);
	public:
		/************************************************************************/
		/*							  引导相关						       	    */
		/************************************************************************/
		guide_mgr&									get_guide_mgr() { return m_guide_mgr; }
		func_unlock_mgr&							get_func_unlock_mgr() { return m_func_unlock_mgr; }

		first_time_do_mgr&							get_first_time_do_mgr() { return m_first_time_do_mgr; }
	public:
		/************************************************************************/
		/*							  角色属性改变相关	   	          	        */
		/************************************************************************/
		bool                                        add_att_permanent(int32 att_type, int32 add_val);
		bool										fuhuo_need_cut_hp() { return fuhuo_cut_hp; };
		void										set_fuhuo_cut_hp(bool temp) { fuhuo_cut_hp = temp; };
		virtual void								set_unit_gs_change();//属性变化后重新计算gs值
		void										get_fruit_eating_info(e_fruit_type fruit_type, int32& cur_eated, int32& can_eat_max); // 获得玩家吃过的水果的信息(两个参数都是用来填充返回值的)
		bool										is_arround_pos(s_map_pos target_pos, float distance);
		virtual bool								can_attack_other(int32 target_unit_idx = -1);

	public:
		/************************************************************************/
		/*							  角色商品购买相关	   	          	        */
		/************************************************************************/
		player_store_helper&						get_goods_helper_mgr() { return m_store_helper; }
		void										buy_goods(int32 goods_id, int32 goods_num, int32 store_id, bool is_auto_buy, int32 back_string = 0);

		void										set_auto_use_hp_pot(float hp_percent, bool is_auto_buy_pot);
		void										auto_use_hp_pot();
		bool										is_auto_by_hp_pot() { return m_is_auto_buy_pot; };
	private:
		bool										m_is_auto_buy_pot;
		float										m_hp_percent;

		/************************************************************************/
		/*							  竞技场相关       	   	          	        */
		/************************************************************************/
	public:
		player_arena_mgr&							get_arena_mgr() { return m_arena_mgr; }

		/************************************************************************/
		/*							  角色GM指令   	          	                */
		/************************************************************************/
		bool										gm_set_level_and_grade(int32 level);
		bool										gm_set_career(int32 career_id);
		bool										gm_set_vip_point(int32 vip_point);
		bool										gm_set_grade_level(int32 grade_level, int32 branch);

		/************************************************************************/
		/*							  NPC交互   	          	                */
		/************************************************************************/
		void										begin_interact_with_npc(const guid_64 npc_guid); // 开始于NPC进行交互
		void										interact_with_npc_tick(const int64 new_time);
		void										finish_interact_with_npc(npc* npc_ptr); // 完成交互并获取交互奖励

		void										set_interact_npc_info(const float interact_time, npc* npc_ptr); // 设置NPC交互信息 如计时等
		bool										is_interact_with_npc() { return m_interact_npc_info.is_interact; } // 如果当前正在和NPC进行交互就返回true
		void										stop_interact_with_npc(bool send_msg = true, bool is_move = false); // 停止与NPC的交互

		bool										is_gathering_crystal_dreamland_chest(npc* npc_ptr);
		int32										get_left_gather_crystal_dreamland_chest_count();
		//世界BOSS掉落宝箱采集
		bool										is_gathering_world_boss_chest(npc* npc_ptr);
		int32										get_left_gather_world_boss_chest();
		bool										is_gathering_boss_island_chest(npc* npc_ptr);//是否可以采集永恒岛
		bool										is_gathering_cross_pk_chest(npc* npc_ptr);//是否可以采集跨服竞技场宝箱
		bool										is_gathering_harry_chest(npc* npc_ptr);//是否可以采集跨服掠夺的物品

		bool										is_arround_npc(npc* npc_ptr);

		void										send_interact_with_npc_state(e_npc_interact_state interact_state, int32 npc_template_id, guid_64 npc_guid);

		void										req_hit_and_get_npc_proc(guid_64 npc_guid);

		bool										is_already_login() { return m_already_login; }
		void										set_is_already_login(bool is_already) { m_already_login = is_already; };

		void										record_server_crystal_log();

	public:
		/************************************************************************/
		/*							  权限相关   	          	                */
		/************************************************************************/
		void										set_role_right(e_role_right role_right_type, int32 type_value);
		int32										get_role_right(e_role_right role_right_type);

		void										check_is_robot(/*const xchar* cur_account*/);
		bool&										get_is_robot() { return m_is_robot; }
		bool										get_write_log();
	public:
		/************************************************************************/
		/*							 日志相关   	          	                */
		/************************************************************************/
		int64										m_battle_begin_time;
		int32										m_already_must_do_remain_data_ary[e_daily_must_do_typ_max];	// 记录当日已经找回的
		int32										m_yesterday_back_type;//记录找回类型
		int32										m_relive_times_in_map;
		int32										get_relive_times() { return m_relive_times_in_map; };

	private:
		cwelfare_mgr								m_welfare_mgr;
		player_store_helper							m_store_helper;
		special_name_mgr							m_special_name_mgr;
		ctitle_mgr									m_title_mgr;
		cinteraction_mgr							m_interaction_mgr;
		player_arena_mgr							m_arena_mgr;
		meditation_manager							m_meditation_mgr;
		cauction_mgr								m_auction_mgr;
	private:
		s_client_uid								m_client_uid;
		bool										m_is_cross_cs_create;
		ui64										m_heart_beat_time;
		int32										m_login_type;
		login_fixed_data							m_login_third_data;
		bool										m_is_in_ancient_battlefield;
		bool										m_is_in_belief_cloister_battlefield;
		bool										m_add_ancient_activity_degree;
		bool										m_already_login;
		int64										m_login_time;
		int64										m_dead_stamp;
		bool										m_is_in_exp_raid;
		bool										m_add_exp_raid_degree;
	public:
		bool                                        m_is_begin_exp_raid;                             //经验副本引导
		bool                                        m_is_need_add_exp_buff;							 //引导暂停时清除经验buff						

	private:
		/************************************************************************/
		/*							  npc占位相关			   					*/
		/************************************************************************/
		bool										m_npc_position[npc_position_max];
		/************************************************************************/
		/*							  加载、存储相关		   					*/
		/************************************************************************/
		bool										m_update_db_flag;								// 定时存储时是否正在存储中
		std::set<int32>								m_loading_flag;									// 加载标志位
		std::set<int32>								m_saving_flag[e_save_data_type_max];			// 存档标志位
		int64										m_time_save_db;									// 定时存档时间差
		uint32										m_time_begin[4];                                // 存储时间
		int64										m_leave_time;									// 断线查询的时间
		int32										m_leave_num;									// 断线查询的次数
		int32										m_cur_pound_num = 0;							// 本次登录分解出的元素粉末数量
																										/************************************************************************/
																										/*							  传送、跨服相关		   					*/
																										/************************************************************************/
		teleport_control							m_teleport_control;								// 传送功能对象
		int32                                       m_teleport_map_id;
		int32										m_teleport_line_id;
		s_map_pos                                   m_teleport_map_pos;
		int32										m_teleport_group_id;
		guid_64										m_teleport_map_guid;
		int32										m_teleport_war_index;
	public:

		bool										is_token_login() { return m_login_type == e_login_type_new_token; };
		void										change_big_player_title(int32 title_type);

		/************************************************************************/
		/*							  财产相关	   					            */
		/************************************************************************/
	public:
		std::shared_ptr<finger_guess_component>		m_finger_guess_component;
		std::shared_ptr<smashing_bottle_component>	m_smashing_bottle_component;
		std::shared_ptr<time_activity_component>	m_time_activity_component;
		std::shared_ptr<item_upgrade_component>		m_item_upgrade_component;
	private:
		s_money_info								m_money_info;                                   // 玩家的财产
		fuben_data_set                              m_fuben_data_set;
		int32										m_exp_buff_template_id;
		/************************************************************************/
		/*							  时间相关	   					            */
		/************************************************************************/
		s_time_info									m_role_time;
		std::shared_ptr<map_record_set>				m_map_record_mgr;
		/************************************************************************/
		/*							  逻辑相关	   					            */
		/************************************************************************/
		s_logic_info								m_role_logic;
		/************************************************************************/
		/*							  信仰相关	   					            */
		/************************************************************************/
		cbelief_mgr									m_belief_mgr;
		belief_rune_mgr								m_belief_rune_mgr;
		/************************************************************************/
		/*							  天赋相关	   					            */
		/************************************************************************/
		ctalent_mgr									m_talent_mgr;
		/************************************************************************/
		/*							  图鉴相关	   					            */
		/************************************************************************/
		cpokedex_mgr								m_pokedex_mgr;

		/************************************************************************/
		/*							  排行榜相关	   					        */
		/************************************************************************/

		cworship_target								m_worship_target;
		cranking_mgr								m_ranking_mgr;

		/************************************************************************/
		/*							  角色属性改变相关	   	          	        */
		/************************************************************************/

		bool										fuhuo_cut_hp;
		cmission_mgr								m_mission_mgr;
		cachievement_mgr							m_achievement_mgr;
		convert_mgr									m_convert_mgr;
		cservice_goal_mgr							m_service_goal_mgr;
		cseven_day_goal_mgr							m_seven_day_goal_mgr;
		feather_heart_mgr                           m_feather_heart_mgr;
		cphantom_mgr                                m_phantom_mgr;
		recycle_mgr                                 m_recycle_mgr;
		goddess_mgr									m_goddess_mgr;
		awaken_mgr                                  m_awakan_mgr;

		/************************************************************************/
		/*							  社交相关       	   	          	        */
		/************************************************************************/
		cmail_mgr									m_mail_mgr;
		cchat_mgr									m_chat_mgr;

		/************************************************************************/
		/*							  每日必做						       	    */
		/************************************************************************/
		s_unit_yesterday_must_do_remain             m_three_day_must_do_remain_ary[resource_find_back_day];

		/************************************************************************/
		/*							  NPC交互						       	    */
		/************************************************************************/
		interact_npc_info							m_interact_npc_info;

		/************************************************************************/
		/*							  首次做相关						        */
		/************************************************************************/
		first_time_do_mgr							m_first_time_do_mgr;

		/************************************************************************/
		/*							  引导相关						       	    */
		/************************************************************************/
		guide_mgr									m_guide_mgr;
		func_unlock_mgr								m_func_unlock_mgr;
		//func_unlock_mgr								m_func_unlock_mgr;

		/************************************************************************/
		/*							  每日刷新						       	    */
		/************************************************************************/
	public:
		bool										is_need_refresh_daily_info_at_zero_hour();
		bool										is_need_refresh_daily_info_at_zero_hour_no_cross();
		bool										is_need_refresh_week_info();
		bool										is_need_refresh_daily_info_at_zero_hour_cross();
		bool										is_need_refresh_daily_info_at_specify_hour();
		bool										is_need_refresh_gs_up_start_value_time();
		void										refresh_daily_info(bool bOnline);
		void										refresh_at_zero_hour(bool bOnline);
		void										refresh_at_zero_hour_no_cross(bool bOnline);
		void										refresh_at_zero_hour_cross(bool bOnline);
		void										refresh_at_weekly();
		void										refresh_at_specify_hour(bool bOnline);
		void										send_mail_daily_activity_no_receive_rewards();//如果有没有领取的活跃度奖励,就发邮件
		void										send_mail_daily_legion_no_receive_rewards();

		bool										is_need_clear_harry_info_at_harry_time();//是否需要清理玩家掠夺数据
		void										clear_harry_info_at_harry_time();//清理玩家掠夺数据
		/************************************************************************/
		/*							  每月刷新						       	    */
		/************************************************************************/
		bool										is_need_refresh_month_info();
		void										refresh_month_info();
		/************************************************************************/
		/*							  权限相关						       	    */
		/************************************************************************/
		s_role_right								m_role_right;
		bool										m_is_robot;
		/************************************************************************/
		/*							  守护神相关						        */
		/************************************************************************/
		cpatron_saint_mgr							m_patron_saint_mgr;
	public:
		int32										get_patron_saint_activated_num();
		int32										get_patron_saint_level_num();

	private:
		s_role_history_high_record_info				m_history_high_record;
	public:
		/************************************************************************/
		/*							 资源找回							        */
		/************************************************************************/
		void                                        refresh_three_day_must_do_remain_info(bool bOnline);
		void                                        update_three_day_arean_rank(int32 arena_rank);
		void                                        tidy_three_day_mustdo_remain_ary();
		void                                        update_three_day_exp();
		void										find_back_must_do_resource_process(int32 must_do_typ, bool is_diamond_find);
		void										find_back_serveral_resource_process(const int32& must_do_typ, bool is_diamond_find, const int32&  find_back_count);
		void										update_three_day_must_do_remain();
		void										updata_daily_must_do_count();
		DailyActivitiesTemplate*					get_find_back_template_ptr(int32 daily_activities_type, int32 exp_level);

		int32                                       get_find_back_template_id(int32 daily_activities_type, int32 exp_level);
		void                                        update_find_back_count(int32 must_do_type,int32 find_back_count);
		s_unit_yesterday_must_do_remain&            get_find_back_info();
		int32                                       get_find_count_by_type(int32 must_do_type);



		/************************************************************************/
		/*							  全民冲榜相关					       	    */
		/************************************************************************/
	private:
		service_rank_mgr							m_service_rank_mgr;
	public:
		service_rank_mgr&							get_service_rank_mgr() { return m_service_rank_mgr; }
		/************************************************************************/
		/*							    打宝相关 					       	    */
		/************************************************************************/
	private:
		gain_treasure_mgr							m_gain_treasure_mgr;
	public:
		gain_treasure_mgr&							get_gain_treasure_mgr() { return m_gain_treasure_mgr; }

		/************************************************************************/
		/*							    永恒岛					       	    */
		/************************************************************************/
	private:
		boss_island_mgr								m_boss_island_mgr;
	public:
		boss_island_mgr&							get_boss_island_mgr() { return m_boss_island_mgr; }

	private:
		server_harry_mgr							m_server_harry_mgr;
	public:
		server_harry_mgr&							get_server_harry_mgr() { return m_server_harry_mgr; }
		void										change_harry_buff();

	public:
		void find_other_player_info(guid_64 target_guid, int32 server_id);
		void find_other_player_info_base_end(s_unit_info other_player_info);
		void find_other_player_info_equiping_end(const s_item_info_db* other_player_info);
		void set_other_player_info_buff(const s_item_info_db* other_player_info);	//通过装备end获取部分buff信息
		void find_other_player_info_special_name_end(const s_special_name_info_db other_player_info);
		void find_other_player_info_base_group_end(int32 group_job, std::string group_name, guid_64 target_guid);
		void find_other_player_info_spirit_end(const s_item_info_db other_player_info);
		void find_other_player_info_wing_or_mount_end(const s_item_info_db* other_player_info_array, bool is_wing);
		void find_other_player_info_buff_end(guid_64 target_guid, const int32* buff_info, const int32& data_num);
		void find_other_player_info_feather_end(const s_item_info_db* feather_info);

		void reconnect_send_red_package_info();
		void send_red_package(int32 template_id, xstring send_text, guid_64 item_guid);
		void send_red_package_end_proc(int32 template_id, int32 send_result);
		void get_red_package(guid_64 red_package_guid);
		void get_red_pack_end(guid_64 red_package_guid, int32 get_result, int32 money_type, int32 money_num);

		void transfer_buff(int32 operate_type);
		void send_role_info_to_gm();
		/************************************************************************/
		/*								天启试炼					       	    */
		/************************************************************************/

		private:
			oracle_trial_mgr								m_oracle_trial_mgr;
		public:
			oracle_trial_mgr&								get_oracle_trial_mgr() { return m_oracle_trial_mgr; }

		void send_stat_num_award(int32 drop_id);
		void send_oracle_trial_fuben_over_award(int32 map_id, int32 drop_id, int32 rank_num);
		void set_oracle_trial_info(int32 tier_num, int32 customs_num, int32 customs_state);
		void lua_oracle_trial_map_game_over(int32 map_template_id, int32 customs_state);

		/************************************************************************/
		/*                           aoi相关									*/
		/************************************************************************/
	public:
		virtual bool							aoi_watch_all_data_in(int32 unit_array_index);
		virtual void							aoi_watch_all_data_out(int32 unit_array_index);
		virtual bool							aoi_is_watch_all_data(int32 unit_array_index);
	private:
		player_aoi_watch						m_player_aoi_watch;
		int64									m_half_sec_tick;// 半s一次
		int64									m_sec_tick;	//	慢tick 1s 一次
		int64									m_min_tick;	//	慢tick 1分钟 一次
		int64									m_min_10_tick;	//	慢tick 10分钟 一次
		int64									m_old_tick_time;
		bool									m_is_waiting_auto_revive;
		int64									m_2sec_tick;//2S一次
		int64									m_hour_1_tick;//慢tick 1小时 一次

		/************************************************************************/
		/*                           充值相关                                   */
		/************************************************************************/

	public:
		void									recharge_by_template_id(int32 goods_id, const xstring& order_id = "", int64 order_num = 0, int32 payment_type = e_payment_method_test, float pay_price = 0, int32 recharge_time_count = 1);
		void									set_recharget_widget_type(bool is_dead_recharge);
		void									dead_widget_recharge_auto_get_welfare();
		void									do_add_vip(const std::vector<int32>& money_array, int32 vip_point, int32 goods_id, std::string order_id_str, float pay_price, int32 fixed_payment_type = -1);
		void									calculate_vip_level();
		void									vip_buff_change(int32 vip_level, bool is_add);
		void									refresh_vip_title();
		bool									is_recharge_by_type(e_recharge_type recharge_type);
		int32									get_month_card_current_activity_time();
		int32									get_month_card_remain_time();
		int32									get_exclusive_card_current_activity_time();
		int32									get_vip_extra_raid_enter(int32 map_template_id);//该函数返回的是可进入的额外剩余次数
		int32									get_vip_level(bool is_check_experience_level = true);
		void                                    buy_message_tip(int32 type, int32 level = 0);
		bool                                    time_is_same_day(int64 in_time_stamp);

	private:
		bool									m_is_dead_recharge = false;
		/************************************************************************/
		/*							  同步到WS						       	    */
		/************************************************************************/
	public:
		void									sync_data_to_ws(e_sync_cs2ws_data_type type, int64 data, int32 sub_data = -1);
		void									check_sync_gs_to_ws();
		void									sync_data_to_ws_lua(e_sync_cs2ws_data_type type, int32 data, int32 sub_data = -1);
	private:
		int64									m_gs_last_sync_ws;

		/************************************************************************/
		/*                         角色属性接口                                 */
		/************************************************************************/
	public:
		int64									get_role_gs();
		void									refresh_service_goal(const e_service_goal_type service_goal_type);
		s_fake_player_info						get_play_fake_player_info();
		/************************************************************************/
		/*							  是否接受机器人聊天					    */
		/************************************************************************/
	public:
		int32 get_is_receive_robot_chat() { return m_is_receive_robot_chat; }
	private:
		int32									m_is_receive_robot_chat;
	public:
		/************************************************************************/
		/*							  宝藏							       	    */
		/************************************************************************/
		ctreasure_mgr	m_treasure_mgr;
		ctreasure_mgr& get_treasure_mgr() { return m_treasure_mgr; }
		
		void treasure_prize(int32 treasure_type, int32 prize_type, int32 cost_money_type, int32 cost_money_value, int32 prize_score, int32 prize_luck, int32 item_id, int32 item_num, int32 prize_list[treasure_prize_num]);


		/************************************************************************/
		/*							  限时活动相关	   					            */
		/************************************************************************/
	private:
		time_limit_activity_mgr					m_time_limit_activity_mgr;
	public:
		time_limit_activity_mgr&				get_time_limit_activity_mgr() { return m_time_limit_activity_mgr; }
		/***********************************************************************/
		/*								角色改名									*/
		/***********************************************************************/
	public:
		void									change_name(xstring role_name);
		void 									sub_rename_card(uint32 rename_card_id);
		/***********************************************************************/
		/*								军团改名									*/
		/***********************************************************************/
		void									sub_re_legion_name(uint32 re_legion_name_card_id);

		/***********************************************************************/
		/*								被动技能							   */
		/***********************************************************************/
	private:
		passive_skill							m_passive_skill;
	public:
		passive_skill&							get_passive_skill() { return m_passive_skill; }
	public:
		/***********************************************************************/
		/*								军团技能							   */
		/***********************************************************************/
	private:
		legion_skill							    m_legion_skill;
	public:
		legion_skill&							get_legion_skill() { return m_legion_skill; }
	public:
		/***********************************************************************/
		/*								击杀提示							   */
		/***********************************************************************/
	private:
		int64									m_continuity_kill_end_time_stamp;
		int32									m_kill_num_now;
	public:
		void									send_kill_prompt(int32 kill_player_array_index, bool is_end = false);

		/***********************************************************************/
		/*								击杀记录							   */
		/***********************************************************************/
	private:
		guid_64									m_last_kill_player_guid;//上一个击杀的玩家guid
		int64									m_last_kill_player_time;
		guid_64									m_last_kill_me_player_guid;//上一个杀自己的玩家guid
		int64									m_last_kill_me_player_time;

		std::vector<int32>						m_first_npc_kill_id_arr;
	public:
		void									set_last_kill_player_guid(guid_64 player_guid);
		guid_64									get_last_kill_player_guid() { return m_last_kill_player_guid; };
		int64									get_last_kill_player_time() { return m_last_kill_player_time; };
		void									set_last_kill_me_player_guid(guid_64 player_guid);
		guid_64									get_last_kill_me_player_guid() { return m_last_kill_me_player_guid; };
		int64									get_last_kill_me_player_time() { return m_last_kill_me_player_time; };

		void									set_first_npc_kill_id_arr();
		void									insert_first_npc_kill_id_str(int32 first_npc_id);
		bool									get_is_have_cur_first_kill_npc_id(int32 npc_id);

	public:
		void									req_cross_ladder_last_score();
		void									receive_cross_ladder_last_score(int32 last_score);
		void									receive_cross_ladder_last_ranking(int32 last_ranking);
		void									delete_cross_ladder_title();
		int32									get_cross_ladder_can_buy_ticket_num() { return m_cross_ladder_can_buy_ticket_num; }
		void									set_cross_ladder_can_buy_ticket_num(int32 num) { m_cross_ladder_can_buy_ticket_num = num; }
	private:
		int32									m_cross_ladder_can_buy_ticket_num;	//跨服天梯每日可购买门票数量（临时数据，只用做判断）

	public:
		void									req_element_ladder_last_score();
		void									receive_element_ladder_last_score(int32 last_score);
		void									receive_element_ladder_last_ranking(int32 last_ranking);
		void									delete_element_ladder_title();
		void									send_gm_add_ticket();

	/***********************************************************************/
	/*								宠物相关							   */
	/***********************************************************************/
	public:
		void									summon_all_pet();
		void									kill_all_pet();
		npc*									summon_pet(int32 template_id);
		void									kill_pet_by_type(e_summon_pet_type type);
		npc*									get_pet_ptr_by_type(e_summon_pet_type type);
		void									change_summon_pet_movement();
		s_map_pos								get_back_pos(bool isLeft = true);
	private:
		int64									m_summon_tick;
		int32									m_summon_pet_array[e_summon_pet_type_max];
	/***********************************************************************/
	/*								提示相关							   */
	/***********************************************************************/
	public:
		void									set_notice_id(int32 notice_id) { m_notice_id = notice_id; }
	private :
		int32									m_notice_id;

	//***********************************************************************/
	/*								个人信息								*/
	//***********************************************************************/

	public:
		person_information_mgr&			get_person_infor_mgr() { return m_person_infor_mgr; }
	private:
		person_information_mgr			m_person_infor_mgr;

	//***********************************************************************/
	/*								赛季奖励信息								*/
	//***********************************************************************/

	public:
		role_competition_mgr&			get_competition_mgr() { return m_role_competition_mgr; }
	private:
		role_competition_mgr			m_role_competition_mgr;

	//***********************************************************************/
	/*							天梯赛季奖励信息								*/
	//***********************************************************************/

	public:
		element_competition_mgr&		get_element_competition_mgr() { return m_element_competition_mgr; }
	private:
		element_competition_mgr			m_element_competition_mgr;

	//***********************************************************************/
	/*								军团共舞信息							*/
	//***********************************************************************/

	public:
		legion_dance_mgr&					get_legion_dance_mgr() { return m_legion_dance_mgr; }
		void								add_legion_dance_daily_must();
	private:		
		legion_dance_mgr					m_legion_dance_mgr;

	//***********************************************************************/
	/*							助战信息							*/
	//***********************************************************************/
	private:
		assist_fight_cs_mgr					m_assist_fight_mgr;
	public: 
		assist_fight_cs_mgr&					get_assist_fight_mgr() { return m_assist_fight_mgr; }
		void  transfer_to_assist_fight(int32 map_template_id, guid_64 map_guid, int32 group_id, s_map_pos map_pos);

		bool						check_can_get_helper_reward();
		int32						get_extra_helper_reward_times();
		bool						check_can_get_be_helped_reward();
		int32						delete_get_helper_reward_times(int32 point = 0);
		int32						delete_get_be_helped_reward_times(int32 point = 0);
		void						send_attacker_info(unit& unit_ref);
		void						send_attacker_info_by_robot_player(npc& unit_ref);
		int32                       get_help_value(int32 point);
		bool						check_can_receive_summon_damage();

	private:
		int32						m_attacker_index;
		int32						m_send_attacker_info_cd;
	//***********************************************************************/
	/*							消息相关							*/
	//***********************************************************************/
	private:
		packet_c2s_s2c				m_msg;

	//***********************************************************************/
	/*							解耦掉落和伤害列表							*/
	//***********************************************************************/
	public:
		bool check_can_get_boss_drop(npc* npc_ptr);
		void delete_boss_challenge_times_after_drop();
		void refresh_boss_challenge_times();
		void set_boss_challenge_restore_times();
		void check_boss_challenge_restore_times(int32 new_time_second);
		bool check_can_enter_gain_treasure_map(e_map_type type);

	//***********************************************************************/
	/*							lua接口封装									*/
	//***********************************************************************/
	public:
		bool add_money_or_exp_with_string(e_money_type money_type, std::string money_value, e_server_log_add_money add_type, int32 param = 0);
		void send_upgrade_wing_notify_with_item(int32 NoticeId, int32 WingNameId, int32 UpgradeNow, citem* item_ptr);
		void skill_cast_by_skill_id(int32 skill_id);

	public:
		void get_log_common_head_info(s_log_common_head &log_head);
		s_log_common_head &get_share_log_head();


	/************************************************************************/
	// 怪物追击时自身追击点
	/************************************************************************/
	public:
		fvector get_position_by_chase(f32 skill_distance, fvector npc_position);

		void set_role_account(xstring temp_account);

	public:
		void show_vip(int32 is_show_vip);
		void enchant_show_type(int32 show_type);
	//***********************************************************************/
	/*							萌龙奇遇							*/
	//***********************************************************************/
	private:
		dragontrip_mgr				m_dragontrip_mgr;
	public:
		dragontrip_mgr&				get_dragontrip_mgr() { return m_dragontrip_mgr; }
	//***********************************************************************/
	/*							坐骑赋能							*/
	//***********************************************************************/
	public:
		mount_power_mgr             m_mount_power_mgr;
	public:
		mount_power_mgr&            get_mount_power_mgr() { return m_mount_power_mgr; }
	//***********************************************************************/
	/*							宝石纹刻							*/
	//***********************************************************************/
	public:
		jewel_carve_mgr             m_jewel_carve_mgr;
	public:
		jewel_carve_mgr&            get_jewel_carve_mgr() { return m_jewel_carve_mgr; }
	//***********************************************************************/
	/*							天空宝库							*/
	//***********************************************************************/
	private:
		skytreasure_mgr             m_skytreasure_mgr;
	public:
		skytreasure_mgr&            get_skytreasure_mgr() { return m_skytreasure_mgr; }
	//***********************************************************************/
	/*							星海方舟							*/
	//***********************************************************************/
	private:
		starark_mgr                m_starark_mgr;
	public:
		starark_mgr&               get_starark_mgr() { return m_starark_mgr; }
	//***********************************************************************/
	/*							 福牌										*/
	//***********************************************************************/
	private:
		lucky_card_mgr                m_lucky_card_mgr;
	public:
		lucky_card_mgr&               get_lucky_card_mgr() { return m_lucky_card_mgr; }
	//***********************************************************************/
	/*							星界异旅										*/
	//***********************************************************************/
	private:
		star_trip_mgr                m_star_trip_mgr;
	public:
		star_trip_mgr&               get_star_trip_mgr() { return m_star_trip_mgr; }
	//***********************************************************************/
	/*							时光回馈									*/
	//***********************************************************************/
	private:
		time_feed_back_mgr             m_time_feed_back_mgr;
	public:
		time_feed_back_mgr&            get_time_feed_back_mgr() { return m_time_feed_back_mgr; }

	//***********************************************************************/
	/*							限时礼包									*/
	//***********************************************************************/
	private:
		time_limit_gift_mgr             m_time_limit_gift_mgr;
	public:
		time_limit_gift_mgr&            get_time_limit_gift_mgr() { return m_time_limit_gift_mgr; }

	//***********************************************************************/
	/*							活动预约									*/
	//***********************************************************************/
	private:
		std::vector<s_subscribe_daily_info>		m_subscribe_daily_list;
	public:	
		void			subscribe_daily(int32 must_do_type);		// 活动预约
		void			check_subscribe_daily();					// 活动参与检测
		void			load_subscribe_daily_info_end(const s_subscribe_daily_db_info * dp_info, int32 data_num);	// 加载活动预约数据
		void			save_subscribe_daily_info(e_save_role_data_type eType);									// 保存活动预约数据
		void			send_subscribe_daily_info();
		void			send_subscribe_daily_end(int32 end_type);
	public: 
		void create_obj_lua();
		void del_obj_lua();
		void save_obj_lua(int32 eType);


	};
}
#endif


