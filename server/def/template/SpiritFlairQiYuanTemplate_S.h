#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void SpiritFlairQiYuanTemplateSize();
#pragma pack(push, 1)
    struct SpiritFlairQiYuanTemplate : public template_base
    {
        int32 TotalFlairNeed;
        std::vector<float> BarserkerAtt;
        std::vector<float> WizardAtt;
        std::vector<float> GuardianAtt;
        std::vector<float> AssassinatorAtt;
    };
#pragma pack(pop)
}
