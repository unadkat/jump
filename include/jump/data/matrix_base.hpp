// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_MATRIX_BASE_HPP
#define JUMP_MATRIX_BASE_HPP

#include <ostream>
#include <string>
#include <utility>

namespace jump {
/// \brief General matrix interface.
template <typename T>
class MatrixBase {
    public:
        /// \brief Construct a square matrix with the given size.
        MatrixBase(std::size_t size = 0);
        /// \brief Construct a general matrix with the given size.
        MatrixBase(std::size_t num_rows, std::size_t num_columns);
        /// \brief Construct a general matrix with the given size.
        MatrixBase(const std::pair<std::size_t, std::size_t>& size);
        /// \brief Virtual destructor for base class.
        virtual ~MatrixBase() = default;

        /// \brief Return the number of rows in the matrix.
        auto num_rows() const -> std::size_t;
        /// \brief Return the number of columns in the matrix.
        auto num_columns() const -> std::size_t;
        /// \brief Interface for returning the number of elements actually
        /// stored by the matrix.
        virtual auto num_elements() const -> std::size_t = 0;
        /// \brief Return the size of the matrix as a pair (rows, columns).
        auto size() const -> const std::pair<std::size_t, std::size_t>&;

        /// \brief Interface for zeroing the matrix.
        virtual void zero() = 0;

        /// \brief Populate with data from a `std::string`. Noting that data
        /// storage is assumed to be column-major, matrices stored as strings
        /// will appear as transposes.
        virtual void operator<<(std::string data) = 0;
        /// \brief Interface for matrix serialisation to a `std::string`.
        virtual auto as_string() const -> std::string = 0;

    protected:
        /// \brief Set the number of rows and columns to be equal.
        void initialise(std::size_t size = 0);
        /// \brief Set the number of rows and columns separately.
        void initialise(std::size_t num_rows, std::size_t num_columns);

    protected:
        /// \brief Matrix size (rows, columns).
        std::pair<std::size_t, std::size_t> m_size{};
};

/// \relates MatrixBase
/// \brief Send internal representation of the matrix (as a `std::string`) to
/// the output stream.
template <typename T>
inline auto operator<<(std::ostream& out, const MatrixBase<T>& rhs)
        -> std::ostream&;

// ========================================================================
// Implementation
// ========================================================================

template <typename T>
inline MatrixBase<T>::MatrixBase(std::size_t size) :
    m_size{size, size} {
}

template <typename T>
inline MatrixBase<T>::MatrixBase(std::size_t num_rows,
        std::size_t num_columns) :
    m_size{num_rows, num_columns} {
}

template <typename T>
inline MatrixBase<T>::MatrixBase(
        const std::pair<std::size_t, std::size_t>& size) :
    m_size{size} {
}

template <typename T>
inline auto MatrixBase<T>::num_rows() const -> std::size_t {
    return m_size.first;
}

template <typename T>
inline auto MatrixBase<T>::num_columns() const -> std::size_t {
    return m_size.second;
}

template <typename T>
inline auto MatrixBase<T>::size() const
        -> const std::pair<std::size_t, std::size_t>& {
    return m_size;
}

template <typename T>
inline void MatrixBase<T>::initialise(std::size_t size) {
    m_size.first = m_size.second = size;
}

template <typename T>
inline void MatrixBase<T>::initialise(std::size_t num_rows,
        std::size_t num_columns) {
    m_size.first = num_rows;
    m_size.second = num_columns;
}

/// \relates MatrixBase
template <typename T>
inline auto operator<<(std::ostream& out, const MatrixBase<T>& rhs)
        -> std::ostream& {
    return out << rhs.as_string();
}
}   // namespace jump

#endif  // JUMP_MATRIX_BASE_HPP

