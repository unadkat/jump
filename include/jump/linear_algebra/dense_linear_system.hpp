// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_DENSE_LINEAR_SYSTEM_HPP
#define JUMP_DENSE_LINEAR_SYSTEM_HPP

#include "jump/data/dense_matrix.hpp"
#include "jump/data/vector.hpp"
#include "jump/linear_algebra/linear_system_base.hpp"

namespace jump {
/// \brief A linear system \f$A\vec{x}=\vec{b}\f$, where \f$A\f$ is a matrix and
/// \f$\vec{b}\f$ is a right-hand side vector. Upon solving, we have the
/// solution \f$x\f$.
template <typename T>
class DenseLinearSystem : public LinearSystemBase {
    public:
        /// \brief Initialise the linear system with references to the
        /// `DenseMatrix` and `Vector` in \f$A\vec{x}=\vec{b}\f$.
        DenseLinearSystem(DenseMatrix<T>& A, Vector<T>& b);

        /// \brief Solve the linear system.
        virtual void solve() override;
        /// \brief Return the number of equations in the linear system.
        virtual auto order() const -> std::size_t override;

    private:
#ifdef JUMP_HAS_LAPACKE
        /// \brief Call the external solver for this linear system type.
        void solve_lapacke();
#endif  // JUMP_HAS_LAPACKE

    private:
        /// \brief Reference to the matrix \f$A\f$.
        DenseMatrix<T>& m_A;
        /// \brief Reference to the right-hand side `Vector` \f$b\f$. Upon
        /// calling `solve`, this will contain the solution.
        Vector<T>& m_b;
};
}   // namespace jump

#endif  // JUMP_DENSE_LINEAR_SYSTEM_HPP

