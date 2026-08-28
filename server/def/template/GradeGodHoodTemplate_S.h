#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void GradeGodHoodTemplateSize();
#pragma pack(push, 1)
    struct GradeGodHoodTemplate : public template_base
    {
        int32 LevelMax;
        int32 GradeName;
        int32 SmallGradeImageId;
        int32 RewardDescribeImageId;
        std::vector<int32> GradeUpCostMoney;
        std::vector<int32> GradeUpCostItem;
        int32 GradeUpMission;
        std::vector<int32> GradeUpSkills;
        std::vector<float> GradeUpProperty;
        std::vector<int32> GradeUpTalent;
    };
#pragma pack(pop)
}
