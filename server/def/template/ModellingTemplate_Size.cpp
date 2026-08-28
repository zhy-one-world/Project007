#include "ModellingTemplate_S.h"

namespace hld
{
    void ModellingTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("Class");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("BodyMeshRes");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("EyeBrow");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("EyeData");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("NoseData");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("MouseData");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("ChinData");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("Faceprofile");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("HairID");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("HairParams1");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("HairParams2");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("HairParams3");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("FaceProfData");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("EyeBallData");
        template_size::get_instance().template_size_map[e_ModellingTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_ModellingTemplate].push_back("SkinData");
    }
}
