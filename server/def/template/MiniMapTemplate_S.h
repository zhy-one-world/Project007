#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void MiniMapTemplateSize();
#pragma pack(push, 1)
    struct MiniMapTemplate : public template_base
    {
        int32 MiniMapNameResId;
        int32 MiniMapResId;
        int32 MiniMapImageStartX;
        int32 MiniMapImageStartY;
        int32 MiniMapRealStartX;
        int32 MiniMapRealStartY;
        int32 MiniMapImageLenthX;
        int32 MiniMapImageLenthY;
        int32 MiniMapRealLenthX;
        int32 MiniMapRealLenthY;
        std::vector<int32> MiniMapBorderPos;
        std::vector<int32> MiniMarkIdList;
        std::vector<int32> MiniDropItemIdList;
    };
#pragma pack(pop)
}
