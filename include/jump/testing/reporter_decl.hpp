// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_REPORTER_DECL_HPP
#define JUMP_TEST_REPORTER_DECL_HPP

#include <iostream>

#include "jump/testing/test_result.hpp"
#include "jump/utility/logging.hpp"

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
        void summarise(const TestResult& results) const;

    private:
        /// \brief Traverse result structure and collect all results in a
        /// top-level result object.
        TestResult flatten(TestResult root) const;
};
}   // namespace jump

#endif  // JUMP_TEST_REPORTER_DECL_HPP

