// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_TEST_HEIRARCHY_HPP
#define JUMP_TEST_HEIRARCHY_HPP

#include "jump/testing/test_result.hpp"
#include "jump/utility/utility.hpp"

#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace jump {
class AtomicTest {
    public:
        /// \brief Constructor for an atomic test requires name and tags to be
        /// supplied at the same time as the function, to keep all information
        /// local to the implementation.
        AtomicTest(std::string name, const std::function<TestResult()>& func,
                std::vector<std::string> tags = {});

        /// \brief Run the test case and return the result directly.
        auto run() const -> TestResult;

        /// \brief Accessor for the atomic test name.
        auto name() const -> const std::string&;
        /// \brief Accessor for the list of tags of this atomic test.
        auto tags() const -> const std::vector<std::string>&;

    private:
        /// \brief Name of the atomic test.
        std::string m_name;
        /// \brief List of tags for which this test can be skipped.
        std::vector<std::string> m_tags;
        /// \brief Handle to test function (accepting no arguments and returning
        /// a TestResult).
        std::function<TestResult()> m_func;
};

class Test {
    public:
        /// \brief Constructor for a test unit requires name and tags to be
        /// supplied at the outset, to keep all information local.
        Test(std::string name, std::vector<std::string> tags = {});

        /// \brief Register a single atomic test to be run with this test unit.
        void register_item(const AtomicTest& test);
        /// \brief Register a single atomic test to be run with this test unit.
        void register_item(AtomicTest&& test);
        /// \brief Register a collection of atomic tests to be run with this
        /// test unit.
        void register_items(const std::vector<AtomicTest>& tests);
        /// \brief Run registered tests, skipping any whose name or tags
        /// intersects with the supplied skip_tags list.
        auto run(const std::vector<std::string>& skip_tags = {}) const
                -> TestResult;

        /// \brief Accessor for the test unit name.
        auto name() const -> const std::string&;
        /// \brief Accessor for the list of tags for this test unit.
        auto tags() const -> const std::vector<std::string>&;
        /// \brief Accessor for the collection of registered atomic tests.
        auto tests() const -> const std::vector<AtomicTest>&;

    private:
        /// \brief Name of the test.
        std::string m_name;
        /// \brief List of tags for which this test (and hence all subtests)
        /// can be skipped.
        std::vector<std::string> m_tags;
        /// \brief List of atomic tests that will be run by this test unit.
        std::vector<AtomicTest> m_atomic_tests;
};

template <typename T>
class TestSuite {
    public:
        /// \brief Constructor for a test suite requires name to be supplied at
        /// the outset, to keep all information local.
        TestSuite(std::string name, std::vector<std::string> tags = {});
        /// \brief Constructor for a test suite requires name to be supplied at
        /// the outset, to keep all information local. Deduce test suite level
        /// by passed argument.
        TestSuite(std::string name, const T& test,
                std::vector<std::string> tags = {});
        /// \brief Constructor for a test suite requires name to be supplied at
        /// the outset, to keep all information local. Deduce test suite level
        /// by passed argument.
        TestSuite(std::string name, std::vector<T> tests,
                std::vector<std::string> tags = {});

        /// \brief Register a test unit to be run with this test suite.
        void register_item(const T& test);
        /// \brief Register a test unit to be run with this test suite.
        void register_item(T&& test);
        /// \brief Register a collection of test units to be run with this test
        /// suite.
        void register_items(const std::vector<T>& tests);
        /// \brief Run registered tests, skipping any whose name or tags
        /// intersects with the supplied skip_tags list. Forward this list on
        /// when running a valid test so that subtests can be further filtered.
        auto run(const std::vector<std::string>& skip_tags = {}) const
                -> TestResult;

        /// \brief Accessor for the test suite name.
        auto name() const -> const std::string&;
        /// \brief Accessor for the list of tags for this test suite.
        auto tags() const -> const std::vector<std::string>&;
        /// \brief Accessor for the collection of registered tests.
        auto tests() const -> const std::vector<T>&;

    private:
        /// \brief Name of the test suite.
        std::string m_name;
        /// \brief List of tags for which this test suite (and hence all
        /// subtests) can be skipped.
        std::vector<std::string> m_tags;
        /// \brief Collection of tests making up this test suite.
        std::vector<T> m_tests;
};

// ========================================================================
// Implementation
// ========================================================================

template <typename T>
inline TestSuite<T>::TestSuite(std::string name,
        std::vector<std::string> tags) :
    m_name{std::move(name)},
    m_tags{std::move(tags)} {
    std::ranges::sort(m_tags);
}

template <typename T>
inline TestSuite<T>::TestSuite(std::string name, const T& test,
        std::vector<std::string> tags) :
    m_name{std::move(name)},
    m_tags{std::move(tags)} {
    std::ranges::sort(m_tags);
    register_item(test);
}

template <typename T>
inline TestSuite<T>::TestSuite(std::string name, std::vector<T> tests,
        std::vector<std::string> tags) :
    m_name{std::move(name)},
    m_tags{std::move(tags)} {
    std::ranges::sort(m_tags);
    register_items(tests);
}

template <typename T>
inline void TestSuite<T>::register_item(const T& test) {
    m_tests.push_back(test);
}

template <typename T>
inline void TestSuite<T>::register_item(T&& test) {
    m_tests.push_back(std::move(test));
}

template <typename T>
inline void TestSuite<T>::register_items(const std::vector<T>& tests) {
    // TODO: std::vector::insert_range
    m_tests.insert(m_tests.end(), tests.begin(), tests.end());
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

