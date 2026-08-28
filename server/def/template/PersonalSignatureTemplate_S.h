#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void PersonalSignatureTemplateSize();
#pragma pack(push, 1)
    struct PersonalSignatureTemplate : public template_base
    {
        int32 Type;
        int32 Sort;
        int32 StringID;
        std::vector<int32> answer;
    };
#pragma pack(pop)
}
