// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_LOGGING_HPP
#define JUMP_LOGGING_HPP

#include "jump/utility/logging_decl.hpp"

namespace jump {
inline auto Log::black(const std::string& message) -> ColourMessage {
    return {ANSIColour::black, message};
}

inline auto Log::red(const std::string& message) -> ColourMessage {
    return {ANSIColour::red, message};
}

inline auto Log::green(const std::string& message) -> ColourMessage {
    return {ANSIColour::green, message};
}

inline auto Log::yellow(const std::string& message) -> ColourMessage {
    return {ANSIColour::yellow, message};
}

inline auto Log::blue(const std::string& message) -> ColourMessage {
    return {ANSIColour::blue, message};
}

inline auto Log::magenta(const std::string& message) -> ColourMessage {
    return {ANSIColour::magenta, message};
}

inline auto Log::cyan(const std::string& message) -> ColourMessage {
    return {ANSIColour::cyan, message};
}

inline auto Log::white(const std::string& message) -> ColourMessage {
    return {ANSIColour::white, message};
}

inline auto Log::bright_black(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_black, message};
}

inline auto Log::bright_red(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_red, message};
}

inline auto Log::bright_green(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_green, message};
}

inline auto Log::bright_yellow(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_yellow, message};
}

inline auto Log::bright_blue(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_blue, message};
}

inline auto Log::bright_magenta(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_magenta, message};
}

inline auto Log::bright_cyan(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_cyan, message};
}

inline auto Log::bright_white(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_white, message};
}

inline auto operator<<(std::ostream& out, const ColourMessage& rhs)
        -> std::ostream& {
    return out << rhs.ansi << rhs.message << ANSIColour::none;
}
}   // namespace jump

#endif  // JUMP_LOGGING_HPP

