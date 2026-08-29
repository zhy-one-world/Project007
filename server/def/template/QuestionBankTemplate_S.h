#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void QuestionBankTemplateSize();
#pragma pack(push, 1)
    struct QuestionBankTemplate : public template_base
    {
        int32  topic;
        int32  answer;
        int32  ItemRewardArray;
        std::vector<int32> BattleRewardArray;
    };
#pragma pack(pop)
}
