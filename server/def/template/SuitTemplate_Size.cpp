#include "SuitTemplate_S.h"

namespace hld
{
    void SuitTemplateSize()
    {
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("SuitNameID");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("DisplayRoleImage");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("RoleClassRequired");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("GradeRequired");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("DisplayCheckLevelRequired");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("EquipsRequiredArray");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("AttArray");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("DisplayUpgradeNum");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("DisplayAddonNum");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("SuitGetAreaStringID");
        template_size::get_instance().template_size_map[e_SuitTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_SuitTemplate].push_back("SuitGetWidgetJumpArray");
    }
}
