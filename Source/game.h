#pragma once

#include <glm/glm.hpp>

#include <polos/context/texture.h>
#include <polos/context/shader.h>

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
        polos::TextureRef m_CharacterSpriteSheet;
        polos::Shader*    m_TextureShader;
        glm::mat4         m_Model;
        glm::mat4         m_ScaledModel;
    };
} // namespace polosformer