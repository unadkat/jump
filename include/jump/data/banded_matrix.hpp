// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_BANDED_MATRIX_HPP
#define JUMP_BANDED_MATRIX_HPP

#include "jump/data/matrix_base.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"

#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
#include "jump/experimental/expression_templates/banded_matrix_operators.hpp"
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

#include <format>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace jump {
/// \brief Banded square matrix for use with LAPACKE.
template <typename T>
class BandedMatrix : public MatrixBase<T> {
    public:
        /// \brief Iterator for algorithms.
        using Iterator = typename Vector<T>::Iterator;
        /// \brief Iterator for algorithms.
        using ConstIterator = typename Vector<T>::ConstIterator;
#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
        /// \brief To satisfy BandedMatrixExpression concept
        using InnerExpressionType = Vector<T>;

        /// \brief To satisfy BandedMatrixExpression concept (and signal that
        /// this data structure can be referenced in evaluations).
        static constexpr bool is_banded_matrix_expression_leaf{true};
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

        /// \brief Construct a square matrix with the given number of diagonals
        /// on each side of the leading diagonal.
        explicit BandedMatrix(std::size_t size = 0, std::size_t num_bands = 0);
        /// \brief Constuct a matrix with the given Vector data, specifying a
        /// consistent size.
        BandedMatrix(std::size_t size, std::size_t num_bands,
                Vector<T> underlying_data);
        /// \brief Default copy constructor.
        BandedMatrix(const BandedMatrix& other) = default;
        /// \brief Templated copy constructor.
        template <typename U>
        BandedMatrix(const BandedMatrix<U>& other);
        /// \brief Default move constructor.
        BandedMatrix(BandedMatrix&& other) = default;
#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
        /// \brief Construct from a BandedMatrixExpression.
        template <BandedMatrixExpressionConvertibleTo<T> Expr>
        BandedMatrix(const Expr& expr);
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

        /// \brief Default copy assignment.
        auto operator=(const BandedMatrix& other) -> BandedMatrix& = default;
        /// \brief Default move assignment.
        auto operator=(BandedMatrix&& other) -> BandedMatrix& = default;
#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
        /// \brief Assignment from a BandedMatrixExpression.
        template <BandedMatrixExpressionConvertibleTo<T> Expr>
        auto operator=(const Expr& expr) -> BandedMatrix&;
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

        /// \brief Initialise matrix with the given size and number of
        /// off-leading diagonal diagonals.
        void assign(std::size_t size, std::size_t num_bands);
        /// \brief Initialise a matrix with the given Vector data, specifying a
        /// consistent size.
        void assign(std::size_t size, std::size_t num_bands,
                Vector<T> underlying_data);
        /// \brief Set matrix storage with the given Vector data, which must
        /// match the existing container size.
        void assign(Vector<T> underlying_data);
        /// \brief Set data via a pair of iterators.
        template <class InputIt>
        void assign(InputIt first, InputIt last);
        /// \brief Return number of off-leading diagonal diagonals.
        auto num_bands() const -> std::size_t;
        /// \brief Return size of internal storage.
        virtual auto num_elements() const -> std::size_t override;

        /// \brief Defaulted spaceship operator.
        auto operator<=>(const BandedMatrix&) const = default;

        /// \brief Const element access, always returns temporary value.
        auto operator[](std::size_t row, std::size_t column) const -> T;
        /// \brief Mutable element access.
        auto get_unsafe(std::size_t row, std::size_t column) -> T&;
        /// \brief Element setter, returns flag for success.
        [[nodiscard]] auto set(std::size_t row, std::size_t column,
                const T& value) -> bool;

        /// \brief Const iterator for algorithms.
        auto begin() const -> ConstIterator;
        /// \brief Const iterator for algorithms.
        auto end() const -> ConstIterator;
        /// \brief Iterator for algorithms.
        auto begin() -> Iterator;
        /// \brief Iterator for algorithms.
        auto end() -> Iterator;

        /// \brief Fill matrix with given value.
        void fill(const T& value);
        /// \brief Zero the matrix.
        virtual void zero() override;

#ifndef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
        /// \brief No operation on matrix.
        auto operator+() const -> const BandedMatrix&;
        /// \brief Negate matrix.
        auto operator-() const -> BandedMatrix;
        /// \brief Add two matrices together in place.
        template <std::convertible_to<T> U>
        auto operator+=(const BandedMatrix<U>& rhs) -> BandedMatrix&;
        /// \brief Subtract a matrix from another in place.
        template <std::convertible_to<T> U>
        auto operator-=(const BandedMatrix<U>& rhs) -> BandedMatrix&;
#else
        /// \brief Add BandedMatrixExpression to BandedMatrix in place.
        template <BandedMatrixExpressionConvertibleTo<T> Expr>
        auto operator+=(const Expr& expr) -> BandedMatrix&;
        /// \brief Subtract a BandedMatrixExpression from a BandedMatrix in
        /// place.
        template <BandedMatrixExpressionConvertibleTo<T> Expr>
        auto operator-=(const Expr& expr) -> BandedMatrix&;
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
        /// \brief Multiply matrix by scalar in place.
        template <std::convertible_to<T> U>
        auto operator*=(const U& k) -> BandedMatrix&;
        /// \brief Divide matrix by scalar in place.
        template <std::convertible_to<T> U>
        auto operator/=(const U& k) -> BandedMatrix&;

        /// \brief Pointer to underlying data, for use with third-party
        /// libraries.
        auto data() -> T*;
        /// \brief Pointer to underlying data, for use with third-party
        /// libraries.
        auto data() const -> const T*;
        /// \brief Const reference to underlying Vector (column-major).
        auto as_vector() const -> const Vector<T>&;

        /// \brief Populate with data from a `std::string`. Noting that data
        /// storage is assumed to be column-major, matrices stored as strings
        /// will be assumed to be a transpose matrix.
        virtual void operator<<(std::string data) override;
        /// \brief Matrix serialisation to a string.
        virtual auto as_string() const -> std::string override;

    private:
        /// \brief Maximum number of diagonals on either side of the leading
        /// diagonal that are permitted to contain non-zero elements.
        ///
        /// In short, the interal storage takes the form of a dense matrix with
        /// `num_columns()` columns and `1 + 3*#num_bands()` rows. Refer to the
        /// <a href="http://www.netlib.org/lapack/lug/node124.html">LAPACK User
        /// Guide</a> for further details about the band storage scheme.
        std::size_t m_num_bands{0};
        /// \brief Internal contiguous storage.
        Vector<T> m_storage;
};

#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
/// \relates BandedMatrix
template <BandedMatrixExpression Expr>
constexpr auto evaluate(const Expr& expr)
        -> BandedMatrix<typename Expr::InnerExpressionType::ValueType>;
#else
/// \relates BandedMatrix
template <typename T>
constexpr auto evaluate(const BandedMatrix<T>& M) -> const BandedMatrix<T>&;
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

#ifndef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
/// \relates BandedMatrix
/// \brief Addition of two BandedMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator+(const BandedMatrix<T>& lhs, const BandedMatrix<U>& rhs)
        -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Addition of two BandedMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator+(BandedMatrix<T>&& lhs, const BandedMatrix<U>& rhs)
        -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Addition of two BandedMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator+(const BandedMatrix<T>& lhs, BandedMatrix<U>&& rhs)
        -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Addition of two BandedMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator+(BandedMatrix<T>&& lhs, BandedMatrix<U>&& rhs) -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Difference of two BandedMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator-(const BandedMatrix<T>& lhs, const BandedMatrix<U>& rhs)
        -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Difference of two BandedMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator-(BandedMatrix<T>&& lhs, const BandedMatrix<U>& rhs)
        -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Difference of two BandedMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator-(const BandedMatrix<T>& lhs, BandedMatrix<U>&& rhs)
        -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Difference of two BandedMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator-(BandedMatrix<T>&& lhs, BandedMatrix<U>&& rhs) -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const T& lhs, const BandedMatrix<U>& rhs) -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const T& lhs, BandedMatrix<U>&& rhs) -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const BandedMatrix<T>& lhs, const U& rhs) -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(BandedMatrix<T>&& lhs, const U& rhs) -> BandedMatrix<R>;
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

/// \relates BandedMatrix
/// \brief Right-hand-side multiplication by vector.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const BandedMatrix<T>& lhs, const Vector<U>& rhs) -> Vector<R>;

#ifndef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
/// \relates BandedMatrix
/// \brief Division by a scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(const BandedMatrix<T>& lhs, const U& rhs) -> BandedMatrix<R>;

/// \relates BandedMatrix
/// \brief Division by a scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(BandedMatrix<T>&& lhs, const U& rhs) -> BandedMatrix<R>;

// ========================================================================
// Exponentiation
// ========================================================================

/// \relates BandedMatrix
template <typename T>
auto exp(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto log(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto pow(const BandedMatrix<T>& M, T p) -> BandedMatrix<T>;

// ========================================================================
// Trigonometry
// ========================================================================

/// \relates BandedMatrix
template <typename T>
auto sin(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto cos(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto tan(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto asin(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto acos(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto atan(const BandedMatrix<T>& M) -> BandedMatrix<T>;

// ========================================================================
// Hyperbolics
// ========================================================================

/// \relates BandedMatrix
template <typename T>
auto sinh(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto cosh(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto tanh(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto asinh(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto acosh(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto atanh(const BandedMatrix<T>& M) -> BandedMatrix<T>;

// ========================================================================
// Miscellaneous
// ========================================================================

/// \relates BandedMatrix
template <typename T>
auto abs(const BandedMatrix<T>& M) -> BandedMatrix<T>;

/// \relates BandedMatrix
template <typename T>
auto sgn(const BandedMatrix<T>& M) -> BandedMatrix<T>;
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

// ========================================================================
// Implementation
// ========================================================================

/// Note that the internal storage takes the form of a dense matrix with
/// `num_columns()` columns and `1 + 3*#num_bands()` rows.
template <typename T>
inline BandedMatrix<T>::BandedMatrix(std::size_t size, std::size_t num_bands) :
    MatrixBase<T>{size},
    m_num_bands{num_bands},
    m_storage(size*(3*num_bands + 1), T{0}) {
}

template <typename T>
inline BandedMatrix<T>::BandedMatrix(std::size_t size, std::size_t num_bands,
        Vector<T> underlying_data) :
    MatrixBase<T>{size},
    m_num_bands{num_bands} {

    if (underlying_data.size() != size*(3*num_bands + 1)) {
        m_storage.assign(size*(3*num_bands + 1), T{0});
        throw RuntimeError{InvalidArgumentError{.argument = "underlying_data",
            .value = std::format("Vector of size {}", underlying_data.size()),
            .expected = std::format(
                    "Vector of size num_columns*(3*num_bands + 1) = {}x{} = {}",
                    size, 3*num_bands + 1, size*(3*num_bands + 1))}};
    } else {
        m_storage = std::move(underlying_data);
    }
}

template <typename T>
template <typename U>
inline BandedMatrix<T>::BandedMatrix(const BandedMatrix<U>& other) :
    MatrixBase<T>{other.size()},
    m_num_bands{other.num_bands()} {
    m_storage = other.as_vector();
}

#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
template <typename T>
template <BandedMatrixExpressionConvertibleTo<T> Expr>
inline BandedMatrix<T>::BandedMatrix(const Expr& expr) :
    MatrixBase<T>{expr.size()},
    m_num_bands{expr.num_bands()},
    m_storage{expr.as_vector()} {
}

template <typename T>
template <BandedMatrixExpressionConvertibleTo<T> Expr>
inline auto BandedMatrix<T>::operator=(const Expr& expr) -> BandedMatrix& {
    MatrixBase<T>::initialise(expr.size());
    m_num_bands = expr.num_bands();
    m_storage = expr.as_vector();
    return *this;
}
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

/// Note that the internal storage takes the form of a dense matrix with
/// `num_columns()` columns and `1 + 3*#num_bands()` rows.
template <typename T>
inline void BandedMatrix<T>::assign(std::size_t size, std::size_t num_bands) {
    MatrixBase<T>::initialise(size);
    m_num_bands = num_bands;
    m_storage.assign(this->num_columns()*(3*num_bands + 1), T{0});
}

template <typename T>
inline void BandedMatrix<T>::assign(std::size_t size, std::size_t num_bands,
        Vector<T> underlying_data) {
    MatrixBase<T>::initialise(size);
    m_num_bands = num_bands;

    if (underlying_data.size() != size*(3*num_bands + 1)) {
        m_storage.assign(size*(3*num_bands + 1), T{0});
        throw RuntimeError{InvalidArgumentError{.argument = "underlying_data",
            .value = std::format("Vector of size {}", underlying_data.size()),
            .expected = std::format(
                    "Vector of size num_columns*(3*num_bands + 1) = {}x{} = {}",
                    size, 3*num_bands + 1, size*(3*num_bands + 1))}};
    } else {
        m_storage = std::move(underlying_data);
    }
}

template <typename T>
inline void BandedMatrix<T>::assign(Vector<T> underlying_data) {
    if (underlying_data.size() != m_storage.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = m_storage.size(),
            .name2 = "underlying_data", .size2 = underlying_data.size()}};
    } else {
        m_storage = std::move(underlying_data);
    }
}

template <typename T>
template <class InputIt>
inline void BandedMatrix<T>::assign(InputIt first, InputIt last) {
#ifndef NDEBUG
    if (last != first + m_storage.size()) {
        throw RuntimeError{InvalidArgumentError{.argument = "last",
            .value = std::format("first + {}", last - first),
            .expected = "first - last == m_storage.size()"}};
    }
#endif  // NDEBUG

    m_storage.assign(first, last);
}

template <typename T>
inline auto BandedMatrix<T>::num_bands() const -> std::size_t {
    return m_num_bands;
}

template <typename T>
inline auto BandedMatrix<T>::num_elements() const -> std::size_t {
    return m_storage.size();
}

/// Note that the internal storage takes the form of a dense matrix with
/// `num_columns()` columns and `1 + 3*#num_bands()` rows. Refer to the
/// <a href="http://www.netlib.org/lapack/lug/node124.html">LAPACK User
/// Guide</a> for further details about the band storage scheme.
///
/// The leading diagonal of the matrix occupies row `1 + 2*#num_bands()` of the
/// internal storage. Remembering that an element in column `j` in the matrix
/// remains in column `j` in the internal storage, we see that the element
/// `(i, j)` must be mapped to the element `(1 + 2*#num_bands() + i - j, j)` in
/// the internal storage.
template <typename T>
inline auto BandedMatrix<T>::operator[](std::size_t row,
        std::size_t column) const -> T {
#ifndef NDEBUG
    if (row >= this->num_rows() || column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    return (row > column + num_bands() || column > row + 2*num_bands()) ?
        T{0} : m_storage[num_bands()*(3*column + 2) + row];
}

/// Note that the internal storage takes the form of a dense matrix with
/// `num_columns()` columns and `1 + 3*#num_bands()` rows. Refer to the
/// <a href="http://www.netlib.org/lapack/lug/node124.html">LAPACK User
/// Guide</a> for further details about the band storage scheme.
///
/// The leading diagonal of the matrix occupies row `1 + 2*#num_bands()` of the
/// internal storage. Remembering that an element in column `j` in the matrix
/// remains in column `j` in the internal storage, we see that the element
/// `(i, j)` must be mapped to the element `(1 + 2*#num_bands() + i - j, j)` in
/// the internal storage.
template <typename T>
inline auto BandedMatrix<T>::get_unsafe(std::size_t row, std::size_t column)
        -> T& {
#ifndef NDEBUG
    if (row >= this->num_rows() || column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = this->size()}};
    }
    if (row > column + num_bands() || column > row + 2*num_bands()) {
        throw RuntimeError{InvalidArgumentError{.argument = "(row, column)",
            .value = std::format("({}, {}) with num_bands() = {}", row, column,
                    num_bands()),
            .expected = "row <= column + num_bands() "
                "&& column <= row + 2*num_bands()"}};
    }
#endif  // NDEBUG

    return m_storage[num_bands()*(3*column + 2) + row];
}

/// Note that the internal storage takes the form of a dense matrix with
/// `num_columns()` columns and `1 + 3*#num_bands()` rows. Refer to the
/// <a href="http://www.netlib.org/lapack/lug/node124.html">LAPACK User
/// Guide</a> for further details about the band storage scheme.
///
/// The leading diagonal of the matrix occupies row `1 + 2*#num_bands()` of the
/// internal storage. Remembering that an element in column `j` in the matrix
/// remains in column `j` in the internal storage, we see that the element
/// `(i, j)` must be mapped to the element `(1 + 2*#num_bands() + i - j, j)` in
/// the internal storage.
template <typename T>
inline auto BandedMatrix<T>::set(std::size_t row, std::size_t column,
        const T& value) -> bool {
#ifndef NDEBUG
    if (row >= this->num_rows() || column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    if (row > column + num_bands() || column > row + 2*num_bands()) {
        return false;
    } else {
        m_storage[num_bands()*(3*column + 2) + row] = value;
        return true;
    }
}

template <typename T>
inline auto BandedMatrix<T>::begin() const -> ConstIterator {
    return m_storage.begin();
}

template <typename T>
inline auto BandedMatrix<T>::end() const -> ConstIterator {
    return m_storage.end();
}

template <typename T>
inline auto BandedMatrix<T>::begin() -> Iterator {
    return m_storage.begin();
}

template <typename T>
inline auto BandedMatrix<T>::end() -> Iterator {
    return m_storage.end();
}

template <typename T>
inline void BandedMatrix<T>::fill(const T& value) {
    m_storage.fill(value);
}

template <typename T>
inline void BandedMatrix<T>::zero() {
    m_storage.zero();
}

#ifndef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
template <typename T>
inline auto BandedMatrix<T>::operator+() const -> const BandedMatrix& {
    return *this;
}

template <typename T>
inline auto BandedMatrix<T>::operator-() const -> BandedMatrix {
    BandedMatrix<T> result{*this};
    result *= T{-1};
    return result;
}

template <typename T>
template <std::convertible_to<T> U>
inline auto BandedMatrix<T>::operator+=(const BandedMatrix<U>& rhs)
        -> BandedMatrix& {
#ifndef NDEBUG
    if (this->size() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
    if (num_bands() != rhs.num_bands()) {
        throw RuntimeError{InvalidArgumentError{.argument = "rhs.num_bands()",
            .value = std::format("{}", rhs.num_bands()),
            .expected = std::format("{}", num_bands())}};
    }
#endif  // NDEBUG

    m_storage += rhs.as_vector();
    return *this;
}

template <typename T>
template <std::convertible_to<T> U>
inline auto BandedMatrix<T>::operator-=(const BandedMatrix<U>& rhs)
        -> BandedMatrix& {
#ifndef NDEBUG
    if (this->size() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
    if (num_bands() != rhs.num_bands()) {
        throw RuntimeError{InvalidArgumentError{.argument = "rhs.num_bands()",
            .value = std::format("{}", rhs.num_bands()),
            .expected = std::format("{}", num_bands())}};
    }
#endif  // NDEBUG

    m_storage -= rhs.as_vector();
    return *this;
}
#else
template <typename T>
template <BandedMatrixExpressionConvertibleTo<T> Expr>
inline auto BandedMatrix<T>::operator+=(const Expr& expr) -> BandedMatrix& {
#ifndef NDEBUG
    if (this->size() != expr.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "expr", .size2 = expr.size()}};
    }
    if (num_bands() != expr.num_bands()) {
        throw RuntimeError{InvalidArgumentError{.argument = "expr.num_bands()",
            .value = std::format("{}", expr.num_bands()),
            .expected = std::format("{}", num_bands())}};
    }
#endif  // NDEBUG

    m_storage += expr.as_vector();
    return *this;
}

template <typename T>
template <BandedMatrixExpressionConvertibleTo<T> Expr>
inline auto BandedMatrix<T>::operator-=(const Expr& expr) -> BandedMatrix& {
#ifndef NDEBUG
    if (this->size() != expr.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "expr", .size2 = expr.size()}};
    }
    if (num_bands() != expr.num_bands()) {
        throw RuntimeError{InvalidArgumentError{.argument = "expr.num_bands()",
            .value = std::format("{}", expr.num_bands()),
            .expected = std::format("{}", num_bands())}};
    }
#endif  // NDEBUG

    m_storage -= expr.as_vector();
    return *this;
}
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

template <typename T>
template <std::convertible_to<T> U>
inline auto BandedMatrix<T>::operator*=(const U& k) -> BandedMatrix& {
    m_storage *= k;
    return *this;
}

template <typename T>
template <std::convertible_to<T> U>
inline auto BandedMatrix<T>::operator/=(const U& k) -> BandedMatrix& {
    m_storage /= k;
    return *this;
}

template <typename T>
inline auto BandedMatrix<T>::data() -> T* {
    return m_storage.data();
}

template <typename T>
inline auto BandedMatrix<T>::data() const -> const T* {
    return m_storage.data();
}

template <typename T>
inline auto BandedMatrix<T>::as_vector() const -> const Vector<T>& {
    return m_storage;
}

/// Loads a block of data from the given string by reading all valid values into
/// the underlying storage. The only thing that will be checked is that the
/// total number of values read is equal to `num_elements()`. Note that this
/// means there is no restriction on the formatting of the data, it may appear
/// as a series of equal-length lines (each line corresponding to a column), a
/// single line, or even lines of unequal length.
/// TODO: add metadata to load arbitrary matrices
template <typename T>
inline void BandedMatrix<T>::operator<<(std::string data) {
    Vector<T> new_data;
    new_data << std::move(data);
    if (new_data.size() != m_storage.size()) {
        throw RuntimeError{InvalidArgumentError{.argument = "data",
            .value = std::format("(matrix data with a total of {} elements)",
                    new_data.size()),
            .expected = std::format(
                    "square matrix size {} with {} bands, total of {} elements",
                    this->num_columns(), num_bands(),
                    (3*num_bands() + 1)*this->num_columns())}};
    }

    m_storage = std::move(new_data);
}

/// Note that the internal storage takes the form of a dense matrix with
/// `num_columns()` columns and `1 + 3*#num_bands()` rows. Refer to the
/// <a href="http://www.netlib.org/lapack/lug/node124.html">LAPACK User
/// Guide</a> for further details about the band storage scheme.
///
/// The leading diagonal of the matrix occupies row `1 + 2*#num_bands()` of the
/// internal storage. Remembering that an element in column `j` in the matrix
/// remains in column `j` in the internal storage, we see that the element
/// `(i, j)` must be mapped to the element `(1 + 2*#num_bands() + i - j, j)` in
/// the internal storage. The matrix is output as the transpose.
template <typename T>
inline auto BandedMatrix<T>::as_string() const -> std::string {
    // TODO: ranges?
    std::ostringstream oss;
    for (std::size_t j{0}, size{this->num_columns()}; j < size; ++j) {
        for (std::size_t i{0}, height{3*num_bands() + 1}; i < height; ++i) {
            oss << m_storage[j*height + i] << ' ';
        }
        oss << '\n';
    }

    return oss.str();
}

#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
/// \relates BandedMatrix
template <BandedMatrixExpression Expr>
inline constexpr auto evaluate(const Expr& expr)
        -> BandedMatrix<typename Expr::InnerExpressionType::ValueType> {
    return {expr};
}
#else
/// \relates BandedMatrix
template <typename T>
constexpr auto evaluate(const BandedMatrix<T>& M) -> const BandedMatrix<T>& {
    return M;
}
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

#ifndef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
/// \relates BandedMatrix
/// \brief Addition of two BandedMatrices.
template <typename T, typename U, typename R>
inline auto operator+(const BandedMatrix<T>& lhs, const BandedMatrix<U>& rhs)
        -> BandedMatrix<R> {
    if constexpr (std::is_same_v<T, R>) {
        BandedMatrix<R> result{lhs};
        result += rhs;
        return result;
    } else {
        BandedMatrix<R> result{rhs};
        result += lhs;
        return result;
    }
}

/// \relates BandedMatrix
/// \brief Addition of two BandedMatrices.
template <typename T, typename U, typename R>
inline auto operator+(BandedMatrix<T>&& lhs, const BandedMatrix<U>& rhs)
        -> BandedMatrix<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs += rhs;
        return lhs;
    } else {
        BandedMatrix<R> result{rhs};
        result += lhs;
        return result;
    }
}

/// \relates BandedMatrix
/// \brief Addition of two BandedMatrices.
template <typename T, typename U, typename R>
inline auto operator+(const BandedMatrix<T>& lhs, BandedMatrix<U>&& rhs)
        -> BandedMatrix<R> {
    if constexpr (std::is_same_v<U, R>) {
        rhs += lhs;
        return rhs;
    } else {
        BandedMatrix<R> result{lhs};
        result += rhs;
        return result;
    }
}

/// \relates BandedMatrix
/// \brief Addition of two BandedMatrices.
template <typename T, typename U, typename R>
inline auto operator+(BandedMatrix<T>&& lhs, BandedMatrix<U>&& rhs)
        -> BandedMatrix<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs += rhs;
        return lhs;
    } else if constexpr (std::is_same_v<U, R>) {
        rhs += lhs;
        return rhs;
    } else {
        BandedMatrix<R> result{lhs};
        result += rhs;
        return result;
    }
}

/// \relates BandedMatrix
/// \brief Difference of two BandedMatrices.
template <typename T, typename U, typename R>
inline auto operator-(const BandedMatrix<T>& lhs, const BandedMatrix<U>& rhs)
        -> BandedMatrix<R> {
    BandedMatrix<R> result{lhs};
    result -= rhs;
    return result;
}

/// \relates BandedMatrix
/// \brief Difference of two BandedMatrices.
template <typename T, typename U, typename R>
inline auto operator-(BandedMatrix<T>&& lhs, const BandedMatrix<U>& rhs)
        -> BandedMatrix<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs -= rhs;
        return lhs;
    } else {
        BandedMatrix<R> result{lhs};
        result -= rhs;
        return result;
    }
}

/// \relates BandedMatrix
/// \brief Difference of two BandedMatrices.
template <typename T, typename U, typename R>
inline auto operator-(const BandedMatrix<T>& lhs, BandedMatrix<U>&& rhs)
        -> BandedMatrix<R> {
    if constexpr (std::is_same_v<U, R>) {
        rhs *= R{-1};
        rhs += lhs;
        return rhs;
    } else {
        BandedMatrix<R> result{lhs};
        result -= rhs;
        return result;
    }
}

/// \relates BandedMatrix
/// \brief Difference of two BandedMatrices.
template <typename T, typename U, typename R>
inline auto operator-(BandedMatrix<T>&& lhs, BandedMatrix<U>&& rhs)
        -> BandedMatrix<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs -= rhs;
        return lhs;
    } else if constexpr (std::is_same_v<U, R>) {
        rhs *= R{-1};
        rhs += lhs;
        return rhs;
    } else {
        BandedMatrix<R> result{lhs};
        result -= rhs;
        return result;
    }
}

/// \relates BandedMatrix
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R>
inline auto operator*(const T& lhs, const BandedMatrix<U>& rhs)
        -> BandedMatrix<R> {
    BandedMatrix<R> result{rhs};
    result *= lhs;
    return result;
}

/// \relates BandedMatrix
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R>
inline auto operator*(const T& lhs, BandedMatrix<U>&& rhs) -> BandedMatrix<R> {
    if constexpr (std::is_same_v<U, R>) {
        rhs *= lhs;
        return rhs;
    } else {
        BandedMatrix<R> result{rhs};
        result *= lhs;
        return result;
    }
}

/// \relates BandedMatrix
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R>
inline auto operator*(const BandedMatrix<T>& lhs, const U& rhs)
        -> BandedMatrix<R> {
    BandedMatrix<R> result{lhs};
    result *= rhs;
    return result;
}

/// \relates BandedMatrix
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R>
inline auto operator*(BandedMatrix<T>&& lhs, const U& rhs) -> BandedMatrix<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs *= rhs;
        return lhs;
    } else {
        BandedMatrix<R> result{lhs};
        result *= rhs;
        return result;
    }
}
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

/// \relates BandedMatrix
/// \brief Right-hand-side multiplication by vector.
template <typename T, typename U, typename R>
inline auto operator*(const BandedMatrix<T>& lhs, const Vector<U>& rhs)
        -> Vector<R> {
#ifndef NDEBUG
    if (lhs.num_columns() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = {rhs.size(), 1}}};
    }
#endif  // NDEBUG

    std::size_t N{lhs.num_rows()}, X{lhs.num_columns()};
    Vector<R> result(N);
    for (std::size_t i{0}; i < X; ++i) {
        for (std::size_t row{0}; row < N; ++row) {
            result[row] += lhs[row, i]*rhs[i];
        }
    }

    return result;
}

#ifndef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
/// \relates BandedMatrix
/// \brief Division by a scalar.
template <typename T, typename U, typename R>
inline auto operator/(const BandedMatrix<T>& lhs, const U& rhs)
        -> BandedMatrix<R> {
    BandedMatrix<R> result{lhs};
    result /= rhs;
    return result;
}

/// \relates BandedMatrix
/// \brief Division by a scalar.
template <typename T, typename U, typename R>
inline auto operator/(BandedMatrix<T>&& lhs, const U& rhs) -> BandedMatrix<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs /= rhs;
        return lhs;
    } else {
        BandedMatrix<R> result{lhs};
        result /= rhs;
        return result;
    }
}

/// \relates BandedMatrix
template <typename T>
inline auto exp(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), exp(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto log(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), log(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto pow(const BandedMatrix<T>& M, T p) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), pow(M.as_vector(), p)};
}

/// \relates BandedMatrix
template <typename T>
inline auto sin(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), sin(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto cos(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), cos(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto tan(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), tan(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto asin(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), asin(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto acos(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), acos(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto atan(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), atan(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto sinh(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), sinh(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto cosh(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), cosh(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto tanh(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), tanh(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto asinh(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), asinh(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto acosh(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), acosh(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto atanh(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), atanh(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto abs(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), abs(M.as_vector())};
}

/// \relates BandedMatrix
template <typename T>
inline auto sgn(const BandedMatrix<T>& M) -> BandedMatrix<T> {
    return {M.size().first, M.num_bands(), sgn(M.as_vector())};
}
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
}   // namespace jump

#endif  // JUMP_BANDED_MATRIX_HPP

