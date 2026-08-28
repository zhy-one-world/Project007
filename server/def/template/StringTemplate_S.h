#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void StringTemplateSize();
#pragma pack(push, 1)
    struct StringTemplate : public template_base
    {
        int32 StrType;
        std::vector<std::string> StrListCH;
        std::vector<std::string> StrListKR;
        std::vector<std::string> StrListTW;
        std::vector<std::string> StrListEN;
        std::vector<std::string> StrListTHA;
        std::vector<std::string> StrListVN;
        std::vector<std::string> StrListJP;
        std::vector<std::string> StrListRUS;
        std::vector<std::string> StrListSG;
    };
#pragma pack(pop)
}
