#pragma once

#include "../../core/internal-ptr.hpp"
#include "../application.hpp"

namespace ast
{
    struct OpenGLApplication : public ast::Application
    {
        OpenGLApplication();
        void render() override;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast
