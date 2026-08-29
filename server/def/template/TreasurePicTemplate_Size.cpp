#include "TreasurePicTemplate_S.h"

namespace faith
{
    void TreasurePicTemplateSize()
    {
        template_size::get_instance().template_size_map[e_TreasurePicTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_TreasurePicTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_TreasurePicTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TreasurePicTemplate].push_back("AwardType");
        template_size::get_instance().template_size_map[e_TreasurePicTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TreasurePicTemplate].push_back("AwardIcon");
        template_size::get_instance().template_size_map[e_TreasurePicTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TreasurePicTemplate].push_back("ExpLevel");
        template_size::get_instance().template_size_map[e_TreasurePicTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TreasurePicTemplate].push_back("DropIdList");
    }
}
