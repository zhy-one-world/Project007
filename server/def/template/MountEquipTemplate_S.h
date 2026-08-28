#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void MountEquipTemplateSize();
#pragma pack(push, 1)
    struct MountEquipTemplate : public template_base
    {
        int32 EquipSlot;
        int32 StarNum;
        int32 StarMax;
        std::vector<float> UpgradeAttEach;
        std::vector<int32> StarCost;
        int32 IsOnly;
        int32 Mount;
        std::vector<int32> StarMountLevel;
        int32 SkillType;
        int32 SkillId;
        std::vector<int32> SkillMountLevel;
        std::vector<int32> SlotID;
        std::vector<float> BaseAtt;
        std::vector<int32> CanEquipID;
        std::vector<int32> ItemGet;
    };
#pragma pack(pop)
}
