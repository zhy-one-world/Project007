#include "FlipBoxTemplate_S.h"

namespace hld
{
    void FlipBoxTemplateSize()
    {
        template_size::get_instance().template_size_map[e_FlipBoxTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_FlipBoxTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_FlipBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_FlipBoxTemplate].push_back("FlipBoxType");
        template_size::get_instance().template_size_map[e_FlipBoxTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_FlipBoxTemplate].push_back("RandomItem");
        template_size::get_instance().template_size_map[e_FlipBoxTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_FlipBoxTemplate].push_back("AwardMoney");
        template_size::get_instance().template_size_map[e_FlipBoxTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_FlipBoxTemplate].push_back("BoxSpawnPoint");
    }
}
