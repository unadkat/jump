// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_DENSE_MATRIX_FRIENDS_HPP
#define JUMP_DENSE_MATRIX_FRIENDS_HPP

/// \file
/// Non-member arithmetic operators must be defined as in-class friends so that
/// the necessary functions are guaranteed to be instantiated and valid targets
/// for overload resolution in the case of mixed-type operands. Without this,
/// template non-member functions will fail argument deduction with mixed types
/// and be excluded from the list of candidates.

/// \relates DenseMatrix
/// \brief Addition of two DenseMatrices.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
friend DenseMatrix operator+(DenseMatrix lhs, const DenseMatrix& rhs) {
    lhs += rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Addition of two DenseMatrices.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
friend DenseMatrix operator+(const DenseMatrix& lhs, DenseMatrix&& rhs) {
    rhs += lhs;
    return rhs;
}

/// \relates DenseMatrix
/// \brief Difference of two DenseMatrices.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
friend DenseMatrix operator-(DenseMatrix lhs, const DenseMatrix& rhs) {
    lhs -= rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Difference of two DenseMatrices.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
friend DenseMatrix operator-(const DenseMatrix& lhs, DenseMatrix&& rhs) {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates DenseMatrix
/// \brief Right-hand-side multiplication by vector.
friend Vector<T> operator*(const DenseMatrix& lhs, const Vector<T>& rhs) {
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

/// \relates DenseMatrix
/// \brief Left-hand multiplication by scalar.
friend DenseMatrix operator*(const T& lhs, DenseMatrix rhs) {
    rhs *= lhs;
    return rhs;
}

/// \relates DenseMatrix
/// \brief Right-hand multiplication by scalar.
friend DenseMatrix operator*(DenseMatrix lhs, const T& rhs) {
    lhs *= rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Multiplication of two DenseMatrices.
friend DenseMatrix operator*(DenseMatrix lhs, const DenseMatrix& rhs) {
    lhs *= rhs;
    return lhs;
}

/// \relates DenseMatrix
/// \brief Division by a scalar.
friend DenseMatrix operator/(DenseMatrix lhs, const T& rhs) {
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
friend Vector<Real> operator*(const DenseMatrix<Real>& lhs,
        const Vector<Real>& rhs) {
#ifndef NDEBUG
    if (lhs.num_columns() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = {rhs.size(), 1}}};
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
friend Vector<Complex> operator*(const DenseMatrix<Complex>& lhs,
        const Vector<Complex>& rhs) {
#ifndef NDEBUG
    if (lhs.num_columns() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = {rhs.size(), 1}}};
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
#endif  // JUMP_HAS_CBLAS

#endif  // JUMP_DENSE_MATRIX_FRIENDS_HPP


