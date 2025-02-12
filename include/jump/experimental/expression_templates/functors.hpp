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

// ========================================================================
// Trigonometry
// ========================================================================

template <VectorExpression Expr>
struct VectorSin : UnaryVectorOp<functional::Sin, Expr> {
    using UnaryVectorOp<functional::Sin, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorCos : UnaryVectorOp<functional::Cos, Expr> {
    using UnaryVectorOp<functional::Cos, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorTan : UnaryVectorOp<functional::Tan, Expr> {
    using UnaryVectorOp<functional::Tan, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorAsin : UnaryVectorOp<functional::Asin, Expr> {
    using UnaryVectorOp<functional::Asin, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorAcos : UnaryVectorOp<functional::Acos, Expr> {
    using UnaryVectorOp<functional::Acos, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorAtan : UnaryVectorOp<functional::Atan, Expr> {
    using UnaryVectorOp<functional::Atan, Expr>::UnaryVectorOp;
};

// ========================================================================
// Hyperbolics
// ========================================================================

template <VectorExpression Expr>
struct VectorSinh : UnaryVectorOp<functional::Sinh, Expr> {
    using UnaryVectorOp<functional::Sinh, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorCosh : UnaryVectorOp<functional::Cosh, Expr> {
    using UnaryVectorOp<functional::Cosh, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorTanh : UnaryVectorOp<functional::Tanh, Expr> {
    using UnaryVectorOp<functional::Tanh, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorAsinh : UnaryVectorOp<functional::Asinh, Expr> {
    using UnaryVectorOp<functional::Asinh, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorAcosh : UnaryVectorOp<functional::Acosh, Expr> {
    using UnaryVectorOp<functional::Acosh, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorAtanh : UnaryVectorOp<functional::Atanh, Expr> {
    using UnaryVectorOp<functional::Atanh, Expr>::UnaryVectorOp;
};

// ========================================================================
// Miscellaneous
// ========================================================================

template <VectorExpression Expr>
struct VectorAbs : UnaryVectorOp<functional::Abs, Expr> {
    using UnaryVectorOp<functional::Abs, Expr>::UnaryVectorOp;
};

template <VectorExpression Expr>
struct VectorSgn : UnaryVectorOp<functional::Sgn, Expr> {
    using UnaryVectorOp<functional::Sgn, Expr>::UnaryVectorOp;
};
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_FUNCTORS_HPP

