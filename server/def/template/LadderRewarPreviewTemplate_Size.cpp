#include "LadderRewarPreviewTemplate_S.h"

namespace hld
{
    void LadderRewarPreviewTemplateSize()
    {
        template_size::get_instance().template_size_map[e_LadderRewarPreviewTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_LadderRewarPreviewTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_LadderRewarPreviewTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_LadderRewarPreviewTemplate].push_back("AwardItems");
        template_size::get_instance().template_size_map[e_LadderRewarPreviewTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_LadderRewarPreviewTemplate].push_back("AwardMoney");
    }
}
