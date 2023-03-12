
#include "animation_manager.h"

#include <polos/core/update_queue.h>
#include <polos/core/scene/scene_view.h>
#include <polos/core/ecs/components/animator_component.h>
#include <polos/core/event_bus.h>
#include "events/animation_over.h"

#include "game.h"

namespace polosformer
{
    AnimationManager::AnimationManager()
    {
        UPDATE_Q_MEM_ADD_LAST(Update);
        SUB_TO_EVENT_MEM_FUN(polos::scene_change, OnSceneChange);
    }
    
    void AnimationManager::Update(float)
    {
        auto scene_view = polos::SceneView<polos::ecs::animator_component>(*m_CurrentScene);
        for (auto entity : scene_view)
        {
            auto* animator_comp = m_CurrentScene->Get<polos::ecs::animator_component>(entity);

            if (animator_comp->isStopped) continue;
            if (animator_comp->frameCounter == animator_comp->fps)
            {
                animator_comp->frameCounter = 0;
                ++animator_comp->currentFrame;
                if (animator_comp->currentFrame == animator_comp->currentAnimation->frames.size())
                {
                    if (!animator_comp->currentAnimation->loop)
                    {
                        animator_comp->currentFrame = 0;
                        polos::EventBus::RaiseEvent<polos::animation_over>(animator_comp->currentAnimation, entity);
                        continue;
                    }
                    animator_comp->currentFrame = 0;
                }
            }
            ++animator_comp->frameCounter;
        }
    }

    void AnimationManager::ChangeAnimation(polos::ecs::Entity p_Entity, polos::base_animation* p_Anim)
    {
        auto* animator_comp = m_CurrentScene->Get<polos::ecs::animator_component>(p_Entity);
        animator_comp->currentAnimation = p_Anim;
        animator_comp->currentFrame = 0;
        animator_comp->frameCounter = 0;
        animator_comp->isStopped = false;
    }
    
    void AnimationManager::OnSceneChange(polos::scene_change& p_Event)
    {
        m_CurrentScene = p_Event.scene;
    }
} // namespace polosformer
