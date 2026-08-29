#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void SearchLevelTemplateSize();
#pragma pack(push, 1)
    struct SearchLevelTemplate : public template_base
    {
        int32 CurText;
        std::vector<float> CurItem;
        std::vector<int32> ColorPercent;
        std::vector<int32> AlreadyItem;
        std::vector<int32> Shall;
        std::vector<int32> UpgradeMoney;
        int32 SearchExp;
    };
#pragma pack(pop)
}
