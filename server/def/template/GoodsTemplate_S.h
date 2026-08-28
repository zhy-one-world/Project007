#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void GoodsTemplateSize();
#pragma pack(push, 1)
    struct GoodsTemplate : public template_base
    {
        int32 ItemId;
        int32 GoodsNum;
        int32 ClassLimit;
        std::vector<int32> Levellimit;
        int32 NeedLegionLevel;
        int32 NeedContribution;
        int32 IsLock;
        int32 TimelimitOnType;
        std::vector<int32> TimelimitOnTime;
        std::vector<int32> DurationTime;
        std::vector<int32> AddTime;
        std::vector<int32> RemoveTime;
        std::vector<int32> OldPrice;
        std::vector<int32> NewPrice;
        std::vector<int32> NeedItemId;
        int32 LimitType;
        int32 RoleMaxNum;
        int32  EffectiveTime;
        int32 NeedVIPLevel;
        std::vector<std::string> ChannelLimit;
    };
#pragma pack(pop)
}
