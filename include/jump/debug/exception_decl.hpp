// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_EXCEPTION_DECL_HPP
#define JUMP_EXCEPTION_DECL_HPP

#include <exception>
#include <format>
#include <source_location>
#include <string>

namespace Jump {
/// \brief Custom exception class to give information about general runtime
/// errors. The template parameter is to package specific information about the
/// type of error being thrown.
template <typename ErrorData>
class RuntimeError : public std::exception {
    private:
        /// \brief Package of data for the exception being raised.
        ErrorData m_data;
        /// \brief Full error message (with banner).
        std::string m_message;
        /// \brief Details about the source location where the error occurred.
        std::source_location m_source;
        // TODO: add std::stacktrace when available

        /// \brief Construct full error message from constituent parts.
        void construct_message();

    public:
        /// \brief Construct error of specified type with details of the problem
        /// and the source information at that location.
        RuntimeError(ErrorData data, std::source_location source =
                std::source_location::current());

        /// \brief Return the full error message (with banner) as a mutable
        /// string, in case catching code needs to add context and rethrow.
        std::string& what() noexcept;
        /// \brief Overload for std::exception::what for quick output of error
        /// message on unhandled exception.
        const char* what() const noexcept;
        /// \brief Return source information at location of the raised
        /// exception.
        const std::source_location& where() const noexcept;
};

/// \brief Data for basic RuntimeError.
struct BasicError {
    const std::string type{"Runtime error"};
    const std::string details{"No details provided"};
    std::string info() const;
};

/// \brief Data for FileIOError, including file name or directory which failed
/// to open/read/write.
struct FileIOError {
    const std::string type{"File IO error"};
    const std::string resource;
    std::string info() const;
};

/// \brief Data for InvalidArgumentError, should be passed information about the
/// invalid argument and what was expected.
struct InvalidArgumentError {
    const std::string type{"Invalid argument error"};
    const std::string argument;
    const std::string value;
    const std::string expected;
    std::string info() const;
};

/// \brief Data for Range1DError, including name, access index, and size of 1D
/// container.
struct Range1DError {
    const std::string type{"Range (1D) error"};
    const std::string name{"internal storage"};
    const std::size_t index{};
    const std::size_t size{};
    std::string info() const;
};

/// \brief Data for Mismatch1DError, including names, access indices, and sizes
/// of 1D containers.
struct Mismatch1DError {
    const std::string type{"Mismatch (1D) error"};
    const std::string name1{"internal storage"};
    const std::size_t size1{};
    const std::string name2{"internal storage"};
    const std::size_t size2{};
    std::string info() const;
};

/// \brief Data for Range2DError, including name, access indices, and size of 2D
/// container.
struct Range2DError {
    const std::string type{"Range (2D) error"};
    const std::string name{"internal storage"};
    const std::pair<std::size_t, std::size_t> indices;
    const std::pair<std::size_t, std::size_t> size;
    std::string info() const;
};

/// \brief Data for Mismatch2DError, including names, acess indices, and sizes
/// of 2D containers.
struct Mismatch2DError {
    const std::string type{"Mismatch (2D) error"};
    const std::string name1{"internal storage"};
    const std::pair<std::size_t, std::size_t> size1;
    const std::string name2{"internal storage"};
    const std::pair<std::size_t, std::size_t> size2;
    std::string info() const;
};
}   // namespace Jump

#endif  // JUMP_EXCEPTION_DECL_HPP

