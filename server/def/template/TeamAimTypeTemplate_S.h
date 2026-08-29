#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void TeamAimTypeTemplateSize();
#pragma pack(push, 1)
    struct TeamAimTypeTemplate : public template_base
    {
        int32 AimType;
        int32 AimGroup;
        int32 AimTypeName;
        int32 AimTypeImage;
        int32 AimTypeValid;
        std::vector<int32> SubTypeId;
        std::vector<int32> SubTypeName;
    };
#pragma pack(pop)
}
