// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_LOGGING_HPP
#define JUMP_LOGGING_HPP

#include <ostream>
#include <string>

namespace jump {
namespace ANSIColour {
    const std::string black          {"\033[30m"};
    const std::string red            {"\033[31m"};
    const std::string green          {"\033[32m"};
    const std::string yellow         {"\033[33m"};
    const std::string blue           {"\033[34m"};
    const std::string magenta        {"\033[35m"};
    const std::string cyan           {"\033[36m"};
    const std::string white          {"\033[37m"};
    const std::string bright_black   {"\033[1;30m"};
    const std::string bright_red     {"\033[1;31m"};
    const std::string bright_green   {"\033[1;32m"};
    const std::string bright_yellow  {"\033[1;33m"};
    const std::string bright_blue    {"\033[1;34m"};
    const std::string bright_magenta {"\033[1;35m"};
    const std::string bright_cyan    {"\033[1;36m"};
    const std::string bright_white   {"\033[1;37m"};
    const std::string none           {"\033[0m"};
}   // namespace ANSIColour

struct ColourMessage {
    const std::string& ansi;
    const std::string& message;
};

/// Provides functions to surround strings with appropriate ANSI colour codes
struct Log {
    static auto black(const std::string& message) -> ColourMessage;
    static auto red(const std::string& message) -> ColourMessage;
    static auto green(const std::string& message) -> ColourMessage;
    static auto yellow(const std::string& message) -> ColourMessage;
    static auto blue(const std::string& message) -> ColourMessage;
    static auto magenta(const std::string& message) -> ColourMessage;
    static auto cyan(const std::string& message) -> ColourMessage;
    static auto white(const std::string& message) -> ColourMessage;

    static auto bright_black(const std::string& message) -> ColourMessage;
    static auto bright_red(const std::string& message) -> ColourMessage;
    static auto bright_green(const std::string& message) -> ColourMessage;
    static auto bright_yellow(const std::string& message) -> ColourMessage;
    static auto bright_blue(const std::string& message) -> ColourMessage;
    static auto bright_magenta(const std::string& message) -> ColourMessage;
    static auto bright_cyan(const std::string& message) -> ColourMessage;
    static auto bright_white(const std::string& message) -> ColourMessage;
};

auto operator<<(std::ostream& out, const ColourMessage& rhs) -> std::ostream&;
}   // namespace jump

#endif  // JUMP_LOGGING_HPP

