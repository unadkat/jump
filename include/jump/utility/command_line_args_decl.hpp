// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_COMMAND_LINE_ARGS_DECL_HPP
#define JUMP_COMMAND_LINE_ARGS_DECL_HPP

#include <algorithm>
#include <cctype>
#include <format>
#include <ranges>
#include <string>
#include <sstream>
#include <vector>

#include "jump/utility/logging.hpp"

namespace jump {
/// \brief Parses and stores information supplied at runtime via command-line
/// arguments, and enables their extraction.
class CommandLineArgs {
    private:
        /// \brief Stores a flag (single character only) and whether this flag
        /// has been extracted by the user.
        struct Flag {
            char flag{0};
            bool read{false};
        };

        /// \brief Stores an option (both name and value) and whether this
        /// option has been extracted by the user.
        struct Option {
            std::string option{""};
            std::string value{""};
            bool read{false};
        };

        /// \brief Collection of flags read from the command line.
        std::vector<Flag> m_flags;
        /// \brief Collection of options read from the command line.
        std::vector<Option> m_options;

    public:
        /// \brief Reads command-line arguments at construction.
        CommandLineArgs(int argc, char** const argv);

        /// \brief Attempt extraction of flag and indicate success.
        bool get(const char& flag, bool& storage);
        /// \brief Attempt extraction of option and indicate success.
        template <typename T>
        bool get(const std::string& option, T& storage);

        /// \brief Print list of extracted flags and options to a stream.
        template <typename Os> friend
        Os& operator<<(Os& out, const CommandLineArgs& rhs);
};
}   // namespace jump

#endif  // JUMP_COMMAND_LINE_ARGS_DECL_HPP

