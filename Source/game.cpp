
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

        s_GameState.currentScene = &m_MainScene;
        s_GameState.playState    = PlayState::k_LevelStart;
        polos::EventBus::RaiseEvent<polos::scene_change>(s_GameState.currentScene);

        m_Character.id = m_MainScene.NewEntity();

        constexpr float anim_sprite_width{50.0f};
        constexpr float anim_sprite_height{37.0f};
        constexpr float character_scale{0.2f};

        constexpr float character_scale_ratio{ anim_sprite_width / anim_sprite_height };

        namespace ecs = polos::ecs;

        m_Character.infoComp = m_MainScene.Assign<ecs::info_component>(m_Character.id);
        m_Character.infoComp->name = "Character";
        m_Character.infoComp->isVisible = true;

        m_Character.transformComp = m_MainScene.Assign<ecs::transform_component>(m_Character.id);
        m_Character.transformComp->scale = glm::vec3(character_scale_ratio * character_scale, character_scale, 1.0f);
        
        m_Character.texture2dComp = m_MainScene.Assign<ecs::texture2d_component>(m_Character.id);
        m_Character.texture2dComp->texture = m_CharacterAnimIdle_1->frames[0];

        m_Character.materialComp = m_MainScene.Assign<ecs::material_component>(m_Character.id);
        m_Character.materialComp->shader = &polos::ShaderLib::Get("texture"_sid);
        
        m_Character.animatorComp = m_MainScene.Assign<ecs::animator_component>(m_Character.id);
        m_Character.animatorComp->fps = 20;
        m_Character.animatorComp->currentAnimation = m_CharacterAnimIdle_1.get();

        m_Character.direction = EntityDirection::k_Right;
    }

    Game::~Game()
    {}

    void Game::Update(float p_DeltaTime)
    {
        UpdateCamera(p_DeltaTime);
        UpdateCharacter(p_DeltaTime);

        auto scene_view = polos::SceneView<polos::ecs::animator_component, polos::ecs::texture2d_component>(m_MainScene);

        //auto* info_comp = m_MainScene.Get<polos::ecs::info_component>(entity);

        UpdateAnimationFrame(m_Character);

        polos::Renderer::RenderScene(m_MainScene);
    }

    game_state& Game::GetCurrentGameState()
    {
        return s_GameState;
    }

    void Game::UpdateCamera(float)
    {
        
    }

    void Game::UpdateCharacter(float p_DeltaTime)
    {
        float move_scalar{ 0.0f };
        int32_t direction{ m_Character.direction };

        int32_t x_axis_pressed_key = (m_Key & Key::k_D) | (m_Key & Key::k_A);
        if (x_axis_pressed_key == Key::k_D)
        {
            move_scalar = 1.0f;
            direction = EntityDirection::k_Right;
        }
        else if (x_axis_pressed_key == Key::k_A)
        {
            move_scalar = -1.0f;
            direction = EntityDirection::k_Left;
        }
        m_Character.transformComp->position += glm::vec3(k_CharacterSpeed * p_DeltaTime * move_scalar, 0.0f, 0.0f);
        float x_scale = glm::abs(m_Character.transformComp->scale.x) * direction;
        m_Character.transformComp->scale.x = x_scale;
        m_Character.direction = static_cast<EntityDirection>(direction);
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
        if (p_Event.entity == m_Character.id)
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
    
    void Game::UpdateAnimationFrame(game_entity& p_Entity)
    {
        if (p_Entity.animatorComp != nullptr && p_Entity.texture2dComp != nullptr)
        {
            p_Entity.texture2dComp->texture = p_Entity.animatorComp->currentAnimation->frames[p_Entity.animatorComp->currentFrame];
        }
    }
} // namespace polosformer
