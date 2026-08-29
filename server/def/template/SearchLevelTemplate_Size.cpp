#include "SearchLevelTemplate_S.h"

namespace faith
{
    void SearchLevelTemplateSize()
    {
        template_size::get_instance().template_size_map[e_SearchLevelTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_SearchLevelTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_SearchLevelTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SearchLevelTemplate].push_back("CurText");
        template_size::get_instance().template_size_map[e_SearchLevelTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SearchLevelTemplate].push_back("CurItem");
        template_size::get_instance().template_size_map[e_SearchLevelTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_SearchLevelTemplate].push_back("ColorPercent");
        template_size::get_instance().template_size_map[e_SearchLevelTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_SearchLevelTemplate].push_back("AlreadyItem");
        template_size::get_instance().template_size_map[e_SearchLevelTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_SearchLevelTemplate].push_back("Shall");
        template_size::get_instance().template_size_map[e_SearchLevelTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_SearchLevelTemplate].push_back("UpgradeMoney");
        template_size::get_instance().template_size_map[e_SearchLevelTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SearchLevelTemplate].push_back("SearchExp");
    }
}
