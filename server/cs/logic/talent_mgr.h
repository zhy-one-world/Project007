/********************************************************************
created: 2017-5-26 16:45:37
file base: talent_mgr
file ext: h
author: zhy

purpose: about role's talent_mgr
*********************************************************************/
#ifndef _TALENT_MGR_H_
#define _TALENT_MGR_H_

#include "talent.h"

namespace hld
{
	class ctalent_mgr
	{
	public:
		ctalent_mgr();
		~ctalent_mgr(void);
	public:
		void heart_tick(const int64& new_time);
		void set_player_ptr(int32 unit_array_index);
		void clear_data();
	public:
		void save_talent_to_db(int32 save_type);
		void load_talent_by_db(const s_talent_info* talent_data, int32 talent_num);
		bool load_talent_by_db_lua(const char *data_ptr, int32 data_len);
		void send_talent_all();
		void send_talent_one(ctalent* talent_ptr);
		void set_talent_by_info(const s_talent_info& talent_data, int32 talent_index);
		void set_talent_all_buff(bool is_add);
		void set_talent_all_skill(bool is_add);
	public:
		ctalent* get_talent_by_template_id(int32 talent_template_id);
		ctalent* get_talent_by_series(int32 series_id);
		ctalent* get_talent_empty();
	public:
		int32 get_layer_talent_num(std::vector<int32>& layer_data);
		int32 get_talent_list_num(int32 talent_list_template_id);
		std::vector<int32>& get_data_by_layer_index(int32 talent_list_template_id, int32 layer_index);
		int32 get_total_gs_value();
		int32 get_talent_cast(const int32 talent_type);

	public:
		void talent_level_up(int32 talent_template_id, int32 talent_list_template_id, int32 layer_index);
		void talent_reset();
	private:
		std::vector<ctalent*>						m_talent_vector;
		int32										m_unit_array_index;						// Ìì¸³ËùÊô½ÇÉ«
		std::vector<int32>							m_empty_layer;
	};
}

#endif
