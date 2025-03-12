// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_OPERATORS_HPP
#define JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_OPERATORS_HPP

#include "jump/experimental/expression_templates/concepts.hpp"
#include "jump/experimental/expression_templates/banded_matrix_functors.hpp"
#include "jump/experimental/expression_templates/banded_matrix_expressions.hpp"

namespace jump {
#ifdef JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
// ========================================================================
// Arithmetic
// ========================================================================

template <BandedMatrixExpression Expr>
constexpr auto operator+(const Expr& expr) -> BandedMatrixIdentity<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto operator-(const Expr& expr) -> BandedMatrixNegate<Expr>;

template <BandedMatrixExpression Left, BandedMatrixExpression Right>
constexpr auto operator+(const Left& lhs, const Right& rhs)
        -> BandedMatrixPlus<Left, Right>;

template <BandedMatrixExpression Left, BandedMatrixExpression Right>
constexpr auto operator-(const Left& lhs, const Right& rhs)
        -> BandedMatrixMinus<Left, Right>;

template <typename T, BandedMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator*(const T& lhs, const Expr& rhs)
        -> BandedMatrixMultiply<ConstantMatrixExpression<T>, Expr>;

template <typename T, BandedMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator*(const Expr& lhs, const T& rhs)
        -> BandedMatrixMultiply<Expr, ConstantMatrixExpression<T>>;

template <typename T, BandedMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator/(const T& lhs, const Expr& rhs)
        -> BandedMatrixDivide<ConstantMatrixExpression<T>, Expr>;

template <typename T, BandedMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
constexpr auto operator/(const Expr& lhs, const T& rhs)
        -> BandedMatrixDivide<Expr, ConstantMatrixExpression<T>>;

// ========================================================================
// Exponentiation
// ========================================================================

template <BandedMatrixExpression Expr>
constexpr auto exp(const Expr& expr) -> BandedMatrixExp<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto log(const Expr& expr) -> BandedMatrixLog<Expr>;

// ========================================================================
// Trigonometry
// ========================================================================

template <BandedMatrixExpression Expr>
constexpr auto sin(const Expr& expr) -> BandedMatrixSin<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto cos(const Expr& expr) -> BandedMatrixCos<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto tan(const Expr& expr) -> BandedMatrixTan<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto asin(const Expr& expr) -> BandedMatrixAsin<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto acos(const Expr& expr) -> BandedMatrixAcos<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto atan(const Expr& expr) -> BandedMatrixAtan<Expr>;

// ========================================================================
// Hyperbolics
// ========================================================================

template <BandedMatrixExpression Expr>
constexpr auto sinh(const Expr& expr) -> BandedMatrixSinh<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto cosh(const Expr& expr) -> BandedMatrixCosh<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto tanh(const Expr& expr) -> BandedMatrixTanh<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto asinh(const Expr& expr) -> BandedMatrixAsinh<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto acosh(const Expr& expr) -> BandedMatrixAcosh<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto atanh(const Expr& expr) -> BandedMatrixAtanh<Expr>;

// ========================================================================
// Miscellaneous
// ========================================================================

template <BandedMatrixExpression Expr>
constexpr auto abs(const Expr& expr) -> BandedMatrixAbs<Expr>;

template <BandedMatrixExpression Expr>
constexpr auto sgn(const Expr& expr) -> BandedMatrixSgn<Expr>;

// ========================================================================
// Implementation
// ========================================================================

template <BandedMatrixExpression Expr>
inline constexpr auto operator+(const Expr& expr)
        -> BandedMatrixIdentity<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto operator-(const Expr& expr) -> BandedMatrixNegate<Expr> {
    return {expr};
}

template <BandedMatrixExpression Left, BandedMatrixExpression Right>
inline constexpr auto operator+(const Left& lhs, const Right& rhs)
        -> BandedMatrixPlus<Left, Right> {
    return {lhs, rhs};
}

template <BandedMatrixExpression Left, BandedMatrixExpression Right>
inline constexpr auto operator-(const Left& lhs, const Right& rhs)
        -> BandedMatrixMinus<Left, Right> {
    return {lhs, rhs};
}

template <typename T, BandedMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator*(const T& lhs, const Expr& rhs)
        -> BandedMatrixMultiply<ConstantMatrixExpression<T>, Expr> {
    return {lhs, rhs};
}

template <typename T, BandedMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator*(const Expr& lhs, const T& rhs)
        -> BandedMatrixMultiply<Expr, ConstantMatrixExpression<T>> {
    return {lhs, rhs};
}

template <typename T, BandedMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator/(const T& lhs, const Expr& rhs)
        -> BandedMatrixDivide<ConstantMatrixExpression<T>, Expr> {
    return {lhs, rhs};
}

template <typename T, BandedMatrixExpression Expr>
requires (!ExpressionTemplate<T>)
inline constexpr auto operator/(const Expr& lhs, const T& rhs)
        -> BandedMatrixDivide<Expr, ConstantMatrixExpression<T>> {
    return {lhs, rhs};
}

template <BandedMatrixExpression Expr>
inline constexpr auto exp(const Expr& expr) -> BandedMatrixExp<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto log(const Expr& expr) -> BandedMatrixLog<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto pow(const Expr& expr,
        const typename Expr::InnerExpressionType::ValueType& p)
        -> BandedMatrixPow<Expr> {
    return {expr, p};
}

template <BandedMatrixExpression Expr>
inline constexpr auto sin(const Expr& expr) -> BandedMatrixSin<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto cos(const Expr& expr) -> BandedMatrixCos<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto tan(const Expr& expr) -> BandedMatrixTan<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto asin(const Expr& expr) -> BandedMatrixAsin<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto acos(const Expr& expr) -> BandedMatrixAcos<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto atan(const Expr& expr) -> BandedMatrixAtan<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto sinh(const Expr& expr) -> BandedMatrixSinh<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto cosh(const Expr& expr) -> BandedMatrixCosh<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto tanh(const Expr& expr) -> BandedMatrixTanh<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto asinh(const Expr& expr) -> BandedMatrixAsinh<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto acosh(const Expr& expr) -> BandedMatrixAcosh<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto atanh(const Expr& expr) -> BandedMatrixAtanh<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto abs(const Expr& expr) -> BandedMatrixAbs<Expr> {
    return {expr};
}

template <BandedMatrixExpression Expr>
inline constexpr auto sgn(const Expr& expr) -> BandedMatrixSgn<Expr> {
    return {expr};
}
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_OPERATORS_HPP

