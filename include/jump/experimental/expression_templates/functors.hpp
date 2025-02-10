// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_FUNCTORS_HPP
#define JUMP_EXPRESSION_TEMPLATE_FUNCTORS_HPP

#include "jump/experimental/expression_templates/functional.hpp"
#include "jump/experimental/expression_templates/vector_expressions.hpp"

#include <functional>

namespace jump {
template <VectorExpression Expr>
struct VectorIdentity : UnaryVectorOp<std::identity, Expr> {
    using UnaryVectorOp<std::identity, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorNegate : UnaryVectorOp<std::negate<>, Expr> {
    using UnaryVectorOp<std::negate<>, Expr>::UnaryVectorOp;
};

template <VectorExpression Left, VectorExpression Right>
struct VectorPlus : BinaryVectorOp<std::plus<>, Left, Right> {
    using BinaryVectorOp<std::plus<>, Left, Right>::BinaryVectorOp;
};

template <VectorExpression Left, VectorExpression Right>
struct VectorMinus : BinaryVectorOp<std::minus<>, Left, Right> {
    using BinaryVectorOp<std::minus<>, Left, Right>::BinaryVectorOp;
};

template <VectorExpression Left, VectorExpression Right>
struct VectorMultiply : BinaryVectorOp<std::multiplies<>, Left, Right> {
    using BinaryVectorOp<std::multiplies<>, Left, Right>::BinaryVectorOp;
};

template <VectorExpression Left, VectorExpression Right>
struct VectorDivide : BinaryVectorOp<std::divides<>, Left, Right> {
    using BinaryVectorOp<std::divides<>, Left, Right>::BinaryVectorOp;
};

// ========================================================================
// Exponentiation
// ========================================================================

template <VectorExpression Expr>
struct VectorExp : UnaryVectorOp<functional::Exp, Expr> {
    using UnaryVectorOp<functional::Exp, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorLog : UnaryVectorOp<functional::Log, Expr> {
    using UnaryVectorOp<functional::Log, Expr>::UnaryVectorOp;
};
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_FUNCTORS_HPP

