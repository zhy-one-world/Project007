#include "ElementHeartGroupTemplate_S.h"

namespace faith
{
    void ElementHeartGroupTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ElementHeartGroupTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ElementHeartGroupTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ElementHeartGroupTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementHeartGroupTemplate].push_back("MoneyCost");
        template_size::get_instance().template_size_map[e_ElementHeartGroupTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_ElementHeartGroupTemplate].push_back("UpgradeRate");
        template_size::get_instance().template_size_map[e_ElementHeartGroupTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartGroupTemplate].push_back("DropGroupId");
        template_size::get_instance().template_size_map[e_ElementHeartGroupTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartGroupTemplate].push_back("Index");
        template_size::get_instance().template_size_map[e_ElementHeartGroupTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartGroupTemplate].push_back("Noticeld");
        template_size::get_instance().template_size_map[e_ElementHeartGroupTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartGroupTemplate].push_back("Integral");
    }
}
