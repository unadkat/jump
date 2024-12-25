// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_MATRIX_BASE_HPP
#define JUMP_MATRIX_BASE_HPP

#include "jump/data/matrix_base_decl.hpp"

namespace jump {
template <typename T>
inline MatrixBase<T>::MatrixBase(std::size_t size) :
    m_num_rows{size},
    m_num_columns{size} {
}

template <typename T>
inline MatrixBase<T>::MatrixBase(std::size_t num_rows,
        std::size_t num_columns) :
    m_num_rows{num_rows},
    m_num_columns{num_columns} {
}

template <typename T>
inline auto MatrixBase<T>::num_rows() const -> std::size_t {
    return m_num_rows;
}

template <typename T>
inline auto MatrixBase<T>::num_columns() const -> std::size_t {
    return m_num_columns;
}

template <typename T>
inline auto MatrixBase<T>::size() const -> std::pair<std::size_t, std::size_t> {
    return {m_num_rows, m_num_columns};
}

template <typename T>
inline void MatrixBase<T>::initialise(std::size_t size) {
    m_num_rows = m_num_columns = size;
}

template <typename T>
inline void MatrixBase<T>::initialise(std::size_t num_rows,
        std::size_t num_columns) {
    m_num_rows = num_rows;
    m_num_columns = num_columns;
}

/// \relates MatrixBase
template <typename T, typename Os>
inline auto operator<<(Os& out, const MatrixBase<T>& rhs) -> Os& {
    return out << rhs.as_string();
}
}   // namespace jump

#endif  // JUMP_MATRIX_BASE_HPP

