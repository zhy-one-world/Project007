#include "StringTemplate_S.h"

namespace faith
{
    void StringTemplateSize()
    {
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("StrType");
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("StrListCH");
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("StrListKR");
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("StrListTW");
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("StrListEN");
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("StrListTHA");
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("StrListVN");
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("StrListJP");
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("StrListRUS");
        template_size::get_instance().template_size_map[e_StringTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_StringTemplate].push_back("StrListSG");
    }
}
