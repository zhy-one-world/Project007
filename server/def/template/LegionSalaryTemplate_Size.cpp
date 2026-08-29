#include "LegionSalaryTemplate_S.h"

namespace faith
{
    void LegionSalaryTemplateSize()
    {
        template_size::get_instance().template_size_map[e_LegionSalaryTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_LegionSalaryTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_LegionSalaryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_LegionSalaryTemplate].push_back("AimType");
        template_size::get_instance().template_size_map[e_LegionSalaryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_LegionSalaryTemplate].push_back("AimTypeName");
        template_size::get_instance().template_size_map[e_LegionSalaryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_LegionSalaryTemplate].push_back("AimTypeImage");
        template_size::get_instance().template_size_map[e_LegionSalaryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_LegionSalaryTemplate].push_back("SubTypeId");
        template_size::get_instance().template_size_map[e_LegionSalaryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_LegionSalaryTemplate].push_back("SubAimReward");
        template_size::get_instance().template_size_map[e_LegionSalaryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_LegionSalaryTemplate].push_back("SubAimRewardName");
        template_size::get_instance().template_size_map[e_LegionSalaryTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_LegionSalaryTemplate].push_back("JobRate");
    }
}
