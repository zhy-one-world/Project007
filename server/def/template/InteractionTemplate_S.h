#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void InteractionTemplateSize();
#pragma pack(push, 1)
    struct InteractionTemplate : public template_base
    {
        int32 ClassType;
        int32 InteractionType;
        int32 CorrespondInteractionType;
        int32 InitiativeGo;
        int32 InviteRange;
        int32 InteractionRange;
        std::vector<int32> HeadOffset;
        int32 InviteTimeLimit;
        std::string SocketName;
        int32 AttachToOther;
        int32 CanMove;
        int32 InteractionPeopleNumber;
        int32 InviteString;
        int32 InteractionName;
        int32 InteractionDescription;
        int32 InteractionIcon;
        int32 CanInitiatorView;
        int32 IsAllowSingle;
        std::vector<int32> InteractiveLimilt;
    };
#pragma pack(pop)
}
