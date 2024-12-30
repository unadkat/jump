// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#include "jump/utility/command_line_args.hpp"

#include "jump/utility/logging.hpp"

#include <algorithm>
#include <cctype>

namespace jump {
/// Reads options (string prefixed by "--") and flags (single alpha chars),
/// which may appear singly or grouped. If an option is followed by another
/// option or a flag group, it is recognised as a boolean flag with truthy
/// value, otherwise it is associated with the following string as its value.
///
/// Note: if an option is specified more than once only the first instance is
/// recognised during extraction. Should probably switch this behaviour to
/// favour overriding previous values.
CommandLineArgs::CommandLineArgs(int argc, char** const argv) :
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
                for (std::size_t j{1}; j < length; ++j) {
                    m_flags.push_back({.flag = arguments[i][j]});
                }
            }
        }
    }
}

/// Query if a specified flag appears in the command-line arguments. If the flag
/// exists, mark it as having been recognised.
auto CommandLineArgs::get(const char& flag, bool& storage) -> bool {
    if (auto it{std::ranges::find(m_flags, flag, &Flag::flag)};
            it != m_flags.end()) {
        return (storage = it->read = true);
    } else {
        return false;
    }
}

/// Query if a specified option appears in the command-line arguments (whether
/// supplied with an accompanying value or not). If the argument exists, mark it
/// as having been recognised.
template <>
auto CommandLineArgs::get(const std::string& option, bool& storage) -> bool {
    if (auto it{std::ranges::find(m_options, option, &Option::option)};
            it != m_options.end()) {
        return (storage = it->read = true);
    } else {
        return false;
    }
}

auto operator<<(std::ostream& out, const CommandLineArgs& rhs)
        -> std::ostream& {
    std::ostringstream oss_flags, oss_options;

    for (const auto& x : rhs.m_flags) {
        oss_flags << '\n' << x.flag << ": "
            << (x.read ? Log::green("extracted") : Log::red("not extracted"));
    }
    for (const auto& x : rhs.m_options) {
        oss_options << '\n' << x.option << '(' << x.value << "): "
            << (x.read ? Log::green("extracted") : Log::red("not extracted"));
    }

    const std::string& flags_string{oss_flags.str()};
    const std::string& options_string{oss_options.str()};
    bool output_flags{flags_string.size() > 0};
    if (output_flags) {
        out << "Flags:" << flags_string;
    }
    if (options_string.size() > 0) {
        if (output_flags) {
            out << '\n';
        }
        out << "Options(value):" << options_string;
    }

    return out;
}
}   // namespace jump

