#ifndef AWAKEN_MGR_H
#define AWAKEN_MGR_H

#include "logic/type_def.hpp"


namespace faith
{
	class citem;
	struct AwakenTemplate;

	const int32  awaken_error_success = 0;
	const int32  awaken_error_need_guid = -1;
	const int32  awaken_error_main_guid = -2;
	const int32  awaken_error_item_color = -3;
	const int32  awaken_error_item_info = -4;
	const int32  awaken_error_item_type = -5;
	const int32  awaken_error_awaken_config = -6;
	const int32  awaken_error_max_level = -7;
	const int32  awaken_error_tran_level = -8;
	const int32  awaken_error_need_power = -9;
	const int32  awaken_error_need_consume = -10;


	class awaken_mgr
	{
	public:
		awaken_mgr();
		~awaken_mgr();
	public:
		void clear_data();
		void set_player_ptr(const int32 array_index);
		void item_awaken_operate_lua(guid_64 main_guid, const std::vector<guid_64>& need_item_guid);
		void operate_msg(guid_64 main_guid, int32 result);
		void syc_client_update_item(citem* p_item);
		bool is_exist_item_guid_array(const std::vector<guid_64>& need_item_guid);
		bool is_item(guid_64 item_guid);
		citem *get_item(guid_64 item_guid);
		int32 get_item_power(guid_64 item_guid);
		int32 get_all_item_power(const std::vector<guid_64>& need_item_guid);
		AwakenTemplate *get_awaken_template_by_item(citem *item_ptr);
		AwakenTemplate *get_next_awaken_template_by_item(citem *item_ptr);
		bool  is_type(int32 type, int32 sub_type);
		int32 get_grade_num();
		bool  is_material(int32 material_id, int32 material_num);
		void  del_material(int32 material_id, int32 material_num);
		void  set_awaken_attr(citem *item, bool is_add);
		bool  is_can_inherit(citem* strip_item_ptr, citem* inherited_item_ptr);
		void  set_inherit(citem* strip_item_ptr, citem* inherited_item_ptr);
		void  sync_fight_power(citem *item);
	
	public:
		bool    is_use_lua();
		xstring item_guid_array_to_string(const std::vector<guid_64>& need_item_guid);
		citem  *get_item_id_str(xstring item_guid_str);
		void    item_str_to_item_guid_array_del(xstring  item_array_str);
		void    operate_msg_str(xstring main_guid_str, int32 result);
	private:
		int32    m_array_index;
	};

}
















#endif