#include "RewardByRegionTemplate_S.h"

namespace hld
{
    void RewardByRegionTemplateSize()
    {
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("RewardType");
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("ChinaMainRegionReward");
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("KRRegionReward");
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("ChinaTWRegionReward");
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("ENRegionReward");
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("THARegionReward");
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("VNRegionReward");
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("JPRegionReward");
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("RUSRegionReward");
        template_size::get_instance().template_size_map[e_RewardByRegionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RewardByRegionTemplate].push_back("SGRegionReward");
    }
}
