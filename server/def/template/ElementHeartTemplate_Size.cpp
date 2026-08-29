#include "ElementHeartTemplate_S.h"

namespace faith
{
    void ElementHeartTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("quality_level");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("AttType");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("MaxUpgradeLevel");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("BasicAtt");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("UpgradeAttEach");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("UpgradeId");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("BuffIdArray");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("ItemTemplateId");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("MergeId");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("MergeCost");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("UnlockLevel");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("UnlockCost");
        template_size::get_instance().template_size_map[e_ElementHeartTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementHeartTemplate].push_back("UpgradeLevel");
    }
}
