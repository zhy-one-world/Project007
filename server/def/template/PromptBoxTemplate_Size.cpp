#include "PromptBoxTemplate_S.h"

namespace hld
{
    void PromptBoxTemplateSize()
    {
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("ActivityType");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("TipStringID");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("RickStringID");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("ButtonStringID");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("ResID");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("GetWayWidgetJumpArray");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("TaskID");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("GoodsID");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("StoreType");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("IsActivityOpen");
        template_size::get_instance().template_size_map[e_PromptBoxTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_PromptBoxTemplate].push_back("MapId");
    }
}
