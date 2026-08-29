#include "TimeLimitGiftTemplate_S.h"

namespace faith
{
    void TimeLimitGiftTemplateSize()
    {
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("TriggerTime");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("MaxTriggerNum");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("MaxBuyTime");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("LifeTime");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("ChangeTime");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("TextImage");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("Reward");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("ConsumeType");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("MoneyList");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("TriggerType");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("NeedLevel");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("NeedMission");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("MapWinList");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("FuncLock");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("GradeLevel");
        template_size::get_instance().template_size_map[e_TimeLimitGiftTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_TimeLimitGiftTemplate].push_back("SpecialTriggerArray");
    }
}
