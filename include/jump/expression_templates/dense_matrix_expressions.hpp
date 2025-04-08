// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_DENSE_MATRIX_EXPRESSIONS_HPP
#define JUMP_EXPRESSION_TEMPLATE_DENSE_MATRIX_EXPRESSIONS_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/expression_templates/concepts.hpp"
#include "jump/expression_templates/constants.hpp"

#include <format>
#include <utility>

namespace jump {
template <template <typename> typename Functor, DenseMatrixExpression Expr>
class UnaryDenseMatrixOp {
    public:
        using InnerExpressionType = Functor<typename Expr::InnerExpressionType>;

        static constexpr bool is_dense_matrix_expression_leaf{false};

        constexpr UnaryDenseMatrixOp(const Expr& expr);

        constexpr auto as_vector() const -> InnerExpressionType;
        constexpr auto size() const
                -> const std::pair<std::size_t, std::size_t>&;
        constexpr auto num_elements() const -> std::size_t;

    protected:
        typename std::conditional<Expr::is_dense_matrix_expression_leaf,
                 const Expr&, Expr>::type m_expr;
};

template <template <typename, typename> typename Functor,
    DenseMatrixExpression Left, DenseMatrixExpression Right>
class BinaryDenseMatrixOp {
    public:
        using InnerExpressionType = Functor<typename Left::InnerExpressionType,
              typename Right::InnerExpressionType>;

        static constexpr bool is_dense_matrix_expression_leaf{false};

        constexpr BinaryDenseMatrixOp(const Left& lhs, const Right& rhs);

        constexpr auto as_vector() const -> InnerExpressionType;
        constexpr auto size() const
                -> const std::pair<std::size_t, std::size_t>&;
        constexpr auto num_elements() const -> std::size_t;

    protected:
        typename std::conditional<Left::is_dense_matrix_expression_leaf,
                 const Left&, Left>::type m_lhs;
        typename std::conditional<Right::is_dense_matrix_expression_leaf,
                 const Right&, Right>::type m_rhs;
};

// ========================================================================
// Implementation
// ========================================================================

template <template <typename> typename Functor, DenseMatrixExpression Expr>
inline constexpr UnaryDenseMatrixOp<Functor, Expr>::UnaryDenseMatrixOp(
        const Expr& expr) :
    m_expr{expr} {
}

template <template <typename> typename Functor, DenseMatrixExpression Expr>
inline constexpr auto UnaryDenseMatrixOp<Functor, Expr>::as_vector() const
        -> InnerExpressionType {
    return {m_expr.as_vector()};
}

template <template <typename> typename Functor, DenseMatrixExpression Expr>
inline constexpr auto UnaryDenseMatrixOp<Functor, Expr>::size() const
        -> const std::pair<std::size_t, std::size_t>& {
    return m_expr.size();
}

template <template <typename> typename Functor, DenseMatrixExpression Expr>
inline constexpr auto UnaryDenseMatrixOp<Functor, Expr>::num_elements() const
        -> std::size_t {
    return m_expr.num_elements();
}

template <template <typename, typename> typename Functor,
    DenseMatrixExpression Left, DenseMatrixExpression Right>
inline constexpr BinaryDenseMatrixOp<Functor, Left, Right>::BinaryDenseMatrixOp(
        const Left& lhs, const Right& rhs) :
    m_lhs{lhs},
    m_rhs{rhs} {
    if constexpr (is_constant_matrix_expression_v<Left>
            && is_constant_matrix_expression_v<Right>) {
        throw RuntimeError{InvalidArgumentError{
            .argument = "is_constant_matrix_expression_v<Left> "
                "&& is_constant_matrix_expression_v<Right>",
            .value = "true",
            .expected = "only one of the arguments being constant"}};
    } else if constexpr (is_constant_matrix_expression_v<Left>) {
        m_lhs.set_dense(rhs.size());
    } else if constexpr (is_constant_matrix_expression_v<Right>) {
        m_rhs.set_dense(lhs.size());
    }
#ifndef NDEBUG
    if (m_lhs.size() != m_rhs.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
#endif  // NDEBUG
}

template <template <typename, typename> typename Functor,
    DenseMatrixExpression Left, DenseMatrixExpression Right>
inline constexpr auto BinaryDenseMatrixOp<Functor, Left, Right>::
    as_vector() const -> InnerExpressionType {
    return {m_lhs.as_vector(), m_rhs.as_vector()};
}

template <template <typename, typename> typename Functor,
    DenseMatrixExpression Left, DenseMatrixExpression Right>
inline constexpr auto BinaryDenseMatrixOp<Functor, Left, Right>::
    size() const -> const std::pair<std::size_t, std::size_t>& {
    return m_lhs.size();
}

template <template <typename, typename> typename Functor,
    DenseMatrixExpression Left, DenseMatrixExpression Right>
inline constexpr auto BinaryDenseMatrixOp<Functor, Left, Right>::
    num_elements() const -> std::size_t {
    return m_lhs.num_elements();
}
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_DENSE_MATRIX_EXPRESSIONS_HPP

