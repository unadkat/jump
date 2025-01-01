// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_ERROR_DATA_HPP
#define JUMP_ERROR_DATA_HPP

#include <string>
#include <utility>

namespace jump{
/// \brief Data for basic RuntimeError.
struct BasicError {
    const std::string type{"Runtime error"};
    const std::string details{"No details provided"};
    auto info() const -> std::string;
};

/// \brief Data for FileIOError, including file name or directory which failed
/// to open/read/write.
struct FileIOError {
    const std::string type{"File IO error"};
    const std::string resource;
    auto info() const -> std::string;
};

/// \brief Data for InvalidArgumentError, should be passed information about the
/// invalid argument and what was expected.
struct InvalidArgumentError {
    const std::string type{"Invalid argument error"};
    const std::string argument;
    const std::string value;
    const std::string expected;
    auto info() const -> std::string;
};

/// \brief Data for Range1DError, including name, access index, and size of 1D
/// container.
struct Range1DError {
    const std::string type{"Range (1D) error"};
    const std::string name{"internal storage"};
    const std::size_t index{};
    const std::size_t size{};
    auto info() const -> std::string;
};

/// \brief Data for Mismatch1DError, including names, access indices, and sizes
/// of 1D containers.
struct Mismatch1DError {
    const std::string type{"Mismatch (1D) error"};
    const std::string name1{"internal storage"};
    const std::size_t size1{};
    const std::string name2{"internal storage"};
    const std::size_t size2{};
    auto info() const -> std::string;
};

/// \brief Data for Range2DError, including name, access indices, and size of 2D
/// container.
struct Range2DError {
    const std::string type{"Range (2D) error"};
    const std::string name{"internal storage"};
    const std::pair<std::size_t, std::size_t> indices;
    const std::pair<std::size_t, std::size_t> size;
    auto info() const -> std::string;
};

/// \brief Data for Mismatch2DError, including names, acess indices, and sizes
/// of 2D containers.
struct Mismatch2DError {
    const std::string type{"Mismatch (2D) error"};
    const std::string name1{"internal storage"};
    const std::pair<std::size_t, std::size_t> size1;
    const std::string name2{"internal storage"};
    const std::pair<std::size_t, std::size_t> size2;
    auto info() const -> std::string;
};
}   // namespace jump

#endif  // JUMP_ERROR_DATA_HPP

