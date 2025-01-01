// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_EIGENDATA_HPP
#define JUMP_EIGENDATA_HPP

#include "jump/data/vector.hpp"
#include "jump/utility/types.hpp"

#include <vector>

namespace jump {
/// \brief Simple struct to hold a complex eigenvalue and corresponding complex
/// eigenvector.
template <typename T>
struct Eigendatum {
    /// \brief Eigenvalue.
    T value{};
    /// \brief Eigenvector corresponding to the stored eigenvalue.
    Vector<T> vector;
};

/// \brief Combine separate eigenvalue/eigenvector data from external solvers
/// into a single vector.
template <typename T>
auto combine_eigendata(const std::vector<T>& eigenvalues,
        const std::vector<Vector<T>>& eigenvectors)
        -> std::vector<Eigendatum<T>>;

/// \brief Predicate for sorting eigendata by real part of associated
/// eigenvalue.
auto sort_eigendata_real(const Complex& lhs, const Complex& rhs)
        -> bool;
/// \brief Predicate for sorting eigendata by imaginary part of associated
/// eigenvalue.
auto sort_eigendata_imag(const Complex& lhs, const Complex& rhs)
        -> bool;
}   // namespace jump

#endif  // JUMP_EIGENDATA_HPP

