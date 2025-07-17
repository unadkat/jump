// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_MAT2_HPP
#define JUMP_MAT2_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/expression_templates/concepts.hpp"
#include "jump/expression_templates/dense_matrix_operators.hpp"

#include "jump/experimental/data/mat.hpp"
#include "jump/experimental/data/storage.hpp"
#include "jump/experimental/data/vec2.hpp"

#include <cmath>
#include <concepts>
#include <type_traits>
#include <utility>

namespace jump {
template <typename T>
class Mat<T, 2> {
    private:
        static constexpr std::size_t N{2};

    public:
        using ValueType = std::remove_cvref_t<T>;
        /// \brief To satisfy DenseMatrixExpression concept
        using InnerExpressionType = Storage<ValueType, N*N>;
        /// \brief To satisfy DenseMatrixExpression concept (and signal that
        /// this data structure can be referenced in evaluations).
        static constexpr bool is_dense_matrix_expression_leaf{true};

        constexpr Mat(const ValueType& value = ValueType{0});
        constexpr Mat(const ValueType& a11, const ValueType& a22);
        constexpr Mat(const ValueType& a11, const ValueType& a21,
                const ValueType& a12, const ValueType& a22);
        constexpr Mat(const Mat& other) = default;
        template <std::convertible_to<ValueType> U>
        constexpr Mat(const Mat<U, N>& other);
        /// \brief Construct from a MatrixExpression.
        template <DenseMatrixExpressionConvertibleTo<ValueType> Expr>
        constexpr Mat(const Expr& expr);

        constexpr auto operator=(const Mat& other) -> Mat& = default;
        template <DenseMatrixExpressionConvertibleTo<ValueType> Expr>
        constexpr auto operator=(const Expr& expr) -> Mat&;

        constexpr auto operator<=>(const Mat&) const = default;

        constexpr auto operator[](std::size_t column) const -> const Vec<T, N>&;
        constexpr auto operator[](std::size_t column) -> Vec<T, N>&;
        constexpr auto operator[](std::size_t row, std::size_t column) const
                -> const ValueType&;
        constexpr auto operator[](std::size_t row, std::size_t column)
                -> ValueType&;

        constexpr auto num_elements() const -> std::size_t;
        constexpr auto size() const
                -> const std::pair<std::size_t, std::size_t>&;

        constexpr void fill(const T& value);
        constexpr void zero();

        template <DenseMatrixExpressionConvertibleTo<ValueType> Expr>
        constexpr auto operator+=(const Expr& expr) -> Mat&;
        template <DenseMatrixExpressionConvertibleTo<ValueType> Expr>
        constexpr auto operator-=(const Expr& expr) -> Mat&;
        template <DenseMatrixExpressionConvertibleTo<ValueType> Expr>
        constexpr auto operator*=(const Expr& expr) -> Mat&;
        template <DenseMatrixExpressionConvertibleTo<ValueType> Expr>
        constexpr auto operator/=(const Expr& expr) -> Mat&;
        template <std::convertible_to<ValueType> U>
        constexpr auto operator*=(const U& rhs) -> Mat&;
        template <std::convertible_to<ValueType> U>
        constexpr auto operator/=(const U& rhs) -> Mat&;

        constexpr auto data() const -> const ValueType*;
        constexpr auto data() -> ValueType*;
        /// \brief Const reference to underlying storage (column-major).
        constexpr auto as_vector() const -> const Storage<ValueType, N*N>&;

    private:
        Storage<ValueType, N*N> m_storage;
        static constexpr std::pair<std::size_t, std::size_t> m_size{N, N};
};

// ========================================================================
// Aliases
// ========================================================================

using Mat2f = Mat<float, 2>;
using Mat2d = Mat<double, 2>;
using Mat2i = Mat<int, 2>;
using Mat2u = Mat<unsigned int, 2>;
using Mat2z = Mat<std::size_t, 2>;

// ========================================================================
// Implementation
// ========================================================================

template <typename T>
inline constexpr Mat<T, 2>::Mat(const ValueType& value) :
    m_storage{value, 0, 0, value} {
}

template <typename T>
inline constexpr Mat<T, 2>::Mat(const ValueType& a11, const ValueType& a22) :
    m_storage{a11, 0, 0, a22} {
}

template <typename T>
inline constexpr Mat<T, 2>::Mat(const ValueType& a11, const ValueType& a21,
        const ValueType& a12, const ValueType& a22) :
    m_storage{
        a11, a21,
        a12, a22
    } {
}

template <typename T>
template <std::convertible_to<typename Mat<T, 2>::ValueType> U>
inline constexpr Mat<T, 2>::Mat(const Mat<U, N>& other) :
    m_storage{other.as_vector()} {
}

template <typename T>
template <DenseMatrixExpressionConvertibleTo<
    typename Mat<T, 2>::ValueType> Expr>
inline constexpr Mat<T, 2>::Mat(const Expr& expr) :
    m_storage{expr.as_vector()} {
}

template <typename T>
template <DenseMatrixExpressionConvertibleTo<
    typename Mat<T, 2>::ValueType> Expr>
inline constexpr auto Mat<T, 2>::operator=(const Expr& expr) -> Mat& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    m_storage = expr.as_vector();
    return *this;
}

template <typename T>
inline constexpr auto Mat<T, 2>::operator[](std::size_t column) const
        -> const Vec<T, N>& {
#ifndef NDEBUG
    if (column >= size().second) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = size()}};
    }
#endif  // NDEBUG

    return *reinterpret_cast<const Vec<T, N>*>(&m_storage[N*column]);
}

template <typename T>
inline constexpr auto Mat<T, 2>::operator[](std::size_t column) -> Vec<T, N>& {
#ifndef NDEBUG
    if (column >= size().second) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = size()}};
    }
#endif  // NDEBUG

    return *reinterpret_cast<Vec<T, N>*>(&m_storage[N*column]);
}

template <typename T>
inline constexpr auto Mat<T, 2>::operator[](std::size_t row,
        std::size_t column) const -> const ValueType& {
#ifndef NDEBUG
    if (row >= size().first || column >= size().second) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = size()}};
    }
#endif  // NDEBUG

    return m_storage[N*column + row];
}

template <typename T>
inline constexpr auto Mat<T, 2>::operator[](std::size_t row, std::size_t column)
        -> ValueType& {
#ifndef NDEBUG
    if (row >= size().first || column >= size().second) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = size()}};
    }
#endif  // NDEBUG

    return m_storage[N*column + row];
}

template <typename T>
inline constexpr auto Mat<T, 2>::num_elements() const -> std::size_t {
    return N*N;
}

template <typename T>
inline constexpr auto Mat<T, 2>::size() const
        -> const std::pair<std::size_t, std::size_t>& {
    return m_size;
}

template <typename T>
inline constexpr void Mat<T, 2>::fill(const T& value) {
    m_storage.storage.fill(value);
}

template <typename T>
inline constexpr void Mat<T, 2>::zero() {
    m_storage.storage.fill(0);
}

template <typename T>
template <DenseMatrixExpressionConvertibleTo<
    typename Mat<T, 2>::ValueType> Expr>
inline constexpr auto Mat<T, 2>::operator+=(const Expr& expr) -> Mat& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}; i < N*N; ++i) {
        m_storage[i] += expr.as_vector()[i];
    }
    return *this;
}

template <typename T>
template <DenseMatrixExpressionConvertibleTo<
    typename Mat<T, 2>::ValueType> Expr>
inline constexpr auto Mat<T, 2>::operator-=(const Expr& expr) -> Mat& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}; i < N*N; ++i) {
        m_storage[i] -= expr.as_vector()[i];
    }
    return *this;
}

template <typename T>
template <DenseMatrixExpressionConvertibleTo<
    typename Mat<T, 2>::ValueType> Expr>
inline constexpr auto Mat<T, 2>::operator*=(const Expr& expr) -> Mat& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}; i < N*N; ++i) {
        m_storage[i] *= expr.as_vector()[i];
    }
    return *this;
}

template <typename T>
template <DenseMatrixExpressionConvertibleTo<
    typename Mat<T, 2>::ValueType> Expr>
inline constexpr auto Mat<T, 2>::operator/=(const Expr& expr) -> Mat& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}; i < N*N; ++i) {
        m_storage[i] /= expr.as_vector()[i];
    }
    return *this;
}

template <typename T>
template <std::convertible_to<typename Mat<T, 2>::ValueType> U>
inline constexpr auto Mat<T, 2>::operator*=(const U& rhs) -> Mat& {
    for (std::size_t i{0}; i < N*N; ++i) {
        m_storage[i] *= rhs;
    }
    return *this;
}

template <typename T>
template <std::convertible_to<typename Mat<T, 2>::ValueType> U>
inline constexpr auto Mat<T, 2>::operator/=(const U& rhs) -> Mat& {
    for (std::size_t i{0}; i < N*N; ++i) {
        m_storage[i] /= rhs;
    }
    return *this;
}

template <typename T>
inline constexpr auto Mat<T, 2>::data() const -> const ValueType* {
    return m_storage.storage.data();
}

template <typename T>
inline constexpr auto Mat<T, 2>::data() -> ValueType* {
    return m_storage.storage.data();
}

template <typename T>
inline constexpr auto Mat<T, 2>::as_vector() const
        -> const Storage<ValueType, N*N>& {
    return m_storage;
}
}   // namespace jump

#endif  // JUMP_MAT2_HPP

