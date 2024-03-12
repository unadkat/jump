// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_DECL_HPP
#define JUMP_TEST_DECL_HPP

#include <algorithm>
#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "jump/utility/logging.hpp"
#include "jump/utility/utility.hpp"

namespace jump {
struct TestResult {
    std::size_t passed{};
    std::size_t failed{};
    std::size_t skipped{};
    std::vector<std::string> failed_tests;
    std::vector<std::string> skipped_tests;

    /// \brief Add new set of results to existing set, either directly appending
    /// the failed/skipped name lists (i.e. the results come from the same
    /// test), or with a prefix (i.e. the results come from a sub-test).
    void append(const TestResult& result, const std::string& prefix = "");
    /// \brief Helper function to append result tracker with success or fail
    /// (with name of failed test), for adding results to an existing set.
    void add_check(bool expr, std::string fail_name);

    static TestResult pass();
    static TestResult fail(std::string name = "");
    static TestResult skip(std::string name = "");
};

class AtomicTest {
    private:
        std::string m_name;
        std::vector<std::string> m_tags;
        std::function<TestResult()> m_func;

    public:
        AtomicTest(std::string_view name, std::vector<std::string> tags,
                const std::function<TestResult()>& func);

        TestResult run() const;

        const std::string& name() const;
        const std::vector<std::string>& tags() const;
};

class Test {
    private:
        std::string m_name;
        std::vector<std::string> m_tags;
        std::vector<AtomicTest> m_atomic_tests;
        std::vector<std::string> m_skip;

    public:
        Test(std::string_view name, std::vector<std::string> tags);

        void register_atomic_test(AtomicTest test);
        void register_atomic_tests(std::vector<AtomicTest> tests);
        void skip(const std::vector<std::string>& skip_tags);
        TestResult run(const std::vector<std::string>& skip_tags) const;

        const std::string& name() const;
        const std::vector<std::string>& tags() const;
        const std::vector<AtomicTest>& tests() const;
};

class TestSuite {
    private:
        std::string m_name;
        std::vector<Test> m_tests;
        std::vector<std::string> m_skip;

    public:
        TestSuite(std::string_view name);

        void register_test(Test test);
        void register_tests(std::vector<Test> tests);
        void skip(const std::vector<std::string>& skip_tags);
        void run() const;

        const std::string& name() const;
        const std::vector<Test>& tests() const;
};
}   // namespace jump

#endif  // JUMP_TEST_DECL_HPP

