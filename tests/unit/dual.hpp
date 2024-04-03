#include <cmath>

#include "jump/autodiff/dual.hpp"
#include "jump/data/vector.hpp"
#include "jump/testing/testing.hpp"

using namespace jump;

template <std::size_t N, typename T>
bool vector_compare(const Vector<Dual<N, T>>& lhs, const Vector<T>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (std::size_t i{0}; i < lhs.size(); ++i) {
        if (!vanishes(lhs[i].value - rhs[i])) {
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

    const auto& [xdual, x] = populate(0., 5., 101);
    Vector<Real> dx(101);

    result.add_check(vector_compare(xdual*2, x*2), "scale real");

    return result;
}

