#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void MoneyTemplateSize();
#pragma pack(push, 1)
    struct MoneyTemplate : public template_base
    {
        int32 MoneyType;
        int32 IconId;
        int32 NameId;
        std::vector<int32> GetWayWidgetJumpArray;
        int32 MissionRequiredId;
        int32 SystemNameId;
        std::vector<int32> GetWayUnlockIdArray;
        int32 ColorStringId;
    };
#pragma pack(pop)
}
