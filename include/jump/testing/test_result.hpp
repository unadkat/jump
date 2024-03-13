// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_RESULT_HPP
#define JUMP_TEST_RESULT_HPP

#include "jump/testing/test_result_decl.hpp"

namespace jump {
inline void TestResult::append(const TestResult& rhs,
        const std::string& prefix) {
    passed += rhs.passed;
    failed += rhs.failed;
    skipped += rhs.skipped;

    if (prefix == "") {
        failed_tests.insert(failed_tests.end(), rhs.failed_tests.begin(),
                rhs.failed_tests.end());
        skipped_tests.insert(skipped_tests.end(), rhs.skipped_tests.begin(),
                rhs.skipped_tests.end());
    } else {
        for (const auto& failure : rhs.failed_tests) {
            if (failure != "") {
                failed_tests.push_back(std::format("{}: {}", prefix, failure));
            } else {
                failed_tests.push_back(prefix);
            }
        }
        for (const auto& skipped : rhs.skipped_tests) {
            if (skipped != "") {
                skipped_tests.push_back(std::format("{}: {}", prefix, skipped));
            } else {
                skipped_tests.push_back(prefix);
            }
        }
    }
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
    return {.passed = 1, .failed = 0, .skipped = 0, .failed_tests = {},
        .skipped_tests = {}};
}

inline TestResult TestResult::fail(std::string name) {
    return {.passed = 0, .failed = 1, .skipped = 0, .failed_tests = {name},
        .skipped_tests = {}};
}

inline TestResult TestResult::skip(std::string name) {
    return {.passed = 0, .failed = 0, .skipped = 1, .failed_tests = {},
        .skipped_tests = {name}};
}
}   // namespace jump

#endif  // JUMP_TEST_RESULT_HPP

