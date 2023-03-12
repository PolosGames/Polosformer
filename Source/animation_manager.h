#pragma once

#include <polos/core/ecs/entity.h>
#include <polos/graphics/animation.h>

#include "events/scene_change.h"

namespace polosformer
{
    class AnimationManager
    {
    public:
        AnimationManager();
    public:
        void Update(float p_DeltaTime);

        void ChangeAnimation(polos::ecs::Entity p_Entity, polos::base_animation* p_Anim);
    private:
        void OnSceneChange(polos::scene_change& p_Event);
    private:
        polos::Scene* m_CurrentScene;
    };
} // namespace polosformer