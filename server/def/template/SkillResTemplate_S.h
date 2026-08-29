#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void SkillResTemplateSize();
#pragma pack(push, 1)
    struct SkillResTemplate : public template_base
    {
        std::vector<int32> IconInfo;
        int32 HurtSoundType;
        int32 HurtSound;
        std::vector<int32> FlyEffNomal;
        std::vector<int32> FlyEffIce;
        std::vector<int32> FlyEffFire;
        std::vector<int32> FlyEffLight;
        std::vector<int32> FlyEffDark;
        std::vector<int32> HurtEff;
        int32 NoOpenStrId;
        std::string SpellSections;
        std::string CastSections;
        std::string ChannelSections;
        std::string AttackSections;
        std::string HurtSections;
        int32 MotionBlendSpaceId;
        std::string OtherSpellSections;
        std::string OtherCastSections;
        std::string OtherChannelSections;
        std::string OtherAttackSections;
        std::string OtherHurtSections;
        int32 SkillName;
        int32 SkillTemplate;
        int32 SkillDiscribe;
        int32 SkillSmallDiscribe;
        int32 SkillUnlockDescribe;
        int32 SkillRecommendDescribe;
        int32 SkillUnlockConditionDescribe;
        float LightStrength;
        float LightR;
        float LightG;
        float LightB;
        float LightA;
        int32 ExploreAtCapsule;
        int32 SkillVedio;
        int32 CacheNpcId;
        float SkillVedioLen;
    };
#pragma pack(pop)
}
