// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#include "jump/linear_algebra/banded_linear_system.hpp"

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/external.hpp"
#include "jump/utility/types.hpp"

#include <format>
#include <vector>

namespace jump {
template <typename T>
BandedLinearSystem<T>::BandedLinearSystem(BandedMatrix<T>& A, Vector<T>& b) :
    m_A{A},
    m_b{b} {
#ifndef NDEBUG
    if (A.num_columns() != b.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "A", .size1 = A.size(),
            .name2 = "b", .size2 = {b.size(), 1}}};
    }
#endif  // NDEBUG
}

/// We delegate the solution to the external linear algebra library, otherwise
/// throw an error.
template <typename T>
void BandedLinearSystem<T>::solve() {
#ifdef JUMP_HAS_LAPACKE
    solve_lapacke();
#else
    throw RuntimeError{BasicError{.details =
        "Native solution of dense linear system has not been implemented"}};
#endif  // JUMP_HAS_LAPACKE
}

template <typename T>
auto BandedLinearSystem<T>::order() const -> std::size_t {
    return m_A.num_rows();
}

#ifdef JUMP_HAS_LAPACKE
/// Call the external solver for a real-valued problem.
template <>
void BandedLinearSystem<Real>::solve_lapacke() {
#ifndef NDEBUG
    if (m_A.num_columns() != m_b.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "m_A", .size1 = m_A.size(),
            .name2 = "m_b", .size2 = {m_b.size(), 1}}};
    }
#endif  // NDEBUG

    // Number of equations in linear system
    auto n{static_cast<lapack_int>(order())};
    // Number of bands
    auto k{static_cast<lapack_int>(m_A.num_bands())};
    // Leading dimension of internal banded storage
    lapack_int ldab{3*k + 1};
    // Return information
    lapack_int info{0};
    // Pivot information
    std::vector<lapack_int> ipiv(n);

    // Solve Ax = b and stores the result in b
    info = LAPACKE_dgbsv(LAPACK_COL_MAJOR, n, k, k, 1, m_A.data(), ldab,
            ipiv.data(), m_b.data(), n);
    if (info != 0) {
        throw RuntimeError{BasicError{
            .details = std::format("LAPACKE_dgbsv returned info = {}", info)}};
    }
}

/// Call the external solver for a complex-valued problem.
template <>
void BandedLinearSystem<Complex>::solve_lapacke() {
#ifndef NDEBUG
    if (m_A.num_columns() != m_b.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "m_A", .size1 = m_A.size(),
            .name2 = "m_b", .size2 = {m_b.size(), 1}}};
    }
#endif  // NDEBUG

    // Number of equations in linear system
    auto n{static_cast<lapack_int>(order())};
    // Number of bands
    auto k{static_cast<lapack_int>(m_A.num_bands())};
    // Leading dimension of internal banded storage
    lapack_int ldab{3*k + 1};
    // Return information
    lapack_int info{0};
    // Pivot information
    std::vector<lapack_int> ipiv(n);

    // Solve Ax = b and stores the result in b
    info = LAPACKE_zgbsv(LAPACK_COL_MAJOR, n, k, k, 1,
            reinterpret_cast<lapack_complex_double*>(m_A.data()), ldab,
            ipiv.data(),
            reinterpret_cast<lapack_complex_double*>(m_b.data()), n);
    if (info != 0) {
        throw RuntimeError{BasicError{
            .details = std::format("LAPACKE_zgbsv returned info = {}", info)}};
    }
}
#endif  // JUMP_HAS_LAPACKE

// ========================================================================
// Instantiation
// ========================================================================
template class BandedLinearSystem<Real>;
template class BandedLinearSystem<Complex>;
}   // namespace jump

