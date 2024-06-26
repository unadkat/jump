// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_VECTOR_DECL_HPP
#define JUMP_VECTOR_DECL_HPP

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "jump/autodiff/dual.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/external.hpp"
#include "jump/utility/types.hpp"
#include "jump/utility/utility.hpp"

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
    std::size_t size() const;

    /// \brief Defaulted spaceship operator.
    auto operator<=>(const Vector&) const = default;

    /// \brief Const element access.
    const T& operator[](std::size_t index) const;
    /// \brief Mutable element access.
    T& operator[](std::size_t index);

    /// \brief Const begin iterator for algorithms.
    ConstIterator begin() const;
    /// \brief Const past-the-end iterator for algorithms.
    ConstIterator end() const;
    /// \brief Begin iterator for algorithms.
    Iterator begin();
    /// \brief Past-end-end iterator for algorithms.
    Iterator end();

    /// \brief Fill with given value.
    void fill(const T& value);
    /// \brief Fill vector with zeroes.
    void zero();

    /// \brief No operation on `Vector`.
    const Vector& operator+() const;
    /// \brief Negate `Vector`.
    Vector operator-() const;
    /// \brief Add two Vectors together in place.
    Vector& operator+=(const Vector& rhs);
    /// \brief Subtract one `Vector` from another in place.
    Vector& operator-=(const Vector& rhs);
    /// \brief Multiply by scalar in place.
    Vector& operator*=(const T& rhs);
    /// \brief Elementwise product by another Vector.
    Vector& operator*=(const Vector& rhs);
    /// \brief Divide by scalar in place.
    Vector& operator/=(const T& rhs);
    /// \brief Elementwise division by another Vector.
    Vector& operator/=(const Vector& rhs);

    /// \brief Return sum of element magnitudes.
    Real L1_norm() const;
    /// \brief Return Euclidean norm.
    Real L2_norm() const;
    /// \brief Return maximum magnitude over all elements.
    Real Linf_norm() const;

    /// \brief Pointer to underlying data, for use with external libraries.
    T* data();
    /// \brief Pointer to underlying data, for use with external libraries.
    const T* data() const;

    /// \brief Populate with data in a `std::string`. Continue to read data
    /// until a `std::stringstream` fails to read a new value.
    void operator<<(std::string data);

    #include "jump/data/vector_friends.hpp"
};

/// \relates Vector
/// \brief Outputs `Vector` data to output stream in a single line with spaces.
template <typename T, typename Os>
Os& operator<<(Os& out, const Vector<T>& rhs);

// ========================================================================
// Exponentiation
// ========================================================================

/// \relates Vector
template <typename T>
Vector<T> exp(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> log(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> pow(Vector<T> v, T p);

// ========================================================================
// Trigonometry
// ========================================================================

/// \relates Vector
template <typename T>
Vector<T> sin(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> cos(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> tan(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> asin(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> acos(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> atan(Vector<T> v);

// ========================================================================
// Hyperbolics
// ========================================================================

/// \relates Vector
template <typename T>
Vector<T> sinh(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> cosh(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> tanh(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> asinh(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> acosh(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> atanh(Vector<T> v);

// ========================================================================
// Miscellaneous
// ========================================================================

/// \relates Vector
template <typename T>
Vector<T> abs(Vector<T> v);

/// \relates Vector
template <typename T>
Vector<T> sgn(Vector<T> v);
}   // namespace jump

#endif  // JUMP_VECTOR_DECL_HPP

