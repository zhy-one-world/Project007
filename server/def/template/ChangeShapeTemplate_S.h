#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void ChangeShapeTemplateSize();
#pragma pack(push, 1)
    struct ChangeShapeTemplate : public template_base
    {
        float ChangeTime;
        std::vector<int32> UnitRes;
        int32 ChangeNpcId;
        std::vector<int32> SkillSeriesList;
        std::vector<int32> BeginBuffList;
        std::vector<int32> EndBuffList;
    };
#pragma pack(pop)
}
