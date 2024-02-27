// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_VECTOR_FRIENDS_HPP
#define JUMP_VECTOR_FRIENDS_HPP

/// \file
/// Non-member arithmetic operators must be defined as in-class friends so that
/// the necessary functions are guaranteed to be instantiated and valid targets
/// for overload resolution in the case of mixed-type operands. Without this,
/// template non-member functions will fail argument deduction with mixed types
/// and be excluded from the list of candidates.

/// \relates Vector
/// \brief Addition of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
friend Vector operator+(Vector lhs, const Vector& rhs) {
    lhs += rhs;
    return lhs;
}

/// \relates Vector
/// \brief Addition of two Vectors.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
friend Vector operator+(const Vector& lhs, Vector&& rhs) {
    rhs += lhs;
    return rhs;
}

/// \relates Vector
/// \brief Difference of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
friend Vector operator-(Vector lhs, const Vector& rhs) {
    lhs -= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Difference of two Vectors.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
friend Vector operator-(const Vector& lhs, Vector&& rhs) {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates Vector
/// \brief Left-hand multiplication by scalar.
friend Vector operator*(const T& lhs, Vector rhs) {
    rhs *= lhs;
    return rhs;
}

/// \relates Vector
/// \brief Right-hand multiplication by scalar.
friend Vector operator*(Vector lhs, const T& rhs) {
    lhs *= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Inner product of two Vectors.
friend T operator*(const Vector& lhs, const Vector& rhs) {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    T dot_product{0};
    for (std::size_t i{0}, N{lhs.size()}; i < N; ++i) {
        dot_product += lhs[i]*rhs[i];
    }
    return dot_product;
}

/// \relates Vector
/// \brief Right-hand division by scalar.
friend Vector operator/(Vector lhs, const T& rhs) {
    lhs /= rhs;
    return lhs;
}

// ========================================================================
// CBLAS
// ========================================================================

#ifdef JUMP_HAS_CBLAS
/// \brief Specialisation of the dot product for two real Vectors, using CBLAS.
friend Real operator*(const Vector<Real>& lhs, const Vector<Real>& rhs) {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // Computes dot product lhs*rhs (with a pointer shift of 1 between elements)
    return cblas_ddot(lhs.size(), lhs.data(), 1, rhs.data(), 1);
}

/// \brief Specialisation of the dot product for two complex Vectors, using
/// CBLAS.
friend Complex operator*(const Vector<Complex>& lhs,
        const Vector<Complex>& rhs) {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // Computes dot product lhs*rhs (with a pointer shift of 1 between elements)
    Complex result;
    cblas_zdotu_sub(lhs.size(), lhs.data(), 1, rhs.data(), 1, &result);
    return result;
}
#endif  // JUMP_HAS_CBLAS

#endif  // JUMP_VECTOR_FRIENDS_HPP

