#include "ItemUpgradeTemplate_S.h"

namespace hld
{
    void ItemUpgradeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ItemUpgradeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ItemUpgradeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ItemUpgradeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemUpgradeTemplate].push_back("EquipSlot");
        template_size::get_instance().template_size_map[e_ItemUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ItemUpgradeTemplate].push_back("NoticeId");
        template_size::get_instance().template_size_map[e_ItemUpgradeTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ItemUpgradeTemplate].push_back("AttArray");
    }
}
