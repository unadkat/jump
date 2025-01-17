// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#include "jump/data/eigendata.hpp"

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"

namespace jump {
/// After the population with the new eigenvalue/eigenvector pairs, the data is
/// left unsorted with respect to the eigenvalues. It is up to the user to sort
/// the data as required for their purposes.
template <typename T>
auto copy_eigendata(const std::vector<T>& eigenvalues,
        const std::vector<Vector<T>>& eigenvectors)
        -> std::vector<Eigendatum<T>> {
#ifndef NDEBUG
    if (eigenvalues.size() != eigenvectors.size())
        throw RuntimeError{Mismatch1DError{.name1 = "eigenvalues",
            .size1 = eigenvalues.size(), .name2 = "eigenvectors",
            .size2 = eigenvectors.size()}};
#endif  // NDEBUG

    // TODO: rewrite when std::ranges::to is available to convert from view
    std::vector<Eigendatum<T>> result(eigenvalues.size());
    for (std::size_t i{0}; i < result.size(); ++i) {
        result[i].value = eigenvalues[i];
        result[i].vector = eigenvectors[i];
    }
    return result;
}

/// After the population with the new eigenvalue/eigenvector pairs, the data is
/// left unsorted with respect to the eigenvalues. It is up to the user to sort
/// the data as required for their purposes.
template <typename T>
auto move_eigendata(const std::vector<T>& eigenvalues,
        std::vector<Vector<T>>&& eigenvectors)
        -> std::vector<Eigendatum<T>> {
#ifndef NDEBUG
    if (eigenvalues.size() != eigenvectors.size())
        throw RuntimeError{Mismatch1DError{.name1 = "eigenvalues",
            .size1 = eigenvalues.size(), .name2 = "eigenvectors",
            .size2 = eigenvectors.size()}};
#endif  // NDEBUG

    // TODO: rewrite when std::ranges::to is available to convert from view
    std::vector<Eigendatum<T>> result(eigenvalues.size());
    for (std::size_t i{0}; i < result.size(); ++i) {
        result[i].value = eigenvalues[i];
        result[i].vector = std::move(eigenvectors[i]);
    }
    return result;
}

/// If Re(w) < Re(z), or Re(w) = Re(z) and Im(w) < Im(z), we assert that w < z.
/// This sets up a strict weak ordering in line with the requirements for
/// `std::sort`.
auto sort_eigendata_real(const Complex& lhs, const Complex& rhs) -> bool {
    return lhs.real() < rhs.real() ? true :
        (lhs.real() == rhs.real() ? lhs.imag() < rhs.imag() : false);
}

/// If Im(w) < Im(z), or Im(w) = Im(z) and Re(w) < Re(z), we assert that w < z.
/// This sets up a strict weak ordering in line with the requirements for
/// `std::sort`.
auto sort_eigendata_imag(const Complex& lhs, const Complex& rhs) -> bool {
    return lhs.imag() < rhs.imag() ? true :
        (lhs.imag() == rhs.imag() ? lhs.real() < rhs.real() : false);
}

// ========================================================================
// Instantiation
// ========================================================================

template struct Eigendatum<Complex>;

template auto copy_eigendata<Complex>(const std::vector<Complex>& eigenvalues,
        const std::vector<Vector<Complex>>& eigenvectors)
        -> std::vector<Eigendatum<Complex>>;
template auto move_eigendata<Complex>(const std::vector<Complex>& eigenvalues,
        std::vector<Vector<Complex>>&& eigenvectors)
        -> std::vector<Eigendatum<Complex>>;
}   // namespace jump

