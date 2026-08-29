#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void SpiritJiBanTemplateSize();
#pragma pack(push, 1)
    struct SpiritJiBanTemplate : public template_base
    {
        int32 Type;
        int32 Level;
        int32 BeginId;
        std::vector<float> Property;
        std::vector<int32> SpriteReqIdArray;
        std::vector<int32> PassiveSkillId;
        int32 StrId;
    };
#pragma pack(pop)
}
