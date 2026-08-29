#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void StopPlaceTemplateSize();
#pragma pack(push, 1)
    struct StopPlaceTemplate : public template_base
    {
        std::vector<int32> StayPoint;
    };
#pragma pack(pop)
}
