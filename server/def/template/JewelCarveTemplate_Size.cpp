#include "JewelCarveTemplate_S.h"

namespace faith
{
    void JewelCarveTemplateSize()
    {
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("EquipSlot");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("Consume");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("RandomSkillSet");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("ShowSkillName");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("ShowSkillDescribe");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("SubType");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("SkillID");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("RealType");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("RealValue");
        template_size::get_instance().template_size_map[e_JewelCarveTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelCarveTemplate].push_back("ResId");
    }
}
