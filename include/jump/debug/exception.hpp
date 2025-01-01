// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_EXCEPTION_HPP
#define JUMP_EXCEPTION_HPP

#include <exception>
#include <format>
#include <source_location>
#include <string>

namespace jump {
/// \brief Custom exception class to give information about general runtime
/// errors. The template parameter is to package specific information about the
/// type of error being thrown.
template <typename ErrorData>
class RuntimeError : public std::exception {
    public:
        /// \brief Construct error of specified type with details of the problem
        /// and the source information at that location.
        RuntimeError(ErrorData data, std::source_location source =
                std::source_location::current());

        /// \brief Return the full error message (with banner) as a mutable
        /// string, in case catching code needs to add context and rethrow.
        auto what() noexcept -> std::string&;
        /// \brief Overload for std::exception::what for quick output of error
        /// message on unhandled exception.
        auto what() const noexcept -> const char*;
        /// \brief Return source information at location of the raised
        /// exception.
        auto where() const noexcept -> const std::source_location&;

    private:
        /// \brief Construct full error message from constituent parts.
        void construct_message();

    private:
        /// \brief Package of data for the exception being raised.
        ErrorData m_data;
        /// \brief Full error message (with banner).
        std::string m_message;
        /// \brief Details about the source location where the error occurred.
        std::source_location m_source;
        // TODO: add std::stacktrace when available
};

// ========================================================================
// Implementation
// ========================================================================

template <typename ErrorData>
inline RuntimeError<ErrorData>::RuntimeError(ErrorData data,
        std::source_location source) :
    m_data{std::move(data)},
    m_source{std::move(source)} {

    construct_message();
}

template <typename ErrorData>
inline auto RuntimeError<ErrorData>::what() noexcept -> std::string& {
    return m_message;
}

template <typename ErrorData>
inline auto RuntimeError<ErrorData>::what() const noexcept -> const char* {
    return m_message.c_str();
}

template <typename ErrorData>
inline auto RuntimeError<ErrorData>::where() const noexcept
        -> const std::source_location& {
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
}   // namespace jump

#endif  // JUMP_EXCEPTION_HPP

