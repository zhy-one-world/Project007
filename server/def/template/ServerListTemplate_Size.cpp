#include "ServerListTemplate_S.h"

namespace hld
{
    void ServerListTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ServerListTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ServerListTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ServerListTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServerListTemplate].push_back("GroupId");
        template_size::get_instance().template_size_map[e_ServerListTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServerListTemplate].push_back("ServerId");
        template_size::get_instance().template_size_map[e_ServerListTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServerListTemplate].push_back("ServerType");
        template_size::get_instance().template_size_map[e_ServerListTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServerListTemplate].push_back("ServerName");
        template_size::get_instance().template_size_map[e_ServerListTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_ServerListTemplate].push_back("ServerIp");
        template_size::get_instance().template_size_map[e_ServerListTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServerListTemplate].push_back("ServerPort");
        template_size::get_instance().template_size_map[e_ServerListTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServerListTemplate].push_back("PortNum");
        template_size::get_instance().template_size_map[e_ServerListTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ServerListTemplate].push_back("IsRecommend");
        template_size::get_instance().template_size_map[e_ServerListTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_ServerListTemplate].push_back("BeginTime");
    }
}
