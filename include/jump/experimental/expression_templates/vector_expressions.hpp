// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_VECTOR_EXPRESSIONS_HPP
#define JUMP_EXPRESSION_TEMPLATE_VECTOR_EXPRESSIONS_HPP

#include "jump/experimental/expression_templates/concepts.hpp"

#include <cassert>
#include <functional>

namespace jump {
template <typename Functor, VectorExpression Expr>
class UnaryVectorOp {
    public:
        static constexpr bool is_vector_expression_leaf{false};

        constexpr UnaryVectorOp(const Expr& expr);

        constexpr auto operator[](std::size_t i) const;
        constexpr auto size() const -> std::size_t;

    protected:
        typename std::conditional<Expr::is_vector_expression_leaf,
                 const Expr&, const Expr>::type m_expr;
        Functor m_operator{};
};

template <typename Functor, VectorExpression Left, VectorExpression Right>
class BinaryVectorOp {
    public:
        static constexpr bool is_vector_expression_leaf{false};

        constexpr BinaryVectorOp(const Left& lhs, const Right& rhs);

        constexpr auto operator[](std::size_t i) const;
        constexpr auto size() const -> std::size_t;

    protected:
        typename std::conditional<Left::is_vector_expression_leaf,
                 const Left&, const Left>::type m_lhs;
        typename std::conditional<Right::is_vector_expression_leaf,
                 const Right&, const Right>::type m_rhs;
        Functor m_operator{};
};

template <typename Functor, VectorExpression Expr>
inline constexpr UnaryVectorOp<Functor, Expr>::UnaryVectorOp(const Expr& expr) :
    m_expr{expr} {
}

template <typename Functor, VectorExpression Expr>
inline constexpr auto UnaryVectorOp<Functor, Expr>::operator[](
        std::size_t i) const {
    return m_operator(m_expr[i]);
}

template <typename Functor, VectorExpression Expr>
inline constexpr auto UnaryVectorOp<Functor, Expr>::size() const
        -> std::size_t {
    return m_expr.size();
}

template <typename Functor, VectorExpression Left, VectorExpression Right>
inline constexpr BinaryVectorOp<Functor, Left, Right>::BinaryVectorOp(
        const Left& lhs, const Right& rhs) :
    m_lhs{lhs}, m_rhs{rhs} {
    assert(m_lhs.size() == m_rhs.size());
}

template <typename Functor, VectorExpression Left, VectorExpression Right>
inline constexpr auto BinaryVectorOp<Functor, Left, Right>::operator[](
        std::size_t i) const {
    return m_operator(m_lhs[i], m_rhs[i]);
}

template <typename Functor, VectorExpression Left, VectorExpression Right>
inline constexpr auto BinaryVectorOp<Functor, Left, Right>::size() const
        -> std::size_t {
    return m_lhs.size();
}

template <VectorExpression Expr>
struct VectorNegate : UnaryVectorOp<std::negate<>, Expr> {
    using UnaryVectorOp<std::negate<>, Expr>::UnaryVectorOp;
};

template <VectorExpression Left, VectorExpression Right>
struct VectorAdd : BinaryVectorOp<std::plus<>, Left, Right> {
    using BinaryVectorOp<std::plus<>, Left, Right>::BinaryVectorOp;
};
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_VECTOR_EXPRESSIONS_HPP

