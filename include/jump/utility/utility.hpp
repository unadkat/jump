// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_UTILITY_HPP
#define JUMP_UTILITY_HPP

#include "jump/utility/utility_decl.hpp"

#include <iostream>

namespace jump {
template <typename T>
bool has_intersection(const std::vector<T>& A, const std::vector<T>& B) {
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
}   // namespace jump

#endif  // JUMP_UTILITY_HPP

