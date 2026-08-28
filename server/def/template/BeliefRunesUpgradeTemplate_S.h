#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void BeliefRunesUpgradeTemplateSize();
#pragma pack(push, 1)
    struct BeliefRunesUpgradeTemplate : public template_base
    {
        std::vector<int32> UpgradeCost;
        std::vector<int32> RecoveryReward;
        int32 Notice_id;
        std::vector<int32> DiamondRecoveryMoney;
        std::vector<int32> DiamondRecoveryCost;
    };
#pragma pack(pop)
}
