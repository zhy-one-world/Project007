#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void BrushStrangeTemplateSize();
#pragma pack(push, 1)
    struct BrushStrangeTemplate : public template_base
    {
        std::vector<int32> WaveId;
        std::vector<float> NextTime;
        std::vector<float> LiveTime;
        std::vector<int32> IsDead;
        std::vector<int32> WaveBornTriggerMessage;
        std::vector<int32> WaveKillTriggerMessage;
    };
#pragma pack(pop)
}
