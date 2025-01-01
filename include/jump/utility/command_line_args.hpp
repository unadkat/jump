// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_COMMAND_LINE_ARGS_HPP
#define JUMP_COMMAND_LINE_ARGS_HPP

#include <ostream>
#include <sstream>
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
        friend auto operator<<(std::ostream& out, const CommandLineArgs& rhs)
                -> std::ostream&;

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

// ========================================================================
// Implementation
// ========================================================================

/// Templated extraction of option values allows on-the-fly conversion of the
/// stored data (in a `std::string`) so it may be stored in the user-supplied
/// variable. If an argument is extracted, mark it as having been recognised.
template <typename T>
inline auto CommandLineArgs::get(const std::string& option, T& storage)
        -> bool {
    if (auto it{std::ranges::find(m_options, option, &Option::option)};
            it != m_options.end()) {
        // Don't overwrite passed variable yet
        T converted;
        std::stringstream ss{it->value};
        ss >> converted;
        // Check conversion fail/badbit
        if (ss.fail()) {
            return (it->read = false);
        } else {
            storage = converted;
            return (it->read = true);
        }
    } else {
        return false;
    }
}
}   // namespace jump

#endif  // JUMP_COMMAND_LINE_ARGS_HPP

