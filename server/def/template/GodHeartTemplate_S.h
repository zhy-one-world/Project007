#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void GodHeartTemplateSize();
#pragma pack(push, 1)
    struct GodHeartTemplate : public template_base
    {
        int32 ItemType;
        int32 MaxUpgradeLevel;
        std::vector<float> BasicAtt;
        std::vector<float> UpgradeAttEach;
        int32 UpgradeId;
        int32 ItemTemplateId;
        std::vector<int32> StarBuffId;
        int32 Way;
        std::vector<int32> ChangeLevel;
        std::vector<int32> StarBuffIdShow;
    };
#pragma pack(pop)
}
