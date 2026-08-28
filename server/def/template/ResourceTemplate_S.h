#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void ResourceTemplateSize();
#pragma pack(push, 1)
    struct ResourceTemplate : public template_base
    {
        int32 ResourceType;
        std::string ResourcePath;
        int32 ResourcePriority;
        int32 IsUnchangeByBuff;
    };
#pragma pack(pop)
}
