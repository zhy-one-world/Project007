#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void QuestionMarryTemplateSize();
#pragma pack(push, 1)
    struct QuestionMarryTemplate : public template_base
    {
        int32 topic;
        std::vector<int32> answer;
        int32 RightAnswer;
        std::vector<int32> ItemRewardArray;
        std::vector<int32> BattleRewardArray;
        std::vector<int32> ItemRewardArray1;
        std::vector<int32> BattleRewardArray1;
    };
#pragma pack(pop)
}
