#include "MapSafeTemplate_S.h"

namespace faith
{
    void MapSafeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_MapSafeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_MapSafeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_MapSafeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MapSafeTemplate].push_back("PointAry");
        template_size::get_instance().template_size_map[e_MapSafeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapSafeTemplate].push_back("SafeName");
        template_size::get_instance().template_size_map[e_MapSafeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapSafeTemplate].push_back("IsAllowPK");
        template_size::get_instance().template_size_map[e_MapSafeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapSafeTemplate].push_back("Priority");
        template_size::get_instance().template_size_map[e_MapSafeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapSafeTemplate].push_back("BackgroundMusic");
        template_size::get_instance().template_size_map[e_MapSafeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapSafeTemplate].push_back("AreaType");
        template_size::get_instance().template_size_map[e_MapSafeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapSafeTemplate].push_back("DependentTerritory");
        template_size::get_instance().template_size_map[e_MapSafeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapSafeTemplate].push_back("MissionId");
    }
}
