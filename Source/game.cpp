#include "game.h"

#include <polos/core/update_queue.h>
#include <polos/core/log.h>
#include <polos/context/shader_lib.h>
#include <polos/renderer/renderer.h>

#include "entry_point.h"


namespace polosformer
{
    Game::Game()
    {
        UPDATE_Q_MEM_ADD_LAST(Update);

        polos::ShaderLib::Load("resources/shaders/uv_texture.vert", "resources/shaders/uv_texture.frag");

        m_CharacterSpriteSheet = polos::Texture::Load("resources/textures/adventurer-v1.5-Sheet.png");
        m_TextureShader        = &polos::ShaderLib::Get("uv_texture"_sid);
    }

    Game::~Game()
    {}

    void Game::Update(float p_DeltaTime)
    {
        polos::RenderTexture2D(m_ScaledModel, m_CharacterSpriteSheet, *m_TextureShader, );
    }
} // namespace polosformer