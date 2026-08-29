#include "StopPlaceTemplate_S.h"

namespace faith
{
    void StopPlaceTemplateSize()
    {
        template_size::get_instance().template_size_map[e_StopPlaceTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_StopPlaceTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_StopPlaceTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_StopPlaceTemplate].push_back("StayPoint");
    }
}
