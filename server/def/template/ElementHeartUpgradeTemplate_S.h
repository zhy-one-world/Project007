#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ElementHeartUpgradeTemplateSize();
#pragma pack(push, 1)
    struct ElementHeartUpgradeTemplate : public template_base
    {
        std::vector<int32> UpgradeCost;
        std::vector<int32> RecoveryReward;
        int32 Notice_id;
        std::vector<int32> DiamondRecoveryMoney;
        std::vector<int32> DiamondRecoveryCost;
        int32 UpgradeNeedNum;
    };
#pragma pack(pop)
}
