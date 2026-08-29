#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void VersionTemplateSize();
#pragma pack(push, 1)
    struct VersionTemplate : public template_base
    {
        std::string Version;
    };
#pragma pack(pop)
}
