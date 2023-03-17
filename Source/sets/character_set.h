#pragma once

#include <polos/core/ecs/common_set.h>
#include <polos/core/ecs/components/components.h>

#include "entity_direction.h"

namespace polos::ecs
{
    struct character_set : CommonSet
    {
        COMMONSET_REGISTER(
            character_set,
            info_component,
            transform_component,
            texture2d_component,
            material_component,
            animator_component
        )

        character_set(
            Entity p_Entity,
            info_component*      p_Info,
            transform_component* p_Transform,
            texture2d_component* p_Texture2d,
            material_component*  p_Material,
            animator_component*  p_Animator
        )
            : entity{p_Entity}
            , infoComponent{p_Info}
            , transformComponent{ p_Transform }
            , texture2dComponent{p_Texture2d}
            , materialComponent{p_Material}
            , animatorComponent{p_Animator}
        {}

        Entity               entity{INVALID_ENTITY};
        info_component*      infoComponent{};
        transform_component* transformComponent{};
        texture2d_component* texture2dComponent{};
        material_component*  materialComponent{};
        animator_component*  animatorComponent{};
        
        polosformer::EntityDirection direction;
    };
} // namespace polos::ecs
