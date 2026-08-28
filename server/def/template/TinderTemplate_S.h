#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void TinderTemplateSize();
#pragma pack(push, 1)
    struct TinderTemplate : public template_base
    {
        int32 NameId;
        int32 CurrentCanto;
        int32 CurrentOrder;
        int32 CurrentStar;
        std::vector<int32> MoneyConsume;
        int32 NeedChapter;
        std::vector<float> AddAtt;
        int32 Mesh;
        int32 AnimBP;
        std::vector<int32> UnlockStateBuffId;
        int32 IconID;
        int32 TinderShotDescribe;
        int32 TinderUnlockClue;
        int32 AddStarLevel;
    };
#pragma pack(pop)
}
