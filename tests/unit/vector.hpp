#ifndef JUMP_TESTS_VECTOR_HPP
#define JUMP_TESTS_VECTOR_HPP

#include "jump/data/vector.hpp"
#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/testing/testing.hpp"
#include "jump/utility/random.hpp"

#include <numeric>

using namespace jump;

auto test_vector_arithmetic_basic() -> TestResult;
auto test_vector_arithmetic_compound() -> TestResult;
auto test_vector_inner_prod() -> TestResult;
auto test_vector_arithmetic_fail() -> TestResult;
auto test_vector_norms() -> TestResult;
auto test_vector_access_in_range() -> TestResult;
auto test_vector_access_fail() -> TestResult;

inline auto vector_tests() {
    std::vector<Test> tests;

    tests.push_back({"arithmetic"});
    tests.back().register_item({"basic", &test_vector_arithmetic_basic});
    tests.back().register_item({"compound", &test_vector_arithmetic_compound});
    tests.back().register_item({"inner product", &test_vector_inner_prod});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"fail", &test_vector_arithmetic_fail});
#endif  // NDEBUG

    tests.push_back({"summary"});
    tests.back().register_item({"norms", &test_vector_norms});

    tests.push_back({"access"});
    tests.back().register_item({"in range", &test_vector_access_in_range});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"fail", &test_vector_access_fail});
#endif  // NDEBUG

    return TestSuite{"vector", tests};
}

inline auto test_vector_arithmetic_basic() -> TestResult {
    TestResult result;
    RandomReal rng(0., 10.);

    Real ar, br;
    Complex az, bz;
    randomise(rng, ar, br, az, bz);

    Vector<Real> var(5, ar), vbr(5, br);
    Vector<Complex> varZ{var}, vbrZ{vbr};
    Vector<Complex> vaz(5, az), vbz(5, bz);

    {
        Real ans_r{ar + br};
        Vector<Real> vans_r(5, ans_r), vcr{var + vbr};
        Vector<Complex> vans_rZ{vans_r};
        Vector<Complex> vcrZ1{var + vbrZ}, vcrZ2{varZ + vbr};
        Complex ans_z{az + bz};
        Vector<Complex> vans_z(5, ans_z), vcz{vaz + vbz};

        result.add_check(approx(vans_r, vcr), "add real");
        result.add_check(approx(vans_rZ, vcrZ1), "add real-complex");
        result.add_check(approx(vans_rZ, vcrZ2), "add complex-real");
        result.add_check(approx(vans_z, vcz), "add complex");
    }
    {
        Real ans_r{ar - br};
        Vector<Real> vans_r(5, ans_r), vcr{var - vbr};
        Vector<Complex> vans_rZ{vans_r};
        Vector<Complex> vcrZ1{var - vbrZ}, vcrZ2{varZ - vbr};
        Complex ans_z{az - bz};
        Vector<Complex> vans_z(5, ans_z), vcz{vaz - vbz};

        result.add_check(approx(vans_r, vcr), "subtract real");
        result.add_check(approx(vans_rZ, vcrZ1), "subtract real-complex");
        result.add_check(approx(vans_rZ, vcrZ2), "subtract complex-real");
        result.add_check(approx(vans_z, vcz), "subtract complex");
    }
    {
        // TODO: Real vector multiplication by complex scalar
        Real ans_r{ar*br};
        Vector<Real> vans_r(5, ans_r), vcr{var*br};
        Complex brZ{br};
        Vector<Complex> vans_rZ{vans_r};
//        Vector<Complex> vcrZ1{var*brZ};
//        Vector<Complex> vcrZ2{brZ*var};
        Complex ans_z{az*bz};
        Vector<Complex> vans_z(5, ans_z), vcz{vaz*bz};

        result.add_check(approx(vans_r, vcr), "scale real");
//        result.add_check(vanishes((vans_rZ - vcrZ1).L2_norm()),
//                "scale real-complex");
//        result.add_check(vanishes((vans_rZ - vcrZ2).L2_norm()),
//                "scale complex-real");
        result.add_check(approx(vans_z, vcz), "scale complex");
    }

    return result;
}

inline auto test_vector_arithmetic_compound() -> TestResult {
    TestResult result;
    RandomReal rng_real(0., 10.);
    RandomInt rng_int(10, 15);

    auto size{static_cast<std::size_t>(rng_int.generate())};
    Vector<Real> ar(size), br(size), cr(size);
    Vector<Complex> az(size), bz(size), cz(size);
    Real kr1, kr2;
    Complex kz1, kz2;
    randomise(rng_real, ar, br, cr, az, bz, cz, kr1, kr2, kz1, kz2);

    while (vanishes(kr2)) {
        randomise(rng_real, kr2);
    }
    while (vanishes(std::abs(kz2))) {
        randomise(rng_real, kz2);
    }

    {
        result.add_check(approx(dot(ar + kr1*br, cr/kr2),
                    (1./kr2)*(dot(ar, cr) + kr1*dot(cr, br))), "real");
        result.add_check(approx(dot(az + kz1*bz, cz/kz2),
                    (1./kz2)*(dot(az, cz) + kz1*dot(cz, bz))), "complex");
    }

    return result;
}

inline auto test_vector_inner_prod() -> TestResult {
    TestResult result;
    RandomInt rng_int(10, 15);

    auto N{static_cast<std::size_t>(rng_int.generate())};
    Vector<Real> ar(N), br(N);
    std::iota(ar.begin(), ar.end(), 1);
    Vector<Complex> arZ{ar};

    Vector<Complex> az(N);
    for (std::size_t i{0}; i < N; ++i) {
        az[i] = {ar[i], 0.5*ar[i]};
    }

    Real real_ans{(N*(N + 1)*(2*N + 1))/6.};
    auto res_complex{dot(az, az)};

    result.add_check(approx(dot(ar, ar), real_ans), "inner product real");
    result.add_check(approx(dot(ar, arZ), {real_ans, 0.}),
            "inner product real-complex");
    result.add_check(approx(dot(arZ, ar), {real_ans, 0.}),
            "inner product complex-real");
    result.add_check(approx(res_complex, {0.75*real_ans, real_ans}),
            "inner product complex");

    return result;
}

inline auto test_vector_arithmetic_fail() -> TestResult {
    TestResult result;

    Vector<Real> var(5), vbr(6);
    Vector<Complex> vaz(6), vbz(5);

    {
        bool real_caught{false};
        bool complex_caught{false};

        try {
            [[maybe_unused]] auto res{var + vbr};
        } catch (RuntimeError<Mismatch1DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto res{vaz + vbz};
        } catch (RuntimeError<Mismatch1DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "add real");
        result.add_check(complex_caught, "add complex");
    }
    {
        bool real_caught{false};
        bool complex_caught{false};

        try {
            [[maybe_unused]] auto res{var - vbr};
        } catch (RuntimeError<Mismatch1DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto res{vaz - vbz};
        } catch (RuntimeError<Mismatch1DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "subtract real");
        result.add_check(complex_caught, "subtract complex");
    }
    {
        bool real_caught{false};
        bool complex_caught{false};

        try {
            [[maybe_unused]] auto res{var*vbr};
        } catch (RuntimeError<Mismatch1DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto res{vaz*vbz};
        } catch (RuntimeError<Mismatch1DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "inner product real");
        result.add_check(complex_caught, "inner product complex");
    }

    return result;
}

inline auto test_vector_norms() -> TestResult {
    TestResult result;
    RandomInt rng_int(10, 15);

    auto N{static_cast<std::size_t>(rng_int.generate())};
    Vector<Real> ar(N);
    std::iota(ar.begin(), ar.end(), 1);

    Vector<Complex> az(N);
    for (std::size_t i{0}; i < N; ++i) {
        az[i] = {ar[i], 0.5*ar[i]};
    }

    auto sum_k{0.5*N*(N + 1)};
    auto sum_k2{(N*(N + 1)*(2*N + 1))/6.};
    result.add_check(approx(ar.L1_norm(), sum_k), "L1 norm real");
    result.add_check(approx(ar.L2_norm(), std::sqrt(sum_k2)),
            "L2 norm real");
    result.add_check(approx(ar.Linf_norm(), static_cast<Real>(N)),
            "Linf norm real");

    result.add_check(approx(az.L1_norm(), 0.5*std::sqrt(5.)*sum_k),
            "L1 norm complex");
    result.add_check(approx(az.L2_norm(), 0.5*std::sqrt(5.*sum_k2)),
            "L2 norm complex");
    result.add_check(approx(az.Linf_norm(), static_cast<Real>(
                    0.5*std::sqrt(5)*N)), "Linf norm complex");

    return result;
}

inline auto test_vector_access_in_range() -> TestResult {
    TestResult result;
    RandomInt rng_int(10, 15);

    auto size{static_cast<std::size_t>(rng_int.generate())};
    Vector<Real> a(size);
    std::iota(a.begin(), a.end(), 1);

    Real acc{0.};
    for (std::size_t i{0}, N{a.size()}; i < N; ++i) {
        acc += a[i];
    }

    result.add_check(approx(acc, 0.5*size*(size + 1)), "all indices");

    return result;
}

inline auto test_vector_access_fail() -> TestResult {
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

#endif  // JUMP_TESTS_VECTOR_HPP

