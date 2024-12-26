// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_HEIRARCHY_DECL_HPP
#define JUMP_TEST_HEIRARCHY_DECL_HPP

#include "jump/testing/test_result.hpp"

#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace jump {
class AtomicTest {
    public:
        /// \brief Constructor for an atomic test requires name and tags to be
        /// supplied at the same time as the function, to keep all information
        /// local to the implementation.
        AtomicTest(std::string_view name,
                const std::function<TestResult()>& func,
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
        Test(std::string_view name, std::vector<std::string> tags = {});

        /// \brief Register a single atomic test to be run with this test unit.
        void register_item(AtomicTest test);
        /// \brief Register a collection of atomic tests to be run with this
        /// test unit.
        void register_items(std::vector<AtomicTest> tests);
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
        TestSuite(std::string_view name, std::vector<std::string> tags = {});
        /// \brief Constructor for a test suite requires name to be supplied at
        /// the outset, to keep all information local. Deduce test suite level
        /// by passed argument.
        TestSuite(std::string_view name, T test,
                std::vector<std::string> tags = {});
        /// \brief Constructor for a test suite requires name to be supplied at
        /// the outset, to keep all information local. Deduce test suite level
        /// by passed argument.
        TestSuite(std::string_view name, std::vector<T> tests,
                std::vector<std::string> tags = {});

        /// \brief Register a test unit to be run with this test suite.
        void register_item(T test);
        /// \brief Register a collection of test units to be run with this test
        /// suite.
        void register_items(std::vector<T> tests);
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
}   // namespace jump

#endif  // JUMP_TEST_HEIRARCHY_DECL_HPP

