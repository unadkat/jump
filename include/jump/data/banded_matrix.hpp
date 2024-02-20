// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_BANDED_MATRIX_HPP
#define JUMP_BANDED_MATRIX_HPP

#include "jump/data/banded_matrix_decl.hpp"

namespace Jump {
/// \var BandedMatrix::m_storage
/// In short, the interal storage takes the form of a dense matrix with
/// `num_columns()` columns and `1 + 3*#num_bands()` rows. Refer to the
/// <a href="http://www.netlib.org/lapack/lug/node124.html">LAPACK User
/// Guide</a> for further details about the band storage scheme.

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
inline std::size_t BandedMatrix<T>::num_bands() const {
    return m_num_bands;
}

template <typename T>
inline std::size_t BandedMatrix<T>::num_elements() const {
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
inline T BandedMatrix<T>::operator[](std::size_t row,
        std::size_t column) const {
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
inline T& BandedMatrix<T>::operator[](std::size_t row, std::size_t column) {
#ifndef NDEBUG
    if (row >= this->num_rows() || column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = this->size()}};
    }
    if (row > column + num_bands() || column > row + 2*num_bands()) {
        throw RuntimeError{InvalidArgumentError{.argument = "(row, column)",
            .value = std::format("({}, {}) with num_bands() = {}", row, column,
                    num_bands()),
            .expected = "row <= column + num_bands() && "
                "column <= row + 2*num_bands()"}};
    }
#endif  // NDEBUG

    return m_storage[num_bands()*(3*column + 2) + row];
}

template <typename T>
inline void BandedMatrix<T>::zero() {
    m_storage.zero();
}

template <typename T>
inline T* BandedMatrix<T>::data() {
    return m_storage.data();
}

/// Loads a block of data from the given string by reading all valid values into
/// the underlying storage. The only thing that will be checked is that the
/// total number of values read is equal to `num_elements()`. Note that this
/// means there is no restriction on the formatting of the data, it may appear
/// as a series of equal-length lines (each line corresponding to a column), a
/// single line, or even lines of unequal length.
/// TODO: add metadata to load arbitrary matrices
template <typename T>
void BandedMatrix<T>::operator<<(std::string data) {
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
inline std::string BandedMatrix<T>::as_string() const {
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
}   // namespace Jump

#endif  // JUMP_BANDED_MATRIX_HPP

