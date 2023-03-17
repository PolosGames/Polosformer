#pragma once

#include "polos/core/ecs/common_set.h"

namespace polos::ecs
{
    struct animation_set : CommonSet
    {
        COMMONSET_REGISTER(
            animation_set,
            polos::ecs::animator_component
        )

        animation_set(polos::ecs::Entity p_Entity, polos::ecs::animator_component* p_Animator)
            : entity{p_Entity}, animatorComponent{p_Animator}
        {}

        Entity              entity;
        animator_component* animatorComponent;
    };
} // namespace polos::ecs
