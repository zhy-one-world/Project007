#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void MapAwardTemplateSize();
#pragma pack(push, 1)
    struct MapAwardTemplate : public template_base
    {
        int32 DropId;
        std::vector<int32> MoneyList;
    };
#pragma pack(pop)
}
