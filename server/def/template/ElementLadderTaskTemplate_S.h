#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void ElementLadderTaskTemplateSize();
#pragma pack(push, 1)
    struct ElementLadderTaskTemplate : public template_base
    {
        int32 TaskType;
        int32 TaskText;
        int32 Value;
        std::vector<int32> Reward;
    };
#pragma pack(pop)
}
