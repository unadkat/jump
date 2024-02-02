// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

inline void RuntimeError::construct_message() {
    m_message =
        "\n--------------------------------------------------\n"
        + std::format("{} in {}\nIn file {} at line {}\n",
                (m_type.empty() ? "Unspecified error" : m_type),
                m_source.function_name(), m_source.file_name(), m_source.line())
        + "--------------------------------------------------\n"
        + (m_info.empty() ? "No details provided" : m_info) + '\n';
}

inline RuntimeError::RuntimeError(const std::string& type,
        const std::string& info, std::source_location source) :
    m_type(type),
    m_info(info),
    m_source(source) {

    construct_message();
}

inline const char* RuntimeError::what() const noexcept {
    return m_message.c_str();
}

