#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void EquipWindowTemplateSize();
#pragma pack(push, 1)
    struct EquipWindowTemplate : public template_base
    {
        int32 ResourceID;
        int32 EquipNameID;
        std::vector<int32> GetEquipStrIDArr;
    };
#pragma pack(pop)
}
