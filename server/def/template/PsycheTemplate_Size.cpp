#include "PsycheTemplate_S.h"

namespace hld
{
    void PsycheTemplateSize()
    {
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("Describe");
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("FetterAttArray");
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("FetterActivateNeedArray");
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("Psyche");
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("PsycheNum");
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("AttArray");
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("SpawnNpcId");
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("ProtectPoint");
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("PokedexTemplateId");
        template_size::get_instance().template_size_map[e_PsycheTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_PsycheTemplate].push_back("WidgetJumpArray");
    }
}
