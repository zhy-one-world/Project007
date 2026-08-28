#include "WheelPlateTemplate_S.h"

namespace hld
{
    void WheelPlateTemplateSize()
    {
        template_size::get_instance().template_size_map[e_WheelPlateTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_WheelPlateTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_WheelPlateTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WheelPlateTemplate].push_back("CurItem");
    }
}
