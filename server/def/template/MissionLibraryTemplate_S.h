#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void MissionLibraryTemplateSize();
#pragma pack(push, 1)
    struct MissionLibraryTemplate : public template_base
    {
        int32 MissionLibraryType;
        std::vector<int32> MissionLibraryArray;
        int32 FinishAwardType;
        float FinishEXP;
        float FinishGold;
        std::vector<int32> AllDoneMoney;
        std::vector<int32> AllDoneitem;
        std::vector<int32> AllDoneitemShow;
    };
#pragma pack(pop)
}
