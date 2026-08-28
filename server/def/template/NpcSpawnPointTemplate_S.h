#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void NpcSpawnPointTemplateSize();
#pragma pack(push, 1)
    struct NpcSpawnPointTemplate : public template_base
    {
        int32 MapId;
        int32 TemplateId;
        int32 NpcType;
        int32 PosX;
        int32 PosY;
        int32 PosZ;
        int32 TpPosX;
        int32 TpPosY;
        int32 TpPosZ;
        int32 EnvironmentType;
        int32 NextPosX;
        int32 NextPosY;
        int32 NextPosZ;
        int32 Roll;
        int32 Pitch;
        int32 Yaw;
        int32 RespawnType;
        int32 RespawnTime;
        std::vector<int32> RespawnPeriod;
        std::vector<int32> RespawnByDayArray;
        int32 IsCreateByMapCreate;
        int32 Area;
        int32 SpawnRate;
        int32 NextSpawnNpcId;
    };
#pragma pack(pop)
}
