// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_RESULT_DECL_HPP
#define JUMP_TEST_RESULT_DECL_HPP

#include "jump/utility/types.hpp"

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

static Real epsilon_relative{1e-6};
static Real epsilon_absolute{1e-12};

template <typename T>
auto approx(const T& lhs, const T& rhs) -> bool;

template <typename T>
auto vanishes(const T& x) -> bool;
}   // namespace jump

#endif  // JUMP_TEST_RESULT_DECL_HPP

