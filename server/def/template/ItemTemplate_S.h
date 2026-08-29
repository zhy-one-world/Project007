#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ItemTemplateSize();
#pragma pack(push, 1)
    struct ItemTemplate : public template_base
    {
        int32 ItemName;
        int32 ItemObtainDescription;
        int32 ItemDescription;
        int32 ItemIcon;
        int32 ItemMesh;
        int32 DropNpcMesh;
        int32 item_type;
        int32 sub_type;
        int32 logic_id;
        int32 item_color;
        std::vector<int32> level_limit;
        int32 sexual_limit;
        std::vector<int32> class_type;
        int32 grade_level_limit;
        int32 max_pile_num;
        std::vector<int32> SellMoney;
        int32 locked;
        int32 AutoUse;
        int32 IsShowRecommend;
        int32 IsValuable;
        int32 FlyToUI;
        std::vector<int32> FlyCount;
        int32 AuctionLabType;
        int32 AuctionLabSubType;
        std::vector<int32> AuctionMinMoneyArray;
        std::vector<int32> AuctionRecommendMoneyArray;
        std::vector<int32> AuctionMaxMoneyArray;
        std::vector<int32> AuctionAutoRecoverMoneyArray;
        std::vector<int32> AuctionCanSellMoneyArray;
        int32 TradingLabType;
        int32 TradingSubLabType;
        std::vector<int32> TradingBaseMoneyArray;
        std::vector<int32> TradingFinalMoneyArray;
        int32 TradingAuctionTime;
        int32 TradingAwaitTime;
        std::vector<int32> skill_array;
        std::vector<int32> buff_trigger_id;
        int32 advanced_type;
        int32 advanced_id;
        int32 god_assembled;
        int32 god_assembled_id;
        std::vector<float> game_location;
        std::vector<float> game_rotation;
        std::vector<float> game_scale;
        std::vector<float> login_location;
        std::vector<float> login_rotation;
        std::vector<float> login_scale;
        std::vector<int32> GetWayWidgetJumpArray;
        std::vector<int32> GetWayUnlockIdArray;
        std::vector<int32> ShowNpc;
        int32 Notice_id;
        int32 FuncUnlockid;
        std::vector<int32> goods_id;
        int32 StoreJumpSpecialItemId;
        int32 DropRecordModel;
        int32 ExtraTidyWeight;
        int32 EffectiveTime;
        int32 LegionDonateAddIntegral;
        int32 LegionDonateReduceIntegral;
        int32 FashionId;
        std::vector<int32> Illusionid;
        int32 AwakenID;
        int32 AwakenPower;
        int32 IsShowOpen;
        float AutoPickTime;
        int32 DecomposeTip;
        int32 AuctionNotice;
    };
#pragma pack(pop)
}
