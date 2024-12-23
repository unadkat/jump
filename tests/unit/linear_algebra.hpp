#ifndef JUMP_TESTS_LINEAR_ALGEBRA_HPP
#define JUMP_TESTS_LINEAR_ALGEBRA_HPP

#include <algorithm>
#include <vector>

#include "jump/data/banded_matrix.hpp"
#include "jump/data/dense_matrix.hpp"
#include "jump/data/eigendata.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/exception.hpp"
#include "jump/linear_algebra/banded_linear_system.hpp"
#include "jump/linear_algebra/dense_linear_system.hpp"
#include "jump/linear_algebra/generalised_eigenvalue_system.hpp"
#include "jump/testing/testing.hpp"
#include "jump/utility/random.hpp"

using namespace jump;

TestResult test_linalg_banded_basic();
TestResult test_linalg_banded_fail();
TestResult test_linalg_banded_mismatch();
TestResult test_linalg_dense_basic();
TestResult test_linalg_dense_fail();
TestResult test_linalg_dense_mismatch();
TestResult test_linalg_gev_basic();
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
    {
        // (10x10 (3 bands))
        Vector<Complex> A_data{
            {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0},
            {0.0, 0.0}, {-2.424, 7.971}, {5.279, -1.775}, {1.171, -5.161},
            {6.997, 0.272}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0},
            {0.0, 0.0}, {-8.783, -5.096}, {6.084, -9.95}, {5.819, -9.02},
            {-9.358, 8.99}, {-0.493, 0.194}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0},
            {0.0, 0.0}, {8.901, -6.441}, {0.477, 2.359}, {6.068, 4.514},
            {-7.427, -2.71}, {2.9, -0.55}, {0.298, 0.798}, {0.0, 0.0},
            {0.0, 0.0}, {0.0, 0.0}, {-4.913, 1.065}, {-1.349, -8.044},
            {-1.631, -8.383}, {9.561, -6.71}, {4.091, 3.716}, {-3.185, -6.63},
            {-1.791, 3.622}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0},
            {2.716, -5.985}, {-9.465, -5.117}, {9.497, -7.021}, {-6.084, 9.991},
            {-1.856, -5.76}, {1.482, -1.574}, {-9.848, -3.76}, {0.0, 0.0},
            {0.0, 0.0}, {0.0, 0.0}, {-8.823, -2.036}, {6.286, 1.523},
            {8.926, -9.943}, {9.486, 8.814}, {2.519, -6.318}, {3.351, -3.72},
            {5.642, -6.084}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.55, -3.734},
            {-4.773, -2.569}, {-0.68, 3.565}, {-5.868, -3.085}, {-0.446, 0.498},
            {-0.144, -1.032}, {7.827, -4.81}, {0.0, 0.0}, {0.0, 0.0},
            {0.0, 0.0}, {-5.966, -9.325}, {-9.425, -1.396}, {6.764, -8.999},
            {3.449, -8.212}, {6.737, -5.243}, {-2.707, -2.669}, {0.0, 0.0},
            {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {-0.771, -3.821},
            {9.595, 1.035}, {-5.564, 6.675}, {-2.572, -9.435}, {-1.744, 3.665},
            {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0},
            {-8.88, 4.927}, {-3.889, 1.257}, {-1.217, 7.054}, {0.117, -6.785},
            {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}
        };
        Vector<Complex> b{
            {-4.771, -7.222}, {-4.947, 8.768}, {-9.801, -4.304},
            {1.306, -0.732}, {-5.299, 5.63}, {-5.029, 1.341}, {-0.774, -1.298},
            {2.941, 7.543}, {-9.507, -0.883}, {-7.157, 3.414}
        };
        Vector<Complex> x{
            {-1.1746694584364086, -4.025705808822034},
            {-2.3040181695815916, 1.6647290623587294},
            {-5.127389007415918, -3.765468450598305},
            {-0.46380469383373185, 0.7637101127491459},
            {-1.3036523859362064, -0.6369109544105513},
            {-0.7917224045410776, 0.8025994730971937},
            {-1.4076525049199973, -0.7782005744745053},
            {0.1835433167023734, -0.35053678402750654},
            {0.43139622898096003, 0.4206447812947137},
            {-0.18156723754128787, 1.6730341735732026}
        };

        BandedMatrix<Complex> A{10, 3, std::move(A_data)};
        Vector<Complex> sol{b};

        BandedLinearSystem<Complex> system(A, sol);
        system.solve();

        result.add_check(approx(sol, x), "complex");
    }

    return result;
}

inline TestResult test_linalg_banded_fail() {
    TestResult result;
    RandomReal rng_real(0., 10.);
    std::size_t N{12}, bands{3};

    {
        bool real_caught{false}, complex_caught{false};

        BandedMatrix<Real> Ar{N, bands};
        Vector<Real> br(N);
        BandedMatrix<Complex> Az{N, bands};
        Vector<Complex> bz(N);
        randomise(rng_real, Ar, br, Az, bz);

        // Prepare iterators for deletion of data from col 6, creating a linear
        // dependency and rank reduction
        std::size_t clear_begin{6*(3*bands + 1)};
        std::size_t clear_end{7*(3*bands + 1)};
        std::transform(Ar.begin() + clear_begin, Ar.begin() + clear_end,
                Ar.begin() + clear_begin, [] (const auto&) { return 0.; });
        std::transform(Az.begin() + clear_begin, Az.begin() + clear_end,
                Az.begin() + clear_begin, [] (const auto&) { return 0.; });

        BandedLinearSystem system_real{Ar, br};
        BandedLinearSystem system_complex{Az, bz};

        try {
            system_real.solve();
        } catch (RuntimeError<BasicError>& e) {
            real_caught = true;
        }
        try {
            system_complex.solve();
        } catch (RuntimeError<BasicError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "real");
        result.add_check(complex_caught, "complex");
    }

    return result;
}

inline TestResult test_linalg_banded_mismatch() {
    TestResult result;

    return result;
}

inline TestResult test_linalg_dense_basic() {
    TestResult result;

    // Test cases generated and verified externally using Python and numpy
    {
        // (10x10)
        Vector<Real> A_data{
             5.883, -7.997,  3.023,  4.581,  8.329,  5.466,  4.869,  7.464,
             9.347,  5.760, -0.634, -7.424, -6.453,  8.188,  1.255, -8.800,
             4.032, -0.067,  5.067,  2.729, -0.102,  4.680,  8.740, -1.733,
             0.699,  4.487, -4.214,  9.791,  0.492, -8.700, -4.022, -6.449,
            -8.681, -4.100, -9.647, -4.794,  1.243,  8.566,  4.955,  6.771,
            -4.302,  8.305, -4.961, -3.234,  1.625,  9.683,  3.238, -3.359,
            -7.859, -4.532, -4.003, -3.718, -2.115,  1.850,  3.143,  0.242,
            -0.434, -5.779, -7.345,  3.013,  6.060, -2.670, -3.942,  1.271,
             3.739,  2.997, -8.008,  5.808,  9.681, -5.248, -5.678,  1.801,
            -0.083,  4.597,  8.060,  0.009,  3.800, -8.019,  3.429,  6.572,
            -4.172,  0.171,  8.301, -6.149,  6.367, -4.138, -5.938, -4.382,
            -1.094,  5.593, -4.504,  2.949, -7.843,  0.345, -9.695, -4.591,
             7.157,  8.229,  8.892,  9.720
        };
        Vector<Real> b{
            -3.318, -8.768,  9.125,  7.484,  8.164,  0.410,  4.002,  4.034,
             8.828, -9.745
        };
        Vector<Real> x{
            0.045969070850807904, 0.6378417649226045, 1.40393226005839,
            2.0578218422737327, -0.3179534198751538, -1.7420249975643702,
            -0.8861390426044499, 2.295590884481307, -1.060615291643999,
            -2.4143062036573104
        };

        DenseMatrix<Real> A{10, 10, std::move(A_data)};
        Vector<Real> sol{b};

        DenseLinearSystem<Real> system(A, sol);
        system.solve();

        result.add_check(approx(sol, x), "real");
    }
    {
        // (10x10)
        Vector<Complex> A_data{
            { 3.946, -0.910}, {-6.282, -2.645}, {-5.016,  7.005},
            {-1.454, -2.051}, {-1.707, -2.229}, {-9.213, -9.653},
            {-8.254,  1.550}, { 8.393,  0.571}, {-5.449,  1.432},
            { 0.564, -3.780}, {-0.803, -8.932}, { 2.787,  1.405},
            { 7.263,  2.191}, {-5.928,  9.888}, { 0.986,  7.245},
            {-1.241, -7.178}, { 1.816, -8.986}, {-2.782,  8.513},
            {-4.830,  5.654}, { 8.035, -8.725}, { 9.760, -6.559},
            {-7.587,  5.515}, {-0.669,  6.285}, { 7.179, -5.408},
            { 1.122, -4.494}, { 2.094, -6.119}, { 6.735,  3.240},
            {-7.730, -7.386}, {-0.718, -7.324}, { 1.326,  6.355},
            { 8.602,  1.977}, {-2.288,  2.272}, { 7.565, -9.779},
            {-1.126,  1.307}, {-1.025,  3.965}, {-9.766, -4.999},
            {-5.137,  2.517}, {-5.964, -3.045}, {-5.596, -4.512},
            {-2.090,  8.449}, {-2.048, -9.829}, {-5.120,  5.062},
            { 2.912,  3.102}, {-4.490, -8.245}, { 3.955,  3.960},
            {-9.281,  3.129}, { 9.702,  7.151}, {-8.773,  7.451},
            {-8.214,  6.759}, { 3.707, -7.097}, {-9.224, -7.932},
            { 6.329, -4.454}, {-7.820, -5.185}, {-7.200, -2.242},
            {-8.416,  8.926}, {-8.832, -5.710}, {-3.618, -7.696},
            {-5.254, -1.588}, { 6.728,  1.193}, {-0.873, -2.100},
            {-7.670, -3.775}, {-3.518, -0.280}, {-3.838,  2.548},
            {-5.954,  8.559}, {-7.945,  6.077}, { 8.591,  7.261},
            {-8.806,  9.357}, { 4.871, -8.088}, {-1.884,  1.442},
            { 8.984, -7.257}, {-1.670,  7.750}, {-8.396, -1.280},
            {-1.624,  1.121}, { 7.887, -7.612}, { 4.385, -1.987},
            { 3.918,  7.734}, {-9.471,  1.608}, {-5.138, -7.139},
            { 2.865, -5.969}, { 9.086, -3.554}, { 1.157, -9.261},
            {-5.192,  5.821}, {-5.073,  4.588}, {-2.839, -7.442},
            { 2.803, -1.146}, {-2.465, -5.512}, { 8.611, -3.195},
            {-4.734, -6.388}, { 8.477,  6.839}, {-7.923,  7.063},
            {-2.077, -1.993}, {-3.664,  1.183}, { 4.150, -0.542},
            {-5.596,  3.056}, { 9.552,  0.289}, {-5.647,  2.310},
            {-3.799, -3.703}, {-6.212,  2.972}, {-3.450, -6.414},
            {-3.273, -8.187}
        };
        Vector<Complex> b{
            { 5.372, -2.508}, { 8.429, -0.207}, {-9.093,  4.204},
            { 4.105,  4.316}, {-3.667, -1.195}, {-5.257,  5.173},
            { 5.843, -5.423}, {-1.833,  4.895}, { 0.554,  1.033},
            {-4.470, -4.966}
        };
        Vector<Complex> x{
            {0.24686732213037405, -1.2791165172232808},
            {0.730463350184863, 0.5351462304809256},
            {1.1912312903362252, -0.005666604514519784},
            {-0.7886206316386909, -0.625005320208439},
            {-0.9755388219069605, -0.6053311837801041},
            {1.28028466010758, -0.5049211286752451},
            {-1.1228580619280542, -0.019085592958605572},
            {0.2777721213492245, -0.31062110380241537},
            {-1.0728368442529181, 0.6998937946431544},
            {-0.17623319545745977, -0.5000444307192732}
        };

        DenseMatrix<Complex> A{10, 10, std::move(A_data)};
        Vector<Complex> sol{b};

        DenseLinearSystem<Complex> system(A, sol);
        system.solve();

        result.add_check(approx(sol, x), "complex");
    }

    return result;
}

inline TestResult test_linalg_dense_fail() {
    TestResult result;
    RandomReal rng_real(0., 10.);
    std::size_t N{12};

    {
        bool real_caught{false}, complex_caught{false};

        DenseMatrix<Real> Ar{N};
        Vector<Real> br(N);
        DenseMatrix<Complex> Az{N};
        Vector<Complex> bz(N);
        randomise(rng_real, Ar, br, Az, bz);

        // Prepare iterators for deletion of data from col 6, creating a linear
        // dependency and rank reduction
        auto col6_real{Ar.column_iterators(6)};
        auto col6_complex{Az.column_iterators(6)};
        std::transform(col6_real.first, col6_real.second, col6_real.first,
                [] (const auto&) { return 0.; });
        std::transform(col6_complex.first, col6_complex.second,
                col6_complex.first, [] (const auto&) { return 0.; });


        DenseLinearSystem system_real{Ar, br};
        DenseLinearSystem system_complex{Az, bz};

        try {
            system_real.solve();
        } catch (RuntimeError<BasicError>& e) {
            real_caught = true;
        }
        try {
            system_complex.solve();
        } catch (RuntimeError<BasicError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "real");
        result.add_check(complex_caught, "complex");
    }

    return result;
}

inline TestResult test_linalg_dense_mismatch() {
    TestResult result;

    return result;
}

inline TestResult test_linalg_gev_basic() {
    TestResult result;

    // Test cases generated and verified externally using Python and numpy
    {
        // (10x10)
        Vector<Complex> A_data{
            { 6.684, -9.075}, {-7.877,  0.269}, { 0.299, -2.762},
            { 1.021,  3.522}, {-0.151,  0.782}, { 8.869, -9.394},
            { 8.693,  4.417}, {-6.758, -4.278}, { 9.798, -1.987},
            { 6.234,  1.858}, { 3.193,  0.407}, { 7.020, -9.146},
            { 8.142,  0.930}, {-5.702, -6.921}, { 3.099, -6.279},
            {-6.371, -5.231}, {-9.490,  6.971}, { 2.295, -3.501},
            {-4.515,  8.453}, {-4.664, -9.280}, { 8.022, -3.645},
            {-0.220, -4.772}, {-1.183, -8.486}, {-4.084,  7.969},
            { 8.746,  2.795}
        };
        Vector<Complex> B_data{
            {-0.624,  8.057}, {-2.750, -3.001}, {-3.853,  6.668},
            {-2.043,  7.318}, {-0.526, -8.618}, {-9.838,  3.332},
            { 1.852, -5.561}, {-2.390, -9.295}, { 8.354,  6.235},
            {-1.213, -3.386}, { 0.589, -6.067}, { 0.924,  4.104},
            {-9.816, -5.595}, { 7.122,  2.228}, { 2.252, -5.026},
            { 7.205, -4.108}, {-4.969,  0.295}, {-9.619,  0.617},
            { 5.708, -0.043}, {-1.483, -2.049}, { 2.096, -3.881},
            {-0.378,  4.087}, {-6.486,  9.120}, { 6.059,  8.412},
            { 4.772, -8.011}
        };
        std::vector<Complex> values{
            {-0.6656833936578743, -3.2225152352732156},
            {-0.02119474758392895, 1.1731323081752432},
            {-0.7323697131287985, -0.4236896849240123},
            {-0.08103925491418665, -0.34533302092990986},
            {0.9850526019357968, 0.3496551373690341}
        };
        std::vector<Vector<Complex>> vectors{
            {{-0.28645884721663056, -0.3808333697651603},
            {-0.06663235895160867, -0.12081126447592545},
            {0.3284458359553031, -0.4104653677236301},
            {-0.6115884257197913, 0.18039111661113455},
            {0.17230679925170905, 0.20308265482073498}},
            {{0.38102137399861147, 0.04024826135289268},
            {-0.4367706042573442, -0.06920968618105494},
            {0.14755806510021896, 0.42782544246397586},
            {-0.30251165326378204, -0.012163292752162153},
            {-0.4836008088368512, -0.35679863852800575}},
            {{0.7335652832116764, -0.4473192091900682},
            {0.14905849615934985, -0.08738111525003661},
            {0.2014030805593339, 0.11644623729975384},
            {-0.18444617014372972, -0.0734998825480814},
            {-0.21966488614667556, 0.30022557205433814}},
            {{0.39812934047473214, 0.5501340692489866},
            {-0.027039426868247198, -0.28550365865038163},
            {-0.06798454544223413, 0.49640529439696496},
            {0.3958735807512513, -0.21063896827426162},
            {0.0063370074113579975, 0.06661174909030658}},
            {{0.33182457366504814, -0.2407290022852715},
            {0.04126277243782262, 0.028579783850193594},
            {0.4986960909144866, -0.24240124204247882},
            {-0.12988026468326308, -0.33942294768542774},
            {-0.5852501267478669, -0.21765085497508993}}
        };
        std::vector<std::size_t> sorted{2, 0, 3, 1, 4};

        DenseMatrix<Complex> A{5, 5, std::move(A_data)};
        DenseMatrix<Complex> B{5, 5, std::move(B_data)};
        std::vector<Eigendatum<Complex>> eigendata;

        GeneralisedEigenvalueSystem<Complex> system(A, B, eigendata);
        system.solve();
        std::ranges::sort(eigendata, sort_eigendata_real,
                &Eigendatum<Complex>::value);

        bool solutions_match{true};
        for (std::size_t i{0}; i < 5; ++i) {
            if (!(approx(eigendata[i].value, values[sorted[i]])
                        && approx(eigendata[i].vector, vectors[sorted[i]]))) {
                solutions_match = false;
            }
        }

        result.add_check(solutions_match, "complex");
    }

    return result;
}

inline TestResult test_linalg_gev_mismatch() {
    TestResult result;

    return result;
}

#endif  // JUMP_TESTS_LINEAR_ALGEBRA_HPP

