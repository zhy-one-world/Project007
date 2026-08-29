#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void PromptBoxTemplateSize();
#pragma pack(push, 1)
    struct PromptBoxTemplate : public template_base
    {
        int32 ActivityType;
        int32 TipStringID;
        int32 RickStringID;
        int32 ButtonStringID;
        int32 ResID;
        std::vector<int32> GetWayWidgetJumpArray;
        int32 TaskID;
        int32 GoodsID;
        int32 StoreType;
        int32 IsActivityOpen;
        int32 MapId;
    };
#pragma pack(pop)
}
