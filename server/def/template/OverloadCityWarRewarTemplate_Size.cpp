#include "OverloadCityWarRewarTemplate_S.h"

namespace hld
{
    void OverloadCityWarRewarTemplateSize()
    {
        template_size::get_instance().template_size_map[e_OverloadCityWarRewarTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_OverloadCityWarRewarTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_OverloadCityWarRewarTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_OverloadCityWarRewarTemplate].push_back("IsCross");
        template_size::get_instance().template_size_map[e_OverloadCityWarRewarTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_OverloadCityWarRewarTemplate].push_back("LevelLimit");
        template_size::get_instance().template_size_map[e_OverloadCityWarRewarTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_OverloadCityWarRewarTemplate].push_back("ShowItemId");
        template_size::get_instance().template_size_map[e_OverloadCityWarRewarTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_OverloadCityWarRewarTemplate].push_back("ShowItemNum");
        template_size::get_instance().template_size_map[e_OverloadCityWarRewarTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_OverloadCityWarRewarTemplate].push_back("RewardItemArray");
        template_size::get_instance().template_size_map[e_OverloadCityWarRewarTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_OverloadCityWarRewarTemplate].push_back("ChiefRewardArray");
    }
}
