// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_VECTOR_FUNCTORS_HPP
#define JUMP_EXPRESSION_TEMPLATE_VECTOR_FUNCTORS_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/expression_templates/functional.hpp"
#include "jump/expression_templates/vector_expressions.hpp"

#include <format>
#include <functional>

namespace jump {
// ========================================================================
// Arithmetic
// ========================================================================

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

// Cross product needs special treatment since it's not simply an elementwise
// operation
template <VectorExpression Left, VectorExpression Right>
struct VectorCross {
    public:
        using ValueType = std::remove_cvref_t<std::common_type_t<
            typename Left::ValueType, typename Right::ValueType>>;

        static constexpr bool is_vector_expression_leaf{false};

        constexpr VectorCross(const Left& lhs, const Right& rhs);

        constexpr auto operator[](std::size_t index) const -> ValueType;
        constexpr auto size() const -> std::size_t;

    protected:
        typename std::conditional<Left::is_vector_expression_leaf,
                 const Left&, Left>::type m_lhs;
        typename std::conditional<Right::is_vector_expression_leaf,
                 const Right&, Right>::type m_rhs;
};

template <VectorExpression Left, VectorExpression Right>
inline constexpr VectorCross<Left, Right>::VectorCross(const Left& lhs,
        const Right& rhs) :
    m_lhs{lhs}, m_rhs{rhs} {
    if constexpr (is_constant_vector_expression_v<Left>
            && is_constant_vector_expression_v<Right>) {
        throw RuntimeError{InvalidArgumentError{
            .argument = "is_constant_vector_expression_v<Left> "
                "&& is_constant_vector_expression_v<Right>",
            .value = "true",
            .expected = "only one of the arguments being constant"}};
    } else if constexpr (is_constant_vector_expression_v<Left>) {
        m_lhs.set_size(rhs.size());
    } else if constexpr (is_constant_vector_expression_v<Right>) {
        m_rhs.set_size(lhs.size());
    }
#ifndef NDEBUG
    if (m_lhs.size() != m_rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "m_lhs",
            .size1 = m_lhs.size(), .name2 = "m_rhs", .size2 = m_rhs.size()}};
    }
    if (m_lhs.size() != 3) {
        throw RuntimeError{InvalidArgumentError{
            .argument = "lhs.size()", .value = std::format("{}", m_lhs.size()),
            .expected = "lhs.size() == 3"}};
    }
#endif  // NDEBUG
}

template <VectorExpression Left, VectorExpression Right>
inline constexpr auto VectorCross<Left, Right>::operator[](
        std::size_t index) const -> ValueType {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    switch (index) {
        case 0:
            return m_lhs[1]*m_rhs[2] - m_lhs[2]*m_rhs[1];
        case 1:
            return m_lhs[2]*m_rhs[0] - m_lhs[0]*m_rhs[2];
        case 2:
            return m_lhs[0]*m_rhs[1] - m_lhs[1]*m_rhs[0];
        default:
            return ValueType{0};
    }
}

template <VectorExpression Left, VectorExpression Right>
inline constexpr auto VectorCross<Left, Right>::size() const -> std::size_t {
    return m_lhs.size();
}

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

template <VectorExpression Expr>
struct VectorPow : UnaryVectorOp<functional::Pow<typename Expr::ValueType>,
        Expr> {
    VectorPow(const Expr& expr, const typename Expr::ValueType& p) :
        UnaryVectorOp<functional::Pow<typename Expr::ValueType>, Expr>{expr} {
        this->m_operator.p = p;
    }
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

#endif  // JUMP_EXPRESSION_TEMPLATE_VECTOR_FUNCTORS_HPP

