#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void DevelopOpenTemplateSize();
#pragma pack(push, 1)
    struct DevelopOpenTemplate : public template_base
    {
        int32 Type;
        int32 OpenLevel;
        int32 NeedRoleLevel;
        int32 NeedWorldLevel;
        int32 NeedMission;
    };
#pragma pack(pop)
}
