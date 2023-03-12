#pragma once

#include <polos/core/events/event.h>
#include <polos/graphics/animation.h>

namespace polos
{
    struct animation_over final : Event<animation_over>
    {
        animation_over() = default;
        animation_over(base_animation* p_Animation, ecs::Entity p_Entity)
            : anim{ p_Animation }
            , entity{ p_Entity }
        {}

        base_animation* anim{};
        ecs::Entity     entity{INVALID_ENTITY};
    };
} // namespace polos
