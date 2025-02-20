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
#endif  // JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_OPERATORS_HPP

