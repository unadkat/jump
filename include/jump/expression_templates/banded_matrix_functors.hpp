// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_FUNCTORS_HPP
#define JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_FUNCTORS_HPP

#include "jump/expression_templates/banded_matrix_expressions.hpp"
#include "jump/expression_templates/vector_functors.hpp"

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

template <BandedMatrixExpression Left, BandedMatrixExpression Right>
struct BandedMatrixPlus : BinaryBandedMatrixOp<VectorPlus, Left, Right> {
    using BinaryBandedMatrixOp<VectorPlus, Left, Right>::BinaryBandedMatrixOp;
};

template <BandedMatrixExpression Left, BandedMatrixExpression Right>
struct BandedMatrixMinus : BinaryBandedMatrixOp<VectorMinus, Left, Right> {
    using BinaryBandedMatrixOp<VectorMinus, Left, Right>::BinaryBandedMatrixOp;
};

template <BandedMatrixExpression Left, BandedMatrixExpression Right>
struct BandedMatrixMultiply : BinaryBandedMatrixOp<VectorMultiply, Left,
        Right> {
    using BinaryBandedMatrixOp<VectorMultiply, Left,
            Right>::BinaryBandedMatrixOp;
};

template <BandedMatrixExpression Left, BandedMatrixExpression Right>
struct BandedMatrixDivide : BinaryBandedMatrixOp<VectorDivide, Left, Right> {
    using BinaryBandedMatrixOp<VectorDivide, Left, Right>::BinaryBandedMatrixOp;
};

// ========================================================================
// Exponentiation
// ========================================================================

template <BandedMatrixExpression Expr>
struct BandedMatrixExp : UnaryBandedMatrixOp<VectorExp, Expr> {
    using UnaryBandedMatrixOp<VectorExp, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixLog : UnaryBandedMatrixOp<VectorLog, Expr> {
    using UnaryBandedMatrixOp<VectorLog, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
class BandedMatrixPow : public UnaryBandedMatrixOp<VectorPow, Expr> {
    public:
        BandedMatrixPow(const Expr& expr,
                const typename Expr::InnerExpressionType::ValueType& p) :
            UnaryBandedMatrixOp<VectorPow, Expr>{expr},
            m_p{p} {
        }

        constexpr auto as_vector() const
                -> UnaryBandedMatrixOp<VectorPow, Expr>::InnerExpressionType {
            return {this->m_expr.as_vector(), m_p};
        }

    private:
        Expr::InnerExpressionType::ValueType m_p{};
};

// ========================================================================
// Trigonometry
// ========================================================================

template <BandedMatrixExpression Expr>
struct BandedMatrixSin : UnaryBandedMatrixOp<VectorSin, Expr> {
    using UnaryBandedMatrixOp<VectorSin, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixCos : UnaryBandedMatrixOp<VectorCos, Expr> {
    using UnaryBandedMatrixOp<VectorCos, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixTan : UnaryBandedMatrixOp<VectorTan, Expr> {
    using UnaryBandedMatrixOp<VectorTan, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixAsin : UnaryBandedMatrixOp<VectorAsin, Expr> {
    using UnaryBandedMatrixOp<VectorAsin, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixAcos : UnaryBandedMatrixOp<VectorAcos, Expr> {
    using UnaryBandedMatrixOp<VectorAcos, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixAtan : UnaryBandedMatrixOp<VectorAtan, Expr> {
    using UnaryBandedMatrixOp<VectorAtan, Expr>::UnaryBandedMatrixOp;
};

// ========================================================================
// Hyperbolics
// ========================================================================

template <BandedMatrixExpression Expr>
struct BandedMatrixSinh : UnaryBandedMatrixOp<VectorSinh, Expr> {
    using UnaryBandedMatrixOp<VectorSinh, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixCosh : UnaryBandedMatrixOp<VectorCosh, Expr> {
    using UnaryBandedMatrixOp<VectorCosh, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixTanh : UnaryBandedMatrixOp<VectorTanh, Expr> {
    using UnaryBandedMatrixOp<VectorTanh, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixAsinh : UnaryBandedMatrixOp<VectorAsinh, Expr> {
    using UnaryBandedMatrixOp<VectorAsinh, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixAcosh : UnaryBandedMatrixOp<VectorAcosh, Expr> {
    using UnaryBandedMatrixOp<VectorAcosh, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixAtanh : UnaryBandedMatrixOp<VectorAtanh, Expr> {
    using UnaryBandedMatrixOp<VectorAtanh, Expr>::UnaryBandedMatrixOp;
};

// ========================================================================
// Miscellaneous
// ========================================================================

template <BandedMatrixExpression Expr>
struct BandedMatrixAbs : UnaryBandedMatrixOp<VectorAbs, Expr> {
    using UnaryBandedMatrixOp<VectorAbs, Expr>::UnaryBandedMatrixOp;
};

template <BandedMatrixExpression Expr>
struct BandedMatrixSgn : UnaryBandedMatrixOp<VectorSgn, Expr> {
    using UnaryBandedMatrixOp<VectorSgn, Expr>::UnaryBandedMatrixOp;
};
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_FUNCTORS_HPP

