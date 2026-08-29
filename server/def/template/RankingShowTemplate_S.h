#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void RankingShowTemplateSize();
#pragma pack(push, 1)
    struct RankingShowTemplate : public template_base
    {
        int32 Type;
        int32 SubType;
        int32 ImgId;
        int32 DesStrId;
        int32 IsHidden;
        int32 RankingType;
        int32 RankingBossID;
        std::vector<int32> RankingRewards;
    };
#pragma pack(pop)
}
