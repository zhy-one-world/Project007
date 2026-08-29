#include "TeamAimTypeTemplate_S.h"

namespace faith
{
    void TeamAimTypeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_TeamAimTypeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_TeamAimTypeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_TeamAimTypeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TeamAimTypeTemplate].push_back("AimType");
        template_size::get_instance().template_size_map[e_TeamAimTypeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TeamAimTypeTemplate].push_back("AimGroup");
        template_size::get_instance().template_size_map[e_TeamAimTypeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TeamAimTypeTemplate].push_back("AimTypeName");
        template_size::get_instance().template_size_map[e_TeamAimTypeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TeamAimTypeTemplate].push_back("AimTypeImage");
        template_size::get_instance().template_size_map[e_TeamAimTypeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TeamAimTypeTemplate].push_back("AimTypeValid");
        template_size::get_instance().template_size_map[e_TeamAimTypeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TeamAimTypeTemplate].push_back("SubTypeId");
        template_size::get_instance().template_size_map[e_TeamAimTypeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TeamAimTypeTemplate].push_back("SubTypeName");
    }
}
