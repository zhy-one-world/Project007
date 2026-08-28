#include "ServiceGoalTemplate_S.h"

namespace hld
{
    void ServiceGoalTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ServiceGoalTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ServiceGoalTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ServiceGoalTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServiceGoalTemplate].push_back("ActiveName");
        template_size::get_instance().template_size_map[e_ServiceGoalTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServiceGoalTemplate].push_back("ActiveType");
        template_size::get_instance().template_size_map[e_ServiceGoalTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServiceGoalTemplate].push_back("ConditionType");
        template_size::get_instance().template_size_map[e_ServiceGoalTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ServiceGoalTemplate].push_back("ConditionValue");
        template_size::get_instance().template_size_map[e_ServiceGoalTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServiceGoalTemplate].push_back("TheData");
        template_size::get_instance().template_size_map[e_ServiceGoalTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServiceGoalTemplate].push_back("NoticeId");
        template_size::get_instance().template_size_map[e_ServiceGoalTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ServiceGoalTemplate].push_back("RewardMoneyArray");
        template_size::get_instance().template_size_map[e_ServiceGoalTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServiceGoalTemplate].push_back("RewardItemArray");
        template_size::get_instance().template_size_map[e_ServiceGoalTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ServiceGoalTemplate].push_back("WidgetJumpArray");
    }
}
