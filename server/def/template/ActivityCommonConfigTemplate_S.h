#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ActivityCommonConfigTemplateSize();
#pragma pack(push, 1)
    struct ActivityCommonConfigTemplate : public template_base
    {
        int32 ActivityType;
        int32 ActivityName;
        int32 IsOpen;
        int32 LimitLevel;
        std::vector<int32> OpenDate;
        std::vector<int32> ActivityOpenTime;
        std::vector<int32> ActivityOpenTime1;
        std::vector<int32> ActivityOpenTime2;
        int32 ActivityCrossType;
        std::vector<int32> CrossOpenDate;
        std::vector<int32> CrossActivityOpenTime;
        int32 ActivityDuration;
        int32 PromptHoldSecond;
        int32 ParamInt1;
        int32 ParamInt2;
        int32 ParamInt3;
        int32 ParamInt4;
        int32 ParamInt5;
        int32 ParamInt6;
        std::string ParamString1;
        std::string ParamString2;
        std::string ParamString3;
        std::string ParamString4;
        std::string ParamString5;
        std::vector<int32> ParamIntArr1;
        std::vector<int32> ParamIntArr2;
        std::vector<int32> ParamIntArr3;
        std::vector<int32> ParamIntArr4;
        std::vector<int32> ParamIntArr5;
        std::vector<int32> ParamIntArr6;
        std::vector<std::string> ParamStringArr1;
        std::vector<std::string> ParamStringArr2;
        std::vector<std::string> ParamStringArr3;
        std::vector<int32> RankRewards;
        int32 RankRewardsLevel;
        std::vector<int32> LevelRankRewards;
        int32 SpecialReward;
        std::vector<int32> LegionWelfareRewards;
        int32 TopNoticeId;
        int32 ShowTimeArrIdex;
        int32 ShowTipOrder;
    };
#pragma pack(pop)
}
