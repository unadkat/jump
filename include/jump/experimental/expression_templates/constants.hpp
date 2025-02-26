// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/


#ifndef JUMP_EXPRESSION_TEMPLATE_CONSTANTS_HPP
#define JUMP_EXPRESSION_TEMPLATE_CONSTANTS_HPP

#include <type_traits>

namespace jump {
template <typename T>
class ConstantVectorExpression {
    public:
        using ValueType = std::remove_cvref_t<T>;

        // Even though this is logically a "leaf" of the expression, we need to
        // store this by value in an expression so we can update the size,
        // proper leaves are stored by const reference which is insufficient
        static constexpr bool is_vector_expression_leaf{false};

        constexpr ConstantVectorExpression(const T& value);

        constexpr auto operator[](std::size_t index) const -> ValueType;
        constexpr auto size() const -> std::size_t;
        constexpr void set_size(std::size_t N);

    private:
        T m_value{};
        std::size_t m_size{};
};

// ========================================================================
// Type traits
// ========================================================================

template <typename>
struct IsConstantVectorExpression : public std::false_type {
};

template <typename T>
struct IsConstantVectorExpression<ConstantVectorExpression<T>>
        : public std::true_type {
};

template <typename T>
inline constexpr bool is_constant_vector_expression_v
        = IsConstantVectorExpression<T>::value;

// ========================================================================
// Implementation
// ========================================================================

template <typename T>
inline constexpr ConstantVectorExpression<T>::ConstantVectorExpression(
        const T& value) :
    m_value{value} {
}

template <typename T>
inline constexpr auto ConstantVectorExpression<T>::operator[](
        [[maybe_unused]] std::size_t index) const -> ValueType {
    return m_value;
}

template <typename T>
inline constexpr auto ConstantVectorExpression<T>::size() const -> std::size_t {
    return m_size;
}

template <typename T>
inline constexpr void ConstantVectorExpression<T>::set_size(std::size_t N) {
    m_size = N;
}
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATES_CONSTANTS_HPP

