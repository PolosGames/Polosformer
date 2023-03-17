#pragma once

#include <polos/core/events/event.h>

#include "../game_entity.h"

namespace polos
{
    struct entity_move_start final : Event<entity_move_start>
    {
        entity_move_start() = default;
        entity_move_start(polosformer::game_entity p_Entity, polosformer::EntityDirection p_Direction)
            : entity{ std::move(p_Entity) }
            , direction{ p_Direction }
        {}

        polosformer::game_entity     entity;
        polosformer::EntityDirection direction;
    };
} // namespace polos::ecs
