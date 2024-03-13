// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_RESULT_DECL_HPP
#define JUMP_TEST_RESULT_DECL_HPP

#include <format>
#include <string>
#include <vector>

namespace jump {
struct TestResult {
    /// \brief Stores number of passed tests (and sub-tests).
    std::size_t passed{};
    /// \brief Stores number of failed tests (and sub-tests).
    std::size_t failed{};
    /// \brief Stores number of skipped tests (and sub-tests).
    std::size_t skipped{};
    /// \brief Stores list of names of failed tests.
    std::vector<std::string> failed_tests;
    /// \brief Stores list of skipped tests.
    std::vector<std::string> skipped_tests;

    /// \brief Add new set of results to existing set, either directly appending
    /// the failed/skipped name lists (i.e. the results come from the same
    /// test), or with a prefix (i.e. the results come from a sub-test).
    void append(const TestResult& result, const std::string& prefix = "");
    /// \brief Helper function to append result tracker with success or fail
    /// (with name of failed test), for adding results to an existing set.
    void add_check(bool expr, std::string fail_name);

    /// \brief Generate a basic passed test.
    static TestResult pass();
    /// \brief Generate a failed test and pass the failing test name on.
    static TestResult fail(std::string name = "");
    /// \brief Generate a skipped test and pass the skipped test name on.
    static TestResult skip(std::string name = "");
};
}   // namespace jump

#endif  // JUMP_TEST_RESULT_DECL_HPP

