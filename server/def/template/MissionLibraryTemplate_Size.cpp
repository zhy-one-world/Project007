#include "MissionLibraryTemplate_S.h"

namespace faith
{
    void MissionLibraryTemplateSize()
    {
        template_size::get_instance().template_size_map[e_MissionLibraryTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_MissionLibraryTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_MissionLibraryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionLibraryTemplate].push_back("MissionLibraryType");
        template_size::get_instance().template_size_map[e_MissionLibraryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MissionLibraryTemplate].push_back("MissionLibraryArray");
        template_size::get_instance().template_size_map[e_MissionLibraryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionLibraryTemplate].push_back("FinishAwardType");
        template_size::get_instance().template_size_map[e_MissionLibraryTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_MissionLibraryTemplate].push_back("FinishEXP");
        template_size::get_instance().template_size_map[e_MissionLibraryTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_MissionLibraryTemplate].push_back("FinishGold");
        template_size::get_instance().template_size_map[e_MissionLibraryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MissionLibraryTemplate].push_back("AllDoneMoney");
        template_size::get_instance().template_size_map[e_MissionLibraryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MissionLibraryTemplate].push_back("AllDoneitem");
        template_size::get_instance().template_size_map[e_MissionLibraryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_MissionLibraryTemplate].push_back("AllDoneitemShow");
    }
}
