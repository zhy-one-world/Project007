#include "OracleTrialTemplate_S.h"

namespace faith
{
    void OracleTrialTemplateSize()
    {
        template_size::get_instance().template_size_map[e_OracleTrialTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_OracleTrialTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_OracleTrialTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_OracleTrialTemplate].push_back("MapId");
        template_size::get_instance().template_size_map[e_OracleTrialTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_OracleTrialTemplate].push_back("AwardNum");
        template_size::get_instance().template_size_map[e_OracleTrialTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_OracleTrialTemplate].push_back("AwardId");
    }
}
