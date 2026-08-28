#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void ResultsPreviewTemplateSize();
#pragma pack(push, 1)
    struct ResultsPreviewTemplate : public template_base
    {
        int32 Type;
        int32 CloseStr;
        int32 LogoResId;
        int32 StrId;
        std::vector<int32> NeedLevel;
        std::vector<int32> LevelArray;
        std::vector<int32> ColorArray;
        std::vector<int32> EquipStrArray;
        std::vector<int32> ArmasStrArray;
        std::vector<int32> TitleStr;
        std::vector<int32> DesStr;
    };
#pragma pack(pop)
}
