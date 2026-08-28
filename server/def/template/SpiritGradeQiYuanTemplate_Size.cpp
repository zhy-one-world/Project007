#include "SpiritGradeQiYuanTemplate_S.h"

namespace hld
{
    void SpiritGradeQiYuanTemplateSize()
    {
        template_size::get_instance().template_size_map[e_SpiritGradeQiYuanTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_SpiritGradeQiYuanTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_SpiritGradeQiYuanTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_SpiritGradeQiYuanTemplate].push_back("TotalGradeNeed");
        template_size::get_instance().template_size_map[e_SpiritGradeQiYuanTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SpiritGradeQiYuanTemplate].push_back("BarserkerAtt");
        template_size::get_instance().template_size_map[e_SpiritGradeQiYuanTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SpiritGradeQiYuanTemplate].push_back("WizardAtt");
        template_size::get_instance().template_size_map[e_SpiritGradeQiYuanTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SpiritGradeQiYuanTemplate].push_back("GuardianAtt");
        template_size::get_instance().template_size_map[e_SpiritGradeQiYuanTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_SpiritGradeQiYuanTemplate].push_back("AssassinatorAtt");
    }
}
