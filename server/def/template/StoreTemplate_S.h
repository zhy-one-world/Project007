#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void StoreTemplateSize();
#pragma pack(push, 1)
    struct StoreTemplate : public template_base
    {
        int32 StoreType;
        std::vector<int32> StoreGoods;
        int32 LevelLimit;
        int32 OpenType;
        std::vector<int32> StoreOpenTimeLimit;
        int32 LastTime;
        int32 FreshType;
        std::vector<int32> FreshTime;
        int32 FreshInterval;
        int32 FirstLabelIndex;
        std::vector<int32> ExtraMoneyShowArray;
        int32 StoreNameId;
    };
#pragma pack(pop)
}
