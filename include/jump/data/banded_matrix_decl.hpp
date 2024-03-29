// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_BANDED_MATRIX_DECL_HPP
#define JUMP_BANDED_MATRIX_DECL_HPP

#include <format>
#include <sstream>
#include <string>

#include "jump/data/matrix_base.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/exception.hpp"

namespace jump {
/// \brief Banded square matrix for use with LAPACKE.
template <typename T>
class BandedMatrix : public MatrixBase<T> {
    private:
        /// \brief Maximum number of diagonals on either side of the leading
        /// diagonal that are permitted to contain non-zero elements.
        std::size_t m_num_bands = 0;
        /// \brief Internal contiguous storage.
        Vector<T> m_storage;

    public:
        /// \brief Iterator for algorithms.
        using Iterator = typename Vector<T>::Iterator;
        /// \brief Iterator for algorithms.
        using ConstIterator = typename Vector<T>::ConstIterator;

        /// \brief Construct a square matrix with the given number of diagonals
        /// on each side of the leading diagonal.
        explicit BandedMatrix(std::size_t size = 0, std::size_t num_bands = 0);
        /// \brief Constuct a matrix with the given Vector data, specifying a
        /// consistent size.
        BandedMatrix(std::size_t size, std::size_t num_bands,
                Vector<T> underlying_data);

        /// \brief Conversion operator to promote a real-valued `BandedMatrix`
        /// to a complex-valued one.
        operator BandedMatrix<Complex>() const;

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
        std::size_t num_bands() const;
        /// \brief Return size of internal storage.
        std::size_t num_elements() const override;

        /// \brief Defaulted spaceship operator.
        auto operator<=>(const BandedMatrix&) const = default;

        /// \brief Const element access, always returns temporary value.
        T operator[](std::size_t row, std::size_t column) const;
        /// \brief Mutable element access.
        T& get_unsafe(std::size_t row, std::size_t column);
        /// \brief Element setter, returns flag for success.
        [[nodiscard]] bool set(std::size_t row, std::size_t column,
                const T& value);

        /// \brief Const iterator for algorithms.
        ConstIterator begin() const;
        /// \brief Const iterator for algorithms.
        ConstIterator end() const;
        /// \brief Iterator for algorithms.
        Iterator begin();
        /// \brief Iterator for algorithms.
        Iterator end();

        /// \brief Fill matrix with given value.
        void fill(const T& value);
        /// \brief Zero the matrix.
        void zero() override;

        /// \brief No operation on matrix.
        const BandedMatrix& operator+() const;
        /// \brief Negate matrix.
        BandedMatrix operator-() const;
        /// \brief Add two matrices together in place.
        BandedMatrix& operator+=(const BandedMatrix& rhs);
        /// \brief Subtract a matrix from another in place.
        BandedMatrix& operator-=(const BandedMatrix& rhs);
        /// \brief Multiply matrix by scalar in place.
        BandedMatrix& operator*=(const T& k);
        /// \brief Divide matrix by scalar in place.
        BandedMatrix& operator/=(const T& k);

        /// \brief Pointer to underlying data, for use with external libraries.
        T* data();
        /// \brief Pointer to underlying data, for use with external libraries.
        const T* data() const;
        /// \brief Const reference to underlying Vector (column-major).
        const Vector<T>& as_vector() const;

        /// \brief Populate with data from a `std::string`. Noting that data
        /// storage is assumed to be column-major, matrices stored as strings
        /// will be assumed to be a transpose matrix.
        void operator<<(std::string data) override;
        /// \brief Matrix serialisation to a string.
        std::string as_string() const override;

        #include "jump/data/banded_matrix_friends.hpp"
};
}   // namespace jump

#endif  // JUMP_BANDED_MATRIX_HPP

