#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void VIPTemplateSize();
#pragma pack(push, 1)
    struct VIPTemplate : public template_base
    {
        int32 Level;
        int32 LevelIcon;
        int32 LevelIcon2;
        std::vector<int32> RegionArr;
        int32 BgIconId;
        int32 BigLevelIconID;
        int32 LevelUpRequest;
        std::vector<int32> PurchaseOriginalCostArray;
        std::vector<int32> PurchaseCostArray;
        std::vector<int32> ItemGetArray;
        std::vector<int32> DescribeImageArray;
        int32 PropertyBonuseBuffID;
        int32 KIllMonsterBUffID;
        int32 MeditationFunc;
        int32 FreeRedPackageNum;
        int32 AutoUseCard;
        int32 AutoUsePsyche;
        int32 IsLimitlessTransfer;
        int32 IsArenaChallengeNoCD;
        int32 LivenessBonuse;
        float AuctionRewardPercentage;
        int32 IsDailyMissionAutoComplete;
        int32 IsAutoRecoveryEquip;
        int32 IsWorldBossTransfer;
        int32 UpgradeSuccessRatioBonuse;
        int32 FreeYuanDiFuHuoTime;
        int32 RedPackageGetTimes;
        float BagSlotOpenRate;
        int32 SiliverChangeTimes;
        int32 ArenaBuyTimeLimit;
        int32 BindDiamondChangeTimes;
        int32 ArenaBuffID;
        int32 DiamondChangeTimes;
        int32 MonsterExpBonuse;
        int32 IsPokedexDiamondSubMit;
        int32 ElementHeartCallLimit;
        int32 IsExclusiveBossRaid;
        int32 IsDoubleMeditationBonuseGet;
        int32 IsFourfoldMeditationBonuseGet;
        int32 BloodCastleTimes;
        int32 DemonSquarTimes;
        std::vector<int32> ExhibitionNpcId;
        std::vector<int32> ExhibitionBuffIdArray;
        int32 ExhibitionIconId;
        std::vector<int32> RightDesIdArray;
        int32 ExtraJewelSlotNum;
        std::vector<float> LegionDonateLimiteRate;
        int32 TitleId;
        std::vector<int32> VIPGainTreasureCost;
        int32 DramaRaidCommonEnter;
        std::vector<int32> RaidMapEnter;
        int32 IsCanTransferByNpc;
        int32 ExperienceExtraImgId;
        int32 CloudShopLimitCount;
        int32 ExpRaidTime;
        std::vector<int32> ElementWarlimit;
        int32 DailyMustMax;
        int32 LimitConvertExpTime;
    };
#pragma pack(pop)
}
