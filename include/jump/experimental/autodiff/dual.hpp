// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_DUAL_HPP
#define JUMP_DUAL_HPP

#include "jump/experimental/autodiff/dual_decl.hpp"

namespace jump::experimental {
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
    dual.fill(T{0});
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

    dual.fill(T{0});
    dual[index] = d;
}

/// \brief Conversion operator to promote a real-valued `Dual` to a
/// complex-valued one.
///
/// Initialises a complex-valued `Dual` using explicit conversion of Real to
/// Complex types.
template <std::size_t N, typename T>
inline Dual<N, T>::operator Dual<N, Complex>() const {
    Dual<N, Complex> result{value};
    for (std::size_t i{0}; i < N; ++i) {
        result.dual[i] = Complex{dual[i]};
    }
    return result;
}

template <std::size_t N, typename T>
inline Dual<N, T> Dual<N, T>::operator+() const {
    return *this;
}

/// Writing \f$\h{x}=x+\sum_{k=0}^{N-1}p'_k\epsilon_k\f$, we have
/// \f$-\h{x}=-x-\sum_{k=0}^{N-1}p'_k\epsilon_k\f$.
template <std::size_t N, typename T>
inline Dual<N, T> Dual<N, T>::operator-() const {
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
inline Dual<N, T>& Dual<N, T>::operator+=(const Dual<N, T>& rhs) {
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
inline Dual<N, T>& Dual<N, T>::operator-=(const Dual<N, T>& rhs) {
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
inline Dual<N, T>& Dual<N, T>::operator*=(const Dual<N, T>& rhs) {
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
inline Dual<N, T>& Dual<N, T>::operator/=(const Dual<N, T>& rhs) {
    for (std::size_t i(0); i < N; ++i) {
        dual[i] = (dual[i] - rhs.dual[i]*value/rhs.value)/rhs.value;
    }
    value /= rhs.value;
    return *this;
}


/// \relates Jump::Dual
template <std::size_t N, typename T, typename Os>
inline Os& operator<<(Os& out, const Dual<N, T>& rhs) {
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
}   // namespace jump::experimental

#endif  // JUMP_DUAL_HPP

