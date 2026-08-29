#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void LadderTemplateSize();
#pragma pack(push, 1)
    struct LadderTemplate : public template_base
    {
        int32 Region;
        int32 SeasonNum;
        int32 RankName;
        int32 RankIcon;
        int32 LowScore;
        int32 HighScore;
        int32 WinScore;
        int32 LoseScore;
        int32 PeaceScore;
        std::vector<int32> WinReward;
        std::vector<int32> LoseReward;
        std::vector<int32> PeaceReward;
        std::vector<int32> SeasonReward;
        int32 ReservedScore;
        int32 BigGradeFlag;
        int32 ExpandTime;
        int32 UpgradeNotify;
        std::vector<int32> UpgradeReward;
        int32 TitleReward;
        int32 RobotRate;
        std::vector<int32> RobotTemplateID;
    };
#pragma pack(pop)
}
