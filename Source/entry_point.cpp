#include "entry_point.h"

#include <polos.h>
#include <polos/core/window_system.h>
#include <polos/core/log.h>

namespace polosformer
{
    EntryPoint::EntryPoint()
        : game{std::make_unique<Game>()}
    {
        
    }

    EntryPoint::~EntryPoint()
    {
    }
}

polos::Application* polos::CreateApplication(void* ptr)
{
    window_props props{};
    props.title       = "Polosformer";
    props.width       = 1920; // width
    props.height      = 1080; // height
    props.refreshRate = 60;   // refresh rate
    props.vsync       = true; // vsync
    props.fullscreen  = false; // fullscreen

    auto a = WindowSystem::NewWindow(props);

    Application* app = ptr == nullptr ? new polosformer::EntryPoint() : new (ptr) polosformer::EntryPoint();

    return app;
}