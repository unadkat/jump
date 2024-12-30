// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#include "jump/utility/logging.hpp"

namespace jump {
auto Log::black(const std::string& message) -> ColourMessage {
    return {ANSIColour::black, message};
}

auto Log::red(const std::string& message) -> ColourMessage {
    return {ANSIColour::red, message};
}

auto Log::green(const std::string& message) -> ColourMessage {
    return {ANSIColour::green, message};
}

auto Log::yellow(const std::string& message) -> ColourMessage {
    return {ANSIColour::yellow, message};
}

auto Log::blue(const std::string& message) -> ColourMessage {
    return {ANSIColour::blue, message};
}

auto Log::magenta(const std::string& message) -> ColourMessage {
    return {ANSIColour::magenta, message};
}

auto Log::cyan(const std::string& message) -> ColourMessage {
    return {ANSIColour::cyan, message};
}

auto Log::white(const std::string& message) -> ColourMessage {
    return {ANSIColour::white, message};
}

auto Log::bright_black(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_black, message};
}

auto Log::bright_red(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_red, message};
}

auto Log::bright_green(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_green, message};
}

auto Log::bright_yellow(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_yellow, message};
}

auto Log::bright_blue(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_blue, message};
}

auto Log::bright_magenta(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_magenta, message};
}

auto Log::bright_cyan(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_cyan, message};
}

auto Log::bright_white(const std::string& message) -> ColourMessage {
    return {ANSIColour::bright_white, message};
}

auto operator<<(std::ostream& out, const ColourMessage& rhs) -> std::ostream& {
    return out << rhs.ansi << rhs.message << ANSIColour::none;
}
}   // namespace jump

