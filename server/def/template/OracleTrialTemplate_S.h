#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void OracleTrialTemplateSize();
#pragma pack(push, 1)
    struct OracleTrialTemplate : public template_base
    {
        std::vector<int32> MapId;
        std::vector<int32> AwardNum;
        std::vector<int32> AwardId;
    };
#pragma pack(pop)
}
