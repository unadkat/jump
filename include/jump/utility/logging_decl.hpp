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

    static auto black(const std::string& message) -> std::string;
    static auto red(const std::string& message) -> std::string;
    static auto green(const std::string& message) -> std::string;
    static auto yellow(const std::string& message) -> std::string;
    static auto blue(const std::string& message) -> std::string;
    static auto magenta(const std::string& message) -> std::string;
    static auto cyan(const std::string& message) -> std::string;
    static auto white(const std::string& message) -> std::string;

    static auto bright_black(const std::string& message) -> std::string;
    static auto bright_red(const std::string& message) -> std::string;
    static auto bright_green(const std::string& message) -> std::string;
    static auto bright_yellow(const std::string& message) -> std::string;
    static auto bright_blue(const std::string& message) -> std::string;
    static auto bright_magenta(const std::string& message) -> std::string;
    static auto bright_cyan(const std::string& message) -> std::string;
    static auto bright_white(const std::string& message) -> std::string;
};
}   // namespace jump

#endif  // JUMP_LOGGING_DECL_HPP

