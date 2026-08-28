#include "ProtectTemplate_S.h"

namespace hld
{
    void ProtectTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("NameId");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("CurrentOrder");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("CurrentStar");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("NextGradeStartId");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("OperateType");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("MoneyConsume");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("ItemID");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("ItemNum");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("PropertyRaisePercentage");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("AnimBP");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("Mesh");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("SelfAtt");
        template_size::get_instance().template_size_map[e_ProtectTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ProtectTemplate].push_back("Notice_id");
    }
}
