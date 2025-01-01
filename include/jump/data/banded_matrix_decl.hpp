// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_BANDED_MATRIX_DECL_HPP
#define JUMP_BANDED_MATRIX_DECL_HPP

#include "jump/data/matrix_base.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/types.hpp"

#include <string>

namespace jump {
/// \brief Banded square matrix for use with LAPACKE.
template <typename T>
class BandedMatrix : public MatrixBase<T> {
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

        /// \brief No operation on matrix.
        auto operator+() const -> const BandedMatrix&;
        /// \brief Negate matrix.
        auto operator-() const -> BandedMatrix;
        /// \brief Add two matrices together in place.
        auto operator+=(const BandedMatrix& rhs) -> BandedMatrix&;
        /// \brief Subtract a matrix from another in place.
        auto operator-=(const BandedMatrix& rhs) -> BandedMatrix&;
        /// \brief Multiply matrix by scalar in place.
        auto operator*=(const T& k) -> BandedMatrix&;
        /// \brief Divide matrix by scalar in place.
        auto operator/=(const T& k) -> BandedMatrix&;

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

        #include "jump/data/banded_matrix_friends.hpp"

    private:
        /// \brief Maximum number of diagonals on either side of the leading
        /// diagonal that are permitted to contain non-zero elements.
        std::size_t m_num_bands{0};
        /// \brief Internal contiguous storage.
        Vector<T> m_storage;
};
}   // namespace jump

#endif  // JUMP_BANDED_MATRIX_HPP

