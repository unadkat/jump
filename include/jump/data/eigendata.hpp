// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_EIGENDATA_HPP
#define JUMP_EIGENDATA_HPP

#include <vector>

#include "jump/data/vector.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/types.hpp"

namespace Jump {

    /// \brief Simple struct to hold a complex eigenvalue and corresponding
    /// complex eigenvector.
    struct Eigendatum
    {
        /// \brief Complex eigenvalue.
        Complex value;
        /// \brief Complex eigenvector corresponding to the stored eigenvalue.
        Vector<Complex> vector;
    };

    /// \brief Collection of eigendata-related helpers.
    namespace Eigendata {

        /// \brief Combine separate eigenvalue/eigenvector data from external
        /// solvers into a single vector.
        std::vector<Eigendatum> combine(const std::vector<Complex>& eigenvalues,
                const std::vector<Vector<Complex>>& eigenvectors);

        /// \brief Predicate for sorting eigendata by real part of associated
        /// eigenvalue.
        bool by_real_part(const Complex& lhs, const Complex& rhs);
        /// \brief Predicate for sorting eigendata by imaginary part of
        /// associated eigenvalue.
        bool by_imaginary_part(const Complex& lhs, const Complex& rhs);

        #include "eigendata_impl.hpp"

    }   // namespace Eigendata

}   // namespace Jump

#endif  // JUMP_EIGENDATA_HPP

