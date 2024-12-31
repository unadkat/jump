#include "unit/dual.hpp"
#include "unit/linear_algebra.hpp"
#include "unit/matrix.hpp"
#include "unit/vector.hpp"

#include "jump/jump.hpp"

#include <iostream>

using namespace jump;

int main(int argc, char** argv) {
    CommandLineArgs args(argc, argv);

    bool do_trace{false};
    args.get("trace", do_trace);
    args.get('t', do_trace);
    std::clog << args << std::endl;

    TestSuite data_tests{"data", std::vector{vector_tests(), matrix_tests()}};
    TestSuite autodiff_tests{"autodiff", dual_tests()};
    TestSuite linalg_tests{"linear algebra", linear_algebra_tests()};

    TestSuite jump_tests{"jump",
        std::vector{data_tests, autodiff_tests, linalg_tests}};

    auto all_results{jump_tests.run()};
    TestReporter report;
    if (do_trace) {
        report.trace(all_results);
    }
    report.summarise(all_results);

    return 0;
}

