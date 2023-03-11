
#include "game.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <polos/core/update_queue.h>
#include <polos/core/log.h>
#include <polos/graphics/shader_lib.h>
#include <polos/graphics/renderer.h>
#include <polos/core/event_bus.h>

#include "entry_point.h"


namespace polosformer
{
    Game::Game()
        : m_SceneCamera{{0.0f, 0.0f, 5.0f}, {0.0f, 1.0f, 0.0f}}
    {
        UPDATE_Q_MEM_ADD_LAST(Update);
        SUB_TO_EVENT_MEM_FUN(polos::key_press, OnKeyPress);
        SUB_TO_EVENT_MEM_FUN(polos::key_release, OnKeyRelease);
        
        m_Character = m_MainScene.NewEntity();

        namespace ecs = polos::ecs;

        auto* character_transform_comp = m_MainScene.Assign<ecs::transform_component>(m_Character);
        auto* character_texture2d_comp = m_MainScene.Assign<ecs::texture2d_component>(m_Character);
        auto* character_material_comp  = m_MainScene.Assign<ecs::material_component>(m_Character);

        constexpr float anim_sprite_width{50};
        constexpr float anim_sprite_height{37};
        constexpr float sprite_sheet_width{350};
        constexpr float sprite_sheet_height{592};

        constexpr float character_scale_ratio{ anim_sprite_width / anim_sprite_height };
        constexpr float row_count{ sprite_sheet_width / anim_sprite_width };
        constexpr float column_count{ sprite_sheet_height / anim_sprite_height };

        constexpr int32_t row{1};
        constexpr int32_t column{2};

        character_transform_comp->scale = glm::vec3(character_scale_ratio, 1.0f, 1.0f);

        character_texture2d_comp->texture       = polos::Texture::Load("resources/textures/adventurer-v1.5-Sheet.png");
        character_texture2d_comp->uvBottomLeft  = glm::vec2((row * anim_sprite_width) / sprite_sheet_width, ((column_count - column) * anim_sprite_height) / sprite_sheet_height);
        character_texture2d_comp->uvBottomRight = glm::vec2(((row + 1) * anim_sprite_width) / sprite_sheet_width, ((column_count - column) * anim_sprite_height) / sprite_sheet_height);
        character_texture2d_comp->uvTopRight    = glm::vec2(((row + 1) * anim_sprite_width) / sprite_sheet_width, ((column_count - column + 1) * anim_sprite_height) / sprite_sheet_height);
        character_texture2d_comp->uvTopLeft     = glm::vec2(((row) * anim_sprite_width) / sprite_sheet_width, ((column_count - column + 1) * anim_sprite_height) / sprite_sheet_height);
        character_texture2d_comp->hasUvChanged  = true;

        character_material_comp->shader = &polos::ShaderLib::Get("texture"_sid);
    }

    Game::~Game()
    {}

    void Game::Update(float p_DeltaTime)
    {
        UpdateCamera(p_DeltaTime);

        polos::Renderer::RenderScene(m_MainScene);
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
} // namespace polosformer
