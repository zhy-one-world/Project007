#ifndef _SKYTREASURE_MGR_H_
#define _SKYTREASURE_MGR_H_

#include "Logic/skytreasure_def.h"
#include "template/ServerTemplateHead.h"

namespace faith
{
	class skytreasure_mgr
	{
	public:
		skytreasure_mgr();
		~skytreasure_mgr();
	public:
		void  clear_data();
		void  set_player_ptr(const int32 array_index);
	public:
		void  save_skytreasure_to_db(s_skytreasure_info skytreasure_info);
		void  load_skytreasure_by_db(const int32 skytreasure_num, const s_skytreasure_info skytreasure_info_list[max_skytreasure_num]);
		bool  load_skytreasure_by_db_lua(const char *data_ptr, int32 data_len);
	public:
		void  sync_all_message_to_client();

	public:
		void  refresh_recycle();
	public:
		void  on_operate(int32 operate_type, int32 param1, int32 param2);
		void  get_reward(int32 operate_type, int32 layer_num, int32 slot_pos);
		void  buy_key(int32 operate_type, int32 branch_template_id, int32 item_num);
		void  operate_end(int32 operate_type, int32 result);
		void  send_skytreasure_notice(int32 item_id);

	private:
		s_skytreasure_info m_skytreasure_info_list[max_skytreasure_num];
		int32 m_skytreasure_num;
		int32 m_array_index;
	};

}














#endif
