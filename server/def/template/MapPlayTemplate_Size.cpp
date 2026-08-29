#include "MapPlayTemplate_S.h"

namespace faith
{
    void MapPlayTemplateSize()
    {
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("PlayType");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("NeedEnergy");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("Icon");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("Title");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("Content");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("ExIcon");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("BossId");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("BornX");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("BornY");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("BornZ");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("BornYaw");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("BornPitch");
        template_size::get_instance().template_size_map[e_MapPlayTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MapPlayTemplate].push_back("BornRoll");
    }
}
