// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

inline void RuntimeError::set_type() {
    m_type = "Runtime error";
}

inline void RuntimeError::construct_message() {
    m_message =
        "\n--------------------------------------------------\n"
        + std::format("{} in {}\nAt location {}:{}:{}\n",
                (m_type.empty() ? "Unspecified error" : m_type),
                m_source.function_name(), m_source.file_name(), m_source.line(),
                m_source.column())
        + "--------------------------------------------------\n"
        + (m_info.empty() ? "No details provided" : m_info) + '\n';
}

inline RuntimeError::RuntimeError(std::string info,
        std::source_location source) :
    m_info{std::move(info)},
    m_source{std::move(source)} {

    set_type();
    construct_message();
}

inline std::string& RuntimeError::what() noexcept {
    return m_message;
}

inline const std::source_location& RuntimeError::where() const noexcept {
    return m_source;
}

inline void FileIOError::set_type() {
    m_type = "File IO error";
}

inline void InvalidArgumentError::set_type() {
    m_type = "Invalid argument error";
}

