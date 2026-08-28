#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void CityWarTerritoryTemplateSize();
#pragma pack(push, 1)
    struct CityWarTerritoryTemplate : public template_base
    {
        int32 TerrName;
        int32 TerrImage;
        int32 TerrLevel;
        int32 Score;
        int32 BidMoney;
        int32 BidAddMoney;
        int32 MaintainGlow;
        int32 MaintainGold;
        std::vector<int32> ContainZoneId;
        std::vector<int32> DailyRewards;
        float TerritoryAddition;
        int32 TerritoryBuff;
    };
#pragma pack(pop)
}
