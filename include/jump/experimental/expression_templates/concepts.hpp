// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_CONCEPTS_HPP
#define JUMP_EXPRESSION_TEMPLATE_CONCEPTS_HPP

#include <concepts>
#include <type_traits>

namespace jump {
template <typename Expr>
concept VectorExpression = requires (Expr expr, std::size_t i) {
    typename Expr::ValueType;
    // Vector expressions should define a bool member that determines if it is a
    // leaf of a compound expression or not
    expr.is_vector_expression_leaf;
    requires std::same_as<
        std::remove_cvref_t<decltype(expr.is_vector_expression_leaf)>, bool>;
    // For use in evaluating the whole vector in a single loop
    {expr.size()} -> std::same_as<std::size_t>;
    // Expression should evaluate, and to a value of appropriate type. They may
    // return results by value or by const reference (in the case of leaves of
    // an expression)
    expr[i];
    requires std::same_as<std::remove_cvref_t<decltype(expr[i])>,
             std::remove_cvref_t<typename Expr::ValueType>>;
};

template <typename Expr, typename T>
concept VectorExpressionConvertibleTo = VectorExpression<Expr>
        && std::convertible_to<typename Expr::ValueType, T>;
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_CONCEPTS_HPP

