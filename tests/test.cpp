#include "jump/jump.hpp"

#include "unit/dual.hpp"
#include "unit/linear_algebra.hpp"
#include "unit/matrix.hpp"
#include "unit/vector.hpp"

using namespace jump;

int main(int argc, char** argv) {
    CommandLineArgs args(argc, argv);

    bool do_trace{false};
    args.get("trace", do_trace);
    args.get('t', do_trace);

    TestSuiteL3 jump_tests{"jump"};

    TestSuiteL2 data_tests{"data"};
    data_tests.register_item(vector_tests());
    data_tests.register_item(matrix_tests());
    jump_tests.register_item(data_tests);

    TestSuiteL2 autodiff_tests{"autodiff"};
    autodiff_tests.register_item(dual_tests());
    jump_tests.register_item(autodiff_tests);

    TestSuiteL2 linalg_tests{"linear algebra"};
    linalg_tests.register_item(linear_algebra_tests());
    jump_tests.register_item(linalg_tests);

    auto all_results{jump_tests.run()};
    TestReporter report;
    if (do_trace) {
        report.trace(all_results);
    }
    report.summarise(all_results);

    return 0;
}

