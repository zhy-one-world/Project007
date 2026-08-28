#include "ActiveDegreeTemplate_S.h"

namespace hld
{
    void ActiveDegreeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ActiveDegreeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ActiveDegreeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ActiveDegreeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ActiveDegreeTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_ActiveDegreeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ActiveDegreeTemplate].push_back("ActivityName");
        template_size::get_instance().template_size_map[e_ActiveDegreeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ActiveDegreeTemplate].push_back("TargetValue");
        template_size::get_instance().template_size_map[e_ActiveDegreeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ActiveDegreeTemplate].push_back("AddValue");
        template_size::get_instance().template_size_map[e_ActiveDegreeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ActiveDegreeTemplate].push_back("WidgetJumpArray");
        template_size::get_instance().template_size_map[e_ActiveDegreeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ActiveDegreeTemplate].push_back("CanGoto");
    }
}
