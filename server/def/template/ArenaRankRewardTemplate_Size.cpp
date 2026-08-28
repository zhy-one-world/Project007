#include "ArenaRankRewardTemplate_S.h"

namespace hld
{
    void ArenaRankRewardTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ArenaRankRewardTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ArenaRankRewardTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ArenaRankRewardTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRankRewardTemplate].push_back("ArenaPos");
        template_size::get_instance().template_size_map[e_ArenaRankRewardTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ArenaRankRewardTemplate].push_back("ItemRewardArray");
        template_size::get_instance().template_size_map[e_ArenaRankRewardTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ArenaRankRewardTemplate].push_back("MoneyRewardArray");
        template_size::get_instance().template_size_map[e_ArenaRankRewardTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRankRewardTemplate].push_back("RewardDescription");
        template_size::get_instance().template_size_map[e_ArenaRankRewardTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRankRewardTemplate].push_back("NoticeId");
    }
}
