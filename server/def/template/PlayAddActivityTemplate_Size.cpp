#include "PlayAddActivityTemplate_S.h"

namespace hld
{
    void PlayAddActivityTemplateSize()
    {
        template_size::get_instance().template_size_map[e_PlayAddActivityTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_PlayAddActivityTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_PlayAddActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PlayAddActivityTemplate].push_back("PlayType");
        template_size::get_instance().template_size_map[e_PlayAddActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PlayAddActivityTemplate].push_back("AddScore");
        template_size::get_instance().template_size_map[e_PlayAddActivityTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PlayAddActivityTemplate].push_back("AddProbability");
    }
}
