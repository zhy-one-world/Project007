#include "GrowUpFundTemplate_S.h"

namespace faith
{
    void GrowUpFundTemplateSize()
    {
        template_size::get_instance().template_size_map[e_GrowUpFundTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_GrowUpFundTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_GrowUpFundTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GrowUpFundTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_GrowUpFundTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GrowUpFundTemplate].push_back("Index");
        template_size::get_instance().template_size_map[e_GrowUpFundTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GrowUpFundTemplate].push_back("TargetStrID");
        template_size::get_instance().template_size_map[e_GrowUpFundTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GrowUpFundTemplate].push_back("TargetValue");
        template_size::get_instance().template_size_map[e_GrowUpFundTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GrowUpFundTemplate].push_back("RewardStrID");
        template_size::get_instance().template_size_map[e_GrowUpFundTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GrowUpFundTemplate].push_back("MoneyRewardArray");
        template_size::get_instance().template_size_map[e_GrowUpFundTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GrowUpFundTemplate].push_back("ImageRes");
        template_size::get_instance().template_size_map[e_GrowUpFundTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GrowUpFundTemplate].push_back("ShowOrder");
    }
}
