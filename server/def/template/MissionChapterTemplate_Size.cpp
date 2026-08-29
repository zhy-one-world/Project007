#include "MissionChapterTemplate_S.h"

namespace faith
{
    void MissionChapterTemplateSize()
    {
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("ChapterNum");
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("MaxChapterNum");
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("TotalMissionNum");
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("FirstMissionId");
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("LastMissionId");
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("vector<float>");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("AttValueArray");
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("ChapterNameId");
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("ChapterDescribeId");
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("ChapterShortDescribeId");
        template_size::get_instance().template_size_map[e_MissionChapterTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_MissionChapterTemplate].push_back("TinderID");
    }
}
