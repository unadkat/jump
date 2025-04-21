// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_VEC3_HPP
#define JUMP_VEC3_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/expression_templates/concepts.hpp"

#include "jump/experimental/data/vec.hpp"

#include <array>
#include <cmath>
#include <concepts>
#include <type_traits>

namespace jump {
template <typename T>
class Vec<T, 3> {
    public:
        using ValueType = std::remove_cvref_t<T>;
        static constexpr bool is_vector_expression_leaf{true};

        constexpr Vec(const ValueType& value = ValueType{0});
        constexpr Vec(const ValueType& x, const ValueType& y,
                const ValueType& z);
        constexpr Vec(const Vec& other) = default;
        template <std::convertible_to<ValueType> U>
        constexpr Vec(const Vec<U, 3>& other);
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

        constexpr auto z() const -> const ValueType&;
        constexpr auto z() -> ValueType&;
        constexpr auto b() const -> const ValueType&;
        constexpr auto b() -> ValueType&;

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
        std::array<ValueType, 3> m_storage;
};

// ========================================================================
// Aliases
// ========================================================================

using Vec3f = Vec<float, 3>;
using Vec3d = Vec<double, 3>;
using Vec3i = Vec<int, 3>;
using Vec3u = Vec<unsigned int, 3>;
using Vec3z = Vec<std::size_t, 3>;

// ========================================================================
// Implementation
// ========================================================================

template <typename T>
inline constexpr Vec<T, 3>::Vec(const ValueType& value) :
    m_storage{value, value} {
}

template <typename T>
inline constexpr Vec<T, 3>::Vec(const ValueType& x, const ValueType& y,
        const ValueType& z) :
    m_storage{x, y, z} {
}

template <typename T>
template <std::convertible_to<typename Vec<T, 3>::ValueType> U>
inline constexpr Vec<T, 3>::Vec(const Vec<U, 3>& other) :
    m_storage{other[0], other[1], other[2]} {
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 3>::ValueType> Expr>
inline constexpr Vec<T, 3>::Vec(const Expr& expr) :
    m_storage{expr[0], expr[1], expr[2]} {
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 3>::ValueType> Expr>
inline constexpr auto Vec<T, 3>::operator=(const Expr& expr) -> Vec& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    m_storage[0] = expr[0];
    m_storage[1] = expr[1];
    m_storage[2] = expr[2];
    return *this;
}

template <typename T>
inline constexpr auto Vec<T, 3>::operator[](std::size_t index) const
        -> const ValueType& {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return m_storage[index];
}

template <typename T>
inline constexpr auto Vec<T, 3>::operator[](std::size_t index) -> ValueType& {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return m_storage[index];
}

template <typename T>
inline constexpr auto Vec<T, 3>::x() const -> const ValueType& {
    return m_storage[0];
}

template <typename T>
inline constexpr auto Vec<T, 3>::x() -> ValueType& {
    return m_storage[0];
}

template <typename T>
inline constexpr auto Vec<T, 3>::r() const -> const ValueType& {
    return m_storage[0];
}

template <typename T>
inline constexpr auto Vec<T, 3>::r() -> ValueType& {
    return m_storage[0];
}

template <typename T>
inline constexpr auto Vec<T, 3>::y() const -> const ValueType& {
    return m_storage[1];
}

template <typename T>
inline constexpr auto Vec<T, 3>::y() -> ValueType& {
    return m_storage[1];
}

template <typename T>
inline constexpr auto Vec<T, 3>::g() const -> const ValueType& {
    return m_storage[1];
}

template <typename T>
inline constexpr auto Vec<T, 3>::g() -> ValueType& {
    return m_storage[1];
}

template <typename T>
inline constexpr auto Vec<T, 3>::z() const -> const ValueType& {
    return m_storage[2];
}

template <typename T>
inline constexpr auto Vec<T, 3>::z() -> ValueType& {
    return m_storage[2];
}

template <typename T>
inline constexpr auto Vec<T, 3>::b() const -> const ValueType& {
    return m_storage[2];
}

template <typename T>
inline constexpr auto Vec<T, 3>::b() -> ValueType& {
    return m_storage[2];
}

template <typename T>
inline constexpr auto Vec<T, 3>::size() const -> std::size_t {
    return 3;
}

template <typename T>
inline constexpr void Vec<T, 3>::fill(const T& value) {
    m_storage[0] = m_storage[1] = m_storage[2] = value;
}

template <typename T>
inline constexpr void Vec<T, 3>::zero() {
    m_storage[0] = m_storage[1] = m_storage[2] = ValueType{0};
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 3>::ValueType> Expr>
inline constexpr auto Vec<T, 3>::operator+=(const Expr& expr) -> Vec& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    m_storage[0] += expr[0];
    m_storage[1] += expr[1];
    m_storage[2] += expr[2];
    return *this;
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 3>::ValueType> Expr>
inline constexpr auto Vec<T, 3>::operator-=(const Expr& expr) -> Vec& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    m_storage[0] -= expr[0];
    m_storage[1] -= expr[1];
    m_storage[2] -= expr[2];
    return *this;
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 3>::ValueType> Expr>
inline constexpr auto Vec<T, 3>::operator*=(const Expr& expr) -> Vec& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    m_storage[0] *= expr[0];
    m_storage[1] *= expr[1];
    m_storage[2] *= expr[2];
    return *this;
}

template <typename T>
template <VectorExpressionConvertibleTo<typename Vec<T, 3>::ValueType> Expr>
inline constexpr auto Vec<T, 3>::operator/=(const Expr& expr) -> Vec& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    m_storage[0] /= expr[0];
    m_storage[1] /= expr[1];
    m_storage[2] /= expr[2];
    return *this;
}

template <typename T>
template <std::convertible_to<typename Vec<T, 3>::ValueType> U>
inline constexpr auto Vec<T, 3>::operator*=(const U& rhs) -> Vec& {
    m_storage[0] *= rhs;
    m_storage[1] *= rhs;
    m_storage[2] *= rhs;
    return *this;
}

template <typename T>
template <std::convertible_to<typename Vec<T, 3>::ValueType> U>
inline constexpr auto Vec<T, 3>::operator/=(const U& rhs) -> Vec& {
    m_storage[0] /= rhs;
    m_storage[1] /= rhs;
    m_storage[2] /= rhs;
    return *this;
}

template <typename T>
inline constexpr auto Vec<T, 3>::squared_magnitude() const -> ValueType {
    return m_storage[0]*m_storage[0] + m_storage[1]*m_storage[1]
        + m_storage[2]*m_storage[2];
}

template <typename T>
inline constexpr auto Vec<T, 3>::magnitude() const -> ValueType {
    return std::sqrt(squared_magnitude());
}

template <typename T>
inline constexpr auto Vec<T, 3>::direction() const -> Vec {
    auto scale{1/magnitude()};
    return {scale*m_storage[0], scale*m_storage[1], scale*m_storage[2]};
}

template <typename T>
inline constexpr auto Vec<T, 3>::normalise() -> Vec& {
    auto scale{1/magnitude()};
    return (*this) *= scale;
}

template <typename T>
inline constexpr auto Vec<T, 3>::data() const -> const ValueType* {
    return m_storage.data();
}

template <typename T>
inline constexpr auto Vec<T, 3>::data() -> ValueType* {
    return m_storage.data();
}
}   // namespace jump

#endif  // JUMP_VEC3_HPP

