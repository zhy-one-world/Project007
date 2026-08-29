#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void GodHeartUpgradeTemplateSize();
#pragma pack(push, 1)
    struct GodHeartUpgradeTemplate : public template_base
    {
        std::vector<int32> UpgradeCost;
        std::vector<int32> RecoveryReward;
        std::vector<int32> DiamondRecoveryMoney;
        std::vector<int32> DiamondRecoveryCost;
    };
#pragma pack(pop)
}
