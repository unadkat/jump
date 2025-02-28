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
requires (!BandedMatrixExpression<T>)
constexpr auto operator*(const T& lhs, const Expr& rhs)
        -> BandedMatrixMultiply<ConstantMatrixExpression<T>, Expr>;

template <typename T, BandedMatrixExpression Expr>
requires (!BandedMatrixExpression<T>)
constexpr auto operator*(const Expr& lhs, const T& rhs)
        -> BandedMatrixMultiply<Expr, ConstantMatrixExpression<T>>;

template <typename T, BandedMatrixExpression Expr>
requires (!BandedMatrixExpression<T>)
constexpr auto operator/(const T& lhs, const Expr& rhs)
        -> BandedMatrixDivide<ConstantMatrixExpression<T>, Expr>;

template <typename T, BandedMatrixExpression Expr>
requires (!BandedMatrixExpression<T>)
constexpr auto operator/(const Expr& lhs, const T& rhs)
        -> BandedMatrixDivide<Expr, ConstantMatrixExpression<T>>;

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
requires (!BandedMatrixExpression<T>)
inline constexpr auto operator*(const T& lhs, const Expr& rhs)
        -> BandedMatrixMultiply<ConstantMatrixExpression<T>, Expr> {
    return {lhs, rhs};
}

template <typename T, BandedMatrixExpression Expr>
requires (!BandedMatrixExpression<T>)
inline constexpr auto operator*(const Expr& lhs, const T& rhs)
        -> BandedMatrixMultiply<Expr, ConstantMatrixExpression<T>> {
    return {lhs, rhs};
}

template <typename T, BandedMatrixExpression Expr>
requires (!BandedMatrixExpression<T>)
inline constexpr auto operator/(const T& lhs, const Expr& rhs)
        -> BandedMatrixDivide<ConstantMatrixExpression<T>, Expr> {
    return {lhs, rhs};
}

template <typename T, BandedMatrixExpression Expr>
requires (!BandedMatrixExpression<T>)
inline constexpr auto operator/(const Expr& lhs, const T& rhs)
        -> BandedMatrixDivide<Expr, ConstantMatrixExpression<T>> {
    return {lhs, rhs};
}
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_OPERATORS_HPP

