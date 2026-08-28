

#ifndef _DROP_H_
#define _DROP_H_

#include <Logic/drop_def.hpp>
#include "logic/player.hpp" 


namespace hld
{
    class cdrop 
    {
    public:
		static void gen_drop_id_list(int32 drop_template_id, std::vector<s_item_template_info>& item_drop_with_att, int32 drop_weight, int32 curstep, int32 class_type = hld::e_class_type_none, int32 exp_level = -1);
		
		static void merge_drop_bag(const std::vector<s_item_template_info>& drop_list_in, std::vector<s_item_template_info>& drop_list_out, int32 class_type = e_class_type_none, int32 exp_level = -1);
		static int32 get_drop_id(int32 drop_template_id);

		// 这个是最方便的一个接口,会返回(item_id,item_num)元组为列表的掉落组
		static void gen_drop_list_by_drop_id(int32 drop_template_id, std::vector<s_item_template_info>& drop_list_with_num_ref, int32 class_type = e_class_type_none, int32 exp_level = -1);
		// 等级是否符合
		static bool meet_level(DropTemplate* drop_ptr, int32 owner_level);

	public:
		static std::vector<s_item_template_info> mission_drop(MissionTemplate* mission_ptr, player* player_ptr);			// 任务物品接口， 返回掉落物品列表

    private:
        static bool rand_trigger(int32 drop_weight, int32 drop_denominator);
		static void sub_drop_rand_one(int32 drop_template_id, std::vector<s_item_template_info>& item_drop_with_att, int32 curstep, int32 class_type = hld::e_class_type_none, int32 exp_level = -1);
		static void sub_drop_rand_all(int32 drop_template_id, std::vector<s_item_template_info>& item_drop_with_att, int32 curstep, int32 class_type = hld::e_class_type_none, int32 exp_level = -1);
		static void add_item_id_to_drop_list(int32 item_template_id, std::vector<s_item_template_info>& drop_list, int32 item_const_att, int32 class_type = e_class_type_none, int32 exp_level = -1);
    };
}

#endif