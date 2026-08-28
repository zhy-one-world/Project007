#include "StoreTemplate_S.h"

namespace hld
{
    void StoreTemplateSize()
    {
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("StoreType");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("StoreGoods");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("LevelLimit");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("OpenType");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("StoreOpenTimeLimit");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("LastTime");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("FreshType");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("FreshTime");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("FreshInterval");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("FirstLabelIndex");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("ExtraMoneyShowArray");
        template_size::get_instance().template_size_map[e_StoreTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_StoreTemplate].push_back("StoreNameId");
    }
}
