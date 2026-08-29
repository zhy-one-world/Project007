#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void WeddingRingTemplateSize();
#pragma pack(push, 1)
    struct WeddingRingTemplate : public template_base
    {
        int32 Name;
        int32 ObjectID;
        int32 RingUpgradeMax;
        int32 RingAddStarMax;
        int32 UpgradeNow;
        int32 StarNow;
        std::vector<float> AttArray;
        float mateGetAttFloat;
        std::vector<int32> AddStarMatirialId;
        int32 AddStarMaxEXP;
        int32 UpgradeMatirialId;
        int32 UpgradeMatirialNum;
        std::vector<int32> InitLevel;
        int32 InitLevel2;
    };
#pragma pack(pop)
}
