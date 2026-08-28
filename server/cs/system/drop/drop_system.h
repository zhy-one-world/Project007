#pragma once
#include "logic/type_def.hpp"

namespace hld
{
	struct s_item_template_info;

    class drop_system
    {
	public:
		static std::vector<s_item_template_info> get_drop_list(int32 drop_id);
	private:
		static void gen_drop_id_list(int32 drop_id, std::vector<s_item_template_info>& drop_list, int32 drop_weight, int32 curstep);
        static bool rand_trigger(int32 drop_weight, int32 drop_denominator);
		static void sub_drop_rand_one(int32 drop_id, std::vector<s_item_template_info>& drop_list, int32 curstep);
		static void sub_drop_rand_all(int32 drop_id, std::vector<s_item_template_info>& drop_list, int32 curstep);
		static void add_item_id_to_drop_list(int32 item_template_id, std::vector<s_item_template_info>& drop_list);
    };
}