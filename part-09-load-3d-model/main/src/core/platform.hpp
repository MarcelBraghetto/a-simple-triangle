#pragma once

namespace ast
{
    enum class Platform
    {
        mac,
        ios,
        android,
        emscripten,
		windows
    };

    Platform getCurrentPlatform();
} // namespace ast
