#pragma once

namespace ast
{
    struct Application
    {
        Application() = default;

        virtual ~Application() = default;

        void startApplication();

        bool runMainLoop();

        virtual void render() = 0;
    };
} // namespace ast
