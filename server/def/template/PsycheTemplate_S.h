#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void PsycheTemplateSize();
#pragma pack(push, 1)
    struct PsycheTemplate : public template_base
    {
        int32 Describe;
        std::vector<float> FetterAttArray;
        std::vector<int32> FetterActivateNeedArray;
        int32 Psyche;
        int32 PsycheNum;
        std::vector<float> AttArray;
        int32 SpawnNpcId;
        int32 ProtectPoint;
        int32 PokedexTemplateId;
        std::vector<int32> WidgetJumpArray;
    };
#pragma pack(pop)
}
