#include "jump/data/vector.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/random.hpp"
#include "jump/experimental/testing/test.hpp"

using namespace jump;
using namespace jump::experimental;

constexpr double epsilon{1e-12};

TestResult test_vector_arithmetic_basic();
TestResult test_vector_arithmetic_compound();
TestResult test_vector_norms();
TestResult test_vector_access_in_range();
TestResult test_vector_access_out_of_range();

inline std::vector<Test> vector_tests() {
    std::vector<Test> tests;

    tests.push_back({{"vector arithmetic"},
            {"vector", "data", "arithmetic"}});
    tests.back().register_atomic_test({"basic", {},
            &test_vector_arithmetic_basic});
    tests.back().register_atomic_test({"compound", {},
            &test_vector_arithmetic_compound});

    tests.push_back({{"vector summary"},
            {"vector", "data", "summary"}});
    tests.back().register_atomic_test({"norms", {},
            &test_vector_norms});

    tests.push_back({{"vector access"},
            {"vector", "data", "access"}});
    tests.back().register_atomic_test({"in range", {},
            &test_vector_access_in_range});

    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_atomic_test({"out of range", {},
            &test_vector_access_out_of_range});
#endif  // NDEBUG

    return tests;
}

inline TestResult test_vector_arithmetic_basic() {
    TestResult result;
    RandomNumbers rng(0., 10.);

    Real ar{rng.generate()}, br{rng.generate()};
    Complex az{rng.generate(), rng.generate()},
            bz{rng.generate(), rng.generate()};

    Vector<Real> var(5, ar), vbr(5, br);
    Vector<Complex> vaz(5, az), vbz(5, bz);

    {
        Real ans_r{ar + br};
        Vector<Real> vans_r(5, ans_r), vcr{var + vbr};
        Complex ans_z{az + bz};
        Vector<Complex> vans_z(5, ans_z), vcz{vaz + vbz};

        result.add_check((vans_r - vcr).L2_norm() < epsilon, "add real");
        result.add_check((vans_z - vcz).L2_norm() < epsilon, "add complex");
    }
    {
        Real ans_r{ar - br};
        Vector<Real> vans_r(5, ans_r), vcr{var - vbr};
        Complex ans_z{az - bz};
        Vector<Complex> vans_z(5, ans_z), vcz{vaz - vbz};

        result.add_check((vans_r - vcr).L2_norm() < epsilon, "subtract real");
        result.add_check((vans_z - vcz).L2_norm() < epsilon,
                "subtract complex");
    }
    {
        Real ans_r{ar*br};
        Vector<Real> vans_r(5, ans_r), vcr{var*br};
        Complex ans_z{az*bz};
        Vector<Complex> vans_z(5, ans_z), vcz{vaz*bz};

        result.add_check((vans_r - vcr).L2_norm() < epsilon, "scale real");
        result.add_check((vans_z - vcz).L2_norm() < epsilon, "scale complex");
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

    ar.randomise(rng_real);
    br.randomise(rng_real);
    cr.randomise(rng_real);
    az.randomise(rng_real);
    bz.randomise(rng_real);
    cz.randomise(rng_real);

    Real kr1{rng_real.generate()}, kr2{rng_real.generate()};
    Complex kz1{rng_real.generate(), rng_real.generate()},
            kz2{rng_real.generate(), rng_real.generate()};

    while (std::abs(kz2) < epsilon) {
        kz2 = {rng_real.generate(), rng_real.generate()};
    }

    {
        result.add_check(((ar + kr1*br)*(cr/kr2) - (1./kr2)*(ar*cr + kr1*cr*br))
                < epsilon, "real");
        result.add_check(std::abs((az + kz1*bz)*(cz/kz2)
                    - (1./kz2)*(az*cz + kz1*cz*bz)) < epsilon,
                "complex");
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

    Vector<Real> a(10, 1.);
    Real acc{0.};
    for (std::size_t i{0}, N{a.size()}; i < N; ++i) {
        acc += a[i];
    }

    result.add_check((acc - 55.) < epsilon, "all indices");

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
