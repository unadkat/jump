// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

/// \class DenseMatrix
/// Implements the internal storage of a dense matrix by storing all elements of
/// an \f$m\f$ by \f$n\f$ matrix as a contiguous `Vector` of length \f$nm\f$. In
/// particular, the element in the matrix occupying row \f$i\f$th and column
/// \f$j\f$ is found at index \f$im+j\f$ in the underlying storage (see
/// documentation for DenseMatrix::m_storage).
///
/// Most operations are simply passed through in some form to the underlying
/// `Vector` storage, including simple element-wise arithmetic operations.
/// `DenseMatrix`-`Vector` operations are implemented separately. Generally, a
/// simple default version is provided, but in the case of the preprocessor
/// definition `JUMP_HAS_CBLAS` existing we instead use an appropriate CBLAS
/// routine.

/// \var DenseMatrix::m_storage
/// The internal storage for a dense matrix consists of a contiguous `Vector`
/// containing `num_rows()*#num_columns()` elements. The matrix elements are
/// stored in column-major format, so that the element at row `i` and column `j`
/// appears at location `m_storage[j*num_rows() + i]`.

template <typename T>
inline DenseMatrix<T>::DenseMatrix(std::size_t size) :
    MatrixBase<T>{size},
    m_storage(size*size, T{0}) {
}

template <typename T>
inline DenseMatrix<T>::DenseMatrix(std::size_t num_rows,
        std::size_t num_columns) :
    MatrixBase<T>{num_rows, num_columns},
    m_storage(num_rows*num_columns, T{0}) {
}

///// Initialises a complex-valued `DenseMatrix` of the correct size and delegates
///// conversion of elements to the underlying `Vector`.
//template <>
//inline DenseMatrix<Real>::operator DenseMatrix<Complex>() const
//{
//    DenseMatrix<Complex> result(num_rows(), num_columns()); 
//    return result.storage = {m_storage};
//}

/// The element at row `i` and column `j` appears at location
/// `m_storage[j*num_rows() + i]` (see `DenseMatrix::m_storage`).
template <typename T>
inline const T& DenseMatrix<T>::operator[](std::size_t row,
        std::size_t column) const {
#ifndef NDEBUG
    if (row >= this->num_rows() || column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    return m_storage[column*this->num_rows() + row];
}

/// The element at row `i` and column `j` appears at location
/// `m_storage[j*num_rows() + i]` (see `DenseMatrix::m_storage`).
template <typename T>
inline T& DenseMatrix<T>::operator[](std::size_t row, std::size_t column) {
#ifndef NDEBUG
    if (row >= this->num_rows() || column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {row, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    return m_storage[column*this->num_rows() + row];
}

template <typename T>
inline void DenseMatrix<T>::assign(std::size_t size) {
    MatrixBase<T>::initialise(size);
    m_storage.assign(this->num_rows()*this->num_columns(), T{0});
}

template <typename T>
inline void DenseMatrix<T>::assign(std::size_t num_rows,
        std::size_t num_columns) {
    MatrixBase<T>::initialise(num_rows, num_columns);
    m_storage.assign(this->num_rows()*this->num_columns(), T{0});
}

template <typename T>
template <class InputIt>
inline void DenseMatrix<T>::assign(InputIt first, InputIt last) {
#ifndef NDEBUG
    if (last != first + m_storage.size()) {
        throw RuntimeError{InvalidArgumentError{.argument = "last", .value =
            std::format("first + {}", last - first),
            .expected = "first - last == m_storage.size()"}};
    }
#endif  // NDEBUG

    m_storage.assign(std::move(first), std::move(last));
}

template <typename T>
inline std::size_t DenseMatrix<T>::num_elements() const {
    return m_storage.size();
}

template <typename T>
inline typename DenseMatrix<T>::ConstIterator DenseMatrix<T>::begin() const {
    return m_storage.begin();
}

template <typename T>
inline typename DenseMatrix<T>::ConstIterator DenseMatrix<T>::end() const {
    return m_storage.end();
}

template <typename T>
inline typename DenseMatrix<T>::Iterator DenseMatrix<T>::begin() {
    return m_storage.begin();
}

template <typename T>
inline typename DenseMatrix<T>::Iterator DenseMatrix<T>::end() {
    return m_storage.end();
}

/// Keeping in mind that the data is stored internally in column-major format,
/// the beginning of column `j` is at index `num_rows()*j` while the end of the
/// column is at index `num_rows()*(j + 1)`.
template <typename T>
inline std::pair<typename DenseMatrix<T>::Iterator,
    typename DenseMatrix<T>::Iterator>
DenseMatrix<T>::column_iterators(std::size_t column) {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    return {m_storage.begin() + this->m_num_rows*column,
            m_storage.begin() + this->m_num_rows*(column + 1)};
}

/// Keeping in mind that the data is stored internally in column-major format,
/// the beginning of column `j` is at index `num_rows()*j` while the end of the
/// column is at index `num_rows()*(j + 1)`.
template <typename T>
inline std::pair<typename DenseMatrix<T>::ConstIterator,
       typename DenseMatrix<T>::ConstIterator> DenseMatrix<T>::column_iterators(
               std::size_t column) const {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    return {m_storage.begin() + this->m_num_rows*column,
            m_storage.begin() + this->m_num_rows*(column + 1)};
}

template <typename T>
inline void DenseMatrix<T>::fill(const T& value) {
    m_storage.fill(value);
}

template <typename T>
inline void DenseMatrix<T>::zero() {
    fill(T{0});
}

template <typename T>
inline const DenseMatrix<T>& DenseMatrix<T>::operator+() const {
    return *this;
}

template <typename T>
inline DenseMatrix<T> DenseMatrix<T>::operator-() const {
    DenseMatrix<T> temp{*this};
    temp *= -1;
    return temp;
}

template <typename T>
inline DenseMatrix<T>& DenseMatrix<T>::operator+=(const DenseMatrix& rhs) {
#ifndef NDEBUG
    if (this->size() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    m_storage += rhs.m_storage;
    return *this;
}

template <typename T>
inline DenseMatrix<T>& DenseMatrix<T>::operator-=(const DenseMatrix& rhs) {
#ifndef NDEBUG
    if (this->size() != rhs.size()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    m_storage -= rhs.m_storage;
    return *this;
}

template <typename T>
inline DenseMatrix<T>& DenseMatrix<T>::operator*=(const T& k) {
    m_storage *= k;
    return *this;
}

template <typename T>
inline DenseMatrix<T>& DenseMatrix<T>::operator*=(const DenseMatrix<T>& rhs) {
#ifndef NDEBUG
    if (this->num_columns() != rhs.num_rows()) {
        throw RuntimeError{Mismatch2DError{.size1 = this->size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // TODO: ranges
    DenseMatrix<T> result(this->num_rows(), rhs.num_columns());
    for (std::size_t row(0); row < this->num_rows(); ++row)
        for (std::size_t col(0); col < rhs.num_columns(); ++col)
            for (std::size_t i(0); i < this->num_columns(); ++i)
                result(row, col) += operator[](row, i)*rhs(i, col);

    this->m_num_columns = rhs.num_columns();
    m_storage = std::move(result.m_storage);
    return *this;
}

template <typename T>
inline DenseMatrix<T>& DenseMatrix<T>::operator/=(const T& k) {
    m_storage /= k;
    return *this;
}

template <typename T>
inline double DenseMatrix<T>::column_L1_norm(std::size_t column) const {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    auto it{column_iterators(column)};
    auto F{[](T acc, const T& x) { return acc + std::abs(x); }};
    return std::ranges::fold_left(it.first, it.second, T{0}, F);
}

template <typename T>
inline double DenseMatrix<T>::column_L2_norm(std::size_t column) const {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    auto it{column_iterators(column)};
    auto F{[](T acc, const T& x) { return acc + std::pow(std::abs(x), 2.); }};
    return std::sqrt(std::ranges::fold_left(it.first, it.second, T{0}, F));
}

template <typename T>
inline double DenseMatrix<T>::column_Linf_norm(std::size_t column) const {
#ifndef NDEBUG
    if (column >= this->num_columns()) {
        throw RuntimeError{Range2DError{.indices = {0, column},
            .size = this->size()}};
    }
#endif  // NDEBUG

    auto it{column_iterators(column)};
    auto F{[](const T& x) { return std::abs(x); }};
    return std::ranges::max(it.first, it.second, {}, F);
}

template <typename T>
inline T* DenseMatrix<T>::data() {
    return m_storage.data();
}

template <typename T>
inline const T* DenseMatrix<T>::data() const {
    return m_storage.data();
}

/// Loads a block of data from the given string by reading all valid values into
/// the underlying storage. The only thing that will be checked is that the
/// total number of values read is equal to `num_elements()`. Note that this
/// means there is no restriction on the formatting of the data, it may appear
/// as a series of equal-length lines (each line corresponding to a column), a
/// single line, or even lines of unequal length.
/// TODO: add metadata to load arbitrary matrices
template <typename T>
inline void DenseMatrix<T>::operator<<(std::string data) {
    Vector<T> new_data;
    new_data << std::move(data);
    if (new_data.size() != m_storage.size()) {
        throw RuntimeError{InvalidArgumentError{.argument = "data",
            .value = std::format("(matrix data with a total of {} elements)",
                    new_data.size()),
            .expected = std::format("matrix data with a total of {} x {} = {} "
                    "elements", this->num_rows(), this->num_columns(),
                    this->num_rows()*this->num_columns())}};
    }

    m_storage = std::move(new_data);
}

/// Iterates through columns of matrix and dumps them to a string, formatted
/// as the transpose.
template <typename T>
inline std::string DenseMatrix<T>::as_string() const
{
    std::ostringstream oss;
    auto inserter{[&oss](const auto& x) { oss << x << ' '; }};
    for (std::size_t i{0}, num_cols{this->num_columns()}; i < num_cols; ++i) {
        auto it{column_iterators(i)};
        std::ranges::for_each(it.first, it.second, inserter);
        oss << '\n';
    }

    return oss.str();
}

template <typename T>
inline DenseMatrix<T> DenseMatrix<T>::identity(std::size_t size)
{
    DenseMatrix<T> I{size};
    for (std::size_t i{0}; i < size; ++i)
        I[i, i] = T{1};
    return I;
}

