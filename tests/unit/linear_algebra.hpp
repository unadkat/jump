#ifndef JUMP_TESTS_LINEAR_ALGEBRA_HPP
#define JUMP_TESTS_LINEAR_ALGEBRA_HPP

#include "jump/data/banded_matrix.hpp"
#include "jump/data/dense_matrix.hpp"
#include "jump/data/eigendata.hpp"
#include "jump/data/vector.hpp"
#include "jump/linear_algebra/banded_linear_system.hpp"
#include "jump/linear_algebra/dense_linear_system.hpp"
#include "jump/linear_algebra/generalised_eigenvalue_system.hpp"
#include "jump/testing/testing.hpp"

using namespace jump;

TestResult test_linalg_banded_basic();
TestResult test_linalg_banded_fail();
TestResult test_linalg_banded_mismatch();
TestResult test_linalg_dense_basic();
TestResult test_linalg_dense_fail();
TestResult test_linalg_dense_mismatch();
TestResult test_linalg_gev_basic();
TestResult test_linalg_gev_fail();
TestResult test_linalg_gev_mismatch();

inline TestSuiteL1 linear_algebra_tests() {
    TestSuiteL1 linear_algebra_suite("linear algebra");
    std::vector<Test> tests;

    tests.push_back({"banded"});
    tests.back().register_item({"basic", {}, &test_linalg_banded_basic});
    tests.back().register_item({"fail", {}, &test_linalg_banded_fail});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"mismatch", {}, &test_linalg_banded_mismatch});
#endif  // NDEBUG

    tests.push_back({"dense"});
    tests.back().register_item({"basic", {}, &test_linalg_dense_basic});
    tests.back().register_item({"fail", {}, &test_linalg_dense_fail});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"mismatch", {}, &test_linalg_dense_mismatch});
#endif  // NDEBUG

    tests.push_back({"generalised eigenvalue"});
    tests.back().register_item({"basic", {}, &test_linalg_gev_basic});
    tests.back().register_item({"fail", {}, &test_linalg_gev_fail});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"mismatch", {}, &test_linalg_gev_mismatch});
#endif  // NDEBUG

    linear_algebra_suite.register_items(tests);
    return linear_algebra_suite;
}

inline TestResult test_linalg_banded_basic() {
    TestResult result;

    // Test cases generated and verified externally using Python and numpy
    {
        // (10x10 (3 bands))
        Vector<Real> A_data{
             0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  8.784, -3.831,
             7.144, -3.351,  0.000,  0.000,  0.000,  0.000,  0.000,  5.653,
             6.611,  2.769, -2.875,  3.646,  0.000,  0.000,  0.000,  0.000,
            -6.451, -5.270, -8.073,  4.240,  2.575, -6.563,  0.000,  0.000,
             0.000,  9.203, -4.255, -1.877,  4.699, -8.807,  9.592, -8.102,
             0.000,  0.000,  0.000, -8.985,  6.931,  1.815,  4.729,  9.221,
             5.752, -0.045,  0.000,  0.000,  0.000,  5.564, -8.881,  5.540,
             1.680,  6.103,  5.290, -8.463,  0.000,  0.000,  0.000, -5.757,
             0.027, -4.204,  8.975, -1.128, -4.368, -1.634,  0.000,  0.000,
             0.000,  2.089, -0.853,  6.256,  8.063, -5.831, -6.778,  0.000,
             0.000,  0.000,  0.000, -1.656,  1.885,  9.264,  9.966, -6.294,
             0.000,  0.000,  0.000,  0.000,  0.000, -6.457, -3.144, -7.411,
            -0.278,  0.000,  0.000,  0.000
        };
        Vector<Real> b{
            3.189, -7.333, -7.249,  7.225, -8.348,  5.241, -1.684,  1.056,
            9.548,  2.904
        };
        Vector<Real> x{
            -0.039746338200725956, -0.2622089423613688, 0.50830778269269,
            0.9018246758746646, -0.08505744095060677, 0.02638949874696343,
            -0.05798237876420149, -0.23978194194929575, -0.1318901419840183,
            -1.2730152407178386
        };

        BandedMatrix<Real> A{10, 3, std::move(A_data)};
        Vector<Real> sol{b};

        BandedLinearSystem<Real> system(A, sol);
        system.solve();

        result.add_check(approx(sol, x), "real");
    }

        return result;
}

inline TestResult test_linalg_banded_fail() {
    TestResult result;

    return result;
}

inline TestResult test_linalg_banded_mismatch() {
    TestResult result;

    return result;
}

inline TestResult test_linalg_dense_basic() {
    TestResult result;

    // Test cases generated and verified externally using Python and numpy
    return result;
}

inline TestResult test_linalg_dense_fail() {
    TestResult result;

    return result;
}

inline TestResult test_linalg_dense_mismatch() {
    TestResult result;

    return result;
}

inline TestResult test_linalg_gev_basic() {
    TestResult result;

    return result;
}

inline TestResult test_linalg_gev_fail() {
    TestResult result;

    // Test cases generated and verified externally using Python and numpy
    return result;
}

inline TestResult test_linalg_gev_mismatch() {
    TestResult result;

    return result;
}

#endif  // JUMP_TESTS_LINEAR_ALGEBRA_HPP

