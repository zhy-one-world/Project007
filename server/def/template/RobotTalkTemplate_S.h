#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void RobotTalkTemplateSize();
#pragma pack(push, 1)
    struct RobotTalkTemplate : public template_base
    {
        int32 TalkType;
        int32 Talklimit;
        std::vector<int32> InitiativeTalk;
        std::vector<int32> PassiveTalk;
        std::vector<int32> FrontActivityTalk;
        std::vector<int32> BackActivityTalk;
    };
#pragma pack(pop)
}
