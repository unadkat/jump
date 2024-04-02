// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_EIGENDATA_DECL_HPP
#define JUMP_EIGENDATA_DECL_HPP

#include <vector>

#include "jump/data/vector.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/types.hpp"

namespace jump {
/// \brief Simple struct to hold a complex eigenvalue and corresponding complex
/// eigenvector.
template <typename T>
struct Eigendatum {
    /// \brief Eigenvalue.
    T value;
    /// \brief Eigenvector corresponding to the stored eigenvalue.
    Vector<T> vector;
};

/// \brief Combine separate eigenvalue/eigenvector data from external solvers
/// into a single vector.
template <typename T>
std::vector<Eigendatum<T>> combine_eigendata(const std::vector<T>& eigenvalues,
        const std::vector<Vector<T>>& eigenvectors);

/// \brief Predicate for sorting eigendata by real part of associated
/// eigenvalue.
bool sort_complex_eigendata_real(const Complex& lhs, const Complex& rhs);
/// \brief Predicate for sorting eigendata by imaginary part of associated
/// eigenvalue.
bool sort_complex_eigendata_imag(const Complex& lhs, const Complex& rhs);
}   // namespace jump

#endif  // JUMP_EIGENDATA_DECL_HPP

