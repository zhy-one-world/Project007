#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void PlayAddActivityTemplateSize();
#pragma pack(push, 1)
    struct PlayAddActivityTemplate : public template_base
    {
        int32 PlayType;
        int32 AddScore;
        int32 AddProbability;
    };
#pragma pack(pop)
}
