#include "SpiritJiBanTemplate_S.h"

namespace hld
{
    void SpiritJiBanTemplateSize()
    {
        template_size::get_instance().template_size_map[e_SpiritJiBanTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_SpiritJiBanTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_SpiritJiBanTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SpiritJiBanTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_SpiritJiBanTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SpiritJiBanTemplate].push_back("Level");
        template_size::get_instance().template_size_map[e_SpiritJiBanTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SpiritJiBanTemplate].push_back("BeginId");
        template_size::get_instance().template_size_map[e_SpiritJiBanTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SpiritJiBanTemplate].push_back("Property");
        template_size::get_instance().template_size_map[e_SpiritJiBanTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_SpiritJiBanTemplate].push_back("SpriteReqIdArray");
        template_size::get_instance().template_size_map[e_SpiritJiBanTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_SpiritJiBanTemplate].push_back("PassiveSkillId");
        template_size::get_instance().template_size_map[e_SpiritJiBanTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SpiritJiBanTemplate].push_back("StrId");
    }
}
