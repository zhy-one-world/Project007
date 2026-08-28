#include "AwakenTemplate_S.h"

namespace hld
{
    void AwakenTemplateSize()
    {
        template_size::get_instance().template_size_map[e_AwakenTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_AwakenTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_AwakenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AwakenTemplate].push_back("Level");
        template_size::get_instance().template_size_map[e_AwakenTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_AwakenTemplate].push_back("AttArray");
        template_size::get_instance().template_size_map[e_AwakenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AwakenTemplate].push_back("MatirialID");
        template_size::get_instance().template_size_map[e_AwakenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AwakenTemplate].push_back("MatirialNum");
        template_size::get_instance().template_size_map[e_AwakenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AwakenTemplate].push_back("GradeLimit");
        template_size::get_instance().template_size_map[e_AwakenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AwakenTemplate].push_back("NeedPower");
        template_size::get_instance().template_size_map[e_AwakenTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_AwakenTemplate].push_back("LevelMax");
    }
}
