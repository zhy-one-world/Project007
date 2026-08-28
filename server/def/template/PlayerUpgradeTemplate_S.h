#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void PlayerUpgradeTemplateSize();
#pragma pack(push, 1)
    struct PlayerUpgradeTemplate : public template_base
    {
        int32 ClassType;
        int32 Level;
        std::string NextLevelExp;
        int32 MoveSpeedForce;
        int32 MaxBagCount;
        int32 EnergyBackValue;
        int32 EnergyBackTime;
        int32 EnergyMax;
        std::vector<int32> RandomItem;
        std::vector<int32> SpecialRandomItem;
        std::vector<int32> RandomTreasure;
        std::vector<float> AttPermanent;
        int32 SkillCount;
        int32 PassiveSkillCount;
        int32 SpriteCount;
        std::vector<float> AttArray;
        int32 BaseExp;
        int32 BaseCoin;
        int32 DailyMissionLibrary;
        int32 AgainstMissionLibrary;
        int32 TargetGsValue;
        int32 TalentNum;
        int32 AddPoints;
        float MeditationExpAddition;
        int32 MeditationStandardGs;
        std::vector<float> PureHurtAttArray;
        int32 MarryMissionLibrary;
        int32 ReplyEnergy;
    };
#pragma pack(pop)
}
