#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void BreakWillTemplateSize();
#pragma pack(push, 1)
    struct BreakWillTemplate : public template_base
    {
        int32 Type;
        int32 UnLockLevel;
        int32 SkillId;
        std::vector<int32> SubList;
        int32 MainTemId;
        std::vector<int32> NeedItemId;
        int32 NameId;
        int32 DescribeId;
        std::vector<float> AttArray;
    };
#pragma pack(pop)
}
