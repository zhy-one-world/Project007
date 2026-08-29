#include "PersonalSignatureTemplate_S.h"

namespace faith
{
    void PersonalSignatureTemplateSize()
    {
        template_size::get_instance().template_size_map[e_PersonalSignatureTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_PersonalSignatureTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_PersonalSignatureTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PersonalSignatureTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_PersonalSignatureTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PersonalSignatureTemplate].push_back("Sort");
        template_size::get_instance().template_size_map[e_PersonalSignatureTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PersonalSignatureTemplate].push_back("StringID");
        template_size::get_instance().template_size_map[e_PersonalSignatureTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_PersonalSignatureTemplate].push_back("answer");
    }
}
