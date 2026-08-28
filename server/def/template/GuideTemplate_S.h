#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void GuideTemplateSize();
#pragma pack(push, 1)
    struct GuideTemplate : public template_base
    {
        int32 GuideType;
        int32 NextGuideID;
        int32 TriggerNextGuideType;
        float LiveTime;
        int32 OperateType;
        std::vector<int32> NeedMoveDistance;
        std::vector<int32> NeedRotateAngle;
        int32 TargetPointType;
        std::vector<float> TargetPointPos;
        std::vector<int32> MovementState;
        std::string MaskBtn;
        std::vector<std::string> MaskBtnArray;
        int32 NeedShieldBlackCloth;
        int32 IsHaveBorder;
        int32 IsHaveArrow;
        int32 ArrowPosX;
        int32 ArrowPosY;
        int32 ArrowRotation;
        int32 IsFlip;
        int32 IsHaveGuideText;
        int32 RoleHeadId;
        int32 GuideText;
        float GuideTextPosX;
        float GuideTextPosY;
        std::string FuncName;
        int32 FuctionTitle;
        int32 FuctionWord;
        int32 FuctionImage;
        int32 OpenSkillType;
        int32 AnimPlayCount;
        std::string AnimID;
        int32 AnimAdpapter;
        std::string CinematicAnimationLevelScene;
        std::vector<int32> CinematicAnimationPosArr;
        std::vector<std::string> HideWidgetTag;
        int32 IsJumpGuide;
        int32 IgnoreUserInput;
        int32 IgnoreUIInput;
        int32 TopJoyStick;
        int32 HiddenOtherPlayer;
        int32 CloseAutoMission;
        int32 OpenAutoMission;
        int32 NeedUpdateUI;
        int32 SpecialDoType;
        int32 CanEnd;
        std::vector<float> CameraCfg;
        float AutoNextLostTime;
        int32 GuideSoundId;
        int32 GuideEndSoundIsNotCLose;
        int32 GuideShineFrameAdjust;
        std::string GuideOpenUI;
        int32 BeginNeedShowOperate;
        int32 NeedShowOperate;
        std::vector<int32> TargetLocation;
        int32 OnMount;
        std::string GuideEndOpenUi;
        std::vector<int32> NotPassCloseGuide;
        std::vector<int32> CloseGuideData;
        std::string BroadcastEvent;
        int32 NeedShowFaishon;
    };
#pragma pack(pop)
}
