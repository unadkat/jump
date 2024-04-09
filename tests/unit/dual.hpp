#ifndef JUMP_TESTS_DUAL_HPP
#define JUMP_TESTS_DUAL_HPP

#include <cmath>

#include "jump/autodiff/dual.hpp"
#include "jump/data/vector.hpp"
#include "jump/utility/random.hpp"
#include "jump/testing/testing.hpp"

using namespace jump;

template <std::size_t N, typename T>
inline bool vector_compare(const Vector<Dual<N, T>>& dual_vector,
        const Vector<T>& values, const Vector<T>& derivatives) {
    if (dual_vector.size() != values.size()
            || dual_vector.size() != derivatives.size()) {
        return false;
    }
    for (std::size_t i{0}; i < dual_vector.size(); ++i) {
        if (!(approx(dual_vector[i].value, values[i])
                    && approx(dual_vector[i].dual[0], derivatives[i]))) {
            return false;
        }
    }
    return true;
}

inline std::pair<Vector<d1f64>, Vector<Real>> populate_real(Real min, Real max,
        std::size_t N) {
    std::pair<Vector<d1f64>, Vector<Real>> result;
    result.first.storage.assign(N, {0., 0});
    result.second.storage.assign(N, {});

    for (std::size_t i{0}; i < N; ++i) {
        auto x{std::lerp(min, max, i/(N - 1.))};
        result.first[i].value = x;
        result.second[i] = x;
    }
    return result;
}

inline std::pair<Vector<d1z64>, Vector<Complex>> populate_complex(Complex begin,
        Complex end, std::size_t N) {
    std::pair<Vector<d1z64>, Vector<Complex>> result;
    result.first.storage.assign(N, {0., 0});
    result.second.storage.assign(N, {});

    for (std::size_t i{0}; i < N; ++i) {
        Complex x{std::lerp(begin.real(), end.real(), i/(N - 1.)),
            std::lerp(begin.imag(), end.imag(), i/(N - 1.))};
        result.first[i].value = x;
        result.second[i] = x;
    }
    return result;
}

TestResult test_dual_arithmetic_basic();

inline TestSuiteL1 dual_tests() {
    TestSuiteL1 dual_suite("dual");
    std::vector<Test> tests;

    tests.push_back({"arithmetic"});
    tests.back().register_item({"basic", {}, &test_dual_arithmetic_basic});

    dual_suite.register_items(tests);
    return dual_suite;
}

inline TestResult test_dual_arithmetic_basic() {
    TestResult result;

    RandomInt rng_int(91, 111);
    RandomReal rng_real(5., 10.);
    auto N{static_cast<std::size_t>(rng_int.generate())};
    auto xmax{static_cast<Real>(rng_real.generate())};
    Complex zend{xmax, static_cast<Real>(rng_real.generate())};
    auto [xdual, x] = populate_real(0.01, xmax, N);
    auto [zdual, z] = populate_complex({0.01, 0.}, zend, N);

    Vector<Real> dx(N, 1.);
    Vector<Complex> dz(N, {1., 0.});
    result.add_check(vector_compare(xdual + xdual - xdual, x, dx),
            "add/sub real");
    result.add_check(vector_compare(zdual + zdual - zdual, z, dz),
            "add/sub complex");

    Real kr;
    Complex kz;
    randomise(rng_real, kr, kz);

    dx = Vector<Real>(N, kr);
    dz = Vector<Complex>(N, kz);
    result.add_check(vector_compare(xdual*kr, x*kr, dx), "scale real");
    result.add_check(vector_compare(zdual*kz, z*kz, dz), "scale complex");

    result.add_check(vector_compare(exp(xdual), exp(x), exp(x)), "exp real");
    result.add_check(vector_compare(log(xdual), log(x), pow(x, -1.)),
            "log real");
    result.add_check(vector_compare(pow(xdual, d1f64{kr}), pow(x, kr),
                kr*pow(x, kr - 1.)), "pow real");
    result.add_check(vector_compare(sin(xdual), sin(x), cos(x)), "sin real");
    result.add_check(vector_compare(cos(xdual), cos(x), -sin(x)), "cos real");
    result.add_check(vector_compare(atan(xdual), atan(x),
                pow(Vector<Real>(N, 1.) + pow(x, 2.), -1.)), "atan real");
    result.add_check(vector_compare(sinh(xdual), sinh(x), cosh(x)),
            "sinh real");
    result.add_check(vector_compare(cosh(xdual), cosh(x), sinh(x)),
            "cosh real");
    result.add_check(vector_compare(tanh(xdual), tanh(x), pow(cosh(x), -2.)),
            "tanh real");
    result.add_check(vector_compare(asinh(xdual), asinh(x),
                pow(Vector<Real>(N, 1.) + pow(x, 2.), -0.5)), "asinh real");

    result.add_check(vector_compare(exp(zdual), exp(z), exp(z)), "exp complex");
    result.add_check(vector_compare(log(zdual), log(z), pow(z, {-1., 0.})),
            "log complex");
    result.add_check(vector_compare(pow(zdual, d1z64{kz}), pow(z, kz),
                kz*pow(z, kz - 1.)), "pow complex");
    result.add_check(vector_compare(sin(zdual), sin(z), cos(z)), "sin complex");
    result.add_check(vector_compare(cos(zdual), cos(z), -sin(z)), "cos complex");
    result.add_check(vector_compare(atan(zdual), atan(z),
                pow(Vector<Complex>(N, 1.) + pow(z, {2., 0.}), {-1., 0.})),
            "atan complex");
    result.add_check(vector_compare(sinh(zdual), sinh(z), cosh(z)),
            "sinh complex");
    result.add_check(vector_compare(cosh(zdual), cosh(z), sinh(z)),
            "cosh complex");
    result.add_check(vector_compare(tanh(zdual), tanh(z),
                pow(cosh(z), {-2., 0.})), "tanh complex");
    result.add_check(vector_compare(asinh(zdual), asinh(z),
                pow(Vector<Complex>(N, 1.) + pow(z, {2., 0.}), {-0.5, 0.})),
            "asinh complex");

    std::tie(xdual, x) = populate_real(-0.99, 0.99, N);
    std::tie(zdual, z) = populate_complex({-0.99, 0}, zend, N);

    result.add_check(vector_compare(tan(xdual), tan(x), pow(cos(x), -2.)),
            "tan real");
    result.add_check(vector_compare(asin(xdual), asin(x),
                pow(Vector<Real>(N, 1.) - pow(x, 2.), -0.5)), "asin real");
    result.add_check(vector_compare(acos(xdual), acos(x),
                -pow(Vector<Real>(N, 1.) - pow(x, 2.), -0.5)), "acos real");
    result.add_check(vector_compare(atanh(xdual), atanh(x),
                pow(Vector<Real>(N, 1.) - pow(x, 2.), -1.)), "atanh real");
    result.add_check(vector_compare(abs(xdual), abs(x), sgn(x)), "abs real");

    result.add_check(vector_compare(tan(zdual), tan(z), pow(cos(z), {-2., 0.})),
            "tan complex");
    result.add_check(vector_compare(asin(zdual), asin(z),
                pow(Vector<Complex>(N, 1.) - pow(z, {2., 0.}), {-0.5, 0.})),
            "asin complex");
    result.add_check(vector_compare(acos(zdual), acos(z),
                -pow(Vector<Complex>(N, 1.) - pow(z, {2., 0.}), {-0.5, 0.})),
            "acos complex");
    result.add_check(vector_compare(atanh(zdual), atanh(z),
                pow(Vector<Complex>(N, 1.) - pow(z, {2., 0.}), {-1., 0.})),
            "atanh complex");

    std::tie(xdual, x) = populate_real(1.01, xmax, N);
    std::tie(zdual, z) = populate_complex({1.01, 0}, zend, N);

    result.add_check(vector_compare(acosh(xdual), acosh(x),
                pow(pow(x, 2.) - Vector<Real>(N, 1.), -0.5)), "acosh real");
    result.add_check(vector_compare(acosh(zdual), acosh(z),
                pow(pow(z, {2., 0.}) - Vector<Complex>(N, 1.), {-0.5, 0.})),
            "acosh real");

    return result;
}

#endif  // JUMP_TESTS_DUAL_HPP

