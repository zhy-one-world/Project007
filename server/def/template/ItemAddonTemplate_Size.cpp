#include "ItemAddonTemplate_S.h"

namespace hld
{
    void ItemAddonTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ItemAddonTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ItemAddonTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ItemAddonTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemAddonTemplate].push_back("level");
        template_size::get_instance().template_size_map[e_ItemAddonTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ItemAddonTemplate].push_back("ItemIdList");
        template_size::get_instance().template_size_map[e_ItemAddonTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ItemAddonTemplate].push_back("ItemNumList");
        template_size::get_instance().template_size_map[e_ItemAddonTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemAddonTemplate].push_back("AddonShopId");
        template_size::get_instance().template_size_map[e_ItemAddonTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ItemAddonTemplate].push_back("MoneyCostList");
        template_size::get_instance().template_size_map[e_ItemAddonTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ItemAddonTemplate].push_back("InheritAddonRatio");
        template_size::get_instance().template_size_map[e_ItemAddonTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemAddonTemplate].push_back("Noticeld");
        template_size::get_instance().template_size_map[e_ItemAddonTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemAddonTemplate].push_back("SuccessRate");
        template_size::get_instance().template_size_map[e_ItemAddonTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ItemAddonTemplate].push_back("EquipBuffArray");
    }
}
