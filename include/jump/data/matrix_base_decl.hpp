// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_MATRIX_BASE_DECL_HPP
#define JUMP_MATRIX_BASE_DECL_HPP

#include <ostream>
#include <string>
#include <utility>

#include "jump/utility/external.hpp"

namespace jump {
/// \brief General matrix interface.
template <typename T>
class MatrixBase {
    protected:
        /// \brief Number of matrix rows.
        std::size_t m_num_rows{0};
        /// \brief Number of matrix columns.
        std::size_t m_num_columns{0};

        /// \brief Set the number of rows and columns to be equal.
        void initialise(std::size_t size = 0);
        /// \brief Set the number of rows and columns separately.
        void initialise(std::size_t num_rows, std::size_t num_columns);

    public:
        /// \brief Construct a square matrix with the given size.
        MatrixBase(std::size_t size = 0);
        /// \brief Construct a general matrix with the given size.
        MatrixBase(std::size_t num_rows, std::size_t num_columns);

        /// \brief Return the number of rows in the matrix.
        std::size_t num_rows() const;
        /// \brief Return the number of columns in the matrix.
        std::size_t num_columns() const;
        /// \brief Interface for returning the number of elements actually
        /// stored by the matrix.
        virtual std::size_t num_elements() const = 0;
        /// \brief Return the size of the matrix as a pair (rows, columns).
        std::pair<std::size_t, std::size_t> size() const;

        /// \brief Interface for zeroing the matrix.
        virtual void zero() = 0;

        /// \brief Populate with data from a `std::string`. Noting that data
        /// storage is assumed to be column-major, matrices stored as strings
        /// will appear as transposes.
        virtual void operator<<(std::string data) = 0;
        /// \brief Interface for matrix serialisation to a `std::string`.
        virtual std::string as_string() const = 0;
};
}   // namespace jump

#endif  // JUMP_MATRIX_BASE_DECL_HPP

