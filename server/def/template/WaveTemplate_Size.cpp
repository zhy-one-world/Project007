#include "WaveTemplate_S.h"

namespace hld
{
    void WaveTemplateSize()
    {
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("MiniMapShowNpcId");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("NpcId");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("BornX");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("BornY");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("BornZ");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("BornYaw");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("BornPitch");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("BornRoll");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("BornTime");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("PlotTips");
        template_size::get_instance().template_size_map[e_WaveTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_WaveTemplate].push_back("FirstPlotTips");
    }
}
