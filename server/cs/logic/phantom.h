#ifndef PHANTOM_H
#define PHANTOM_H

#include "Logic/phantom_def.h"
#include "template/ServerTemplateHead.h"

namespace hld
{
	class player;
	class cphantom
	{
	public:
		cphantom();
		~cphantom();
	public:
		void clear_data();

		bool init_phantom_by_template(int32 phantom_id);
		bool init_phantom_by_info(s_phantom_info phantom_info);
	public:
		IllusionTemplate* get_config_ptr() { return m_config_ptr; }
	public:
		bool set_inst_data(int32 idx, int32 value);
		int32 get_inst_data(int32 idx) const;
		s_phantom_info& get_phantom_info_all() { return m_phantom_info; }
	public:
		int32 get_finish_count(int32 class_type);
		bool is_finish(int32 class_type);
		bool is_acitve();
		void set_item_value(int32 idx, int32 value);
		bool is_item_finish(int32 idx);
		int32 get_skill_id();
		bool  is_valid();

	private:
		IllusionTemplate *m_config_ptr;
		s_phantom_info m_phantom_info;
	};
	

}



#endif