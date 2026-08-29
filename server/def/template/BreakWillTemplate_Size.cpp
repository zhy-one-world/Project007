#include "BreakWillTemplate_S.h"

namespace faith
{
    void BreakWillTemplateSize()
    {
        template_size::get_instance().template_size_map[e_BreakWillTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_BreakWillTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_BreakWillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BreakWillTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_BreakWillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BreakWillTemplate].push_back("UnLockLevel");
        template_size::get_instance().template_size_map[e_BreakWillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BreakWillTemplate].push_back("SkillId");
        template_size::get_instance().template_size_map[e_BreakWillTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BreakWillTemplate].push_back("SubList");
        template_size::get_instance().template_size_map[e_BreakWillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BreakWillTemplate].push_back("MainTemId");
        template_size::get_instance().template_size_map[e_BreakWillTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BreakWillTemplate].push_back("NeedItemId");
        template_size::get_instance().template_size_map[e_BreakWillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BreakWillTemplate].push_back("NameId");
        template_size::get_instance().template_size_map[e_BreakWillTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BreakWillTemplate].push_back("DescribeId");
        template_size::get_instance().template_size_map[e_BreakWillTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BreakWillTemplate].push_back("AttArray");
    }
}
