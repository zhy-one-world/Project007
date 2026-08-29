#include "ConvertTemplate_S.h"

namespace faith
{
    void ConvertTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ConvertTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ConvertTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ConvertTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ConvertTemplate].push_back("ConvertType");
        template_size::get_instance().template_size_map[e_ConvertTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ConvertTemplate].push_back("CostMoneyArray");
        template_size::get_instance().template_size_map[e_ConvertTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ConvertTemplate].push_back("StaticRewardMoneyArray");
        template_size::get_instance().template_size_map[e_ConvertTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ConvertTemplate].push_back("DyamicRewardMoneyArray");
        template_size::get_instance().template_size_map[e_ConvertTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ConvertTemplate].push_back("AdditionalMoneyArray");
        template_size::get_instance().template_size_map[e_ConvertTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ConvertTemplate].push_back("RegionArr");
        template_size::get_instance().template_size_map[e_ConvertTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ConvertTemplate].push_back("ExperienceConversionTimes");
        template_size::get_instance().template_size_map[e_ConvertTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ConvertTemplate].push_back("RedDiamondFreeConvert");
        template_size::get_instance().template_size_map[e_ConvertTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ConvertTemplate].push_back("FreeChangeGoldDistanceSecond");
    }
}
