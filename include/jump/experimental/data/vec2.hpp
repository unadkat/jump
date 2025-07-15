// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_VEC2_HPP
#define JUMP_VEC2_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/expression_templates/concepts.hpp"
#include "jump/expression_templates/vector_operators.hpp"

#include "jump/experimental/data/storage.hpp"
#include "jump/experimental/data/vec.hpp"

#include <cmath>
#include <concepts>
#include <type_traits>

namespace jump {
template <typename T>
class Vec<T, 2> {
    private:
        static constexpr std::size_t N{2};

    public:
        using ValueType = std::remove_cvref_t<T>;
        static constexpr bool is_vector_expression_leaf{true};

        constexpr Vec(const ValueType& value = ValueType{0});
        constexpr Vec(const ValueType& x, const ValueType& y);
        constexpr Vec(const Vec& other) = default;
        template <std::convertible_to<ValueType> U>
        constexpr Vec(const Vec<U, N>& other);
        /// \brief Construct from a VectorExpression.
        template <VectorExpressionConvertibleTo<ValueType> Expr>
        constexpr Vec(const Expr& expr);

        constexpr auto operator=(const Vec& other) -> Vec& = default;
        template <VectorExpressionConvertibleTo<ValueType> Expr>
        constexpr auto operator=(const Expr& expr) -> Vec&;

        constexpr auto operator<=>(const Vec&) const = default;

        constexpr auto operator[](std::size_t index) const -> const ValueType&;
        constexpr auto operator[](std::size_t index) -> ValueType&;

        constexpr auto x() const -> const ValueType&;
        constexpr auto x() -> ValueType&;
        constexpr auto r() const -> const ValueType&;
        constexpr auto r() -> ValueType&;

        constexpr auto y() const -> const ValueType&;
        constexpr auto y() -> ValueType&;
        constexpr auto g() const -> const ValueType&;
        constexpr auto g() -> ValueType&;

        constexpr auto size() const -> std::size_t;

        constexpr void fill(const T& value);
        constexpr void zero();

        template <VectorExpressionConvertibleTo<ValueType> Expr>
        constexpr auto operator+=(const Expr& expr) -> Vec&;
        template <VectorExpressionConvertibleTo<ValueType> Expr>
        constexpr auto operator-=(const Expr& expr) -> Vec&;
        template <VectorExpressionConvertibleTo<ValueType> Expr>
        constexpr auto operator*=(const Expr& expr) -> Vec&;
        template <VectorExpressionConvertibleTo<ValueType> Expr>
        constexpr auto operator/=(const Expr& expr) -> Vec&;
        template <std::convertible_to<ValueType> U>
        constexpr auto operator*=(const U& rhs) -> Vec&;
        template <std::convertible_to<ValueType> U>
        constexpr auto operator/=(const U& rhs) -> Vec&;

        constexpr auto squared_magnitude() const -> ValueType;
        constexpr auto magnitude() const -> ValueType;
        constexpr auto direction() const -> Vec;
        constexpr auto normalise() -> Vec&;

        constexpr auto data() const -> const ValueType*;
        constexpr auto data() -> ValueType*;

    private:
        Storage<ValueType, N> m_storage;
};

// ========================================================================
// Aliases
// ========================================================================

using Vec2f = Vec<float, 2>;
using Vec2d = Vec<double, 2>;
using Vec2i = Vec<int, 2>;
using Vec2u = Vec<unsigned int, 2>;
using Vec2z = Vec<std::size_t, 2>;

// ========================================================================
// Implementation
// ========================================================================

template <typename T>
inline constexpr Vec<T, 2>::Vec(const ValueType& value) :
    m_storage{value, value} {
}

template <typename T>
inline constexpr Vec<T, 2>::Vec(const ValueType& x, const ValueType& y) :
    m_storage{x, y} {
}

template <typename T>
template <std::convertible_to<typename Vec<T, 2>::ValueType> U>
inline constexpr Vec<T, 2>::Vec(const Vec<U, N>& other) :
    m_storage{other[0], other[1]} {
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 2>::ValueType> Expr>
inline constexpr Vec<T, 2>::Vec(const Expr& expr) :
    m_storage{expr[0], expr[1]} {
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 2>::ValueType> Expr>
inline constexpr auto Vec<T, 2>::operator=(const Expr& expr) -> Vec& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}; i < N; ++i) {
        m_storage[i] = expr[i];
    }
    return *this;
}

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
inline constexpr auto Vec<T, 2>::x() const -> const ValueType& {
    return m_storage[0];
}

template <typename T>
inline constexpr auto Vec<T, 2>::x() -> ValueType& {
    return m_storage[0];
}

template <typename T>
inline constexpr auto Vec<T, 2>::r() const -> const ValueType& {
    return m_storage[0];
}

template <typename T>
inline constexpr auto Vec<T, 2>::r() -> ValueType& {
    return m_storage[0];
}

template <typename T>
inline constexpr auto Vec<T, 2>::y() const -> const ValueType& {
    return m_storage[1];
}

template <typename T>
inline constexpr auto Vec<T, 2>::y() -> ValueType& {
    return m_storage[1];
}

template <typename T>
inline constexpr auto Vec<T, 2>::g() const -> const ValueType& {
    return m_storage[1];
}

template <typename T>
inline constexpr auto Vec<T, 2>::g() -> ValueType& {
    return m_storage[1];
}

template <typename T>
inline constexpr auto Vec<T, 2>::size() const -> std::size_t {
    return 2;
}

template <typename T>
inline constexpr void Vec<T, 2>::fill(const T& value) {
    m_storage[0] = m_storage[1] = value;
}

template <typename T>
inline constexpr void Vec<T, 2>::zero() {
    m_storage[0] = m_storage[1] = ValueType{0};
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 2>::ValueType> Expr>
inline constexpr auto Vec<T, 2>::operator+=(const Expr& expr) -> Vec& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}; i < N; ++i) {
        m_storage[i] += expr[i];
    }
    return *this;
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 2>::ValueType> Expr>
inline constexpr auto Vec<T, 2>::operator-=(const Expr& expr) -> Vec& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}; i < N; ++i) {
        m_storage[i] -= expr[i];
    }
    return *this;
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 2>::ValueType> Expr>
inline constexpr auto Vec<T, 2>::operator*=(const Expr& expr) -> Vec& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}; i < N; ++i) {
        m_storage[i] *= expr[i];
    }
    return *this;
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 2>::ValueType> Expr>
inline constexpr auto Vec<T, 2>::operator/=(const Expr& expr) -> Vec& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}; i < N; ++i) {
        m_storage[i] /= expr[i];
    }
    return *this;
}

template <typename T>
template <std::convertible_to<typename Vec<T, 2>::ValueType> U>
inline constexpr auto Vec<T, 2>::operator*=(const U& rhs) -> Vec& {
    for (std::size_t i{0}; i < N; ++i) {
        m_storage[i] *= rhs;
    }
    return *this;
}

template <typename T>
template <std::convertible_to<typename Vec<T, 2>::ValueType> U>
inline constexpr auto Vec<T, 2>::operator/=(const U& rhs) -> Vec& {
    for (std::size_t i{0}; i < N; ++i) {
        m_storage[i] /= rhs;
    }
    return *this;
}

template <typename T>
inline constexpr auto Vec<T, 2>::squared_magnitude() const -> ValueType {
    return m_storage[0]*m_storage[0] + m_storage[1]*m_storage[1];
}

template <typename T>
inline constexpr auto Vec<T, 2>::magnitude() const -> ValueType {
    return std::sqrt(squared_magnitude());
}

template <typename T>
inline constexpr auto Vec<T, 2>::direction() const -> Vec {
    auto scale{1/magnitude()};
    return {scale*m_storage[0], scale*m_storage[1]};
}

template <typename T>
inline constexpr auto Vec<T, 2>::normalise() -> Vec& {
    auto scale{1/magnitude()};
    return (*this) *= scale;
}

template <typename T>
inline constexpr auto Vec<T, 2>::data() const -> const ValueType* {
    return m_storage.data();
}

template <typename T>
inline constexpr auto Vec<T, 2>::data() -> ValueType* {
    return m_storage.data();
}
}   // namespace jump

#endif  // JUMP_VEC2_HPP

