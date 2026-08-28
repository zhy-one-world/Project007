#include "ElementLadderTaskTemplate_S.h"

namespace hld
{
    void ElementLadderTaskTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ElementLadderTaskTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ElementLadderTaskTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ElementLadderTaskTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTaskTemplate].push_back("TaskType");
        template_size::get_instance().template_size_map[e_ElementLadderTaskTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTaskTemplate].push_back("TaskText");
        template_size::get_instance().template_size_map[e_ElementLadderTaskTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTaskTemplate].push_back("Value");
        template_size::get_instance().template_size_map[e_ElementLadderTaskTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementLadderTaskTemplate].push_back("Reward");
    }
}
