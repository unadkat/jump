// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_DENSE_MATRIX_HPP
#define JUMP_DENSE_MATRIX_HPP

#include <algorithm>
#include <ranges>
#include <sstream>
#include <string>

#include "jump/data/matrix_base.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/types.hpp"

namespace Jump {

    /// \brief Stores all elements of a general \f$m\f$ by \f$n\f$ matrix, with
    /// common arithmetic operations enabled.
    template <typename T>
    class DenseMatrix : public MatrixBase<T>
    {
        protected:
            /// \brief Internal contiguous storage.
            Vector<T> m_storage;

        public:
            /// \brief Iterator for algorithms.
            using Iterator = typename Vector<T>::Iterator;
            /// \brief Iterator for algorithms.
            using ConstIterator = typename Vector<T>::ConstIterator;

            /// \brief Construct square matrix with the given size.
            DenseMatrix(std::size_t size = 0);
            /// \brief Construct a general matrix with the given size.
            DenseMatrix(std::size_t num_rows, std::size_t num_columns);

//            /// \brief Conversion operator to promote a real-valued
//            /// `DenseMatrix` to a complex-valued one.
//            operator DenseMatrix<Complex>() const;

            /// \brief Const element access.
            const T& operator[](std::size_t row, std::size_t column) const;
            /// \brief Mutable element access.
            T& operator[](std::size_t row, std::size_t column);

            /// \brief Initialise a square matrix with the given size.
            void assign(std::size_t size);
            /// \brief Initialise a general matrix with the given size.
            void assign(std::size_t num_rows, std::size_t num_columns);
            /// \brief Set data via a pair of iterators.
            template <class InputIt>
            void assign(InputIt first, InputIt last);
            /// \brief Return size of internal storage.
            std::size_t num_elements() const override;

            /// \brief Const iterator for algorithms.
            ConstIterator begin() const;
            /// \brief Const iterator for algorithms.
            ConstIterator end() const;
            /// \brief Iterator for algorithms.
            Iterator begin();
            /// \brief Iterator for algorithms.
            Iterator end();
            /// \brief Return iterators to extract/manipulate a column.
            std::pair<Iterator, Iterator> column_iterators(std::size_t column);
            /// \brief Return iterators to extract/manipulate a column.
            std::pair<ConstIterator, ConstIterator> column_iterators(
                    std::size_t column) const;

            /// \brief Fill matrix with given value.
            void fill(const T& value);
            /// \brief Zero the matrix.
            void zero() override;

            /// \brief No operation on matrix.
            const DenseMatrix& operator+() const;
            /// \brief Negate matrix.
            DenseMatrix operator-() const;
            /// \brief Add two matrices together in place.
            DenseMatrix& operator+=(const DenseMatrix& rhs);
            /// \brief Subtract a matrix from another in place.
            DenseMatrix& operator-=(const DenseMatrix& rhs);
            /// \brief Multiply matrix by scalar in place.
            DenseMatrix& operator*=(const T& k);
            /// \brief Multiply matrix by another in place
            DenseMatrix& operator*=(const DenseMatrix& rhs);
            /// \brief Divide matrix by scalar in place.
            DenseMatrix& operator/=(const T& k);

            /// \brief Return sum of element magnitudes in a column.
            Real column_L1_norm(std::size_t column) const;
            /// \brief Return Euclidean norm of a column.
            Real column_L2_norm(std::size_t column) const;
            /// \brief Return maximum magnitude over all elements in a column.
            Real column_Linf_norm(std::size_t column) const;

            /// \brief Pointer to underlying data, for use with external
            /// libraries.
            T* data();
            /// \brief Pointer to underlying data, for use with external
            /// libraries.
            const T* data() const;

            /// \brief Populate with data from a `std::string`. Noting that data
            /// storage is assumed to be column-major, matrices stored as
            /// strings will be assumed to be a transpose matrix.
            void operator<<(std::string data) override;
            /// \brief Matrix serialisation to a string.
            std::string as_string() const override;

            /// \brief Return identity matrix of specified size.
            static DenseMatrix identity(std::size_t size);
    };

    #include "dense_matrix_impl.hpp"

}   // namespace Jump

#endif  // JUMP_DENSE_MATRIX_HPP

