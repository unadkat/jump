// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_UTILITY_DECL_HPP
#define JUMP_UTILITY_DECL_HPP

#include <concepts>
#include <limits>
#include <vector>

namespace jump {
/// \brief Tests two vectors (assumed sorted) to see if they have an
/// intersection.
template <typename T>
bool has_intersection(const std::vector<T>& A, const std::vector<T>& B);

/// \brief Signum function.
template <typename T>
int sgn(T val);
}   // namespace jump

#endif  // JUMP_UTILITY_DECL_HPP

