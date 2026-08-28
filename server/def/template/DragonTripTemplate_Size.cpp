#include "DragonTripTemplate_S.h"

namespace hld
{
    void DragonTripTemplateSize()
    {
        template_size::get_instance().template_size_map[e_DragonTripTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_DragonTripTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_DragonTripTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DragonTripTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_DragonTripTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DragonTripTemplate].push_back("Pos");
        template_size::get_instance().template_size_map[e_DragonTripTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DragonTripTemplate].push_back("PosType");
        template_size::get_instance().template_size_map[e_DragonTripTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DragonTripTemplate].push_back("ItemId");
    }
}
