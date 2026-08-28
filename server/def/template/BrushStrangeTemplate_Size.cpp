#include "BrushStrangeTemplate_S.h"

namespace hld
{
    void BrushStrangeTemplateSize()
    {
        template_size::get_instance().template_size_map[e_BrushStrangeTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_BrushStrangeTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_BrushStrangeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BrushStrangeTemplate].push_back("WaveId");
        template_size::get_instance().template_size_map[e_BrushStrangeTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BrushStrangeTemplate].push_back("NextTime");
        template_size::get_instance().template_size_map[e_BrushStrangeTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_BrushStrangeTemplate].push_back("LiveTime");
        template_size::get_instance().template_size_map[e_BrushStrangeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BrushStrangeTemplate].push_back("IsDead");
        template_size::get_instance().template_size_map[e_BrushStrangeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BrushStrangeTemplate].push_back("WaveBornTriggerMessage");
        template_size::get_instance().template_size_map[e_BrushStrangeTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_BrushStrangeTemplate].push_back("WaveKillTriggerMessage");
    }
}
