// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#include "jump/jump.hpp"

#include "jump/experimental/expression_templates/concepts.hpp"
#ifdef JUMP_ENABLE_SMALL_VEC
#include "jump/experimental/data/vec2.hpp"
#endif  // JUMP_ENABLE_SMALL_VEC

#include <iostream>

using namespace jump;

template <BandedMatrixExpression Expr>
void f(const Expr& M) {
    std::cout << M << std::endl;
}

int banded_matrix_test(int argc, char** argv);
int vector_test(int argc, char** argv);
int small_vector_test(int argc, char** argv);

int main(int argc, char** argv) {
    CommandLineArgs args(argc, argv);

    bool do_vector_test{false};
    bool do_banded_matrix_test{false};
    bool do_small_vector_test{false};
    int sum_return_vals{0};

    args.get("vector", do_vector_test);
    args.get("banded_matrix", do_banded_matrix_test);
    args.get("small_vector", do_small_vector_test);

    if (do_vector_test) {
        sum_return_vals += vector_test(argc, argv);
    }
    if (do_banded_matrix_test) {
        sum_return_vals += banded_matrix_test(argc, argv);
    }
    if (do_small_vector_test) {
        sum_return_vals += small_vector_test(argc, argv);
    }

    return sum_return_vals;
}

int vector_test(int argc, char** argv) {
    CommandLineArgs args(argc, argv);

    Timer sum{"Simple sum"};
    Timer mult_sum{"Mult sum"};

    RandomReal rng_real{0., 100.};
    std::size_t num_vectors{1000000}, vector_size{1000}, num_trials{5};

    args.get("N", num_vectors);
    args.get("size", vector_size);
    args.get("trials", num_trials);

    Vector<Vector<Real>> A(num_vectors, Vector<Real>(vector_size)),
        B(num_vectors, Vector<Real>(vector_size)),
        C(num_vectors, Vector<Real>(vector_size));
    randomise(rng_real, A, B, C);

    for (std::size_t i{0}; i < num_trials; ++i) {
        sum.start();
        for (std::size_t j{0}; j < num_vectors; ++j) {
            A[j] = A[j] + B[j];
        }
        sum.stop();
    }

    for (std::size_t i{0}; i < num_trials; ++i) {
        mult_sum.start();
        for (std::size_t j{0}; j < num_vectors; ++j) {
            A[j] = A[j]*(B[j] + C[j]);
        }
        mult_sum.stop();
    }

    std::cout << sum << std::endl;
    std::cout << mult_sum << std::endl;

    return 0;
}

int banded_matrix_test(int argc, char** argv) {
    CommandLineArgs args(argc, argv);

    BandedMatrix<Real> test{10, 2};
    RandomReal rng_real{0., 1.};
    randomise(rng_real, test);
    f(test);

    BandedMatrix<Real> test2{-test};
    f(test2);

    BandedMatrix<Real> test3{+test};
    f(test3);

    std::cout << "scalar*matrix" << std::endl;
    auto test4{4.*test};
    std::cout << "matrix*scalar" << std::endl;
    auto test5{test*5.};
    std::cout << "scalar/matrix" << std::endl;
    auto test6{6./test};
    std::cout << "matrix/scalar" << std::endl;
    auto test7{test/7.};

    std::cout << evaluate(exp(test)) << std::endl;
    std::cout << evaluate(log(test)) << std::endl;
    std::cout << evaluate(pow(test, 2.)) << std::endl;
    std::cout << evaluate(sin(test)) << std::endl;
    std::cout << evaluate(cos(test)) << std::endl;
    std::cout << evaluate(tan(test)) << std::endl;
    std::cout << evaluate(asin(test)) << std::endl;
    std::cout << evaluate(acos(test)) << std::endl;
    std::cout << evaluate(atan(test)) << std::endl;
    std::cout << evaluate(sinh(test)) << std::endl;
    std::cout << evaluate(cosh(test)) << std::endl;
    std::cout << evaluate(tanh(test)) << std::endl;
    std::cout << evaluate(asinh(test)) << std::endl;
    std::cout << evaluate(acosh(test)) << std::endl;
    std::cout << evaluate(atanh(test)) << std::endl;
    std::cout << evaluate(abs(test)) << std::endl;
    std::cout << evaluate(sgn(test)) << std::endl;

    return 0;
}

int small_vector_test(int argc, char** argv) {
    CommandLineArgs args(argc, argv);

#ifdef JUMP_ENABLE_SMALL_VEC
    Vec<float, 2> test{42};

    std::cout << test << std::endl;
#endif  // JUMP_ENABLE_SMALL_VEC

    return 0;
}

