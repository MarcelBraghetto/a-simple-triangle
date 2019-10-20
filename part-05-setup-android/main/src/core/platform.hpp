#pragma once

namespace ast
{
    enum class Platform
    {
        mac,
        ios,
        android
    };

    Platform getCurrentPlatform();
} // namespace ast