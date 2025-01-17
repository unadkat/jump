// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_TEST_REPORTER_HPP
#define JUMP_TEST_REPORTER_HPP

#include "jump/testing/test_result.hpp"

#include <string>

namespace jump {
class TestReporter {
    public:
        /// \brief Constructor for reporter object. Take the supplied test
        /// results, flatten them, and store the summaries.
        TestReporter(const TestResult& results);
        /// \brief Delimiter for output of test structure.
        std::string delimiter{"->"};

        /// \brief Trace results of each level of the test structure for which
        /// there are reported results, does not print failed and skipped tests.
        void trace() const;
        /// \brief Print summary of the total reported results in the tree, and
        /// give details of all failed and skipped tests.
        void summarise() const;

        /// \brief Return total number of passed tests.
        auto passed() const -> std::size_t;
        /// \brief Return total number of skipped tests.
        auto skipped() const -> std::size_t;
        /// \brief Return total number of failed tests.
        auto failed() const -> std::size_t;

    private:
        /// \brief Trace results of each level of the test structure for which
        /// there are reported results, does not print failed and skipped tests.
        void trace(const TestResult& results, std::string current = "") const;
        /// \brief Traverse result structure and collect all results in a
        /// top-level result object.
        auto flatten(TestResult root) const -> TestResult;

    private:
        TestResult m_results;
        TestResult m_flattened_results;
};
}   // namespace jump

#endif  // JUMP_TEST_REPORTER_HPP

