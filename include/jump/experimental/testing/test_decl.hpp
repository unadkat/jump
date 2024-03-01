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

namespace jump::experimental {
struct TestResult {
    std::size_t passed{};
    std::size_t failed{};
};

class Test {
    private:
        std::string m_name;
        std::vector<std::string> m_tags;
        std::function<TestResult()> m_func;
        TestResult m_result;

    public:
        Test(std::string_view name, std::vector<std::string> tags,
                const std::function<TestResult()>& func);

        const std::string& name() const;
        const std::vector<std::string>& tags() const;
        bool skip_test(const std::vector<std::string>& skip_tags) const;
        const TestResult& run();
        const TestResult& result() const;
};

class TestSuite {
    private:
        std::string m_name;
        std::vector<std::string> m_tags;
        std::vector<Test> m_tests;
        std::vector<std::string> m_skip;

    public:
        TestSuite(std::string_view name, std::vector<std::string> tags);

        void register_test(const Test& test);
        void register_test(Test&& test);
        void skip_tests(const std::vector<std::string>& skip_tags);
        void run();
};
}   // namespace jump::experimental

#endif  // JUMP_TEST_DECL_HPP

