// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_EXCEPTION_HPP
#define JUMP_EXCEPTION_HPP

#include "jump/debug/exception_decl.hpp"

namespace jump {
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

inline std::string Range1DError::info() const {
    return std::format("Attempted access at index {} in container {} (size {})",
            index, name, size);
}

inline std::string Mismatch1DError::info() const {
    return std::format("Mismatch between container {} (size {}) and "
            "container {} (size {})", name1, size1, name2, size2);
}

inline std::string Range2DError::info() const {
    return std::format("Attempted access at index ({}, {}) in container {} "
            "(size ({}, {}))", indices.first, indices.second, name, size.first,
            size.second);
}

inline std::string Mismatch2DError::info() const {
    return std::format("Mismatch between container {} (size ({}, {})) and "
            "container {} (size ({}, {}))", name1, size1.first, size1.second,
            name2, size2.first, size2.second);
}
}   // namespace jump

#endif  // JUMP_EXCEPTION_HPP

