#include "CompositTemplate_S.h"

namespace faith
{
    void CompositTemplateSize()
    {
        template_size::get_instance().template_size_map[e_CompositTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_CompositTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_CompositTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CompositTemplate].push_back("TypeStringID");
        template_size::get_instance().template_size_map[e_CompositTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CompositTemplate].push_back("CompositItemTemplateID");
        template_size::get_instance().template_size_map[e_CompositTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_CompositTemplate].push_back("MaterialList");
        template_size::get_instance().template_size_map[e_CompositTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_CompositTemplate].push_back("CostMoney");
        template_size::get_instance().template_size_map[e_CompositTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_CompositTemplate].push_back("Rate");
        template_size::get_instance().template_size_map[e_CompositTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CompositTemplate].push_back("noticeID");
        template_size::get_instance().template_size_map[e_CompositTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_CompositTemplate].push_back("RegionArr");
    }
}
