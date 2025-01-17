// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_DENSE_MATRIX_HPP
#define JUMP_DENSE_MATRIX_HPP

#include "jump/autodiff/dual.hpp"
#include "jump/data/matrix_base.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/third_party.hpp"
#include "jump/utility/types.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace jump {
/// \brief Stores all elements of a general \f$m\f$ by \f$n\f$ matrix, with
/// common arithmetic operations enabled.
///
/// Implements the internal storage of a dense matrix by storing all elements of
/// an \f$m\f$ by \f$n\f$ matrix as a contiguous `Vector` of length \f$nm\f$. In
/// particular, the element in the matrix occupying row \f$i\f$th and column
/// \f$j\f$ is found at index \f$im+j\f$ in the underlying storage (see
/// documentation for DenseMatrix::m_storage).
///
/// Most operations are simply passed through in some form to the underlying
/// `Vector` storage, including simple element-wise arithmetic operations.
/// `DenseMatrix`-`Vector` operations are implemented separately. Generally, a
/// simple default version is provided, but in the case of the preprocessor
/// definition `JUMP_HAS_CBLAS` existing we instead use an appropriate CBLAS
/// routine.
template <typename T>
class DenseMatrix : public MatrixBase<T> {
    public:
        /// \brief Iterator for algorithms.
        using Iterator = typename Vector<T>::Iterator;
        /// \brief Iterator for algorithms.
        using ConstIterator = typename Vector<T>::ConstIterator;

        /// \brief Construct square matrix with the given size.
        explicit DenseMatrix(std::size_t size = 0);
        /// \brief Construct a general matrix with the given size.
        DenseMatrix(std::size_t num_rows, std::size_t num_columns);
        /// \brief Constuct a matrix with the given Vector data, specifying a
        /// consistent size.
        DenseMatrix(std::size_t num_rows, std::size_t num_columns,
                Vector<T> underlying_data);
        /// \brief Default copy constructor.
        DenseMatrix(const DenseMatrix& other) = default;
        /// \brief Templated copy constructor.
        template <typename U>
        DenseMatrix(const DenseMatrix<U>& other);
        /// \brief Default move constructor.
        DenseMatrix(DenseMatrix&& other) = default;

        /// \brief Default copy assignment.
        auto operator=(const DenseMatrix& other) -> DenseMatrix& = default;
        /// \brief Default move assignment.
        auto operator=(DenseMatrix&& other) -> DenseMatrix& = default;

        /// \brief Initialise a square matrix with the given size.
        void assign(std::size_t size);
        /// \brief Initialise a general matrix with the given size.
        void assign(std::size_t num_rows, std::size_t num_columns);
        /// \brief Initialise a matrix with the given Vector data, specifying a
        /// consistent size.
        void assign(std::size_t num_rows, std::size_t num_columns,
                Vector<T> underlying_data);
        /// \brief Set matrix storage with the given Vector data, which must
        /// match the existing container size.
        void assign(Vector<T> underlying_data);
        /// \brief Set data via a pair of iterators.
        template <class InputIt>
        void assign(InputIt first, InputIt last);
        /// \brief Return size of internal storage.
        auto num_elements() const -> std::size_t override;

        /// \brief Defaulted spaceship operator.
        auto operator<=>(const DenseMatrix&) const = default;

        /// \brief Const element access.
        auto operator[](std::size_t row, std::size_t column) const -> const T&;
        /// \brief Mutable element access.
        auto operator[](std::size_t row, std::size_t column) -> T&;

        /// \brief Const iterator for algorithms.
        auto begin() const -> ConstIterator;
        /// \brief Const iterator for algorithms.
        auto end() const -> ConstIterator;
        /// \brief Iterator for algorithms.
        auto begin() -> Iterator;
        /// \brief Iterator for algorithms.
        auto end() -> Iterator;
        /// \brief Return iterators to extract/manipulate a column.
        auto column_iterators(std::size_t column)
                -> std::pair<Iterator, Iterator>;
        /// \brief Return iterators to extract/manipulate a column.
        auto column_iterators(std::size_t column) const
                -> std::pair<ConstIterator, ConstIterator>;

        /// \brief Fill matrix with given value.
        void fill(const T& value);
        /// \brief Zero the matrix.
        virtual void zero() override;

        /// \brief No operation on matrix.
        auto operator+() const -> const DenseMatrix&;
        /// \brief Negate matrix.
        auto operator-() const -> DenseMatrix;
        /// \brief Add two matrices together in place.
        auto operator+=(const DenseMatrix& rhs) -> DenseMatrix&;
        /// \brief Subtract a matrix from another in place.
        auto operator-=(const DenseMatrix& rhs) -> DenseMatrix&;
        /// \brief Multiply matrix by scalar in place.
        auto operator*=(const T& k) -> DenseMatrix&;
        /// \brief Multiply matrix by another in place
        auto operator*=(const DenseMatrix& rhs) -> DenseMatrix&;
        /// \brief Divide matrix by scalar in place.
        auto operator/=(const T& k) -> DenseMatrix&;

        /// \brief Return sum of element magnitudes in a column.
        auto column_L1_norm(std::size_t column) const -> Real;
        /// \brief Return Euclidean norm of a column.
        auto column_L2_norm(std::size_t column) const -> Real;
        /// \brief Return maximum magnitude over all elements in a column.
        auto column_Linf_norm(std::size_t column) const -> Real;

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

        /// \brief Return identity matrix of specified size.
        static auto identity(std::size_t size) -> DenseMatrix;

    private:
        /// \brief Internal contiguous storage.
        ///
        /// The internal storage for a dense matrix consists of a contiguous
        /// `Vector` containing `num_rows()*#num_columns()` elements. The matrix
        /// elements are stored in column-major format, so that the element at
        /// row `i` and column `j` appears at location
        /// `m_storage[j*num_rows() + i]`.
        Vector<T> m_storage;
};

/// \relates DenseMatrix
/// \brief Addition of two DenseMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator+(const DenseMatrix<T>& lhs, const DenseMatrix<U>& rhs)
        -> DenseMatrix<R>;

/// \relates DenseMatrix
/// \brief Difference of two DenseMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator-(const DenseMatrix<T>& lhs, const DenseMatrix<U>& rhs)
        -> DenseMatrix<R>;

/// \relates DenseMatrix
/// \brief Right-hand-side multiplication by vector.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const DenseMatrix<T>& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates DenseMatrix
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const T& lhs, const DenseMatrix<U>& rhs) -> DenseMatrix<R>;

/// \relates DenseMatrix
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const DenseMatrix<T>& lhs, const U& rhs) -> DenseMatrix<R>;

/// \relates DenseMatrix
/// \brief Multiplication of two DenseMatrices.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const DenseMatrix<T>& lhs, const DenseMatrix<U>& rhs)
        -> DenseMatrix<R>;

/// \relates DenseMatrix
/// \brief Division by a scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(const DenseMatrix<T>& lhs, const U& rhs) -> DenseMatrix<R>;

/// \relates DenseMatrix
/// \brief Addition of two DenseMatrices.
template <typename T>
auto operator+(DenseMatrix<T> lhs, const DenseMatrix<T>& rhs) -> DenseMatrix<T>;

/// \relates DenseMatrix
/// \brief Addition of two DenseMatrices.
template <typename T>
auto operator+(const DenseMatrix<T>& lhs, DenseMatrix<T>&& rhs)
        -> DenseMatrix<T>;

/// \relates DenseMatrix
/// \brief Difference of two DenseMatrices.
template <typename T>
auto operator-(DenseMatrix<T> lhs, const DenseMatrix<T>& rhs) -> DenseMatrix<T>;

/// \relates DenseMatrix
/// \brief Difference of two DenseMatrices.
template <typename T>
auto operator-(const DenseMatrix<T>& lhs, DenseMatrix<T>&& rhs)
        -> DenseMatrix<T>;

/// \relates DenseMatrix
/// \brief Right-hand-side multiplication by vector.
template <typename T>
auto operator*(const DenseMatrix<T>& lhs, const Vector<T>& rhs) -> Vector<T>;

/// \relates DenseMatrix
/// \brief Left-hand multiplication by scalar.
template <typename T>
auto operator*(const T& lhs, DenseMatrix<T> rhs) -> DenseMatrix<T>;

/// \relates DenseMatrix
/// \brief Right-hand multiplication by scalar.
template <typename T>
auto operator*(DenseMatrix<T> lhs, const T& rhs) -> DenseMatrix<T>;

/// \relates DenseMatrix
/// \brief Multiplication of two DenseMatrices.
template <typename T>
auto operator*(DenseMatrix<T> lhs, const DenseMatrix<T>& rhs) -> DenseMatrix<T>;

/// \relates DenseMatrix
/// \brief Division by a scalar.
template <typename T>
auto operator/(DenseMatrix<T> lhs, const T& rhs) -> DenseMatrix<T>;

// ========================================================================
// Implementation
// ========================================================================

template <typename T>
inline DenseMatrix<T>::DenseMatrix(std::size_t size) :
    MatrixBase<T>{size},
    m_storage(size*size, T{0}) {
}

template <typename T>
inline DenseMatrix<T>::DenseMatrix(std::size_t num_rows,
        std::size_t num_columns) :
    MatrixBase<T>{num_rows, num_columns},
    m_storage(num_rows*num_columns, T{0}) {
}

template <typename T>
inline DenseMatrix<T>::DenseMatrix(std::size_t num_rows,
        std::size_t num_columns, Vector<T> underlying_data) :
    MatrixBase<T>{num_rows, num_columns} {

    if (underlying_data.size() != num_rows*num_columns) {
        m_storage.assign(num_rows*num_columns, T{0});
        throw RuntimeError{InvalidArgumentError{.argument = "underlying_data",
            .value = std::format("Vector of size {}", underlying_data.size()),
            .expected = std::format(
                    "Vector of size num_rows*num_columns = {}x{} = {}",
                    num_rows, num_columns, num_rows*num_columns)}};
    } else {
        m_storage = std::move(underlying_data);
    }
}

template <typename T>
template <typename U>
inline DenseMatrix<T>::DenseMatrix(const DenseMatrix<U>& other) :
    MatrixBase<T>{other.size()} {
    m_storage = other.as_vector();
}

template <typename T>
inline void DenseMatrix<T>::assign(std::size_t size) {
    MatrixBase<T>::initialise(size);
    m_storage.assign(this->num_rows()*this->num_columns(), T{0});
}

template <typename T>
inline void DenseMatrix<T>::assign(std::size_t num_rows,
        std::size_t num_columns) {
    MatrixBase<T>::initialise(num_rows, num_columns);
    m_storage.assign(num_rows*num_columns, T{0});
}

template <typename T>
inline void DenseMatrix<T>::assign(std::size_t num_rows,
        std::size_t num_columns, Vector<T> underlying_data) {
    MatrixBase<T>::initialise(num_rows, num_columns);

    if (underlying_data.size() != num_rows*num_columns) {
        m_storage.assign(num_rows*num_columns, T{0});
        throw RuntimeError{InvalidArgumentError{.argument = "underlying_data",
            .value = std::format("Vector of size {}", underlying_data.size()),
            .expected = std::format(
                    "Vector of size num_rows*num_columns = {}x{} = {}",
                    num_rows, num_columns, num_rows*num_columns)}};
    } else {
        m_storage = std::move(underlying_data);
    }
}

template <typename T>
inline void DenseMatrix<T>::assign(Vector<T> underlying_data) {
    if (underlying_data.size() != m_storage.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = m_storage.size(),
            .name2 = "underlying_data", .size2 = underlying_data.size()}};
    } else {
        m_storage = std::move(underlying_data);
    }
}

template <typename T>
template <class InputIt>
inline void DenseMatrix<T>::assign(InputIt first, InputIt last) {
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
inline auto DenseMatrix<T>::num_elements() const -> std::size_t {
    return m_storage.size();
}

/// The element at row `i` and column `j` appears at location
/// `m_storage[j*num_rows() + i]` (see `DenseMatrix::m_storage`).
template <typename T>
inline auto DenseMatrix<T>::operator[](std::size_t row,
        std::size_t column) const -> const T& {
#ifndef NDEBUG
    if (row >= this->num_rows() || column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    return m_storage[column*this->num_rows() + row];
}

/// The element at row `i` and column `j` appears at location
/// `m_storage[j*num_rows() + i]` (see `DenseMatrix::m_storage`).
template <typename T>
inline auto DenseMatrix<T>::operator[](std::size_t row, std::size_t column)
        -> T& {
#ifndef NDEBUG
    if (row >= this->num_rows() || column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    return m_storage[column*this->num_rows() + row];
}

template <typename T>
inline auto DenseMatrix<T>::begin() const -> ConstIterator {
    return m_storage.begin();
}

template <typename T>
inline auto DenseMatrix<T>::end() const -> ConstIterator {
    return m_storage.end();
}

template <typename T>
inline auto DenseMatrix<T>::begin() -> Iterator {
    return m_storage.begin();
}

template <typename T>
inline auto DenseMatrix<T>::end() -> Iterator {
    return m_storage.end();
}

/// Keeping in mind that the data is stored internally in column-major format,
/// the beginning of column `j` is at index `num_rows()*j` while the end of the
/// column is at index `num_rows()*(j + 1)`.
template <typename T>
inline auto DenseMatrix<T>::column_iterators(std::size_t column)
        -> std::pair<Iterator, Iterator> {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    return {m_storage.begin() + this->m_num_rows*column,
            m_storage.begin() + this->m_num_rows*(column + 1)};
}

/// Keeping in mind that the data is stored internally in column-major format,
/// the beginning of column `j` is at index `num_rows()*j` while the end of the
/// column is at index `num_rows()*(j + 1)`.
template <typename T>
inline auto DenseMatrix<T>::column_iterators(std::size_t column) const
        -> std::pair<ConstIterator, ConstIterator> {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    return {m_storage.begin() + this->m_num_rows*column,
            m_storage.begin() + this->m_num_rows*(column + 1)};
}

template <typename T>
inline void DenseMatrix<T>::fill(const T& value) {
    m_storage.fill(value);
}

template <typename T>
inline void DenseMatrix<T>::zero() {
    m_storage.zero();
}

template <typename T>
inline auto DenseMatrix<T>::operator+() const -> const DenseMatrix& {
    return *this;
}

template <typename T>
inline auto DenseMatrix<T>::operator-() const -> DenseMatrix {
    DenseMatrix<T> result{*this};
    result *= T{-1};
    return result;
}

template <typename T>
inline auto DenseMatrix<T>::operator+=(const DenseMatrix& rhs) -> DenseMatrix& {
#ifndef NDEBUG
    if (this->size() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    m_storage += rhs.m_storage;
    return *this;
}

template <typename T>
inline auto DenseMatrix<T>::operator-=(const DenseMatrix& rhs) -> DenseMatrix& {
#ifndef NDEBUG
    if (this->size() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    m_storage -= rhs.m_storage;
    return *this;
}

template <typename T>
inline auto DenseMatrix<T>::operator*=(const T& k) -> DenseMatrix& {
    m_storage *= k;
    return *this;
}

template <typename T>
inline auto DenseMatrix<T>::operator*=(const DenseMatrix<T>& rhs)
        -> DenseMatrix& {
#ifndef NDEBUG
    if (this->num_columns() != rhs.num_rows()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // TODO: ranges
    std::size_t N{this->num_rows()}, M{rhs.num_columns()};
    std::size_t X{this->num_columns()};
    DenseMatrix<T> result{N, M};
    for (std::size_t col{0}; col < M; ++col) {
        for (std::size_t row{0}; row < N; ++row) {
            for (std::size_t i{0}; i < X; ++i) {
                result[row, col] += (*this)[row, i]*rhs[i, col];
            }
        }
    }

    return *this = std::move(result);
}

template <typename T>
inline auto DenseMatrix<T>::operator/=(const T& k) -> DenseMatrix& {
    m_storage /= k;
    return *this;
}

template <typename T>
inline auto DenseMatrix<T>::column_L1_norm(std::size_t column) const -> Real {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    auto it{column_iterators(column)};
    if constexpr (is_dual_v<T>) {
        auto F{[](Real acc, const T& x) { return acc + abs(x).value; }};
        return std::accumulate(it.first, it.second, Real{0}, F);
    } else {
        auto F{[](Real acc, const T& x) { return acc + std::abs(x); }};
        return std::accumulate(it.first, it.second, Real{0}, F);
    }
}

template <typename T>
inline auto DenseMatrix<T>::column_L2_norm(std::size_t column) const -> Real {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    auto it{column_iterators(column)};
    if constexpr (is_dual_v<T>) {
        auto F{[](Real acc, const T& x) {
            return acc + pow(abs(x), 2.).value; }};
        return std::sqrt(std::accumulate(it.first, it.second, Real{0}, F));
    } else {
        auto F{[](Real acc, const T& x) {
            return acc + std::pow(std::abs(x), 2.); }};
        return std::sqrt(std::accumulate(it.first, it.second, Real{0}, F));
    }
}

template <typename T>
inline auto DenseMatrix<T>::column_Linf_norm(std::size_t column) const -> Real {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    auto it{column_iterators(column)};
    if constexpr (is_dual_v<T>) {
        auto F{[](const T& x) { return abs(x).value; }};
        return F(*std::ranges::max_element(it.first, it.second, {}, F));
    } else {
        auto F{[](const T& x) { return std::abs(x); }};
        return F(*std::ranges::max_element(it.first, it.second, {}, F));
    }
}

template <typename T>
inline auto DenseMatrix<T>::data() -> T* {
    return m_storage.data();
}

template <typename T>
inline auto DenseMatrix<T>::data() const -> const T* {
    return m_storage.data();
}

template <typename T>
inline auto DenseMatrix<T>::as_vector() const -> const Vector<T>& {
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
inline void DenseMatrix<T>::operator<<(std::string data) {
    Vector<T> new_data;
    new_data << std::move(data);
    if (new_data.size() != m_storage.size()) {
        throw RuntimeError{InvalidArgumentError{.argument = "data",
            .value = std::format("(matrix data with a total of {} elements)",
                    new_data.size()),
            .expected = std::format(
                    "matrix data with a total of {}x{} = {} elements",
                    this->num_rows(), this->num_columns(),
                    this->num_rows()*this->num_columns())}};
    }

    m_storage = std::move(new_data);
}

/// Iterates through columns of matrix and dumps them to a string, formatted as
/// the transpose.
template <typename T>
inline auto DenseMatrix<T>::as_string() const -> std::string {
    std::ostringstream oss;
    auto inserter{[&oss](const auto& x) { oss << x << ' '; }};
    for (std::size_t i{0}, num_cols{this->num_columns()}; i < num_cols; ++i) {
        auto it{column_iterators(i)};
        std::ranges::for_each(it.first, it.second, inserter);
        oss << '\n';
    }

    return oss.str();
}

template <typename T>
inline auto DenseMatrix<T>::identity(std::size_t size) -> DenseMatrix {
    DenseMatrix<T> I{size};
    for (std::size_t i{0}; i < size; ++i) {
        I[i, i] = T{1};
    }
    return I;
}

/// \relates DenseMatrix
/// \brief Addition of two DenseMatrices.
template <typename T, typename U, typename R>
inline auto operator+(const DenseMatrix<T>& lhs, const DenseMatrix<U>& rhs)
        -> DenseMatrix<R> {
    if constexpr (std::is_same_v<T, R>) {
        DenseMatrix<R> result{rhs};
        result += lhs;
        return result;
    } else {
        DenseMatrix<R> result{lhs};
        result += rhs;
        return result;
    }
}

/// \relates DenseMatrix
/// \brief Difference of two DenseMatrices.
template <typename T, typename U, typename R>
inline auto operator-(const DenseMatrix<T>& lhs, const DenseMatrix<U>& rhs)
        -> DenseMatrix<R> {
    if constexpr (std::is_same_v<T, R>) {
        DenseMatrix<R> result{rhs};
        result *= R{-1};
        result += lhs;
        return result;
    } else {
        DenseMatrix<R> result{lhs};
        result -= rhs;
        return result;
    }
}

/// \relates DenseMatrix
/// \brief Right-hand-side multiplication by vector.
template <typename T, typename U, typename R>
inline auto operator*(const DenseMatrix<T>& lhs, const Vector<U>& rhs)
        -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        return lhs*Vector<R>{rhs};
    } else {
        return DenseMatrix<R>{lhs}*rhs;
    }
}

/// \relates DenseMatrix
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R>
inline auto operator*(const T& lhs, const DenseMatrix<U>& rhs)
        -> DenseMatrix<R> {
    DenseMatrix<R> result{rhs};
    result *= lhs;
    return result;
}

/// \relates DenseMatrix
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R>
inline auto operator*(const DenseMatrix<T>& lhs, const U& rhs)
        -> DenseMatrix<R> {
    DenseMatrix<R> result{lhs};
    result *= rhs;
    return result;
}

/// \relates DenseMatrix
/// \brief Multiplication of two DenseMatrices.
template <typename T, typename U, typename R>
inline auto operator*(const DenseMatrix<T>& lhs, const DenseMatrix<U>& rhs)
        -> DenseMatrix<R> {
#ifndef NDEBUG
    if (lhs.num_columns() != rhs.num_rows()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // TODO: ranges
    std::size_t N{lhs.num_rows()}, M{rhs.num_columns()};
    std::size_t X{lhs.num_columns()};
    DenseMatrix<R> result{N, M};
    for (std::size_t col{0}; col < M; ++col) {
        for (std::size_t row{0}; row < N; ++row) {
            for (std::size_t i{0}; i < X; ++i) {
                result[row, col] += lhs[row, i]*rhs[i, col];
            }
        }
    }

    return result;
}

/// \relates DenseMatrix
/// \brief Division by a scalar.
template <typename T, typename U, typename R>
inline auto operator/(const DenseMatrix<T>& lhs, const U& rhs)
        -> DenseMatrix<R> {
    DenseMatrix<R> result{lhs};
    result /= rhs;
    return result;
}

/// \relates DenseMatrix
/// \brief Addition of two DenseMatrices.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
template <typename T>
inline auto operator+(DenseMatrix<T> lhs, const DenseMatrix<T>& rhs)
        -> DenseMatrix<T> {
    lhs += rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Addition of two DenseMatrices.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
template <typename T>
inline auto operator+(const DenseMatrix<T>& lhs, DenseMatrix<T>&& rhs)
        -> DenseMatrix<T> {
    rhs += lhs;
    return rhs;
}

/// \relates DenseMatrix
/// \brief Difference of two DenseMatrices.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
template <typename T>
inline auto operator-(DenseMatrix<T> lhs, const DenseMatrix<T>& rhs)
        -> DenseMatrix<T> {
    lhs -= rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Difference of two DenseMatrices.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
template <typename T>
inline auto operator-(const DenseMatrix<T>& lhs, DenseMatrix<T>&& rhs)
        -> DenseMatrix<T> {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates DenseMatrix
/// \brief Right-hand-side multiplication by vector.
template <typename T>
inline auto operator*(const DenseMatrix<T>& lhs, const Vector<T>& rhs)
        -> Vector<T> {
#ifndef NDEBUG
    if (lhs.num_columns() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = {rhs.size(), 1}}};
    }
#endif  // NDEBUG

    std::size_t N{lhs.num_rows()}, X{lhs.num_columns()};
    Vector<T> result(N);
    for (std::size_t i{0}; i < X; ++i) {
        for (std::size_t row{0}; row < N; ++row) {
            result[row] += lhs[row, i]*rhs[i];
        }
    }

    return result;
}

/// \relates DenseMatrix
/// \brief Left-hand multiplication by scalar.
template <typename T>
inline auto operator*(const T& lhs, DenseMatrix<T> rhs) -> DenseMatrix<T> {
    rhs *= lhs;
    return rhs;
}

/// \relates DenseMatrix
/// \brief Right-hand multiplication by scalar.
template <typename T>
inline auto operator*(DenseMatrix<T> lhs, const T& rhs) -> DenseMatrix<T> {
    lhs *= rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Multiplication of two DenseMatrices.
template <typename T>
inline auto operator*(DenseMatrix<T> lhs, const DenseMatrix<T>& rhs)
        -> DenseMatrix<T> {
    lhs *= rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Division by a scalar.
template <typename T>
inline auto operator/(DenseMatrix<T> lhs, const T& rhs) -> DenseMatrix<T> {
    lhs /= rhs;
    return lhs;
}

// ========================================================================
// CBLAS
// ========================================================================

#ifdef JUMP_HAS_CBLAS
/// \brief Specialisation of multiplication of two real DenseMatrices, using
/// CBLAS.
template <>
inline auto DenseMatrix<Real>::operator*=(const DenseMatrix<Real>& rhs)
        -> DenseMatrix& {
#ifndef NDEBUG
    if (this->num_columns() != rhs.num_rows()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // Computes 1.*lhs*rhs + 0.
    DenseMatrix<Real> result{this->num_rows(), rhs.num_columns()};
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, this->num_rows(),
            rhs.num_columns(), this->num_columns(), 1., m_storage.data(),
            this->num_rows(), rhs.m_storage.data(), rhs.num_rows(), 0.,
            result.m_storage.data(), this->num_rows());

    return *this = std::move(result);
}

/// \brief Specialisation of multiplication of two complex DenseMatrices, using
/// CBLAS.
template <>
inline auto DenseMatrix<Complex>::operator*=(const DenseMatrix<Complex>& rhs)
        -> DenseMatrix& {
#ifndef NDEBUG
    if (this->num_columns() != rhs.num_rows()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // Computes 1.*lhs*rhs + 0.
    DenseMatrix<Complex> result{this->num_rows(), rhs.num_columns()};
    Complex alpha{1.}, beta{0.};
    cblas_zgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, this->num_rows(),
            rhs.num_columns(), this->num_columns(), &alpha, m_storage.data(),
            this->num_rows(), rhs.m_storage.data(), rhs.num_rows(), &beta,
            result.m_storage.data(), this->num_rows());

    return *this = std::move(result);
}

/// \brief Specialisation of the column L1-norm calculation for a real
/// `DenseMatrix`, using CBLAS.
template <>
inline auto DenseMatrix<Real>::column_L1_norm(std::size_t column) const
        -> Real {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    // Computes sum of absolute element values in a column (with a pointer shift
    // of 1 between elements)
    return cblas_dasum(this->m_num_rows,
            &(*(m_storage.begin() + this->m_num_rows*column)), 1);
}

/// \brief Specialisation of the column L1-norm calculation for a complex
/// `DenseMatrix`, using CBLAS.
template <>
inline auto DenseMatrix<Complex>::column_L1_norm(std::size_t column) const
        -> Real {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    // Computes sum of absolute real and imaginary element values (with a
    // pointer shift of 1 between elements). Note: this is not the same as the
    // usual definition of this norm
    return cblas_dzasum(this->m_num_rows,
            &(*(m_storage.begin() + this->m_num_rows*column)), 1);
}

/// \brief Specialisation of the column L2-norm calculation for a real
/// `DenseMatrix`, using CBLAS.
template <>
inline auto DenseMatrix<Real>::column_L2_norm(std::size_t column) const
        -> Real {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    // Computes the Euclidean norm of the Vector (with a pointer shift of 1
    // between elements)
    return cblas_dnrm2(this->m_num_rows,
            &(*(m_storage.begin() + this->m_num_rows*column)), 1);
}

/// \brief Specialisation of the column L2-norm calculation for a complex
/// `DenseMatrix`, using CBLAS.
template <>
inline auto DenseMatrix<Complex>::column_L2_norm(std::size_t column) const
        -> Real {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    // Computes the Euclidean norm of the Vector (with a pointer shift of 1
    // between elements)
    return cblas_dznrm2(this->m_num_rows,
            &(*(m_storage.begin() + this->m_num_rows*column)), 1);
}

/// \relates DenseMatrix
/// \brief Specialisation of multiplication of a real `DenseMatrix` by a real
/// Vector, using CBLAS.
inline auto operator*(const DenseMatrix<Real>& lhs, const Vector<Real>& rhs)
        -> Vector<Real> {
#ifndef NDEBUG
    if (lhs.num_columns() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = {rhs.size(), 1}}};
    }
#endif  // NDEBUG

    // Computes 1.*lhs*rhs + 0. (with a pointer shift of 1 between elements)
    Vector<Real> result(lhs.num_rows());
    cblas_dgemv(CblasColMajor, CblasNoTrans, lhs.num_rows(), lhs.num_columns(),
            1., lhs.data(), lhs.num_rows(), rhs.data(), 1, 0., result.data(),
            1);
    return result;
}

/// \relates DenseMatrix
/// \brief Specialisation of multiplication of a complex `DenseMatrix` by a
/// complex Vector, using CBLAS.
inline auto operator*(const DenseMatrix<Complex>& lhs,
        const Vector<Complex>& rhs) -> Vector<Complex> {
#ifndef NDEBUG
    if (lhs.num_columns() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = {rhs.size(), 1}}};
    }
#endif  // NDEBUG

    // Computes 1.*lhs*rhs + 0. (with a pointer shift of 1 between elements)
    Vector<Complex> result(lhs.num_rows());
    Complex alpha{1.}, beta{0.};
    cblas_zgemv(CblasColMajor, CblasNoTrans, lhs.num_rows(), lhs.num_columns(),
            &alpha, lhs.data(), lhs.num_rows(), rhs.data(), 1, &beta,
            result.data(), 1);
    return result;
}
#endif  // JUMP_HAS_CBLAS
}   // namespace jump

#endif  // JUMP_DENSE_MATRIX_HPP

