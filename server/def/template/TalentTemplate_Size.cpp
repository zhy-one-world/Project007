#include "TalentTemplate_S.h"

namespace faith
{
    void TalentTemplateSize()
    {
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("Series");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("Level");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("LevelMax");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("Name");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("Icon");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("Describe");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("OpenMoney");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("OpenTalentId");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("BuffArray");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("ReplaceSkill");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("ReplaceTime");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("Active");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("NotActive");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("BuffAddType");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("GSAddValue");
        template_size::get_instance().template_size_map[e_TalentTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TalentTemplate].push_back("OpenrLayerLimit");
    }
}
