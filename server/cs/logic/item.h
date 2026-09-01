/********************************************************************
  created: 2015年8月28日11:14:26
  file base: item
  file ext: h
  author: zhy
  
  purpose: about role's item
*********************************************************************/
#ifndef _ITEM_H_
#define _ITEM_H_

#include <Logic/item_def.hpp>

namespace faith
{
	class item_proto_item_info;
	class item_proto_update_character_item;
	struct ItemTemplate;
	struct MountTemplate;
	struct FasionTemplate;
	struct BeastSpiritTemplate;
	struct EquipTemplate;
	struct WingTemplate;
	struct ItemUpgradeTemplate;
	struct ItemSuccinctTemplate;
	struct ItemAddonTemplate;
	struct ProtectSpiritTemplate;
	struct SpiritTemplate;
	struct DeityTemplate;
	struct WeddingRingTemplate;
	struct WeddingRingTemplate;
	struct AdditionBuffTemplate;
	struct MountEquipTemplate;

	class citem
	{
	public:
		citem(void);
		~citem(void);
	public:
		void										heart_tick(const int64& new_time);
		void										clear_data();
	public:
		int32										get_item_guid_A() { return m_item_data.item_guid.A; }
		int32										get_item_guid_B() { return m_item_data.item_guid.B; }
		const s_item_info&							get_item_inst() const					{ return m_item_data; }																					//实例	
		const guid_64&								get_item_guid() const					{ return m_item_data.item_guid; }								//GUID	
		int32										get_data_info(e_item_info eIndex) const	{ return m_item_data.data_ary[eIndex]; }
		void										set_data_info(e_item_info eIndex, int32 iValue) { m_item_data.data_ary[eIndex] = iValue; }					//模板指针
		void										set_item_guid(guid_64& item_guid) { m_item_data.item_guid = item_guid; }
		ItemTemplate*								get_item_info_ptr() const				{ return m_item_info_ptr; }		
		const int32									get_item_template_id() const;			// 模板id
		int32										get_item_logic_id();
		s_item_info									get_item_inst_tem()						{ return m_item_data; }
		MountTemplate*								get_mount_template_ptr();
		FasionTemplate*								get_fashion_template_ptr();
		BeastSpiritTemplate*						get_beast_spirit_template_ptr();
		EquipTemplate*								get_equip_info_ptr();
		WingTemplate*								get_wing_template_ptr();
		ItemUpgradeTemplate*						get_upgrade_info_ptr();
		ItemSuccinctTemplate*						get_succinc_info_ptr();
		ItemAddonTemplate*							get_add_on_info_ptr();
		ProtectSpiritTemplate*						get_protect_spirit_ptr();
		SpiritTemplate*								get_sprite_info_ptr();
		DeityTemplate*								get_deity_info_ptr();
		WeddingRingTemplate*						get_wedding_ring_info_ptr();
		WeddingRingTemplate*						get_wedding_ring_info_ptr_by_mate_ring_level();
		AdditionBuffTemplate*						get_heart_value_template_ptr();
		MountEquipTemplate*                         get_mount_equip_template_ptr();
		int32										get_real_id();
		bool										is_item_belong_to_class(const e_class_type class_type);
	public:
		void										set_item_data(const s_item_info& item_data);											//设置实例信息
		void										set_unit_skill(const int32& unit_array_index, bool is_add);
		void										set_unit_buff(const int32& unit_array_index, bool is_add); // 通过使用物品来添加BUFF
		void										set_unit_att(const int32& unit_array_index, bool is_add); // 通过使用物品来添加属性
		void										set_unit_equip_passive_skill(const int32& unit_array_index, bool is_add);	//添加装备被动技能
		bool										is_equip_type();
		e_fruit_type								is_fruit();
		bool										can_merge();
		int32                                       get_excellent_att_num(); // 获得装备卓越属性的个数
		void										set_temp_succinct_att(item_temp_succinct_property index , int32 value);
		int32										get_temp_succinct_att(item_temp_succinct_property index);

		void										set_forge_property(int32 i);
		void										get_forge_property(int32 i, int32* forge_att, int32* forge_name_att, int32* forge_name_flag_att);
		int32										get_forge_att(int32 item_data_index);
		int32										get_forge_name_att(int32 item_data_index);
		int32										get_forge_name_flag_att(int32 item_data_index);
		int32										get_forge_change_gs(int32 player_index);
		int32										have_not_save_forge_att_num();//存在未保存的属性的条数
		int32										have_excellent_att_num();//当前存在特殊属性（卓越属性）的条数

		void										fill_item_info_msg(item_proto_item_info& item_info_msg);

		bool										can_used_by_class_type(e_class_type class_type);
		bool										is_succinct_full();
		int32										get_item_succinct_level();
		int32										get_item_grade();
		int32										get_item_color();

		bool										get_add_att() { return m_is_add_att; };			
		void										set_add_att(bool is_add_att) { m_is_add_att = is_add_att; };	//设置是否激活

		void										get_property_max_limit(std::vector<int32>& vec_property_max_limit); //获取真正的洗练属性上限
		e_item_succinct_buff_level					get_item_succinct_current_buff_level(); //获取当前物品的洗练buff等级
		void										get_item_msg(item_proto_update_character_item& item_msg, bool to_self = true);
	public:
		int64 calcu_single_equip_fighting_power(const double* att_array);
		int64 calcu_single_item_fighting_power(int32 array_index);
	private:
		s_item_info									m_item_data;		// 物品实例数据	
		ItemTemplate*								m_item_info_ptr;	// 物品模板数据指针	
		int32										m_item_temp_succinct_block[item_temp_succinct_property_max];			// 装备洗练临时数据，此数据不存档

		int32										m_item_temp_forge_flag;											 //重铸属性标记位

		bool										m_is_add_att;//属性已激活，可以正常加属性
	};
}

#endif
