#include "ElementHeartUpgradeTemplate_S.h"

namespace hld
{
    void ElementHeartUpgradeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ElementHeartUpgradeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ElementHeartUpgradeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ElementHeartUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementHeartUpgradeTemplate].push_back("UpgradeCost");
        template_size::get_instance().template_size_map[e_ElementHeartUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementHeartUpgradeTemplate].push_back("RecoveryReward");
        template_size::get_instance().template_size_map[e_ElementHeartUpgradeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartUpgradeTemplate].push_back("Notice_id");
        template_size::get_instance().template_size_map[e_ElementHeartUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementHeartUpgradeTemplate].push_back("DiamondRecoveryMoney");
        template_size::get_instance().template_size_map[e_ElementHeartUpgradeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementHeartUpgradeTemplate].push_back("DiamondRecoveryCost");
        template_size::get_instance().template_size_map[e_ElementHeartUpgradeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartUpgradeTemplate].push_back("UpgradeNeedNum");
    }
}
