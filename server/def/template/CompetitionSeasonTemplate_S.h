#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void CompetitionSeasonTemplateSize();
#pragma pack(push, 1)
    struct CompetitionSeasonTemplate : public template_base
    {
        int32 RightAnswer;
        int32 Region;
        int32 SeasonNum;
        int32 Level;
        std::vector<int32> NormalRewardArray;
        std::vector<int32> SpecialRewardArray;
        int32 exp;
        int32 BigRewardTag;
    };
#pragma pack(pop)
}
