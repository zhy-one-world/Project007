#include "RobotTalkTemplate_S.h"

namespace faith
{
    void RobotTalkTemplateSize()
    {
        template_size::get_instance().template_size_map[e_RobotTalkTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_RobotTalkTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_RobotTalkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RobotTalkTemplate].push_back("TalkType");
        template_size::get_instance().template_size_map[e_RobotTalkTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_RobotTalkTemplate].push_back("Talklimit");
        template_size::get_instance().template_size_map[e_RobotTalkTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotTalkTemplate].push_back("InitiativeTalk");
        template_size::get_instance().template_size_map[e_RobotTalkTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotTalkTemplate].push_back("PassiveTalk");
        template_size::get_instance().template_size_map[e_RobotTalkTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotTalkTemplate].push_back("FrontActivityTalk");
        template_size::get_instance().template_size_map[e_RobotTalkTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_RobotTalkTemplate].push_back("BackActivityTalk");
    }
}
