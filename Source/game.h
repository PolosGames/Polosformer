#pragma once

#include <glm/glm.hpp>

#include <polos/graphics/texture.h>
#include <polos/graphics/shader.h>
#include <polos/core/scene/scene.h>
#include <polos/core/camera.h>
#include <polos/core/events/events.h>

namespace polosformer
{
    class Game
    {
    public:
        Game();
        ~Game();
    public:
        void Update(float p_DeltaTime);
    private:
        void UpdateCamera(float p_DeltaTime);

        void OnKeyPress(polos::key_press& p_Event);
        void OnKeyRelease(polos::key_release& p_Event);
    private:
        polos::Shader* m_TextureShader{};

        polos::Scene   m_MainScene;
        polos::Camera  m_SceneCamera;
        int32_t        m_Key{};

        polos::ecs::Entity m_Character{INVALID_ENTITY};

    };
} // namespace polosformer
