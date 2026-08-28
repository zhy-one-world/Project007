#include "WingAddSpiritTemplate_S.h"

namespace hld
{
    void WingAddSpiritTemplateSize()
    {
        template_size::get_instance().template_size_map[e_WingAddSpiritTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_WingAddSpiritTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_WingAddSpiritTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_WingAddSpiritTemplate].push_back("SpiritNum");
        template_size::get_instance().template_size_map[e_WingAddSpiritTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_WingAddSpiritTemplate].push_back("AttArray");
        template_size::get_instance().template_size_map[e_WingAddSpiritTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_WingAddSpiritTemplate].push_back("AddSpiritMatirialId");
        template_size::get_instance().template_size_map[e_WingAddSpiritTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_WingAddSpiritTemplate].push_back("AddSpiritMatirialNum");
        template_size::get_instance().template_size_map[e_WingAddSpiritTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WingAddSpiritTemplate].push_back("AddSpiritMoney");
    }
}
