// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_BANDED_MATRIX_HPP
#define JUMP_BANDED_MATRIX_HPP

#include <format>
#include <sstream>
#include <string>

#include "jump/data/matrix_base.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/exception.hpp"

namespace Jump {

    /// \brief Banded square matrix for use with LAPACKE.
    template <typename T>
    class BandedMatrix : public MatrixBase<T> {
        private:
            /// \brief Maximum number of diagonals on either side of the
            /// leading diagonal that are permitted to contain non-zero
            /// elements.
            std::size_t m_num_bands = 0;
            /// \brief Internal contiguous storage.
            Vector<T> m_storage;

        public:
            /// \brief Construct a square matrix with the given number of
            /// diagonals on each side of the leading diagonal.
            BandedMatrix(std::size_t size = 0, std::size_t num_bands = 0);

            /// \brief Initialise matrix with the given size and number of
            /// off-leading diagonal diagonals.
            void assign(std::size_t size = 0, std::size_t num_bands = 0);
            /// \brief Zero the matrix.
            void zero() override;

            /// \brief Const element access.
            T operator[](std::size_t row, std::size_t column) const;
            /// \brief Mutable element access.
            T& operator[](std::size_t row, std::size_t column);

            /// \brief Return number of off-leading diagonal diagonals.
            std::size_t num_bands() const;
            /// \brief Return size of internal storage.
            std::size_t num_elements() const override;

            /// \brief Pointer to underlying data, for use with external
            /// libraries.
            T* data();

            /// \brief Populate with data from a `std::string`. Note that data
            /// storage is assumed to be column-major.
            void operator<<(std::string data) override;
            /// \brief Matrix serialisation to a string.
            std::string as_string() const;
    };

    #include "banded_matrix_impl.hpp"
}

#endif  // JUMP_BANDED_MATRIX_HPP

