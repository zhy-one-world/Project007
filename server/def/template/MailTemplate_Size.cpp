#include "MailTemplate_S.h"

namespace faith
{
    void MailTemplateSize()
    {
        template_size::get_instance().template_size_map[e_MailTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_MailTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_MailTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MailTemplate].push_back("TitleID");
        template_size::get_instance().template_size_map[e_MailTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MailTemplate].push_back("SenderNameID");
        template_size::get_instance().template_size_map[e_MailTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MailTemplate].push_back("ContentTextID");
        template_size::get_instance().template_size_map[e_MailTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MailTemplate].push_back("MoneyArray");
        template_size::get_instance().template_size_map[e_MailTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MailTemplate].push_back("ItemArray");
    }
}
