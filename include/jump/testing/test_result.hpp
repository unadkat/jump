// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_RESULT_HPP
#define JUMP_TEST_RESULT_HPP

#include "jump/testing/test_result_decl.hpp"

namespace jump {
inline bool TestResult::has_info() const {
    return passed + failed + skipped > 0;
}

inline void TestResult::operator+=(const TestResult& rhs) {
    passed += rhs.passed;
    failed += rhs.failed;
    skipped += rhs.skipped;
    failed_tests.insert(failed_tests.end(), rhs.failed_tests.begin(),
            rhs.failed_tests.end());
    skipped_tests.insert(skipped_tests.end(), rhs.skipped_tests.begin(),
            rhs.skipped_tests.end());
}

inline void TestResult::add_check(bool expr, std::string fail_name) {
    if (expr) {
        ++passed;
    } else {
        ++failed;
        failed_tests.push_back(std::move(fail_name));
    }
}

inline TestResult TestResult::pass() {
    return {.name = {}, .sub_results = {}, .passed = 1, .failed = 0,
        .skipped = 0, .failed_tests = {}, .skipped_tests = {}};
}

inline TestResult TestResult::fail(std::string name) {
    return {.name = {}, .sub_results = {}, .passed = 0, .failed = 1,
        .skipped = 0, .failed_tests = {name}, .skipped_tests = {}};
}

inline TestResult TestResult::skip(std::string name) {
    return {.name = {}, .sub_results = {}, .passed = 0, .failed = 0,
        .skipped = 1, .failed_tests = {}, .skipped_tests = {name}};
}

template <typename T>
inline bool approx(const T& lhs, const T& rhs) {
    if (lhs == rhs) {
        return true;
    }
    auto diff{std::abs(lhs - rhs)};
    auto norm{std::min(std::abs(lhs) + std::abs(rhs),
            std::numeric_limits<T>::max())};
    return diff < std::max(epsilon_absolute, epsilon_relative*norm);
}

template <typename T>
inline bool approx(const std::complex<T>& lhs, const std::complex<T>& rhs) {
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
inline bool approx(const Dual<N, T>& lhs, const Dual<N, T>& rhs) {
    return approx(lhs.value, rhs.value);
}

template <typename T>
inline bool approx(const Vector<T>& lhs, const Vector<T>& rhs) {
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
inline bool approx(const BandedMatrix<T>& lhs, const BandedMatrix<T>& rhs) {
    return approx(lhs.as_vector(), rhs.as_vector());
}

template <typename T>
inline bool approx(const DenseMatrix<T>& lhs, const DenseMatrix<T>& rhs) {
    return approx(lhs.as_vector(), rhs.as_vector());
}

template <typename T>
inline bool vanishes(const T& x) {
    return approx(x, T{0});
}

template <typename T>
inline bool vanishes(const Vector<T>& v) {
    for (const auto& x : v) {
        if (!vanishes(x)) {
            return false;
        }
    }
    return true;
}

template <typename T>
inline bool vanishes(const BandedMatrix<T>& M) {
    return vanishes(M.as_vector());
}

template <typename T>
inline bool vanishes(const DenseMatrix<T>& M) {
    return vanishes(M.as_vector());
}
}   // namespace jump

#endif  // JUMP_TEST_RESULT_HPP

