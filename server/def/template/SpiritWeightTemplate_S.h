#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void SpiritWeightTemplateSize();
#pragma pack(push, 1)
    struct SpiritWeightTemplate : public template_base
    {
        int32 SpiritTemplateID;
        int32 Weight;
    };
#pragma pack(pop)
}
