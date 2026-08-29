#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ServerListTemplateSize();
#pragma pack(push, 1)
    struct ServerListTemplate : public template_base
    {
        int32 GroupId;
        int32 ServerId;
        int32 ServerType;
        int32 ServerName;
        std::string ServerIp;
        int32 ServerPort;
        int32 PortNum;
        int32 IsRecommend;
        std::string BeginTime;
    };
#pragma pack(pop)
}
