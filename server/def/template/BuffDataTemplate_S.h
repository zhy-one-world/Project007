#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void BuffDataTemplateSize();
#pragma pack(push, 1)
    struct BuffDataTemplate : public template_base
    {
        int32 Type;
        int32 Series;
        int32 Race;
        int32 Level;
        std::vector<int32> BeginActions;
        std::vector<int32> EndActions;
        std::vector<int32> EffectActions;
        std::vector<int32> CycActions;
        std::vector<int32> EventCondition;
        std::vector<int32> EventActions;
        float DuringTime;
        float JumpTime;
        float BuffPercent;
        int32 CanCannel;
        int32 ExistWhenDeath;
        int32 DelByResetSkill;
        int32 TimeEndKeep;
        int32 MaxPileDepth;
        int32 SameLevelCover;
        int32 DiffLevelCover ;
        int32 SameLevelSameSenderWrap ;
        int32 SameLevelDifSenderWrap;
        int32 SameLevelTimeCumulative;
        int32 DiffevelTimeCumulative;
        int32 SaveType;
        int32 Sync;
        int32 OtherPlayerCanSee;
        int32 IsShowInExhibition;
        int32 ResourceID	;
        int32 DisplayPriority;
        std::vector<int32> InvalidMapTypeArray;
        std::vector<int32> ValidMapTypeArray;
        int32 IsShowAbNormalBuff;
        int32 MapEndClear;
        int32 InvalidBoss;
        int32 NotInvalidFakerPlayer;
        int32 IsFashionBuff;
        int32 MountStatusHidden;
        int32 NeedRefresh;
    };
#pragma pack(pop)
}
