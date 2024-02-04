// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_EXCEPTION_HPP
#define JUMP_EXCEPTION_HPP

#include <exception>
#include <format>
#include <source_location>
#include <string>

namespace Jump {

    /// \brief Custom exception class to give information about general runtime
    /// errors.
    class RuntimeError : public std::exception {
        protected:
            /// \brief Type of error.
            std::string m_type;
            /// \brief Detailed information about the error to aid debugging.
            std::string m_info;
            /// \brief Full error message (with banner).
            std::string m_message;
            /// \brief Details about the source location where the error
            /// occurred.
            std::source_location m_source;

            /// \brief Set exception type for the error message.
            virtual void set_type();
            /// \brief Construct full error message from constituent parts.
            void construct_message();

        public:
            /// \brief Construct error of specified type with details of the
            /// problem and the source information at that location.
            RuntimeError(std::string info = "",
                    std::source_location source =
                    std::source_location::current());
            /// \brief Enable default destructor.
            virtual ~RuntimeError() = default;

            /// \brief Return the full error message (with banner) as a
            /// mutable string, in case catching code needs to add context and
            /// rethrow.
            std::string& what() noexcept;
            /// \brief Return source information at location of the raised
            /// exception.
            const std::source_location& where() const noexcept;
    };

    /// \brief File IO exception class.
    class FileIOError : public RuntimeError {
        protected:
            /// \brief Set exception type for the error message.
            virtual void set_type() override;

        public:
            using RuntimeError::RuntimeError;
    };

    /// \brief Invalid argument causing unrecoverable error.
    class InvalidArgumentError : public RuntimeError {
        protected:
            /// \brief Set exception type for the error message.
            virtual void set_type() override;

        public:
            using RuntimeError::RuntimeError;
    };

    #include "exception_impl.hpp"

}   // namespace Jump

#endif  // JUMP_EXCEPTION_HPP

