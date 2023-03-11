#include "entry_point.h"

#include <polos.h>
#include <polos/core/window_system.h>
#include <polos/core/log.h>
#include <polos/graphics/shader_lib.h>

namespace polosformer
{
    EntryPoint::EntryPoint()
    {
        polos::ShaderLib::Load("resources/shaders/texture.vert", "resources/shaders/texture.frag");
        game = std::make_unique<Game>();
    }

    EntryPoint::~EntryPoint()
    {}
}

polos::Application* polos::CreateApplication(void* ptr)
{
    window_props props{};
    props.title       = "Polosformer";
    props.width       = 3840; // width
    props.height      = 2160; // height
    props.refreshRate = 60;   // refresh rate
    props.vsync       = true; // vsync
    props.fullscreen  = false; // fullscreen

    auto a = WindowSystem::NewWindow(props);

    Application* app = ptr == nullptr ? new polosformer::EntryPoint() : new (ptr) polosformer::EntryPoint();

    return app;
}