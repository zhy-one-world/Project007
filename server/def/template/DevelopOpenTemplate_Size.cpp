#include "DevelopOpenTemplate_S.h"

namespace hld
{
    void DevelopOpenTemplateSize()
    {
        template_size::get_instance().template_size_map[e_DevelopOpenTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_DevelopOpenTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_DevelopOpenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DevelopOpenTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_DevelopOpenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DevelopOpenTemplate].push_back("OpenLevel");
        template_size::get_instance().template_size_map[e_DevelopOpenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DevelopOpenTemplate].push_back("NeedRoleLevel");
        template_size::get_instance().template_size_map[e_DevelopOpenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DevelopOpenTemplate].push_back("NeedWorldLevel");
        template_size::get_instance().template_size_map[e_DevelopOpenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DevelopOpenTemplate].push_back("NeedMission");
    }
}
