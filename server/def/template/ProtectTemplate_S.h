#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ProtectTemplateSize();
#pragma pack(push, 1)
    struct ProtectTemplate : public template_base
    {
        int32 NameId;
        int32 CurrentOrder;
        int32 CurrentStar;
        int32 NextGradeStartId;
        int32 OperateType;
        std::vector<int32> MoneyConsume;
        int32 ItemID;
        int32 ItemNum;
        float PropertyRaisePercentage;
        int32 AnimBP;
        int32 Mesh;
        std::vector<float> SelfAtt;
        int32 Notice_id;
    };
#pragma pack(pop)
}
