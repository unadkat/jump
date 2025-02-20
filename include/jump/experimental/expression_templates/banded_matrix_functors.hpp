// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_FUNCTORS_HPP
#define JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_FUNCTORS_HPP

#include "jump/experimental/expression_templates/banded_matrix_expressions.hpp"
#include "jump/experimental/expression_templates/vector_functors.hpp"

namespace jump {
// ========================================================================
// Arithmetic
// ========================================================================

template <BandedMatrixExpression Expr>
struct BandedMatrixIdentity : UnaryBandedMatrixOp<VectorIdentity, Expr> {
    using UnaryBandedMatrixOp<VectorIdentity, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixNegate : UnaryBandedMatrixOp<VectorNegate, Expr> {
    using UnaryBandedMatrixOp<VectorNegate, Expr>::UnaryBandedMatrixOp;
};
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_FUNCTORS_HPP

