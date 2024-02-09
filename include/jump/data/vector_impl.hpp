// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

/// \class Vector
/// Most operations are simply passed through to the underlying `std::vector`
/// storage, but arithmetic operations are implemented in two forms. Generally,
/// a simple default version for all basic `Vector` operations is provided, but
/// in the case of the preprocessor definition `JUMP_HAS_CBLAS` we will offload
/// to a local cblas implementation.

template <typename T>
inline Vector<T>::Vector(const std::size_t& size, const T& value) :
    m_storage(size, value) {
}

template <typename T>
inline Vector<T>::Vector(std::initializer_list<T>&& list) :
    m_storage(std::forward<std::initializer_list<T>>(list)) {
}

/// Useful when extracting columns from a matrix, for example. Combined with
/// a way of getting iterators to the internal storage locations
/// representing the top and bottom of a given matrix column, this is the
/// fastest way of obtaining that isolated column.
template <typename T>
template <class InputIt>
inline Vector<T>::Vector(InputIt first, InputIt last) :
    m_storage(std::move(first), std::move(last)) {
}

/// Initialises a complex-valued `Vector` of the correct size and loops
/// through it to convert the real-valued elements in the source `Vector`.
/// This is not terribly efficient but the only way of achieving the
/// promotion easily.
template <>
inline Vector<Real>::operator Vector<Complex>() const {
    const std::size_t N{size()};
    Vector<Complex> result(N);
    for (std::size_t i{0}; i < N; ++i) {
        result[i] = m_storage[i];
    }
    return result;
}

template <typename T>
inline const T& Vector<T>::operator[](std::size_t index) const {
#ifndef NDEBUG
    if (index >= size())
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
#endif  // NDEBUG

    return m_storage[index];
}

template <typename T>
inline T& Vector<T>::operator[](std::size_t index) {
#ifndef NDEBUG
    if (index >= size())
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
#endif  // NDEBUG

    return m_storage[index];
}

template <typename T>
inline void Vector<T>::assign(std::size_t size, const T& value) {
    m_storage.assign(size, value);
}

template <typename T>
template <class InputIt>
inline void Vector<T>::assign(InputIt first, InputIt last) {
    m_storage.assign(std::move(first), std::move(last));
}

template <typename T>
inline void Vector<T>::reserve(std::size_t size) {
    m_storage.reserve(size);
}

template <typename T>
inline void Vector<T>::clear() {
    m_storage.clear();
}

template <typename T>
inline void Vector<T>::resize(std::size_t size) {
    m_storage.resize(size);
}

template <typename T>
inline std::size_t Vector<T>::size() const {
    return m_storage.size();
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::begin() const {
    return m_storage.begin();
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::end() const {
    return m_storage.end();
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::begin() {
    return m_storage.begin();
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::end() {
    return m_storage.end();
}

template <typename T>
inline void Vector<T>::fill(const T& value) {
    std::ranges::fill(m_storage, value);
}

template <typename T>
inline void Vector<T>::zero() {
    fill(T{0});
}

template <typename T>
inline const Vector<T>& Vector<T>::operator+() const {
    return *this;
}

template <typename T>
inline Vector<T> Vector<T>::operator-() const {
    Vector<T> temp{*this};
    return temp *= -1.;
}

template <typename T>
inline Vector<T>& Vector<T>::operator+=(const Vector<T>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size())
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
#endif  // NDEBUG

    for (std::size_t i{0}, N{size()}; i < N; ++i) {
        m_storage[i] += rhs[i];
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator-=(const Vector<T>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size())
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
#endif  // NDEBUG

    for (std::size_t i{0}, N{size()}; i < N; ++i) {
        m_storage[i] -= rhs[i];
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator*=(const T& rhs) {
    for (auto& x : m_storage) {
        x *= rhs;
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator/=(const T& rhs) {
    return *this *= (T{1}/rhs);
}

template <typename T>
inline Real Vector<T>::L1_norm() const {
    auto F = [](T acc, const T& x) { return acc + std::abs(x); };
    return std::ranges::fold_left(m_storage, T{0}, F);
}

template <typename T>
inline Real Vector<T>::L2_norm() const {
    auto F = [](T acc, const T& x) { return acc + std::pow(std::abs(x), 2.); };
    return std::sqrt(std::ranges::fold_left(m_storage, T{0}, F));
}

template <typename T>
inline Real Vector<T>::Linf_norm() const {
    auto F = [](const T& x) { return std::abs(x); };
    return std::ranges::max(m_storage, {}, F);
}

template <typename T>
inline T* Vector<T>::data() {
    return m_storage.data();
}

template <typename T>
inline const T* Vector<T>::data() const {
    return m_storage.data();
}

// ============================================================================
// Non-member functions
// ============================================================================

/// \relates Vector
/// \brief Addition of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
template <typename T>
inline Vector<T> operator+(Vector<T> lhs, const Vector<T>& rhs) {
    lhs += rhs;
    return lhs;
}

/// \relates Vector
/// \brief Addition of two Vectors.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
template <typename T>
inline Vector<T> operator+(const Vector<T>& lhs, Vector<T>&& rhs) {
    rhs += lhs;
    return rhs;
}

/// \relates Vector
/// \brief Difference of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
template <typename T>
inline Vector<T> operator-(Vector<T> lhs, const Vector<T>& rhs) {
    lhs -= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Difference of two Vectors.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
template <typename T>
inline Vector<T> operator-(const Vector<T>& lhs, Vector<T>&& rhs) {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates Vector
/// \brief Left-hand multiplication by scalar.
template <typename T>
inline Vector<T> operator*(const T& lhs, Vector<T> rhs) {
    rhs *= lhs;
    return rhs;
}

/// \relates Vector
/// \brief Right-hand multiplication by scalar.
template <typename T>
inline Vector<T> operator*(Vector<T> lhs, const T& rhs) {
    lhs *= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Inner product of two Vectors.
template <typename T>
inline T operator*(const Vector<T>& lhs, const Vector<T>& rhs) {
#ifndef NDEBUG
    if (lhs.size() != rhs.size())
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
#endif  // NDEBUG

    T dot_product{0};
    for (std::size_t i{0}, N{lhs.size()}; i < N; ++i) {
        dot_product += lhs[i]*rhs[i];
    }
    return dot_product;
}

/// \relates Vector
/// \brief Right-hand division by scalar.
template <typename T>
inline Vector<T> operator/(Vector<T> lhs, const T& rhs) {
    lhs /= rhs;
    return lhs;
}

/// Note: takes string data by value so we can optimise in the case of quickly
/// reading from a file, no copies are taken.
template <typename T>
inline void Vector<T>::operator<<(std::string data) {
    T element;
    std::stringstream ss(std::move(data));
    std::vector<T> new_data;

    // After each extraction, use `std::basic_ios::operator bool` to check
    // failbit, at which point we'll assume we have everything
    while (ss >> element)
        new_data.push_back(element);

    // Don't leave m_storage in an in-between state if anything happens
    m_storage = std::move(new_data);
}

/// Outputs `Vector` data to output stream in a single line with spaces.
template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vector<T>& rhs) {
    for (const auto& x : rhs)
        out << x << ' ';
    return out << std::endl;
}

// ============================================================================
// CBLAS
// ============================================================================

#ifdef JUMP_HAS_CBLAS
/// \brief Specialisation of in-place addition of two real Vectors, using CBLAS.
template <>
inline Vector<Real>& Vector<Real>::operator+=(const Vector<Real>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size())
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
#endif  // NDEBUG

    // Computes 1.*rhs + *this (with a pointer shift of 1 between elements)
    cblas_daxpy(m_storage.size(), 1., rhs.m_storage.data(), 1, m_storage.data(),
            1);
    return *this;
}

/// \brief Specialisation of in-place addition of two complex Vectors, using
/// CBLAS.
template <>
inline Vector<Complex>& Vector<Complex>::operator+=(
        const Vector<Complex>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size())
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
#endif  // NDEBUG

    // Computes 1.*rhs + *this (with a pointer shift of 1 between elements)
    Complex a {1., 0.};
    cblas_zaxpy(m_storage.size(), &a, rhs.m_storage.data(), 1, m_storage.data(),
            1);
    return *this;
}

/// \brief Specialisation of in-place subtraction of two real Vectors, using
/// CBLAS.
template <>
inline Vector<Real>& Vector<Real>::operator-=(const Vector<Real>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size())
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
#endif  // NDEBUG

    // Computes -1.*rhs + *this (with a pointer shift of 1 between elements)
    cblas_daxpy(m_storage.size(), -1., rhs.m_storage.data(), 1,
            m_storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place subtraction of two complex Vectors, using
/// CBLAS.
template <>
inline Vector<Complex>& Vector<Complex>::operator-=(
        const Vector<Complex>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size())
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
#endif  // NDEBUG

    // Computes -1.*rhs + *this (with a pointer shift of 1 between elements)
    Complex a {-1., 0.};
    cblas_zaxpy(m_storage.size(), &a, rhs.m_storage.data(), 1, m_storage.data(),
            1);
    return *this;
}

/// \brief Specialisation of in-place multiplication of a real `Vector` by a
/// real scalar, using CBLAS.
template <>
inline Vector<Real>& Vector<Real>::operator*=(const Real& rhs) {
    // Computes rhs*(*this) (with a pointer shift of 1 between elements)
    cblas_dscal(m_storage.size(), rhs, m_storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place multiplication of a complex `Vector` by a
/// complex scalar, using CBLAS.
template <>
inline Vector<Complex>& Vector<Complex>::operator*=(const Complex& rhs) {
    // Computes rhs*(*this) (with a pointer shift of 1 between elements)
    cblas_zscal(m_storage.size(), &rhs, m_storage.data(), 1);
    return *this;
}

/// \brief Specialisation of the dot product for two real Vectors, using CBLAS.
inline Real operator*(const Vector<Real>& lhs, const Vector<Real>& rhs) {
#ifndef NDEBUG
    if (lhs.size() != rhs.size())
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
#endif  // NDEBUG

    // Computes dot product lhs*rhs (with a pointer shift of 1 between elements)
    return cblas_ddot(lhs.size(), lhs.data(), 1, rhs.data(), 1);
}

/// \brief Specialisation of the dot product for two complex Vectors, using
/// CBLAS.
inline Complex operator*(const Vector<Complex>& lhs,
        const Vector<Complex>& rhs) {
#ifndef NDEBUG
    if (lhs.size() != rhs.size())
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
#endif  // NDEBUG

    // Computes dot product lhs*rhs (with a pointer shift of 1 between elements)
    Complex result;
    cblas_zdotu_sub(lhs.size(), lhs.data(), 1, rhs.data(), 1, &result);
    return result;
}

/// \brief Specialisation of in-place division of a real `Vector` by a real
/// scalar, using CBLAS.
template <>
inline Vector<Real>& Vector<Real>::operator/=(const Real& rhs) {
    // Computes (1./rhs)*(*this) (with a pointer shift of 1 between elements)
    cblas_dscal(m_storage.size(), 1./rhs, m_storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place division of a complex `Vector` by a
/// complex scalar using CBLAS.
template <>
inline Vector<Complex>& Vector<Complex>::operator/=(const Complex& rhs) {
    // Computes (1./rhs)*(*this) (with a pointer shift of 1 between elements)
    Complex a{1./rhs};
    cblas_zscal(m_storage.size(), &a, m_storage.data(), 1);
    return *this;
}

/// \brief Specialisation of the L1-norm calculation for a real `Vector`, using
/// CBLAS.
template <>
inline double Vector<Real>::L1_norm() const {
    // Computes sum of absolute element values (with a pointer shift of 1
    // between elements)
    return cblas_dasum(m_storage.size(), m_storage.data(), 1);
}

/// \brief Specialisation of the L1-norm calculation for a complex `Vector`,
/// using CBLAS.
template <>
inline double Vector<Complex>::L1_norm() const {
    // Computes sum of absolute real and imaginary element values (with a
    // pointer shift of 1 between elements). Note: this is not the same as the
    // usual definition of this norm
    return cblas_dzasum(m_storage.size(), m_storage.data(), 1);
}

/// \brief Specialisation of the L2-norm calculation for a real `Vector`, using
/// CBLAS.
template <>
inline double Vector<Real>::L2_norm() const {
    // Computes the Euclidean norm of the `Vector` (with a pointer shift of 1
    // between elements)
    return cblas_dnrm2(m_storage.size(), m_storage.data(), 1);
}

/// \brief Specialisation of the L2-norm calculation for a complex `Vector`,
/// using CBLAS.
template <>
inline double Vector<Complex>::L2_norm() const {
    // Computes the Euclidean norm of the `Vector` (with a pointer shift of 1
    // between elements)
    return cblas_dznrm2(m_storage.size(), m_storage.data(), 1);
}
#endif  // JUMP_HAS_CBLAS

