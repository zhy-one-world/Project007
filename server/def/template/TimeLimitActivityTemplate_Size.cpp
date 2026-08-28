#include "TimeLimitActivityTemplate_S.h"

namespace hld
{
    void TimeLimitActivityTemplateSize()
    {
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("ActivityType");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("CycleParamType");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("CycleParamNum");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("CycleFirstId");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("OpenType");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("OpenServerDate");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("OpenSpencificDate");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("FirstOpenDayOfWeek");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("DurationDays");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("CycleDays");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("ShowType");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("TextImage");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("ParamArr1");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("ParamArr2");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("ParamArr3");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("ParamArr4");
        template_size::get_instance().template_size_map[e_TimeLimitActivityTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityTemplate].push_back("ParamArr5");
    }
}
