#include "RobotItemsTemplate_S.h"

namespace hld
{
    void RobotItemsTemplateSize()
    {
        template_size::get_instance().template_size_map[e_RobotItemsTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_RobotItemsTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_RobotItemsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotItemsTemplate].push_back("WarriorEquipment");
        template_size::get_instance().template_size_map[e_RobotItemsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotItemsTemplate].push_back("MasterEquipment");
        template_size::get_instance().template_size_map[e_RobotItemsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotItemsTemplate].push_back("KnightEquipment");
        template_size::get_instance().template_size_map[e_RobotItemsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotItemsTemplate].push_back("AssassinEquipment");
        template_size::get_instance().template_size_map[e_RobotItemsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotItemsTemplate].push_back("ApplyItem");
        template_size::get_instance().template_size_map[e_RobotItemsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotItemsTemplate].push_back("ApplyMoney");
        template_size::get_instance().template_size_map[e_RobotItemsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotItemsTemplate].push_back("VipApplyItem");
    }
}
