#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void TimeLimitActivityTemplateSize();
#pragma pack(push, 1)
    struct TimeLimitActivityTemplate : public template_base
    {
        int32 ActivityType;
        int32 CycleParamType;
        std::vector<int32> CycleParamNum;
        std::vector<int32> CycleFirstId;
        int32 OpenType;
        int32 OpenServerDate;
        std::vector<int32> OpenSpencificDate;
        int32 FirstOpenDayOfWeek;
        int32 DurationDays;
        int32 CycleDays;
        int32 ShowType;
        std::vector<int32> TextImage;
        std::vector<int32> ParamArr1;
        std::vector<int32> ParamArr2;
        std::vector<int32> ParamArr3;
        std::vector<int32> ParamArr4;
        std::vector<int32> ParamArr5;
    };
#pragma pack(pop)
}
