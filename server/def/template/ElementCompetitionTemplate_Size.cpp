#include "ElementCompetitionTemplate_S.h"

namespace faith
{
    void ElementCompetitionTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ElementCompetitionTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ElementCompetitionTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ElementCompetitionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementCompetitionTemplate].push_back("RightAnswer");
        template_size::get_instance().template_size_map[e_ElementCompetitionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementCompetitionTemplate].push_back("Region");
        template_size::get_instance().template_size_map[e_ElementCompetitionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementCompetitionTemplate].push_back("SeasonNum");
        template_size::get_instance().template_size_map[e_ElementCompetitionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementCompetitionTemplate].push_back("Level");
        template_size::get_instance().template_size_map[e_ElementCompetitionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementCompetitionTemplate].push_back("NormalRewardArray");
        template_size::get_instance().template_size_map[e_ElementCompetitionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementCompetitionTemplate].push_back("SpecialRewardArray");
        template_size::get_instance().template_size_map[e_ElementCompetitionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementCompetitionTemplate].push_back("exp");
        template_size::get_instance().template_size_map[e_ElementCompetitionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementCompetitionTemplate].push_back("BigRewardTag");
    }
}
