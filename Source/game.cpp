
#include "game.h"

#include <format>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <polos/core/update_queue.h>
#include <polos/core/log.h>
#include <polos/graphics/shader_lib.h>
#include <polos/graphics/renderer.h>
#include <polos/core/event_bus.h>
#include <polos/core/scene/scene_view.h>
#include <polos/core/ecs/components/components.h>

namespace polosformer
{
    game_state Game::m_GameState;

    Game::Game()
        : m_SceneCamera{{0.0f, 0.0f, 5.0f}, {0.0f, 1.0f, 0.0f}}
    {
        UPDATE_Q_MEM_ADD_LAST(Update);
        SUB_TO_EVENT_MEM_FUN(polos::key_press, OnKeyPress);
        SUB_TO_EVENT_MEM_FUN(polos::key_release, OnKeyRelease);

        LoadSprites();

        m_GameState.currentScene = &m_MainScene;
        m_GameState.playState    = PlayState::k_LevelStart;

        m_Character = m_MainScene.NewEntity();

        constexpr float anim_sprite_width{50};
        constexpr float anim_sprite_height{37};

        constexpr float character_scale_ratio{ anim_sprite_width / anim_sprite_height };

        namespace ecs = polos::ecs;

        auto* character_info_comp = m_MainScene.Assign<ecs::info_component>(m_Character);
        std::string name("Character");
        std::ranges::copy(name, character_info_comp->name);
        character_info_comp->isVisible = true;

        auto* character_transform_comp = m_MainScene.Assign<ecs::transform_component>(m_Character);
        character_transform_comp->scale = glm::vec3(character_scale_ratio, 1.0f, 1.0f);
        
        auto* character_texture2d_comp = m_MainScene.Assign<ecs::texture2d_component>(m_Character);
        character_texture2d_comp->texture = m_CharacterAnimIdle_1->frames[0];

        auto* character_material_comp  = m_MainScene.Assign<ecs::material_component>(m_Character);
        character_material_comp->shader = &polos::ShaderLib::Get("texture"_sid);
        
        auto* character_animation_comp = m_MainScene.Assign<ecs::animator_component>(m_Character);
        character_animation_comp->fps = 20;
        character_animation_comp->currentAnimation = m_CharacterAnimIdle_1.get();
    }

    Game::~Game()
    {}

    void Game::Update(float p_DeltaTime)
    {
        UpdateCamera(p_DeltaTime);

        auto scene_view = polos::SceneView<polos::ecs::animator_component, polos::ecs::texture2d_component>(m_MainScene);
        for (auto entity : scene_view)
        {
            auto* animator_comp = m_MainScene.Get<polos::ecs::animator_component>(entity);
            auto* texture2d_comp = m_MainScene.Get<polos::ecs::texture2d_component>(entity);
            //auto* info_comp = m_MainScene.Get<polos::ecs::info_component>(entity);

            texture2d_comp->texture = animator_comp->currentAnimation->frames[animator_comp->currentFrame];

            int a = 3; a = 4;
        }

        polos::Renderer::RenderScene(m_MainScene);
    }

    game_state& Game::GetCurrentGameState()
    {
        return m_GameState;
    }

    void Game::UpdateCamera(float p_DeltaTime)
    {
        using namespace polos;
        CameraMovement camera_move{ k_None };

        if ((m_Key & (1)) != 0)
        {
            camera_move = k_Up;
        }
        else if ((m_Key & (2)) != 0)
        {
            camera_move = k_Down;
        }
        m_SceneCamera.ProcessKeyboard(camera_move, p_DeltaTime);
        if ((m_Key & (4)) != 0)
        {
            camera_move = k_Left;
        }
        else if ((m_Key & (8)) != 0)
        {
            camera_move = k_Right;
        }
        m_SceneCamera.ProcessKeyboard(camera_move, p_DeltaTime);
    }
    
    void Game::OnKeyPress(polos::key_press& p_Event)
    {
        int32_t current_key{};

        switch (p_Event.key)
        {
        case GLFW_KEY_W:
            current_key = 1;
            break;
        case GLFW_KEY_S:
            current_key = 2;
            break;
        case GLFW_KEY_A:
            current_key = 4;
            break;
        case GLFW_KEY_D:
            current_key = 8;
            break;
        }

        m_Key |= current_key;
    }
    
    void Game::OnKeyRelease(polos::key_release& p_Event)
    {
        int32_t current_key{};

        switch (p_Event.key)
        {
        case GLFW_KEY_W:
            current_key = 1;
            break;
        case GLFW_KEY_S:
            current_key = 2;
            break;
        case GLFW_KEY_A:
            current_key = 4;
            break;
        case GLFW_KEY_D:
            current_key = 8;
            break;
        }

        m_Key &= ~current_key;
    }

    void Game::LoadSprites()
    {
        m_CharacterAnimIdle_1 = std::unique_ptr<polos::animation<4>>(new polos::animation<4>);

        for (std::size_t i{}; i < m_CharacterAnimIdle_1->frames.capacity(); i++)
        {
            m_CharacterAnimIdle_1->frames.push_back(
                polos::Texture::Load(std::format("resources/textures/character/adventurer-idle-0{}.png", i))
            );
        }
    }
} // namespace polosformer
