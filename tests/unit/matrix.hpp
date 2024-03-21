#include "jump/data/banded_matrix.hpp"
#include "jump/data/dense_matrix.hpp"
#include "jump/data/vector.hpp"
#include "jump/testing/testing.hpp"
#include "jump/utility/random.hpp"

using namespace jump;

TestResult test_matrix_initialise_basic();
TestResult test_matrix_initialise_chained();
TestResult test_matrix_initialise_fail();
TestResult test_matrix_multiply();
TestResult test_matrix_norms();
TestResult test_matrix_access_in_range();
TestResult test_matrix_dense_columns();
TestResult test_matrix_access_fail();

inline TestSuiteL1 matrix_tests() {
    TestSuiteL1 matrix_suite("matrix");
    std::vector<Test> tests;

    tests.push_back({"initialise"});
    tests.back().register_item({"basic", {}, &test_matrix_initialise_basic});
    tests.back().register_item({"chained", {},
            &test_matrix_initialise_chained});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"fail", {}, &test_matrix_initialise_fail});
#endif  // NDEBUG

    tests.push_back({"arithmetic"});
    tests.back().register_item({"multiply", {}, &test_matrix_multiply});

    tests.push_back({"summary"});
    tests.back().register_item({"norms", {}, &test_matrix_norms});

    tests.push_back({"access"});
    tests.back().register_item({"in range", {}, &test_matrix_access_in_range});
    tests.back().register_item({"columns", {}, &test_matrix_dense_columns});
    // These tests should throw exceptions which are disabled by the NDEBUG flag
#ifndef NDEBUG
    tests.back().register_item({"fail", {}, &test_matrix_access_fail});
#endif  // NDEBUG

    matrix_suite.register_items(tests);
    return matrix_suite;
}

inline TestResult test_matrix_initialise_basic() {
    TestResult result;

    RandomNumbers<int, std::uniform_int_distribution> rng_int_low(10, 15);
    RandomNumbers<int, std::uniform_int_distribution> rng_int_high(20, 25);
    RandomNumbers rng(0., 10.);

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

inline TestResult test_matrix_initialise_chained() {
    TestResult result;

    RandomNumbers<int, std::uniform_int_distribution> rng_int_low(10, 15);
    RandomNumbers<int, std::uniform_int_distribution> rng_int_high(20, 25);
    RandomNumbers rng(0., 15.);

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

inline TestResult test_matrix_initialise_fail() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_multiply() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_norms() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_access_in_range() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_dense_columns() {
    TestResult result;

    return result;
}

inline TestResult test_matrix_access_fail() {
    TestResult result;

    return result;
}


