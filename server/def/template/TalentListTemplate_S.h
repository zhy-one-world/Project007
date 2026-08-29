#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void TalentListTemplateSize();
#pragma pack(push, 1)
    struct TalentListTemplate : public template_base
    {
        int32 Type;
        int32 ClassType;
        std::vector<int32> Layer1;
        std::vector<int32> Layer2;
        std::vector<int32> Layer3;
        std::vector<int32> Layer4;
        std::vector<int32> Layer5;
        std::vector<int32> Layer6;
        std::vector<int32> Layer7;
        std::vector<int32> Layer8;
        std::vector<int32> Layer9;
        std::vector<int32> Layer10;
    };
#pragma pack(pop)
}
