#ifndef _DARGONTRIP_MGR_H_
#define _DARGONTRIP_MGR_H_

#include "Logic/dragontrip_def.h"
#include "template/ServerTemplateHead.h"

namespace faith
{
	class dragontrip_mgr
	{
	public:
		dragontrip_mgr();
		~dragontrip_mgr();
	public:
		void clear_data();
		void set_player_ptr(const int32 array_index);
	public:
		void save_dragontrip_to_db(int32 save_type);
		void load_dragontrip_by_db(const s_dragontrip_info & data_info);
		bool load_dragontrip_by_db_lua(const char *data_ptr, int32 data_len);
	public:
		void sync_all_message_to_client();
	public:
		bool  is_use_lua();
		void  on_operate(int32 oper_type, int32 param);
		void  on_money(int32 oper_type, int32 param);
		void  on_item(int32 oper_type, int32 param);
		void  on_roll(int32 oper_type, int32 param);
		void  on_five_roll(int32 oper_type, int32 param);
		void  on_reward_round(int32 oper_type, int32 param);
		void  on_reward_all(int32 oper_type, int32 param);
		void  on_quick(int32 oper_type, int32 param);
		void  oper_msg(int32 oper_type, int32 result, int32 roll_num = 0, int32 item_id =0, int32 item_num = 0);
		void  oper_msg_item_list(int32 oper_type, int32 result, int32 roll_num, std::vector<int32> item_list);
		void  get_activity_item(int32 level, int32& out_item_id, int32& out_item_num);
	public:
		int32 get_config_param(e_dragontrip_param type);
		DragonTripTemplate *get_item_by_type_pos(int32 type, int32 pos);
		void  get_reward(int32 type, int32 pos);
		void  get_round_reward(int32 pos, int32& out_item_id, int32& out_item_num);

	public:
		void  day_refresh();
		bool  is_free();
		void  set_free_time();
		void  refresh_recycle();
	public:
		void  set_data(int32 idx, int32 value);
		int32 get_data(int32 idx) const;
	public:
		int32 get_reward_flag(int32 idx);
		void  set_reward_flag(int32 idx);


	private:

		s_dragontrip_info m_data;
		int32 m_array_index;
	};

}














#endif
