#include "RecycleTemplate_S.h"

namespace faith
{
    void RecycleTemplateSize()
    {
        template_size::get_instance().template_size_map[e_RecycleTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_RecycleTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_RecycleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RecycleTemplate].push_back("RecycleType");
        template_size::get_instance().template_size_map[e_RecycleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RecycleTemplate].push_back("Pos");
        template_size::get_instance().template_size_map[e_RecycleTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RecycleTemplate].push_back("MasterLevel");
        template_size::get_instance().template_size_map[e_RecycleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RecycleTemplate].push_back("ConditionName");
        template_size::get_instance().template_size_map[e_RecycleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RecycleTemplate].push_back("ConditionType");
        template_size::get_instance().template_size_map[e_RecycleTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RecycleTemplate].push_back("ConditionValue");
        template_size::get_instance().template_size_map[e_RecycleTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RecycleTemplate].push_back("RewardItemArray");
        template_size::get_instance().template_size_map[e_RecycleTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RecycleTemplate].push_back("WidgetJumpArray");
    }
}
