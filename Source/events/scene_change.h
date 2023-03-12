#pragma once

#include <polos/core/events/event.h>
#include <polos/core/scene/scene.h>

namespace polos
{
    struct scene_change final : Event<scene_change>
    {
        scene_change() = default;
        scene_change(Scene* p_Scene) : scene{p_Scene} {}

        Scene* scene{};
    };
} // namespace polos