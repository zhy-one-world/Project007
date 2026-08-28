#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void MountTemplateSize();
#pragma pack(push, 1)
    struct MountTemplate : public template_base
    {
        int32 MountNameId;
        int32 MountOrderNum;
        int32 ViewOrder;
        int32 MountClassMax;
        int32 MountGradeMax;
        int32 GradeNow;
        int32 ClassNow;
        int32 StarGradeMax;
        int32 StarGradeNow;
        std::vector<float> AttArray;
        int32 Type;
        int32 UpgradeMatirialId;
        int32 UpgradeMatirialShopId;
        int32 UpgradeMatirialNum;
        int32 LiftingClassMatirialId;
        int32 LiftingClassShopId;
        int32 LiftingClassMatirialNum;
        int32 LoginAnimationBlueprintRes;
        int32 AnimationBlueprintRes;
        int32 BornAnimRes;
        int32 UpAnimRes;
        int32 DownAnimRes;
        int32 FreeAnimRes;
        int32 GlideAnimRes;
        int32 AttackAnimRes;
        int32 ChannelAnimRes;
        int32 UpDistance;
        float UpTime;
        int32 IsQuickDownAndUp;
        int32 PictureNameRes;
        int32 ButtonPictureNameRes;
        int32 MeshNameRes;
        float MaxWalkSpeed;
        float MaxFlySpeed;
        std::vector<int32> PreMountRequirement;
        std::vector<int32> UnlockMoneyRequirement;
        std::vector<int32> UnlockMatirialRequirement;
        int32 IllusionOpenClass;
        std::vector<int32> IllusionList;
        std::vector<int32> IllusionOriginalIdRange;
        std::vector<int32> ExhibitionPos;
        std::vector<float> ExhibitionScale;
        std::vector<int32> RidingExhiibitionPos;
        std::vector<float> RidingExhiibitionScale;
        int32 NoticeId;
        int32 buff_mount_name;
        int32 buff_image_res;
        std::vector<int32> SelfMaxBuffIdArray;
        std::vector<int32> IllusionMaxBuffIdArray;
        std::vector<int32> mount_image_res_array;
        int32 ClickAnimRes;
        int32 ChooseRoleBornAniRes;
        int32 MountUnlockAni;
        std::vector<float> CallMountCameraAniList;
        std::vector<float> CallMountInSkyCameraAniList;
        std::vector<float> DisMountCameraAniList;
        std::vector<float> GotoSkyCameraAniList;
        std::vector<float> LandingCameraAniList;
        int32 TopHeadPos;
        int32 StarBuffId;
        int32 GradeLimit;
        int32 UpgradeLimit;
        std::vector<int32> RegionArr;
        int32 PowerID;
    };
#pragma pack(pop)
}
