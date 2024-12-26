// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_VECTOR_DECL_HPP
#define JUMP_VECTOR_DECL_HPP

#include "jump/debug/exception.hpp"
#include "jump/utility/external.hpp"
#include "jump/utility/types.hpp"

#include <initializer_list>
#include <numeric>
#include <string>
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
    /// \brief Construct a `Vector` via a pair of iterators.
    template <typename InputIt>
    Vector(InputIt first, InputIt last);

    /// \brief Conversion operator to promote a real-valued `Vector` to a
    /// complex-valued one.
    operator Vector<Complex>() const;

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

    #include "jump/data/vector_friends.hpp"
};

/// \relates Vector
/// \brief Outputs `Vector` data to output stream in a single line with spaces.
template <typename T, typename Os>
auto operator<<(Os& out, const Vector<T>& rhs) -> Os&;

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
}   // namespace jump

#endif  // JUMP_VECTOR_DECL_HPP

