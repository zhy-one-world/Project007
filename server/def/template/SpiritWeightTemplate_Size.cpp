#include "SpiritWeightTemplate_S.h"

namespace faith
{
    void SpiritWeightTemplateSize()
    {
        template_size::get_instance().template_size_map[e_SpiritWeightTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_SpiritWeightTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_SpiritWeightTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SpiritWeightTemplate].push_back("SpiritTemplateID");
        template_size::get_instance().template_size_map[e_SpiritWeightTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SpiritWeightTemplate].push_back("Weight");
    }
}
