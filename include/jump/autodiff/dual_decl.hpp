// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_DUAL_DECL_HPP
#define JUMP_DUAL_DECL_HPP

#include "jump/utility/types.hpp"

#include <array>

namespace jump {
/// \brief An augmented number type that can be used to automatically
/// differentiate numerical expressions in which it appears. Primarily used to
/// effect an automatic linearisation of equation systems for an iterated
/// solution process.
template <std::size_t N, typename T = Real>
struct Dual {
    /// \brief The value of the number (Real or Complex) to which dual
    /// component(s) is/are added.
    T value{0};
    /// \brief The array of `N` distinct dual components of matching type to the
    /// leading-order part (Real or Complex).
    std::array<T, N> dual{};

    /// \brief Construct a `Dual` with a given leading-order part and vanishing
    /// dual components.
    Dual(const T& x = T{0});
    /// \brief Construct a `Dual` with given leading-order and dual parts.
    Dual(const T& x, const std::array<T, N>& d);
    /// \brief Construct a `Dual` with a given leading-order part and a single
    /// non-zero dual part.
    Dual(const T& x, std::size_t index, const T& d = T{1});

    /// \brief Defaulted spaceship operator.
    auto operator<=>(const Dual&) const = default;

    /// \brief Conversion operator to promote a real-valued `Dual` to a
    /// complex-valued one.
    operator Dual<N, Complex>() const;

    /// \brief No operation on `Dual`.
    auto operator+() const -> Dual;
    /// \brief Negate `Dual`.
    auto operator-() const -> Dual;
    /// \brief Add one `Dual` to another in place.
    auto operator+=(const Dual& rhs) -> Dual&;
    /// \brief Subtract one `Dual` from another in place.
    auto operator-=(const Dual& rhs) -> Dual&;
    /// \brief Multiply one `Dual` by another in place.
    auto operator*=(const Dual& rhs) -> Dual&;
    /// \brief Divide one `Dual` by another in place.
    auto operator/=(const Dual& rhs) -> Dual&;

    #include "jump/autodiff/dual_friends.hpp"
};

/// \relates Jump::Dual
template <std::size_t N, typename T, typename Os>
inline auto operator<<(Os& out, const Dual<N, T>& rhs) -> Os&;

// ========================================================================
// Exponentiation
// ========================================================================

/// \relates Dual
template <std::size_t N, typename T>
auto exp(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto log(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto pow(Dual<N, T> x, Dual<N, T> p) -> Dual<N, T>;

// ========================================================================
// Trigonometry
// ========================================================================

/// \relates Dual
template <std::size_t N, typename T>
auto sin(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto cos(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto tan(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto asin(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto acos(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto atan(Dual<N, T> x) -> Dual<N, T>;

// ========================================================================
// Hyperbolics
// ========================================================================

/// \relates Dual
template <std::size_t N, typename T>
auto sinh(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto cosh(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto tanh(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto asinh(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto acosh(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto atanh(Dual<N, T> x) -> Dual<N, T>;

// ========================================================================
// Miscellaneous
// ========================================================================

/// \relates Dual
template <std::size_t N, typename T>
auto abs(Dual<N, T> x) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto sgn(Dual<N, T> x) -> Dual<N, T>;

// ========================================================================
// Type traits and aliases
// ========================================================================

template <typename>
struct is_dual : public std::false_type {
};

template <std::size_t N, typename T>
struct is_dual<Dual<N, T>> : public std::true_type {
};

template <typename T>
inline constexpr bool is_dual_v = is_dual<T>::value;

using d1f64 = Dual<1, Real>;
using d2f64 = Dual<2, Real>;
using d3f64 = Dual<3, Real>;
using d1z64 = Dual<1, Complex>;
using d2z64 = Dual<2, Complex>;
using d3z64 = Dual<3, Complex>;
}   // namespace jump

#endif  // JUMP_DUAL_DECL_HPP

