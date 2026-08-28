#include "BagExtendTemplate_S.h"

namespace hld
{
    void BagExtendTemplateSize()
    {
        template_size::get_instance().template_size_map[e_BagExtendTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_BagExtendTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_BagExtendTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BagExtendTemplate].push_back("Time");
        template_size::get_instance().template_size_map[e_BagExtendTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BagExtendTemplate].push_back("MoneyNeed");
    }
}
