#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void RoleRewardTemplateSize();
#pragma pack(push, 1)
    struct RoleRewardTemplate : public template_base
    {
        int32 ServerId;
        std::string RoleId;
        std::vector<int32> MailItem;
    };
#pragma pack(pop)
}
