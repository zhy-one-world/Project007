#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void BoxMapTemplateSize();
#pragma pack(push, 1)
    struct BoxMapTemplate : public template_base
    {
        std::vector<int32> MonsterBrushId;
        std::vector<int32> AwardBrushId;
        std::vector<int32> BigAward;
        std::vector<int32> MiddleAward;
        int32 MiddleAwardCount;
        std::vector<int32> MiddleAwardRefrush;
        int32 AwardMaxCount;
        std::vector<int32> MarqueeRandomRange;
        std::vector<int32> MapPlayId;
        int32 TreasureNpcId;
        int32 TreasureMaxCount;
        int32 TreasureDropId;
        int32 TreasureRefrushBoss;
        int32 TreasureMultipleMax;
        std::vector<int32> BossBrushId;
        std::vector<int32> BossFightId;
        int32 SearchRefrushBossValue;
        int32 BoxMonsterId;
        int32 MiddleBoxOpen;
        int32 BigBoxOpen;
        int32 SpecialId;
        std::vector<int32> SpecialBoxRefrush;
        int32 SpecialBoxOpen;
    };
#pragma pack(pop)
}
