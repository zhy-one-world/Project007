#include "PokedexTemplate_S.h"

namespace faith
{
    void PokedexTemplateSize()
    {
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("PokedexName");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("PokedexIcon");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("Describe");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("OpenLevel");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("UnLockMissionID");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("NpcID");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("Color");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("Type");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("SubType");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("UpgradeLevel");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("MaxLevel");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("UnlockCost");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("UpgradeCost");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("AttArray");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("FetterArr");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("PsycheIdArray");
        template_size::get_instance().template_size_map[e_PokedexTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PokedexTemplate].push_back("ProtectId");
    }
}
