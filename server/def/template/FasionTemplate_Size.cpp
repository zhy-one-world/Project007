#include "FasionTemplate_S.h"

namespace hld
{
    void FasionTemplateSize()
    {
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("EquipSlot");
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("AttArray");
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("MeshID");
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("BuffIdArray");
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("FeaturesDes");
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("UpgradeMaterial");
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("upgrade_num");
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("GradeLimit");
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("RegionArr");
        template_size::get_instance().template_size_map[e_FasionTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_FasionTemplate].push_back("NeedFasionId");
    }
}
