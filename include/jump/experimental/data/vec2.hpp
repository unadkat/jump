// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_VEC2_HPP
#define JUMP_VEC2_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"

#include "jump/experimental/data/vec.hpp"
#include "jump/experimental/expression_templates/concepts.hpp"

#include <type_traits>

namespace jump {
template <typename T>
struct Vec<T, 2> {
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
    using ValueType = std::remove_cvref_t<T>;
    static constexpr bool is_vector_expression_leaf{true};
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

    union {T x, r, s;};
    union {T y, g, t;};

    constexpr Vec(const T& value = T{0});
    constexpr Vec(const T& x, const T& y);
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
    /// \brief Construct from a VectorExpression.
    template <VectorExpressionConvertibleTo<T> Expr>
    constexpr Vec(const Expr& expr);
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

    constexpr auto operator[](std::size_t index) const -> const T&;
    constexpr auto operator[](std::size_t index) -> T&;

    constexpr auto size() const -> std::size_t;
};

template <typename T>
inline constexpr Vec<T, 2>::Vec(const T& value) :
    x{value},
    y{value} {
}

template <typename T>
inline constexpr Vec<T, 2>::Vec(const T& x, const T& y) :
    x{x},
    y{y} {
}

#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
template <typename T>
template <VectorExpressionConvertibleTo<T> Expr>
inline constexpr Vec<T, 2>::Vec(const Expr& expr) :
    x{expr[0]},
    y{expr[1]} {
}
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

template <typename T>
inline constexpr auto Vec<T, 2>::operator[](std::size_t index) const
        -> const T& {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    switch (index) {
        default:
        case 0:
            return x;
        case 1:
            return y;
    };
}

template <typename T>
inline constexpr auto Vec<T, 2>::operator[](std::size_t index) -> T& {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    switch (index) {
        default:
        case 0:
            return x;
        case 1:
            return y;
    };
}

template <typename T>
inline constexpr auto Vec<T, 2>::size() const -> std::size_t {
    return 2;
}
}   // namespace jump

#endif  // JUMP_VEC2_HPP

