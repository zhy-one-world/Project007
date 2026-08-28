#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void WorldTemplateSize();
#pragma pack(push, 1)
    struct WorldTemplate : public template_base
    {
        std::vector<int32> CsWorldMap;
    };
#pragma pack(pop)
}
