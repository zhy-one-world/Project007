#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void SpecialNameTemplateSize();
#pragma pack(push, 1)
    struct SpecialNameTemplate : public template_base
    {
        int32 NameType;
        int32 NameLevel;
        int32 NameQulity;
        int32 NameString;
        int32 NameIcon;
        int32 NameStar;
        std::vector<int32> UpgradeCost;
        int32 NextLevelId;
        std::vector<float> NameAttribute;
        int32 IconRes;
        int32 IsShow;
        int32 BuffLogicId;
        int32 NoticeId;
    };
#pragma pack(pop)
}
