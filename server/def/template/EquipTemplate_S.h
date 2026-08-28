#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void EquipTemplateSize();
#pragma pack(push, 1)
    struct EquipTemplate : public template_base
    {
        int32 EquipSlot;
        int32 equip_addon_num;
        int32 equip_enchant_num;
        int32 ExcellentShowGs;
        std::vector<float> AttArray;
        std::vector<float> EquipAddonAtt;
        int32 AddOnTemplateId;
        int32 SuccinctTemplateID;
        std::vector<int32> GodAssembledSpend;
        float GodAssembledRadio;
        int32 Smelt_limit;
        int32 MaterialID;
        int32 MaterialCost;
        std::vector<int32> LockMaterialID;
        std::vector<int32> LockMaterialCost;
        std::vector<int32> ForgeExcellentAttQualityWeight;
        int32 SuitType;
        std::vector<int32> SuitAttArr;
        std::vector<int32> StarBuffId;
        int32 JewelCarveID;
    };
#pragma pack(pop)
}
