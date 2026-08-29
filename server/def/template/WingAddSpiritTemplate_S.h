#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void WingAddSpiritTemplateSize();
#pragma pack(push, 1)
    struct WingAddSpiritTemplate : public template_base
    {
        float SpiritNum;
        std::vector<float> AttArray;
        int32 AddSpiritMatirialId;
        int32 AddSpiritMatirialNum;
        std::vector<int32> AddSpiritMoney;
    };
#pragma pack(pop)
}
