#include "JewelTemplate_S.h"

namespace faith
{
    void JewelTemplateSize()
    {
        template_size::get_instance().template_size_map[e_JewelTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_JewelTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_JewelTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelTemplate].push_back("JewelType");
        template_size::get_instance().template_size_map[e_JewelTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelTemplate].push_back("JewelLevel");
        template_size::get_instance().template_size_map[e_JewelTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_JewelTemplate].push_back("JewelValue");
        template_size::get_instance().template_size_map[e_JewelTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_JewelTemplate].push_back("JewelAtt");
        template_size::get_instance().template_size_map[e_JewelTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelTemplate].push_back("ComposeId");
        template_size::get_instance().template_size_map[e_JewelTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_JewelTemplate].push_back("NextLevelItemId");
    }
}
