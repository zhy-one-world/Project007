#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void WordTemplateSize();
#pragma pack(push, 1)
    struct WordTemplate : public template_base
    {
        std::string SensitiveTextCH;
        std::string SensitiveTextKR;
        std::string SensitiveTextTW;
        std::string SensitiveTextEN;
        std::string SensitiveTextTHA;
        std::string SensitiveTextVN;
        std::string SensitiveTextJP;
        std::string SensitiveTextRus;
        std::string SensitiveTextSG;
    };
#pragma pack(pop)
}
