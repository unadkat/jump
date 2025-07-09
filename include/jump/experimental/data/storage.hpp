// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_STORAGE_HPP
#define JUMP_STORAGE_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"

#include "jump/expression_templates/concepts.hpp"

#include <array>
#include <type_traits>

namespace jump {
template <typename T, std::size_t N>
struct Storage {
    using ValueType = std::remove_cvref_t<T>;
    static constexpr bool is_vector_expression_leaf{true};

    constexpr auto operator[](std::size_t index) const -> const ValueType&;
    constexpr auto operator[](std::size_t index) -> ValueType&;

    constexpr auto size() const -> std::size_t;

    std::array<ValueType, N> storage;
};

// ========================================================================
// Implementation
// ========================================================================

template <typename T, std::size_t N>
inline constexpr auto Storage<T, N>::operator[](std::size_t index) const
        -> const ValueType& {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return storage[index];
}

template <typename T, std::size_t N>
inline constexpr auto Storage<T, N>::operator[](std::size_t index)
        -> ValueType& {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return storage[index];
}

template <typename T, std::size_t N>
inline constexpr auto Storage<T, N>::size() const -> std::size_t {
    return N;
}
}   // namespace jump

#endif  // JUMP_STORAGE_HPP

