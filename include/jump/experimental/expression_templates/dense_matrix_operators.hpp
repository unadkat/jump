// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_DENSE_MATRIX_OPERATORS_HPP
#define JUMP_EXPRESSION_TEMPLATE_DENSE_MATRIX_OPERATORS_HPP

#include "jump/experimental/expression_templates/concepts.hpp"
#include "jump/experimental/expression_templates/dense_matrix_functors.hpp"
#include "jump/experimental/expression_templates/dense_matrix_expressions.hpp"

namespace jump {
#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
// ========================================================================
// Arithmetic
// ========================================================================

template <DenseMatrixExpression Expr>
constexpr auto operator+(const Expr& expr) -> DenseMatrixIdentity<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto operator-(const Expr& expr) -> DenseMatrixNegate<Expr>;

template <DenseMatrixExpression Left, DenseMatrixExpression Right>
constexpr auto operator+(const Left& lhs, const Right& rhs)
        -> DenseMatrixPlus<Left, Right>;

template <DenseMatrixExpression Left, DenseMatrixExpression Right>
constexpr auto operator-(const Left& lhs, const Right& rhs)
        -> DenseMatrixMinus<Left, Right>;

template <typename T, DenseMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator*(const T& lhs, const Expr& rhs)
        -> DenseMatrixMultiply<ConstantMatrixExpression<T>, Expr>;

template <typename T, DenseMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator*(const Expr& lhs, const T& rhs)
        -> DenseMatrixMultiply<Expr, ConstantMatrixExpression<T>>;

template <typename T, DenseMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator/(const T& lhs, const Expr& rhs)
        -> DenseMatrixDivide<ConstantMatrixExpression<T>, Expr>;

template <typename T, DenseMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator/(const Expr& lhs, const T& rhs)
        -> DenseMatrixDivide<Expr, ConstantMatrixExpression<T>>;

// ========================================================================
// Exponentiation
// ========================================================================

template <DenseMatrixExpression Expr>
constexpr auto exp(const Expr& expr) -> DenseMatrixExp<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto log(const Expr& expr) -> DenseMatrixLog<Expr>;

// ========================================================================
// Trigonometry
// ========================================================================

template <DenseMatrixExpression Expr>
constexpr auto sin(const Expr& expr) -> DenseMatrixSin<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto cos(const Expr& expr) -> DenseMatrixCos<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto tan(const Expr& expr) -> DenseMatrixTan<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto asin(const Expr& expr) -> DenseMatrixAsin<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto acos(const Expr& expr) -> DenseMatrixAcos<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto atan(const Expr& expr) -> DenseMatrixAtan<Expr>;

// ========================================================================
// Hyperbolics
// ========================================================================

template <DenseMatrixExpression Expr>
constexpr auto sinh(const Expr& expr) -> DenseMatrixSinh<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto cosh(const Expr& expr) -> DenseMatrixCosh<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto tanh(const Expr& expr) -> DenseMatrixTanh<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto asinh(const Expr& expr) -> DenseMatrixAsinh<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto acosh(const Expr& expr) -> DenseMatrixAcosh<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto atanh(const Expr& expr) -> DenseMatrixAtanh<Expr>;

// ========================================================================
// Miscellaneous
// ========================================================================

template <DenseMatrixExpression Expr>
constexpr auto abs(const Expr& expr) -> DenseMatrixAbs<Expr>;

template <DenseMatrixExpression Expr>
constexpr auto sgn(const Expr& expr) -> DenseMatrixSgn<Expr>;

// ========================================================================
// Implementation
// ========================================================================

template <DenseMatrixExpression Expr>
inline constexpr auto operator+(const Expr& expr)
        -> DenseMatrixIdentity<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto operator-(const Expr& expr) -> DenseMatrixNegate<Expr> {
    return {expr};
}

template <DenseMatrixExpression Left, DenseMatrixExpression Right>
inline constexpr auto operator+(const Left& lhs, const Right& rhs)
        -> DenseMatrixPlus<Left, Right> {
    return {lhs, rhs};
}

template <DenseMatrixExpression Left, DenseMatrixExpression Right>
inline constexpr auto operator-(const Left& lhs, const Right& rhs)
        -> DenseMatrixMinus<Left, Right> {
    return {lhs, rhs};
}

template <typename T, DenseMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator*(const T& lhs, const Expr& rhs)
        -> DenseMatrixMultiply<ConstantMatrixExpression<T>, Expr> {
    return {lhs, rhs};
}

template <typename T, DenseMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator*(const Expr& lhs, const T& rhs)
        -> DenseMatrixMultiply<Expr, ConstantMatrixExpression<T>> {
    return {lhs, rhs};
}

template <typename T, DenseMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator/(const T& lhs, const Expr& rhs)
        -> DenseMatrixDivide<ConstantMatrixExpression<T>, Expr> {
    return {lhs, rhs};
}

template <typename T, DenseMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator/(const Expr& lhs, const T& rhs)
        -> DenseMatrixDivide<Expr, ConstantMatrixExpression<T>> {
    return {lhs, rhs};
}

template <DenseMatrixExpression Expr>
inline constexpr auto exp(const Expr& expr) -> DenseMatrixExp<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto log(const Expr& expr) -> DenseMatrixLog<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto pow(const Expr& expr,
        const typename Expr::InnerExpressionType::ValueType& p)
        -> DenseMatrixPow<Expr> {
    return {expr, p};
}

template <DenseMatrixExpression Expr>
inline constexpr auto sin(const Expr& expr) -> DenseMatrixSin<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto cos(const Expr& expr) -> DenseMatrixCos<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto tan(const Expr& expr) -> DenseMatrixTan<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto asin(const Expr& expr) -> DenseMatrixAsin<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto acos(const Expr& expr) -> DenseMatrixAcos<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto atan(const Expr& expr) -> DenseMatrixAtan<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto sinh(const Expr& expr) -> DenseMatrixSinh<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto cosh(const Expr& expr) -> DenseMatrixCosh<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto tanh(const Expr& expr) -> DenseMatrixTanh<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto asinh(const Expr& expr) -> DenseMatrixAsinh<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto acosh(const Expr& expr) -> DenseMatrixAcosh<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto atanh(const Expr& expr) -> DenseMatrixAtanh<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto abs(const Expr& expr) -> DenseMatrixAbs<Expr> {
    return {expr};
}

template <DenseMatrixExpression Expr>
inline constexpr auto sgn(const Expr& expr) -> DenseMatrixSgn<Expr> {
    return {expr};
}
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_DENSE_MATRIX_OPERATORS_HPP

