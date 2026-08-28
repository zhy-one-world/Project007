#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void LuaMessageTemplateSize();
#pragma pack(push, 1)
    struct LuaMessageTemplate : public template_base
    {
        int32 UseLua;
    };
#pragma pack(pop)
}
