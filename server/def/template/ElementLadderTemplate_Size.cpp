#include "ElementLadderTemplate_S.h"

namespace hld
{
    void ElementLadderTemplateSize()
    {
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("RegionType");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("SeasonNum");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("RankName");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("RankIcon");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("LowScore");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("HighScore");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("SeasonReward");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("ReservedScore");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("BigGradeFlag");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("ExpandTime");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("UpgradeReward");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("TitleReward");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("RobotRate");
        template_size::get_instance().template_size_map[e_ElementLadderTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_ElementLadderTemplate].push_back("RobotTemplateID");
    }
}
