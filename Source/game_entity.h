#pragma once

#include <polos/core/ecs/components/components.h>

namespace polosformer
{
    enum EntityDirection : int32_t
    {
        k_Left = -1,
        k_Right = 1
    };

    struct game_entity
    {
        polos::ecs::Entity id;

        polos::ecs::info_component*      infoComp{};
        polos::ecs::transform_component* transformComp{};
        polos::ecs::texture2d_component* texture2dComp{};
        polos::ecs::material_component*  materialComp{};
        polos::ecs::animator_component*  animatorComp{};

        EntityDirection direction{};
    };
} // namespace polosformer