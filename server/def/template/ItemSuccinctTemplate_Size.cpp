#include "ItemSuccinctTemplate_S.h"

namespace hld
{
    void ItemSuccinctTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("PropertyID");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("PropertyMaxLimit");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("GoldCost");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("DiamondCost");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("MaterialID");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("MaterialCost");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("GoldSuccinctEffectChance");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("DiamondSuccinctEffectChance");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("GoldSuccinctAttUpChance");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("GoldPropertyChangeRange");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("DiamondSuccinctAttUpChance");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("DiamondPropertyChangeRange");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("Noticeld");
        template_size::get_instance().template_size_map[e_ItemSuccinctTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ItemSuccinctTemplate].push_back("MaterialShopID");
    }
}
