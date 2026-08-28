#include "RoleRewardTemplate_S.h"

namespace hld
{
    void RoleRewardTemplateSize()
    {
        template_size::get_instance().template_size_map[e_RoleRewardTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_RoleRewardTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_RoleRewardTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RoleRewardTemplate].push_back("ServerId");
        template_size::get_instance().template_size_map[e_RoleRewardTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_RoleRewardTemplate].push_back("RoleId");
        template_size::get_instance().template_size_map[e_RoleRewardTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RoleRewardTemplate].push_back("MailItem");
    }
}
