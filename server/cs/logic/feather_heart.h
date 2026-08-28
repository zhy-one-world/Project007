#ifndef FEAHTER_HEART_H
#define FEAHTER_HEART_H

#include"item.h"

#include"Logic/feather_def.hpp"
#include "event_processer.h"


namespace hld
{
	class feather_heart_mgr;
	struct FeatherTemplate;

	class feather_heart : game_event_handler
	{
	public:
		feather_heart() { clear_data(); };
		~feather_heart() {};
	public:
		void clear_data();
	public:
		bool on_event(s_trigger_param& trigger_param);
		void feather_event_param(int32 skill_template_id, int64 skill_cd, bool apply);
		void trigger_event(int32 target_index, bool is_add);
	public:
		void set_base_info(feather_heart_mgr *in_feather_mgr_ptr, int32 in_owner_index, int32 in_template_id,guid_64 in_item_guid);
		bool is_use();
		void set_skill(int32 slot ,int32 buffid);
		void clear_skill();
		int32 get_template_id();
		FeatherTemplate *get_feather_template();
		int32 get_feather_slot_startbuff(int32 slot);
	private:
		bool  m_trigger_event;
		int32 m_have_skill[e_feather_have_skill_max];
		int32 m_owner_index;
		int32 m_template_id;
		guid_64 m_item_guid;
		feather_heart_mgr *m_feather_mgr;
	};
}



#endif // !FEAHTER_HEART_MGR_H

