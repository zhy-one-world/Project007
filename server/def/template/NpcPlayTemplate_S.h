#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void NpcPlayTemplateSize();
#pragma pack(push, 1)
    struct NpcPlayTemplate : public template_base
    {
        int32 PlayType;
    };
#pragma pack(pop)
}
