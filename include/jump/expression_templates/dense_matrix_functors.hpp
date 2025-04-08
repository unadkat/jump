// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_DENSE_MATRIX_FUNCTORS_HPP
#define JUMP_EXPRESSION_TEMPLATE_DENSE_MATRIX_FUNCTORS_HPP

#include "jump/expression_templates/dense_matrix_expressions.hpp"
#include "jump/expression_templates/vector_functors.hpp"

namespace jump {
// ========================================================================
// Arithmetic
// ========================================================================

template <DenseMatrixExpression Expr>
struct DenseMatrixIdentity : UnaryDenseMatrixOp<VectorIdentity, Expr> {
    using UnaryDenseMatrixOp<VectorIdentity, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixNegate : UnaryDenseMatrixOp<VectorNegate, Expr> {
    using UnaryDenseMatrixOp<VectorNegate, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Left, DenseMatrixExpression Right>
struct DenseMatrixPlus : BinaryDenseMatrixOp<VectorPlus, Left, Right> {
    using BinaryDenseMatrixOp<VectorPlus, Left, Right>::BinaryDenseMatrixOp;
};

template <DenseMatrixExpression Left, DenseMatrixExpression Right>
struct DenseMatrixMinus : BinaryDenseMatrixOp<VectorMinus, Left, Right> {
    using BinaryDenseMatrixOp<VectorMinus, Left, Right>::BinaryDenseMatrixOp;
};

template <DenseMatrixExpression Left, DenseMatrixExpression Right>
struct DenseMatrixMultiply : BinaryDenseMatrixOp<VectorMultiply, Left,
        Right> {
    using BinaryDenseMatrixOp<VectorMultiply, Left,
            Right>::BinaryDenseMatrixOp;
};

template <DenseMatrixExpression Left, DenseMatrixExpression Right>
struct DenseMatrixDivide : BinaryDenseMatrixOp<VectorDivide, Left, Right> {
    using BinaryDenseMatrixOp<VectorDivide, Left, Right>::BinaryDenseMatrixOp;
};

// ========================================================================
// Exponentiation
// ========================================================================

template <DenseMatrixExpression Expr>
struct DenseMatrixExp : UnaryDenseMatrixOp<VectorExp, Expr> {
    using UnaryDenseMatrixOp<VectorExp, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixLog : UnaryDenseMatrixOp<VectorLog, Expr> {
    using UnaryDenseMatrixOp<VectorLog, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
class DenseMatrixPow : public UnaryDenseMatrixOp<VectorPow, Expr> {
    public:
        DenseMatrixPow(const Expr& expr,
                const typename Expr::InnerExpressionType::ValueType& p) :
            UnaryDenseMatrixOp<VectorPow, Expr>{expr},
            m_p{p} {
        }

        constexpr auto as_vector() const
                -> UnaryDenseMatrixOp<VectorPow, Expr>::InnerExpressionType {
            return {this->m_expr.as_vector(), m_p};
        }

    private:
        Expr::InnerExpressionType::ValueType m_p{};
};

// ========================================================================
// Trigonometry
// ========================================================================

template <DenseMatrixExpression Expr>
struct DenseMatrixSin : UnaryDenseMatrixOp<VectorSin, Expr> {
    using UnaryDenseMatrixOp<VectorSin, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixCos : UnaryDenseMatrixOp<VectorCos, Expr> {
    using UnaryDenseMatrixOp<VectorCos, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixTan : UnaryDenseMatrixOp<VectorTan, Expr> {
    using UnaryDenseMatrixOp<VectorTan, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixAsin : UnaryDenseMatrixOp<VectorAsin, Expr> {
    using UnaryDenseMatrixOp<VectorAsin, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixAcos : UnaryDenseMatrixOp<VectorAcos, Expr> {
    using UnaryDenseMatrixOp<VectorAcos, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixAtan : UnaryDenseMatrixOp<VectorAtan, Expr> {
    using UnaryDenseMatrixOp<VectorAtan, Expr>::UnaryDenseMatrixOp;
};

// ========================================================================
// Hyperbolics
// ========================================================================

template <DenseMatrixExpression Expr>
struct DenseMatrixSinh : UnaryDenseMatrixOp<VectorSinh, Expr> {
    using UnaryDenseMatrixOp<VectorSinh, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixCosh : UnaryDenseMatrixOp<VectorCosh, Expr> {
    using UnaryDenseMatrixOp<VectorCosh, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixTanh : UnaryDenseMatrixOp<VectorTanh, Expr> {
    using UnaryDenseMatrixOp<VectorTanh, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixAsinh : UnaryDenseMatrixOp<VectorAsinh, Expr> {
    using UnaryDenseMatrixOp<VectorAsinh, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixAcosh : UnaryDenseMatrixOp<VectorAcosh, Expr> {
    using UnaryDenseMatrixOp<VectorAcosh, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixAtanh : UnaryDenseMatrixOp<VectorAtanh, Expr> {
    using UnaryDenseMatrixOp<VectorAtanh, Expr>::UnaryDenseMatrixOp;
};

// ========================================================================
// Miscellaneous
// ========================================================================

template <DenseMatrixExpression Expr>
struct DenseMatrixAbs : UnaryDenseMatrixOp<VectorAbs, Expr> {
    using UnaryDenseMatrixOp<VectorAbs, Expr>::UnaryDenseMatrixOp;
};

template <DenseMatrixExpression Expr>
struct DenseMatrixSgn : UnaryDenseMatrixOp<VectorSgn, Expr> {
    using UnaryDenseMatrixOp<VectorSgn, Expr>::UnaryDenseMatrixOp;
};
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_DENSE_MATRIX_FUNCTORS_HPP

