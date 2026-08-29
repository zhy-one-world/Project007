#include "NpcPlayTemplate_S.h"

namespace faith
{
    void NpcPlayTemplateSize()
    {
        template_size::get_instance().template_size_map[e_NpcPlayTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_NpcPlayTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_NpcPlayTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NpcPlayTemplate].push_back("PlayType");
    }
}
