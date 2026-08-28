#include "RewardSettlementTemplate_S.h"

namespace hld
{
    void RewardSettlementTemplateSize()
    {
        template_size::get_instance().template_size_map[e_RewardSettlementTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_RewardSettlementTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_RewardSettlementTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RewardSettlementTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_RewardSettlementTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_RewardSettlementTemplate].push_back("Experiencerewardfactor");
        template_size::get_instance().template_size_map[e_RewardSettlementTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_RewardSettlementTemplate].push_back("Coinrewards");
        template_size::get_instance().template_size_map[e_RewardSettlementTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RewardSettlementTemplate].push_back("LowScore");
        template_size::get_instance().template_size_map[e_RewardSettlementTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RewardSettlementTemplate].push_back("HighScore");
        template_size::get_instance().template_size_map[e_RewardSettlementTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RewardSettlementTemplate].push_back("ItemRewards");
    }
}
