// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_DENSE_MATRIX_HPP
#define JUMP_DENSE_MATRIX_HPP

#include "jump/data/dense_matrix_decl.hpp"

namespace Jump {
/// \class DenseMatrix
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

/// \var DenseMatrix::m_storage
/// The internal storage for a dense matrix consists of a contiguous `Vector`
/// containing `num_rows()*#num_columns()` elements. The matrix elements are
/// stored in column-major format, so that the element at row `i` and column `j`
/// appears at location `m_storage[j*num_rows() + i]`.

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

/// Initialises a complex-valued `DenseMatrix` of the correct size and delegates
/// conversion of elements to the underlying `Vector`.
template <>
inline DenseMatrix<Real>::operator DenseMatrix<Complex>() const {
    DenseMatrix<Complex> result;
    result.assign(num_rows(), num_columns(), {m_storage});
    return result;
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
template <class InputIt>
inline void DenseMatrix<T>::assign(InputIt first, InputIt last) {
#ifndef NDEBUG
    if (last != first + m_storage.size()) {
        throw RuntimeError{InvalidArgumentError{.argument = "last",
            .value = std::format("first + {}", last - first),
            .expected = "first - last == m_storage.size()"}};
    }
#endif  // NDEBUG

    m_storage.assign(std::move(first), std::move(last));
}

template <typename T>
inline std::size_t DenseMatrix<T>::num_elements() const {
    return m_storage.size();
}

/// The element at row `i` and column `j` appears at location
/// `m_storage[j*num_rows() + i]` (see `DenseMatrix::m_storage`).
template <typename T>
inline const T& DenseMatrix<T>::operator[](std::size_t row,
        std::size_t column) const {
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
inline T& DenseMatrix<T>::operator[](std::size_t row, std::size_t column) {
#ifndef NDEBUG
    if (row >= this->num_rows() || column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    return m_storage[column*this->num_rows() + row];
}

template <typename T>
inline typename DenseMatrix<T>::ConstIterator DenseMatrix<T>::begin() const {
    return m_storage.begin();
}

template <typename T>
inline typename DenseMatrix<T>::ConstIterator DenseMatrix<T>::end() const {
    return m_storage.end();
}

template <typename T>
inline typename DenseMatrix<T>::Iterator DenseMatrix<T>::begin() {
    return m_storage.begin();
}

template <typename T>
inline typename DenseMatrix<T>::Iterator DenseMatrix<T>::end() {
    return m_storage.end();
}

/// Keeping in mind that the data is stored internally in column-major format,
/// the beginning of column `j` is at index `num_rows()*j` while the end of the
/// column is at index `num_rows()*(j + 1)`.
template <typename T>
inline std::pair<typename DenseMatrix<T>::Iterator,
       typename DenseMatrix<T>::Iterator>
       DenseMatrix<T>::column_iterators(std::size_t column) {
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
inline std::pair<typename DenseMatrix<T>::ConstIterator,
       typename DenseMatrix<T>::ConstIterator>
       DenseMatrix<T>::column_iterators(std::size_t column) const {
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
    fill(T{0});
}

template <typename T>
inline const DenseMatrix<T>& DenseMatrix<T>::operator+() const {
    return *this;
}

template <typename T>
inline DenseMatrix<T> DenseMatrix<T>::operator-() const {
    DenseMatrix<T> temp{*this};
    temp *= -1;
    return temp;
}

template <typename T>
inline DenseMatrix<T>& DenseMatrix<T>::operator+=(const DenseMatrix& rhs) {
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
inline DenseMatrix<T>& DenseMatrix<T>::operator-=(const DenseMatrix& rhs) {
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
inline DenseMatrix<T>& DenseMatrix<T>::operator*=(const T& k) {
    m_storage *= k;
    return *this;
}

template <typename T>
inline DenseMatrix<T>& DenseMatrix<T>::operator*=(const DenseMatrix<T>& rhs) {
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
inline DenseMatrix<T>& DenseMatrix<T>::operator/=(const T& k) {
    m_storage /= k;
    return *this;
}

template <typename T>
inline Real DenseMatrix<T>::column_L1_norm(std::size_t column) const {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    auto it{column_iterators(column)};
    auto F{[](Real acc, const T& x) { return acc + std::abs(x); }};
    return std::ranges::fold_left(it.first, it.second, Real{0}, F);
}

template <typename T>
inline Real DenseMatrix<T>::column_L2_norm(std::size_t column) const {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    auto it{column_iterators(column)};
    auto F{[](Real acc, const T& x) {
        return acc + std::pow(std::abs(x), 2.); }};
    return std::sqrt(std::ranges::fold_left(it.first, it.second, Real{0}, F));
}

template <typename T>
inline Real DenseMatrix<T>::column_Linf_norm(std::size_t column) const {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    auto it{column_iterators(column)};
    auto F{[](const T& x) { return std::abs(x); }};
    return std::ranges::max(it.first, it.second, {}, F);
}

template <typename T>
inline T* DenseMatrix<T>::data() {
    return m_storage.data();
}

template <typename T>
inline const T* DenseMatrix<T>::data() const {
    return m_storage.data();
}

template <typename T>
inline const Vector<T>& DenseMatrix<T>::as_vector() const {
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
inline std::string DenseMatrix<T>::as_string() const {
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
inline DenseMatrix<T> DenseMatrix<T>::identity(std::size_t size) {
    DenseMatrix<T> I{size};
    for (std::size_t i{0}; i < size; ++i)
        I[i, i] = T{1};
    return I;
}

// ========================================================================
// Non-member functions
// ========================================================================

/// \relates DenseMatrix
/// \brief Addition of two DenseMatrices.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
template <typename T>
inline DenseMatrix<T> operator+(DenseMatrix<T> lhs, const DenseMatrix<T>& rhs) {
    lhs += rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Addition of two DenseMatrices.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
template <typename T>
inline DenseMatrix<T> operator+(const DenseMatrix<T>& lhs,
        DenseMatrix<T>&& rhs) {
    rhs += lhs;
    return rhs;
}

/// \relates DenseMatrix
/// \brief Difference of two DenseMatrices.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
template <typename T>
inline DenseMatrix<T> operator-(DenseMatrix<T> lhs, const DenseMatrix<T>& rhs) {
    lhs -= rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Difference of two DenseMatrices.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
template <typename T>
inline DenseMatrix<T> operator-(const DenseMatrix<T>& lhs,
        DenseMatrix<T>&& rhs) {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates DenseMatrix
/// \brief Right-hand-side multiplication by vector.
template <typename T>
inline Vector<T> operator*(const DenseMatrix<T>& lhs, const Vector<T>& rhs) {
#ifndef NDEBUG
    if (lhs.num_columns() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = lhs.size(), .name2 = "rhs",
            .size2 = {rhs.size(), 0}}};
    }
#endif  // NDEBUG

    std::size_t N{rhs.size()}, X{lhs.num_columns()};
    Vector<T> result(N);
    for (std::size_t row{0}; row < N; ++row) {
        for (std::size_t i{0}; i < X; ++i) {
            result[row] += lhs[row, i]*rhs[i];
        }
    }

    return result;
}

/// \relates DenseMatrix
/// \brief Left-hand multiplication by scalar.
template <typename T>
inline DenseMatrix<T> operator*(const T& lhs, DenseMatrix<T> rhs) {
    rhs *= lhs;
    return rhs;
}

/// \relates DenseMatrix
/// \brief Right-hand multiplication by scalar.
template <typename T>
inline DenseMatrix<T> operator*(DenseMatrix<T> lhs, const T& rhs) {
    lhs *= rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Multiplication of two DenseMatrices.
template <typename T>
inline DenseMatrix<T> operator*(DenseMatrix<T> lhs, const DenseMatrix<T>& rhs) {
    lhs *= rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Division by a scalar.
template <typename T>
DenseMatrix<T> operator/(DenseMatrix<T> lhs, const T& rhs) {
    lhs /= rhs;
    return lhs;
}

// ========================================================================
// CBLAS
// ========================================================================

#ifdef JUMP_HAS_CBLAS
/// \relates DenseMatrix
/// \brief Specialisation of multiplication of a real `DenseMatrix` by a real
/// Vector, using CBLAS.
inline Vector<Real> operator*(const DenseMatrix<Real>& lhs,
        const Vector<Real>& rhs) {
#ifndef NDEBUG
    if (lhs.num_columns() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = lhs.size(), .name2 = "rhs",
            .size2 = {rhs.size(), 0}}};
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
inline Vector<Complex> operator*(const DenseMatrix<Complex>& lhs,
        const Vector<Complex>& rhs) {
#ifndef NDEBUG
    if (lhs.num_columns() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = lhs.size(), .name2 = "rhs",
            .size2 = {rhs.size(), 0}}};
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

/// \brief Specialisation of multiplication of two real DenseMatrices, using
/// CBLAS.
template <>
inline DenseMatrix<Real>& DenseMatrix<Real>::operator*=(
        const DenseMatrix<Real>& rhs) {
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
inline DenseMatrix<Complex>& DenseMatrix<Complex>::operator*=(
        const DenseMatrix<Complex>& rhs) {
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
inline double DenseMatrix<Real>::column_L1_norm(std::size_t column) const {
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
inline double DenseMatrix<Complex>::column_L1_norm(std::size_t column) const {
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
inline double DenseMatrix<Real>::column_L2_norm(std::size_t column) const {
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
inline double DenseMatrix<Complex>::column_L2_norm(std::size_t column) const {
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
#endif  // JUMP_HAS_CBLAS
}   // namespace Jump

#endif  // JUMP_DENSE_MATRIX_HPP

