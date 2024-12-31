// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_GENERALISED_EIGENVALUE_SYSTEM_HPP
#define JUMP_GENERALISED_EIGENVALUE_SYSTEM_HPP

#include "jump/data/dense_matrix.hpp"
#include "jump/data/eigendata.hpp"
#include "jump/linear_algebra/linear_system_base.hpp"

#include <vector>

namespace jump {
/// \brief A linear eigenvalue system \f$A\vec{x}=\lambda B\vec{x}\f$, where
/// \f$A\f$ and \f$B\f$ are DenseMatrices. Upon solving, we have pairs
/// comprising an eigenvalue \f$\lambda\f$ and corresponding eigenvector
/// \f$\vec{x}\f$.
template <typename T>
class GeneralisedEigenvalueSystem : public LinearSystemBase {
    public:
        /// \brief Initialise the eigenvalue system with references to the
        /// DenseMatrices in \f$A\vec{x}=\lambda*B*\vec{x}\f$ and the solution
        /// data structure.
        GeneralisedEigenvalueSystem(DenseMatrix<T>& A, DenseMatrix<T>& B,
                std::vector<Eigendatum<T>>& data);

        /// \brief Solve the eigenvalue system.
        virtual void solve() override;
        /// \brief Return the number of equations in the eigenvalue system.
        virtual auto order() const -> std::size_t override;

    private:
#ifdef JUMP_HAS_LAPACKE
        /// \brief Call the external solver for this eigenvalue system type.
        void solve_lapacke();
#endif  // JUMP_HAS_LAPACKE

    private:
        /// \brief Reference to the (square) matrix \f$A\f$.
        DenseMatrix<T>& m_A;
        /// \brief Reference to the (square) matrix \f$B\f$.
        DenseMatrix<T>& m_B;
        /// \brief Reference to the solution data structure, which will store
        /// eigenvalue/eigenvector pairs.
        std::vector<Eigendatum<T>>& m_data;
};
}   // namespace jump

#endif  // JUMP_GENERALISED_EIGENVALUE_SYSTEM_HPP

