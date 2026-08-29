#include "QuestionMarryTemplate_S.h"

namespace faith
{
    void QuestionMarryTemplateSize()
    {
        template_size::get_instance().template_size_map[e_QuestionMarryTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_QuestionMarryTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_QuestionMarryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_QuestionMarryTemplate].push_back("topic");
        template_size::get_instance().template_size_map[e_QuestionMarryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_QuestionMarryTemplate].push_back("answer");
        template_size::get_instance().template_size_map[e_QuestionMarryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_QuestionMarryTemplate].push_back("RightAnswer");
        template_size::get_instance().template_size_map[e_QuestionMarryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_QuestionMarryTemplate].push_back("ItemRewardArray");
        template_size::get_instance().template_size_map[e_QuestionMarryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_QuestionMarryTemplate].push_back("BattleRewardArray");
        template_size::get_instance().template_size_map[e_QuestionMarryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_QuestionMarryTemplate].push_back("ItemRewardArray1");
        template_size::get_instance().template_size_map[e_QuestionMarryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_QuestionMarryTemplate].push_back("BattleRewardArray1");
    }
}
