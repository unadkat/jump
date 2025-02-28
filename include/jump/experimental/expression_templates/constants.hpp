// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/


#ifndef JUMP_EXPRESSION_TEMPLATE_CONSTANTS_HPP
#define JUMP_EXPRESSION_TEMPLATE_CONSTANTS_HPP

#include <type_traits>
#include <utility>

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

template <typename T>
class ConstantMatrixExpression {
    public:
        using ValueType = std::remove_cvref_t<T>;
        using InnerExpressionType = ConstantVectorExpression<ValueType>;

        // Even though this is logically a "leaf" of the expression, we need to
        // store this by value in an expression so we can update the size, etc.,
        // proper leaves are stored by const reference which is insufficient
        static constexpr bool is_banded_matrix_expression_leaf{false};

        constexpr ConstantMatrixExpression(const T& value);

        constexpr auto as_vector() const
                -> const ConstantVectorExpression<ValueType>&;
        constexpr auto size() const
                -> const std::pair<std::size_t, std::size_t>&;
        constexpr auto num_elements() const -> std::size_t;
        constexpr auto num_bands() const -> std::size_t;

        constexpr void set_banded(
                const std::pair<std::size_t, std::size_t>& size,
                std::size_t num_bands);

    private:
        ConstantVectorExpression<ValueType> m_inner;
        std::pair<std::size_t, std::size_t> m_size{};
        std::size_t m_num_elements{};
        std::size_t m_num_bands{};
};

// ========================================================================
// Type traits
// ========================================================================

template <typename>
struct IsConstantVectorExpression : public std::false_type {
};

template <typename T>
struct IsConstantVectorExpression<ConstantVectorExpression<T>> :
        public std::true_type {
};

template <typename T>
inline constexpr bool is_constant_vector_expression_v
        = IsConstantVectorExpression<T>::value;

template <typename>
struct IsConstantMatrixExpression : public std::false_type {
};

template <typename T>
struct IsConstantMatrixExpression<ConstantMatrixExpression<T>> :
        public std::true_type {
};

template <typename T>
inline constexpr bool is_constant_matrix_expression_v
        = IsConstantMatrixExpression<T>::value;

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

template <typename T>
constexpr ConstantMatrixExpression<T>::ConstantMatrixExpression(
        const T& value) :
    m_inner{value} {
}

template <typename T>
inline constexpr auto ConstantMatrixExpression<T>::as_vector() const
        -> const ConstantVectorExpression<ValueType>& {
    return m_inner;
}

template <typename T>
inline constexpr auto ConstantMatrixExpression<T>::size() const
        -> const std::pair<std::size_t, std::size_t>& {
    return m_size;
}

template <typename T>
inline constexpr auto ConstantMatrixExpression<T>::num_elements() const
        -> std::size_t {
    return m_num_elements;
}

template <typename T>
inline constexpr auto ConstantMatrixExpression<T>::num_bands() const
        -> std::size_t {
    return m_num_bands;
}

template <typename T>
inline constexpr void ConstantMatrixExpression<T>::set_banded(
        const std::pair<std::size_t, std::size_t>& size,
        std::size_t num_bands) {
    m_size = size;
    m_num_bands = num_bands;
    m_num_elements = size.second*(3*num_bands + 1);
    m_inner.set_size(m_num_elements);
}
}   // namespace jump

#endif  // JUMP_EXPRESSION_TEMPLATES_CONSTANTS_HPP

