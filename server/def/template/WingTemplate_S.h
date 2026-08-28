#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void WingTemplateSize();
#pragma pack(push, 1)
    struct WingTemplate : public template_base
    {
        int32 WingNameId;
        int32 ViewOrder;
        int32 WingUpgradeMax;
        int32 WingAddStarMax;
        int32 StarNow;
        int32 UpgradeNow;
        int32 StarGradeMax;
        int32 StarGradeNow;
        std::vector<float> AttArray;
        int32 Type;
        std::vector<int32> IIllusionList;
        int32 WingOpenNeedRoleClass;
        std::vector<int32> PreWingRequirement;
        std::vector<int32> UnlockNeedMatirial;
        std::vector<int32> UnlockNeedMoney;
        int32 AddStarMatirialId;
        int32 StarShopId;
        int32 AddStarMatirialNum;
        int32 AddStarMaxEXP;
        int32 EachAddStarExp;
        float AddStarCriticalRate;
        int32 UpgradeMatirialId;
        int32 UpgradeShopId;
        int32 UpgradeMatirialNum;
        std::vector<int32> UpgradeMoney;
        int32 EachUpgradeBless;
        int32 MinUpgradeBless;
        int32 MaxUpgradeBless;
        float BasicRate;
        float RateChange;
        int32 WingMeshID;
        int32 WingAnimBlueprint;
        int32 WingFreeAnim;
        int32 WingGlideAnim;
        int32 WingCallAnim;
        int32 WingDeadAnim;
        int32 WingSkillAnim;
        int32 WingSpecialSkillAnim;
        int32 PictureNameRes;
        int32 ButtonPictureNameRes;
        int32 NoticeId;
        std::vector<int32> IllusionOriginalIdRange;
        int32 buff_wing_name;
        int32 buff_image_res;
        std::vector<int32> buff_trigger_id;
        std::vector<int32> wing_image_res_array;
        int32 WingClickAnim;
        std::vector<int32> ExhibitionPos;
        std::vector<float> ExhibitionScale;
        float MaxWalkSpeed;
        float MaxFlySpeed;
        int32 FlyUpAnimRes;
        int32 FlyDownAnimRes;
        float FlyUpTime;
        std::vector<int32> SprintAniRes;
        std::vector<float> SprintLenAndTime;
        int32 StarBuffId;
        int32 GradeLimit;
        int32 UpgradeLimit;
        std::vector<int32> RegionArr;
        std::vector<int32> SkillPool;
        std::vector<int32> AddLingMaterial;
        std::vector<int32> AddLingMaterialNum;
        int32 LengQueTime;
        std::vector<float> ExcellentAttArray;
    };
#pragma pack(pop)
}
