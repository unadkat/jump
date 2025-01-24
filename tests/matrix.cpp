// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#include "jump/data/banded_matrix.hpp"
#include "jump/data/dense_matrix.hpp"
#include "jump/data/vector.hpp"
#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/testing/testing.hpp"
#include "jump/utility/random.hpp"

using namespace jump;

auto test_matrix_initialise_basic() -> TestResult;
auto test_matrix_initialise_chained() -> TestResult;
auto test_matrix_initialise_fail() -> TestResult;
auto test_matrix_arithmetic_basic() -> TestResult;
auto test_matrix_multiply() -> TestResult;
auto test_matrix_arithmetic_compound() -> TestResult;
auto test_matrix_arithmetic_fail() -> TestResult;
auto test_matrix_norms() -> TestResult;
auto test_matrix_access_in_range() -> TestResult;
auto test_matrix_dense_columns() -> TestResult;
auto test_matrix_access_fail() -> TestResult;

int main() {
    std::vector<Test> tests;

    tests.push_back({"initialise"});
    tests.back().register_item({"basic", &test_matrix_initialise_basic});
    tests.back().register_item({"chained", &test_matrix_initialise_chained});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"fail", &test_matrix_initialise_fail});
#endif  // NDEBUG

    tests.push_back({"arithmetic"});
    tests.back().register_item({"basic", &test_matrix_arithmetic_basic});
    tests.back().register_item({"multiply", &test_matrix_multiply});
    tests.back().register_item({"compound", &test_matrix_arithmetic_compound});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"fail", &test_matrix_arithmetic_fail});
#endif  // NDEBUG

    tests.push_back({"summary"});
    tests.back().register_item({"norms", &test_matrix_norms});

    tests.push_back({"access"});
    tests.back().register_item({"in range", &test_matrix_access_in_range});
    tests.back().register_item({"columns", &test_matrix_dense_columns});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"fail", &test_matrix_access_fail});
#endif  // NDEBUG

    auto test_suite{TestSuite{"matrix", tests}};

    TestReporter report{test_suite.run()};
    report.trace();
    report.summarise();

    return report.failed() > 0;
}

auto test_matrix_initialise_basic() -> TestResult {
    TestResult result;

    RandomInt rng_int_low(10, 15);
    RandomInt rng_int_high(20, 25);
    RandomReal rng(0., 10.);

    auto size1{static_cast<std::size_t>(rng_int_low.generate())};
    auto size2{static_cast<std::size_t>(rng_int_high.generate())};
    std::size_t bands{3};

    Vector<Real> vrb(size1*(3*bands + 1)), vrd(size1*size2);
    Vector<Complex> vzb(size1*(3*bands + 1)), vzd(size1*size2);
    randomise(rng, vrb, vrd, vzb, vzd);

    BandedMatrix<Real> abr{};
    BandedMatrix<Real> bbr{size1};
    BandedMatrix<Real> cbr{size1, bands};
    BandedMatrix<Real> dbr{size1, bands, vrb};

    result.add_check(abr.num_elements() == 0
            && abr.size() == std::pair(0zu, 0zu) && abr.num_bands() == 0,
            "banded real trivial");
    result.add_check(bbr.num_elements() == size1
            && bbr.size() == std::pair(size1, size1) && bbr.num_bands() == 0,
            "banded real diagonal");
    result.add_check(cbr.num_elements() == size1*(3*bands + 1)
            && cbr.size() == std::pair(size1, size1)
            && cbr.num_bands() == bands, "banded real general");
    result.add_check(dbr.num_elements() == size1*(3*bands + 1)
            && dbr.size() == std::pair(size1, size1) && dbr.num_bands() == bands
            && dbr.as_vector() == vrb, "banded real underlying");

    BandedMatrix<Complex> abz{};
    BandedMatrix<Complex> bbz{size1};
    BandedMatrix<Complex> cbz{size1, bands};
    BandedMatrix<Complex> dbz{size1, bands, vzb};

    result.add_check(abz.num_elements() == 0
            && abz.size() == std::pair(0zu, 0zu) && abz.num_bands() == 0,
            "banded complex trivial");
    result.add_check(bbz.num_elements() == size1
            && bbz.size() == std::pair(size1, size1) && bbz.num_bands() == 0,
            "banded complex diagonal");
    result.add_check(cbz.num_elements() == size1*(3*bands + 1)
            && cbz.size() == std::pair(size1, size1)
            && cbz.num_bands() == bands, "banded complex general");
    result.add_check(dbz.num_elements() == size1*(3*bands + 1)
            && dbz.size() == std::pair(size1, size1)
            && dbz.num_bands() == bands && dbz.as_vector() == vzb,
            "banded complex underlying");

    DenseMatrix<Real> adr{};
    DenseMatrix<Real> bdr{size1};
    DenseMatrix<Real> cdr{size1, size2};
    DenseMatrix<Real> ddr{size1, size2, vrd};

    result.add_check(adr.num_elements() == 0
            && adr.size() == std::pair(0zu, 0zu), "dense real trivial");
    result.add_check(bdr.num_elements() == size1*size1
            && bdr.size() == std::pair(size1, size1), "dense real square");
    result.add_check(cdr.num_elements() == size1*size2
            && cdr.size() == std::pair(size1, size2), "dense real general");
    result.add_check(ddr.num_elements() == size1*size2
            && ddr.size() == std::pair(size1, size2) && ddr.as_vector() == vrd,
            "dense real underlying");

    DenseMatrix<Complex> adz{};
    DenseMatrix<Complex> bdz{size1};
    DenseMatrix<Complex> cdz{size1, size2};
    DenseMatrix<Complex> ddz{size1, size2, vzd};

    result.add_check(adz.num_elements() == 0
            && adz.size() == std::pair(0zu, 0zu), "dense complex trivial");
    result.add_check(bdz.num_elements() == size1*size1
            && bdz.size() == std::pair(size1, size1), "dense complex square");
    result.add_check(cdz.num_elements() == size1*size2
            && cdz.size() == std::pair(size1, size2), "dense complex general");
    result.add_check(ddz.num_elements() == size1*size2
            && ddz.size() == std::pair(size1, size2) && ddz.as_vector() == vzd,
            "dense complex underlying");

    return result;
}

auto test_matrix_initialise_chained() -> TestResult {
    TestResult result;

    RandomInt rng_int_low(10, 15);
    RandomInt rng_int_high(20, 25);
    RandomReal rng(0., 15.);

    auto size01{static_cast<std::size_t>(rng_int_low.generate())};
    auto size02{static_cast<std::size_t>(rng_int_low.generate())};
    auto size11{static_cast<std::size_t>(rng_int_high.generate())};
    auto size12{static_cast<std::size_t>(rng_int_high.generate())};
    std::size_t bands0{3}, bands1{5};

    Vector<Real> vrb0(size11*(3*bands1 + 1)), vrb1(vrb0), vrd0(size11*size12),
        vrd1(vrd0);
    Vector<Complex> vzb0(size11*(3*bands1 + 1)), vzb1(vrb0),
        vzd0(size11*size12), vzd1(vrd0);
    randomise(rng, vrb0, vrb1, vrd0, vrd1, vzb0, vzb1, vzd0, vzd1);

    BandedMatrix<Real> br{};
    br.assign(size01, bands0);
    result.add_check(br.num_elements() == size01*(3*bands0 + 1)
            && br.size() == std::pair(size01, size01)
            && br.num_bands() == bands0
            && br.as_vector() == Vector<Real>(br.num_elements()),
            "banded real assign general");

    br.assign(size11, bands1, vrb1);
    result.add_check(br.num_elements() == size11*(3*bands1 + 1)
            && br.size() == std::pair(size11, size11)
            && br.num_bands() == bands1 && br.as_vector() == vrb1,
            "banded real assign underlying");

    br.assign(vrb0);
    result.add_check(br.num_elements() == size11*(3*bands1 + 1)
            && br.size() == std::pair(size11, size11)
            && br.num_bands() == bands1 && br.as_vector() == vrb0,
            "banded real assign underlying only");

    br.assign(vrb1.begin(), vrb1.end());
    result.add_check(br.num_elements() == size11*(3*bands1 + 1)
            && br.size() == std::pair(size11, size11)
            && br.num_bands() == bands1 && br.as_vector() == vrb1,
            "banded real assign iterators");

    BandedMatrix<Complex> bz{};
    bz.assign(size01, bands0);
    result.add_check(bz.num_elements() == size01*(3*bands0 + 1)
            && bz.size() == std::pair(size01, size01)
            && bz.num_bands() == bands0
            && bz.as_vector() == Vector<Complex>(bz.num_elements()),
            "banded complex assign general");

    bz.assign(size11, bands1, vzb1);
    result.add_check(bz.num_elements() == size11*(3*bands1 + 1)
            && bz.size() == std::pair(size11, size11)
            && bz.num_bands() == bands1 && bz.as_vector() == vzb1,
            "banded complex assign underlying");

    bz.assign(vzb0);
    result.add_check(bz.num_elements() == size11*(3*bands1 + 1)
            && bz.size() == std::pair(size11, size11)
            && bz.num_bands() == bands1 && bz.as_vector() == vzb0,
            "banded complex assign underlying only");

    bz.assign(vzb1.begin(), vzb1.end());
    result.add_check(bz.num_elements() == size11*(3*bands1 + 1)
            && bz.size() == std::pair(size11, size11)
            && bz.num_bands() == bands1 && bz.as_vector() == vzb1,
            "banded complex assign iterators");

    DenseMatrix<Real> dr{};
    dr.assign(size01);
    result.add_check(dr.num_elements() == size01*size01
            && dr.size() == std::pair(size01, size01)
            && dr.as_vector() == Vector<Real>(dr.num_elements()),
            "dense real assign square");

    dr.assign(size01, size02);
    result.add_check(dr.num_elements() == size01*size02
            && dr.size() == std::pair(size01, size02)
            && dr.as_vector() == Vector<Real>(dr.num_elements()),
            "dense real assign general");

    dr.assign(size11, size12, vrd1);
    result.add_check(dr.num_elements() == size11*size12
            && dr.size() == std::pair(size11, size12)
            && dr.as_vector() == vrd1, "dense real assign underlying");

    dr.assign(vrd0);
    result.add_check(dr.num_elements() == size11*size12
            && dr.size() == std::pair(size11, size12)
            && dr.as_vector() == vrd0, "dense real assign underlying only");

    dr.assign(vrd1.begin(), vrd1.end());
    result.add_check(dr.num_elements() == size11*size12
            && dr.size() == std::pair(size11, size12)
            && dr.as_vector() == vrd1, "dense real assign iterators");

    DenseMatrix<Complex> dz{};
    dz.assign(size01);
    result.add_check(dz.num_elements() == size01*size01
            && dz.size() == std::pair(size01, size01)
            && dz.as_vector() == Vector<Complex>(dz.num_elements()),
            "dense complex assign square");

    dz.assign(size01, size02);
    result.add_check(dz.num_elements() == size01*size02
            && dz.size() == std::pair(size01, size02)
            && dz.as_vector() == Vector<Complex>(dz.num_elements()),
            "dense complex assign general");

    dz.assign(size11, size12, vzd1);
    result.add_check(dz.num_elements() == size11*size12
            && dz.size() == std::pair(size11, size12)
            && dz.as_vector() == vzd1, "dense complex assign underlying");

    dz.assign(vzd0);
    result.add_check(dz.num_elements() == size11*size12
            && dz.size() == std::pair(size11, size12)
            && dz.as_vector() == vzd0, "dense complex assign underlying only");

    dz.assign(vzd1.begin(), vzd1.end());
    result.add_check(dz.num_elements() == size11*size12
            && dz.size() == std::pair(size11, size12)
            && dz.as_vector() == vzd1, "dense complex assign iterators");

    return result;
}

auto test_matrix_initialise_fail() -> TestResult {
    TestResult result;

    RandomInt rng_int(10, 15);
    std::size_t bands{3};
    auto size1{static_cast<std::size_t>(rng_int.generate())};
    auto size2{static_cast<std::size_t>(rng_int.generate())};
    auto underlying_size_banded1{size1*(3*bands + 1) + 1};
    auto underlying_size_banded2{size1*(3*bands + 1) - 1};
    auto underlying_size_dense1{size1*size2 + 1};
    auto underlying_size_dense2{size1*size2 - 1};

    Vector<Real> vbr(underlying_size_banded1), vdr(underlying_size_dense1);
    Vector<Complex> vbz(underlying_size_banded2), vdz(underlying_size_dense2);

    {
        bool real_caught{false};
        bool complex_caught{false};

        try {
            [[maybe_unused]] BandedMatrix<Real> br{size1, bands, vbr};
        } catch (RuntimeError<InvalidArgumentError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] BandedMatrix<Complex> bz{size1, bands, vbz};
        } catch (RuntimeError<InvalidArgumentError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "banded real underlying");
        result.add_check(complex_caught, "banded complex underlying");
    }
    {
        bool real_caught{false};
        bool complex_caught{false};

        BandedMatrix<Real> br;
        BandedMatrix<Complex> bz;

        try {
            br.assign(size1, bands, vbr);
        } catch (RuntimeError<InvalidArgumentError>& e) {
            real_caught = true;
        }
        try {
            bz.assign(size1, bands, vbz);
        } catch (RuntimeError<InvalidArgumentError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "banded real assign underlying");
        result.add_check(complex_caught, "banded complex assign underlying");

        real_caught = complex_caught = false;

        try {
            br.assign(vbr);
        } catch (RuntimeError<Mismatch1DError>& e) {
            real_caught = true;
        }
        try {
            bz.assign(vbz);
        } catch (RuntimeError<Mismatch1DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "banded real assign underlying only");
        result.add_check(complex_caught,
                "banded complex assign underlying only");

        real_caught = complex_caught = false;

        try {
            br.assign(vbr.begin(), vbr.end());
        } catch (RuntimeError<InvalidArgumentError>& e) {
            real_caught = true;
        }
        try {
            bz.assign(vbz.begin(), vbz.end());
        } catch (RuntimeError<InvalidArgumentError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "banded real assign iterators");
        result.add_check(complex_caught, "banded complex assign iterators");
    }
    {
        bool real_caught{false};
        bool complex_caught{false};

        try {
            [[maybe_unused]] DenseMatrix<Real> dr{size1, size2, vdr};
        } catch (RuntimeError<InvalidArgumentError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] DenseMatrix<Complex> bz{size1, size2, vdz};
        } catch (RuntimeError<InvalidArgumentError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "dense real underlying");
        result.add_check(complex_caught, "dense complex underlying");
    }
    {
        bool real_caught{false};
        bool complex_caught{false};

        DenseMatrix<Real> dr;
        DenseMatrix<Complex> dz;

        try {
            dr.assign(size1, size2, vdr);
        } catch (RuntimeError<InvalidArgumentError>& e) {
            real_caught = true;
        }
        try {
            dz.assign(size1, size2, vdz);
        } catch (RuntimeError<InvalidArgumentError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "dense real assign underlying");
        result.add_check(complex_caught, "dense complex assign underlying");

        real_caught = complex_caught = false;

        try {
            dr.assign(vdr);
        } catch (RuntimeError<Mismatch1DError>& e) {
            real_caught = true;
        }
        try {
            dz.assign(vdz);
        } catch (RuntimeError<Mismatch1DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "dense real assign underlying only");
        result.add_check(complex_caught,
                "dense complex assign underlying only");

        real_caught = complex_caught = false;

        try {
            dr.assign(vdr.begin(), vdr.end());
        } catch (RuntimeError<InvalidArgumentError>& e) {
            real_caught = true;
        }
        try {
            dz.assign(vdz.begin(), vdz.end());
        } catch (RuntimeError<InvalidArgumentError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "dense real assign iterators");
        result.add_check(complex_caught, "dense complex assign iterators");
    }

    return result;
}

auto test_matrix_arithmetic_basic() -> TestResult {
    TestResult result;
    RandomReal rng_real(0., 10.);
    RandomInt rng_int(10, 15);

    auto size{static_cast<std::size_t>(rng_int.generate())};
    auto size2{size + 2};
    Real k1, k2;
    Complex kz1, kz2;
    randomise(rng_real, k1, k2, kz1, kz2);

    auto sum_r = k1 + k2;
    auto diff_r = k1 - k2;
    auto sum_z = kz1 + kz2;
    auto diff_z = kz1 - kz2;
    auto sum_zr = kz1 + k2;
    auto sum_rz = k1 + kz2;
    auto diff_zr = kz1 - k2;
    auto diff_rz = k1 - kz2;

    {
        BandedMatrix<Real> Ar{size, 3}, Br{Ar};
        BandedMatrix<Complex> Az{size, 3}, Bz{Az};

        Ar.fill(k1);
        Br.fill(k2);
        Az.fill(kz1);
        Bz.fill(kz2);

        BandedMatrix<Real> ans_r{size, 3};
        ans_r.fill(sum_r);
        result.add_check(approx((Ar + Br), ans_r), "add banded real");

        ans_r.fill(diff_r);
        result.add_check(approx((Ar - Br), ans_r), "subtract banded real");

        BandedMatrix<Complex> ans_z{size, 3};
        ans_z.fill(sum_rz);
        result.add_check(approx((Ar + Bz), ans_z), "add banded real-complex");
        ans_z.fill(sum_zr);
        result.add_check(approx((Az + Br), ans_z), "add banded complex-real");
        ans_z.fill(sum_z);
        result.add_check(approx((Az + Bz), ans_z), "add banded complex");

        ans_z.fill(diff_rz);
        result.add_check(approx((Ar - Bz), ans_z),
                "subtract banded real-complex");
        ans_z.fill(diff_zr);
        result.add_check(approx((Az - Br), ans_z),
                "subtract banded complex-real");
        ans_z.fill(diff_z);
        result.add_check(approx((Az - Bz), ans_z),
                "subtract banded complex");
    }
    {
        DenseMatrix<Real> Ar{size, size2}, Br{Ar};
        DenseMatrix<Complex> Az{size, size2}, Bz{Az};

        Ar.fill(k1);
        Br.fill(k2);
        Az.fill(kz1);
        Bz.fill(kz2);

        DenseMatrix<Real> ans_r{size, size2};
        ans_r.fill(sum_r);
        result.add_check(approx((Ar + Br), ans_r), "add dense real");

        ans_r.fill(diff_r);
        result.add_check(approx((Ar - Br), ans_r), "subtract dense real");

        DenseMatrix<Complex> ans_z{size, size2};
        ans_z.fill(sum_rz);
        result.add_check(approx((Ar + Bz), ans_z), "add dense real-complex");
        ans_z.fill(sum_zr);
        result.add_check(approx((Az + Br), ans_z), "add dense complex-real");
        ans_z.fill(sum_z);
        result.add_check(approx((Az + Bz), ans_z), "add dense complex");

        ans_z.fill(diff_rz);
        result.add_check(approx((Ar - Bz), ans_z),
                "subtract dense real-complex");
        ans_z.fill(diff_zr);
        result.add_check(approx((Az - Br), ans_z),
                "subtract dense complex-real");
        ans_z.fill(diff_z);
        result.add_check(approx((Az - Bz), ans_z),
                "subtract dense complex");
    }

    return result;
}

auto test_matrix_multiply() -> TestResult {
    TestResult result;

    // Test cases generated and verified externally using Python and numpy
    {
        // (5x7) x (7x2)
        Vector<Real> A_data{
            -3.828e+00,  4.755e+00,  5.808e+00,  1.489e+00,  4.500e+00,
            -5.542e+00, -4.373e+00, -1.734e+00,  9.573e+00,  1.399e+00,
             3.569e+00,  9.853e+00,  1.776e+00,  8.157e+00, -8.255e+00,
             6.640e+00,  9.711e+00,  7.631e+00, -1.223e+00,  8.551e+00,
            -9.621e+00,  3.868e+00, -5.732e+00,  1.518e+00, -9.847e+00,
            -4.597e+00,  5.980e+00, -3.231e+00, -7.326e+00,  6.630e+00,
            -8.876e+00, -6.655e+00, -8.827e+00, -6.066e+00,  3.000e-03
        };
        Vector<Real> B_data{
            -8.97 ,  0.476, -8.364,  1.317,  0.424, -6.809,  1.484,
            -1.858,  2.02 ,  4.791,  2.298, -9.578,  7.897,  1.447
        };
        Vector<Real> AB_data{
            24.642617, -163.308811,  -51.257338,  -37.110999,   -8.706935,
            71.27924 ,   52.399922,   28.36413 ,  -28.329697,  121.24149
        };

        DenseMatrix<Real> A{5, 7, std::move(A_data)};
        DenseMatrix<Complex> Az{A};
        DenseMatrix<Real> B{7, 2, std::move(B_data)};
        DenseMatrix<Complex> Bz{B};
        DenseMatrix<Real> AB{5, 2, std::move(AB_data)};
        DenseMatrix<Complex> ABz{AB};

        result.add_check(approx(A*B, AB),
                "dense real matrix-matrix case 1");
        result.add_check(approx(A*Bz, ABz),
                "dense real-complex matrix-matrix case 1");
        result.add_check(approx(Az*B, ABz),
                "dense complex-real matrix-matrix case 1");
    }
    {
        // (4x6) x (6x3)
        Vector<Complex> A_data{
            {5.023, 2.056}, {7.394, -7.283}, {2.577, -8.24}, {0.105, -0.227},
            {-9.978, -9.09}, {6.373, 9.381}, {-6.206, 2.088}, {-6.495, -1.871},
            {6.625, 1.26}, {-6.314, -6.322}, {-8.706, 5.559}, {0.31, 6.179},
            {3.908, 5.616}, {0.698, -0.954}, {1.452, -0.317}, {8.26, 9.563},
            {-8.59, -6.031}, {0.472, 8.992}, {-2.425, -7.086}, {-2.853, -9.352},
            {-4.469, -7.396}, {9.066, 5.8}, {4.166, -1.575}, {4.065, -6.873}
        };
        Vector<Complex> B_data{
            {8.065, -7.861}, {-1.432, 4.535}, {7.118, -6.167}, {9.243, 8.58},
            {1.399, 3.439}, {1.323, 8.541}, {-6.272, 3.185}, {5.627, -2.314},
            {0.61, 6.984}, {4.537, -1.348}, {-0.883, -5.986}, {7.445, -4.375},
            {1.632, 7.332}, {-2.689, 7.668}, {9.75, -1.829}, {0.617, -1.915},
            {-6.145, 1.75}, {9.697, -9.384}
        };
        Vector<Complex> AB_data{
            {221.027839, -87.518896}, {-186.388558, -10.975084},
            {-16.17961, 0.184562}, {143.707785, 174.631519},
            {-188.836509, 63.525861}, {222.85254, 46.797955},
            {-74.132658, 16.012509}, {-86.678239, 0.828577},
            {120.319915, -23.48906}, {25.578539, -69.309459},
            {43.791261, 4.3252}, {80.200821, -46.930565}
        };

        DenseMatrix<Complex> A{4, 6, std::move(A_data)};
        DenseMatrix<Complex> B{6, 3, std::move(B_data)};
        DenseMatrix<Complex> AB{4, 3, std::move(AB_data)};

        result.add_check(approx(A*B, AB),
                "dense complex matrix-matrix case 1");
    }
    {
        // (5x6) x (6x1)
        Vector<Real> A_data{
            -6.495, 7.421, 2.715, -4.091, 4.558,
            6.481, 1.964, -3.939, -3.144, 0.264,
            2.464, -7.543, 9.813, 2.163, 7.616,
            9.47, 4.117, 8.726, 7.507, -1.674,
            -0.263, -9.422, 7.134, 3.846, -2.227,
            2.748, -3.954, -3.613, -5.047, -0.4
        };
        Vector<Real> b{
            7.729, 5.181, -2.949, 5.98, 7.178, -7.998
        };
        Vector<Real> Ab{
            8.876152, 78.389336, 103.923845, 58.577264, -8.659744
        };

        DenseMatrix<Real> A{5, 6, std::move(A_data)};
        DenseMatrix<Complex> Az{A};
        Vector<Complex> bz{b}, Abz{Ab};

        result.add_check(approx(A*b, Ab),
                "dense real matrix-vector case 1");
        result.add_check(approx(A*bz, Abz),
                "dense real-complex matrix-vector case 1");
        result.add_check(approx(Az*b, Abz),
                "dense complex-real matrix-vector case 1");
    }
    {
        // (7x4) x (4x1)
        Vector<Complex> A_data{
            {-2.472, 4.53}, {-2.402, -3.835}, {1.441, -5.366}, {2.97, -0.005},
            {0.211, 7.707}, {-4.774, 3.405}, {9.515, -3.917}, {6.759, -0.156},
            {-7.078, -8.797}, {-1.813, -8.914}, {2.504, -0.672}, {0.214, -5.59},
            {8.708, 8.636}, {-7.851, -5.833}, {-4.882, -2.955}, {4.08, 2.014},
            {-4.509, 4.445}, {0.371, 5.458}, {-2.251, 4.559}, {5.328, 9.113},
            {-6.051, 1.01}, {-0.604, 7.284}, {-8.507, 2.951}, {-1.986, 9.47},
            {5.786, 4.983}, {-3.74, -9.201}, {5.993, -2.369}, {6.658, -9.813}
        };
        Vector<Complex> b{
            {-8.009, 8.2}, {0.263, 0.617}, {-0.902, -0.828}, {-1.763, -2.041}
        };
        Vector<Complex> Ab{
            {2.414437, -57.323117}, {73.259117, 11.302684},
            {68.060516, 38.411369}, {-18.518387, -0.062282},
            {-67.762354, -39.726982}, {-5.385322, -79.460017},
            {-68.024192, 110.826656}
        };

        DenseMatrix<Complex> A{7, 4, std::move(A_data)};

        result.add_check(approx(A*b, Ab), "dense complex matrix-vector case 1");
    }
    {
        // (10x10 (3 bands)) x (10x1)
        Vector<Real> A_data{
             0.000,  0.000,  0.000,  0.000,  0.000,  0.000, -9.649,  5.705,
            -7.733, -3.903,  0.000,  0.000,  0.000,  0.000,  0.000, -5.364,
             6.435,  9.593, -6.783, -8.628,  0.000,  0.000,  0.000,  0.000,
            -5.272,  7.047,  2.491,  0.462, -9.809,  5.998,  0.000,  0.000,
             0.000,  5.929,  5.342, -4.768, -9.391,  9.758,  8.822,  7.830,
             0.000,  0.000, -3.930, -6.120, -1.033,  6.505, -6.683, -7.287,
            -1.807,  5.814,  0.000, -8.859,  8.597,  0.095, -6.698, -7.842,
            -1.500,  9.122,  4.018,  5.733,  5.505,  2.891,  5.400, -0.886,
            -2.236, -1.320,  3.997,  4.431, -6.959,  8.328,  7.420,  3.787,
            -6.950, -8.722,  4.196,  6.768, -7.238, -9.236, -8.063,  0.000,
            -9.877,  0.594, -8.889,  1.859, -1.791, -1.556,  6.607, -2.870,
             0.000,  0.000, -1.788,  6.752, -0.911,  4.240, -6.396,  7.930,
            -3.983,  0.000,  0.000,  0.000
        };
        Vector<Real> b{
            -1.356, -9.911, -5.795,  3.688, -0.901,  7.528,  8.324,  8.243,
             1.650, -1.691
        };
        Vector<Real> Ab{
            101.338038, 62.810746, -55.09532, -81.736372, 8.737182, 21.258512,
            178.110225, 10.478136, -93.40912,   4.858716
        };

        BandedMatrix<Real> A{10, 3, std::move(A_data)};
        BandedMatrix<Complex> Az{A};
        Vector<Complex> bz{b}, Abz{Ab};

        result.add_check(approx(A*b, Ab), "banded real matrix-vector case 1");
        result.add_check(approx(A*bz, Abz),
                "banded real-complex matrix-vector case 1");
        result.add_check(approx(Az*b, Abz),
                "banded complex-real matrix-vector case 1");
    }
    {
        // (10x10 (3 bands)) x (10x1)
        Vector<Complex> A_data{
            { 0.000,  0.000}, { 0.000,  0.000}, { 0.000,  0.000},
            { 0.000,  0.000}, { 0.000,  0.000}, { 0.000,  0.000},
            {-8.320, -2.393}, { 5.195, -0.234}, { 7.548, -0.855},
            { 6.738,  0.655}, { 0.000,  0.000}, { 0.000,  0.000},
            { 0.000,  0.000}, { 0.000,  0.000}, { 0.000,  0.000},
            {-5.815,  3.558}, {-4.603, -9.155}, {-1.024, -3.876},
            {-1.709, -5.886}, { 4.196,  2.184}, { 0.000,  0.000},
            { 0.000,  0.000}, { 0.000,  0.000}, { 0.000,  0.000},
            {-4.543,  7.663}, {-6.449,  0.614}, {-8.899,  5.356},
            { 7.832,  9.446}, {-6.747,  5.521}, { 4.415,  3.378},
            { 0.000,  0.000}, { 0.000,  0.000}, { 0.000,  0.000},
            {-5.574,  1.150}, { 6.774,  5.223}, {-2.345, -4.819},
            { 3.532,  1.355}, {-9.887,  7.517}, { 2.147, -5.629},
            { 8.126,  2.839}, { 0.000,  0.000}, { 0.000,  0.000},
            {-5.930,  1.207}, { 7.101, -7.669}, {-6.502,  5.418},
            {-8.473, -9.294}, {-7.775, -9.227}, {-1.876, -1.057},
            {-1.824, -3.733}, { 3.808,  4.098}, { 0.000,  0.000},
            {-9.216, -4.653}, { 7.577, -6.495}, {-7.479, -2.824},
            {-0.032,  6.589}, { 1.399, -8.072}, { 2.729, -8.936},
            { 9.500,  7.645}, { 2.227, -4.876}, { 5.577,  8.066},
            { 4.055,  7.930}, {-1.229, -0.784}, {-1.571,  3.336},
            { 8.893, -3.653}, {-1.627,  1.420}, {-5.075,  2.800},
            { 5.039, -8.465}, {-8.357,  0.030}, { 6.993, -4.448},
            { 4.254, -4.164}, {-1.356,  2.084}, {-0.155,  2.997},
            { 1.751, -7.095}, {-1.228, -1.487}, {-1.339,  5.892},
            { 3.657, -3.064}, {-9.439, -7.835}, {-0.439, -0.024},
            { 7.975,  5.126}, { 0.000,  0.000}, { 1.310, -8.161},
            {-2.882, -6.306}, { 9.784, -2.628}, { 3.324, -9.654},
            {-8.789,  1.377}, { 8.331,  5.229}, {-2.692, -2.244},
            { 8.730,  6.699}, { 0.000,  0.000}, { 0.000,  0.000},
            { 1.807,  7.345}, {-9.154,  7.222}, {-1.625,  7.178},
            { 1.320, -1.295}, {-0.913,  5.398}, {-0.639,  9.302},
            { 1.889,  8.646}, { 0.000,  0.000}, { 0.000,  0.000},
            { 0.000,  0.000}
        };
        Vector<Complex> b{
            { 5.482, -7.860}, { 6.604, -1.570}, { 8.361,  9.008},
            {-8.412, -4.030}, {-7.414,  8.702}, { 2.197,  2.975},
            { 5.362,  9.744}, {-8.645,  8.299}, { 6.119,  7.971},
            {-5.818, -9.949}
        };
        Vector<Complex> Ab{
            {-181.19494, 104.680556}, {-67.981991, -138.068312},
            {-98.901961, -262.711272}, {323.940196, 68.556902},
            {409.804343, 62.397473}, {107.768607, -67.947057},
            {5.216361, -6.448086}, {125.339343, -17.122512},
            {170.766304, -7.784181}, {26.949408, 82.475785}
        };

        BandedMatrix<Complex> A{10, 3, std::move(A_data)};

        result.add_check(approx(A*b, Ab),
                "banded complex matrix-vector case 1");
    }

    return result;
}

auto test_matrix_arithmetic_compound() -> TestResult {
    TestResult result;
    RandomReal rng_real(0., 10.);
    RandomInt rng_int(10, 15);

    auto size{static_cast<std::size_t>(rng_int.generate())};
    Real kr1, kr2, kr3;
    Complex kz1, kz2, kz3;
    randomise(rng_real, kr1, kr2, kr3, kz1, kz2, kz3);

    while (vanishes(kr3)) {
        randomise(rng_real, kr3);
    }
    while (vanishes(std::abs(kz3))) {
        randomise(rng_real, kz3);
    }

    {
        std::size_t bands{3};
        BandedMatrix<Real> Ar{size, bands}, Br{Ar}, Cr{Ar};
        BandedMatrix<Complex> Az{size, bands}, Bz{Az}, Cz{Az};
        randomise(rng_real, Ar, Br, Cr, Az, Bz, Cz);

        auto lhs_r{kr1*(Ar + Br) + kr2*(Ar - Cr)/kr3};
        auto rhs_r{(kr1 + kr2/kr3)*Ar + kr1*Br - kr2/kr3*Cr};
        auto lhs_z{kz1*(Az + Bz) + kz2*(Az - Cz)/kz3};
        auto rhs_z{(kz1 + kz2/kz3)*Az + kz1*Bz - kz2/kz3*Cz};

        result.add_check(approx(lhs_r, rhs_r), "banded real");
        result.add_check(approx(lhs_z, rhs_z), "banded complex");
    }
    {
        std::size_t size2{size + 2};
        DenseMatrix<Real> Ar{size, size2}, Br{Ar}, Cr{Ar}, Dr{size2, size};
        DenseMatrix<Complex> Az{size, size2}, Bz{Az}, Cz{Az}, Dz{size2, size};
        randomise(rng_real, Ar, Br, Cr, Dr, Az, Bz, Cz, Dz);

        auto lhs_r{(kr1*(Ar + Br) + kr2*(Ar - Cr)/kr3)*Dr};
        auto rhs_r{(kr1*(Ar + Br) + kr2*(Ar - Cr)/kr3)*Dr};
        auto lhs_z{(kz1 + kz2/kz3)*Az*Dz + kz1*Bz*Dz + kz2/kz3*Cz*Dz};
        auto rhs_z{(kz1 + kz2/kz3)*Az*Dz + kz1*Bz*Dz + kz2/kz3*Cz*Dz};

        result.add_check(approx(lhs_r, rhs_r), "dense real");
        result.add_check(approx(lhs_z, rhs_z), "dense complex");
    }

    return result;
}

auto test_matrix_arithmetic_fail() -> TestResult {
    TestResult result;

    std::size_t size1{10}, size2{11};
    std::size_t bands1{2}, bands2{3};
    Vector<Real> vr(size2);
    Vector<Complex> vz(size2);
    bool real_caught{false}, complex_caught{false};

    {
        BandedMatrix<Real> Ar{size1, bands1}, Br{size2, bands1},
            Cr{size1, bands2};
        BandedMatrix<Complex> Az{size1, bands1}, Bz{size2, bands1},
            Cz{size1, bands2};

        try {
            [[maybe_unused]] auto temp{Ar + Br};
        } catch (RuntimeError<Mismatch2DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Az + Bz};
        } catch (RuntimeError<Mismatch2DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "add banded real, size diff");
        result.add_check(complex_caught, "add banded complex, size diff");

        real_caught = complex_caught = false;

        try {
            [[maybe_unused]] auto temp{Ar + Cr};
        } catch (RuntimeError<InvalidArgumentError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Az + Cz};
        } catch (RuntimeError<InvalidArgumentError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "add banded real, bands diff");
        result.add_check(complex_caught, "add banded complex, bands diff");

        real_caught = complex_caught = false;

        try {
            [[maybe_unused]] auto temp{Ar - Br};
        } catch (RuntimeError<Mismatch2DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Az - Bz};
        } catch (RuntimeError<Mismatch2DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "subtract banded real, size diff");
        result.add_check(complex_caught, "subtract banded complex, size diff");

        real_caught = complex_caught = false;

        try {
            [[maybe_unused]] auto temp{Ar - Cr};
        } catch (RuntimeError<InvalidArgumentError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Az - Cz};
        } catch (RuntimeError<InvalidArgumentError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "subtract banded real, bands diff");
        result.add_check(complex_caught, "subtract banded complex, bands diff");

        real_caught = complex_caught = false;

        try {
            [[maybe_unused]] auto temp{Ar*vr};
        } catch (RuntimeError<Mismatch2DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Az*vz};
        } catch (RuntimeError<Mismatch2DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "matrix-vector banded real");
        result.add_check(complex_caught, "matrix-vector banded complex");
    }
    {
        DenseMatrix<Real> Ar{size1, size2}, Br{size2, size2},
            Cr{size1, size1};
        DenseMatrix<Complex> Az{size1, size2}, Bz{size2, size2},
            Cz{size1, size1};

        try {
            [[maybe_unused]] auto temp{Ar + Br};
        } catch (RuntimeError<Mismatch2DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Az + Bz};
        } catch (RuntimeError<Mismatch2DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "add dense real, row diff");
        result.add_check(complex_caught, "add dense complex, row diff");

        real_caught = complex_caught = false;

        try {
            [[maybe_unused]] auto temp{Ar + Cr};
        } catch (RuntimeError<Mismatch2DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Az + Cz};
        } catch (RuntimeError<Mismatch2DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "add dense real, col diff");
        result.add_check(complex_caught, "add dense complex, col diff");

        real_caught = complex_caught = false;

        try {
            [[maybe_unused]] auto temp{Ar - Br};
        } catch (RuntimeError<Mismatch2DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Az - Bz};
        } catch (RuntimeError<Mismatch2DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "subtract dense real, row diff");
        result.add_check(complex_caught, "subtract dense complex, row diff");

        real_caught = complex_caught = false;

        try {
            [[maybe_unused]] auto temp{Ar - Cr};
        } catch (RuntimeError<Mismatch2DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Az - Cz};
        } catch (RuntimeError<Mismatch2DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "subtract dense real, col diff");
        result.add_check(complex_caught, "subtract dense complex, col diff");

        real_caught = complex_caught = false;

        try {
            [[maybe_unused]] auto temp{Cr*vr};
        } catch (RuntimeError<Mismatch2DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Cz*vz};
        } catch (RuntimeError<Mismatch2DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "matrix-vector dense real");
        result.add_check(complex_caught, "matrix-vector dense complex");

        real_caught = complex_caught = false;

        try {
            [[maybe_unused]] auto temp{Ar*Cr};
        } catch (RuntimeError<Mismatch2DError>& e) {
            real_caught = true;
        }
        try {
            [[maybe_unused]] auto temp{Az*Cz};
        } catch (RuntimeError<Mismatch2DError>& e) {
            complex_caught = true;
        }

        result.add_check(real_caught, "matrix-matrix dense real");
        result.add_check(complex_caught, "matrix-matrix dense complex");
    }

    return result;
}

auto test_matrix_norms() -> TestResult {
    TestResult result;
    RandomInt rng_int(10, 15);

    auto size{static_cast<std::size_t>(rng_int.generate())};
    DenseMatrix<Real> A{size, size + 2};
    DenseMatrix<Complex> B{size + 2, size};

    for (std::size_t col{0}; col < size + 2; ++col) {
        for (std::size_t row{0}; row < size; ++row) {
            const auto val{static_cast<Real>(row + 1)};
            A[row, col] = (col + 1)*val;
        }
    }
    for (std::size_t col{0}; col < size; ++col) {
        for (std::size_t row{0}; row < size + 2; ++row) {
            const auto val{static_cast<Real>(row + 1)};
            B[row, col] = {(col + 1)*val, (col + 2)*val};
        }
    }

    Vector<Real> real_col_1_norm(size + 2);
    Vector<Real> real_col_2_norm(size + 2);
    Vector<Real> real_col_inf_norm(size + 2);
    Vector<Real> complex_col_1_norm(size);
    Vector<Real> complex_col_2_norm(size);
    Vector<Real> complex_col_inf_norm(size);

    Real lin{0.5*size*(size + 1)};
    Real quad{size*(size + 1)*(2*size + 1)/6.};
    for (std::size_t col{0}; col < size + 2; ++col) {
        real_col_1_norm[col] = A.column_L1_norm(col) - (col + 1)*lin;
        real_col_2_norm[col] = A.column_L2_norm(col)
            - (col + 1)*std::sqrt(quad);
        real_col_inf_norm[col] = A.column_Linf_norm(col) - (col + 1)*size;
    }

    lin = 0.5*(size + 2)*(size + 3);
    quad = (size + 2)*(size + 3)*(2*size + 5)/6.;
    for (std::size_t col{0}; col < size; ++col) {
        Real scale{std::sqrt((col + 1)*(col + 1) + (col + 2)*(col + 2))};
        complex_col_1_norm[col] = B.column_L1_norm(col) - scale*lin;
        complex_col_2_norm[col] = B.column_L2_norm(col) - scale*std::sqrt(quad);
        complex_col_inf_norm[col] = B.column_Linf_norm(col) - scale*(size + 2);
    }

    result.add_check(vanishes(real_col_1_norm), "real L1 norm");
    result.add_check(vanishes(real_col_2_norm), "real L2 norm");
    result.add_check(vanishes(real_col_inf_norm), "real Linf norm");
    result.add_check(vanishes(complex_col_1_norm), "complex L1 norm");
    result.add_check(vanishes(complex_col_2_norm), "complex L2 norm");
    result.add_check(vanishes(complex_col_inf_norm), "complex Linf norm");

    return result;
}

auto test_matrix_access_in_range() -> TestResult {
    TestResult result;
    RandomInt rng_int(10, 15);

    auto size1{static_cast<std::size_t>(rng_int.generate())};
    auto size2{size1 + 2};
    std::size_t bands{3};

    auto real_val{[](std::size_t row, std::size_t col) -> Real {
        return std::sin(row)*std::cos(col);
    }};
    auto complex_val{[](std::size_t row, std::size_t col) -> Complex {
        return {std::sin(row)*std::cos(col), std::cos(row)};
    }};

    {
        Real real_sum{0};
        Complex complex_sum{0};

        BandedMatrix<Real> Ar{size1, bands};
        BandedMatrix<Complex> Az{size2, bands};

        for (std::size_t col{0}; col < size1; ++col) {
            for (std::size_t row{0}; row < size1; ++row) {
                const auto val{real_val(row, col)};
                if (Ar.set(row, col, val)) {
                    real_sum += val;
                }
            }
        }
        for (std::size_t col{0}; col < size2; ++col) {
            for (std::size_t row{0}; row < size2; ++row) {
                const auto val{complex_val(row, col)};
                if (Az.set(row, col, val)) {
                    complex_sum += val;
                }
            }
        }

        Real real_acc{0};
        for (std::size_t col{0}; col < size1; ++col) {
            for (std::size_t row{0}; row < size1; ++row) {
                real_acc += Ar[row, col];
            }
        }

        Complex complex_acc{0};
        for (std::size_t col{0}; col < size2; ++col) {
            for (std::size_t row{0}; row < size2; ++row) {
                complex_acc += Az[row, col];
            }
        }

        result.add_check(approx(real_sum, real_acc), "banded real");
        result.add_check(approx(complex_sum, complex_acc),
                "banded complex");
    }
    {
        Real real_sum{0};
        Complex complex_sum{0};

        DenseMatrix<Real> Ar{size1, size2};
        DenseMatrix<Complex> Az{size2, size1};

        for (std::size_t col{0}; col < size2; ++col) {
            for (std::size_t row{0}; row < size1; ++row) {
                const auto val{real_val(row, col)};
                Ar[row, col] = val;
                real_sum += val;
            }
        }
        for (std::size_t col{0}; col < size1; ++col) {
            for (std::size_t row{0}; row < size2; ++row) {
                const auto val{complex_val(row, col)};
                Az[row, col] = val;
                complex_sum += val;
            }
        }

        auto real_acc{std::accumulate(Ar.as_vector().begin(),
                Ar.as_vector().end(), Real{0})};
        auto complex_acc{std::accumulate(Az.as_vector().begin(),
                Az.as_vector().end(), Complex{0})};

        result.add_check(approx(real_sum, real_acc), "dense real");
        result.add_check(approx(complex_sum, complex_acc), "dense complex");
    }

    return result;
}

auto test_matrix_dense_columns() -> TestResult {
    TestResult result;
    RandomReal rng_real(0., 10.);
    RandomInt rng_int(10, 15);

    auto size1{static_cast<std::size_t>(rng_int.generate())};
    auto size2{size1 + 2};
    DenseMatrix<Real> Ar{size1, size2};
    DenseMatrix<Complex> Az{size1, size2};
    Vector<Real> vr(size1);
    Vector<Complex> vz(size1);
    randomise(rng_real, vr, vz);

    bool real_success{true}, complex_success{true};

    for (std::size_t col{0}; col < size2; ++col) {
        auto real_it{Ar.column_iterators(col)};
        std::transform(vr.begin(), vr.end(), real_it.first,
                [&](const Real& x) -> Real { return (col + 1.)*x; });
        auto complex_it{Az.column_iterators(col)};
        std::transform(vz.begin(), vz.end(), complex_it.first,
                [&](const Complex& z) -> Complex { return (col + 1.)*z; });
    }

    for (std::size_t col{0}; col < size2; ++col) {
        if (!approx(Ar.column_L2_norm(col), (col + 1)*vr.L2_norm())) {
            real_success = false;
        }
        if (!approx(Az.column_L2_norm(col), (col + 1)*vz.L2_norm())) {
            complex_success = false;
        }
    }

    result.add_check(real_success, "real");
    result.add_check(complex_success, "complex");

    return result;
}

auto test_matrix_access_fail() -> TestResult {
    TestResult result;
    RandomInt rng_int(10, 15);

    auto size1{static_cast<std::size_t>(rng_int.generate())};
    auto size2{size1 + 2};
    std::size_t bands{2};
    bool caught1{false}, caught2{false};

    BandedMatrix<Real> A{size1, bands};
    DenseMatrix<Real> B{size2, size1};

    try {
        (void)A.set(0, -1, 1.);
    } catch (RuntimeError<Range2DError>& e) {
        caught1 = true;
    }
    try {
        (void)A.set(0, size1, 1.);
    } catch (RuntimeError<Range2DError>& e) {
        caught2 = true;
    }

    result.add_check(caught1, "banded out of bounds column low");
    result.add_check(caught2, "banded out of bounds column high");
    caught1 = caught2 = false;

    try {
        (void)A.set(-1, 0, 1.);
    } catch (RuntimeError<Range2DError>& e) {
        caught1 = true;
    }
    try {
        (void)A.set(0, size1, 1.);
    } catch (RuntimeError<Range2DError>& e) {
        caught2 = true;
    }

    result.add_check(caught1, "banded out of bounds row low");
    result.add_check(caught2, "banded out of bounds row high");
    caught1 = caught2 = false;

    auto col{size1 - bands - 2};
    try {
        A.get_unsafe(col - 2*bands - 1, col) = 1.;
    } catch (RuntimeError<InvalidArgumentError>& e) {
        caught1 = true;
    }
    try {
        A.get_unsafe(col + bands + 1, col) = 1.;
    } catch (RuntimeError<InvalidArgumentError>& e) {
        caught2 = true;
    }

    result.add_check(caught1, "banded out of bands low");
    result.add_check(caught2, "banded out of bands high");
    caught1 = caught2 = false;

    try {
        B[0, -1] = 1.;
    } catch (RuntimeError<Range2DError>& e) {
        caught1 = true;
    }
    try {
        B[0, size1] = 1.;
    } catch (RuntimeError<Range2DError>& e) {
        caught2 = true;
    }

    result.add_check(caught1, "dense out of bounds column low");
    result.add_check(caught2, "dense out of bounds column high");
    caught1 = caught2 = false;

    try {
        B[-1, 0] = 1.;
    } catch (RuntimeError<Range2DError>& e) {
        caught1 = true;
    }
    try {
        B[size2, 0] = 1.;
    } catch (RuntimeError<Range2DError>& e) {
        caught2 = true;
    }

    result.add_check(caught1, "dense out of bounds row low");
    result.add_check(caught2, "dense out of bounds row high");

    return result;
}

