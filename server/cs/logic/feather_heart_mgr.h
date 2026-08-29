#ifndef FEAHTER_HEART_MGR_H
#define FEAHTER_HEART_MGR_H

#include"feather_heart.h"

namespace faith
{
	class player;
	class citem;
	struct GodHeartTemplate;

	class feather_heart_mgr
	{
	public:
		feather_heart_mgr();
	    ~feather_heart_mgr();
	public:
	    void set_player_ptr(player *in_player);
		void clear_data();
		void feather_event_param(int32 skill_template_id, int64 skill_cd, bool apply, int32 feather_heart_index);

		GodHeartTemplate* get_feather_heart_template(citem& feather_heart);

		GodHeartTemplate* get_feather_heart_template_by_template_id(int32 template_id);
		void  set_feather_heart_attr(int32 template_id, int32 level, bool is_add);
		void  set_feather_heart_attr_all(bool is_add);
		void  set_feather_heart_attr_all(citem &item, bool is_add);
		void  put_feather_heart_in_bag(int32 template_id, int32 level);

		void  init_set_feather_skill_info();
		feather_heart* get_empty_slot();
		feather_heart* get_slot_by_template_id(int32 template_id);

	


		int32 get_slot_add_level_by_item(citem *item_ptr);
		int32 get_slot_add_level_by_template_id(int32 template_id, int32 level);
		int32 get_slot_add_level_by_core(const std::vector<int32>& change_level_vec, int32 level);
		int32 get_buff_id(int32 template_id, int32 level, int32 add_level);
		void  add_buff_att(int32 template_id, int32 level,int32 add_level);
		void  dec_buff_att(int32 template_id, int32 level,int32 add_level);

		void sync_ranking_feather_heart_data();

	private:
		player* m_player_ptr;
		feather_heart m_feather_heart[e_feather_heart_max];
	};
}



#endif // !FEAHTER_HEART_MGR_H

