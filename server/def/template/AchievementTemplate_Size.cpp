#include "AchievementTemplate_S.h"

namespace faith
{
    void AchievementTemplateSize()
    {
        template_size::get_instance().template_size_map[e_AchievementTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_AchievementTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_AchievementTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AchievementTemplate].push_back("PageId");
        template_size::get_instance().template_size_map[e_AchievementTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AchievementTemplate].push_back("AchievementType");
        template_size::get_instance().template_size_map[e_AchievementTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AchievementTemplate].push_back("AchievementNameId");
        template_size::get_instance().template_size_map[e_AchievementTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AchievementTemplate].push_back("AchievementDiscreptionId");
        template_size::get_instance().template_size_map[e_AchievementTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_AchievementTemplate].push_back("AchievementGoal");
        template_size::get_instance().template_size_map[e_AchievementTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_AchievementTemplate].push_back("FinishMoney");
    }
}
