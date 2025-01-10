// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_DUAL_HPP
#define JUMP_DUAL_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/utility/types.hpp"
#include "jump/utility/utility.hpp"

#include <array>
#include <cmath>
#include <ostream>

namespace jump {
/// \brief An augmented number type that can be used to automatically
/// differentiate numerical expressions in which it appears. Primarily used to
/// effect an automatic linearisation of equation systems for an iterated
/// solution process.
template <std::size_t N, typename T>
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
    /// \brief Default copy constructor
    Dual(const Dual& other) = default;
    /// \brief Templated copy constructor
    template <typename U>
    Dual(const Dual<N, U>& other);
    /// \brief Default move constructor
    Dual(Dual&& other) = default;

    /// \brief Default copy assignment
    auto operator=(const Dual& other) -> Dual& = default;
    /// \brief Default move assignment
    auto operator=(Dual&& other) -> Dual& = default;

    /// \brief Defaulted spaceship operator.
    auto operator<=>(const Dual&) const = default;

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
};

/// \relates Jump::Dual
template <std::size_t N, typename T>
auto operator<<(std::ostream& out, const Dual<N, T>& rhs) -> std::ostream&;

// ========================================================================
// Arithmetic
// ========================================================================

/// \relates Dual
template <std::size_t N, typename T>
auto operator+(Dual<N, T> lhs, const Dual<N, T>& rhs) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto operator+(const Dual<N, T>& lhs, Dual<N, T>&& rhs) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto operator-(Dual<N, T> lhs, const Dual<N, T>& rhs) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto operator-(const Dual<N, T>& lhs, Dual<N, T>&& rhs) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto operator*(Dual<N, T> lhs, const Dual<N, T>& rhs) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto operator*(const Dual<N, T>& lhs, Dual<N, T>&& rhs) -> Dual<N, T>;

/// \relates Dual
template <std::size_t N, typename T>
auto operator/(Dual<N, T> lhs, const Dual<N, T>& rhs) -> Dual<N, T>;

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

// ========================================================================
// Implementation
// ========================================================================

/// \class Dual
/// Denote a dual number (with a single dual component) as
/// \f$\h{x}=x+x'\epsilon\f$, where \f$x\f$ is the leading-order part and
/// \f$x'\f$ is the dual component with \f$\epsilon\f$ an infinitesimal. The
/// dual number follows all usual rules of real or complex arithmetic, as
/// appropriate, with the condition that \f$\epsilon^2=0\f$, i.e. the dual unit
/// is nilpotent. The result of this arithmetic on dual numbers yields, for
/// example
/// \f{align*}
///  \h{x}+\h{y}&=\left(x+x'\epsilon\right)+\left(y+y'\epsilon\right)
///  \\ &= (x+y)+\left(x'+y'\right)\epsilon,
/// \f}
/// and
/// \f{align*}
///  \h{x}\h{y}&=\left(x+x'\epsilon\right)\left(y+y'\epsilon\right)
///  \\ &= xy+\left(x'y+y'x\right)\epsilon.
/// \f}
/// For a polynomial \f$P(\xi)=\sum_{k=0}^na_k\xi^k\f$, we have
/// \f{align*}
///  P\left(\h{x}\right)&=\sum_{k=0}^{n}a_k\left(x+x'\epsilon\right)^k
///  \\&=a_0+\sum_{k=1}^{n}a_k\left(x^k+kx^{k-1}x'\epsilon\right)
///  \\&=\sum_{k=0}^na_kx^k+x'\left(\sum_{k=1}^na_kkx^{k-1}\right)\epsilon
///  \\&=P(x)+x'P'(x)\epsilon.
/// \f}
///
/// From these examples we can see the essential idea that the dual components
/// of the dual numbers transform as would first derivatives of the
/// leading-order expressions, satisfying product and chain rules under
/// arithmetic and application of (smooth) functions. By insisting that the dual
/// component always transforms according to the arithmetic of first
/// derivatives, we obtain a system by which derivatives of expressions are
/// automatically evaluated along with the expression itself. In order for this
/// system to work, we must provide rules for the application of elementary
/// functions to dual numbers, for instance
/// \f{equation*}
///  \sin(\h{x})=\sin\left(x+x'\epsilon\right)=\sin(x) + x'\cos(x)\epsilon.
/// \f}
/// Once this is done, in combination with an implementation of all arithmetic
/// manipulations on `Duals`, dual components are calculated using the chain
/// rule, product rule, and basic arithmetic rules for derivatives. The
/// calculated values are then accurate to machine precision and not subject to
/// truncation error as would be the case for finite differences, for example.
/// For this system to work we must seed the derivatives by setting a unity dual
/// component on the variable with respect to which we wish to differentiate an
/// expression.
///
/// As a further example, remembering that \f$\epsilon\f$ is an infinitesimal we
/// may consider division
/// \f{align*}
///  \frac{\h{x}}{\h{y}}&=\frac{x+x'\epsilon}{y+y'\epsilon}
///  \\&=\frac{x+x'\epsilon}{y\left(1+\frac{y'}{y}\epsilon\right)}
///  \\&=\frac{x+x'\epsilon}{y}\left(1-\frac{y'}{y}\epsilon\right)
///  \\&=\frac{x}{y}+\frac{x'y-xy'}{y^2}\epsilon.
/// \f}
/// Considering the original expression \f$\frac{x}{y}\f$, we see that setting
/// \f$(x',y')=(1,0)\f$ gives the dual component value
/// \f$\frac{1}{y}=\pdiff{}{}{x}\left(\frac{x}{y}\right)\f$, and setting
/// \f$(x',y')=(0,1)\f$ yields
/// \f$\frac{-x}{y^2}=\pdiff{}{}{y}\left(\frac{x}{y}\right)\f$.
/// The seed values represent the derivatives of the most basic appearing
/// variables with respect to the independent variable being considered. In
/// these basic examples, our basic variables are \f$x\f$ and \f$y\f$, so the
/// seed variables will only ever be unity or zero. As we shall see in our
/// application of this method to solving systems of differential equations, our
/// basic variables can indeed have a further dependence on other structures
/// whose influence is fed through in the seed values used (see the
/// documentation of the `Equation1D` and `VariableSnapshot1D` classes for some
/// further detail).
///
/// The above formulation of dual numbers illustrates the concept clearly, but
/// has a drawback in that our seed values must propagate through the evaluation
/// of a function. This means that only one derivative value can be calculated
/// at a time, which makes this process increasingly expensive if we have
/// multiple variables with respect to which we want to take derivatives. A
/// modified approach to improve computational efficiency is to allow multiple
/// independent infinitesimals where quadratic-or-higher products involving any
/// combination of them vanish. Suppose we have three basic variables appearing
/// in our equations, \f$x\f$, \f$y\f$, and \f$u\f$. We define three dual
/// numbers
/// \f{equation*}
///  \h{x}=x+x'\epsilon_0,\qquad\h{y}=y+y'\epsilon_1,\qquad
///   \h{z}=z+z'\epsilon_2.
/// \f}
/// Then we have, for example,
/// \f{align*}
///  \h{x}\h{y}&=\left(x+x'\epsilon_0\right)\left(y+y'\epsilon_1\right)
///  \\&=xy+x'y\epsilon_0+xy'\epsilon_1,
/// \f}
/// and
/// \f{align*}
///  \frac{\h{x}}{\h{z}}&=\frac{x+x'\epsilon_0}{z+z'\epsilon_2}
///  \\&=\frac{x+x'\epsilon_0}{z\left(1+\frac{z'}{z}\epsilon_2\right)}
///  \\&=\frac{x+x'\epsilon_0}{z}\left(1-\frac{z'}{z}\epsilon_2\right)
///  \\&=\frac{x}{z}+\frac{x'}{z}\epsilon_0-\frac{xz'}{z^2}\epsilon_2.
/// \f}
/// In this way we may obtain all derivatives of a function with respect to a
/// set of variables by seeding all the dual components simulataneously.
///
/// This method is used in `Equation1D` and `VariableSnapshot1D` to provide a
/// mechanism by which nonlinear equations are linearised for an iterative
/// solution process. As a brief motivation, suppose we wish to find a root of
/// some nonlinear function \f$f(\xi)=0\f$. We might do this by considering a
/// Taylor series expansion of \f$f\f$ about some guess at the root value
/// \f$\xi\approx\xi_0\f$, writing \f$\xi-\xi_0=\h{h}_0\f$
/// \f{equation*}
///  0=f(\xi)=f(\xi_0+\h{h}_0)=f(\xi_0)+\h{h}_0f'(\xi_0)+\BigO{\h{h}_0^2},
/// \f}
/// and rearranging this as
/// \f{equation*}
///  h_0f'(\xi_0)=-f(\xi_0)
/// \f}
/// to find \f$h_0\f$, the first-order approximation to the correction value
/// \f$\h{h}_0\f$. Since the function \f$f\f$ is known as well as the current
/// guess for the root \f$\xi_0\f$, all we have to do is solve the above for
/// \f$h_0\f$ to obtain a correction to the guess, whereupon we set
/// \f$\xi_1=\xi_0+h_0\f$ and repeat the process. Under certain conditions,
/// which we shall not discuss here, the sequence \f$\left(\xi_n\right)\f$
/// generated by this process should converge on the root of the function
/// "nearest" the initial guess \f$\xi_0\f$. In this case we should only need a
/// small number of iterations to converge to a guess for the root that
/// satisfies \f$|f(\xi_n)|<\delta\f$, where \f$\delta\f$ is some small positive
/// tolerance. There is a direct correspondence between the method described
/// here for finding a root of a nonlinear algebraic equation, using Newton
/// iteration, and that used elsewhere in this library to iterate towards the
/// solution of nonlinear differential equation systems.

/// \var Dual::value
/// Sometimes referred to as the leading-order component due to its application
/// in this library (see detailed description of `Dual`).

/// \var Dual::dual
/// The unit quantities in each component are denoted by \f$\epsilon_i\f$ for
/// \f$i\in\{0,1,\ldots,N-1\}\f$. Sometimes referred to as the linear components
/// due to their application in this library (see detailed description of
/// `Dual`).

template <std::size_t N, typename T>
inline Dual<N, T>::Dual(const T& x) :
    value{x} {
}

template <std::size_t N, typename T>
inline Dual<N, T>::Dual(const T& x, const std::array<T, N>& d) :
    value{x}, dual{d} {
}

template <std::size_t N, typename T>
inline Dual<N, T>::Dual(const T& x, std::size_t index, const T& d) :
    value{x} {
#ifndef NDEBUG
    if (index >= N) {
        throw RuntimeError{InvalidArgumentError{.argument = "index",
            .value = std::format("{}", index),
            .expected = std::format("index < N (= {})", N)}};
    }
#endif  // NDEBUG

    dual[index] = d;
}

template <std::size_t N, typename T>
template <typename U>
inline Dual<N, T>::Dual(const Dual<N, U>& other) :
    value{other.value},
    dual{other.dual} {
}

template <std::size_t N, typename T>
inline auto Dual<N, T>::operator+() const -> Dual {
    return *this;
}

/// Writing \f$\h{x}=x+\sum_{k=0}^{N-1}p'_k\epsilon_k\f$, we have
/// \f$-\h{x}=-x-\sum_{k=0}^{N-1}p'_k\epsilon_k\f$.
template <std::size_t N, typename T>
inline auto Dual<N, T>::operator-() const -> Dual {
    std::array<T, N> temp{dual};
    for (auto& x : temp) {
        x *= T{-1};
    }
    return {-value, temp};
}

/// Writing \f$\h{x}=x+\sum_{k=0}^{N-1}p'_k\epsilon_k\f$ and
/// \f$\h{y}=y+\sum_{k=0}^{N-1}q'_k\epsilon_k\f$, we have
/// \f$\h{x}+\h{y}=(x+y)+\sum_{k=0}^{N-1}(p'_k+q'_k)\epsilon_k\f$.
template <std::size_t N, typename T>
inline auto Dual<N, T>::operator+=(const Dual<N, T>& rhs) -> Dual& {
    value += rhs.value;
    for (std::size_t i(0); i < N; ++i) {
        dual[i] += rhs.dual[i];
    }
    return *this;
}

/// Writing \f$\h{x}=x+\sum_{k=0}^{N-1}p'_k\epsilon_k\f$ and
/// \f$\h{y}=y+\sum_{k=0}^{N-1}q'_k\epsilon_k\f$, we have
/// \f$\h{x}-\h{y}=(x-y)+\sum_{k=0}^{N-1}(p'_k-q'_k)\epsilon_k\f$.
template <std::size_t N, typename T>
inline auto Dual<N, T>::operator-=(const Dual<N, T>& rhs) -> Dual& {
    value -= rhs.value;
    for (std::size_t i(0); i < N; ++i) {
        dual[i] -= rhs.dual[i];
    }
    return *this;
}

/// Writing \f$\h{x}=x+\sum_{k=0}^{N-1}p'_k\epsilon_k\f$ and
/// \f$\h{y}=y+\sum_{k=0}^{N-1}q'_k\epsilon_k\f$, we have
/// \f{align*}
///  \h{x}\h{y}&=\left(x+\sum_{k=0}^{N-1}p'_k\epsilon_k\right)
///   \left(y+\sum_{k=0}^{N-1}q'_k\epsilon_k\right)
///  \\&=xy+\sum_{k=0}^{N-1}(yp'_k+xq'_k)\epsilon_k.
/// \f}
template <std::size_t N, typename T>
inline auto Dual<N, T>::operator*=(const Dual<N, T>& rhs) -> Dual& {
    for (std::size_t i(0); i < N; ++i) {
        dual[i] = dual[i]*rhs.value + value*rhs.dual[i];
    }
    value *= rhs.value;
    return *this;
}

/// Writing \f$\h{x}=x+\sum_{k=0}^{N-1}p'_k\epsilon_k\f$ and
/// \f$\h{y}=y+\sum_{k=0}^{N-1}q'_k\epsilon_k\f$, we have
/// \f{align*}
///  \frac{\h{x}}{\h{y}}&=
///   \frac{\left(x+\sum_{k=0}^{N-1}p'_k\epsilon_k\right)}
///   {\left(y+\sum_{k=0}^{N-1}q'_k\epsilon_k\right)}
///  \\&=\frac{1}{y}\left(x+\sum_{k=0}^{N-1}p'_k\epsilon_k\right)
///   \left(1-\frac{1}{y}\sum_{k=0}^{N-1}q'_k\epsilon_k\right)
///  \\&=\frac{x}{y}+\frac{1}{y}\sum_{k=0}^{N-1}
///   \left(p'_k-\frac{xq'_k}{y}\right)\epsilon_k.
/// \f}
template <std::size_t N, typename T>
inline auto Dual<N, T>::operator/=(const Dual<N, T>& rhs) -> Dual& {
    for (std::size_t i(0); i < N; ++i) {
        dual[i] = (dual[i] - rhs.dual[i]*value/rhs.value)/rhs.value;
    }
    value /= rhs.value;
    return *this;
}

/// \relates Jump::Dual
template <std::size_t N, typename T>
inline auto operator<<(std::ostream& out, const Dual<N, T>& rhs)
        -> std::ostream& {
    out << "(" << rhs.value << ", {";
    if constexpr (N > 0) {
        out << rhs.dual[0];
    }
    if constexpr (N > 1) {
        for (std::size_t i{1}; i < N; ++i) {
            out << ", " << rhs.dual[i];
        }
    }
    return out << "})";
}

// ========================================================================
// Arithmetic
// ========================================================================

/// \relates Dual
template <std::size_t N, typename T>
inline auto operator+(Dual<N, T> lhs, const Dual<N, T>& rhs) -> Dual<N, T> {
    lhs += rhs;
    return lhs;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto operator+(const Dual<N, T>& lhs, Dual<N, T>&& rhs) -> Dual<N, T> {
    rhs += lhs;
    return rhs;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto operator-(Dual<N, T> lhs, const Dual<N, T>& rhs) -> Dual<N, T> {
    lhs -= rhs;
    return lhs;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto operator-(const Dual<N, T>& lhs, Dual<N, T>&& rhs) -> Dual<N, T> {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto operator*(Dual<N, T> lhs, const Dual<N, T>& rhs) -> Dual<N, T> {
    lhs *= rhs;
    return lhs;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto operator*(const Dual<N, T>& lhs, Dual<N, T>&& rhs) -> Dual<N, T> {
    rhs *= lhs;
    return rhs;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto operator/(Dual<N, T> lhs, const Dual<N, T>& rhs) -> Dual<N, T> {
    lhs /= rhs;
    return lhs;
}

// ========================================================================
// Exponentiation
// ========================================================================

/// \relates Dual
template <std::size_t N, typename T>
inline auto exp(Dual<N, T> x) -> Dual<N, T> {
    x.value = std::exp(x.value);
    for (auto& d : x.dual) {
        d *= x.value;
    }
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto log(Dual<N, T> x) -> Dual<N, T> {
    for (auto& d : x.dual) {
        d /= x.value;
    }
    x.value = std::log(x.value);
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto pow(Dual<N, T> x, Dual<N, T> p) -> Dual<N, T> {
    return exp(log(x)*p);
}

// ========================================================================
// Trigonometry
// ========================================================================

/// \relates Dual
template <std::size_t N, typename T>
inline auto sin(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{std::cos(x.value)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::sin(x.value);
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto cos(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{-std::sin(x.value)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::cos(x.value);
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto tan(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{1./(std::pow(std::cos(x.value), 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::tan(x.value);
    return  x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto asin(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{1./std::sqrt(1. - std::pow(x.value, 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::asin(x.value);
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto acos(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{-1./std::sqrt(1. - std::pow(x.value, 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::acos(x.value);
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto atan(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{1./(1. + std::pow(x.value, 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::atan(x.value);
    return x;
}

// ========================================================================
// Hyperbolics
// ========================================================================

/// \relates Dual
template <std::size_t N, typename T>
inline auto sinh(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{std::cosh(x.value)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::sinh(x.value);
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto cosh(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{std::sinh(x.value)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::cosh(x.value);
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto tanh(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{1./(std::pow(std::cosh(x.value), 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::tanh(x.value);
    return  x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto asinh(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{1./std::sqrt(1. + std::pow(x.value, 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::asinh(x.value);
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto acosh(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{1./std::sqrt(std::pow(x.value, 2.) - 1.)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::acosh(x.value);
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto atanh(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{1./(1. - std::pow(x.value, 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::atanh(x.value);
    return x;
}

// ========================================================================
// Miscellaneous
// ========================================================================

/// \relates Dual
template <std::size_t N, typename T>
inline auto abs(Dual<N, T> x) -> Dual<N, T> {
    auto derivative{sgn(x.value)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::abs(x.value);
    return x;
}

/// \relates Dual
template <std::size_t N, typename T>
inline auto sgn(Dual<N, T> x) -> Dual<N, T> {
    x.dual.fill(0.);
    x.value = sgn(x.value);
}
}   // namespace jump

#endif  // JUMP_DUAL_HPP

