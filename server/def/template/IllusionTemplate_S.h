#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void IllusionTemplateSize();
#pragma pack(push, 1)
    struct IllusionTemplate : public template_base
    {
        std::vector<int32> WarriorItemSet;
        std::vector<int32> MageItemSet;
        std::vector<int32> KnightItemSet;
        std::vector<int32> AssassinItemSet;
        int32 SkillID;
        std::vector<int32> RegionArr;
    };
#pragma pack(pop)
}
