// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_TEST_REPORTER_HPP
#define JUMP_TEST_REPORTER_HPP

#include "jump/testing/test_result.hpp"

#include <string>

namespace jump {
class TestReporter {
    public:
        /// \brief Delimiter for output of test structure.
        std::string delimiter{"->"};

        /// \brief Trace results of each level of the test structure for which
        /// there are reported results, does not print failed and skipped tests.
        void trace(const TestResult& results, std::string current = "") const;
        /// \brief Print summary of the total reported results in the tree, and
        /// give details of all failed and skipped tests.
        auto summarise(const TestResult& results) const -> int;

    private:
        /// \brief Traverse result structure and collect all results in a
        /// top-level result object.
        auto flatten(TestResult root) const -> TestResult;
};
}   // namespace jump

#endif  // JUMP_TEST_REPORTER_HPP

