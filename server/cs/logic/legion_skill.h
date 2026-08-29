#pragma once
/*@@

file name :	passive_skill.h

author : gy

description :

change list :

@@*/
//////////////////////////////////////////////////////////////////////////
//
//	file include
//
//////////////////////////////////////////////////////////////////////////
#include "Logic/skill_def.hpp"

//////////////////////////////////////////////////////////////////////////
//
//	class declare
//
//////////////////////////////////////////////////////////////////////////

namespace faith
{
	typedef std::map<int32, s_legion_skill_info> legion_skill_map;
	typedef legion_skill_map::iterator legion_skill_map_it;
	class legion_skill
	{
	public:
		legion_skill();
		~legion_skill();
	public:
		void clear_data();
		void load_legion_skill(const s_legion_skill_info* legion_skill_data, int32 data_num);
		bool load_legion_skill_by_db_lua(const char *data_ptr, int32 data_len);
		void save_legion_skill(int32 save_type_ex);
		void update_one_legion_skill_info(s_legion_skill_info temp_info);
		void send_all_legion_skill_level_to_client();
		void set_unit_index(int32 array_index) { m_array_index = array_index; }
		int32 get_skill_level_by_id(int32 legion_skill_id);
		int32 legion_skill_level_up(int32 legion_skill_id);
		void legion_buff_change(int32 legion_skill_id,int32 legion_skill_level);
		void level_up_legion_skill_func(const void* data_ptr);
		void lua_level_up_legion_skill_func(const char* data_ptr, int32 data_len);
		void init_legion_skill_buff();
		std::vector<int32> get_all_legion_skill_level();

	protected:
	private:
	public:
		int32 m_array_index;
		legion_skill_map      m_legion_skill_map;
	};
}