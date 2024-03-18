#include "jump/data/vector.hpp"
#include "jump/debug/exception.hpp"
#include "jump/testing/testing.hpp"
#include "jump/utility/random.hpp"

using namespace jump;

TestResult test_vector_arithmetic_basic();
TestResult test_vector_arithmetic_compound();
TestResult test_vector_norms();
TestResult test_vector_access_in_range();
TestResult test_vector_access_out_of_range();

inline TestSuiteL1 vector_tests() {
    TestSuiteL1 vector_suite("vector");
    std::vector<Test> tests;

    tests.push_back({"arithmetic"});
    tests.back().register_item({"basic", {},
            &test_vector_arithmetic_basic});
    tests.back().register_item({"compound", {},
            &test_vector_arithmetic_compound});

    tests.push_back({"summary"});
    tests.back().register_item({"norms", {},
            &test_vector_norms});

    tests.push_back({"access"});
    tests.back().register_item({"in range", {},
            &test_vector_access_in_range});

    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"out of range", {},
            &test_vector_access_out_of_range});
#endif  // NDEBUG

    vector_suite.register_items(tests);
    return vector_suite;
}

inline TestResult test_vector_arithmetic_basic() {
    TestResult result;
    RandomNumbers rng(0., 10.);

    Real ar, br;
    Complex az, bz;
    randomise(rng, ar, br, az, bz);

    Vector<Real> var(5, ar), vbr(5, br);
    Vector<Complex> vaz(5, az), vbz(5, bz);

    {
        Real ans_r{ar + br};
        Vector<Real> vans_r(5, ans_r), vcr{var + vbr};
        Complex ans_z{az + bz};
        Vector<Complex> vans_z(5, ans_z), vcz{vaz + vbz};

        result.add_check(vanishes((vans_r - vcr).L2_norm()), "add real");
        result.add_check(vanishes((vans_z - vcz).L2_norm()), "add complex");
    }
    {
        Real ans_r{ar - br};
        Vector<Real> vans_r(5, ans_r), vcr{var - vbr};
        Complex ans_z{az - bz};
        Vector<Complex> vans_z(5, ans_z), vcz{vaz - vbz};

        result.add_check(vanishes((vans_r - vcr).L2_norm()), "subtract real");
        result.add_check(vanishes((vans_z - vcz).L2_norm()),
                "subtract complex");
    }
    {
        Real ans_r{ar*br};
        Vector<Real> vans_r(5, ans_r), vcr{var*br};
        Complex ans_z{az*bz};
        Vector<Complex> vans_z(5, ans_z), vcz{vaz*bz};

        result.add_check(vanishes((vans_r - vcr).L2_norm()), "scale real");
        result.add_check(vanishes((vans_z - vcz).L2_norm()), "scale complex");
    }

    return result;
}

inline TestResult test_vector_arithmetic_compound() {
    TestResult result;
    RandomNumbers rng_real(0., 10.);
    RandomNumbers<int, std::uniform_int_distribution> rng_int(10, 15);

    auto size{static_cast<std::size_t>(rng_int.generate())};
    Vector<Real> ar(size), br(size), cr(size);
    Vector<Complex> az(size), bz(size), cz(size);
    Real kr1, kr2;
    Complex kz1, kz2;
    randomise(rng_real, ar, br, cr, az, bz, cz, kr1, kr2, kz1, kz2);

    while (vanishes(std::abs(kz2))) {
        randomise(rng_real, kz2);
    }

    {
        result.add_check(vanishes((ar + kr1*br)*(cr/kr2)
                    - (1./kr2)*(ar*cr + kr1*cr*br)), "real");
        result.add_check(vanishes(std::abs((az + kz1*bz)*(cz/kz2)
                    - (1./kz2)*(az*cz + kz1*cz*bz))), "complex");
    }

    return result;
}

inline TestResult test_vector_norms() {
    TestResult result;

    RandomNumbers<int, std::uniform_int_distribution> rng_int(10, 15);

    auto N{static_cast<std::size_t>(rng_int.generate())};
    Vector<Real> ar(N);
    Vector<Complex> az(N);

    for (std::size_t i{0}; i < N; ++i) {
    }

    return result;
}

inline TestResult test_vector_access_in_range() {
    TestResult result;

    Vector<Real> a(10);
    Real acc{0.};
    for (std::size_t i{0}, N{a.size()}; i < N; ++i) {
        a[i] = i + 1;
        acc += a[i];
    }

    result.add_check(approx_abs(acc, 55.), "all indices");

    return result;
}

inline TestResult test_vector_access_out_of_range() {
    TestResult result;
    bool exception_caught{false};
    Vector<Complex> b(10, {1., 2.});

    try {
        b[10] = {1., 0.};
    } catch (RuntimeError<Range1DError>& e) {
        exception_caught = true;
    }

    result.add_check(exception_caught, "just past the end");

    return result;
}

