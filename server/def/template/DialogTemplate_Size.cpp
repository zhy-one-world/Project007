#include "DialogTemplate_S.h"

namespace hld
{
    void DialogTemplateSize()
    {
        template_size::get_instance().template_size_map[e_DialogTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_DialogTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_DialogTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DialogTemplate].push_back("NextDialogId");
        template_size::get_instance().template_size_map[e_DialogTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DialogTemplate].push_back("FinnalStepId");
        template_size::get_instance().template_size_map[e_DialogTemplate].push_back("bool");
        template_size::get_instance().template_key_map[e_DialogTemplate].push_back("LeftOrRight");
        template_size::get_instance().template_size_map[e_DialogTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_DialogTemplate].push_back("DialogWordRes");
    }
}
