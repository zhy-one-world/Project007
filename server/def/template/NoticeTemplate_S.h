#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void NoticeTemplateSize();
#pragma pack(push, 1)
    struct NoticeTemplate : public template_base
    {
        std::vector<int32> ShowType;
        std::vector<int32> ContentText;
        int32 LiveTime;
        std::vector<int32> OpenWidgetArray;
        int32 VisibleType;
        std::vector<int32> ShowIcon;
        float ChangeIconTime;
        std::vector<int32> RobotRespond;
        std::vector<int32> AutoRespondIdArray;
        std::vector<int32> ParseTypeArrray;
        int32 LaunchNoticeBossId;
        int32 NoticeActType;
    };
#pragma pack(pop)
}
