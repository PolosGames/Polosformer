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

namespace polosformer
{
    class Game
    {
    public:
        Game();
        ~Game();
    public:
        void Update(float p_DeltaTime);

        static game_state& GetCurrentGameState();
    private:
        void UpdateCamera(float p_DeltaTime);

        void OnKeyPress(polos::key_press& p_Event);
        void OnKeyRelease(polos::key_release& p_Event);

        void LoadSprites();
    private:
        static game_state m_GameState;

        AnimationManager m_AnimManager;

        std::unique_ptr<polos::base_animation> m_CharacterAnimIdle_1;
        std::unique_ptr<polos::base_animation> m_CharacterAnimIdle_2;
        std::unique_ptr<polos::base_animation> m_CharacterAnimAttack_1;
        std::unique_ptr<polos::base_animation> m_CharacterAnimAttack_2;
        std::unique_ptr<polos::base_animation> m_CharacterAnimAttack_3;

        polos::Shader* m_TextureShader{};

        polos::Scene   m_MainScene;
        polos::Camera  m_SceneCamera;
        int32_t        m_Key{};

        polos::ecs::Entity m_Character{INVALID_ENTITY};

    };
} // namespace polosformer
