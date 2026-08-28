#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void TalentTemplateSize();
#pragma pack(push, 1)
    struct TalentTemplate : public template_base
    {
        int32 Type;
        int32 Series;
        int32 Level;
        int32 LevelMax;
        int32 Name;
        int32 Icon;
        int32 Describe;
        std::vector<int32> OpenMoney;
        int32 OpenTalentId;
        std::vector<int32> BuffArray;
        std::vector<int32> ReplaceSkill;
        float ReplaceTime;
        std::vector<int32> Active;
        std::vector<int32> NotActive;
        int32 BuffAddType;
        int32 GSAddValue;
        std::vector<int32> OpenrLayerLimit;
    };
#pragma pack(pop)
}
