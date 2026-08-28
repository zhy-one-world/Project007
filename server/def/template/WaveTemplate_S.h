#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void WaveTemplateSize();
#pragma pack(push, 1)
    struct WaveTemplate : public template_base
    {
        std::vector<int32> MiniMapShowNpcId;
        std::vector<int32> NpcId;
        std::vector<int32> BornX;
        std::vector<int32> BornY;
        std::vector<int32> BornZ;
        std::vector<int32> BornYaw;
        std::vector<int32> BornPitch;
        std::vector<int32> BornRoll;
        std::vector<float> BornTime;
        std::vector<int32> PlotTips;
        std::vector<int32> FirstPlotTips;
    };
#pragma pack(pop)
}
