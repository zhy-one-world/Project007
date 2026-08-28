#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void DragonTripTemplateSize();
#pragma pack(push, 1)
    struct DragonTripTemplate : public template_base
    {
        int32 Type;
        int32 Pos;
        int32 PosType;
        int32 ItemId;
    };
#pragma pack(pop)
}
