// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#include "jump/testing/reporter.hpp"

#include "jump/utility/logging.hpp"

#include <iostream>
#include <format>

namespace jump {
TestReporter::TestReporter(const TestResult& results) :
    m_results{results},
    m_flattened_results{flatten(results)} {
}

void TestReporter::trace() const {
    trace(m_results);
}

void TestReporter::summarise() const {
    auto passes{std::format("{}/{} tests passed", m_flattened_results.passed,
            m_flattened_results.passed + m_flattened_results.failed)};
    auto skips{std::format("{} skipped", m_flattened_results.skipped)};

    std::clog << "Overall results for \"" << m_results.name << "\":\n";
    if (m_flattened_results.failed > 0) {
        std::clog << Log::red(passes);
    } else if (m_flattened_results.passed > 0) {
        std::clog << Log::green(passes);
    } else {
        std::clog << passes;
    }
    std::clog << ", ";
    if (m_flattened_results.skipped > 0) {
        std::clog << Log::yellow(skips);
    } else {
        std::clog << skips;
    }
    std::clog << '\n';

    if (m_flattened_results.failed_tests.size() > 0) {
        std::clog << "Failed tests:\n";
        for (const auto& fail : m_flattened_results.failed_tests) {
            std::clog << std::format("  \"{}{}{}\"\n", m_flattened_results.name, 
                    delimiter, fail);
        }
    }
    if (m_flattened_results.skipped_tests.size() > 0) {
        std::clog << "Skipped tests:\n";
        for (const auto& skip : m_flattened_results.skipped_tests) {
            std::clog << std::format("  \"{}{}{}\"\n", m_flattened_results.name,
                    delimiter, skip);
        }
    }
}

auto TestReporter::passed() const -> std::size_t {
    return m_flattened_results.passed;
}

auto TestReporter::skipped() const -> std::size_t {
    return m_flattened_results.skipped;
}

auto TestReporter::failed() const -> std::size_t {
    return m_flattened_results.failed;
}

void TestReporter::trace(const TestResult& results, std::string current) const {
    if (current == "") {
        current = results.name;
    } else {
        current = std::format("{}{}{}", current, delimiter, results.name);
    }

    if (results.has_info()) {
        auto passes{std::format("{}/{} tests passed", results.passed,
                results.passed + results.failed)};
        auto skips{std::format("{} skipped", results.skipped)};

        std::clog << '\"' << current << "\": ";
        if (results.failed > 0) {
            std::clog << Log::red(passes);
        } else if (results.passed > 0) {
            std::clog << Log::green(passes);
        } else {
            std::clog << passes;
        }
        std::clog << ", ";
        if (results.skipped > 0) {
            std::clog << Log::yellow(skips);
        } else {
            std::clog << skips;
        }
        std::clog << '\n';
    }
    for (const auto& sub : results.sub_results) {
        trace(sub, current);
    }
}

auto TestReporter::flatten(TestResult root) const -> TestResult {
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

