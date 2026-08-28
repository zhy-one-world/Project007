#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void TimeActivityTemplateSize();
#pragma pack(push, 1)
    struct TimeActivityTemplate : public template_base
    {
        int32 ActivityType;
        int32 ActivityName;
        int32 ActivityIcon;
        int32 ActivityDec;
        std::vector<int32> ActivityLogic;
        std::vector<int32> PlayScoreType;
        int32 ScoreItem;
        int32 OpenType;
        std::string NaturalOpenTime;
        std::string NaturalEndTime;
        int32 ServerOpenTime;
        int32 CompleteHide;
    };
#pragma pack(pop)
}
