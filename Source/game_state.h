#pragma once

#include <polos/core/scene/scene.h>

namespace polosformer
{
    enum class PlayState
    {
        k_LevelStart,
        k_Play,
        k_Pause,
        k_Over
    };

    struct game_state
    {
        polos::Scene* currentScene;
        PlayState playState;
    };
} // namespace polosformer