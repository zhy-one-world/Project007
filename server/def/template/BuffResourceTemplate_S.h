#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void BuffResourceTemplateSize();
#pragma pack(push, 1)
    struct BuffResourceTemplate : public template_base
    {
        int32 Icon;
        std::vector<int32> Effects;
        std::vector<int32> ComponentIndex;
        std::vector<std::string> Sockets;
        std::vector<int32> EffectsX;
        std::vector<int32> EffectsY;
        std::vector<int32> EffectsZ;
        std::vector<int32> EffectsRoll;
        std::vector<int32> EffectsPitch;
        std::vector<int32> EffectsYaw;
        int32 ColorR;
        int32 ColorG;
        int32 ColorB;
        int32 ColorA;
        int32 MeshName;
        std::string MeshSlot;
        int32 MeshAnim;
        std::vector<float> MeshLocation;
        std::vector<float> MeshRotation;
        int32 NpcId;
        float ScaleX;
        float ScaleY;
        float ScaleZ;
        float MoveLength;
        float MoveTime;
        float MoveAcceleration;
        int32 ActionType;
        int32 IsStealth ;
        int32 IsPose;
        std::vector<int32> ShapeshiftId;
        int32 ChangeBase;
        int32 GhostNum;
        int32 GhostMesh;
        int32 GhostBp;
        std::vector<int32> GhostShapeshiftId;
        int32 IsChangeMount;
        int32 IsPlayChangeMountAnim;
        int32 IsChangeWing;
        int32 IsPlayChangeWingAnim;
        int32 IsPlayChangePatronSaintAnim;
        int32 ShowIcon;
        int32 BuffName;
        int32 BuffDescription;
        std::vector<float> FriendLegionSeeColor;
        std::vector<float> EnemyLegionSeeColor;
        std::vector<int32> TrailParticleEffects;
        std::vector<int32> TrailComponentIndex;
        std::vector<std::string> TrailParticleSockets;
        std::string AdsorbentSocket;
        std::vector<float> CameraVague;
        int32 IsShowBuffWithFashion;
        int32 ChangeShapeId;
    };
#pragma pack(pop)
}
