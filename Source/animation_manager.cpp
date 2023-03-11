
#include "animation_manager.h"

#include <polos/core/update_queue.h>
#include <polos/core/scene/scene_view.h>
#include <polos/core/ecs/components/animator_component.h>

#include "game.h"

namespace polosformer
{
    AnimationManager::AnimationManager()
    {
        UPDATE_Q_MEM_ADD_LAST(Update);
    }
    
    void AnimationManager::Update(float)
    {
        polos::Scene& scene = *Game::GetCurrentGameState().currentScene;
        auto scene_view = polos::SceneView<polos::ecs::animator_component>(scene);
        for (auto entity : scene_view)
        {
            auto* animator_comp = scene.Get<polos::ecs::animator_component>(entity);

            if (animator_comp->isStopped) continue;
            if (animator_comp->frameCounter == animator_comp->fps)
            {
                animator_comp->frameCounter = 0;
                ++animator_comp->currentFrame;
                if (animator_comp->currentFrame == animator_comp->currentAnimation->frames.size())
                {
                    if (!animator_comp->currentAnimation->loop)
                    {
                        animator_comp->isStopped = true;
                        --animator_comp->currentFrame;
                        continue;
                    }
                    animator_comp->currentFrame = 0;
                }
            }
            ++animator_comp->frameCounter;
        }
    }
} // namespace polosformer
