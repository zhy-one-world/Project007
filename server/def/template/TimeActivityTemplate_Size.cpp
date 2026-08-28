#include "TimeActivityTemplate_S.h"

namespace hld
{
    void TimeActivityTemplateSize()
    {
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("ActivityType");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("ActivityName");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("ActivityIcon");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("ActivityDec");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("ActivityLogic");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("PlayScoreType");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("ScoreItem");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("OpenType");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("NaturalOpenTime");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("NaturalEndTime");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("ServerOpenTime");
        template_size::get_instance().template_size_map[e_TimeActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeActivityTemplate].push_back("CompleteHide");
    }
}
