#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void JewelTemplateSize();
#pragma pack(push, 1)
    struct JewelTemplate : public template_base
    {
        int32 JewelType;
        int32 JewelLevel;
        std::vector<int32> JewelValue;
        std::vector<float> JewelAtt;
        int32 ComposeId;
        int32 NextLevelItemId;
    };
#pragma pack(pop)
}
