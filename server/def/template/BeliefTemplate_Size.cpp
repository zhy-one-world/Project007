#include "BeliefTemplate_S.h"

namespace faith
{
    void BeliefTemplateSize()
    {
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("BeliefType");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("ActivateLevelRequire");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("CurStarNumber");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("CurRankNumber");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("MaxStarNumber");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("MaxRankNumber");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("AttArray");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("OperateType");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("SuccessRatio");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("MinActivateTimeNeed");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("MaxActivateTimeLimit");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("MoneyCostList");
        template_size::get_instance().template_size_map[e_BeliefTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_BeliefTemplate].push_back("Notice_id");
    }
}
