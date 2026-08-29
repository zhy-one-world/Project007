#include "TitleTemplate_S.h"

namespace faith
{
    void TitleTemplateSize()
    {
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("HighTitleType");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("EndTimeType");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("HighTitleName");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("HighTitleString");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("HighTitleIcon");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("HighTitleChatIcon");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("AdditionArray");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("StaticAttArray");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("LastTime");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("DeadActiveLine");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("NeedValue1");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("NeedValue2");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("Noticeld");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("ChannelLimit");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("PlayerType");
        template_size::get_instance().template_size_map[e_TitleTemplate].push_back("string");
        template_size::get_instance().template_key_map[e_TitleTemplate].push_back("AniStrId");
    }
}
