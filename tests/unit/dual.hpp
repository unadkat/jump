#include <cmath>

#include "jump/autodiff/dual.hpp"
#include "jump/data/vector.hpp"
#include "jump/utility/random.hpp"
#include "jump/testing/testing.hpp"

using namespace jump;

template <std::size_t N, typename T>
bool vector_compare(const Vector<Dual<N, T>>& dual_vector,
        const Vector<T>& values, const Vector<T>& derivatives) {
    if (dual_vector.size() != values.size()
            || dual_vector.size() != derivatives.size()) {
        return false;
    }
    for (std::size_t i{0}; i < dual_vector.size(); ++i) {
        if (!(vanishes(dual_vector[i].value - values[i])
                    && vanishes(dual_vector[i].dual[0] - derivatives[i]))) {
            return false;
        }
    }
    return true;
}

template <typename T>
std::pair<Vector<Dual<1, T>>, Vector<T>> populate(T min, T max, std::size_t N) {
    std::pair<Vector<Dual<1, T>>, Vector<T>> result;
    result.first.storage.assign(N, {T{0}, 0});
    result.second.storage.assign(N, {});

    for (std::size_t i{0}; i < N; ++i) {
        auto x{std::lerp(min, max, i/(N - 1.))};
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
    const auto& [xdual, x] = populate(0., xmax, N);

    Vector<Real> dx(N, 1.);
    result.add_check(vector_compare(xdual + xdual - xdual, x, dx),
            "add/sub real");

    Real scale{rng_real.generate()};
    dx = Vector<Real>(N, scale);
    result.add_check(vector_compare(xdual*scale, x*scale, dx), "scale real");

    return result;
}

