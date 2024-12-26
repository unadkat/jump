// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_HEIRARCHY_HPP
#define JUMP_TEST_HEIRARCHY_HPP

#include "jump/testing/heirarchy_decl.hpp"

#include "jump/utility/utility.hpp"

#include <algorithm>
#include <utility>

namespace jump {
inline AtomicTest::AtomicTest(std::string_view name,
        const std::function<TestResult()>& func,
        std::vector<std::string> tags) :
    m_name{name},
    m_tags{std::move(tags)},
    m_func{func} {
    std::ranges::sort(m_tags);
}

inline auto AtomicTest::run() const -> TestResult {
    TestResult result{m_func()};
    result.name = name();
    return result;
}

inline auto AtomicTest::name() const -> const std::string& {
    return m_name;
}

inline auto AtomicTest::tags() const -> const std::vector<std::string>& {
    return m_tags;
}

inline Test::Test(std::string_view name, std::vector<std::string> tags) :
    m_name{name},
    m_tags{std::move(tags)} {
    std::ranges::sort(m_tags);
}

inline void Test::register_item(AtomicTest test) {
    m_atomic_tests.push_back(std::move(test));
}

inline void Test::register_items(std::vector<AtomicTest> tests) {
    for (auto& test: tests) {
        register_item(std::move(test));
    }
}

inline auto Test::run(const std::vector<std::string>& skip_tags) const
        -> TestResult {
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

inline auto Test::name() const -> const std::string& {
    return m_name;
}

inline auto Test::tags() const -> const std::vector<std::string>& {
    return m_tags;
}

inline auto Test::tests() const -> const std::vector<AtomicTest>& {
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
inline TestSuite<T>::TestSuite(std::string_view name, T test,
        std::vector<std::string> tags) :
    m_name{name},
    m_tags{std::move(tags)} {
    std::ranges::sort(m_tags);
    register_item(test);
}

template <typename T>
inline TestSuite<T>::TestSuite(std::string_view name, std::vector<T> tests,
        std::vector<std::string> tags) :
    m_name{name},
    m_tags{std::move(tags)} {
    std::ranges::sort(m_tags);
    register_items(tests);
}

template <typename T>
inline void TestSuite<T>::register_item(T test) {
    m_tests.push_back(std::move(test));
}

template <typename T>
inline void TestSuite<T>::register_items(std::vector<T> tests) {
    for (auto& test : tests) {
        register_item(std::move(test));
    }
}

template <typename T>
inline auto TestSuite<T>::run(const std::vector<std::string>& skip_tags) const
        -> TestResult {
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
inline auto TestSuite<T>::name() const -> const std::string& {
    return m_name;
}

template <typename T>
inline auto TestSuite<T>::tags() const -> const std::vector<std::string>& {
    return m_tags;
}

template <typename T>
inline auto TestSuite<T>::tests() const -> const std::vector<T>& {
    return m_tests;
}
}   // namespace jump

#endif  // JUMP_TEST_HEIRARCHY_HPP

