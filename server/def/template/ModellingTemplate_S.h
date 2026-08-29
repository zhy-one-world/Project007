#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ModellingTemplateSize();
#pragma pack(push, 1)
    struct ModellingTemplate : public template_base
    {
        int32 Class;
        int32 BodyMeshRes;
        std::vector<std::string> EyeBrow;
        std::vector<std::string> EyeData;
        std::vector<std::string> NoseData;
        std::vector<std::string> MouseData;
        std::vector<std::string> ChinData;
        std::vector<std::string> Faceprofile;
        std::vector<int32> HairID;
        std::vector<std::string> HairParams1;
        std::vector<std::string> HairParams2;
        std::vector<std::string> HairParams3;
        std::vector<int32> FaceProfData;
        std::vector<int32> EyeBallData;
        std::vector<std::string> SkinData;
    };
#pragma pack(pop)
}
