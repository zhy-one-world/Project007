#include "MapAwardTemplate_S.h"

namespace faith
{
    void MapAwardTemplateSize()
    {
        template_size::get_instance().template_size_map[e_MapAwardTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_MapAwardTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_MapAwardTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MapAwardTemplate].push_back("DropId");
        template_size::get_instance().template_size_map[e_MapAwardTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MapAwardTemplate].push_back("MoneyList");
    }
}
