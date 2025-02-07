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
#include "jump/experimental/expression_templates/functors.hpp"
#include "jump/experimental/expression_templates/vector_expressions.hpp"

namespace jump {
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
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
requires (!VectorExpression<T>)
inline constexpr auto operator*(const T& lhs, const Expr& rhs)
        -> VectorMultiply<ConstantVectorExpression<T>, Expr>;

template <typename T, VectorExpression Expr>
requires (!VectorExpression<T>)
inline constexpr auto operator*(const Expr& lhs, const T& rhs)
        -> VectorMultiply<Expr, ConstantVectorExpression<T>>;

template <VectorExpression Left, VectorExpression Right>
constexpr auto operator/(const Left& lhs, const Right& rhs)
        -> VectorDivide<Left, Right>;

template <typename T, VectorExpression Expr>
requires (!VectorExpression<T>)
inline constexpr auto operator/(const T& lhs, const Expr& rhs)
        -> VectorDivide<ConstantVectorExpression<T>, Expr>;

template <typename T, VectorExpression Expr>
requires (!VectorExpression<T>)
inline constexpr auto operator/(const Expr& lhs, const T& rhs)
        -> VectorDivide<Expr, ConstantVectorExpression<T>>;

template <VectorExpression Left, VectorExpression Right,
         typename R = std::common_type_t<
         typename Left::ValueType, typename Right::ValueType>>
constexpr auto dot(const Left& lhs, const Right& rhs) -> R;

// ========================================================================
// Implementation
// ========================================================================

template <VectorExpression Expr>
inline constexpr auto operator+(const Expr& expr) -> VectorIdentity<Expr> {
    return VectorIdentity<Expr>(expr);
}

template <VectorExpression Expr>
inline constexpr auto operator-(const Expr& expr) -> VectorNegate<Expr> {
    return VectorNegate<Expr>(expr);
}

template <VectorExpression Left, VectorExpression Right>
inline constexpr auto operator+(const Left& lhs, const Right& rhs)
        -> VectorPlus<Left, Right> {
    return VectorPlus<Left, Right>(lhs, rhs);
}

template <VectorExpression Left, VectorExpression Right>
inline constexpr auto operator-(const Left& lhs, const Right& rhs)
        -> VectorMinus<Left, Right> {
    return VectorMinus<Left, Right>(lhs, rhs);
}

template <VectorExpression Left, VectorExpression Right>
inline constexpr auto operator*(const Left& lhs, const Right& rhs)
        -> VectorMultiply<Left, Right> {
    return VectorMultiply<Left, Right>(lhs, rhs);
}

template <typename T, VectorExpression Expr>
requires (!VectorExpression<T>)
inline constexpr auto operator*(const T& lhs, const Expr& rhs)
        -> VectorMultiply<ConstantVectorExpression<T>, Expr> {
    return VectorMultiply<ConstantVectorExpression<T>, Expr>(lhs, rhs);
}

template <typename T, VectorExpression Expr>
requires (!VectorExpression<T>)
inline constexpr auto operator*(const Expr& lhs, const T& rhs)
        -> VectorMultiply<Expr, ConstantVectorExpression<T>> {
    return VectorMultiply<Expr, ConstantVectorExpression<T>>(lhs, rhs);
}

template <VectorExpression Left, VectorExpression Right>
inline constexpr auto operator/(const Left& lhs, const Right& rhs)
        -> VectorDivide<Left, Right> {
    return VectorDivide<Left, Right>(lhs, rhs);
}

template <typename T, VectorExpression Expr>
requires (!VectorExpression<T>)
inline constexpr auto operator/(const T& lhs, const Expr& rhs)
        -> VectorDivide<ConstantVectorExpression<T>, Expr> {
    return VectorDivide<ConstantVectorExpression<T>, Expr>(lhs, rhs);
}

template <typename T, VectorExpression Expr>
requires (!VectorExpression<T>)
inline constexpr auto operator/(const Expr& lhs, const T& rhs)
        -> VectorDivide<Expr, ConstantVectorExpression<T>> {
    return VectorDivide<Expr, ConstantVectorExpression<T>>(lhs, rhs);
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
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_VECTOR_OPERATORS_HPP

