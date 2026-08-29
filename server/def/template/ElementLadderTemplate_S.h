#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ElementLadderTemplateSize();
#pragma pack(push, 1)
    struct ElementLadderTemplate : public template_base
    {
        int32 RegionType;
        int32 SeasonNum;
        int32 RankName;
        int32 RankIcon;
        int32 LowScore;
        int32 HighScore;
        std::vector<int32> SeasonReward;
        int32 ReservedScore;
        int32 BigGradeFlag;
        int32 ExpandTime;
        std::vector<int32> UpgradeReward;
        int32 TitleReward;
        int32 RobotRate;
        std::vector<int32> RobotTemplateID;
    };
#pragma pack(pop)
}
