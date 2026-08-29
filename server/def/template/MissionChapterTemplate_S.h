#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void MissionChapterTemplateSize();
#pragma pack(push, 1)
    struct MissionChapterTemplate : public template_base
    {
        int32 ChapterNum;
        int32 MaxChapterNum;
        int32 TotalMissionNum;
        int32 FirstMissionId;
        int32 LastMissionId;
        std::vector<float> AttValueArray;
        int32 ChapterNameId;
        int32 ChapterDescribeId;
        int32 ChapterShortDescribeId;
        int32 TinderID;
    };
#pragma pack(pop)
}
