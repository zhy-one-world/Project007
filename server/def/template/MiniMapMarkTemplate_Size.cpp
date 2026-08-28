#include "MiniMapMarkTemplate_S.h"

namespace hld
{
    void MiniMapMarkTemplateSize()
    {
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("MarkType");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("NameStringId");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("NpcId");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("RegionLevel");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("MapGoalX");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("MapGoalY");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("MapGoalZ");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("MiniMapX");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("MiniGoalY");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("MiniGoalZ");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("NpcPriority");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("IfShow");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("MissionId");
        template_size::get_instance().template_size_map[e_MiniMapMarkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapMarkTemplate].push_back("SafePoint");
    }
}
