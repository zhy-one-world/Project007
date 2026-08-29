#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void RechargeTemplateSize();
#pragma pack(push, 1)
    struct RechargeTemplate : public template_base
    {
        int32 Name;
        int32 Describe;
        int32 ImageID;
        int32 PaymentType;
        int32 Type;
        float Price;
        int32 VipPointBonuse;
        std::vector<int32> MoneyGetArray;
        int32 NewSignID;
        int32 MonthCardAddTime;
        std::vector<int32> FirstRechargeRewardMoneyArray;
        int32 OfflineItem;
        int32 IsShowInWidget;
        int32 SortNum;
        int32 PriceText;
        std::string ProductId;
    };
#pragma pack(pop)
}
