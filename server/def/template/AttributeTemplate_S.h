#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void AttributeTemplateSize();
#pragma pack(push, 1)
    struct AttributeTemplate : public template_base
    {
        int32 AttId;
        int32 ClientNameString;
        int32 IconId;
        int32 DecId;
        int32 ValueLimit;
        float AttGs;
        float MinValue;
        int32 ShowPercent;
    };
#pragma pack(pop)
}
