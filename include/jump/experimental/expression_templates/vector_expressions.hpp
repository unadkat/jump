// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_VECTOR_EXPRESSIONS_HPP
#define JUMP_EXPRESSION_TEMPLATE_VECTOR_EXPRESSIONS_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/experimental/expression_templates/concepts.hpp"

#include <type_traits>

namespace jump {
template <typename Functor, VectorExpression Expr>
class UnaryVectorOp {
    public:
        using ValueType = std::invoke_result_t<Functor,
              typename Expr::ValueType>;

        static constexpr bool is_vector_expression_leaf{false};

        constexpr UnaryVectorOp(const Expr& expr);

        constexpr auto operator[](std::size_t index) const -> ValueType;
        constexpr auto size() const -> std::size_t;

    protected:
        typename std::conditional<Expr::is_vector_expression_leaf,
                 const Expr&, const Expr>::type m_expr;
        Functor m_operator{};
};

template <typename Functor, VectorExpression Left, VectorExpression Right>
class BinaryVectorOp {
    public:
        using ValueType = std::invoke_result_t<Functor,
              typename Left::ValueType, typename Right::ValueType>;

        static constexpr bool is_vector_expression_leaf{false};

        constexpr BinaryVectorOp(const Left& lhs, const Right& rhs);

        constexpr auto operator[](std::size_t index) const -> ValueType;
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
        std::size_t index) const -> ValueType {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return m_operator(m_expr[index]);
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
#ifndef NDEBUG
    if (lhs.size() != rhs.size()) {
        throw RuntimeError{Mismatch1DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG
}

template <typename Functor, VectorExpression Left, VectorExpression Right>
inline constexpr auto BinaryVectorOp<Functor, Left, Right>::operator[](
        std::size_t index) const -> ValueType {
#ifndef NDEBUG
    if (index >= size()) {
        throw RuntimeError{Range1DError{.index = index, .size = size()}};
    }
#endif  // NDEBUG

    return m_operator(m_lhs[index], m_rhs[index]);
}

template <typename Functor, VectorExpression Left, VectorExpression Right>
inline constexpr auto BinaryVectorOp<Functor, Left, Right>::size() const
        -> std::size_t {
    return m_lhs.size();
}
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_VECTOR_EXPRESSIONS_HPP

