#pragma once

#include "internal-ptr.hpp"

namespace ast
{
    struct Engine
    {
        Engine();

        void run();

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast
