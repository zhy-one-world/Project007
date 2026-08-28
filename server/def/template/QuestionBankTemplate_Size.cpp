#include "QuestionBankTemplate_S.h"

namespace hld
{
    void QuestionBankTemplateSize()
    {
        template_size::get_instance().template_size_map[e_QuestionBankTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_QuestionBankTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_QuestionBankTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_QuestionBankTemplate].push_back(" topic");
        template_size::get_instance().template_size_map[e_QuestionBankTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_QuestionBankTemplate].push_back(" answer");
        template_size::get_instance().template_size_map[e_QuestionBankTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_QuestionBankTemplate].push_back(" ItemRewardArray");
        template_size::get_instance().template_size_map[e_QuestionBankTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_QuestionBankTemplate].push_back("BattleRewardArray");
    }
}
