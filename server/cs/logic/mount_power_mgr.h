#ifndef MOUNT_POWER_MGR_H
#define MOUNT_POWER_MGR_H

#include "logic/type_def.hpp"


namespace faith
{
	class player;
	class citem;
	struct MountEquipTemplate;

	class mount_power_mgr
	{
	public:
		mount_power_mgr();
		~mount_power_mgr();
	public:
		void clear_data();
		void set_player_ptr(const int32 array_index);
	public:
		void  on_oper(std::vector<guid_64> & s_item_guid, int32 oper_type,int32 slot, guid_64 d_item_guid);
		int32 on_equip(guid_64 s_item_guid, int32 slot, guid_64 d_item_guid);
		int32 on_star(guid_64 s_item_guid, int32 slot);
	public:
		MountEquipTemplate *get_mount_equ_template_by_template_id(int32 template_id);
		MountEquipTemplate *get_mount_equ_template_by_mount_id(int32 mount_id);
		MountEquipTemplate *get_mount_equ_template_by_item(citem *itemm, int32 slot, int32 level);
		void  set_mount_power_attr(int32 template_id, int32 level, bool is_add);
		bool  put_mount_equ_in_bag(int32 template_id, int32 level);
		void  set_core_is_active(citem * item);

	public:
		int64  calc_mount_power_fight(citem *item);
		void   set_item_attr_all(citem *item, bool is_add);
		void   set_core_skill(citem *item);
	private:
		int32 m_array_index;
	};

}













#endif
