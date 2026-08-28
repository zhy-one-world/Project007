#include "BuffActionTemplate_S.h"

namespace hld
{
    void BuffActionTemplateSize()
    {
        template_size::get_instance().template_size_map[e_BuffActionTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_BuffActionTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_BuffActionTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_BuffActionTemplate].push_back("Action");
        template_size::get_instance().template_size_map[e_BuffActionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BuffActionTemplate].push_back("ActionParams");
        template_size::get_instance().template_size_map[e_BuffActionTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BuffActionTemplate].push_back("AttArray");
        template_size::get_instance().template_size_map[e_BuffActionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BuffActionTemplate].push_back("Condition");
        template_size::get_instance().template_size_map[e_BuffActionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BuffActionTemplate].push_back("ReplaceSkill");
    }
}
