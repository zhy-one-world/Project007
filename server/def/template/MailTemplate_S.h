#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void MailTemplateSize();
#pragma pack(push, 1)
    struct MailTemplate : public template_base
    {
        int32 TitleID;
        int32 SenderNameID;
        int32 ContentTextID;
        std::vector<int32> MoneyArray;
        std::vector<int32> ItemArray;
    };
#pragma pack(pop)
}
