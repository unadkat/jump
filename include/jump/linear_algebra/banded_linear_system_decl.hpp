// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_BANDED_LINEAR_SYSTEM_DECL_HPP
#define JUMP_BANDED_LINEAR_SYSTEM_DECL_HPP

#include <format>
#include <vector>

#include "jump/data/banded_matrix.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/exception.hpp"
#include "jump/linear_algebra/linear_system_base.hpp"
#include "jump/utility/external.hpp"

namespace Jump {
/// \brief A linear system \f$A\vec{x}=\vec{b}\f$, where \f$A\f$ is a matrix and
/// \f$\vec{b}\f$ is a right-hand side vector. Upon solving, we have the
/// solution \f$x\f$.
template <typename T>
class BandedLinearSystem : public LinearSystemBase {
    private:
        /// \brief Reference to the matrix \f$A\f$.
        BandedMatrix<T>& m_A;
        /// \brief Reference to the right-hand side `Vector` \f$b\f$. Upon
        /// calling `solve`, this will contain the solution.
        Vector<T>& m_b;

    public:
        /// \brief Initialise the linear system with references to the
        /// `BandedMatrix` and `Vector` in \f$A\vec{x}=\vec{b}\f$.
        BandedLinearSystem(BandedMatrix<T>& A, Vector<T>& b);

        /// \brief Solve the linear system.
        void solve();
#ifdef JUMP_HAS_LAPACKE
        /// \brief Call the external solver for this linear system type.
        void solve_lapacke();
#endif  // JUMP_HAS_LAPACKE

        /// \brief Return the number of equations in the linear system.
        std::size_t order() const;
};
}   // namespace Jump

#endif  // JUMP_BANDED_LINEAR_SYSTEM_DECL_HPP

