#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void DialogTemplateSize();
#pragma pack(push, 1)
    struct DialogTemplate : public template_base
    {
        int32 NextDialogId;
        int32 FinnalStepId;
        bool LeftOrRight;
        int32 DialogWordRes;
    };
#pragma pack(pop)
}
