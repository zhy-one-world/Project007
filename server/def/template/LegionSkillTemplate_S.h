#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void LegionSkillTemplateSize();
#pragma pack(push, 1)
    struct LegionSkillTemplate : public template_base
    {
        int32 UnlockLevel;
        int32 Buff;
        int32 Name;
        int32 Icon;
        int32 Describe;
        std::vector<int32> UpLevelCost1;
        std::vector<int32> UpLevelCost2;
        std::vector<int32> UnlockContribution;
    };
#pragma pack(pop)
}
