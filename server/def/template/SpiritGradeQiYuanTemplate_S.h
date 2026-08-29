#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void SpiritGradeQiYuanTemplateSize();
#pragma pack(push, 1)
    struct SpiritGradeQiYuanTemplate : public template_base
    {
        int32 TotalGradeNeed;
        std::vector<float> BarserkerAtt;
        std::vector<float> WizardAtt;
        std::vector<float> GuardianAtt;
        std::vector<float> AssassinatorAtt;
    };
#pragma pack(pop)
}
