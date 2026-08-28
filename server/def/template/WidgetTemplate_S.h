#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void WidgetTemplateSize();
#pragma pack(push, 1)
    struct WidgetTemplate : public template_base
    {
        std::string WidgetName;
        std::string WidgetRes;
        int32 ShowMode;
        int32 IsGcWhenClose;
        int32 IsShowJoyStick;
        int32 IsCacheWhenClose;
        int32 FuncUnlockReqId;
        int32 CloseSelfWhenTriggerGuide;
        int32 Zorder;
        int32 IsCloseOther;
        int32 IsMainWidget;
        std::string ExceptWidgetWhenCloseOther;
        int32 AdapterScale;
        std::vector<std::string> LeftAdapterWidgetTagArr;
        std::vector<int32> LeftAdapterWidgetOffsetArr;
        std::vector<std::string> RightAdapterWidgetTagArr;
        std::vector<int32> RightAdapterWidgetOffsetArr;
        int32 IsNeedResetJoyStick;
        int32 IsResponseBackBtn;
    };
#pragma pack(pop)
}
