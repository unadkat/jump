// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_EXPRESSIONS_HPP
#define JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_EXPRESSIONS_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"
#include "jump/experimental/expression_templates/concepts.hpp"
#include "jump/experimental/expression_templates/constants.hpp"

#include <format>
#include <utility>

namespace jump {
template <template <typename> typename Functor, BandedMatrixExpression Expr>
class UnaryBandedMatrixOp {
    public:
        using InnerExpressionType = Functor<typename Expr::InnerExpressionType>;

        static constexpr bool is_banded_matrix_expression_leaf{false};

        constexpr UnaryBandedMatrixOp(const Expr& expr);

        constexpr auto as_vector() const -> InnerExpressionType;
        constexpr auto size() const
                -> const std::pair<std::size_t, std::size_t>&;
        constexpr auto num_elements() const -> std::size_t;
        constexpr auto num_bands() const -> std::size_t;

    protected:
        typename std::conditional<Expr::is_banded_matrix_expression_leaf,
                 const Expr&, Expr>::type m_expr;
};

template <template <typename, typename> typename Functor,
         BandedMatrixExpression Left, BandedMatrixExpression Right>
class BinaryBandedMatrixOp {
    public:
        using InnerExpressionType = Functor<typename Left::InnerExpressionType,
              typename Right::InnerExpressionType>;

        static constexpr bool is_banded_matrix_expression_leaf{false};

        constexpr BinaryBandedMatrixOp(const Left& lhs, const Right& rhs);

        constexpr auto as_vector() const -> InnerExpressionType;
        constexpr auto size() const
                -> const std::pair<std::size_t, std::size_t>&;
        constexpr auto num_elements() const -> std::size_t;
        constexpr auto num_bands() const -> std::size_t;

    protected:
        typename std::conditional<Left::is_banded_matrix_expression_leaf,
                 const Left&, Left>::type m_lhs;
        typename std::conditional<Right::is_banded_matrix_expression_leaf,
                 const Right&, Right>::type m_rhs;
};

// ========================================================================
// Implementation
// ========================================================================

template <template <typename> typename Functor, BandedMatrixExpression Expr>
inline constexpr UnaryBandedMatrixOp<Functor, Expr>::UnaryBandedMatrixOp(
        const Expr& expr) :
    m_expr{expr} {
}

template <template <typename> typename Functor, BandedMatrixExpression Expr>
inline constexpr auto UnaryBandedMatrixOp<Functor, Expr>::as_vector() const
        -> InnerExpressionType {
    return InnerExpressionType{m_expr.as_vector()};
}

template <template <typename> typename Functor, BandedMatrixExpression Expr>
inline constexpr auto UnaryBandedMatrixOp<Functor, Expr>::size() const
        -> const std::pair<std::size_t, std::size_t>& {
    return m_expr.size();
}

template <template <typename> typename Functor, BandedMatrixExpression Expr>
inline constexpr auto UnaryBandedMatrixOp<Functor, Expr>::num_elements() const
        -> std::size_t {
    return m_expr.num_elements();
}

template <template <typename> typename Functor, BandedMatrixExpression Expr>
inline constexpr auto UnaryBandedMatrixOp<Functor, Expr>::num_bands() const
        -> std::size_t {
    return m_expr.num_bands();
}

template <template <typename, typename> typename Functor,
    BandedMatrixExpression Left, BandedMatrixExpression Right>
inline constexpr BinaryBandedMatrixOp<Functor, Left, Right>::
    BinaryBandedMatrixOp(const Left& lhs, const Right& rhs) :
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
        m_lhs.set_banded(rhs.size(), rhs.num_bands());
    } else if constexpr (is_constant_matrix_expression_v<Right>) {
        m_rhs.set_banded(lhs.size(), lhs.num_bands());
    }
#ifndef NDEBUG
    if (m_lhs.size() != m_rhs.size()) {
        throw RuntimeError{Mismatch2DError{.name1 = "lhs", .size1 = lhs.size(),
            .name2 = "rhs", .size2 = rhs.size()}};
    }
    if (m_lhs.num_bands() != m_rhs.num_bands()) {
        throw RuntimeError{InvalidArgumentError{.argument = "rhs.num_bands()",
            .value = std::format("{}", rhs.num_bands()),
            .expected = std::format("{}", lhs.num_bands())}};
    }
#endif  // NDEBUG
}

template <template <typename, typename> typename Functor,
    BandedMatrixExpression Left, BandedMatrixExpression Right>
inline constexpr auto BinaryBandedMatrixOp<Functor, Left, Right>::
    as_vector() const -> InnerExpressionType {
    return InnerExpressionType{m_lhs.as_vector(), m_rhs.as_vector()};
}

template <template <typename, typename> typename Functor,
    BandedMatrixExpression Left, BandedMatrixExpression Right>
inline constexpr auto BinaryBandedMatrixOp<Functor, Left, Right>::
    size() const -> const std::pair<std::size_t, std::size_t>& {
    return m_lhs.size();
}

template <template <typename, typename> typename Functor,
    BandedMatrixExpression Left, BandedMatrixExpression Right>
inline constexpr auto BinaryBandedMatrixOp<Functor, Left, Right>::
    num_elements() const -> std::size_t {
    return m_lhs.num_elements();
}

template <template <typename, typename> typename Functor,
    BandedMatrixExpression Left, BandedMatrixExpression Right>
inline constexpr auto BinaryBandedMatrixOp<Functor, Left, Right>::
    num_bands() const -> std::size_t {
    return m_lhs.num_bands();
}
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_EXPRESSIONS_HPP

