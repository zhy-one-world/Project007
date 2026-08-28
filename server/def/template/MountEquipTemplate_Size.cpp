#include "MountEquipTemplate_S.h"

namespace hld
{
    void MountEquipTemplateSize()
    {
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("EquipSlot");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("StarNum");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("StarMax");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("UpgradeAttEach");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("StarCost");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("IsOnly");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("Mount");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("StarMountLevel");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("SkillType");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("SkillId");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("SkillMountLevel");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("SlotID");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("BaseAtt");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("CanEquipID");
        template_size::get_instance().template_size_map[e_MountEquipTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MountEquipTemplate].push_back("ItemGet");
    }
}
