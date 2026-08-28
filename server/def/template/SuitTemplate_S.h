#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void SuitTemplateSize();
#pragma pack(push, 1)
    struct SuitTemplate : public template_base
    {
        int32 SuitNameID;
        int32 DisplayRoleImage;
        int32 RoleClassRequired;
        int32 GradeRequired;
        int32 DisplayCheckLevelRequired;
        std::vector<int32> EquipsRequiredArray;
        std::vector<float> AttArray;
        int32 DisplayUpgradeNum;
        int32 DisplayAddonNum;
        int32 SuitGetAreaStringID;
        std::vector<int32> SuitGetWidgetJumpArray;
    };
#pragma pack(pop)
}
