#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void JewelCarveTemplateSize();
#pragma pack(push, 1)
    struct JewelCarveTemplate : public template_base
    {
        int32 EquipSlot;
        std::vector<int32> Consume;
        std::vector<int32> RandomSkillSet;
        int32 ShowSkillName;
        int32 ShowSkillDescribe;
        int32 Type;
        int32 SubType;
        int32 SkillID;
        int32 RealType;
        std::vector<int32> RealValue;
        int32 ResId;
    };
#pragma pack(pop)
}
