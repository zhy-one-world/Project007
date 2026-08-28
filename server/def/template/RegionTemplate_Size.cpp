#include "RegionTemplate_S.h"

namespace hld
{
    void RegionTemplateSize()
    {
        template_size::get_instance().template_size_map[e_RegionTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_RegionTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_RegionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RegionTemplate].push_back("RegionCode");
    }
}
