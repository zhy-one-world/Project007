#include "GodHeartTemplate_S.h"

namespace hld
{
    void GodHeartTemplateSize()
    {
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("ItemType");
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("MaxUpgradeLevel");
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("BasicAtt");
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("UpgradeAttEach");
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("UpgradeId");
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("ItemTemplateId");
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("StarBuffId");
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("Way");
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("ChangeLevel");
        template_size::get_instance().template_size_map[e_GodHeartTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GodHeartTemplate].push_back("StarBuffIdShow");
    }
}
