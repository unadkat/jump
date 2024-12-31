// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#include "jump/testing/test_result.hpp"

namespace jump {
auto TestResult::has_info() const -> bool {
    return passed + failed + skipped > 0;
}

void TestResult::operator+=(const TestResult& rhs) {
    passed += rhs.passed;
    failed += rhs.failed;
    skipped += rhs.skipped;
    failed_tests.insert(failed_tests.end(), rhs.failed_tests.begin(),
            rhs.failed_tests.end());
    skipped_tests.insert(skipped_tests.end(), rhs.skipped_tests.begin(),
            rhs.skipped_tests.end());
}

void TestResult::add_check(bool expr, std::string fail_name) {
    if (expr) {
        ++passed;
    } else {
        ++failed;
        failed_tests.push_back(std::move(fail_name));
    }
}

auto TestResult::pass() -> TestResult {
    return {.name = {}, .sub_results = {}, .passed = 1, .failed = 0,
        .skipped = 0, .failed_tests = {}, .skipped_tests = {}};
}

auto TestResult::fail(std::string name) -> TestResult {
    return {.name = {}, .sub_results = {}, .passed = 0, .failed = 1,
        .skipped = 0, .failed_tests = {name}, .skipped_tests = {}};
}

auto TestResult::skip(std::string name) -> TestResult {
    return {.name = {}, .sub_results = {}, .passed = 0, .failed = 0,
        .skipped = 1, .failed_tests = {}, .skipped_tests = {name}};
}
}   // namespace jump

