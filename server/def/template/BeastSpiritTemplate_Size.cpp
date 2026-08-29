#include "BeastSpiritTemplate_S.h"

namespace faith
{
    void BeastSpiritTemplateSize()
    {
        template_size::get_instance().template_size_map[e_BeastSpiritTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_BeastSpiritTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_BeastSpiritTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeastSpiritTemplate].push_back("BeastSpiritType");
        template_size::get_instance().template_size_map[e_BeastSpiritTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeastSpiritTemplate].push_back("ItemId");
        template_size::get_instance().template_size_map[e_BeastSpiritTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BeastSpiritTemplate].push_back("UseNumArray");
        template_size::get_instance().template_size_map[e_BeastSpiritTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BeastSpiritTemplate].push_back("UnlockConditionArray");
        template_size::get_instance().template_size_map[e_BeastSpiritTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BeastSpiritTemplate].push_back("GetAttributes");
        template_size::get_instance().template_size_map[e_BeastSpiritTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeastSpiritTemplate].push_back("ShowIndex");
    }
}
