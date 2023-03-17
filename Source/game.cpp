
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
#include "entity_direction.h"
#include "sets/character_set.h"

namespace polosformer
{
    game_state Game::s_GameState;

    Game::Game()
        : m_SceneCamera{{0.0f, 0.0f, 5.0f}, {0.0f, 1.0f, 0.0f}}
    {
        UPDATE_Q_MEM_ADD_LAST(Update);
        SUB_TO_EVENT_MEM_FUN(polos::key_press, OnKeyPress);
        SUB_TO_EVENT_MEM_FUN(polos::key_release, OnKeyRelease);
        SUB_TO_EVENT_MEM_FUN(polos::mouse_button_press, OnMouseButtonPress);
        SUB_TO_EVENT_MEM_FUN(polos::animation_over, OnAnimationOver);

        LoadSprites();

        m_Scenes.emplace_back();
        m_CurrentScene = 0;

        s_GameState.currentScene = &m_Scenes[m_CurrentScene];
        s_GameState.playState    = PlayState::k_LevelStart;
        polos::EventBus::RaiseEvent<polos::scene_change>(s_GameState.currentScene);

        m_Character = m_Scenes[m_CurrentScene].NewEntity();

        constexpr float anim_sprite_width{50.0f};
        constexpr float anim_sprite_height{37.0f};
        constexpr float character_scale{0.2f};

        constexpr float character_scale_ratio{ anim_sprite_width / anim_sprite_height };

        namespace ecs = polos::ecs;

        auto* info_comp = m_Scenes[m_CurrentScene].Assign<ecs::info_component>(m_Character);
        info_comp->name = "Character";
        info_comp->isVisible = true;

        auto* transform_comp = m_Scenes[m_CurrentScene].Assign<ecs::transform_component>(m_Character);
        transform_comp->scale = glm::vec3(character_scale_ratio * character_scale, character_scale, 1.0f);

        auto* texture2d_comp = m_Scenes[m_CurrentScene].Assign<ecs::texture2d_component>(m_Character);
        texture2d_comp->texture = m_CharacterAnimIdle_1->frames[0];

        auto* material_comp = m_Scenes[m_CurrentScene].Assign<ecs::material_component>(m_Character);
        material_comp->shader = &polos::ShaderLib::Get("texture"_sid);

        auto* animator_comp = m_Scenes[m_CurrentScene].Assign<ecs::animator_component>(m_Character);
        animator_comp->fps = 20;
        animator_comp->currentAnimation = m_CharacterAnimIdle_1.get();
    }

    Game::~Game()
    {}

    void Game::Update(float p_DeltaTime)
    {
        UpdateCamera(p_DeltaTime);
        UpdateCharacter(p_DeltaTime);
        UpdateEntities(p_DeltaTime);
        UpdateAnimationFrame(m_Character);

        polos::Renderer::RenderScene(m_Scenes[m_CurrentScene]);
    }

    game_state& Game::GetCurrentGameState()
    {
        return s_GameState;
    }

    void Game::UpdateCamera(float)
    {
        
    }

    void Game::UpdateEntities(float const)
    {
        for (auto [transform_comp] : polos::SceneView<polos::ecs::transform_component>(m_Scenes[m_CurrentScene]))
        {

        }
    }

    void Game::UpdateCharacter(float const p_DeltaTime)
    {
        float x_move_scalar{ 1.0f };

        for (auto character_set : polos::SceneView<polos::ecs::character_set>(m_Scenes[m_CurrentScene]))
        {
            int32_t x_axis_pressed_key = (m_Key & Key::k_D) | (m_Key & Key::k_A);
            int32_t should_move = 0;
            if (x_axis_pressed_key == Key::k_D)
            {
                x_move_scalar = 1.0f;
                should_move = 1;
            }
            else if (x_axis_pressed_key == Key::k_A)
            {
                x_move_scalar = -1.0f;
                should_move = 1;
            }

            character_set.transformComponent->position += glm::vec3(k_CharacterSpeed * p_DeltaTime * x_move_scalar * should_move, 0.0f, 0.0f);
            float x_scale = glm::abs(character_set.transformComponent->scale.x) * static_cast<int32_t>(x_move_scalar);
            character_set.transformComponent->scale.x = x_scale;
        } 
    }
    
    void Game::OnKeyPress(polos::key_press& p_Event)
    {
        int32_t current_key{};

        switch (p_Event.key)
        {
        case GLFW_KEY_W:
            current_key = Key::k_W;
            break;
        case GLFW_KEY_S:
            current_key = Key::k_S;
            break;
        case GLFW_KEY_A:
            current_key = Key::k_A;
            break;
        case GLFW_KEY_D:
            current_key = Key::k_D;
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
            current_key = Key::k_W;
            break;
        case GLFW_KEY_S:
            current_key = Key::k_S;
            break;
        case GLFW_KEY_A:
            current_key = Key::k_A;
            break;
        case GLFW_KEY_D:
            current_key = Key::k_D;
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

        m_CharacterAnimRun     = std::unique_ptr<polos::animation<6>>(new polos::animation<6>);
        m_CharacterAnimRun->id = static_cast<int64_t>(AnimationType::k_Run);
        for (std::size_t i{}; i < m_CharacterAnimRun->frames.capacity(); i++)
        {
            m_CharacterAnimRun->frames.push_back(
                polos::Texture::Load(std::format("resources/textures/character/adventurer-run-0{}.png", i))
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
    
    void Game::UpdateAnimationFrame(polos::ecs::Entity p_Entity)
    {
        auto* animator_comp = m_Scenes[m_CurrentScene].Get<polos::ecs::animator_component>(p_Entity);
        auto* texture2d_comp = m_Scenes[m_CurrentScene].Get<polos::ecs::texture2d_component>(p_Entity);
        if (animator_comp != nullptr && texture2d_comp != nullptr)
        {
            texture2d_comp->texture = animator_comp->currentAnimation->frames[animator_comp->currentFrame];
        }
    }
} // namespace polosformer
