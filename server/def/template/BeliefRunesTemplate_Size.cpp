#include "BeliefRunesTemplate_S.h"

namespace faith
{
    void BeliefRunesTemplateSize()
    {
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("quality_level");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("AttType");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("MaxUpgradeLevel");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("AttNum");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("BasicAtt1");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("BasicAtt2");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("BasicAtt3");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("UpgradeAttEach1");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("UpgradeAttEach2");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("UpgradeAttEach3");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("UpgradeId");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("BuffIdArray");
        template_size::get_instance().template_size_map[e_BeliefRunesTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefRunesTemplate].push_back("ItemTemplateId");
    }
}
