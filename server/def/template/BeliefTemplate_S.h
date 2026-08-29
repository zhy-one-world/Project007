#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void BeliefTemplateSize();
#pragma pack(push, 1)
    struct BeliefTemplate : public template_base
    {
        int32 BeliefType;
        int32 ActivateLevelRequire;
        int32 CurStarNumber;
        int32 CurRankNumber;
        int32 MaxStarNumber;
        int32 MaxRankNumber;
        std::vector<float> AttArray;
        int32 OperateType;
        float SuccessRatio;
        int32 MinActivateTimeNeed;
        int32 MaxActivateTimeLimit;
        std::vector<int32> MoneyCostList;
        int32 Notice_id;
    };
#pragma pack(pop)
}
