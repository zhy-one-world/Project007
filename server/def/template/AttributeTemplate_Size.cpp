#include "AttributeTemplate_S.h"

namespace hld
{
    void AttributeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_AttributeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_AttributeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_AttributeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AttributeTemplate].push_back("AttId");
        template_size::get_instance().template_size_map[e_AttributeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AttributeTemplate].push_back("ClientNameString");
        template_size::get_instance().template_size_map[e_AttributeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AttributeTemplate].push_back("IconId");
        template_size::get_instance().template_size_map[e_AttributeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AttributeTemplate].push_back("DecId");
        template_size::get_instance().template_size_map[e_AttributeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AttributeTemplate].push_back("ValueLimit");
        template_size::get_instance().template_size_map[e_AttributeTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_AttributeTemplate].push_back("AttGs");
        template_size::get_instance().template_size_map[e_AttributeTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_AttributeTemplate].push_back("MinValue");
        template_size::get_instance().template_size_map[e_AttributeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AttributeTemplate].push_back("ShowPercent");
    }
}
