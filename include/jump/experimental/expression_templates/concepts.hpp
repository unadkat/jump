// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_CONCEPTS_HPP
#define JUMP_EXPRESSION_TEMPLATE_CONCEPTS_HPP

#include <concepts>

namespace jump {
template <typename Expr>
concept VectorExpression = requires (Expr expr, std::size_t i) {
    {expr.is_vector_expression_leaf} -> std::same_as<const bool&>;
    {expr[i]};
    {expr.size()} -> std::same_as<std::size_t>;
};
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATE_CONCEPTS_HPP

