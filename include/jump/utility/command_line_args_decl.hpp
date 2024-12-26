// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_COMMAND_LINE_ARGS_DECL_HPP
#define JUMP_COMMAND_LINE_ARGS_DECL_HPP

#include <string>
#include <vector>

namespace jump {
/// \brief Parses and stores information supplied at runtime via command-line
/// arguments, and enables their extraction.
class CommandLineArgs {
    public:
        /// \brief Reads command-line arguments at construction.
        CommandLineArgs(int argc, char** const argv);

        /// \brief Attempt extraction of flag and indicate success.
        auto get(const char& flag, bool& storage) -> bool;
        /// \brief Attempt extraction of option and indicate success.
        template <typename T>
        auto get(const std::string& option, T& storage) -> bool;

        /// \brief Print list of extracted flags and options to a stream.
        template <typename Os> friend
        auto operator<<(Os& out, const CommandLineArgs& rhs) -> Os&;

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
};
}   // namespace jump

#endif  // JUMP_COMMAND_LINE_ARGS_DECL_HPP

