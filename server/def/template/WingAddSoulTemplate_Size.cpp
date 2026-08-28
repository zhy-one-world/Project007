#include "WingAddSoulTemplate_S.h"

namespace hld
{
    void WingAddSoulTemplateSize()
    {
        template_size::get_instance().template_size_map[e_WingAddSoulTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_WingAddSoulTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_WingAddSoulTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_WingAddSoulTemplate].push_back("SoulNum");
        template_size::get_instance().template_size_map[e_WingAddSoulTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_WingAddSoulTemplate].push_back("AttArray");
        template_size::get_instance().template_size_map[e_WingAddSoulTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_WingAddSoulTemplate].push_back("AddSoulMatirialId");
        template_size::get_instance().template_size_map[e_WingAddSoulTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_WingAddSoulTemplate].push_back("AddSoulMatirialNum");
        template_size::get_instance().template_size_map[e_WingAddSoulTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WingAddSoulTemplate].push_back("AddSoulMoney");
    }
}
