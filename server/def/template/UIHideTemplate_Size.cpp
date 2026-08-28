#include "UIHideTemplate_S.h"

namespace hld
{
    void UIHideTemplateSize()
    {
        template_size::get_instance().template_size_map[e_UIHideTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_UIHideTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_UIHideTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_UIHideTemplate].push_back("ParentWidgetName");
        template_size::get_instance().template_size_map[e_UIHideTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_UIHideTemplate].push_back("ChildWidgetsName");
        template_size::get_instance().template_size_map[e_UIHideTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_UIHideTemplate].push_back("ChildWidgetsShowNeedFuncUnlock");
        template_size::get_instance().template_size_map[e_UIHideTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_UIHideTemplate].push_back("ChildWidgetsHideSetting");
    }
}
