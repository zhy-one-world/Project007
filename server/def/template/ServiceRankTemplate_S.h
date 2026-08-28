#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void ServiceRankTemplateSize();
#pragma pack(push, 1)
    struct ServiceRankTemplate : public template_base
    {
        int32 Type;
        int32 RankingType;
        std::vector<int32> RankingSubType;
        int32 EndTime;
        int32 GetRewardEndTime;
        int32 MinValue;
        std::vector<int32> RankNum;
        std::vector<int32> RankImgArray;
        std::vector<int32> ItemRewardOne;
        std::vector<int32> ItemRewardTwo;
        std::vector<int32> ItemRewardThree;
        std::vector<int32> ItemRewardFour;
        std::vector<int32> ItemRewardFive;
        std::vector<int32> ShowTextStrArr;
        std::vector<int32> GetWayWidgetJumpArray1;
        std::vector<int32> GetWayUnlockIdArray1;
        std::vector<int32> GetWayWidgetJumpArray2;
        std::vector<int32> GetWayUnlockIdArray2;
        std::vector<int32> GetWayWidgetJumpArray3;
        std::vector<int32> GetWayUnlockIdArray3;
        std::vector<int32> GetWayWidgetJumpArray4;
        std::vector<int32> GetWayUnlockIdArray4;
        int32 RankingShowImg;
        std::vector<int32> ModelTemplateIdArray;
        std::vector<int32> RankIndexAndValueText;
    };
#pragma pack(pop)
}
