#pragma once

#include <string>

namespace ast
{
    void log(const std::string& tag, const std::string& message);

    void log(const std::string& tag, const std::string& message, const std::exception& error);
} // namespace ast
