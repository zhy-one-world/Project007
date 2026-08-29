#ifndef _GUIDE_MGR_H_
#define _GUIDE_MGR_H_

#include "logic/type_def.hpp"
#include "Logic/guide_def.hpp"

namespace faith
{
	struct SkillTemplate;

	class guide_mgr
	{
	public:
		explicit guide_mgr();

		void		clear_data();
		void		set_unit_index(int32 unit_index) { m_unit_index = unit_index; }

		void		save_guide_record(s_record_info& record_info);
		void        save_guide_record_lua(int32 guide_trigger_id);
		void		load_guide_record(const int32 record_num, const s_record_info record_info_list[faith::max_guide_record_num]);
		bool        load_guide_record_by_db_lua(const char *data_ptr, int32 data_len);
		void		load_guide_record_reset(const int32 record_num, const s_record_info record_info_list[faith::max_guide_record_num]);
		void		send_all_guide_record();

		void		get_add_guide_record(int32& add_record_info_num, s_record_info add_record_info_arr[faith::max_guide_record_num]);
		bool		if_exist_guide_record(s_record_info record_info);
		bool		if_guide_triggered(int32 guide_trigger_id);
	
		void		logout_save_guide_record();
		void		add_skip_begin_guide_record(e_class_type class_type);
		void		add_once_guide_recond(s_record_info record_info, bool need_save = false);
		void		guide_trigger_start_proc(int32 guide_trigger_template_id);

		void		check_guide();
	private:
		int32				m_unit_index;
		s_record_info		m_record_info_list[faith::max_guide_record_num];
		int32				m_record_num;
		SkillTemplate* m_skill_template_ptr;
	};
}

#endif
