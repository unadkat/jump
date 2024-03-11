// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_LOGGING_DECL_HPP
#define JUMP_LOGGING_DECL_HPP

#include <string>

namespace jump {
/// Provides functions to surround strings with appropriate ANSI colour codes,
/// with a static flag to enable/disable colours.
struct Log {
    static bool colours;

    static std::string black(const std::string& message);
    static std::string red(const std::string& message);
    static std::string green(const std::string& message);
    static std::string yellow(const std::string& message);
    static std::string blue(const std::string& message);
    static std::string magenta(const std::string& message);
    static std::string cyan(const std::string& message);
    static std::string white(const std::string& message);

    static std::string bright_black(const std::string& message);
    static std::string bright_red(const std::string& message);
    static std::string bright_green(const std::string& message);
    static std::string bright_yellow(const std::string& message);
    static std::string bright_blue(const std::string& message);
    static std::string bright_magenta(const std::string& message);
    static std::string bright_cyan(const std::string& message);
    static std::string bright_white(const std::string& message);
};
}   // namespace jump

#endif  // JUMP_LOGGING_DECL_HPP

