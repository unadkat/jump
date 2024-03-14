// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_HEIRARCHY_HPP
#define JUMP_TEST_HEIRARCHY_HPP

#include "jump/testing/heirarchy_decl.hpp"

namespace jump {
inline AtomicTest::AtomicTest(std::string_view name,
        std::vector<std::string> tags,
        const std::function<TestResult()>& func) :
    m_name{name},
    m_tags{std::move(tags)},
    m_func{func} {
    std::ranges::sort(m_tags);
}

inline TestResult AtomicTest::run() const {
    TestResult result{m_func()};
    result.name = name();
    return result;
}

inline const std::string& AtomicTest::name() const {
    return m_name;
}

inline const std::vector<std::string>& AtomicTest::tags() const {
    return m_tags;
}

inline Test::Test(std::string_view name, std::vector<std::string> tags) :
    m_name{name},
    m_tags{std::move(tags)} {
    std::ranges::sort(m_tags);
}

inline void Test::register_atomic_test(AtomicTest test) {
    m_atomic_tests.push_back(std::move(test));
}

inline void Test::register_atomic_tests(std::vector<AtomicTest> tests) {
    for (auto& test: tests) {
        register_atomic_test(std::move(test));
    }
}

inline TestResult Test::run(const std::vector<std::string>& skip_tags) const {
    TestResult result;
    result.name = name();
    for (const auto& test: m_atomic_tests) {
        if (std::ranges::find(skip_tags, test.name()) != skip_tags.end()
                || has_intersection(test.tags(), skip_tags)) {
            result += TestResult::skip(test.name());
            continue;
        }

        try {
            result.sub_results.push_back(test.run());
        } catch (std::exception& e) {
            result += TestResult::fail(test.name());
        }
    }
    return result;
}

inline const std::string& Test::name() const {
    return m_name;
}

inline const std::vector<std::string>& Test::tags() const {
    return m_tags;
}

inline const std::vector<AtomicTest>& Test::tests() const {
    return m_atomic_tests;
}

template <typename T>
inline TestSuite<T>::TestSuite(std::string_view name,
        std::vector<std::string> tags) :
    m_name{name},
    m_tags{std::move(tags)} {
    std::ranges::sort(m_tags);
}

template <typename T>
inline void TestSuite<T>::register_test(T test) {
    m_tests.push_back(std::move(test));
}

template <typename T>
inline void TestSuite<T>::register_tests(std::vector<T> tests) {
    for (auto& test : tests) {
        register_test(std::move(test));
    }
}

template <typename T>
inline TestResult TestSuite<T>::run(
        const std::vector<std::string>& skip_tags) const {
    TestResult all_results;
    all_results.name = name();
    for (const auto& test : m_tests) {
        if (std::ranges::find(skip_tags, test.name()) != skip_tags.end()
                || has_intersection(test.tags(), skip_tags)) {
            all_results += TestResult::skip(test.name());
        } else {
            all_results.sub_results.push_back(test.run(skip_tags));
        }
    }

    return all_results;
}

template <typename T>
inline const std::string& TestSuite<T>::name() const {
    return m_name;
}

template <typename T>
inline const std::vector<std::string>& TestSuite<T>::tags() const {
    return m_tags;
}

template <typename T>
inline const std::vector<T>& TestSuite<T>::tests() const {
    return m_tests;
}
}   // namespace jump

#endif  // JUMP_TEST_HEIRARCHY_HPP

