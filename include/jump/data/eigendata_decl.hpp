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
struct Eigendatum {
    /// \brief Complex eigenvalue.
    Complex value;
    /// \brief Complex eigenvector corresponding to the stored eigenvalue.
    Vector<Complex> vector;
};

/// \brief Collection of eigendata-related helpers.
namespace eigendata {
    /// \brief Combine separate eigenvalue/eigenvector data from external
    /// solvers into a single vector.
    std::vector<Eigendatum> combine(const std::vector<Complex>& eigenvalues,
            const std::vector<Vector<Complex>>& eigenvectors);

    /// \brief Predicate for sorting eigendata by real part of associated
    /// eigenvalue.
    bool by_real_part(const Complex& lhs, const Complex& rhs);
    /// \brief Predicate for sorting eigendata by imaginary part of associated
    /// eigenvalue.
    bool by_imaginary_part(const Complex& lhs, const Complex& rhs);
}   // namespace eigendata
}   // namespace jump

#endif  // JUMP_EIGENDATA_DECL_HPP

