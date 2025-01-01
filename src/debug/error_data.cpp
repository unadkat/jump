// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#include "jump/debug/error_data.hpp"

#include <format>

namespace jump {
auto BasicError::info() const -> std::string {
    return details;
}

auto FileIOError::info() const -> std::string {
    return std::format("Resource \"{}\" failed to read/write", resource);
}

auto InvalidArgumentError::info() const -> std::string {
    return std::format("Argument {} had invalid value {}\nExpected: {}",
            argument, value, expected);
}

auto Range1DError::info() const -> std::string {
    return std::format("Attempted access at index {} in container {} (size {})",
            index, name, size);
}

auto Mismatch1DError::info() const -> std::string {
    return std::format("Mismatch between container {} (size {}) and "
            "container {} (size {})", name1, size1, name2, size2);
}

auto Range2DError::info() const -> std::string {
    return std::format("Attempted access at index ({}, {}) in container {} "
            "(size ({}, {}))", indices.first, indices.second, name, size.first,
            size.second);
}

auto Mismatch2DError::info() const -> std::string {
    return std::format("Mismatch between container {} (size ({}, {})) and "
            "container {} (size ({}, {}))", name1, size1.first, size1.second,
            name2, size2.first, size2.second);
}
}   // namespace jump

