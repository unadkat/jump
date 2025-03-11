// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_TEST_RESULT_HPP
#define JUMP_TEST_RESULT_HPP

#include "jump/autodiff/dual.hpp"
#include "jump/data/banded_matrix.hpp"
#include "jump/data/dense_matrix.hpp"
#include "jump/utility/types.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"

#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
#include "jump/experimental/expression_templates/concepts.hpp"
#endif //   JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

#include <cmath>
#include <complex>
#include <limits>
#include <string>
#include <vector>

namespace jump {
struct TestResult {
    /// \brief Stores name of test entity.
    std::string name;
    /// \brief Stores results of subtests (if this is empty, the structure is a
    /// leaf in the result structure of a set of tests).
    std::vector<TestResult> sub_results;

    /// \brief Stores number of passed tests.
    std::size_t passed{};
    /// \brief Stores number of failed tests.
    std::size_t failed{};
    /// \brief Stores number of skipped tests.
    std::size_t skipped{};
    /// \brief Stores list of names of failed tests.
    std::vector<std::string> failed_tests;
    /// \brief Stores list of names of skipped tests.
    std::vector<std::string> skipped_tests;

    /// \brief Indicates if this instance has some data to report (in addition
    /// to any children).
    auto has_info() const -> bool;
    /// \brief Add new set of results to the existing set (intended for a leaf).
    /// Results for subtests should be pushed onto the sub_results collection.
    void operator+=(const TestResult& result);
    /// \brief Helper function to append result tracker with success or fail
    /// (with name of failed test), for adding results to an existing leaf.
    void add_check(bool expr, std::string fail_name);

    /// \brief Generate a basic passed test.
    static auto pass() -> TestResult;
    /// \brief Generate a failed test and pass the failing test name on.
    static auto fail(std::string name = "") -> TestResult;
    /// \brief Generate a skipped test and pass the skipped test name on.
    static auto skip(std::string name = "") -> TestResult;
};

inline Real epsilon_relative{1e-6};
inline Real epsilon_absolute{1e-12};

template <typename T, typename U>
requires (true
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
    && !VectorExpression<T> && !VectorExpression<U>
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
    && !BandedMatrixExpression<T> && !BandedMatrixExpression<U>
    && !DenseMatrixExpression<T> && !DenseMatrixExpression<U>
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
)
auto approx(const T& lhs, const U& rhs) -> bool;

template <typename T>
requires (true
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
    && !VectorExpression<T>
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
    && !BandedMatrixExpression<T>
    && !DenseMatrixExpression<T>
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
)
auto vanishes(const T& x) -> bool;

#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
template <VectorExpression Left, VectorExpression Right>
constexpr auto approx(const Left& lhs, const Right& rhs) -> bool;

template <VectorExpression Expr>
constexpr auto vanishes(const Expr& expr) -> bool;
#endif //   JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
template <BandedMatrixExpression Left, BandedMatrixExpression Right>
constexpr auto approx(const Left& lhs, const Right& rhs) -> bool;

template <BandedMatrixExpression Expr>
constexpr auto vanishes(const Expr& expr) -> bool;

template <DenseMatrixExpression Left, DenseMatrixExpression Right>
constexpr auto approx(const Left& lhs, const Right& rhs) -> bool;

template <DenseMatrixExpression Expr>
constexpr auto vanishes(const Expr& expr) -> bool;
#endif //   JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES

// ========================================================================
// Implementation
// ========================================================================

template <typename T, typename U>
requires (true
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
    && !VectorExpression<T> && !VectorExpression<U>
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
    && !BandedMatrixExpression<T> && !BandedMatrixExpression<U>
    && !DenseMatrixExpression<T> && !DenseMatrixExpression<U>
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
)
inline auto approx(const T& lhs, const U& rhs) -> bool {
    if (lhs == rhs) {
        return true;
    }
    auto diff{std::abs(lhs - rhs)};
    auto norm{std::min(std::abs(lhs) + std::abs(rhs),
            std::numeric_limits<T>::max())};
    return diff < std::max(epsilon_absolute, epsilon_relative*norm);
}

template <typename T>
inline auto approx(const std::complex<T>& lhs, const std::complex<T>& rhs)
        -> bool {
    if (lhs == rhs) {
        return true;
    }
    auto diff_real{std::abs(lhs.real() - rhs.real())};
    auto norm_real{std::min(std::abs(lhs.real()) + std::abs(rhs.real()),
            std::numeric_limits<T>::max())};
    auto diff_imag{std::abs(lhs.imag() - rhs.imag())};
    auto norm_imag{std::min(std::abs(lhs.imag()) + std::abs(rhs.imag()),
            std::numeric_limits<T>::max())};
    return diff_real < std::max(epsilon_absolute, epsilon_relative*norm_real)
        && diff_imag < std::max(epsilon_absolute, epsilon_relative*norm_imag);
}

template <std::size_t N, typename T>
inline auto approx(const Dual<N, T>& lhs, const Dual<N, T>& rhs) -> bool {
    return approx(lhs.value, rhs.value);
}

template <typename T>
inline auto approx(const Vector<T>& lhs, const Vector<T>& rhs) -> bool {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}, N{lhs.size()}; i < N; ++i) {
        if (!approx(lhs[i], rhs[i])) {
            return false;
        }
    }
    return true;
}

template <typename T>
inline auto approx(const BandedMatrix<T>& lhs, const BandedMatrix<T>& rhs)
        -> bool {
    return approx(lhs.as_vector(), rhs.as_vector());
}

template <typename T>
inline auto approx(const DenseMatrix<T>& lhs, const DenseMatrix<T>& rhs)
        -> bool {
    return approx(lhs.as_vector(), rhs.as_vector());
}

template <typename T>
requires (true
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
    && !VectorExpression<T>
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
    && !BandedMatrixExpression<T>
    && !DenseMatrixExpression<T>
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
)
inline auto vanishes(const T& x) -> bool {
    return approx(x, T{0});
}

template <typename T>
inline auto vanishes(const Vector<T>& v) -> bool {
    for (const auto& x : v) {
        if (!vanishes(x)) {
            return false;
        }
    }
    return true;
}

template <typename T>
inline auto vanishes(const BandedMatrix<T>& M) -> bool {
    return vanishes(M.as_vector());
}

template <typename T>
inline auto vanishes(const DenseMatrix<T>& M) -> bool {
    return vanishes(M.as_vector());
}

#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
template <VectorExpression Left, VectorExpression Right>
inline constexpr auto approx(const Left& lhs, const Right& rhs) -> bool {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}, N{lhs.size()}; i < N; ++i) {
        if (!approx(lhs[i], rhs[i])) {
            return false;
        }
    }
    return true;
}

template <VectorExpression Expr>
inline constexpr auto vanishes(const Expr& expr) -> bool {
    for (std::size_t i{0}, N{expr.size()}; i < N; ++i) {
        if (!vanishes(expr[i])) {
            return false;
        }
    }
    return true;
}
#endif //   JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
template <BandedMatrixExpression Left, BandedMatrixExpression Right>
inline constexpr auto approx(const Left& lhs, const Right& rhs) -> bool {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
    if (lhs.num_bands() != rhs.num_bands()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs.num_bands()",
            .size1 = lhs.num_bands(), .name2 = "rhs.num_bands()",
            .size2 = rhs.num_bands()}};
    }
#endif  // NDEBUG

    return approx(lhs.as_vector(), rhs.as_vector());
}

template <BandedMatrixExpression Expr>
inline constexpr auto vanishes(const Expr& expr) -> bool {
    return vanishes(expr.as_vector());
}

template <DenseMatrixExpression Left, DenseMatrixExpression Right>
inline constexpr auto approx(const Left& lhs, const Right& rhs) -> bool {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    return approx(lhs.as_vector(), rhs.as_vector());
}

template <DenseMatrixExpression Expr>
inline constexpr auto vanishes(const Expr& expr) -> bool {
    return vanishes(expr.as_vector());
}
#endif //   JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
}   // namespace jump

#endif  // JUMP_TEST_RESULT_HPP

