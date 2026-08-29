#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void BeliefRunesTemplateSize();
#pragma pack(push, 1)
    struct BeliefRunesTemplate : public template_base
    {
        int32 quality_level;
        int32 AttType;
        int32 MaxUpgradeLevel;
        int32 AttNum;
        std::vector<float> BasicAtt1;
        std::vector<float> BasicAtt2;
        std::vector<float> BasicAtt3;
        std::vector<float> UpgradeAttEach1;
        std::vector<float> UpgradeAttEach2;
        std::vector<float> UpgradeAttEach3;
        int32 UpgradeId;
        std::vector<int32> BuffIdArray;
        int32 ItemTemplateId;
    };
#pragma pack(pop)
}
