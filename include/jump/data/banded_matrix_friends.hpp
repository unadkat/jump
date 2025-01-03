// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_BANDED_MATRIX_FRIENDS_HPP
#define JUMP_BANDED_MATRIX_FRIENDS_HPP

/// \file
/// Non-member arithmetic operators must be defined as in-class friends so that
/// the necessary functions are guaranteed to be instantiated and valid targets
/// for overload resolution in the case of mixed-type operands. Without this,
/// template non-member functions will fail argument deduction with mixed types
/// and be excluded from the list of candidates.

/// \relates BandedMatrix
/// \brief Addition of two BandedMatrices.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
friend auto operator+(BandedMatrix lhs, const BandedMatrix& rhs)
        -> BandedMatrix {
    lhs += rhs;
    return lhs;
}

/// \relates BandedMatrix
/// \brief Addition of two BandedMatrices.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
friend auto operator+(const BandedMatrix& lhs, BandedMatrix&& rhs)
        -> BandedMatrix {
    rhs += lhs;
    return rhs;
}

/// \relates BandedMatrix
/// \brief Difference of two BandedMatrices.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
friend auto operator-(BandedMatrix lhs, const BandedMatrix& rhs)
        -> BandedMatrix {
    lhs -= rhs;
    return lhs;
}

/// \relates BandedMatrix
/// \brief Difference of two BandedMatrices.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
friend auto operator-(const BandedMatrix& lhs, BandedMatrix&& rhs)
        -> BandedMatrix {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates BandedMatrix
/// \brief Right-hand-side multiplication by vector.
friend auto operator*(const BandedMatrix& lhs, const Vector<T>& rhs)
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

/// \relates BandedMatrix
/// \brief Left-hand multiplication by scalar.
friend auto operator*(const T& lhs, BandedMatrix rhs) -> BandedMatrix {
    rhs *= lhs;
    return rhs;
}

/// \relates BandedMatrix
/// \brief Right-hand multiplication by scalar.
friend auto operator*(BandedMatrix lhs, const T& rhs) -> BandedMatrix {
    lhs *= rhs;
    return lhs;
}

/// \relates BandedMatrix
/// \brief Division by a scalar.
friend auto operator/(BandedMatrix lhs, const T& rhs) -> BandedMatrix {
    lhs /= rhs;
    return lhs;
}

#endif  // JUMP_BANDED_MATRIX_FRIENDS_HPP

