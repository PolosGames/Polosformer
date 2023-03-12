
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

#include "anim_type.h"
#include "events/scene_change.h"

namespace polosformer
{
    game_state Game::m_GameState;

    Game::Game()
        : m_SceneCamera{{0.0f, 0.0f, 5.0f}, {0.0f, 1.0f, 0.0f}}
    {
        UPDATE_Q_MEM_ADD_LAST(Update);
        SUB_TO_EVENT_MEM_FUN(polos::key_press, OnKeyPress);
        SUB_TO_EVENT_MEM_FUN(polos::key_release, OnKeyRelease);
        SUB_TO_EVENT_MEM_FUN(polos::mouse_button_press, OnMouseButtonPress);
        SUB_TO_EVENT_MEM_FUN(polos::animation_over, OnAnimationOver);


        LoadSprites();

        m_GameState.currentScene = &m_MainScene;
        m_GameState.playState    = PlayState::k_LevelStart;
        polos::EventBus::RaiseEvent<polos::scene_change>(m_GameState.currentScene);

        m_Character = m_MainScene.NewEntity();

        constexpr float anim_sprite_width{50.0f};
        constexpr float anim_sprite_height{37.0f};
        constexpr float character_scale{0.2f};

        constexpr float character_scale_ratio{ anim_sprite_width / anim_sprite_height };

        namespace ecs = polos::ecs;

        auto* character_info_comp = m_MainScene.Assign<ecs::info_component>(m_Character);
        character_info_comp->name = "Character";
        character_info_comp->isVisible = true;

        auto* character_transform_comp = m_MainScene.Assign<ecs::transform_component>(m_Character);
        character_transform_comp->scale = glm::vec3(character_scale_ratio * character_scale, character_scale, 1.0f);
        
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

        auto* animator_comp = m_MainScene.Get<polos::ecs::animator_component>(m_Character);
        auto* texture2d_comp = m_MainScene.Get<polos::ecs::texture2d_component>(m_Character);
        //auto* info_comp = m_MainScene.Get<polos::ecs::info_component>(entity);

        texture2d_comp->texture = animator_comp->currentAnimation->frames[animator_comp->currentFrame];

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

    void Game::OnMouseButtonPress(polos::mouse_button_press& p_Event)
    {
        if (p_Event.button == GLFW_MOUSE_BUTTON_LEFT)
        {
            m_AnimManager.ChangeAnimation(m_Character, m_CharacterAnimAttack_1.get());
        }
    }

    void Game::OnAnimationOver(polos::animation_over& p_Event)
    {
        if (p_Event.entity == m_Character)
        {
            if (p_Event.anim->id == static_cast<int64_t>(AnimationType::k_Attack_1))
            {
                m_AnimManager.ChangeAnimation(m_Character, m_CharacterAnimIdle_2.get());
            }
        }
    }

    void Game::LoadSprites()
    {
        m_CharacterAnimIdle_1     = std::unique_ptr<polos::animation<4>>(new polos::animation<4>);
        m_CharacterAnimIdle_1->id = static_cast<int64_t>(AnimationType::k_Idle_1);
        for (std::size_t i{}; i < m_CharacterAnimIdle_1->frames.capacity(); i++)
        {
            m_CharacterAnimIdle_1->frames.push_back(
                polos::Texture::Load(std::format("resources/textures/character/adventurer-idle-0{}.png", i))
            );
        }

        m_CharacterAnimIdle_2     = std::unique_ptr<polos::animation<4>>(new polos::animation<4>);
        m_CharacterAnimIdle_2->id = static_cast<int64_t>(AnimationType::k_Idle_2);
        for (std::size_t i{}; i < m_CharacterAnimIdle_2->frames.capacity(); i++)
        {
            m_CharacterAnimIdle_2->frames.push_back(
                polos::Texture::Load(std::format("resources/textures/character/adventurer-idle-2-0{}.png", i))
            );
        }

        m_CharacterAnimAttack_1       = std::unique_ptr<polos::animation<5>>(new polos::animation<5>);
        m_CharacterAnimAttack_1->id   = static_cast<int64_t>(AnimationType::k_Attack_1);
        m_CharacterAnimAttack_1->loop = false;
        for (std::size_t i{}; i < m_CharacterAnimAttack_1->frames.capacity(); i++)
        {
            m_CharacterAnimAttack_1->frames.push_back(
                polos::Texture::Load(std::format("resources/textures/character/adventurer-attack1-0{}.png", i))
            );
        }

        m_CharacterAnimAttack_2       = std::unique_ptr<polos::animation<6>>(new polos::animation<6>);
        m_CharacterAnimAttack_2->id   = static_cast<int64_t>(AnimationType::k_Attack_2);
        m_CharacterAnimAttack_2->loop = false;
        for (std::size_t i{}; i < m_CharacterAnimAttack_2->frames.capacity(); i++)
        {
            m_CharacterAnimAttack_2->frames.push_back(
                polos::Texture::Load(std::format("resources/textures/character/adventurer-attack2-0{}.png", i))
            );
        }

        m_CharacterAnimAttack_3       = std::unique_ptr<polos::animation<4>>(new polos::animation<4>);
        m_CharacterAnimAttack_3->id   = static_cast<int64_t>(AnimationType::k_Attack_3);
        m_CharacterAnimAttack_3->loop = false;
        for (std::size_t i{}; i < m_CharacterAnimAttack_3->frames.capacity(); i++)
        {
            m_CharacterAnimAttack_3->frames.push_back(
                polos::Texture::Load(std::format("resources/textures/character/adventurer-attack3-0{}.png", i))
            );
        }
    }
} // namespace polosformer
