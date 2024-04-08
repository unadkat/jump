// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_VECTOR_HPP
#define JUMP_VECTOR_HPP

#include "jump/data/vector_decl.hpp"

namespace jump {
/// \class Vector
/// Most operations are simply passed through to the underlying `std::vector`
/// storage, but arithmetic operations are implemented in two forms. Generally,
/// a simple default version for all basic `Vector` operations is provided, but
/// in the case of the preprocessor definition `JUMP_HAS_CBLAS` we will offload
/// to a local cblas implementation.

template <typename T>
inline Vector<T>::Vector(std::size_t size, const T& value) :
    storage(size, value) {
}

template <typename T>
inline Vector<T>::Vector(std::initializer_list<T>&& list) :
    storage(std::forward<std::initializer_list<T>>(list)) {
}

/// Useful when extracting columns from a matrix, for example. Combined with a
/// way of getting iterators to the internal storage locations representing the
/// top and bottom of a given matrix column, this is the fastest way of
/// obtaining that isolated column.
template <typename T>
template <class InputIt>
inline Vector<T>::Vector(InputIt first, InputIt last) :
    storage(std::move(first), std::move(last)) {
}

/// Initialises a complex-valued `Vector` of the correct size and loops through
/// it to convert the real-valued elements in the source `Vector`. This is not
/// terribly efficient but the only way of achieving the promotion easily.
template <>
inline Vector<Real>::operator Vector<Complex>() const {
    const std::size_t N{size()};
    Vector<Complex> result(N);
    for (std::size_t i{0}; i < N; ++i) {
        result[i] = Complex{storage[i]};
    }
    return result;
}

template <typename T>
inline void Vector<T>::assign(std::size_t size, const T& value) {
    storage.assign(size, value);
}

template <typename T>
template <class InputIt>
inline void Vector<T>::assign(InputIt first, InputIt last) {
    storage.assign(std::move(first), std::move(last));
}

template <typename T>
inline void Vector<T>::clear() {
    storage.clear();
}

template <typename T>
inline void Vector<T>::resize(std::size_t size) {
    storage.resize(size);
}

template <typename T>
inline std::size_t Vector<T>::size() const {
    return storage.size();
}

template <typename T>
inline const T& Vector<T>::operator[](std::size_t index) const {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return storage[index];
}

template <typename T>
inline T& Vector<T>::operator[](std::size_t index) {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return storage[index];
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::begin() const {
    return storage.cbegin();
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::end() const {
    return storage.cend();
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::begin() {
    return storage.begin();
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::end() {
    return storage.end();
}

template <typename T>
inline void Vector<T>::fill(const T& value) {
    std::ranges::fill(storage, value);
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
    return temp *= -1;
}

template <typename T>
inline Vector<T>& Vector<T>::operator+=(const Vector<T>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}, N{size()}; i < N; ++i) {
        storage[i] += rhs[i];
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator-=(const Vector<T>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}, N{size()}; i < N; ++i) {
        storage[i] -= rhs[i];
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator*=(const T& rhs) {
    for (auto& x : storage) {
        x *= rhs;
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator*=(const Vector<T>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}, N{size()}; i < N; ++i) {
        storage[i] *= rhs[i];
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator/=(const T& rhs) {
    return *this *= (T{1}/rhs);
}

template <typename T>
inline Vector<T>& Vector<T>::operator/=(const Vector<T>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}, N{size()}; i < N; ++i) {
        storage[i] /= rhs[i];
    }
    return *this;
}

template <typename T>
inline Real Vector<T>::L1_norm() const {
    if constexpr (is_dual_v<T>) {
        auto F{[](Real acc, const T& x) { return acc + abs(x).value; }};
        return std::accumulate(storage.begin(), storage.end(), Real{0}, F);
    } else {
        auto F{[](Real acc, const T& x) { return acc + std::abs(x); }};
        return std::accumulate(storage.begin(), storage.end(), Real{0}, F);
    }
}

template <typename T>
inline Real Vector<T>::L2_norm() const {
    if constexpr (is_dual_v<T>) {
        auto F{[](Real acc, const T& x) {
            return acc + pow(abs(x), 2.).value; }};
        return std::sqrt(std::accumulate(storage.begin(), storage.end(),
                    Real{0}, F));
    } else {
        auto F{[](Real acc, const T& x) { return acc + std::pow(std::abs(x),
                2.); }};
        return std::sqrt(std::accumulate(storage.begin(), storage.end(),
                    Real{0}, F));
    }
}

template <typename T>
inline Real Vector<T>::Linf_norm() const {
    if constexpr (is_dual_v<T>) {
        auto F{[](const T& x) { return abs(x).value; }};
        return F(std::ranges::max(storage, {}, F));
    } else {
        auto F{[](const T& x) { return std::abs(x); }};
        return F(std::ranges::max(storage, {}, F));
    }
}

template <typename T>
inline T* Vector<T>::data() {
    return storage.data();
}

template <typename T>
inline const T* Vector<T>::data() const {
    return storage.data();
}

/// Note: takes string data by value so we can optimise in the case of quickly
/// reading from a file, no copies are taken.
template <typename T>
inline void Vector<T>::operator<<(std::string data) {
    T element;
    std::istringstream iss(std::move(data));
    std::vector<T> new_data;

    // After each extraction, use `std::basic_ios::operator bool` to check
    // failbit, at which point we'll assume we have everything
    while (iss >> element) {
        new_data.push_back(element);
    }

    // Don't leave storage in an in-between state if anything happens
    storage = std::move(new_data);
}

/// \relates Vector
template <typename T, typename Os>
inline Os& operator<<(Os& out, const Vector<T>& rhs) {
    for (const auto& x : rhs) {
        out << x << ' ';
    }
    return out;
}

// ========================================================================
// Exponentiation
// ========================================================================

/// \relates Vector
template <typename T>
Vector<T> exp(Vector<T> v) {
    using std::exp;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return exp(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> log(Vector<T> v) {
    using std::log;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return log(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> pow(Vector<T> v, T p) {
    using std::pow;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return pow(x, p); });
    return v;
}

// ========================================================================
// Trigonometry
// ========================================================================

/// \relates Vector
template <typename T>
Vector<T> sin(Vector<T> v) {
    using std::sin;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return sin(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> cos(Vector<T> v) {
    using std::cos;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return cos(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> tan(Vector<T> v) {
    using std::tan;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return tan(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> asin(Vector<T> v) {
    using std::asin;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return asin(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> acos(Vector<T> v) {
    using std::acos;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return acos(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> atan(Vector<T> v) {
    using std::atan;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return atan(x); });
    return v;
}

// ========================================================================
// Hyperbolics
// ========================================================================

/// \relates Vector
template <typename T>
Vector<T> sinh(Vector<T> v) {
    using std::sinh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return sinh(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> cosh(Vector<T> v) {
    using std::cosh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return cosh(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> tanh(Vector<T> v) {
    using std::tanh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return tanh(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> asinh(Vector<T> v) {
    using std::asinh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return asinh(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> acosh(Vector<T> v) {
    using std::acosh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return acosh(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> atanh(Vector<T> v) {
    using std::atanh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return atanh(x); });
    return v;
}

// ========================================================================
// Miscellaneous
// ========================================================================

/// \relates Vector
template <typename T>
Vector<T> abs(Vector<T> v) {
    using std::abs;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return abs(x); });
    return v;
}

/// \relates Vector
template <typename T>
Vector<T> sgn(Vector<T> v) {
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return sgn(x); });
    return v;
}

// ========================================================================
// CBLAS
// ========================================================================

#ifdef JUMP_HAS_CBLAS
/// \brief Specialisation of in-place addition of two real Vectors, using CBLAS.
template <>
inline Vector<Real>& Vector<Real>::operator+=(const Vector<Real>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // Computes 1.*rhs + *this (with a pointer shift of 1 between elements)
    cblas_daxpy(storage.size(), 1., rhs.storage.data(), 1, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place addition of two complex Vectors, using
/// CBLAS.
template <>
inline Vector<Complex>& Vector<Complex>::operator+=(
        const Vector<Complex>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // Computes 1.*rhs + *this (with a pointer shift of 1 between elements)
    Complex a {1.};
    cblas_zaxpy(storage.size(), &a, rhs.storage.data(), 1, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place subtraction of two real Vectors, using
/// CBLAS.
template <>
inline Vector<Real>& Vector<Real>::operator-=(const Vector<Real>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // Computes -1.*rhs + *this (with a pointer shift of 1 between elements)
    cblas_daxpy(storage.size(), -1., rhs.storage.data(), 1, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place subtraction of two complex Vectors, using
/// CBLAS.
template <>
inline Vector<Complex>& Vector<Complex>::operator-=(
        const Vector<Complex>& rhs) {
#ifndef NDEBUG
    if (size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "rhs",
            .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // Computes -1.*rhs + *this (with a pointer shift of 1 between elements)
    Complex a {-1.};
    cblas_zaxpy(storage.size(), &a, rhs.storage.data(), 1, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place multiplication of a real `Vector` by a
/// real scalar, using CBLAS.
template <>
inline Vector<Real>& Vector<Real>::operator*=(const Real& rhs) {
    // Computes rhs*(*this) (with a pointer shift of 1 between elements)
    cblas_dscal(storage.size(), rhs, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place multiplication of a complex `Vector` by a
/// complex scalar, using CBLAS.
template <>
inline Vector<Complex>& Vector<Complex>::operator*=(const Complex& rhs) {
    // Computes rhs*(*this) (with a pointer shift of 1 between elements)
    cblas_zscal(storage.size(), &rhs, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place division of a real `Vector` by a real
/// scalar, using CBLAS.
template <>
inline Vector<Real>& Vector<Real>::operator/=(const Real& rhs) {
    // Computes (1./rhs)*(*this) (with a pointer shift of 1 between elements)
    cblas_dscal(storage.size(), 1./rhs, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place division of a complex `Vector` by a
/// complex scalar using CBLAS.
template <>
inline Vector<Complex>& Vector<Complex>::operator/=(const Complex& rhs) {
    // Computes (1./rhs)*(*this) (with a pointer shift of 1 between elements)
    Complex a{1./rhs};
    cblas_zscal(storage.size(), &a, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of the L1-norm calculation for a real `Vector`, using
/// CBLAS.
template <>
inline Real Vector<Real>::L1_norm() const {
    // Computes sum of absolute element values (with a pointer shift of 1
    // between elements)
    return cblas_dasum(storage.size(), storage.data(), 1);
}

/// \brief Specialisation of the L1-norm calculation for a complex `Vector`,
/// using CBLAS.
template <>
inline Real Vector<Complex>::L1_norm() const {
    // Computes sum of absolute real and imaginary element values (with a
    // pointer shift of 1 between elements). Note: this is not the same as the
    // usual definition of this norm
    return cblas_dzasum(storage.size(), storage.data(), 1);
}

/// \brief Specialisation of the L2-norm calculation for a real `Vector`, using
/// CBLAS.
template <>
inline Real Vector<Real>::L2_norm() const {
    // Computes the Euclidean norm of the `Vector` (with a pointer shift of 1
    // between elements)
    return cblas_dnrm2(storage.size(), storage.data(), 1);
}

/// \brief Specialisation of the L2-norm calculation for a complex `Vector`,
/// using CBLAS.
template <>
inline Real Vector<Complex>::L2_norm() const {
    // Computes the Euclidean norm of the `Vector` (with a pointer shift of 1
    // between elements)
    return cblas_dznrm2(storage.size(), storage.data(), 1);
}
#endif  // JUMP_HAS_CBLAS
}   // namespace jump

#endif  // JUMP_VECTOR_HPP

