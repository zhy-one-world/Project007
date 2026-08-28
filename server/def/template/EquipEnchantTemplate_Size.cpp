#include "EquipEnchantTemplate_S.h"

namespace hld
{
    void EquipEnchantTemplateSize()
    {
        template_size::get_instance().template_size_map[e_EquipEnchantTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_EquipEnchantTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_EquipEnchantTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_EquipEnchantTemplate].push_back("Level");
        template_size::get_instance().template_size_map[e_EquipEnchantTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_EquipEnchantTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_EquipEnchantTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_EquipEnchantTemplate].push_back("Slot");
        template_size::get_instance().template_size_map[e_EquipEnchantTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_EquipEnchantTemplate].push_back("ItemConsume");
        template_size::get_instance().template_size_map[e_EquipEnchantTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_EquipEnchantTemplate].push_back("ItemGet");
        template_size::get_instance().template_size_map[e_EquipEnchantTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_EquipEnchantTemplate].push_back("Describe");
        template_size::get_instance().template_size_map[e_EquipEnchantTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_EquipEnchantTemplate].push_back("AttArray");
    }
}
