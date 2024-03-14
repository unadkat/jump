#include "jump/jump.hpp"

#include "unit/vector.hpp"

using namespace jump;

int main(int argc, char** argv) {
    CommandLineArgs args(argc, argv);

    auto vector_suite{vector_tests()};
    auto all_results = vector_suite.run();

    TestReporter report;
    report.trace(all_results);
    report.summarise(all_results);

    return 0;
}

