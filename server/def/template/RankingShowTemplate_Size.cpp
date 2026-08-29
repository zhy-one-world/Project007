#include "RankingShowTemplate_S.h"

namespace faith
{
    void RankingShowTemplateSize()
    {
        template_size::get_instance().template_size_map[e_RankingShowTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_RankingShowTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_RankingShowTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RankingShowTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_RankingShowTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RankingShowTemplate].push_back("SubType");
        template_size::get_instance().template_size_map[e_RankingShowTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RankingShowTemplate].push_back("ImgId");
        template_size::get_instance().template_size_map[e_RankingShowTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RankingShowTemplate].push_back("DesStrId");
        template_size::get_instance().template_size_map[e_RankingShowTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RankingShowTemplate].push_back("IsHidden");
        template_size::get_instance().template_size_map[e_RankingShowTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RankingShowTemplate].push_back("RankingType");
        template_size::get_instance().template_size_map[e_RankingShowTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RankingShowTemplate].push_back("RankingBossID");
        template_size::get_instance().template_size_map[e_RankingShowTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RankingShowTemplate].push_back("RankingRewards");
    }
}
