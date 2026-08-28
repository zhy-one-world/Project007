#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void WelfareTemplateSize();
#pragma pack(push, 1)
    struct WelfareTemplate : public template_base
    {
        int32 Type;
        int32 TargetValue;
        int32 Index;
        int32 NoticeId;
        std::vector<int32> RewardMoneyArray;
        std::vector<int32> RewardItemArray;
        std::vector<int32> AdditionalMoneyRewardID;
        std::vector<int32> AdditionalItemRewardID;
        std::vector<int32> RewardDropID;
        std::vector<int32> ShowedItemArray;
        std::vector<int32> ExhibitionNpcIdArray;
        std::vector<int32> ExhibitionNpcBuffArray;
        int32 VipDoubleRank;
        int32 DesTextId;
        std::vector<int32> NewExhibitionNpcIdArray;
        std::vector<int32> ReceiveTime;
        std::vector<int32> PackageIcon;
    };
#pragma pack(pop)
}
