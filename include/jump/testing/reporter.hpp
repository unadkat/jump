// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TEST_REPORTER_HPP
#define JUMP_TEST_REPORTER_HPP

#include "jump/testing/reporter_decl.hpp"

#include "jump/utility/logging.hpp"

#include <iostream>

namespace jump {
inline void TestReporter::trace(const TestResult& results,
        std::string current) const {
    if (current == "") {
        current = results.name;
    } else {
        current = std::format("{}{}{}", current, delimiter, results.name);
    }

    if (results.has_info()) {
        auto passes{std::format("{}/{} tests passed", results.passed,
                results.passed + results.failed)};
        auto skips{std::format("{} skipped", results.skipped)};

        if (results.failed > 0) {
            passes = Log::red(passes);
        } else if (results.passed > 0) {
            passes = Log::green(passes);
        }
        if (results.skipped > 0) {
            skips = Log::yellow(skips);
        }

        std::clog << std::format("\"{}\": {}, {}\n", current, passes, skips);
    }
    for (const auto& sub : results.sub_results) {
        trace(sub, current);
    }
}

inline void TestReporter::summarise(const TestResult& results) const {
    auto flat{flatten(results)};

    auto passes{std::format("{}/{} tests passed", flat.passed,
            flat.passed + flat.failed)};
    auto skips{std::format("{} skipped", flat.skipped)};

    if (flat.failed > 0) {
        passes = Log::red(passes);
    } else if (flat.passed > 0) {
        passes = Log::green(passes);
    }
    if (flat.skipped > 0) {
        skips = Log::yellow(skips);
    }

    std::clog << std::format("Overall results for \"{}\":\n{}, {}\n",
            results.name, passes, skips);
    if (flat.failed_tests.size() > 0) {
        std::clog << "Failed tests:\n";
        for (const auto& fail : flat.failed_tests) {
            std::clog << std::format("  \"{}{}{}\"\n", flat.name, delimiter,
                    fail);
        }
    }
    if (flat.skipped_tests.size() > 0) {
        std::clog << "Skipped tests:\n";
        for (const auto& skip : flat.skipped_tests) {
            std::clog << std::format("  \"{}{}{}\"\n", flat.name, delimiter,
                    skip);
        }
    }
}

inline auto TestReporter::flatten(TestResult root) const -> TestResult {
    for (const auto& sub : root.sub_results) {
        auto flat_sub = flatten(sub);

        root.passed += flat_sub.passed;
        root.failed += flat_sub.failed;
        root.skipped += flat_sub.skipped;
        for (const auto& fail : flat_sub.failed_tests) {
            root.failed_tests.push_back(std::format("{}{}{}", flat_sub.name,
                        delimiter, fail));
        }
        for (const auto& skip : flat_sub.skipped_tests) {
            root.skipped_tests.push_back(std::format("{}{}{}", flat_sub.name,
                        delimiter, skip));
        }
    }
    root.sub_results.clear();

    return root;
}
}   // namespace jump

#endif  // JUMP_TEST_REPORTER_HPP

