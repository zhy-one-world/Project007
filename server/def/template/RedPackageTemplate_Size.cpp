#include "RedPackageTemplate_S.h"

namespace faith
{
    void RedPackageTemplateSize()
    {
        template_size::get_instance().template_size_map[e_RedPackageTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_RedPackageTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_RedPackageTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RedPackageTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_RedPackageTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RedPackageTemplate].push_back("SubType");
        template_size::get_instance().template_size_map[e_RedPackageTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RedPackageTemplate].push_back("CostMoneyArray");
        template_size::get_instance().template_size_map[e_RedPackageTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RedPackageTemplate].push_back("CanGetMoneyArray");
        template_size::get_instance().template_size_map[e_RedPackageTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RedPackageTemplate].push_back("CanGetTimes");
        template_size::get_instance().template_size_map[e_RedPackageTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RedPackageTemplate].push_back("NoTiceId");
        template_size::get_instance().template_size_map[e_RedPackageTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RedPackageTemplate].push_back("LogicID");
    }
}
