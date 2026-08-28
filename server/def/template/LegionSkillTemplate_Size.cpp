#include "LegionSkillTemplate_S.h"

namespace hld
{
    void LegionSkillTemplateSize()
    {
        template_size::get_instance().template_size_map[e_LegionSkillTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_LegionSkillTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_LegionSkillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_LegionSkillTemplate].push_back("UnlockLevel");
        template_size::get_instance().template_size_map[e_LegionSkillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_LegionSkillTemplate].push_back("Buff");
        template_size::get_instance().template_size_map[e_LegionSkillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_LegionSkillTemplate].push_back("Name");
        template_size::get_instance().template_size_map[e_LegionSkillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_LegionSkillTemplate].push_back("Icon");
        template_size::get_instance().template_size_map[e_LegionSkillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_LegionSkillTemplate].push_back("Describe");
        template_size::get_instance().template_size_map[e_LegionSkillTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_LegionSkillTemplate].push_back("UpLevelCost1");
        template_size::get_instance().template_size_map[e_LegionSkillTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_LegionSkillTemplate].push_back("UpLevelCost2");
        template_size::get_instance().template_size_map[e_LegionSkillTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_LegionSkillTemplate].push_back("UnlockContribution");
    }
}
