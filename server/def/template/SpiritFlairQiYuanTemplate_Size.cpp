#include "SpiritFlairQiYuanTemplate_S.h"

namespace faith
{
    void SpiritFlairQiYuanTemplateSize()
    {
        template_size::get_instance().template_size_map[e_SpiritFlairQiYuanTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_SpiritFlairQiYuanTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_SpiritFlairQiYuanTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SpiritFlairQiYuanTemplate].push_back("TotalFlairNeed");
        template_size::get_instance().template_size_map[e_SpiritFlairQiYuanTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SpiritFlairQiYuanTemplate].push_back("BarserkerAtt");
        template_size::get_instance().template_size_map[e_SpiritFlairQiYuanTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SpiritFlairQiYuanTemplate].push_back("WizardAtt");
        template_size::get_instance().template_size_map[e_SpiritFlairQiYuanTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SpiritFlairQiYuanTemplate].push_back("GuardianAtt");
        template_size::get_instance().template_size_map[e_SpiritFlairQiYuanTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SpiritFlairQiYuanTemplate].push_back("AssassinatorAtt");
    }
}
