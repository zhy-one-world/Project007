/********************************************************************
  created: 2015年8月28日13:56:28
  file base: item_set
  file ext: h
  author: zhy
  
  purpose: about role's item_set
*********************************************************************/
#ifndef _ITEM_SET_H_
#define _ITEM_SET_H_

#include "item.h"
#include "Logic/char_def.hpp"
#include "Logic/server_log_def.hpp"
#include "utility/globle_data.h"


namespace hld
{
	class unit;
	class player;
	class item_proto_item_use_end;
	class item_proto_item_save_load;
	struct PropBasicAttributeTemplate;
	enum e_error_code;
	enum e_bag_type;
	enum e_item_mail_type;

	class item_set
	{
	public:
		item_set();
		~item_set(void);
	private://内存已经创建好了 禁止拷贝
		item_set(const item_set& unit_ref);
		item_set& operator=(const item_set&);
	public:
		void heart_tick(const int64& new_time);
		void set_unit_ptr(int32 unit_array_index);
		void clear_data();
		void save_item_to_db(int32 save_type);
		void save_show_info_to_db(int32 save_type);
		void load_item_by_db(const item_proto_item_save_load& load_proto);
		bool load_item_by_db_lua(const char *data_ptr, int32 data_len);
		void load_char_show_info(const s_char_show_info* show_data);
		void send_item_all();
		void send_wing_item_all();
		void send_item_one(citem* item_ref, bool to_self = true, e_update_item_info_mode update_mode = e_update_item_info_mode_default);
		void send_item_one_self(citem* item_ref, e_update_item_info_mode update_mode = e_update_item_info_mode_default);
		void send_item_equip(unit& temp_unit);
		void send_item_tidy_guid_array(const std::unordered_map<int32, guid_64>& guid_arr);
		void send_item_sell_earn_info(int32 money_id, int32 money_num);
		void send_item_composit_response(int32 composit_template_id, std::vector<int32>& result_code);
		void init_skill_and_item_gs();//初始化物品和技能buff的战力，不放在loading链逻辑里是因为会有回调函数的返回时间造成影响

	public://物品属性管理,所有装备操作改变属性都按照拆装更替装备进行
		void set_equip_skill_buff_all();//初始化人物时全部武器防具技能和buff
		void set_equip_att_all();//初始化人物全部的武器防具坐骑属性
		void set_show_fashion_buff();

		void set_equip_att(citem* equip_ptr, bool is_add);//拆卸装备改变一个物品的buff,技能和属性
		void set_sky_equip_att(citem* equip_ptr, bool is_add);
		void set_skygod_equip_att(citem* equip_ptr, bool is_add);
		void set_supreme_equip_att(citem* equip_ptr, bool is_add);
		bool can_show_sky_shape(int32 suit_type);
		bool can_show_skygod_shape(int32 suit_type);
		bool can_show_supreme_shape(int32 suit_type);
		void set_equip_upgrade_att(const EquipTemplate* equip_template_ptr, const int32& upgrade_num, bool is_add);
		void set_equip_addon_att(const EquipTemplate* equip_template_ptr, const int32& addon_num, bool is_add);
		void set_equip_succinct_att(const ItemSuccinctTemplate* succinct_template_ptr, const citem& equip_ptr, bool is_add);
		void set_equip_excellent_att(const EquipTemplate* equip_template_ptr, const citem& equip_ptr, bool is_add);
		void set_equip_jewel_att(citem& equip_ptr, bool is_add);
		void set_equip_enchant_att(citem& equip_ptr, const int32& enchant_num, bool is_add);

		void set_goddess_equip_att(citem* equip_ptr, bool is_add);
		int32 get_goddess_equip_slot(citem* equip_ptr);

		void set_wedding_equip_att(citem* equip_ptr, bool is_add);
		citem* get_wedding_equip_item();
		void set_wedding_init_fashion_state(bool activate);

		void set_fashion_att(citem* fashion_ptr, bool is_add, int32 star_num);//拆卸时装改变一个物品的buff,技能和属性
		void set_wing_att(bool is_add);
		void all_wing_sort();
		void set_feather_att(const citem& feather_ptr, bool is_add);
		void set_showed_wing();
		citem* get_showed_wing();
		void set_showed_mount();
		citem* get_showed_mount();

		citem* get_illusion_range_mount_or_wing(citem* item_mount_or_wing);
		void set_mount_att(bool is_add);	
	public:
		bool item_operate(const std::vector<guid_64>& item_guid, int32 item_slot, int32 operation_type,int32 use_up_item, int32 first_use_lock, int32 item_num, int32 force_do);
		bool equip_on(const guid_64& item_guid, int32& item_slot, bool is_without_auto_inherit = false, bool is_against_buy_time = false);
		bool equip_off(const guid_64& item_guid, int32 item_slot = -1, bool is_auto_equip_off = false, bool is_change_equipment = false);//是否因穿装备而执行的脱下操作
		void reset_fashion_effect();
		void reset_weapon_fashion_effect();
		void reset_cloth_fashion_effect();
		void reset_foot_fashion_effect();
		bool is_can_add_fashion_buff(citem* item_fashion);
		//装备不符合穿戴需求时扣除该装备加成的一切属性
		void change_att_for_equip(const guid_64& item_guid,bool is_add);
		//判断装备是否失效
		bool is_can_add_att(const guid_64& item_guid);
		//判断物品是否在使用期间 未失效
		bool is_in_duration(const guid_64& item_guid);
		//判断装备是否有效 包含属性和时间判定
		bool is_equipment_effective(const guid_64& item_guid);
		//遍历所有的已装备装备，初始化是否失效
		//void init_all_equip_add_att();

		int32 get_equip_smallest_upgrade_level();
		int32 get_equip_smallest_addon_level();
		int32 get_equip_smallest_awaken_level();
		int32 get_equip_smallest_forge_level();
		int32 get_equip_smallest_enchant_level(bool is_jewelry = false);
		int32 get_equip_smallest_enchant_level_by_type(int32 show_type);

		int32 get_equipment_upgrade_buff_id(bool is_weapon);
		int32 get_equipment_addon_buff_id();
		int32 get_equipment_awaken_buff_id();
		int32 get_equipment_forge_buff_id();
		int32 get_equipment_enchant_buff_id();

		int32 refresh_enchant_buff_show_type();
		int32 get_enchant_max_level();

		int32 get_awaken_fetter_max_num();

		void equip_off_upgrade_show_buff();		//去除装备强化光效buff
		void equip_on_upgrade_show_buff();		//添加装备强化光效buff
		void equip_off_addon_show_buff();		//去除装备追加光效buff
		void equip_on_addon_show_buff();		//添加装备追加光效buff
		void equip_off_awaken_show_buff();		//去除装备觉醒光效buff
		void equip_on_awaken_show_buff();		//添加装备觉醒光效buff
		void equip_off_forge_show_buff();		//去除装备重铸光效buff
		void equip_on_forge_show_buff();		//添加装备重铸光效buff
		void equip_off_enchant_show_buff();		//去除装备附魔光效buff
		void equip_on_enchant_show_buff();		//添加装备附魔光效buff

		void equip_off_equip_show_buff();		//去除装备光效buff
		void equip_on_equip_show_buff();		//添加装备光效buff

		int32 get_equip_num_by_color(int32 target_color);
		bool activate_weapon(int32 item_slot);
		bool activate_weapon_first(int32 item_slot);

		bool check_equip_by_item_id(int32 item_id);

		int32 item_enchant(const guid_64& item_guid, int32 enchant_template_id, int32 first_use_lock);
		int32 item_unenchant(const guid_64& item_guid);

		int32 upgrade_protect_item_can_use(const ItemUpgradeTemplate* item_upgrade_ptr, int32 cur_upgrade_num, int32 protect_item_index);
		int32 set_jewel_on(const guid_64& target_guid, const int32& target_slot, const guid_64& jewel_guid);
		int32 set_jewel_off(const guid_64& target_guid, const int32& target_slot);
		int32 jewel_upgrade(const guid_64& target_guid, const int32& target_slot);

		int32 item_advance(const std::vector<guid_64>& item_guids, int32 use_up_item);			//装备进阶
		int32 item_inherit(const std::vector<guid_64>& item_guids, bool is_auto = false);//装备传承

		bool is_can_inherit(citem* strip_item_ptr, citem* inherited_item_ptr);//剥离装备至少强化、追加、洗练属性中，至少有一项大于被传承物品
		bool is_will_lose_point(citem* strip_item_ptr, citem* inherited_item_ptr);//剥离装备至少强化、追加、洗练属性中，至少有一项大于被传承物品的对应培养上限
		int64 get_inherit_money_num(citem* strip_item_ptr, int32 cost_type);

		int32 item_assembly(const guid_64& item_guid, int32 first_use_lock);//神装再造
		int32 item_assembled(const guid_64& item_guid, int32 first_use_lock);//神装合成
		int32 random_property_value(int32 up_chance, int32 max_des, int32 min_des, int32 min_ins, int32 max_ins);


		e_error_code  item_succinct_is_fight_add(const guid_64& item_guid);
		int32 item_succinct_get_effect_num(const guid_64& item_guid, int32 money_cost_type);
		int32 item_succinct_random_property(const std::vector<float> *p_vec_att_up_change, const std::vector<int32> *p_vec_change_range, int32 offset_index, int32 cur_property_value);
		int32 item_succinct_n_times(const guid_64& item_guid, int32 money_cost_type, int32 first_use_lock, int32 is_auto, int32 level = 1, int32 is_use_jewel = 0);
		int32 item_succinct_one_times(const guid_64& item_guid, int32 money_cost_type, int32 frist_use_lock, int32 is_auto, int32 level, int32 is_use_jewel);


		int32 item_save_succinct(const guid_64& item_guid);

		int32 rand_excellent_att_color(EquipTemplate* equip_template_ptr);
		int32 rand_spirit_excellent_att_color(SpiritTemplate* spirit_template_ptr);
		void refresh_succinct_buff(bool is_add);
		int32 get_succinct_weapon_buff();
		int32 get_succinct_equip_buff();
		e_error_code item_use_check(citem& item_ref, int32& use_num); // PS:use_num是返回型参数,传入的use_num可能会被修改
		bool item_use(const guid_64& item_guid, citem*& end_item, int32 item_num = 1, int32 force_use = 0, bool is_cotinue_use = false,int32 item_slot=0); // 使用物品
		int32 item_buy_and_use(int32 item_id, int32 item_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list);//购买并且使用
		int32 item_buy_and_use(int32 goods_id);
		bool use_beast_spirit(const guid_64& item_guid, int32 item_num = 1);
		bool item_one_key_use(const std::vector<guid_64>& item_guid_array);									// 一键使用
		bool item_sell(const guid_64& item_guid, int32 item_num);  // 道具出售
		void item_merge(e_bag_type merge_bag);
		void set_merge_array(s_merge_stat* merge_array, int32& merge_num, const guid_64& item_guid, const int32& item_count);
		s_money_tuple get_item_recovery_earn_money(int32 item_template_id, int32 excellent_att_num = 0);
		bool item_recovery(const std::vector<guid_64>& item_guid, bool is_sprite_use_diamond = false); // 物品回收
		int32 wing_upgrade(const guid_64& wing_guid, int32 use_up_item);//翅膀升级
		int32 wing_starupgrade(const guid_64& wing_guid, int32 use_up_item);//翅膀碎片升星
		int32 wing_zhuling(const guid_64& wing_guid, int32 use_up_item, int32 item_num);//翅膀铸灵
		int32 wing_lengque(const guid_64& wing_guid, int32 use_up_item);//翅膀冷却
		int32 wing_xiling(const guid_64& wing_guid, int32 use_up_item);//翅膀冷却
		int32 wing_illusionupgrade(const guid_64& wing_guid, int32 use_up_item);//翅膀碎片升星
		int32 wing_shape_upgrade(citem& ref_wing_item_shape, int32 use_up_item); // 翅膀外形升级
		int32 wing_shape_unlock(const guid_64& wing_guid, int32 item_slot);
		int32 wing_feather_unlock(const guid_64& wing_guid, int32 item_slot);
		int32 wing_shape_illusion_unlock(const guid_64& wing_guid, const int32 illusion_slot);
		WingTemplate* get_wing_template_ptr(citem& ref_wing_item_shape);
		void  sync_ranking_wing();		//刷新排行榜翅膀
		bool wing_upgrade_rate(int32 bless, WingTemplate* wing_template_ptr);//翅膀升阶概率判断
		int32 wing_add_soul(const guid_64& wing_guid);//翅膀注魂
		int32 wing_add_spirit(const guid_64& wing_guid);//翅膀注灵
		int32 feather_upgrade(const guid_64& feather_guid, int32 use_up_item); // 翎羽升阶
		void equip_all_feather_and_spirit_and_soul();//装备全部翎羽,注灵,注魂
		void equip_all_spirit();//装备注灵
		void equip_all_feather();//装备翎羽
		void equip_all_soul();//装备注魂
		bool judge_can_recovery(citem* item_ptr);

		int32 show_this_wing(citem* wing_ptr, int32 item_slot, bool is_mission_wing = false, bool is_need_send_notice = false);
		guid_64 get_item_by_type(e_item_type item_type, int32 sub_type = -1);
		citem* get_item_by_template_id(int32 target_template_id);
		citem* get_item_by_template_id(int32 item_template_id, e_bag_type bag_type);
		citem* get_mount_by_mount_template_id(int32 mount_template_id);
		void unlock_all_mount();
		void init_mount_skill_att();
		void unlock_all_wing();
		void init_wing_skill_att();
		int32 reset_wings_order();
		int32 calculate_total_wing_upgrade_num();
		int32 get_cur_wing_level();
		//int32 find_empty_mile_item_slot();
		int32 put_item_into_bag_from_another_bag(const guid_64& item_guid);
		int32 get_lingyu_total_grade();
		int32 star_skill_unlock(guid_64 choosed_guid, int32 choosed_index, bool use_item = true,bool is_star = true);
		int32 item_star_skill_unlock(guid_64 choosed_guid, int32 choosed_index, bool use_item = true, bool is_star = true);
		int32 wing_psychic_skill_unlock(int32 passive_skill_template_id, bool is_add); //羽翼通灵技能解锁
		int32 get_mark_activated_num();
		int32 get_double_att_element_num();

		void init_break_will_skill_att();					// 初始化技能与属性
		void refresh_break_will_skill_by_id(int32 tem_id);	// 根据主id刷新技能
		int32 get_break_will_level();

		void init_awaken_fetter_att();
	public:
		int32 get_item_num(e_bag_type temp_bag_type);
		int32 get_empty_slot_num(e_bag_type temp_bag_type);
		int32 get_empty_slot(e_bag_type temp_bag_type);
		bool is_empty_slot_enough(e_bag_type temp_bag_typ, int32 request_num);

		citem* get_item(const guid_64& item_guid); // 从背包中获得物品
		citem* get_item(const int32& item_guid_A, const int32& item_guid_B); // 从背包中获得物品
		citem* get_item(std::string item_guid_str);
		std::vector<citem*> get_item_array(e_bag_type bag_type);

		citem* get_equip_item_by_slot(e_role_equip_slot item_slot);
		citem* get_equip_sprite_by_slot(int32 equip_slot);
		citem* get_item_by_slot(int32 bag_type,int32 item_slot);

		citem* creat_spirit_stone_by_spirit_template_id(int32 spirit_templateid);
		void  put_item_into_protect_spirit_bag(const guid_64& item_guid);
		citem* get_item_in_bag_with_template_id(int32 item_template_id);
		bool	is_class_enable(citem* target_item);
		int32	get_current_highest_upgrade_on_character();
		int32	get_current_highest_addon_on_character();
		int32   get_highest_mount_level();

		e_item_succinct_buff_level get_succinct_equip_buff_level();


	public:
		int32 get_bind_material_num() { return m_bind_material_num; }
		int32 get_no_bind_material_num() { return m_no_bind_material_num; }
		void cost_item_by_id_with_lock_states(e_server_log_del_item causeid, int32 location,int32 item_template_id, int32 item_cost_num, int32 first_use_lock);//消耗某类物品，是否优先锁定
		citem* get_cur_level_wing();
		citem* get_wing_add_spirit();
		citem* get_wing_add_soul();
		std::vector<citem*> get_wing_feather();
		std::vector<citem*> get_all_shape_wing();
		std::vector<citem*> get_all_wing();

		void cost_star_map_spirit_item_states(e_server_log_del_item causeid, int32 location, int32 item_template_id, int32 item_cost_num);//消耗星图可升格精灵
		int32 count_star_map_spirit(int32 item_template_id);//星图可消耗物品数量
	private:
		bool can_put_items_into_bag(e_bag_type bag_type, const std::vector<s_item_template_info>& item_array);
		int32 del_item_instance(e_server_log_del_item causeid, int32 location, const guid_64& item_guid, int32 del_num = 0);
		int32 del_item_instance(e_server_log_del_item causeid, int32 location, citem* item_ptr, int32 del_num = 0);
		bool put_item_into_bag(citem* item_ptr, e_bag_type bag_type = e_bag_type_bag, bool merge = true, e_update_item_info_mode update_mode = e_update_item_info_mode_default, bool is_show_go_to_bag = false, bool is_continue_put = false); // 把物品放入背包中，只有返回>0时才表示执行正确
	public:

		citem* create_item_by_template(e_server_log_add_item add_type, int32 param, int32 item_template_id, int32 item_num = 1, int32 locked = 1, int32 item_over_time = 0, e_bag_type bag_type = e_bag_type_none, int32 item_const_att = 0);
		citem* create_item_by_info(const s_item_info& temp_info, e_server_log_add_item add_type, int32 param, bool write_log);
		std::vector<citem*> create_items(e_server_log_add_item add_type, int32 param, const  std::vector<s_item_template_info>& item_template_info_array);	// 批量创建item
	public:
		bool can_put_items(const std::vector<s_item_template_info>& item_array, e_bag_type bag_type = e_bag_type_none);
		bool can_put_items(const std::vector<citem*>& item_array, e_bag_type bag_type = e_bag_type_none);
		bool put_in_bag(std::vector<citem*>& item_inst_array, e_bag_type bag_type = e_bag_type_none, e_item_mail_type mail_type = e_item_mail_type_nomal);	// 把物品放入背包中 放不下就自动发邮件
		bool put_in_bag(citem* item_ptr, e_bag_type bag_type = e_bag_type_none, e_item_mail_type mail_type = e_item_mail_type_nomal);	// 把物品放入背包中 放不下就自动发邮件

		bool put_in_bag(e_server_log_add_item add_type, int32 param,
			const std::vector<int32>& item_data_array,
			e_bag_type bag_type = e_bag_type_none,
			e_item_transform_type item_transform_type = e_item_transform_type_two,
			e_item_mail_type mail_type = e_item_mail_type_nomal,
			int32 notice_id = 0
			);

		bool put_in_bag(e_server_log_add_item add_type, int32 param,
			const std::vector<s_item_template_info>& item_array,
			e_bag_type bag_type = e_bag_type_none,
			e_item_mail_type mail_type = e_item_mail_type_nomal,
			int32 notice_id = 0
		);
	public:
		// locked: 0表示只获得非绑定物品数组 1表示只获取绑定的物品数组 2表示绑定和非绑定的都获取
		// only_bag表示是否只获取背包中的相应物品
		std::vector<citem*> get_items_by_template_id(int32 item_template_id, int32 locked = e_item_lock_status_ignore, bool only_bag = true);
		int32 get_item_count(e_bag_type bag_type, int32 item_template_id, int32 lock = -1);//所有物品数量
		std::vector<s_item_template_info> get_item_template_info_array(const std::vector<int32>& item_array, e_item_transform_type item_transform_type = e_item_transform_type_two);	// 将物品数组转换为结构体 目前支持二元数组和三元数组
		std::vector<s_item_info> get_item_info_array(std::vector<citem*>& item_ptr_array);
	public:
		void clear_bag(e_bag_type bag_type);
		void del_item(e_server_log_del_item causeid, int32 location, e_bag_type bag_type, int32 item_template_id, int32 del_num = 0);
		void del_item(e_server_log_del_item causeid, int32 location, e_bag_type bag_type, const std::vector<s_del_item_info>& del_info_array);//删除物品(item_id, del_num)
		void del_item(e_server_log_del_item causeid, int32 location, const guid_64& item_guid, int32 del_num = 0);
		void del_item(e_server_log_del_item causeid, int32 location, const std::vector<s_del_item_guid>& del_guid_array);
		void del_item(e_server_log_del_item causeid, int32 location, citem* item_ptr, int32 del_num = 0);
	public:
		bool tidy_item(e_bag_type tidy_bag_typ,	bool is_not_tidy_time = false); // 整理物品栏中的所有物品
		void add_and_del_tidy_type(e_bag_type tidy_bag_typ, bool is_add);
		int32 get_use_time_item_empty_slot();
		bool set_mount_or_wing_beast_spirit_bag_data(int32 item_id,int32 item_type, int32 upgrade_num);
		void change_beast_spirit_att(int32 beast_spirit_type, bool is_add_att);
		int32 mount_upgrade(const guid_64& mount_guid, int32 use_up_item);
		int32 fashion_upgrade(const guid_64& fashion_guid, int32 use_up_item, int32 use_num = 1);
		int32 mount_starupgrade(const guid_64& mount_guid, int32 use_up_item);
		int32 mount_illusionupgrade(const guid_64& mount_guid, int32 use_up_item);
		int32 mount_unlock(const guid_64& mount_guid);
		void  sync_ranking_mount();
		void  sync_ranking_spirit();
		int32 show_this_mount(citem* mount_ptr, int32 item_slot, bool is_buff = true);
		int32 show_this_mount_illusion(const guid_64& mount_guid, int32 item_slot);
		int32 get_mount_illusion(const guid_64& mount_guid, int32 illusion_slot);

		int32 get_item_from_treasure_to_bag();
		void  init_item_from_treasure_to_mail();

		void set_quick_mount_call_array(guid_64* guid_array);
		void add_new_quick_mount_call(const guid_64& new_mount_guid);
		void send_quick_mount_call();
		void clear_quick_mount_call_element(const guid_64& mount_guid);
		void get_hope_item(int32 get_typ);

		int32 get_total_mount_grade();
		int32 get_total_mount_activate_num();
		int32 get_highest_level_activate_mount_template_id();
		int32 get_highest_level_activate_wing_template_id();
		int32 get_wing_and_total_feather_grade_num();
		int32 get_wing_and_total_feather_star_num();
		int32 calcu_beast_sprite_fighting_power(int32 bag_type);

		guid_64 get_target_slot_guid(e_bag_type target_bag, int32 target_slot);
		void set_target_slot_guid(e_bag_type target_bag, int32 target_slot, guid_64 new_guid);
		void clear_target_slot(e_bag_type target_bag, int32 target_slot);

		void get_item_send_promp_msg_to_client(const std::vector<s_item_template_info>& item_tuple_array, bool is_continue_get = false, int32 boss_id = 0);

		int32 get_mount_illusion_slot(const int32 mount_illusion_template_id);

		/************************************************************************/
		/*                                    外部接口                          */
		/************************************************************************/
		public:
			int32 get_equip_num_by_strength_limit(int32 limit_level);
			int32 get_equip_num_by_addition_limie(int32 limit_level);
			int32 get_equip_num_by_awaken_limit(int32 limit_level);
			int32 get_equip_num_by_succinct_limit(int32 limit_level);
			int32 get_equip_num_by_limit(int32 limit_item_grade, int32 limit_item_color);
			int32 get_equip_num_by_spirit_baptism_color(int32 limit_level);
			int32 get_equip_num_by_spirit_color(int32 limit_level);
			int32 get_equip_num_by_inset_genstone();
			
			int32 get_equip_num_awaken_level();

			void check_target_mission(int32 item_id, int32 item_num, e_server_log_del_item del_type);

			void  get_mount_grade_and_level_by_order_num(int32 order_num, int32& grade, int32& level);
			void  get_cur_wing_grade_and_level(int32& grade, int32& level);

	public: // 计算相关
		void calcu_addition(e_addition_buff addition_buff_type, bool need_send_notice = true);//计算强化加成
		void calcu_addition_with_fake_player(e_addition_buff addition_buff_type);//计算雕像强化buff
		void calcu_addition_by_one(AdditionBuffTemplate* addition_tmpl_ptr, std::vector<int32>& buff_id_arr, std::vector<int32>& buff_equip_num_arr, std::vector<int32>& buff_need_num_arr, std::vector<int32>& buff_sub_type_arr, std::vector<int32>& buff_level_arr);// 计算一条buff是否符合
		void protect_spirit_change_att(std::vector<f32> const & att_array, int32 add_count, float radio, bool is_addo);
		void item_change_att(std::vector<f32> const & att_array, f32 add_count, bool is_add);
		int32 get_element_heart_equip_num();
		int32 get_total_element_heart_level();
		//计算战斗力
		int64 calcu_whole_equip_fighting_power();
		int64 calcu_feather_total_fighting_power();		//计算翎羽战斗力
		void sync_whole_euip_fighting_power();

		void refresh_element_heart_faker_player_buff();//用于雕像加buff

		int32 init_fake_player_item_info(s_item_info * item_info_list, int32 max_num);				//使用玩家信息初始化玩家数据怪物的物品信息,返回增加的物品数量
		/************************************************************************/
		//						放背包   接口使用文档
		// 功能 将物品放入背包中 

		//	put_item_into_bag 是由后面几个函数组合而成的 外部接口
		//		包含以下n个参数 
		//		item_array					由id num组成的容器
		//		item_put_into_bag_info	    物品放包时会触发的事情 比如是否发邮件 具体的 alt+g 进去看吧
		//		chat_notice_data_info		公告配置 
		//		mail_info					邮件配置 如果发送邮件 邮件的具体设置 默认的话是 发送人: 系统 邮件标题: 背包已满 邮件正文: 由于拾取时背包已满，物品通过邮件发送 请在邮件有效期内提取
		//		item_transform_type			转换规则 目前的填表规则是二元组 创建出来的物品默认都是锁定的 因此留出扩展余地 比如三元组为 id num lock 
		//	以上规则不满足需要的情况下 可以选择扩展 s_item_put_into_bag_info结构体
		/************************************************************************/
	public:
		void send_item_by_mail(std::vector<citem*>& item_array, e_item_mail_type mail_type);	// 用邮件发送物品
		void send_item_by_mail(std::vector<s_item_info>& item_array, e_item_mail_type mail_type);	// 用邮件发送物品
		s_item_info filter_items_with_notice(std::vector<citem*> item_ptr_array, std::string& item_names_str, int32 notices_id = 0);//筛选指定物品
		void set_quick_hp_guid(guid_64 newguid);
		int32 against_buy_item(guid_64 item_guid, int32 goods_id);
		void  check_time_limit_item();
		void  check_time_limit_fashion_item();
		void  check_time_limit_mount_and_wing_item();
		void  check_time_limit_equip_item();

		citem*	get_can_auto_use_hp_item();//获取自动使用的药品

		int32  show_this_init_mount();
		int32  show_this_init_max_wing(citem* item_ptr = nullptr);

		//精灵
		bool  has_spirit_on_equip();
		void send_spirit_msg(citem* temp_item,bool is_del = false);



		void  change_buff_time();//该函数用于改变因下线而不倒计时的buff时间，目前用于特殊羽翼翅膀，其buff时间与翅膀时间挂钩

		static s_item_template_info get_const_att_item_by_id(int32 item_tem_id, int32 item_num, int32 item_const_att, int32 item_lock, int32 class_type, int32 exp_level);//通过物品ID和人物职业获取实际的物品，目前主要用于固定卓越属性物品

		void add_fashion_star_num() { m_fashion_star_num++; }
		int32 get_fashion_star_num() { return m_fashion_star_num;}
		void refresh_spirit_att(int32 old_spirit_id, int32 new_spirit_id);
	private:
		int32 add_money_by_card(const std::vector<int32>& money_card, const int32 money_card_num, std::vector<int32>& get_money_list);
		int32 open_package(const e_prop_sub_type item_sub_type, const PropBasicAttributeTemplate* prop_template, const int32 package_num, e_error_code& item_use_end_result, std::vector<int32>& get_money_list, std::vector<s_item_template_info>& get_item_list , int32 notice_id = 0, ItemTemplate* parent_item_ptr = nullptr);
		int32 open_fruit_bag(const PropBasicAttributeTemplate* prop_template, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list, int32 notice_id);
		int32 open_package_bag(const int32 drop_template_id, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list, int32 notice_id = 0, ItemTemplate* parent_item_ptr = nullptr);
		int32 open_choose_package_bag(const int32 logic_id, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list, int32 notice_id = 0,int32 item_slot =0, ItemTemplate* parent_item_ptr = nullptr);
		int32 open_package_elementbag(const int32 drop_template_id, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list, int32 notice_id = 0);
		int32 open_rand_equip_package_bag(const int32 drop_template_id, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list);
		int32 open_package_wingbag(const int32 drop_template_id, item_proto_item_use_end& item_use_end_msg);
		int32 open_package_mountbag(const int32 drop_template_id, item_proto_item_use_end& item_use_end_msg, int32 over_time = 0);
		int32 open_package_time_limit_prop(const int32 drop_template_id, item_proto_item_use_end& item_use_end_msg, int32 over_time = 0);
		int32 open_const_att_equip_bag(const int32 drop_template_id, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list, citem*& end_item);
		
		
		void jewel_array_set_to_item(citem* target_item, std::vector<int32> jewel_array, bool is_send_mail);

	private:
		int32										m_unit_array_index;
		std::map<int32, int64>					 m_item_use_cd; // 记录所有物品的CD倒计时,每条记录为<TemplateID, CD剩余时间>
		// item_map，item_in_bag和item_in_equip三个数据结构的作用：
		//   1.item_map是个map结构，以每个物品实例的guid值作为key，value是物品实例的数据。
		//   2.item_in_bag存储了角色背包物品栏中相应位置的对应物品的guid。
		//   3.item_in_equip存储了角色背包装备栏中相应位置的对应物品的guid。
		//   4.item_in_bag_count和item_in_equip_count分别记录了物品栏和装备栏中物品的数量，可以更快的检验背包是否已满
		//
		// 查找物品实例的数据都要通过guid在item_map中查找，如果能提供物品在物品栏或装备栏中的位置，
		// 则可以根据提供的位置在item_in_bag或item_in_equip中找到对应的guid，然后再通过item_map获取
		// 物品的实例数据。
		//									// 物品所属角色下标
		int32 m_item_load_flag;
		std::unordered_map<ui64, citem*> m_item_map; // 以物品的guid为key，树上的每个节点存储item的数据
		std::unordered_map<int32,guid_64> m_item_bag_array[e_bag_type_max];
		int32 m_item_bag_length[e_bag_type_max];

		std::unordered_map<int32, guid_64>										m_used_has_time_item_array;			//带有时限的物品
		std::unordered_map<int32, guid_64>										m_time_limit_item_in_equip;			//装备的限时装备
		std::unordered_map<int32, guid_64>										m_quick_call_mount_array;			//快速召唤坐骑
		guid_64										m_quick_hp_guid;
		uint64										m_item_tidy_cd;
		std::vector<int32>							m_tidy_type_list;
		int32										m_fashion_star_num;
		//日志用的参数
		int32										m_bind_material_num;
		int32										m_no_bind_material_num;
	private:
		bool										m_wing_att_change;
		bool										m_mount_att_change;
		citem*										m_show_wing_item_ptr;
		citem*										m_show_mount_item_ptr;
	};
#define GET_BAG(bag_type) m_item_bag_array[bag_type] 
}

#endif
