#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void AwakenFetterTemplateSize();
#pragma pack(push, 1)
    struct AwakenFetterTemplate : public template_base
    {
        int32 Stage;
        int32 MaxNum;
        int32 Type;
        int32 LogicId;
        int32 Index;
        int32 SubIndex;
        int32 NeedLevel;
        std::vector<int32> NeedItem;
        std::vector<int32> UnLockSkillList;
        std::vector<int32> ResList;
        std::vector<float> AttArray;
        int32 ShowOffset;
        std::vector<float> LocationData;
        std::vector<float> ImageLocationData;
    };
#pragma pack(pop)
}
