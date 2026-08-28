#include "ChangeShapeTemplate_S.h"

namespace hld
{
    void ChangeShapeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ChangeShapeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ChangeShapeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ChangeShapeTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_ChangeShapeTemplate].push_back("ChangeTime");
        template_size::get_instance().template_size_map[e_ChangeShapeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ChangeShapeTemplate].push_back("UnitRes");
        template_size::get_instance().template_size_map[e_ChangeShapeTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ChangeShapeTemplate].push_back("ChangeNpcId");
        template_size::get_instance().template_size_map[e_ChangeShapeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ChangeShapeTemplate].push_back("SkillSeriesList");
        template_size::get_instance().template_size_map[e_ChangeShapeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ChangeShapeTemplate].push_back("BeginBuffList");
        template_size::get_instance().template_size_map[e_ChangeShapeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ChangeShapeTemplate].push_back("EndBuffList");
    }
}
