/********************************************************************
created: 2016年10月18日17:00
file base: title_mgr
file ext: h
author: wucun

purpose: about title
*********************************************************************/
#ifndef _TITLE_MGR_H_
#define _TITLE_MGR_H_

#include "title.h"

namespace hld
{
	typedef std::map<int32, ctitle> title_map;
	typedef title_map::iterator title_map_it;

	class ctitle_mgr
	{
	public:
		//初始化
		ctitle_mgr();
		~ctitle_mgr(void);
		void clear_data();
		void heart_tick(const int64& new_time);	
		void set_unit_index(int32 unit_index) { m_unit_array_index = unit_index; }

		//消息
		void push_msg_own_title_all();
		void push_msg_one_title (int32 title_template_id);
		void add_title_msg_send(int32 title_template_id, int32 result);
		void open_title_system_send();
		
		//存档
		void load_title_by_db(const s_title_info* title_data, int32 data_num);
		bool load_title_by_db_lua(const char *data_ptr, int32 data_len);
		void save_title_to_db(int32 save_type);

		//基本操作
		bool			is_have_title(int32 title_template_id);				
		bool			is_equip_title(int32 title_template_id);
		int32			get_title_template_id_by_type(e_title_type title_type);
		int32			get_template_id_by_type_and_value(e_title_type title_type, int32 value1, int32  value2);
		void			add_loading_finish_title_type(e_title_type title_type, e_title_special_handle handle_type);
		void			add_loading_ranking_finish_title_type( const bool(&loading_title_flag)[hld::e_title_type_max]);

		int32 get_first_active_title_template_id();

		//添加与删除
		int32 is_add_title_by_template_id(int32 title_template_id);
		void add_title_by_template_id(int32 title_template_id);
		void add_title_by_info(const s_title_info& title_info);
		void remove_title_by_template_id(int32 title_template_id);

		//装备与卸下
		void equip_on_title(int32 title_template_id);
		void equip_off_title();

		//添加称号接口
		void add_title_by_type_and_value(e_title_type title_type, int32 value1 = 0, int32  value2 = 0);

		void ranking_first_changed(int32 ranking_type, bool is_first, bool is_notice);							//排行升到第一名或者从第一名掉下来

		//操作
		void set_title_no_need_see();									//设置无需查看称号

		//获取当前称号
		ctitle& get_title_by_template_id(int32 template_id);

	private:
		int32						m_unit_array_index;
		title_map					m_title_info;
		e_title_special_handle		m_loading_finish_need_add_title_type_arr[MAX_TITLE_NUM];		//loading后需要处理
		ctitle m_ctitle_default;
	};
}

#endif
