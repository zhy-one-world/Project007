#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void PreloadCacheTemplateSize();
#pragma pack(push, 1)
    struct PreloadCacheTemplate : public template_base
    {
        std::string ResName;
    };
#pragma pack(pop)
}
