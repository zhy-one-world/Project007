#ifndef _FUNC_UNLOCK_MGR_HPP_
#define _FUNC_UNLOCK_MGR_HPP_

#include "../../def/template/template_manager.h"

namespace faith
{
	typedef std::map<std::string, FuncUnlockTemplate*> func_unlock_template_map;
	typedef func_unlock_template_map::iterator func_unlock_template_map_it;
	class func_unlock_mgr
	{
	public:
		func_unlock_mgr();
		~func_unlock_mgr(void);
		void				clear_data();
		void				set_unit_index(int32 unit_index) { m_unit_index = unit_index; }

		void				load_func_unlock_template_map();

		FuncUnlockTemplate*	get_func_unlock_template_by_func_name(const std::string& func_name);
		static FuncUnlockTemplate* get_func_unlock_template_by_func_name_of_all_func(const std::string& func_name);
		bool				is_func_unlock(const std::string& func_name, int32 recursion_num = 0);
		bool				is_func_unlock(const int32 func_template_id);
		void				func_unlock_trigger_activate();
		bool				get_func_unluck_enable() { return m_func_unluck_enable; }
		void				set_func_unluck_enable(bool func_unluck_enable) { m_func_unluck_enable = func_unluck_enable; }

	private:
		void activate(std::vector<int32>& activate_arr);

	private:
		func_unlock_template_map					m_func_unlock_template_map;
		std::list<FuncUnlockTemplate*>				m_can_trigger_active_funcs;

		int32										m_unit_index;
		bool										m_func_unluck_enable;
	};
}

#endif
