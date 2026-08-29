#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void TitleTemplateSize();
#pragma pack(push, 1)
    struct TitleTemplate : public template_base
    {
        int32 HighTitleType;
        int32 EndTimeType;
        int32 HighTitleName;
        int32 HighTitleString;
        int32 HighTitleIcon;
        int32 HighTitleChatIcon;
        std::vector<float> AdditionArray;
        std::vector<float> StaticAttArray;
        int32 LastTime;
        int32 DeadActiveLine;
        int32 NeedValue1;
        int32 NeedValue2;
        int32 Noticeld;
        std::vector<std::string> ChannelLimit;
        int32 PlayerType;
        std::string AniStrId;
    };
#pragma pack(pop)
}
