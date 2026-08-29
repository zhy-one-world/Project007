#include "VersionTemplate_S.h"

namespace faith
{
    void VersionTemplateSize()
    {
        template_size::get_instance().template_size_map[e_VersionTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_VersionTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_VersionTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_VersionTemplate].push_back("Version");
    }
}
