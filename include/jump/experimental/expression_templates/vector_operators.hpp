// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_VECTOR_OPERATORS_HPP
#define JUMP_EXPRESSION_TEMPLATE_VECTOR_OPERATORS_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/experimental/expression_templates/concepts.hpp"
#include "jump/experimental/expression_templates/constants.hpp"
#include "jump/experimental/expression_templates/vector_functors.hpp"
#include "jump/experimental/expression_templates/vector_expressions.hpp"

namespace jump {
// ========================================================================
// Arithmetic
// ========================================================================

template <VectorExpression Expr>
constexpr auto operator+(const Expr& expr) -> VectorIdentity<Expr>;

template <VectorExpression Expr>
constexpr auto operator-(const Expr& expr) -> VectorNegate<Expr>;

template <VectorExpression Left, VectorExpression Right>
constexpr auto operator+(const Left& lhs, const Right& rhs)
        -> VectorPlus<Left, Right>;

template <VectorExpression Left, VectorExpression Right>
constexpr auto operator-(const Left& lhs, const Right& rhs)
        -> VectorMinus<Left, Right>;

template <VectorExpression Left, VectorExpression Right>
constexpr auto operator*(const Left& lhs, const Right& rhs)
        -> VectorMultiply<Left, Right>;

template <typename T, VectorExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator*(const T& lhs, const Expr& rhs)
        -> VectorMultiply<ConstantVectorExpression<T>, Expr>;

template <typename T, VectorExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator*(const Expr& lhs, const T& rhs)
        -> VectorMultiply<Expr, ConstantVectorExpression<T>>;

template <VectorExpression Left, VectorExpression Right>
constexpr auto operator/(const Left& lhs, const Right& rhs)
        -> VectorDivide<Left, Right>;

template <typename T, VectorExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator/(const T& lhs, const Expr& rhs)
        -> VectorDivide<ConstantVectorExpression<T>, Expr>;

template <typename T, VectorExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator/(const Expr& lhs, const T& rhs)
        -> VectorDivide<Expr, ConstantVectorExpression<T>>;

template <VectorExpression Left, VectorExpression Right,
         typename R = std::common_type_t<
         typename Left::ValueType, typename Right::ValueType>>
constexpr auto dot(const Left& lhs, const Right& rhs) -> R;

// ========================================================================
// Exponentiation
// ========================================================================

template <VectorExpression Expr>
constexpr auto exp(const Expr& expr) -> VectorExp<Expr>;

template <VectorExpression Expr>
constexpr auto log(const Expr& expr) -> VectorLog<Expr>;

template <VectorExpression Expr>
constexpr auto pow(const Expr& expr, const typename Expr::ValueType& p)
        -> VectorPow<Expr>;

// ========================================================================
// Trigonometry
// ========================================================================

template <VectorExpression Expr>
constexpr auto sin(const Expr& expr) -> VectorSin<Expr>;

template <VectorExpression Expr>
constexpr auto cos(const Expr& expr) -> VectorCos<Expr>;

template <VectorExpression Expr>
constexpr auto tan(const Expr& expr) -> VectorTan<Expr>;

template <VectorExpression Expr>
constexpr auto asin(const Expr& expr) -> VectorAsin<Expr>;

template <VectorExpression Expr>
constexpr auto acos(const Expr& expr) -> VectorAcos<Expr>;

template <VectorExpression Expr>
constexpr auto atan(const Expr& expr) -> VectorAtan<Expr>;

// ========================================================================
// Hyperbolics
// ========================================================================

template <VectorExpression Expr>
constexpr auto sinh(const Expr& expr) -> VectorSinh<Expr>;

template <VectorExpression Expr>
constexpr auto cosh(const Expr& expr) -> VectorCosh<Expr>;

template <VectorExpression Expr>
constexpr auto tanh(const Expr& expr) -> VectorTanh<Expr>;

template <VectorExpression Expr>
constexpr auto asinh(const Expr& expr) -> VectorAsinh<Expr>;

template <VectorExpression Expr>
constexpr auto acosh(const Expr& expr) -> VectorAcosh<Expr>;

template <VectorExpression Expr>
constexpr auto atanh(const Expr& expr) -> VectorAtanh<Expr>;

// ========================================================================
// Miscellaneous
// ========================================================================

template <VectorExpression Expr>
constexpr auto abs(const Expr& expr) -> VectorAbs<Expr>;

template <VectorExpression Expr>
constexpr auto sgn(const Expr& expr) -> VectorSgn<Expr>;

// ========================================================================
// Implementation
// ========================================================================

template <VectorExpression Expr>
inline constexpr auto operator+(const Expr& expr) -> VectorIdentity<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto operator-(const Expr& expr) -> VectorNegate<Expr> {
    return {expr};
}

template <VectorExpression Left, VectorExpression Right>
inline constexpr auto operator+(const Left& lhs, const Right& rhs)
        -> VectorPlus<Left, Right> {
    return {lhs, rhs};
}

template <VectorExpression Left, VectorExpression Right>
inline constexpr auto operator-(const Left& lhs, const Right& rhs)
        -> VectorMinus<Left, Right> {
    return {lhs, rhs};
}

template <VectorExpression Left, VectorExpression Right>
inline constexpr auto operator*(const Left& lhs, const Right& rhs)
        -> VectorMultiply<Left, Right> {
    return {lhs, rhs};
}

template <typename T, VectorExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator*(const T& lhs, const Expr& rhs)
        -> VectorMultiply<ConstantVectorExpression<T>, Expr> {
    return {lhs, rhs};
}

template <typename T, VectorExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator*(const Expr& lhs, const T& rhs)
        -> VectorMultiply<Expr, ConstantVectorExpression<T>> {
    return {lhs, rhs};
}

template <VectorExpression Left, VectorExpression Right>
inline constexpr auto operator/(const Left& lhs, const Right& rhs)
        -> VectorDivide<Left, Right> {
    return {lhs, rhs};
}

template <typename T, VectorExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator/(const T& lhs, const Expr& rhs)
        -> VectorDivide<ConstantVectorExpression<T>, Expr> {
    return {lhs, rhs};
}

template <typename T, VectorExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator/(const Expr& lhs, const T& rhs)
        -> VectorDivide<Expr, ConstantVectorExpression<T>> {
    return {lhs, rhs};
}

template <VectorExpression Left, VectorExpression Right, typename R>
inline constexpr auto dot(const Left& lhs, const Right& rhs) -> R {
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG

    R result{0};
    for (std::size_t i{0}, N{lhs.size()}; i < N; ++i) {
        result += lhs[i]*rhs[i];
    }
    return result;
}

template <VectorExpression Expr>
inline constexpr auto exp(const Expr& expr) -> VectorExp<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto log(const Expr& expr) -> VectorLog<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto pow(const Expr& expr, const typename Expr::ValueType& p)
        -> VectorPow<Expr> {
    return {expr, p};
}

template <VectorExpression Expr>
inline constexpr auto sin(const Expr& expr) -> VectorSin<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto cos(const Expr& expr) -> VectorCos<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto tan(const Expr& expr) -> VectorTan<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto asin(const Expr& expr) -> VectorAsin<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto acos(const Expr& expr) -> VectorAcos<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto atan(const Expr& expr) -> VectorAtan<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto sinh(const Expr& expr) -> VectorSinh<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto cosh(const Expr& expr) -> VectorCosh<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto tanh(const Expr& expr) -> VectorTanh<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto asinh(const Expr& expr) -> VectorAsinh<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto acosh(const Expr& expr) -> VectorAcosh<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto atanh(const Expr& expr) -> VectorAtanh<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto abs(const Expr& expr) -> VectorAbs<Expr> {
    return {expr};
}

template <VectorExpression Expr>
inline constexpr auto sgn(const Expr& expr) -> VectorSgn<Expr> {
    return {expr};
}
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_VECTOR_OPERATORS_HPP

