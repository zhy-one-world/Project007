#include "ResourceTemplate_S.h"

namespace faith
{
    void ResourceTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ResourceTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ResourceTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ResourceTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ResourceTemplate].push_back("ResourceType");
        template_size::get_instance().template_size_map[e_ResourceTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_ResourceTemplate].push_back("ResourcePath");
        template_size::get_instance().template_size_map[e_ResourceTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ResourceTemplate].push_back("ResourcePriority");
        template_size::get_instance().template_size_map[e_ResourceTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ResourceTemplate].push_back("IsUnchangeByBuff");
    }
}
