#include "PowerUpTargetTemplate_S.h"

namespace faith
{
    void PowerUpTargetTemplateSize()
    {
        template_size::get_instance().template_size_map[e_PowerUpTargetTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_PowerUpTargetTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_PowerUpTargetTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_PowerUpTargetTemplate].push_back("BasicInfoArray");
        template_size::get_instance().template_size_map[e_PowerUpTargetTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_PowerUpTargetTemplate].push_back("WidgetJumpArray");
        template_size::get_instance().template_size_map[e_PowerUpTargetTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PowerUpTargetTemplate].push_back("PagingType");
        template_size::get_instance().template_size_map[e_PowerUpTargetTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PowerUpTargetTemplate].push_back("TargetTyp");
        template_size::get_instance().template_size_map[e_PowerUpTargetTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PowerUpTargetTemplate].push_back("OpenLevel");
        template_size::get_instance().template_size_map[e_PowerUpTargetTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_PowerUpTargetTemplate].push_back("ReqFuncUnlockString");
    }
}
