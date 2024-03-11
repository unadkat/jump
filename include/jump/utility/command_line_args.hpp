// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_COMMAND_LINE_ARGS_HPP
#define JUMP_COMMAND_LINE_ARGS_HPP

#include "jump/utility/command_line_args_decl.hpp"

namespace jump {
/// Reads options (string prefixed by "--") and flags (single alpha chars),
/// which may appear singly or grouped. If an option is followed by another
/// option or a flag group, it is recognised as a boolean flag with truthy
/// value, otherwise it is associated with the following string as its value.
///
/// Note: if an option is specified more than once only the first instance is
/// recognised during extraction. Should probably switch this behaviour to
/// favour overriding previous values.
inline CommandLineArgs::CommandLineArgs(int argc, char** const argv) :
    m_flags{},
    m_options{} {

    // Populate list of strings with all supplied command-line arguments
    std::vector<std::string> arguments(argv + 1, argv + argc);
    // Helper functions
    auto is_option{[](const std::string& s) -> bool {
        return s.substr(0, 2) == "--";
    }};
    auto is_flag_group{[](const std::string& s) -> bool {
        if (s[0] != '-') {
            return false;
        }
        return std::all_of(s.begin() + 1, s.end(),
                [](unsigned char c) -> bool { return std::isalpha(c); });
    }};

    // Loop through all arguments and analyse them
    for (std::size_t i{0}, num_args = argc - 1, length; i < num_args; ++i) {
        length = arguments[i].size();
        // If the length is 1 then it is not valid and is ignored
        if (length > 1) {
            if (is_option(arguments[i])) {
                if (i != num_args - 1 && !(is_option(arguments[i + 1])
                                || is_flag_group(arguments[i + 1]))) {
                    // This option is not the last argument, and the following
                    // argument doesn't register as an option or flag group.
                    // Therefore the following argument is a value associated
                    // with this option
                    m_options.push_back({.option = arguments[i].substr(2),
                            .value = arguments[i + 1]});
                    // We've stolen the next argument so skip it
                    ++i;
                } else {
                    // This option is either the last argument, or is
                    // followed by another option or flag group. It has no
                    // associated value and will be interpreted as truthy
                    m_options.push_back({.option = arguments[i].substr(2)});
                }
            } else if (is_flag_group(arguments[i])) {
                for (std::size_t j(1); j < length; ++j) {
                    m_flags.push_back({.flag = arguments[i][j]});
                }
            }
        }
    }
}

/// Query if a specified flag appears in the command-line arguments. If the flag
/// exists, mark it as having been recognised.
inline bool CommandLineArgs::get(const char& flag, bool& storage) {
    if (auto it{std::ranges::find(m_flags, flag, &Flag::flag)};
            it != m_flags.end()) {
        return (storage = it->read = true);
    } else {
        return (storage = false);
    }
}

/// Templated extraction of option values allows on-the-fly conversion of the
/// stored data (in a `std::string`) so it may be stored in the user-supplied
/// variable. If an argument is extracted, mark it as having been recognised.
template <typename T>
inline bool CommandLineArgs::get(const std::string& option, T& storage) {
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

/// Query if a specified option appears in the command-line arguments (whether
/// supplied with an accompanying value or not). If the argument exists, mark it
/// as having been recognised.
template <>
inline bool CommandLineArgs::get(const std::string& option, bool& storage) {
    if (auto it{std::ranges::find(m_options, option, &Option::option)};
            it != m_options.end()) {
        return (storage = it->read = true);
    } else {
        return (storage = false);
    }
}

template <typename Os>
inline Os& operator<<(Os& out, const CommandLineArgs& rhs) {
    std::string flags_text{""}, options_text{""};

    for (const auto& x : rhs.m_flags) {
        flags_text += std::format("\n{}: {}", x.flag, x.read ?
                Log::green("extracted") : Log::red("not extracted"));
    }
    for (const auto& x : rhs.m_options) {
        options_text += std::format("\n{}({}): {}", x.option, x.value, x.read ?
                Log::green("extracted") : Log::red("not extracted"));
    }

    if (flags_text.size() > 0) {
        out << "Flags:" << flags_text << '\n';
    }
    if (options_text.size() > 0) {
        out << "Options(value):" << options_text;
    }

    return out;
}
}   // namespace jump

#endif  // JUMP_COMMAND_LINE_ARGS_HPP

