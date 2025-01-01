// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

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
friend auto operator+(Vector lhs, const Vector& rhs) -> Vector {
    lhs += rhs;
    return lhs;
}

/// \relates Vector
/// \brief Addition of two Vectors.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
friend auto operator+(const Vector& lhs, Vector&& rhs) -> Vector {
    rhs += lhs;
    return rhs;
}

/// \relates Vector
/// \brief Difference of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
friend auto operator-(Vector lhs, const Vector& rhs) -> Vector {
    lhs -= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Difference of two Vectors.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
friend auto operator-(const Vector& lhs, Vector&& rhs) -> Vector {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates Vector
/// \brief Left-hand multiplication by scalar.
friend auto operator*(const T& lhs, Vector rhs) -> Vector {
    rhs *= lhs;
    return rhs;
}

/// \relates Vector
/// \brief Right-hand multiplication by scalar.
friend auto operator*(Vector lhs, const T& rhs) -> Vector {
    lhs *= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Elementwise product of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
friend auto operator*(Vector lhs, const Vector& rhs) -> Vector {
    lhs *= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Elementwise product of two Vectors.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
friend auto operator*(const Vector& lhs, Vector&& rhs) -> Vector {
    rhs *= lhs;
    return rhs;
}

/// \relates Vector
/// \brief Inner product of two Vectors.
friend auto dot(const Vector& lhs, const Vector& rhs) -> T {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), T{0});
}

/// \relates Vector
/// \brief Right-hand division by scalar.
friend auto operator/(Vector lhs, const T& rhs) -> Vector {
    lhs /= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Elementwise division of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
friend auto operator/(Vector lhs, const Vector& rhs) -> Vector {
    lhs /= rhs;
    return lhs;
}

// ========================================================================
// CBLAS
// ========================================================================

#ifdef JUMP_HAS_CBLAS
/// \brief Specialisation of the dot product for two real Vectors, using CBLAS.
friend auto operator*(const Vector<Real>& lhs, const Vector<Real>& rhs)
        -> Real {
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
friend auto operator*(const Vector<Complex>& lhs, const Vector<Complex>& rhs)
        -> Complex {
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

