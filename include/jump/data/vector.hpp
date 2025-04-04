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
#include "jump/experimental/expression_templates/vector_operators.hpp"
#include "jump/utility/types.hpp"
#include "jump/utility/utility.hpp"

#include <algorithm>
#include <cmath>
#include <concepts>
#include <initializer_list>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace jump {
/// \brief Permissive encapulation of `std::vector` with arithmetic
/// operators enabled.
template <typename T>
struct Vector {
    /// \brief Iterator for algorithms.
    using Iterator = typename std::vector<T>::iterator;
    /// \brief Iterator for algorithms.
    using ConstIterator = typename std::vector<T>::const_iterator;
    /// \brief To satisfy VectorExpression concept
    using ValueType = T;

    /// \brief To satisfy VectorExpression concept (and signal that this data
    /// structure can be referenced in evaluations).
    static constexpr bool is_vector_expression_leaf{true};

    /// \brief Internal contiguous storage.
    std::vector<T> storage;

    /// \brief Construct a `Vector` with a given size (empty by default).
    explicit Vector(std::size_t size = 0);
    /// \brief Construct a `Vector` with a given size filled with copies of the
    /// given item.
    Vector(std::size_t size, const T& value);
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
    /// \brief Construct from a VectorExpression.
    template <VectorExpressionConvertibleTo<T> Expr>
    Vector(const Expr& expr);

    /// \brief Default copy assignment.
    auto operator=(const Vector& other) -> Vector& = default;
    /// \brief Default move assignment.
    auto operator=(Vector&& other) -> Vector& = default;
    /// \brief Assignment from a VectorExpression.
    template <VectorExpressionConvertibleTo<T> Expr>
    auto operator=(const Expr& expr) -> Vector&;

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

    /// \brief Add VectorExpression to Vector in place.
    template <VectorExpressionConvertibleTo<T> Expr>
    auto operator+=(const Expr& expr) -> Vector&;
    /// \brief Subtract a VectorExpression from a Vector in place.
    template <VectorExpressionConvertibleTo<T> Expr>
    auto operator-=(const Expr& expr) -> Vector&;
    /// \brief Elementwise product by VectorExpression.
    template <VectorExpressionConvertibleTo<T> Expr>
    auto operator*=(const Expr& expr) -> Vector&;
    /// \brief Elementwise division by a VectorExpression.
    template <VectorExpressionConvertibleTo<T> Expr>
    auto operator/=(const Expr& expr) -> Vector&;
    /// \brief Multiply by scalar in place.
    template <std::convertible_to<T> U>
    auto operator*=(const U& rhs) -> Vector&;
    /// \brief Divide by scalar in place.
    template <std::convertible_to<T> U>
    auto operator/=(const U& rhs) -> Vector&;

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

/// \relates Vector
template <VectorExpression Expr>
constexpr auto evaluate(const Expr& expr) -> Vector<typename Expr::ValueType>;

// ========================================================================
// Implementation
// ========================================================================

template <typename T>
inline Vector<T>::Vector(std::size_t size) :
    storage(size) {
}

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

template <typename T>
template <VectorExpressionConvertibleTo<T> Expr>
inline Vector<T>::Vector(const Expr& expr) :
    storage(expr.size()) {
    for (std::size_t i{0}, N{expr.size()}; i < N; ++i) {
        storage[i] = expr[i];
    }
}

template <typename T>
template <VectorExpressionConvertibleTo<T> Expr>
inline auto Vector<T>::operator=(const Expr& expr) -> Vector& {
    std::size_t N{expr.size()};
    storage.resize(N);
    for (std::size_t i{0}; i < N; ++i) {
        storage[i] = expr[i];
    }
    return *this;
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

/// \brief Add VectorExpression to Vector in place.
template <typename T>
template <VectorExpressionConvertibleTo<T> Expr>
inline auto Vector<T>::operator+=(const Expr& expr) -> Vector& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}, N{size()}; i < N; ++i) {
        storage[i] += expr[i];
    }
    return *this;
}

/// \brief Subtract a VectorExpression from a Vector in place.
template <typename T>
template <VectorExpressionConvertibleTo<T> Expr>
inline auto Vector<T>::operator-=(const Expr& expr) -> Vector& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}, N{size()}; i < N; ++i) {
        storage[i] -= expr[i];
    }
    return *this;
}

/// \brief Elementwise product by VectorExpression.
template <typename T>
template <VectorExpressionConvertibleTo<T> Expr>
inline auto Vector<T>::operator*=(const Expr& expr) -> Vector& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}, N{size()}; i < N; ++i) {
        storage[i] *= expr[i];
    }
    return *this;
}

/// \brief Elementwise division by a VectorExpression.
template <typename T>
template <VectorExpressionConvertibleTo<T> Expr>
inline auto Vector<T>::operator/=(const Expr& expr) -> Vector& {
#ifndef NDEBUG
    if (size() != expr.size()) {
        throw RuntimeError{Mismatch1DError{.size1 = size(), .name2 = "expr",
            .size2 = expr.size()}};
    }
#endif  // NDEBUG

    for (std::size_t i{0}, N{size()}; i < N; ++i) {
        storage[i] /= expr[i];
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
inline auto Vector<T>::operator/=(const U& rhs) -> Vector<T>& {
    return *this *= (T{1}/rhs);
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

/// \relates Vector
template <VectorExpression Expr>
inline constexpr auto evaluate(const Expr& expr) -> Vector<typename Expr::ValueType> {
    return {expr};
}
}   // namespace jump

#endif  // JUMP_VECTOR_HPP

