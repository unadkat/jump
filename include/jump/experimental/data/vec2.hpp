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
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
#include "jump/experimental/expression_templates/concepts.hpp"
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

#include <array>
#include <type_traits>

namespace jump {
template <typename T>
class Vec<T, 2> {
    public:
        using ValueType = std::remove_cvref_t<T>;
        static constexpr bool is_vector_expression_leaf{true};

        constexpr Vec(const ValueType& value = ValueType{0});
        constexpr Vec(const ValueType& x, const ValueType& y);
        constexpr Vec(const Vec& other) = default;
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
        /// \brief Construct from a VectorExpression.
        template <VectorExpressionConvertibleTo<ValueType> Expr>
        constexpr Vec(const Expr& expr);
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

        constexpr auto operator[](std::size_t index) const -> const ValueType&;
        constexpr auto operator[](std::size_t index) -> ValueType&;

        constexpr auto size() const -> std::size_t;


    private:
        std::array<ValueType, 2> m_storage;
};

template <typename T>
inline constexpr Vec<T, 2>::Vec(const ValueType& value) :
    m_storage{value, value} {
}

template <typename T>
inline constexpr Vec<T, 2>::Vec(const ValueType& x, const ValueType& y) :
    m_storage{x, y} {
}

#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 2>::ValueType> Expr>
inline constexpr Vec<T, 2>::Vec(const Expr& expr) :
    m_storage{expr[0], expr[1]} {
}
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

template <typename T>
inline constexpr auto Vec<T, 2>::operator[](std::size_t index) const
        -> const ValueType& {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return m_storage[index];
}

template <typename T>
inline constexpr auto Vec<T, 2>::operator[](std::size_t index) -> ValueType& {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return m_storage[index];
}

template <typename T>
inline constexpr auto Vec<T, 2>::size() const -> std::size_t {
    return 2;
}
}   // namespace jump

#endif  // JUMP_VEC2_HPP

