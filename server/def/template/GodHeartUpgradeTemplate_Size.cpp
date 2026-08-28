#include "GodHeartUpgradeTemplate_S.h"

namespace hld
{
    void GodHeartUpgradeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_GodHeartUpgradeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_GodHeartUpgradeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_GodHeartUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GodHeartUpgradeTemplate].push_back("UpgradeCost");
        template_size::get_instance().template_size_map[e_GodHeartUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GodHeartUpgradeTemplate].push_back("RecoveryReward");
        template_size::get_instance().template_size_map[e_GodHeartUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GodHeartUpgradeTemplate].push_back("DiamondRecoveryMoney");
        template_size::get_instance().template_size_map[e_GodHeartUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GodHeartUpgradeTemplate].push_back("DiamondRecoveryCost");
    }
}
