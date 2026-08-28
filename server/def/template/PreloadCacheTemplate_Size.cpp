#include "PreloadCacheTemplate_S.h"

namespace hld
{
    void PreloadCacheTemplateSize()
    {
        template_size::get_instance().template_size_map[e_PreloadCacheTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_PreloadCacheTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_PreloadCacheTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_PreloadCacheTemplate].push_back("ResName");
    }
}
