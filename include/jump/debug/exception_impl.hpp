// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

inline std::string BasicError::info() const {
    return details;
}

inline std::string FileIOError::info() const {
    return std::format("Resource \"{}\" failed to read/write", resource);
}

inline std::string InvalidArgumentError::info() const {
    return std::format("Argument {} had invalid value {}\nExpected: {}",
            argument, value, expected);
}

template <typename ErrorData>
inline void RuntimeError<ErrorData>::construct_message() {
    m_message =
        "\n--------------------------------------------------\n"
        + std::format("{} in {}\nAt location {}:{}:{}\n", m_data.type,
                m_source.function_name(), m_source.file_name(), m_source.line(),
                m_source.column())
        + "--------------------------------------------------\n"
        + m_data.info() + '\n';
}

template <typename ErrorData>
inline RuntimeError<ErrorData>::RuntimeError(ErrorData data,
        std::source_location source) :
    m_data{std::move(data)},
    m_source{std::move(source)} {

    construct_message();
}

template <typename ErrorData>
inline std::string& RuntimeError<ErrorData>::what() noexcept {
    return m_message;
}

template <typename ErrorData>
inline const char* RuntimeError<ErrorData>::what() const noexcept {
    return m_message.c_str();
}

template <typename ErrorData>
inline const std::source_location& RuntimeError<ErrorData>::where()
    const noexcept {
    return m_source;
}

