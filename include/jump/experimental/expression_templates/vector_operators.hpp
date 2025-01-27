// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_VECTOR_OPERATORS_HPP
#define JUMP_EXPRESSION_TEMPLATE_VECTOR_OPERATORS_HPP

#include "jump/experimental/expression_templates/concepts.hpp"
#include "jump/experimental/expression_templates/vector_expressions.hpp"

namespace jump {
#ifdef JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
template <VectorExpression Left, VectorExpression Right>
inline constexpr auto operator+(const Left& lhs, const Right& rhs) 
        -> VectorAdd<Left, Right>  {
    return VectorAdd<Left, Right>(lhs, rhs);
}

template <VectorExpression Expr>
inline constexpr auto operator-(const Expr& expr) -> VectorNegate<Expr> {
    return VectorNegate<Expr>(expr);
}
#endif  // JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_VECTOR_OPERATORS_HPP

