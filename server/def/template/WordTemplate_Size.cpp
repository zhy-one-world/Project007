#include "WordTemplate_S.h"

namespace hld
{
    void WordTemplateSize()
    {
        template_size::get_instance().template_size_map[e_WordTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_WordTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_WordTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_WordTemplate].push_back("SensitiveTextCH");
        template_size::get_instance().template_size_map[e_WordTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_WordTemplate].push_back("SensitiveTextKR");
        template_size::get_instance().template_size_map[e_WordTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_WordTemplate].push_back("SensitiveTextTW");
        template_size::get_instance().template_size_map[e_WordTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_WordTemplate].push_back("SensitiveTextEN");
        template_size::get_instance().template_size_map[e_WordTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_WordTemplate].push_back("SensitiveTextTHA");
        template_size::get_instance().template_size_map[e_WordTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_WordTemplate].push_back("SensitiveTextVN");
        template_size::get_instance().template_size_map[e_WordTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_WordTemplate].push_back("SensitiveTextJP");
        template_size::get_instance().template_size_map[e_WordTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_WordTemplate].push_back("SensitiveTextRus");
        template_size::get_instance().template_size_map[e_WordTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_WordTemplate].push_back("SensitiveTextSG");
    }
}
