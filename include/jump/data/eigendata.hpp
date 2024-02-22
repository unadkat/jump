// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_EIGENDATA_HPP
#define JUMP_EIGENDATA_HPP

#include "jump/data/eigendata_decl.hpp"

namespace jump::eigendata {
/// After the population with the new eigenvalue/eigenvector pairs, the data is
/// left unsorted with respect to the eigenvalues. It is up to the user to sort
/// the data as required for their purposes.
inline std::vector<Eigendatum> combine(const std::vector<Complex>& eigenvalues,
        const std::vector<Vector<Complex>>& eigenvectors) {
#ifndef NDEBUG
    if (eigenvalues.size() != eigenvectors.size())
        throw RuntimeError{Mismatch1DError{.name1 = "eigenvalues",
            .size1 = eigenvalues.size(), .name2 = "eigenvectors",
            .size2 = eigenvectors.size()}};
#endif  // NDEBUG

    // TODO: rewrite when std::ranges::to is available to convert from view
    std::vector<Eigendatum> result;
    for (std::size_t i{0}, size{eigenvalues.size()}; i < size; ++i) {
        result.push_back(std::move(Eigendatum{eigenvalues[i],
                    eigenvectors[i]}));
    }
    return result;
}

/// If Re(w) < Re(z), or Re(w) = Re(z) and Im(w) < Im(z), we assert that w < z.
/// This sets up a strict weak ordering in line with the requirements for
/// `std::sort`.
inline bool by_real_part(const Complex& lhs, const Complex& rhs) {
    return lhs.real() < rhs.real() ? true :
        (lhs.real() == rhs.real() ? lhs.imag() < rhs.imag() : false);
}

/// If Im(w) < Im(z), or Im(w) = Im(z) and Re(w) < Re(z), we assert that w < z.
/// This sets up a strict weak ordering in line with the requirements for
/// `std::sort`.
inline bool by_imaginary_part(const Complex& lhs, const Complex& rhs) {
    return lhs.real() < rhs.real() ? true :
        (lhs.real() == rhs.real() ? lhs.imag() < rhs.imag() : false);
}
}   // namespace jump::eigendata

#endif  // JUMP_EIGENDATA_HPP

