#pragma once

namespace ast
{
    enum class Platform
    {
        mac,
        ios
    };

    Platform getCurrentPlatform();
} // namespace ast
