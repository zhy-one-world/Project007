#include "ArenaRobotTemplate_S.h"

namespace hld
{
    void ArenaRobotTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("Rank");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("NpcId");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("UpgradeId");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("PropertyRatio");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("PropertyPlus");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("Items");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("Wing");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("Sprite");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("EffectBuffArray");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("GsValue");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("MilitaryLevel");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("RobotNameId");
        template_size::get_instance().template_size_map[e_ArenaRobotTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ArenaRobotTemplate].push_back("ExpLevel");
    }
}
