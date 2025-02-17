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

#include <utility>

namespace jump {
template <typename Functor, BandedMatrixExpression Expr>
class UnaryBandedMatrixOp {
    public:
        using ValueType = std::invoke_result_t<Functor,
              typename Expr::ValueType>;

        static constexpr bool is_banded_matrix_expression_leaf{false};

        constexpr UnaryBandedMatrixOp(const Expr& expr);

        constexpr auto as_vector() const;
        constexpr auto size() const -> std::pair<std::size_t, std::size_t>;
        constexpr auto num_elements() const -> std::size_t;
        constexpr auto num_bands() const -> std::size_t;

    protected:
        typename std::conditional<Expr::is_banded_matrix_expression_leaf,
                 const Expr&, Expr>::type m_expr;
        Functor m_operator{};
};

// ========================================================================
// Implementation
// ========================================================================

template <typename Functor, BandedMatrixExpression Expr>
inline constexpr UnaryBandedMatrixOp<Functor, Expr>::UnaryBandedMatrixOp(
        const Expr& expr) :
    m_expr{expr} {
}

template <typename Functor, BandedMatrixExpression Expr>
inline constexpr auto UnaryBandedMatrixOp<Functor, Expr>::as_vector() const {
    return m_expr(m_expr.as_vector());
}

template <typename Functor, BandedMatrixExpression Expr>
inline constexpr auto UnaryBandedMatrixOp<Functor, Expr>::size() const
        -> std::pair<std::size_t, std::size_t> {
    return m_expr.size();
}

template <typename Functor, BandedMatrixExpression Expr>
inline constexpr auto UnaryBandedMatrixOp<Functor, Expr>::num_elements() const
        -> std::size_t {
    return m_expr.num_elements();
}

template <typename Functor, BandedMatrixExpression Expr>
inline constexpr auto UnaryBandedMatrixOp<Functor, Expr>::num_bands() const
        -> std::size_t {
    return m_expr.num_bands();
}
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_BANDED_MATRIX_EXPRESSIONS_HPP

