// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_UTILITY_HPP
#define JUMP_UTILITY_HPP

#include <concepts>
#include <limits>
#include <vector>

namespace jump {
/// \brief Tests two vectors (assumed sorted) to see if they have an
/// intersection.
template <typename T>
auto has_intersection(const std::vector<T>& A, const std::vector<T>& B) -> bool;

/// \brief Signum function.
template <typename T>
auto sgn(T val) -> int;

// ========================================================================
// Implementation
// ========================================================================

template <typename T>
inline auto has_intersection(const std::vector<T>& A, const std::vector<T>& B)
        -> bool {
    std::size_t nA{A.size()}, b{0}, nB{B.size()};
    if (nA == 0 || nB == 0) {
        return false;
    }

    for (std::size_t a{0}; a < nA; ++a) {
        // Skip forwards in B until we have a candidate intersection (current
        // item in A is <= current item in B
        while (b < nB && A[a] > B[b]) {
            ++b;
        }
        // Reached end of B with no match or found a match
        if (b == nB) {
            return false;
        } else if (A[a] == B[b]) {
            return true;
        }
        // Current item is no match, move to next item in A
    }
    return false;
}

template <typename T>
inline auto sgn(T val) -> int {
    if constexpr (std::totally_ordered<T>) {
        return (T{0} < val) - (val < T{0});
    } else {
        return std::numeric_limits<T>::quiet_NaN();
    }
}
}   // namespace jump

#endif  // JUMP_UTILITY_HPP

