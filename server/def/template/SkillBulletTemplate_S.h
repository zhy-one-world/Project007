#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void SkillBulletTemplateSize();
#pragma pack(push, 1)
    struct SkillBulletTemplate : public template_base
    {
        int32 MoveType;
        float Speed;
        float Acceleration;
        int32 MaxLength;
        int32 EffectImmediately;
        float Duration;
        float BeginRadius;
        float EndRadius;
        int32 Angle;
        int32 EffectBPName;
        int32 BornZ;
        std::vector<int32> EffectPos;
        std::vector<int32> RandomPoint;
        std::vector<int32> EffectRotation;
        int32 CreateOther;
        int32 HitRepeat;
        int32 HitNum;
        int32 AmmoType;
        int32 AmmoId;
    };
#pragma pack(pop)
}
