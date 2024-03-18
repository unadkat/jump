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
inline bool approx_rel(const T& lhs, const T& rhs) {
    if constexpr (is_dual_v<T>) {
        auto a{abs(lhs).value}, b{abs(rhs).value};
        if (a > b) {
            return abs(a - b)/b < epsilon_rel;
        } else {
            return abs(b - a)/a < epsilon_rel;
        }
    } else {
        auto a{std::abs(lhs)}, b{std::abs(rhs)};
        if (a > b) {
            return std::abs(a - b)/b < epsilon_rel;
        } else {
            return std::abs(b - a)/a < epsilon_rel;
        }
    }
}

template <typename T>
inline bool approx_abs(const T& lhs, const T& rhs) {
    if constexpr (is_dual_v<T>) {
        return abs(lhs - rhs).value < epsilon_abs;
    } else {
        return std::abs(lhs - rhs) < epsilon_abs;
    }
}

template <typename T>
inline bool vanishes(const T& x) {
    return approx_abs(x, T{0});
}
}   // namespace jump

#endif  // JUMP_TEST_RESULT_HPP

