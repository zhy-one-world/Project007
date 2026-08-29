#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void MissionTemplateSize();
#pragma pack(push, 1)
    struct MissionTemplate : public template_base
    {
        int32 MissionShowType;
        int32 MissionEndType;
        int32 NeedLevel;
        int32 ShowLevel;
        int32 CurChapterIndex;
        int32 MissionIndexInChapter;
        int32 MissionName;
        int32 MissionDescriptionId;
        int32 GetDialogId;
        int32 NotFinishDialogID;
        int32 FinishDialogId;
        int32 PreMissionId;
        int32 NextMissionId;
        std::vector<int32> GoalNpcKill;
        std::vector<int32> GoalMapName;
        int32 GoalMissionId;
        int32 FinishMissionNpcId;
        int32 FinishMissionNpcMapName;
        int32 MapId;
        std::vector<float> PathFindLocation;
        int32 CanTransfer;
        std::vector<float> FinishMissionNpcLocation;
        std::vector<int32> CreateNpc;
        std::vector<int32> HiddenNpc;
        int32 TransferID;
        std::vector<float> TransferPos;
        int32 MissionEvent;
        int32 FinishAwardType;
        int32 FinishEXP;
        int32 FinishGold;
        int32 FinishItemDropId;
        std::vector<int32> FinishMoney;
        std::vector<int32> ActivateInfoArray;
        int32 IsNeedMissionHint;
        float FinishItemGetRate;
        int32 FinishItemNameId;
        std::vector<int32> SpecialMissionRequirement;
        int32 NextSideMissionTriggerId;
        std::vector<int32> TriggerSideMissionArray;
        int32 SideMissionReqMainMissionId;
        int32 MissionGroupId;
        std::vector<int32> FinishTriggerBuffArray;
        int32 AutoEndDialogTime;
        int32 HighExperienceIdentity;
        int32 MissionSucceedEndAuto;
        int32 ClickIsEndAuto;
        int32 CanShowMissionEnd;
        int32 HimSelfSeeNpcId;
        std::vector<int32> MissionStateEndAuto;
        std::vector<std::string> MissionJumpToWidgetPlayAni;
        std::vector<int32> TemporaryActivateWing;
        int32 IsWingFlyMission;
        std::vector<int32> HimSelfSeeNpcIdArr;
    };
#pragma pack(pop)
}
