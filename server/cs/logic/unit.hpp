#pragma once
#include "pawn_att.hpp"
#include "Logic/item_def.hpp"
#include "war_state.h"
#include "base/ecs_world.h"
#include "logic/item_manager.h"
#include "unit_move_mgr.h"
#include "pk_community_mgr.h"
#include "legion_cs_mgr.h"
#include "team_cs_mgr.h"

namespace hld
{
	class aoi_proto_unit_aoi_all;
	struct s_map_block;
	struct item_manager_component;

	// 所有场景逻辑对象的基类
	class unit
	{
	protected:
		unit();
		virtual ~unit();
	private://内存已经创建好了 禁止拷贝
		unit(const unit& unit_ref);
		unit& operator=(const unit&);
	public:
		//角色的数据状态
		void							set_data_use(bool is_use) { m_is_data_use = is_use; }
		const bool&						is_valid() const { return m_is_data_use; }
		void							set_array_index(int32 array_index);
		const int32&					get_array_index() const { return m_array_index; }
		const s_unit_identifier&		get_identifier() const { return m_unit_identifier; }
	public:
		void							set_session_state(e_session_status eStatus) { m_status = eStatus; }
		e_session_status				get_session_state() { return m_status; }
	public:
		virtual void					heart_tick(const int64& new_time, const int32& tick_time);
		virtual void					clear_data();
		/************************************************************************/
		/*                          非战斗属性相关                              */
		/************************************************************************/
		virtual void					add_unit(const guid_64& unit_guid) {}
		virtual void					remove_unit(const guid_64& unit_guid) {}
		virtual void					update_unit(const guid_64& unit_guid) {}
		void							set_unit_info_all(const s_unit_info& tempInfo);
		int32&							get_unit_info(e_role_info eType) { return m_role_info.data_ary[eType]; }
		int64							get_unit_gs_value() { return m_role_info.data_i64_ary[e_role_i64_info_gs_value]; }
		int64							get_unit_i64_info_data(e_role_i64_info eType);
		void							set_unit_i64_info_data(int32 idex, int64 gs_value);
		s_unit_info&					get_unit_info_inst() { return m_role_info; }
		void							set_unit_info(e_role_info eType, int32 iValue);
		xchar*							get_name() { return m_role_info.role_name; }
		void							set_name(xstring p_role_name);
		xchar*							get_appearance() { return m_role_info.role_appearance; }
		void							set_appearance(xstring role_apearance);
		xchar*							get_account() { return m_role_info.account; }
		virtual void					set_unit_guid(const guid_64& unit_guid) { m_role_info.role_guid = unit_guid; }
		const guid_64&					get_unit_guid() const { return m_role_info.role_guid; }
		const int32						get_unit_guid_A() const { return m_role_info.role_guid.A; }
		const int32						get_unit_guid_B() const { return m_role_info.role_guid.B; }
		const char*						get_unit_guid_str() const { return m_role_guid_str; }
		xstring							lua_get_unit_guid_str() { return m_role_info.role_guid.to_string().c_str(); }
		void							set_map_ent(Entity* map_ent) { m_map_ent = map_ent; }
		const guid_64					get_map_guid() const { return m_map_ent->getEntityId(); }
		Entity*							get_map_ent() const { return m_map_ent; }
		e_unit_type						get_unit_type() const { return m_unit_type; }
		int32							get_unit_sub_type() const { return m_unit_sub_type; }
		virtual e_unit_type				get_owner_unit_type() { return m_unit_type; }
		virtual int32					get_owner_unit_level() { return m_role_info.data_ary[e_role_info_exp_level]; }
		virtual bool					can_become_target() { return true; }
		virtual bool					can_attack_other(int32 target_unit_idx = -1) { return true; }
		void							set_line_id(int32 line_id);
		int32							get_line_id() { return m_line_id; }
		void							set_main_line_id(int32 line_id);
		int32							get_main_line_id();
		void							set_war_index(int32 war_index);
		int32							get_war_index() { return m_war_index; }
		int32&							get_must_do_count(e_daily_must_do_typ eType) { return m_daily_must_do_count.data_ary[eType];}
		void							set_must_do_count(e_daily_must_do_typ eType, int32 iValue);
		virtual bool					is_mission_see(int32 unit_array_index) { return true; }
		bool							is_self_server(int32 unit_index = 0);
		virtual bool					check_can_show_head(guid_64 unit_guid) { return true; }
		xstring							get_class_name();
		virtual int32                   is_control_boss() { return 0; }
	public:
		/************************************************************************/
		/*                            战斗属性相关                              */
		/************************************************************************/
		virtual void					set_unit_gs_change() {}//属性变化后重新计算gs值
		pawn_att&						get_pawn_att() { return m_pawn_att; }
		bool							change_hp(double hp_value, int32 array_index, bool be_critical, int32 skill_template_id = 0);
		void							full_hp();
		virtual void					be_attacked(const int32& attack_index, const double hp) {}
		void							send_suck_blood_value(int32 suck_value);
		float							get_left_hp_per();
		void							trigger_unit_event(int32 event_type, int64 param1 = 0, int64 param2 = 0, int64 param3 = 0);
		float							get_game_att(int32 att_index);


		/************************************************************************/
		/*							  角色背包BUFF	   					        */
		/************************************************************************/
		int32							m_addition_buff[e_addition_buff_max][hld::max_each_type_addition_buff_num];           // 背包界面的4个加持buff的当前生效的模板Buff ID
		std::vector<int32>				get_addition_buff_id_arr(e_addition_buff addition_buff_type);
		void                            add_addition_buff_id_arr(e_addition_buff addition_buff_type, int32 buff_id);
		void							reset_addition_buff_id_by_type(e_addition_buff addition_buff_type);
		void							send_addition_buff_info(guid_64 guid, e_addition_buff addition_buff_type, int32 addition_buff_id);
		void							send_addition_buff_info_arr(guid_64 guid, e_addition_buff addition_buff_type, std::vector<int32> addition_buff_id_arr);
		void							send_all_addition_buff_info(unit& temp_player);
		/************************************************************************/
		/*                          战斗属性相关                                */
		/************************************************************************/
		void							send_warning(e_warning_type warn_type, int32 warn_value, bool warn_show = true);
		virtual	void					be_dead(bool is_auto_put_into_bag = false) {}
		bool							is_dead();
		const s_map_block*				get_in_map_safe();
		virtual f32						get_current_weapon_attack_speed(bool apply = true) { return 1.0f; }									// 得到当前主手武器的攻击速度
		virtual f32						get_current_weapon_attack_distance() { return 260.0f; }									// 得到当前主手武器的攻击范围
		virtual bool					is_equip_shield() { return false; }									// 得到当前主手武器的熟练度（没有武器返回徒手熟练度）
		virtual int32					get_assistant_weapon_type(bool& is_weapon_sub) { is_weapon_sub = true; return e_weap_sub_type_none; }	// 取得副手武器类型 is_weapon_sub 返回true 函数返回值表示武器子类型 false 函数返回值表示防具类型
		virtual void					on_mount_ammo(bool v) { m_mount_ammo_flag = v; };
		bool							is_mount_ammo() const { return m_mount_ammo_flag; }
		int32							get_killer_skill_template_id() { return m_kill_skill_template_id; }
		int32							get_killer_array_index() { return m_kill_array_index; }
		void							set_killer_array_index(int32 killer_unit_index) { m_kill_array_index = killer_unit_index; }
		e_be_dead_reason				get_kill_dead_reason() { return m_kill_dead_reason; }
		void							set_kill_dead_reason(e_be_dead_reason dead_reason) { m_kill_dead_reason = dead_reason; }
		bool							get_be_critical() { return m_be_critical; }
		/************************************************************************/
		/*                           道具、装备									*/
		/************************************************************************/
		item_set&						get_item_set() { return item_manager::get_item_set(m_array_index); }
		virtual bool					valid_items_for_consume(int32 info_id, int32 number) { return true; }// true == 满足消耗条件，false == 不满足消耗条件
		virtual void					consume_items(int32 info_id, int32 number) {}				// true == 满足消耗条件，false == 不满足消耗条件
		std::shared_ptr<item_manager_component>				m_item_manager_component;
		/************************************************************************/
		/*                           移动同步、协议处理							*/
		/************************************************************************/
		virtual	void					set_pos_move(const s_map_pos& map_pos) {}
		void							set_new_map_pos(const s_map_pos& map_pos);
		void							set_new_map_angle(const int32& map_angel);
		void							set_move_pos(const s_map_pos& map_pos);
		void							set_main_pos(const s_map_pos& map_pos);
		s_map_pos						get_main_pos();
		const s_map_pos&				get_new_map_pos() { return m_new_map_pos; }
		void							set_old_map_pos(const s_map_pos& map_pos) { memcpy(&m_old_map_pos, &map_pos, sizeof(m_old_map_pos)); }
		void							set_old_map_yaw(const int32 yaw) { m_old_map_pos.unit_rotation.yaw = yaw; }
		const s_map_pos&				get_old_map_pos() const { return m_old_map_pos; }
		void							set_time_stamp(const int64& time_stamp) { m_time_stamp = time_stamp; }
		int64&							get_time_stamp() { return m_time_stamp; }
		virtual void					get_aoi_msg(hld::aoi_proto_unit_aoi_all& msg) {}
		virtual int32					send_message(const void* data, uint32 date_len) { return 0; }
		virtual int32					send_message(google::protobuf::Message* net_pro, uint32 header) { return 0; }
		virtual void					send_message_to_self(void* data, uint32 date_len) {}
		virtual void					send_message_to_self(google::protobuf::Message* net_pro, uint32 header) {}
		virtual void					send_message_to_aoi(google::protobuf::Message* net_pro, uint32 header, bool to_self = true, bool use_length = false) {}
		void							send_info_one(e_role_info info_index, bool is_to_aoi = false);
		void							send_info_all(bool is_refresh_by_zero = false, bool is_refresh_by_special_time = false);
		void							broadcast_info_one(e_role_info info_index);
		void							send_unit_begin_info();
		void							send_role_i64_value(int32 idex, bool is_to_aoi = false);

		void							send_daily_must_do_count_info_one(e_daily_must_do_typ info_index);
		void							send_daily_must_do_count_info_all();
		MapTemplate*					get_cur_map_template_ptr() { return m_cur_map_template_ptr; }
		int32                           get_cur_map_type() { return m_cur_map_template_ptr == nullptr ? -1 : m_cur_map_template_ptr->Type; }
		/************************************************************************/
		/*                           pk相关										*/
		/************************************************************************/
		pk_community_mgr&				get_pk_community_mgr() { return m_pk_community_mgr; }
		c_war_state&					get_war_state_mgr() { return m_war_state_mgr; }

		unit_move_mgr&					get_move_mgr() { return m_unit_move_mgr; }
		int32							get_unit_capsule_width() { return m_unit_capsule; }
		int32							get_unit_half_height() { return m_unit_half_height; }
		virtual int32					get_random_num(int32 random_index) { return -1; }
		/************************************************************************/
		/*                           aoi相关									*/
		/************************************************************************/
		virtual bool							aoi_watch_all_data_in(int32 unit_array_index) { return true; }
		virtual void							aoi_watch_all_data_out(int32 unit_array_index){}
		virtual bool							aoi_is_watch_all_data(int32 unit_array_index) { return true; }

		/************************************************************************/
		/*							  军团相关						       	    */
		/************************************************************************/
		legion_cs_mgr&					get_legion_cs_mgr() { return m_legion_cs_mgr; }
		s_player_legion_info&			get_legion_info() { return m_legion_cs_mgr.get_legion_info(); }
		bool							is_in_the_same_legion(unit& another_unit);
		void							set_legion_guid(guid_64 legion_guid) { get_legion_info().legion_guid = legion_guid; }
		virtual guid_64&				get_legion_guid();
		void							set_legion_name(const xstring& legion_name);
		
		/************************************************************************/
		/*							  组队相关						       	    */
		/************************************************************************/
		team_cs_mgr&					get_team_cs_mgr() { return m_team_cs_mgr; }
		s_player_team_info&				get_team_info() { return m_team_cs_mgr.get_unit_team_info(); }
		bool							is_in_same_team(unit& another_unit);
		virtual guid_64&				get_team_guid();
		void							set_team_guid(guid_64 team_guid) { get_team_info().team_guid = team_guid; }
		int32							get_last_attack_idex() { return m_last_attack_idex; };
	public:
		bool							m_is_data_use;		// 对象状态
		s_unit_identifier				m_unit_identifier;
		pawn_att						m_pawn_att;
		int32							m_unit_capsule;
		int32							m_unit_half_height;
		pk_community_mgr				m_pk_community_mgr;
		unit_move_mgr					m_unit_move_mgr;
		c_war_state						m_war_state_mgr;
		legion_cs_mgr					m_legion_cs_mgr;
		team_cs_mgr						m_team_cs_mgr;
		s_unit_info						m_role_info;
		xchar							m_role_guid_str[max_unit_guid_str_max];
		s_map_pos						m_new_map_pos;
		s_map_pos						m_old_map_pos;
		int64							m_time_stamp;
		Entity*							m_map_ent;
		std::vector<s_map_block>*		m_safe_point_vec;
		int32							m_safe_point_vec_len;
		int32							m_array_index;		// 在管理器中的位置信息
		e_unit_type						m_unit_type;		// unit的类型
		int32							m_unit_sub_type;		// 扩展的类型
		int32							m_line_id;
		int32							m_war_index;
		e_session_status				m_status;

		int32							m_kill_array_index;
		int32							m_kill_skill_template_id;
		bool							m_mount_ammo_flag; // test code
		bool							m_be_critical;
		e_be_dead_reason				m_kill_dead_reason;
		MapTemplate*					m_cur_map_template_ptr;

		guid_64							m_move_map_guid;

		int32							m_last_attack_idex;//上一次造成攻击的idex

		int32							m_be_kill_num;
	public:
		s_role_daily_must_do_count      m_daily_must_do_count;
	};
}
