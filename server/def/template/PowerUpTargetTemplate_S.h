#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void PowerUpTargetTemplateSize();
#pragma pack(push, 1)
    struct PowerUpTargetTemplate : public template_base
    {
        std::vector<int32> BasicInfoArray;
        std::vector<int32> WidgetJumpArray;
        int32 PagingType;
        int32 TargetTyp;
        int32 OpenLevel;
        std::string ReqFuncUnlockString;
    };
#pragma pack(pop)
}
