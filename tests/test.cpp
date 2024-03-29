#include "jump/jump.hpp"

#include "unit/matrix.hpp"
#include "unit/vector.hpp"

using namespace jump;

int main(int argc, char** argv) {
    CommandLineArgs args(argc, argv);

    TestSuiteL3 jump_tests{"jump"};

    TestSuiteL2 data_tests{"data"};
    data_tests.register_item(vector_tests());
    data_tests.register_item(matrix_tests());
    jump_tests.register_item(data_tests);

    auto all_results{jump_tests.run()};
    TestReporter report;
    report.trace(all_results);
    report.summarise(all_results);

    return 0;
}

