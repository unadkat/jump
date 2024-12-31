// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#include "jump/testing/test_suite.hpp"

#include <exception>

namespace jump {
AtomicTest::AtomicTest(std::string_view name,
        const std::function<TestResult()>& func,
        std::vector<std::string> tags) :
    m_name{name},
    m_tags{std::move(tags)},
    m_func{func} {
    std::ranges::sort(m_tags);
}

auto AtomicTest::run() const -> TestResult {
    TestResult result{m_func()};
    result.name = name();
    return result;
}

auto AtomicTest::name() const -> const std::string& {
    return m_name;
}

auto AtomicTest::tags() const -> const std::vector<std::string>& {
    return m_tags;
}

Test::Test(std::string_view name, std::vector<std::string> tags) :
    m_name{name},
    m_tags{std::move(tags)} {
    std::ranges::sort(m_tags);
}

void Test::register_item(AtomicTest test) {
    m_atomic_tests.push_back(std::move(test));
}

void Test::register_items(std::vector<AtomicTest> tests) {
    for (auto& test: tests) {
        register_item(std::move(test));
    }
}

auto Test::run(const std::vector<std::string>& skip_tags) const
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

auto Test::name() const -> const std::string& {
    return m_name;
}

auto Test::tags() const -> const std::vector<std::string>& {
    return m_tags;
}

auto Test::tests() const -> const std::vector<AtomicTest>& {
    return m_atomic_tests;
}
}   // namespace jump

