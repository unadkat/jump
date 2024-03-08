#include "jump/jump.hpp"
#include "jump/experimental.hpp"

#include "unit/vector.hpp"

using namespace jump;
using namespace jump::experimental;

int main(int argc, char** argv) {
    CommandLineArgs args(argc, argv);

    TestSuite all_tests("jump tests");
    all_tests.register_tests(vector_tests());
    all_tests.run();

    return 0;
}

