#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void OutboardTemplateSize();
#pragma pack(push, 1)
    struct OutboardTemplate : public template_base
    {
        int32 MonsterLevel;
        int32 MonsterId;
        std::vector<int32> ItemShow;
        int32 RecommendationGS;
        int32 ElementalMonsterType;
    };
#pragma pack(pop)
}
