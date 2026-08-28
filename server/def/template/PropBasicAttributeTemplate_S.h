#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void PropBasicAttributeTemplateSize();
#pragma pack(push, 1)
    struct PropBasicAttributeTemplate : public template_base
    {
        int32 UseType;
        std::vector<int32> MoneyChange;
        int32 ItemGet;
        std::vector<float> RoleAttributeChange;
        int32 UseCD;
        std::vector<int32> WidgetJumpArray;
        int32 SpawnNpcId;
        int32 TitleGet;
        int32 BagSendInType;
        int32 FriendlinessAdd;
        std::vector<int32> OtherRecvItemID;
        std::vector<int32> LevelUp;
        std::vector<int32> FuncLevelLimit;
        std::vector<int32> UpgradeProtectInfoArray;
        std::vector<int32> ReducePkValue;
        int32 GetRechargeNum;
        int32 logic_id;
        int32 NeedRedTip;
        int32 EffectiveTime;
        int32 ExperienceVipLevel;
        std::vector<int32> AddMapCount;
        float ExpNum;
        std::vector<int32> ChooseItem;
        int32 MeditationMedicineTime;
        int32 OnlyUseInSelfServer;
        int32 AddHeartValue;
        int32 SummonNpcID;
        std::vector<int32> AddMapTimeArr;
    };
#pragma pack(pop)
}
