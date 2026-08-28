#include "GradeGodHoodTemplate_S.h"

namespace hld
{
    void GradeGodHoodTemplateSize()
    {
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("LevelMax");
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("GradeName");
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("SmallGradeImageId");
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("RewardDescribeImageId");
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("GradeUpCostMoney");
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("GradeUpCostItem");
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("GradeUpMission");
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("GradeUpSkills");
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("GradeUpProperty");
        template_size::get_instance().template_size_map[e_GradeGodHoodTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GradeGodHoodTemplate].push_back("GradeUpTalent");
    }
}
