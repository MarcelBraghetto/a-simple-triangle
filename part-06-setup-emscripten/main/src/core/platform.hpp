#pragma once

namespace ast
{
    enum class Platform
    {
        mac,
        ios,
        android,
        emscripten
    };

    Platform getCurrentPlatform();
} // namespace ast
