// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_DUAL_DECL_HPP
#define JUMP_DUAL_DECL_HPP

#include <array>
#include <cmath>
#include <type_traits>

#include "jump/debug/exception.hpp"
#include "jump/utility/types.hpp"

namespace jump::experimental {
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

    /// \brief Conversion operator to promote a real-valued `Dual` to a
    /// complex-valued one.
    operator Dual<N, Complex>() const;

    /// \brief No operation on `Dual`.
    Dual operator+() const;
    /// \brief Negate `Dual`.
    Dual operator-() const;
    /// \brief Add one `Dual` to another in place.
    Dual& operator+=(const Dual& rhs);
    /// \brief Subtract one `Dual` from another in place.
    Dual& operator-=(const Dual& rhs);
    /// \brief Multiply one `Dual` by another in place.
    Dual& operator*=(const Dual& rhs);
    /// \brief Divide one `Dual` by another in place.
    Dual& operator/=(const Dual& rhs);

    #include "jump/experimental/autodiff/dual_friends.hpp"
};

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
}   // namespace jump::experimental

#endif  // JUMP_DUAL_DECL_HPP

