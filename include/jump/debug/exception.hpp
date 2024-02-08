// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_EXCEPTION_HPP
#define JUMP_EXCEPTION_HPP

#include <exception>
#include <format>
#include <source_location>
#include <string>

namespace Jump {

    /// \brief Data for basic RuntimeError.
    struct BasicError {
        const std::string type{"Runtime error"};
        const std::string details{"No details provided"};
        std::string info() const;
    };

    /// \brief Data for FileIOError, including file name or directory which
    /// failed to open/read/write.
    struct FileIOError {
        const std::string type{"File IO error"};
        const std::string resource{""};
        std::string info() const;
    };

    /// \brief Data for InvalidArgumentError, should be passed information about
    /// the invalid argument and what was expected.
    struct InvalidArgumentError {
        const std::string type{"Invalid argument error"};
        const std::string argument{""};
        const std::string value{""};
        const std::string expected{""};
        std::string info() const;
    };

    /// \brief Custom exception class to give information about general runtime
    /// errors. The template parameter is to package specific information about
    /// the type of error being thrown.
    template <typename ErrorData>
    class RuntimeError : public std::exception {
        private:
            /// \brief Package of data for the exception being raised.
            ErrorData m_data;
            /// \brief Full error message (with banner).
            std::string m_message;
            /// \brief Details about the source location where the error
            /// occurred.
            std::source_location m_source;

            /// \brief Construct full error message from constituent parts.
            void construct_message();

        public:
            /// \brief Construct error of specified type with details of the
            /// problem and the source information at that location.
            RuntimeError(ErrorData data, std::source_location source =
                    std::source_location::current());

            /// \brief Return the full error message (with banner) as a
            /// mutable string, in case catching code needs to add context and
            /// rethrow.
            std::string& what() noexcept;
            /// \brief Overload for std::exception::what for quick output of
            /// error message on unhandled exception.
            const char* what() const noexcept;
            /// \brief Return source information at location of the raised
            /// exception.
            const std::source_location& where() const noexcept;
    };

    #include "exception_impl.hpp"

}   // namespace Jump

#endif  // JUMP_EXCEPTION_HPP

