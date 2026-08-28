#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void GuideTriggerTemplateSize();
#pragma pack(push, 1)
    struct GuideTriggerTemplate : public template_base
    {
        int32 TriggerGuideID;
        int32 NeedLevel;
        std::vector<int32> NeedMission;
        std::vector<int32> MustNeedMission;
        std::string NeedFuncUnlock;
        int32 NewFuncUnlockFuncId;
        int32 LandWelfareId;
        int32 IsHitFirstGuide;
        std::vector<int32> NeedItem;
        std::vector<int32> NeedItemType;
        std::vector<int32> NeedMoney;
        int32 RoleState;
        std::string OpenWidget;
        int32 FirstTimeDo;
        int32 NeedGodHoodGrade;
        int32 NeedTinderOrder;
        std::string NeedShowWidgetName;
        int32 NeedFlyMaxHedit;
        int32 DeadCanGuide;
        int32 NeedMapID;
        int32 NeedMapState;
        int32 NewGudieNeedRePlay;
        int32 EndGuideNeedOpenMp;
        int32 OpenGuideNoOpenMp;
        int32 TriggerInPublicMap;
        int32 ResetSkillType;
        int32 KeepWidget;
        int32 is_accord;
        std::vector<int32> LoginCheckGuideMission;
    };
#pragma pack(pop)
}
