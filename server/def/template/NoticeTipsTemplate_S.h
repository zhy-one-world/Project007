#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void NoticeTipsTemplateSize();
#pragma pack(push, 1)
    struct NoticeTipsTemplate : public template_base
    {
        int32 ShowWeight;
        std::vector<int32> TextImage;
        std::vector<int32> ShowCondition;
        std::vector<int32> JumpCondition;
        int32 JumpTips;
        std::vector<int32> WidgetJumpArray;
    };
#pragma pack(pop)
}
