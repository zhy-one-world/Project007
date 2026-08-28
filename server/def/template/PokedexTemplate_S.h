#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void PokedexTemplateSize();
#pragma pack(push, 1)
    struct PokedexTemplate : public template_base
    {
        int32 PokedexName;
        int32 PokedexIcon;
        int32 Describe;
        int32 OpenLevel;
        int32 UnLockMissionID;
        int32 NpcID;
        int32 Color;
        int32 Type;
        int32 SubType;
        int32 UpgradeLevel;
        int32 MaxLevel;
        std::vector<int32> UnlockCost;
        std::vector<int32> UpgradeCost;
        std::vector<float> AttArray;
        std::vector<int32> FetterArr;
        std::vector<int32> PsycheIdArray;
        int32 ProtectId;
    };
#pragma pack(pop)
}
