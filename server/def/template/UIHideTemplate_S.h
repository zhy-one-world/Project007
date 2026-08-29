#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void UIHideTemplateSize();
#pragma pack(push, 1)
    struct UIHideTemplate : public template_base
    {
        std::string ParentWidgetName;
        std::vector<std::string> ChildWidgetsName;
        std::vector<std::string> ChildWidgetsShowNeedFuncUnlock;
        std::vector<int32> ChildWidgetsHideSetting;
    };
#pragma pack(pop)
}
