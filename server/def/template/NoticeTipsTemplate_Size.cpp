#include "NoticeTipsTemplate_S.h"

namespace faith
{
    void NoticeTipsTemplateSize()
    {
        template_size::get_instance().template_size_map[e_NoticeTipsTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_NoticeTipsTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_NoticeTipsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NoticeTipsTemplate].push_back("ShowWeight");
        template_size::get_instance().template_size_map[e_NoticeTipsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTipsTemplate].push_back("TextImage");
        template_size::get_instance().template_size_map[e_NoticeTipsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTipsTemplate].push_back("ShowCondition");
        template_size::get_instance().template_size_map[e_NoticeTipsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTipsTemplate].push_back("JumpCondition");
        template_size::get_instance().template_size_map[e_NoticeTipsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NoticeTipsTemplate].push_back("JumpTips");
        template_size::get_instance().template_size_map[e_NoticeTipsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTipsTemplate].push_back("WidgetJumpArray");
    }
}
