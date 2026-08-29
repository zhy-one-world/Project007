#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void BuffActionTemplateSize();
#pragma pack(push, 1)
    struct BuffActionTemplate : public template_base
    {
        std::string Action;
        std::vector<int32> ActionParams;
        std::vector<float> AttArray;
        int32 Condition;
        std::vector<int32> ReplaceSkill;
    };
#pragma pack(pop)
}
