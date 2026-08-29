#include "BeliefRunesUpgradeTemplate_S.h"

namespace faith
{
    void BeliefRunesUpgradeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_BeliefRunesUpgradeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_BeliefRunesUpgradeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_BeliefRunesUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BeliefRunesUpgradeTemplate].push_back("UpgradeCost");
        template_size::get_instance().template_size_map[e_BeliefRunesUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BeliefRunesUpgradeTemplate].push_back("RecoveryReward");
        template_size::get_instance().template_size_map[e_BeliefRunesUpgradeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefRunesUpgradeTemplate].push_back("Notice_id");
        template_size::get_instance().template_size_map[e_BeliefRunesUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BeliefRunesUpgradeTemplate].push_back("DiamondRecoveryMoney");
        template_size::get_instance().template_size_map[e_BeliefRunesUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BeliefRunesUpgradeTemplate].push_back("DiamondRecoveryCost");
    }
}
