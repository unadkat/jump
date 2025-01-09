// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_VECTOR_HPP
#define JUMP_VECTOR_HPP

#include "jump/autodiff/dual.hpp"
#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/external.hpp"
#include "jump/utility/types.hpp"
#include "jump/utility/utility.hpp"

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace jump {
/// \brief Permissive encapulation of `std::vector` with arithmetic
/// operators enabled.
template <typename T>
struct Vector {
    /// \brief Internal contiguous storage.
    std::vector<T> storage;

    /// \brief Iterator for algorithms.
    using Iterator = typename std::vector<T>::iterator;
    /// \brief Iterator for algorithms.
    using ConstIterator = typename std::vector<T>::const_iterator;

    /// \brief Construct a `Vector` with a given size (empty by default) filled
    /// with the given item (`T{0}` by default).
    explicit Vector(std::size_t size = 0, const T& value = T{0});
    /// \brief Construct a `Vector` from a brace-enclosed list.
    Vector(std::initializer_list<T>&& list);
    /// \brief Default copy constructor
    Vector(const Vector& other) = default;
    /// \brief Templated copy constructor
    template <typename U>
    Vector(const Vector<U>& other);
    /// \brief Construct a `Vector` via a pair of iterators.
    template <typename InputIt>
    Vector(InputIt first, InputIt last);
    /// \brief Default move constructor
    Vector(Vector&& other) = default;

    /// \brief Default copy assignment
    auto operator=(const Vector& other) -> Vector& = default;
    /// \brief Default move assignment
    auto operator=(Vector&& other) -> Vector& = default;

    /// \brief Set size and fill with a given value.
    void assign(std::size_t size, const T& value = T{0});
    /// \brief Set data via pair of iterators.
    template <class InputIt>
    void assign(InputIt first, InputIt last);
    /// \brief Empty the `Vector`.
    void clear();
    /// \brief Resize the `Vector`.
    void resize(std::size_t size);
    /// \brief Return number of elements.
    auto size() const -> std::size_t;

    /// \brief Defaulted spaceship operator.
    auto operator<=>(const Vector&) const = default;

    /// \brief Const element access.
    auto operator[](std::size_t index) const -> const T&;
    /// \brief Mutable element access.
    auto operator[](std::size_t index) -> T&;

    /// \brief Const begin iterator for algorithms.
    auto begin() const -> ConstIterator;
    /// \brief Const past-the-end iterator for algorithms.
    auto end() const -> ConstIterator;
    /// \brief Begin iterator for algorithms.
    auto begin() -> Iterator;
    /// \brief Past-end-end iterator for algorithms.
    auto end() -> Iterator;

    /// \brief Fill with given value.
    void fill(const T& value);
    /// \brief Fill vector with zeroes.
    void zero();

    /// \brief No operation on `Vector`.
    auto operator+() const -> const Vector&;
    /// \brief Negate `Vector`.
    auto operator-() const -> Vector;
    /// \brief Add two Vectors together in place.
    auto operator+=(const Vector& rhs) -> Vector&;
    /// \brief Subtract one `Vector` from another in place.
    auto operator-=(const Vector& rhs) -> Vector&;
    /// \brief Multiply by scalar in place.
    auto operator*=(const T& rhs) -> Vector&;
    /// \brief Elementwise product by another Vector.
    auto operator*=(const Vector& rhs) -> Vector&;
    /// \brief Divide by scalar in place.
    auto operator/=(const T& rhs) -> Vector&;
    /// \brief Elementwise division by another Vector.
    auto operator/=(const Vector& rhs) -> Vector&;

    /// \brief Return sum of element magnitudes.
    auto L1_norm() const -> Real;
    /// \brief Return Euclidean norm.
    auto L2_norm() const -> Real;
    /// \brief Return maximum magnitude over all elements.
    auto Linf_norm() const -> Real;

    /// \brief Pointer to underlying data, for use with external libraries.
    auto data() -> T*;
    /// \brief Pointer to underlying data, for use with external libraries.
    auto data() const -> const T*;

    /// \brief Populate with data in a `std::string`. Continue to read data
    /// until a `std::stringstream` fails to read a new value.
    void operator<<(std::string data);
};

/// \relates Vector
/// \brief Outputs `Vector` data to output stream in a single line with spaces.
template <typename T>
auto operator<<(std::ostream& out, const Vector<T>& rhs) -> std::ostream&;

/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator+(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator-(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const T& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const Vector<T>& lhs, const U& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Inner product of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto dot(const Vector<T>& lhs, const Vector<U>& rhs) -> R;

/// \relates Vector
/// \brief Right-hand division by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(const Vector<T>& lhs, const U& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Elementwise division of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T>
auto operator+(Vector<T> lhs, const Vector<T>& rhs) -> Vector<T>;

/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T>
auto operator+(const Vector<T>& lhs, Vector<T>&& rhs) -> Vector<T>;

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T>
auto operator-(Vector<T> lhs, const Vector<T>& rhs) -> Vector<T>;

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T>
auto operator-(const Vector<T>& lhs, Vector<T>&& rhs) -> Vector<T>;

/// \relates Vector
/// \brief Left-hand multiplication by scalar.
template <typename T>
auto operator*(const T& lhs, Vector<T> rhs) -> Vector<T>;

/// \relates Vector
/// \brief Right-hand multiplication by scalar.
template <typename T>
auto operator*(Vector<T> lhs, const T& rhs) -> Vector<T>;

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T>
auto operator*(Vector<T> lhs, const Vector<T>& rhs) -> Vector<T>;

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T>
auto operator*(const Vector<T>& lhs, Vector<T>&& rhs) -> Vector<T>;

/// \relates Vector
/// \brief Inner product of two Vectors.
template <typename T>
auto dot(const Vector<T>& lhs, const Vector<T>& rhs) -> T;

/// \relates Vector
/// \brief Right-hand division by scalar.
template <typename T>
auto operator/(Vector<T> lhs, const T& rhs) -> Vector<T>;

/// \relates Vector
/// \brief Elementwise division of two Vectors.
template <typename T>
auto operator/(Vector<T> lhs, const Vector<T>& rhs) -> Vector<T>;

// ========================================================================
// CBLAS
// ========================================================================

#ifdef JUMP_HAS_CBLAS
/// \relates Vector
/// \brief Specialisation of the dot product for two real Vectors, using CBLAS.
auto dot(const Vector<Real>& lhs, const Vector<Real>& rhs) -> Real;

/// \relates Vector
/// \brief Specialisation of the dot product for two complex Vectors, using
/// CBLAS.
auto dot(const Vector<Complex>& lhs, const Vector<Complex>& rhs) -> Complex;
#endif  // JUMP_HAS_CBLAS

// ========================================================================
// Exponentiation
// ========================================================================

/// \relates Vector
template <typename T>
auto exp(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto log(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto pow(Vector<T> v, T p) -> Vector<T>;

// ========================================================================
// Trigonometry
// ========================================================================

/// \relates Vector
template <typename T>
auto sin(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto cos(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto tan(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto asin(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto acos(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto atan(Vector<T> v) -> Vector<T>;

// ========================================================================
// Hyperbolics
// ========================================================================

/// \relates Vector
template <typename T>
auto sinh(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto cosh(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto tanh(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto asinh(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto acosh(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto atanh(Vector<T> v) -> Vector<T>;

// ========================================================================
// Miscellaneous
// ========================================================================

/// \relates Vector
template <typename T>
auto abs(Vector<T> v) -> Vector<T>;

/// \relates Vector
template <typename T>
auto sgn(Vector<T> v) -> Vector<T>;

// ========================================================================
// Implementation
// ========================================================================

template <typename T>
inline Vector<T>::Vector(std::size_t size, const T& value) :
    storage(size, value) {
}

template <typename T>
inline Vector<T>::Vector(std::initializer_list<T>&& list) :
    storage(std::forward<std::initializer_list<T>>(list)) {
}

template <typename T>
template <typename U>
inline Vector<T>::Vector(const Vector<U>& other) :
    storage(other.begin(), other.end()) {
}

/// Useful when extracting columns from a matrix, for example. Combined with a
/// way of getting iterators to the internal storage locations representing the
/// top and bottom of a given matrix column, this is the fastest way of
/// obtaining that isolated column.
template <typename T>
template <class InputIt>
inline Vector<T>::Vector(InputIt first, InputIt last) :
    storage(first, last) {
}

template <typename T>
inline void Vector<T>::assign(std::size_t size, const T& value) {
    storage.assign(size, value);
}

template <typename T>
template <class InputIt>
inline void Vector<T>::assign(InputIt first, InputIt last) {
    storage.assign(first, last);
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
inline auto Vector<T>::size() const -> std::size_t {
    return storage.size();
}

template <typename T>
inline auto Vector<T>::operator[](std::size_t index) const -> const T& {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return storage[index];
}

template <typename T>
inline auto Vector<T>::operator[](std::size_t index) -> T& {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return storage[index];
}

template <typename T>
inline auto Vector<T>::begin() const -> ConstIterator {
    return storage.cbegin();
}

template <typename T>
inline auto Vector<T>::end() const -> ConstIterator {
    return storage.cend();
}

template <typename T>
inline auto Vector<T>::begin() -> Iterator {
    return storage.begin();
}

template <typename T>
inline auto Vector<T>::end() -> Iterator {
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
inline auto Vector<T>::operator+() const -> const Vector<T>& {
    return *this;
}

template <typename T>
inline auto Vector<T>::operator-() const -> Vector<T> {
    Vector<T> temp{*this};
    return temp *= -1;
}

template <typename T>
inline auto Vector<T>::operator+=(const Vector<T>& rhs) -> Vector<T>& {
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
inline auto Vector<T>::operator-=(const Vector<T>& rhs) -> Vector<T>& {
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
inline auto Vector<T>::operator*=(const T& rhs) -> Vector<T>& {
    for (auto& x : storage) {
        x *= rhs;
    }
    return *this;
}

template <typename T>
inline auto Vector<T>::operator*=(const Vector<T>& rhs) -> Vector<T>& {
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
inline auto Vector<T>::operator/=(const T& rhs) -> Vector<T>& {
    return *this *= (T{1}/rhs);
}

template <typename T>
inline auto Vector<T>::operator/=(const Vector<T>& rhs) -> Vector<T>& {
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
inline auto Vector<T>::L1_norm() const -> Real {
    if constexpr (is_dual_v<T>) {
        auto F{[](Real acc, const T& x) { return acc + abs(x).value; }};
        return std::accumulate(storage.begin(), storage.end(), Real{0}, F);
    } else {
        auto F{[](Real acc, const T& x) { return acc + std::abs(x); }};
        return std::accumulate(storage.begin(), storage.end(), Real{0}, F);
    }
}

template <typename T>
inline auto Vector<T>::L2_norm() const -> Real {
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
inline auto Vector<T>::Linf_norm() const -> Real {
    if constexpr (is_dual_v<T>) {
        auto F{[](const T& x) { return abs(x).value; }};
        return F(std::ranges::max(storage, {}, F));
    } else {
        auto F{[](const T& x) { return std::abs(x); }};
        return F(std::ranges::max(storage, {}, F));
    }
}

template <typename T>
inline auto Vector<T>::data() -> T* {
    return storage.data();
}

template <typename T>
inline auto Vector<T>::data() const -> const T* {
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
template <typename T>
inline auto operator<<(std::ostream& out, const Vector<T>& rhs)
        -> std::ostream& {
    for (const auto& x : rhs) {
        out << x << ' ';
    }
    return out;
}

/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T, typename U, typename R>
inline auto operator+(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        Vector<R> result{rhs};
        result += lhs;
        return result;
    } else {
        Vector<R> result{lhs};
        result += rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T, typename U, typename R>
inline auto operator-(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        Vector<R> result{rhs};
        result *= R{-1};
        result += lhs;
        return result;
    } else {
        Vector<R> result{lhs};
        result -= rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R>
inline auto operator*(const T& lhs, const Vector<U>& rhs) -> Vector<R> {
    Vector<R> result{rhs};
    result *= lhs;
    return result;
}

/// \relates Vector
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R>
inline auto operator*(const Vector<T>& lhs, const U& rhs) -> Vector<R> {
    Vector<R> result{lhs};
    result *= rhs;
    return result;
}

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T, typename U, typename R>
inline auto operator*(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        Vector<R> result{rhs};
        result *= lhs;
        return result;
    } else {
        Vector<R> result{lhs};
        result *= rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Inner product of two Vectors.
template <typename T, typename U, typename R>
inline auto dot(const Vector<T>& lhs, const Vector<U>& rhs) -> R {
    if constexpr (std::is_same_v<T, R>) {
        return dot(lhs, Vector<R>{rhs});
    } else if constexpr(std::is_same_v<U, R>) {
        return dot(Vector<R>{lhs}, rhs);
    } else {
        return dot(Vector<R>{lhs}, Vector<R>{rhs});
    }
}

/// \relates Vector
/// \brief Right-hand division by scalar.
template <typename T, typename U, typename R>
inline auto operator/(const Vector<T>& lhs, const U& rhs) -> Vector<R> {
    Vector<R> result{lhs};
    result /= rhs;
    return result;
}

/// \relates Vector
/// \brief Elementwise division of two Vectors.
template <typename T, typename U, typename R>
inline auto operator/(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R> {
    Vector<R> result{lhs};
    result /= rhs;
    return result;
}

/// \relates Vector
/// \brief Addition of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
template <typename T>
inline auto operator+(Vector<T> lhs, const Vector<T>& rhs) -> Vector<T> {
    lhs += rhs;
    return lhs;
}

/// \relates Vector
/// \brief Addition of two Vectors.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
template <typename T>
inline auto operator+(const Vector<T>& lhs, Vector<T>&& rhs) -> Vector<T> {
    rhs += lhs;
    return rhs;
}

/// \relates Vector
/// \brief Difference of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
template <typename T>
inline auto operator-(Vector<T> lhs, const Vector<T>& rhs) -> Vector<T> {
    lhs -= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Difference of two Vectors.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
template <typename T>
inline auto operator-(const Vector<T>& lhs, Vector<T>&& rhs) -> Vector<T> {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates Vector
/// \brief Left-hand multiplication by scalar.
template <typename T>
inline auto operator*(const T& lhs, Vector<T> rhs) -> Vector<T> {
    rhs *= lhs;
    return rhs;
}

/// \relates Vector
/// \brief Right-hand multiplication by scalar.
template <typename T>
inline auto operator*(Vector<T> lhs, const T& rhs) -> Vector<T> {
    lhs *= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Elementwise product of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
template <typename T>
inline auto operator*(Vector<T> lhs, const Vector<T>& rhs) -> Vector<T> {
    lhs *= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Elementwise product of two Vectors.
///
/// Handles the case of rhs being given an rvalue, no ambiguity due to rvalue
/// reference parameter (NRVO).
template <typename T>
inline auto operator*(const Vector<T>& lhs, Vector<T>&& rhs) -> Vector<T> {
    rhs *= lhs;
    return rhs;
}

/// \relates Vector
/// \brief Inner product of two Vectors.
template <typename T>
inline auto dot(const Vector<T>& lhs, const Vector<T>& rhs) -> T {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), T{0});
}

/// \relates Vector
/// \brief Right-hand division by scalar.
template <typename T>
inline auto operator/(Vector<T> lhs, const T& rhs) -> Vector<T> {
    lhs /= rhs;
    return lhs;
}

/// \relates Vector
/// \brief Elementwise division of two Vectors.
///
/// If both lhs and rhs are given lvalues, take copy of lhs and elide copy on
/// return. Also handles the case that lhs is given an rvalue (NRVO).
template <typename T>
inline auto operator/(Vector<T> lhs, const Vector<T>& rhs) -> Vector<T> {
    lhs /= rhs;
    return lhs;
}

// ========================================================================
// Exponentiation
// ========================================================================

/// \relates Vector
template <typename T>
inline auto exp(Vector<T> v) -> Vector<T> {
    using std::exp;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return exp(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto log(Vector<T> v) -> Vector<T> {
    using std::log;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return log(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto pow(Vector<T> v, T p) -> Vector<T> {
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
inline auto sin(Vector<T> v) -> Vector<T> {
    using std::sin;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return sin(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto cos(Vector<T> v) -> Vector<T> {
    using std::cos;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return cos(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto tan(Vector<T> v) -> Vector<T> {
    using std::tan;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return tan(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto asin(Vector<T> v) -> Vector<T> {
    using std::asin;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return asin(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto acos(Vector<T> v) -> Vector<T> {
    using std::acos;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return acos(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto atan(Vector<T> v) -> Vector<T> {
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
inline auto sinh(Vector<T> v) -> Vector<T> {
    using std::sinh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return sinh(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto cosh(Vector<T> v) -> Vector<T> {
    using std::cosh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return cosh(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto tanh(Vector<T> v) -> Vector<T> {
    using std::tanh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return tanh(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto asinh(Vector<T> v) -> Vector<T> {
    using std::asinh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return asinh(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto acosh(Vector<T> v) -> Vector<T> {
    using std::acosh;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return acosh(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto atanh(Vector<T> v) -> Vector<T> {
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
inline auto abs(Vector<T> v) -> Vector<T> {
    using std::abs;
    std::transform(v.begin(), v.end(), v.begin(),
            [&](const T& x) { return abs(x); });
    return v;
}

/// \relates Vector
template <typename T>
inline auto sgn(Vector<T> v) -> Vector<T> {
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
inline auto Vector<Real>::operator+=(const Vector<Real>& rhs) -> Vector<Real>& {
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
inline auto Vector<Complex>::operator+=(const Vector<Complex>& rhs)
        -> Vector<Complex>& {
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
inline auto Vector<Real>::operator-=(const Vector<Real>& rhs) -> Vector<Real>& {
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
inline auto Vector<Complex>::operator-=(const Vector<Complex>& rhs)
        -> Vector<Complex>& {
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
inline auto Vector<Real>::operator*=(const Real& rhs) -> Vector<Real>& {
    // Computes rhs*(*this) (with a pointer shift of 1 between elements)
    cblas_dscal(storage.size(), rhs, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place multiplication of a complex `Vector` by a
/// complex scalar, using CBLAS.
template <>
inline auto Vector<Complex>::operator*=(const Complex& rhs)
        -> Vector<Complex>& {
    // Computes rhs*(*this) (with a pointer shift of 1 between elements)
    cblas_zscal(storage.size(), &rhs, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place division of a real `Vector` by a real
/// scalar, using CBLAS.
template <>
inline auto Vector<Real>::operator/=(const Real& rhs) -> Vector<Real>& {
    // Computes (1./rhs)*(*this) (with a pointer shift of 1 between elements)
    cblas_dscal(storage.size(), 1./rhs, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of in-place division of a complex `Vector` by a
/// complex scalar using CBLAS.
template <>
inline auto Vector<Complex>::operator/=(const Complex& rhs)
        -> Vector<Complex>& {
    // Computes (1./rhs)*(*this) (with a pointer shift of 1 between elements)
    Complex a{1./rhs};
    cblas_zscal(storage.size(), &a, storage.data(), 1);
    return *this;
}

/// \brief Specialisation of the L1-norm calculation for a real `Vector`, using
/// CBLAS.
template <>
inline auto Vector<Real>::L1_norm() const -> Real {
    // Computes sum of absolute element values (with a pointer shift of 1
    // between elements)
    return cblas_dasum(storage.size(), storage.data(), 1);
}

/// \brief Specialisation of the L1-norm calculation for a complex `Vector`,
/// using CBLAS.
template <>
inline auto Vector<Complex>::L1_norm() const -> Real {
    // Computes sum of absolute real and imaginary element values (with a
    // pointer shift of 1 between elements). Note: this is not the same as the
    // usual definition of this norm
    return cblas_dzasum(storage.size(), storage.data(), 1);
}

/// \brief Specialisation of the L2-norm calculation for a real `Vector`, using
/// CBLAS.
template <>
inline auto Vector<Real>::L2_norm() const -> Real {
    // Computes the Euclidean norm of the `Vector` (with a pointer shift of 1
    // between elements)
    return cblas_dnrm2(storage.size(), storage.data(), 1);
}

/// \brief Specialisation of the L2-norm calculation for a complex `Vector`,
/// using CBLAS.
template <>
inline auto Vector<Complex>::L2_norm() const -> Real {
    // Computes the Euclidean norm of the `Vector` (with a pointer shift of 1
    // between elements)
    return cblas_dznrm2(storage.size(), storage.data(), 1);
}

/// \brief Specialisation of the dot product for two real Vectors, using CBLAS.
inline auto dot(const Vector<Real>& lhs, const Vector<Real>& rhs) -> Real {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // Computes dot product lhs*rhs (with a pointer shift of 1 between elements)
    return cblas_ddot(lhs.size(), lhs.data(), 1, rhs.data(), 1);
}

/// \brief Specialisation of the dot product for two complex Vectors, using
/// CBLAS.
inline auto dot(const Vector<Complex>& lhs, const Vector<Complex>& rhs)
        -> Complex {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    // Computes dot product lhs*rhs (with a pointer shift of 1 between elements)
    Complex result;
    cblas_zdotu_sub(lhs.size(), lhs.data(), 1, rhs.data(), 1, &result);
    return result;
}
#endif  // JUMP_HAS_CBLAS
}   // namespace jump

#endif  // JUMP_VECTOR_HPP

