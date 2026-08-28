#include "MiniMapTemplate_S.h"

namespace hld
{
    void MiniMapTemplateSize()
    {
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapNameResId");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapResId");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapImageStartX");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapImageStartY");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapRealStartX");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapRealStartY");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapImageLenthX");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapImageLenthY");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapRealLenthX");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapRealLenthY");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMapBorderPos");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniMarkIdList");
        template_size::get_instance().template_size_map[e_MiniMapTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MiniMapTemplate].push_back("MiniDropItemIdList");
    }
}
