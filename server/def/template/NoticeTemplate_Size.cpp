#include "NoticeTemplate_S.h"

namespace hld
{
    void NoticeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("ShowType");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("ContentText");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("LiveTime");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("OpenWidgetArray");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("VisibleType");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("ShowIcon");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("ChangeIconTime");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("RobotRespond");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("AutoRespondIdArray");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("ParseTypeArrray");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("LaunchNoticeBossId");
        template_size::get_instance().template_size_map[e_NoticeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NoticeTemplate].push_back("NoticeActType");
    }
}
