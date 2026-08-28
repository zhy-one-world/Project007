#include "MoneyTemplate_S.h"

namespace hld
{
    void MoneyTemplateSize()
    {
        template_size::get_instance().template_size_map[e_MoneyTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_MoneyTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_MoneyTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MoneyTemplate].push_back("MoneyType");
        template_size::get_instance().template_size_map[e_MoneyTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MoneyTemplate].push_back("IconId");
        template_size::get_instance().template_size_map[e_MoneyTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MoneyTemplate].push_back("NameId");
        template_size::get_instance().template_size_map[e_MoneyTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MoneyTemplate].push_back("GetWayWidgetJumpArray");
        template_size::get_instance().template_size_map[e_MoneyTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MoneyTemplate].push_back("MissionRequiredId");
        template_size::get_instance().template_size_map[e_MoneyTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MoneyTemplate].push_back("SystemNameId");
        template_size::get_instance().template_size_map[e_MoneyTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MoneyTemplate].push_back("GetWayUnlockIdArray");
        template_size::get_instance().template_size_map[e_MoneyTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MoneyTemplate].push_back("ColorStringId");
    }
}
