#include "NotificationTemplate_S.h"

namespace faith
{
    void NotificationTemplateSize()
    {
        template_size::get_instance().template_size_map[e_NotificationTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_NotificationTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_NotificationTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NotificationTemplate].push_back("NotificationType");
        template_size::get_instance().template_size_map[e_NotificationTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NotificationTemplate].push_back("WeekNum");
        template_size::get_instance().template_size_map[e_NotificationTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NotificationTemplate].push_back("DayTime");
        template_size::get_instance().template_size_map[e_NotificationTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_NotificationTemplate].push_back("ActivityTime");
        template_size::get_instance().template_size_map[e_NotificationTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NotificationTemplate].push_back("OfflineTime");
        template_size::get_instance().template_size_map[e_NotificationTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NotificationTemplate].push_back("Title");
        template_size::get_instance().template_size_map[e_NotificationTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NotificationTemplate].push_back("Body");
        template_size::get_instance().template_size_map[e_NotificationTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_NotificationTemplate].push_back("Action");
        template_size::get_instance().template_size_map[e_NotificationTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_NotificationTemplate].push_back("Event");
    }
}
