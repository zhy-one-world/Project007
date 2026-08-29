#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void FasionTemplateSize();
#pragma pack(push, 1)
    struct FasionTemplate : public template_base
    {
        int32 EquipSlot;
        std::vector<float> AttArray;
        int32 MeshID;
        std::vector<int32> BuffIdArray;
        std::vector<int32> FeaturesDes;
        std::vector<int32> UpgradeMaterial;
        int32 upgrade_num;
        int32 GradeLimit;
        std::vector<int32> RegionArr;
        std::vector<int32> NeedFasionId;
    };
#pragma pack(pop)
}
