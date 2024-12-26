// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_GENERALISED_EIGENVALUE_SYSTEM_HPP
#define JUMP_GENERALISED_EIGENVALUE_SYSTEM_HPP

#include "jump/linear_algebra/generalised_eigenvalue_system_decl.hpp"

#include "jump/debug/exception.hpp"
#include "jump/utility/external.hpp"
#include "jump/utility/types.hpp"

#include <format>

namespace jump {
template <typename T>
inline GeneralisedEigenvalueSystem<T>::GeneralisedEigenvalueSystem(
        DenseMatrix<T>& A, DenseMatrix<T>& B,
        std::vector<Eigendatum<T>>& data) :
    m_A{A},
    m_B{B},
    m_data{data} {
#ifndef NDEBUG
    if (A.num_rows() != A.num_columns()) {
        throw RuntimeError{InvalidArgumentError{.argument = "A",
            .value = std::format("Matrix of size ({}, {})", A.num_rows(),
                    A.num_columns()),
            .expected = "a square matrix"}};
    }
    if (B.num_rows() != B.num_columns()) {
        throw RuntimeError{InvalidArgumentError{.argument = "B",
            .value = std::format("Matrix of size ({}, {})", B.num_rows(),
                    B.num_columns()),
            .expected = "a square matrix"}};
    }
    if (A.num_rows() != B.num_rows()) {
        throw RuntimeError{Mismatch2DError{.name1 = "A", .size1 = A.size(),
            .name2 = "B", .size2 = B.size()}};
    }
#endif  // NDEBUG
}

/// We delegate the solution to the external linear algebra library, otherwise
/// throw an error.
template <typename T>
inline void GeneralisedEigenvalueSystem<T>::solve() {
#ifdef JUMP_HAS_LAPACKE
    solve_lapacke();
#else
    throw RuntimeError{BasicError{.details =
        "Native solution of generalised EVP has not been implemented"}};
#endif  // JUMP_HAS_LAPACKE
}

template <typename T>
inline auto GeneralisedEigenvalueSystem<T>::order() const -> std::size_t {
    return m_A.num_rows();
}

#ifdef JUMP_HAS_LAPACKE
/// \brief Call the external solver for a complex-valued problem.
template <>
inline void GeneralisedEigenvalueSystem<Complex>::solve_lapacke() {
#ifndef NDEBUG
    if (m_A.num_rows() != m_A.num_columns()) {
        throw RuntimeError{InvalidArgumentError{.argument = "m_A",
            .value = std::format("Matrix of size ({}, {})", m_A.num_rows(),
                    m_A.num_columns()),
            .expected = "a square matrix"}};
    }
    if (m_B.num_rows() != m_B.num_columns()) {
        throw RuntimeError{InvalidArgumentError{.argument = "m_B",
            .value = std::format("Matrix of size ({}, {})", m_B.num_rows(),
                    m_B.num_columns()),
            .expected = "a square matrix"}};
    }
    if (m_A.num_rows() != m_B.num_rows()) {
        throw RuntimeError{Mismatch2DError{.name1 = "m_A", .size1 = m_A.size(),
            .name2 = "m_B", .size2 = m_B.size()}};
    }
#endif  // NDEBUG

    // Calculate right-hand side eigenvectors and not left-hand side ones
    char jobvl{'N'};
    char jobvr{'V'};
    // Number of equations in linear system
    auto n{static_cast<lapack_int>(order())};
    // Return information
    lapack_int info{0};
    // Storage for right-hand side eigenvectors
    DenseMatrix<Complex> vr{order()};
    // (Unused) storage for left-hand side eigenvectors
    DenseMatrix<Complex> vl;
    // Leading-order dimension of vl
    lapack_int ldvl{1};

    // Eigenvalues are returned in the form \f$\frac{\alpha}{\beta}\f$, and
    // these values are stored in the these Vectors
    Vector<Complex> alpha(n);
    Vector<Complex> beta(n);

    // Solve generalised eigenvalue system
    info = LAPACKE_zggev(LAPACK_COL_MAJOR, jobvl, jobvr, n,
            reinterpret_cast<lapack_complex_double*>(m_A.data()), n,
            reinterpret_cast<lapack_complex_double*>(m_B.data()), n,
            reinterpret_cast<lapack_complex_double*>(alpha.data()),
            reinterpret_cast<lapack_complex_double*>(beta.data()),
            reinterpret_cast<lapack_complex_double*>(vl.data()), ldvl,
            reinterpret_cast<lapack_complex_double*>(vr.data()), n);
    if (info != 0) {
        throw RuntimeError{BasicError{
            .details = std::format("LAPACKE_zggev returned info = {}", info)}};
    }

    // Prepare eigendata storage
    m_data.clear();

    // Loop through returned solutions
    for (int i{0}; i < n; ++i) {
        // Form eigenvalue from the two components alpha and beta
        Complex ev{alpha[i]/beta[i]};
        // Discard this solution if the eigenvalue is not finite
        if (!(std::isfinite(ev.real()) && std::isfinite(ev.imag()))) {
            continue;
        }

        // Valid eigenvalue, so grab the corresponding column of vr, which is
        // the corresponding eigenvector
        auto it{vr.column_iterators(i)};
        Vector<Complex> evec(it.first, it.second);
        // Normalise eigenvector on L2 norm
        evec *= 1./evec.L2_norm();

        // Store the (valid) eigenvalue and eigenvector pair
        m_data.push_back({ev, evec});
    }
}
#endif  // JUMP_HAS_LAPACKE
}   // namespace jump

#endif  // JUMP_GENERALISED_EIGENVALUE_SYSTEM_HPP

