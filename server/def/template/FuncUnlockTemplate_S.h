#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void FuncUnlockTemplateSize();
#pragma pack(push, 1)
    struct FuncUnlockTemplate : public template_base
    {
        std::string FuncName;
        int32 FuncShowName;
        int32 Icon;
        int32 Description;
        int32 DetailIcon;
        std::vector<int32> ItemAwardArr;
        int32 IsPreview;
        int32 IsSpecialLogic;
        int32 UnlockNeedMissionID;
        int32 UnlockNeedLevel;
        int32 IsNeedCrossServer;
        int32 UnlockNeedGuideID;
        int32 NeedOpenServerDays;
        std::string PrecondFuncUnlock;
        std::vector<int32> Activate;
        std::vector<int32> ExhibitionNpcIdArray;
        std::vector<int32> ExhibitionNpcBuffArray;
        std::vector<int32> NewExhibitionNpcIdArray;
        int32 TitleDescribeId;
        std::vector<int32> JumpToWidget;
        std::vector<int32> ShieldRegion;
    };
#pragma pack(pop)
}
