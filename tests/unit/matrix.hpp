#include "jump/data/banded_matrix.hpp"
#include "jump/data/dense_matrix.hpp"
#include "jump/testing/testing.hpp"
#include "jump/utility/random.hpp"

using namespace jump;

TestResult test_matrix_initialise_basic();
TestResult test_matrix_initialise_chained();
TestResult test_matrix_initialise_fail();
TestResult test_matrix_multiply();
TestResult test_matrix_norms();
TestResult test_matrix_access_in_range();
TestResult test_matrix_access_fail();

inline TestSuiteL1 matrix_tests() {
    TestSuiteL1 matrix_suite("matrix");
    std::vector<Test> tests;

    tests.push_back({"initialise"});
    tests.back().register_item({"basic", {}, &test_matrix_initialise_basic});
    tests.back().register_item({"chained", {},
            &test_matrix_initialise_chained});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"fail", {}, &test_matrix_initialise_fail});
#endif  // NDEBUG

    tests.push_back({"arithmetic"});
    tests.back().register_item({"multiply", {}, &test_matrix_multiply});

    tests.push_back({"summary"});
    tests.back().register_item({"norms", {}, &test_matrix_norms});

    tests.push_back({"access"});
    tests.back().register_item({"in range", {}, &test_matrix_access_in_range});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"fail", {}, &test_matrix_access_fail});
#endif  // NDEBUG

    matrix_suite.register_items(tests);
    return matrix_suite;
}

inline TestResult test_matrix_initialise_basic() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_initialise_chained() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_initialise_fail() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_multiply() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_norms() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_access_in_range() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_access_fail() {
    TestResult result;

    return result;
}


