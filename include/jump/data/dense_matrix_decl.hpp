// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_DENSE_MATRIX_DECL_HPP
#define JUMP_DENSE_MATRIX_DECL_HPP

#include "jump/data/matrix_base.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/external.hpp"
#include "jump/utility/types.hpp"

#include <string>
#include <utility>

namespace jump {
/// \brief Stores all elements of a general \f$m\f$ by \f$n\f$ matrix, with
/// common arithmetic operations enabled.
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

        /// \brief Conversion operator to promote a real-valued `DenseMatrix`
        /// to a complex-valued one.
        operator DenseMatrix<Complex>() const;

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

        /// \brief Pointer to underlying data, for use with external libraries.
        auto data() -> T*;
        /// \brief Pointer to underlying data, for use with external libraries.
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

        #include "jump/data/dense_matrix_friends.hpp"

    private:
        /// \brief Internal contiguous storage.
        Vector<T> m_storage;
};
}   // namespace jump

#endif  // JUMP_DENSE_MATRIX_DECL_HPP

