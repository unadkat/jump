// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_VECTOR_HPP
#define JUMP_VECTOR_HPP

#include "jump/autodiff/dual.hpp"
#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
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

#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
#include "jump/experimental/expression_templates/vector_operators.hpp"

#include <concepts>
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

namespace jump {
/// \brief Permissive encapulation of `std::vector` with arithmetic
/// operators enabled.
template <typename T>
struct Vector {
    /// \brief Iterator for algorithms.
    using Iterator = typename std::vector<T>::iterator;
    /// \brief Iterator for algorithms.
    using ConstIterator = typename std::vector<T>::const_iterator;
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
    /// \brief To satisfy VectorExpression concept
    using ValueType = T;

    /// \brief To satisfy VectorExpression concept (and signal that this data
    /// structure can be referenced in evaluations).
    static constexpr bool is_vector_expression_leaf{true};
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

    /// \brief Internal contiguous storage.
    std::vector<T> storage;

    /// \brief Construct a `Vector` with a given size (empty by default) filled
    /// with the given item (`T{0}` by default).
    explicit Vector(std::size_t size = 0, const T& value = T{0});
    /// \brief Construct a `Vector` from a brace-enclosed list.
    Vector(std::initializer_list<T> list);
    /// \brief Construct a `Vector` via a pair of iterators.
    template <typename InputIt>
    Vector(InputIt first, InputIt last);
    /// \brief Default copy constructor.
    Vector(const Vector& other) = default;
    /// \brief Templated copy constructor.
    template <typename U>
    Vector(const Vector<U>& other);
    /// \brief Default move constructor.
    Vector(Vector&& other) = default;
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
    template <VectorExpressionConvertibleTo<T> Expr>
    Vector(const Expr& expr);
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

    /// \brief Default copy assignment.
    auto operator=(const Vector& other) -> Vector& = default;
    /// \brief Default move assignment.
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
#ifndef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
    /// \brief Negate `Vector`.
    auto operator-() const -> Vector;
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
    /// \brief Add two Vectors together in place.
    template <std::convertible_to<T> U>
    auto operator+=(const Vector<U>& rhs) -> Vector&;
    /// \brief Subtract one `Vector` from another in place.
    template <std::convertible_to<T> U>
    auto operator-=(const Vector<U>& rhs) -> Vector&;
    /// \brief Multiply by scalar in place.
    template <std::convertible_to<T> U>
    auto operator*=(const U& rhs) -> Vector&;
    /// \brief Elementwise product by another Vector.
    template <std::convertible_to<T> U>
    auto operator*=(const Vector<U>& rhs) -> Vector&;
    /// \brief Divide by scalar in place.
    template <std::convertible_to<T> U>
    auto operator/=(const U& rhs) -> Vector&;
    /// \brief Elementwise division by another Vector.
    template <std::convertible_to<T> U>
    auto operator/=(const Vector<U>& rhs) -> Vector&;

    /// \brief Return sum of element magnitudes.
    auto L1_norm() const -> Real;
    /// \brief Return Euclidean norm.
    auto L2_norm() const -> Real;
    /// \brief Return maximum magnitude over all elements.
    auto Linf_norm() const -> Real;

    /// \brief Pointer to underlying data, for use with third-party libraries.
    auto data() -> T*;
    /// \brief Pointer to underlying data, for use with third-party libraries.
    auto data() const -> const T*;

    /// \brief Populate with data in a `std::string`. Continue to read data
    /// until a `std::stringstream` fails to read a new value.
    void operator<<(std::string data);
    /// \brief Vector serialisation to a string.
    auto as_string() const -> std::string;
};

/// \relates Vector
/// \brief Outputs `Vector` data to output stream in a single line with spaces.
template <typename T>
auto operator<<(std::ostream& out, const Vector<T>& rhs) -> std::ostream&;

#ifndef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator+(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator+(Vector<T>&& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator+(const Vector<T>& lhs, Vector<U>&& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator+(Vector<T>&& lhs, Vector<U>&& rhs) -> Vector<R>;
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator-(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator-(Vector<T>&& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator-(const Vector<T>& lhs, Vector<U>&& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator-(Vector<T>&& lhs, Vector<U>&& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const T& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const T& lhs, Vector<U>&& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const Vector<T>& lhs, const U& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(Vector<T>&& lhs, const U& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(Vector<T>&& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(const Vector<T>& lhs, Vector<U>&& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator*(Vector<T>&& lhs, Vector<U>&& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Inner product of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto dot(const Vector<T>& lhs, const Vector<U>& rhs) -> R;

/// \relates Vector
/// \brief Right-hand division by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(const Vector<T>& lhs, const U& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Right-hand division by scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(Vector<T>&& lhs, const U& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Quotient with scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(const T& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Quotient with scalar.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(const T& lhs, Vector<U>&& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Elementwise division of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Elementwise division of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(Vector<T>&& lhs, const Vector<U>& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Elementwise division of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(const Vector<T>& lhs, Vector<U>&& rhs) -> Vector<R>;

/// \relates Vector
/// \brief Elementwise division of two Vectors.
template <typename T, typename U, typename R = std::common_type_t<T, U>>
auto operator/(Vector<T>&& lhs, Vector<U>&& rhs) -> Vector<R>;

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
inline Vector<T>::Vector(std::initializer_list<T> list) :
    storage{list} {
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

#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
template <typename T>
template <VectorExpressionConvertibleTo<T> Expr>
inline Vector<T>::Vector(const Expr& expr) : storage(expr.size()) {
    for (std::size_t i{0}, N{expr.size()}; i < N; ++i) {
        storage[i] = expr[i];
    }
}
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

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

#ifndef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
template <typename T>
inline auto Vector<T>::operator-() const -> Vector<T> {
    Vector<T> temp{*this};
    return temp *= T{-1};
}
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

template <typename T>
template <std::convertible_to<T> U>
inline auto Vector<T>::operator+=(const Vector<U>& rhs) -> Vector<T>& {
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
template <std::convertible_to<T> U>
inline auto Vector<T>::operator-=(const Vector<U>& rhs) -> Vector<T>& {
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
template <std::convertible_to<T> U>
inline auto Vector<T>::operator*=(const U& rhs) -> Vector<T>& {
    for (auto& x : storage) {
        x *= rhs;
    }
    return *this;
}

template <typename T>
template <std::convertible_to<T> U>
inline auto Vector<T>::operator*=(const Vector<U>& rhs) -> Vector<T>& {
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
template <std::convertible_to<T> U>
inline auto Vector<T>::operator/=(const U& rhs) -> Vector<T>& {
    return *this *= (T{1}/rhs);
}

template <typename T>
template <std::convertible_to<T> U>
inline auto Vector<T>::operator/=(const Vector<U>& rhs) -> Vector<T>& {
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

template <typename T>
inline auto Vector<T>::as_string() const -> std::string {
    std::ostringstream oss;
    oss << *this;
    return oss.str();
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

#ifndef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T, typename U, typename R>
inline auto operator+(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        Vector<R> result{lhs};
        result += rhs;
        return result;
    } else {
        Vector<R> result{rhs};
        result += lhs;
        return result;
    }
}

/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T, typename U, typename R>
inline auto operator+(Vector<T>&& lhs, const Vector<U>& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs += rhs;
        return lhs;
    } else {
        Vector<R> result{rhs};
        result += lhs;
        return result;
    }
}

/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T, typename U, typename R>
inline auto operator+(const Vector<T>& lhs, Vector<U>&& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<U, R>) {
        rhs += lhs;
        return rhs;
    } else {
        Vector<R> result{lhs};
        result += rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Addition of two Vectors.
template <typename T, typename U, typename R>
inline auto operator+(Vector<T>&& lhs, Vector<U>&& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs += rhs;
        return lhs;
    } else if constexpr (std::is_same_v<U, R>) {
        rhs += lhs;
        return rhs;
    } else {
        Vector<R> result{lhs};
        result += rhs;
        return result;
    }
}
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T, typename U, typename R>
inline auto operator-(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R> {
    Vector<R> result{lhs};
    result -= rhs;
    return result;
}

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T, typename U, typename R>
inline auto operator-(Vector<T>&& lhs, const Vector<U>& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs -= rhs;
        return lhs;
    } else {
        Vector<R> result{lhs};
        result -= rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T, typename U, typename R>
inline auto operator-(const Vector<T>& lhs, Vector<U>&& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<U, R>) {
#ifndef NDEBUG
        if (lhs.size() != rhs.size()) {
            throw RuntimeError{Mismatch1DError{.name1 = "lhs",
                .size1 = lhs.size(), .name2 = "rhs", .size2 = rhs.size()}};
        }
#endif  // NDEBUG

        for (std::size_t i{0}, N{lhs.size()}; i < N; ++i) {
            rhs.storage[i] = lhs.storage[i] - rhs.storage[i];
        }
        return rhs;
    } else {
        Vector<R> result{lhs};
        result -= rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Difference of two Vectors.
template <typename T, typename U, typename R>
inline auto operator-(Vector<T>&& lhs, Vector<U>&& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs -= rhs;
        return lhs;
    } else if constexpr (std::is_same_v<U, R>) {
#ifndef NDEBUG
        if (lhs.size() != rhs.size()) {
            throw RuntimeError{Mismatch1DError{.name1 = "lhs",
                .size1 = lhs.size(), .name2 = "rhs", .size2 = rhs.size()}};
        }
#endif  // NDEBUG

        for (std::size_t i{0}, N{lhs.size()}; i < N; ++i) {
            rhs.storage[i] = lhs.storage[i] - rhs.storage[i];
        }
        return rhs;
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
/// \brief Left-hand multiplication by scalar.
template <typename T, typename U, typename R>
inline auto operator*(const T& lhs, Vector<U>&& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<U, R>) {
        rhs *= lhs;
        return rhs;
    } else {
        Vector<T> result{rhs};
        result *= lhs;
        return result;
    }
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
/// \brief Right-hand multiplication by scalar.
template <typename T, typename U, typename R>
inline auto operator*(Vector<T>&& lhs, const U& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs *= rhs;
        return lhs;
    } else {
        Vector<R> result{lhs};
        result *= rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T, typename U, typename R>
inline auto operator*(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        Vector<R> result{lhs};
        result *= rhs;
        return result;
    } else {
        Vector<R> result{rhs};
        result *= lhs;
        return result;
    }
}

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T, typename U, typename R>
inline auto operator*(Vector<T>&& lhs, const Vector<U>& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs *= rhs;
        return lhs;
    } else {
        Vector<R> result{rhs};
        result *= lhs;
        return result;
    }
}

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T, typename U, typename R>
inline auto operator*(const Vector<T>& lhs, Vector<U>&& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<U, R>) {
        rhs *= lhs;
        return rhs;
    } else {
        Vector<R> result{lhs};
        result *= rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Elementwise product of two Vectors.
template <typename T, typename U, typename R>
inline auto operator*(Vector<T>&& lhs, Vector<U>&& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs *= rhs;
        return lhs;
    } else if constexpr (std::is_same_v<U, R>) {
        rhs *= lhs;
        return rhs;
    } else {
        Vector<R> result{lhs};
        lhs *= rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Inner product of two Vectors.
template <typename T, typename U, typename R>
inline auto dot(const Vector<T>& lhs, const Vector<U>& rhs) -> R {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), R{0});
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
/// \brief Right-hand division by scalar.
template <typename T, typename U, typename R>
auto operator/(Vector<T>&& lhs, const U& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs /= rhs;
        return lhs;
    } else {
        Vector<R> result{lhs};
        result /= rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Quotient with scalar.
template <typename T, typename U, typename R>
auto operator/(const T& lhs, const Vector<U>& rhs) -> Vector<R> {
    std::size_t N{rhs.size()};
    Vector<R> result(N, R{lhs});
    for (std::size_t i{0}; i < N; ++i) {
        result[i] /= rhs[i];
    }
    return result;
}

/// \relates Vector
/// \brief Quotient with scalar.
template <typename T, typename U, typename R>
auto operator/(const T& lhs, Vector<U>&& rhs) -> Vector<R> {
    std::size_t N{rhs.size()};
    if constexpr (std::is_same_v<U, R>) {
        for (std::size_t i{0}; i < N; ++i) {
            rhs[i] = lhs/rhs[i];
        }
        return rhs;
    } else {
        Vector<R> result(N, R{lhs});
        for (std::size_t i{0}; i < N; ++i) {
            result[i] /= rhs[i];
        }
        return result;
    }
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
/// \brief Elementwise division of two Vectors.
template <typename T, typename U, typename R>
inline auto operator/(Vector<T>&& lhs, const Vector<U>& rhs) -> Vector<R> {
    if constexpr (std::is_same_v<T, R>) {
        lhs /= rhs;
        return lhs;
    } else {
        Vector<R> result{lhs};
        result /= rhs;
        return result;
    }
}

/// \relates Vector
/// \brief Elementwise division of two Vectors.
template <typename T, typename U, typename R>
inline auto operator/(const Vector<T>& lhs, Vector<U>&& rhs) -> Vector<R> {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    std::size_t N{lhs.size()};
    if constexpr (std::is_same_v<U, R>) {
        for (std::size_t i{0}; i < N; ++i) {
            rhs[i] = lhs[i]/rhs[i];
        }
        return rhs;
    } else {
        Vector<R> result{lhs};
        for (std::size_t i{0}; i < N; ++i) {
            result[i] /= rhs[i];
        }
        return result;
    }
}

/// \relates Vector
/// \brief Elementwise division of two Vectors.
template <typename T, typename U, typename R>
inline auto operator/(Vector<T>&& lhs, Vector<U>&& rhs) -> Vector<R> {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    std::size_t N{lhs.size()};
    if constexpr (std::is_same_v<T, R>) {
        for (std::size_t i{0}; i < N; ++i) {
            lhs[i] /= rhs[i];
        }
        return lhs;
    } else if constexpr (std::is_same_v<U, R>) {
        for (std::size_t i{0}; i < N; ++i) {
            rhs[i] = lhs[i]/rhs[i];
        }
        return rhs;
    } else {
        Vector<R> result{lhs};
        for (std::size_t i{0}; i < N; ++i) {
            result[i] /= rhs[i];
        }
        return result;
    }
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
}   // namespace jump

#endif  // JUMP_VECTOR_HPP

