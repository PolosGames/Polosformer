#pragma once

#include <memory>

#include <polos/core/application.h>

#include "game.h"

namespace polosformer
{
    class EntryPoint : public polos::Application
    {
    public:
        EntryPoint();
        virtual ~EntryPoint();
    private:
        std::unique_ptr<Game> game;
    };
} // namespace polosformer