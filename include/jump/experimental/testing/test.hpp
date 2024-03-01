// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_HPP
#define JUMP_TEST_HPP

#include "jump/experimental/testing/test_decl.hpp"

namespace jump::experimental {
inline Test::Test(std::string_view name, std::vector<std::string> tags,
        const std::function<TestResult()>& func) :
    m_name{name},
    m_tags{std::move(tags)},
    m_func{func} {
}

inline const std::string& Test::name() const {
    return m_name;
}

inline const std::vector<std::string>& Test::tags() const {
    return m_tags;
}

inline bool Test::skip_test(const std::vector<std::string>& skip_tags) const {
    for (const auto& tag : skip_tags) {
        if (std::find(m_tags.begin(), m_tags.end(), tag)
                != m_tags.end()) {
            return true;
        }
    }
    return false;
}

inline const TestResult& Test::run() {
    m_result = m_func();
    return m_result;
}

inline const TestResult& Test::result() const {
    return m_result;
}

inline TestSuite::TestSuite(std::string_view name,
        std::vector<std::string> tags) :
    m_name{name},
    m_tags{std::move(tags)} {
}

inline void TestSuite::register_test(const Test& test) {
    m_tests.push_back(test);
}

inline void TestSuite::register_test(Test&& test) {
    m_tests.push_back(test);
}

inline void TestSuite::skip_tests(const std::vector<std::string>& skip_tags) {
    for (const auto& tag : skip_tags) {
        if (std::find(m_skip.begin(), m_skip.end(), tag)
                == m_skip.end()) {
            m_skip.push_back(tag);
        }
    }
}

inline void TestSuite::run() {
    TestResult all_results{};
    for (auto& test : m_tests) {
        if (test.skip_test(m_skip)) {
            continue;
        }

        test.run();
        std::cout << std::format("Test {}: {}/{} tests passed\n", test.name(),
                test.result().passed,
                test.result().passed + test.result().failed);
        all_results.passed += test.result().passed;
        all_results.failed += test.result().failed;
    }

    std::cout << std::format("Overall results: {}/{} tests passed\n",
            all_results.passed, all_results.passed + all_results.failed);
}
}   // namespace jump::experimental

#endif  // JUMP_TEST_HPP

