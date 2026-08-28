#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void PassiveSkillTemplateSize();
#pragma pack(push, 1)
    struct PassiveSkillTemplate : public template_base
    {
        int32 Series;
        int32 Type;
        int32 Level;
        int32 LevelMax;
        int32 Name;
        int32 Icon;
        int32 Describe;
        std::vector<int32> OpenMoney;
        std::vector<int32> OpenItem;
        std::vector<int32> BuffArray;
        std::vector<int32> FeatherArray;
        std::vector<float> AttArray;
        std::vector<float> AttAddArray;
        int32 BuffAddType;
        int32 GSAddValue;
        float ShowCD;
        int32 ObtainDescription;
        int32 WidgetJumpId;
        std::vector<int32> RegionArr;
        int32 OpenTipStr;
        int32 NoOpenTipStr;
        int32 NewGetTip;
        int32 SkillVedio;
    };
#pragma pack(pop)
}
