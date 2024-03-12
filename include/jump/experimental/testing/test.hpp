// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_HPP
#define JUMP_TEST_HPP

#include "jump/experimental/testing/test_decl.hpp"

namespace jump::experimental {
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

inline AtomicTest::AtomicTest(std::string_view name,
        std::vector<std::string> tags,
        const std::function<TestResult()>& func) :
    m_name{name},
    m_tags{std::move(tags)},
    m_func{func} {
    std::ranges::sort(m_tags);
}

inline TestResult AtomicTest::run() const {
    return m_func();
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

inline void Test::skip(const std::vector<std::string>& skip_tags) {
    for (const auto& tag : skip_tags) {
        if (std::ranges::find(m_skip, tag) == m_skip.end()) {
            m_skip.push_back(tag);
        }
    }
    std::ranges::sort(m_skip);
}

inline TestResult Test::run(const std::vector<std::string>& skip_tags) const {
    TestResult result;
    for (const auto& test: m_atomic_tests) {
        if (std::ranges::find(skip_tags, test.name()) != skip_tags.end()
                || has_intersection(test.tags(), skip_tags)) {
            result.append(TestResult::skip(test.name()));
            continue;
        }

        try {
            result.append(test.run(), test.name());
        } catch (std::exception& e) {
            result.append(TestResult::fail(test.name()));
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

inline TestSuite::TestSuite(std::string_view name) :
    m_name{name} {
}

inline void TestSuite::register_test(Test test) {
    m_tests.push_back(std::move(test));
}

inline void TestSuite::register_tests(std::vector<Test> tests) {
    for (auto& test : tests) {
        register_test(std::move(test));
    }
}

inline void TestSuite::skip(const std::vector<std::string>& skip_tags) {
    for (const auto& tag : skip_tags) {
        if (std::ranges::find(m_skip, tag) == m_skip.end()) {
            m_skip.push_back(tag);
        }
    }
    std::ranges::sort(m_skip);
}

inline void TestSuite::run() const {
    auto suite_title{std::format("Test suite \"{}\"", m_name)};
    std::clog << std::format("{}\n{}\n", suite_title,
            std::string(suite_title.size(), '-'));

    TestResult all_results;
    for (const auto& test : m_tests) {
        TestResult result;

        if (std::ranges::find(m_skip, test.name()) != m_skip.end()
                || has_intersection(test.tags(), m_skip)) {
            for (const auto& atomic_test : test.tests()) {
                result.append(TestResult::skip(atomic_test.name()));
            }
        } else {
            result = test.run(m_skip);
        }

        auto passes{std::format("{}/{} tests passed", result.passed,
                result.passed + result.failed)};
        auto skips{std::format("{} skipped", result.skipped)};

        if (result.failed > 0) {
            passes = Log::red(passes);
        } else if (result.passed > 0) {
            passes = Log::green(passes);
        }
        if (result.skipped > 0) {
            skips = Log::yellow(skips);
        }

        std::clog << std::format("Test \"{}\": {}, {}\n", test.name(), passes,
                skips);
        all_results.append(result, test.name());
    }

    auto passes{std::format("{}/{} tests passed", all_results.passed,
            all_results.passed + all_results.failed)};
    auto skips{std::format("{} skipped", all_results.skipped)};

    if (all_results.failed > 0) {
        passes = Log::red(passes);
    } else if (all_results.passed > 0) {
        passes = Log::green(passes);
    }
    if (all_results.skipped > 0) {
        skips = Log::yellow(skips);
    }

    std::clog << std::format("Overall results: {}, {}\n", passes, skips);
    if (all_results.failed_tests.size() > 0) {
        std::clog << Log::red("Failed tests:") << '\n';
        for (const auto& name : all_results.failed_tests) {
            std::clog << '\"' << name << "\"\n";
        }
    }
    if (all_results.skipped_tests.size() > 0) {
        std::clog << Log::yellow("Skipped tests:") << '\n';
        for (const auto& name : all_results.skipped_tests) {
            std::clog << '\"' << name << "\"\n";
        }
    }
}

inline const std::string& TestSuite::name() const {
    return m_name;
}

inline const std::vector<Test>& TestSuite::tests() const {
    return m_tests;
}
}   // namespace jump::experimental

#endif  // JUMP_TEST_HPP

