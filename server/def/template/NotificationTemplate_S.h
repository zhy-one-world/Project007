#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void NotificationTemplateSize();
#pragma pack(push, 1)
    struct NotificationTemplate : public template_base
    {
        int32 NotificationType;
        std::vector<int32> WeekNum;
        std::vector<int32> DayTime;
        std::vector<int32> ActivityTime;
        int32 OfflineTime;
        int32 Title;
        int32 Body;
        std::string Action;
        int32 Event;
    };
#pragma pack(pop)
}
