#include "EquipWindowTemplate_S.h"

namespace faith
{
    void EquipWindowTemplateSize()
    {
        template_size::get_instance().template_size_map[e_EquipWindowTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_EquipWindowTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_EquipWindowTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_EquipWindowTemplate].push_back("ResourceID");
        template_size::get_instance().template_size_map[e_EquipWindowTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_EquipWindowTemplate].push_back("EquipNameID");
        template_size::get_instance().template_size_map[e_EquipWindowTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_EquipWindowTemplate].push_back("GetEquipStrIDArr");
    }
}
