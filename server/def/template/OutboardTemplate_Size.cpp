#include "OutboardTemplate_S.h"

namespace hld
{
    void OutboardTemplateSize()
    {
        template_size::get_instance().template_size_map[e_OutboardTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_OutboardTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_OutboardTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_OutboardTemplate].push_back("MonsterLevel");
        template_size::get_instance().template_size_map[e_OutboardTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_OutboardTemplate].push_back("MonsterId");
        template_size::get_instance().template_size_map[e_OutboardTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_OutboardTemplate].push_back("ItemShow");
        template_size::get_instance().template_size_map[e_OutboardTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_OutboardTemplate].push_back("RecommendationGS");
        template_size::get_instance().template_size_map[e_OutboardTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_OutboardTemplate].push_back("ElementalMonsterType");
    }
}
