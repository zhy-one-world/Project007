#include "DropTemplate_S.h"

namespace faith
{
    void DropTemplateSize()
    {
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("ClassType");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("DropLevelLimit");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("BelongID");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("BelongIDParam");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("BelongType");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("Distribute");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("DropType");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("IDs");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("IDTypes");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("Counts");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("Bounds");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("Weights");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("ConstAtt");
        template_size::get_instance().template_size_map[e_DropTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_DropTemplate].push_back("LevelChangeId");
    }
}
