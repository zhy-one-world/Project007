#include "TimeLimitActivityBranchTemplate_S.h"

namespace hld
{
    void TimeLimitActivityBranchTemplateSize()
    {
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("ActivityType");
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("CycleIndex");
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("Condition");
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("Reward");
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("TextImage");
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("ParamArr1");
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("ParamArr2");
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("ParamArr3");
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("ParamArr4");
        template_size::get_instance().template_size_map[e_TimeLimitActivityBranchTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitActivityBranchTemplate].push_back("ParamArr5");
    }
}
