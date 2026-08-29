#ifndef _ROLE_COMPETITION_MGR_H_
#define _ROLE_COMPETITION_MGR_H_

#include "item.h"

namespace faith
{
	class role_competition_mgr
	{
	public:
		role_competition_mgr();
		~role_competition_mgr();

	public:
		void									set_unit_index(int32 array_index) { m_array_index = array_index; };
		void									clear_data();
		void									heart_tick(const int64& new_time);
		void									load_competition_info_end(s_role_competition_info info);
		int32                                   load_role_competition_by_db_lua(const char *data_ptr, int32 data_len);
		void									reset_competition_data();
		void									save_to_db(e_save_role_data_type eType);
		void									sync_competition_info();
		void									get_reward(int32 id, int32 type);
		void									get_reward_all(int32 get_type);
		void									add_competition_exp(int64 point_num);
		void									sync_lv_exp();
		void									send_reward(int32 m_id,int32 m_type);
		int64									transfer_date_to_stamp(int64 date);
		s_role_competition_info&				get_competition_info() { return m_cur_competition_info; };
		void									buy_competition_level(int32 level);
		void									check_is_manual(int32 goods_id);
		void									buy_speical_manual(int32 manual_type);
	private:
		void									activati_manual(int32 manual_type);
		void									activati_manual_reward_by_type(int32 active_type, int32 active_ext0, int32 active_ext1);
		void									init_data();
		void									change_point();
		void									check_state(const int64& new_time);
		void									calcu_cur_competition();
	private:
		int32									m_array_index;
		s_role_competition_info					m_cur_competition_info;
		std::vector<s_competition_time>			m_all_competition;
		e_competition_manual_state				cur_state;
		bool									can_sync;
	};
}
#endif
