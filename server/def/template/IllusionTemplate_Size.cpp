#include "IllusionTemplate_S.h"

namespace faith
{
    void IllusionTemplateSize()
    {
        template_size::get_instance().template_size_map[e_IllusionTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_IllusionTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_IllusionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_IllusionTemplate].push_back("WarriorItemSet");
        template_size::get_instance().template_size_map[e_IllusionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_IllusionTemplate].push_back("MageItemSet");
        template_size::get_instance().template_size_map[e_IllusionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_IllusionTemplate].push_back("KnightItemSet");
        template_size::get_instance().template_size_map[e_IllusionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_IllusionTemplate].push_back("AssassinItemSet");
        template_size::get_instance().template_size_map[e_IllusionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_IllusionTemplate].push_back("SkillID");
        template_size::get_instance().template_size_map[e_IllusionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_IllusionTemplate].push_back("RegionArr");
    }
}
