#include "jump/jump.hpp"

#include "unit/vector.hpp"

using namespace jump;

int main(int argc, char** argv) {
    CommandLineArgs args(argc, argv);

    TestSuite<TestSuite<Test>> jump_tests{"jump"};
    jump_tests.register_test(vector_tests());
    auto all_results = jump_tests.run();

    TestReporter report;
    report.trace(all_results);
    report.summarise(all_results);

    return 0;
}

