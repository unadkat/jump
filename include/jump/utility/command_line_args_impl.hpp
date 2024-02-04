// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

/// Note: there is currently an edge case where a valueless option followed
/// directly by a flag collection will register the flag collection as the value
/// for the option.
///
/// TODO: require all flag collections to appear before all options.
inline CommandLineArgs::CommandLineArgs(const int& argc, char** const argv) :
    m_flags{},
    m_options{} {

    // Populate list of strings with all supplied command-line arguments
    std::vector<std::string> arguments(argv + 1, argv + argc);

    // Loop through all arguments and analyse them
    for (std::size_t i(0), num_args(argc - 1), length; i < num_args;
            ++i) {
        length = arguments[i].size();
        if (length > 1) {
            // If the length is 1 then it is not valid and is ignored
            if (arguments[i][0] == '-') {
                // First character must be a dash or it is ignored
                if (arguments[i][1] == '-') {
                    // Second character being a dash makes this an option,
                    // possibly followed by an associated value
                    if (i != num_args - 1
                            && arguments[i + 1].substr(0, 2) != "--") {
                        // This option is not the last argument, and the
                        // following argument doesn't register as an option as
                        // well. Therefore the following argument is a
                        // value associated with this option
                        m_options.push_back({.option = arguments[i].substr(2),
                                .value = arguments[i + 1]});
                        // We've stolen the next argument so skip it
                        ++i;
                    } else {
                        // This option is either the last argument, or is
                        // followed by another option. It has no associated
                        // value and will be interpreted as truthy
                        m_options.push_back({.option = arguments[i].substr(2)});
                    }
                } else {
                    // Second character being a non-dash makes it a flag
                    // collection
                    for (std::size_t j(1); j < length; ++j) {
                        m_flags.push_back({.flag = arguments[i][j]});
                    }
                }
            }
        }
    }
}

/// Query if a specified flag appears in the command-line arguments. If the flag
/// exists, mark it as having been recognised.
inline bool CommandLineArgs::get(const char& flag, bool& storage) {
    if (auto it {std::ranges::find(m_flags, flag, &Flag::flag)};
            it != m_flags.end()) {
        return (storage = it->recognised = true);
    } else {
        return (storage = false);
    }
}

/// Templated extraction of option values allows on-the-fly conversion of the
/// stored data (in a `std::string`) so it may be stored in the user-supplied
/// variable. If an argument is extracted, mark it as having been recognised.
template <typename T>
inline bool CommandLineArgs::get(const std::string& option, T& storage) {
    if (auto it {std::ranges::find(m_options, option, &Option::option)};
            it != m_options.end()) {
        // Don't overwrite passed variable yet
        T converted;
        std::stringstream ss(it->value);
        ss >> converted;
        // Check conversion fail/badbit
        if (ss.fail()) {
            return (it->recognised = false);
        } else {
            storage = converted;
            return (it->recognised = true);
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
    if (auto it {std::ranges::find(m_options, option, &Option::option)};
            it != m_options.end()) {
        return (storage = it->recognised = true);
    } else {
        return (storage = false);
    }
}

template <typename Os>
inline Os& operator<<(Os& out, const CommandLineArgs& rhs) {
    std::string flags_text(""), options_text("");

    for (const auto& x : rhs.m_flags | std::views::filter([](const auto& x) {
                return x.recognised; })) {
        flags_text += std::format("\n{}", x.flag);
    }
    for (const auto& x : rhs.m_options | std::views::filter(
                [](const auto& x) { return x.recognised; })) {
        options_text += std::format("\n{}({})", x.option, x.value);
    }

    if (flags_text.size() > 0) {
        out << "Recognised flags:" << flags_text << std::endl;
    }
    if (options_text.size() > 0) {
        out << "Recognised options(value):" << options_text;
    }

    return out;
}

