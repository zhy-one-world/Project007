#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void SpiritTemplateSize();
#pragma pack(push, 1)
    struct SpiritTemplate : public template_base
    {
        int32 SpiritName;
        int32 Type;
        int32 Color;
        int32 ColorBlink;
        int32 Level;
        int32 Image;
        int32 HeadImage;
        int32 SpriteAnimBP;
        int32 Mesh;
        int32 SpriteVedio;
        std::vector<float> BasicProperty;
        std::vector<float> FlairProperty;
        std::vector<int32> FlairPropertyWeight;
        int32 StarGradeMax;
        int32 StarGradeNow;
        int32 StarGradeTemplateID;
        std::vector<int32> UpLevelCost;
        int32 UpgradeMatirialId;
        int32 UpgradeMatirialNum;
        std::vector<float> FightAddBonus;
        int32 UpgradeCost;
        int32 ExpPerUpgrade;
        std::vector<int32> RecoveryMoney;
        std::vector<int32> DiamondRecoveryMoney;
        std::vector<int32> DiamondRecoveryCost;
        int32 ItemTemplateID;
        std::vector<float> ExhibitionScale;
        std::vector<float> ExhibitionPosition;
        int32 Noticeld;
        std::vector<int32> GetIllusionItemReqArray;
        std::vector<int32> GetIllusionMoneyReqArray;
        std::vector<int32> IllusionArray;
        std::vector<int32> StarBuffId;
        int32 AccessChannel;
        std::vector<float> BaseExcellentAtt;
        std::vector<int32> GreenExcellentRatio;
        std::vector<int32> BlueExcellentRatio;
        std::vector<int32> PurpleExcellentRatio;
        std::vector<int32> OrangeExcellentRatio;
        std::vector<int32> RedExcellentRatio;
        std::vector<int32> PinkExcellentRatio;
        std::vector<int32> ExcellentAttQualityWeight;
        std::vector<int32> ExcellentAttNumGenRate;
        int32 SpiritDisplayBuffID;
        int32 InnerSpiritDisplayBuffID;
        int32 MaterialID;
        int32 MaterialCost;
        std::vector<int32> LockMaterialID;
        std::vector<int32> LockMaterialCost;
        std::vector<int32> ForgeExcellentAttQualityWeight;
        int32 GradeLimit;
        std::vector<int32> RegionArr;
        int32 ComopsitAni;
        int32 ComopsitDesc;
        int32 ComopsitName;
        std::vector<float> CompsitExhibitionScale;
        std::vector<float> CompsitExhibitionPosition;
        int32 UpgradePromoteMaxLevel;
        int32 UpgradePromoteLevel;
        int32 UpgradePromoteId;
        std::vector<int32> UpgradePromoteMaterials;
        std::vector<int32> SpiritXiuXianAniArray;
        std::vector<int32> AniIntervalTime;
    };
#pragma pack(pop)
}
