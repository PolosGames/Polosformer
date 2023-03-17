#pragma once

#include <glm/glm.hpp>

#include <polos/graphics/texture.h>
#include <polos/graphics/shader.h>
#include <polos/core/scene/scene.h>
#include <polos/core/camera.h>
#include <polos/core/events/events.h>
#include <polos/graphics/animation.h>

#include "game_state.h"
#include "animation_manager.h"
#include "events/animation_over.h"

namespace polosformer
{
    class Game
    {
    private:
        enum Key : int32_t
        {
            k_W = 1,
            k_S = 1 << 1,
            k_A = 1 << 2,
            k_D = 1 << 3,
        };

        static constexpr float k_CharacterSpeed{ 1.f };
    public:
        Game();
        ~Game();
    public:
        void Update(float p_DeltaTime);

        static game_state& GetCurrentGameState();
    private:
        void UpdateCamera(float p_DeltaTime);
        void UpdateEntities(float p_DeltaTime);
        void UpdateCharacter(float p_DeltaTime);

        void OnKeyPress(polos::key_press& p_Event);
        void OnKeyRelease(polos::key_release& p_Event);
        void OnMouseButtonPress(polos::mouse_button_press& p_Event);
        void OnAnimationOver(polos::animation_over& p_Event);

        void LoadSprites();

        void UpdateAnimationFrame(polos::ecs::Entity p_Entity);
    private:
        static game_state s_GameState;

        AnimationManager m_AnimManager;

        std::unique_ptr<polos::base_animation> m_CharacterAnimIdle_1;
        std::unique_ptr<polos::base_animation> m_CharacterAnimIdle_2;
        std::unique_ptr<polos::base_animation> m_CharacterAnimRun;
        std::unique_ptr<polos::base_animation> m_CharacterAnimAttack_1;
        std::unique_ptr<polos::base_animation> m_CharacterAnimAttack_2;
        std::unique_ptr<polos::base_animation> m_CharacterAnimAttack_3;

        polos::Shader* m_TextureShader{};

        std::vector<polos::Scene> m_Scenes;
        std::size_t               m_CurrentScene{};
        polos::Camera             m_SceneCamera;
        int32_t                   m_Key{};

        polos::ecs::Entity m_Character;
    };
} // namespace polosformer
